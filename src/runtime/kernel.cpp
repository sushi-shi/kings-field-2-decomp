#include "runtime.h"

#include <kf/platform/host.h>
#include <kf/psx/sdk.h>

#include <algorithm>
#include <array>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <utility>
#include <vector>

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

// Kernel, BIOS and C-library services for the original programs, plus the
// program loader that replaces overlay loading in a single host executable.

extern "C" {
void kf_psx_main(void);
void kf_open_main(void);
void kf_game_main(void);
void kf_end_main(void);
}

// Each program's mutable globals are placed in its own sections by the
// generated program wrappers; the linker provides these bounds.
#define KF_PROGRAM_SECTIONS(name)                                         \
    extern "C" __attribute__((weak)) char __start_kf_##name##_data[];   \
    extern "C" __attribute__((weak)) char __stop_kf_##name##_data[];    \
    extern "C" __attribute__((weak)) char __start_kf_##name##_bss[];    \
    extern "C" __attribute__((weak)) char __stop_kf_##name##_bss[];
KF_PROGRAM_SECTIONS(psx)
KF_PROGRAM_SECTIONS(open)
KF_PROGRAM_SECTIONS(game)
KF_PROGRAM_SECTIONS(end)

u_char kf_psx_overlay_request;

namespace kf::psx {
namespace {
struct ProgramImage {
    const char *disc_name;
    void (*main)();
    char *data_start, *data_stop, *bss_start, *bss_stop;
    std::vector<char> initial_data;
};

std::array<ProgramImage, static_cast<std::size_t>(Program::Count)> programs = {{
    {"PSX.EXE", kf_psx_main, __start_kf_psx_data, __stop_kf_psx_data, __start_kf_psx_bss, __stop_kf_psx_bss, {}},
    {"OPEN.EXE", kf_open_main, __start_kf_open_data, __stop_kf_open_data, __start_kf_open_bss, __stop_kf_open_bss, {}},
    {"GAME.EXE", kf_game_main, __start_kf_game_data, __stop_kf_game_data, __start_kf_game_bss, __stop_kf_game_bss, {}},
    {"END.EXE", kf_end_main, __start_kf_end_data, __stop_kf_end_data, __start_kf_end_bss, __stop_kf_end_bss, {}},
}};

// A first-fit heap over the region a program passes to InitHeap, so each
// program keeps its retail heap budget and loses its heap when replaced.
struct HeapBlock {
    std::size_t size; // payload bytes
    bool used;
};
constexpr std::size_t heap_alignment = 8;
constexpr std::size_t heap_header = 8;
std::vector<unsigned char> heap_storage;

std::size_t align_up(std::size_t value) { return (value + heap_alignment - 1) & ~(heap_alignment - 1); }
HeapBlock *heap_block(std::size_t offset) { return reinterpret_cast<HeapBlock *>(heap_storage.data() + offset); }

u32 random_state = 1;

struct Event {
    unsigned long descriptor;
    long spec, mode;
    void (*handler)();
    bool open, enabled, delivered;
};
std::array<Event, 32> events;
constexpr long event_id_base = 0xf1000000;

std::string saves_directory = "saves";

// The memory card is a directory: "bu00:NAME" is the file NAME in it.
constexpr long card_write = 0x0002;
constexpr long card_create = 0x0200;
constexpr std::size_t card_block_bytes = 8192;
struct CardFile {
    std::FILE *stream;
};
std::array<CardFile, 8> card_files;
std::vector<std::pair<std::string, long>> card_listing;
std::size_t card_listing_index;

bool card_path(const char *device_path, std::string &path) {
    if (std::strncmp(device_path, "bu00:", 5) != 0)
        return false;
    std::string name = device_path + 5;
    while (!name.empty() && name.back() == ' ')
        name.pop_back();
    if (name == "*") {
        path = saves_directory;
        return true;
    }
    if (name.empty() || name.find_first_of("/\\") != std::string::npos || name[0] == '.')
        return false;
    mkdir(saves_directory.c_str(), 0700);
    path = saves_directory + "/" + name;
    return true;
}
}

void programs_capture_initial_state() {
    for (auto &program : programs)
        if (program.data_start && program.data_stop)
            program.initial_data.assign(program.data_start, program.data_stop);
}

void programs_run_boot() {
    programs[static_cast<std::size_t>(Program::Psx)].main();
}

void set_saves_directory(const char *directory) { saves_directory = directory; }

namespace {
struct PendingInterrupt {
    unsigned long descriptor;
    long spec;
};
std::vector<PendingInterrupt> pending_interrupts;
bool in_critical_section;
bool dispatching;
constexpr long event_mode_interrupt = 0x1000;
constexpr std::size_t maximum_interrupts_per_dispatch = 1u << 16;
}

bool interrupt_enabled(unsigned long descriptor, long spec) {
    for (const auto &event : events)
        if (event.open && event.enabled && event.descriptor == descriptor && (event.spec & spec) != 0)
            return true;
    return false;
}

void interrupt_raise(unsigned long descriptor, long spec) {
    // A disabled event misses the interrupt, as on the console.
    if (interrupt_enabled(descriptor, spec))
        pending_interrupts.push_back({descriptor, spec});
}

void interrupts_dispatch() {
    if (in_critical_section || dispatching)
        return;
    dispatching = true;
    for (std::size_t count = 0; count < maximum_interrupts_per_dispatch; ++count) {
        if (pending_interrupts.empty() && !cd_service_stream())
            break;
        if (pending_interrupts.empty())
            continue;
        const auto interrupt = pending_interrupts.front();
        pending_interrupts.erase(pending_interrupts.begin());
        for (auto &event : events) {
            if (!event.open || !event.enabled || event.descriptor != interrupt.descriptor ||
                (event.spec & interrupt.spec) == 0)
                continue;
            if (event.mode == event_mode_interrupt && event.handler)
                event.handler();
            else
                event.delivered = true;
        }
    }
    dispatching = false;
}

void event_deliver(unsigned long descriptor, long spec) {
    interrupt_raise(descriptor, spec);
}

[[noreturn]] void unimplemented(const char *function) {
    std::string message = "Unimplemented PlayStation library call: ";
    message += function;
    host_fail(message.c_str());
}

void warn_once(const char *function) {
    static std::vector<const char *> reported;
    for (const auto *name : reported)
        if (name == function)
            return;
    reported.push_back(function);
    std::fprintf(stderr, "kf2: %s is not implemented yet; continuing.\n", function);
}
} // namespace kf::psx

using namespace kf::psx;

extern "C" {
void kf_psx_note_unported(const char *what) { warn_once(what); }

// ------------------------------------------------------------ program loading
long Load(const char *name, struct EXEC *header) {
    const char *base = std::strchr(name, ':');
    base = base ? base + 1 : name;
    for (std::size_t i = 0; i < programs.size(); ++i) {
        if (std::strncmp(base, programs[i].disc_name, std::strlen(programs[i].disc_name)) == 0) {
            std::memset(header, 0, sizeof *header);
            header->pc0 = static_cast<unsigned long>(i);
            return 1;
        }
    }
    return 0;
}

long Exec(struct EXEC *header, long, char **) {
    auto &program = programs.at(header->pc0);
    // Reloading an executable restores its initialized data and clears its BSS.
    if (!program.initial_data.empty())
        std::memcpy(program.data_start, program.initial_data.data(), program.initial_data.size());
    if (program.bss_start && program.bss_stop)
        std::memset(program.bss_start, 0, static_cast<std::size_t>(program.bss_stop - program.bss_start));
    std::printf("kf2: running %s\n", program.disc_name);
    std::fflush(stdout);
    program.main();
    return 1;
}

void SetMem(long) {}
long _96_init(void) { return 0; }
long _96_remove(void) { return 0; }
void EnterCriticalSection(void) { in_critical_section = true; }
void ExitCriticalSection(void) {
    in_critical_section = false;
    interrupts_dispatch();
}

// ------------------------------------------------------------------ events
long OpenEvent(unsigned long descriptor, long spec, long mode, void (*handler)(void)) {
    for (std::size_t i = 0; i < events.size(); ++i) {
        if (!events[i].open) {
            events[i] = {descriptor, spec, mode, handler, true, false, false};
            return event_id_base | static_cast<long>(i);
        }
    }
    return -1;
}

static Event *find_event(long id) {
    const auto index = static_cast<std::size_t>(id & 0xffff);
    if ((id & ~0xffffL) != event_id_base || index >= events.size() || !events[index].open)
        return nullptr;
    return &events[index];
}

long EnableEvent(long id) {
    if (auto *event = find_event(id)) {
        event->enabled = true;
        return 1;
    }
    return 0;
}

long DisableEvent(long id) {
    if (auto *event = find_event(id)) {
        event->enabled = false;
        return 1;
    }
    return 0;
}

long TestEvent(long id) {
    // Polling an event is a safe point for pending deliveries.
    interrupts_dispatch();
    if (auto *event = find_event(id)) {
        if (event->delivered) {
            event->delivered = false;
            return 1;
        }
    }
    return 0;
}

long UnDeliverEvent(long id) {
    if (auto *event = find_event(id))
        event->delivered = false;
    return 1;
}

long CloseEvent(long id) {
    if (auto *event = find_event(id)) {
        *event = {};
        return 1;
    }
    return 0;
}

// -------------------------------------------------------------------- heap
void InitHeap(void *, long size) {
    const std::size_t bytes = size > 0 ? static_cast<std::size_t>(size) : 0;
    heap_storage.assign(align_up(bytes) + heap_header, 0);
    *heap_block(0) = {heap_storage.size() - heap_header, false};
}

void *kf_psx_malloc(unsigned long size) {
    const std::size_t want = align_up(size ? size : 1);
    for (std::size_t offset = 0; offset + heap_header <= heap_storage.size();) {
        auto *block = heap_block(offset);
        if (!block->used && block->size >= want) {
            if (block->size >= want + heap_header + heap_alignment) {
                *heap_block(offset + heap_header + want) = {block->size - want - heap_header, false};
                block->size = want;
            }
            block->used = true;
            return heap_storage.data() + offset + heap_header;
        }
        offset += heap_header + block->size;
    }
    return nullptr;
}

void kf_psx_free(void *allocation) {
    if (!allocation || heap_storage.empty())
        return;
    auto *bytes = static_cast<unsigned char *>(allocation);
    if (bytes < heap_storage.data() + heap_header || bytes >= heap_storage.data() + heap_storage.size())
        return;
    reinterpret_cast<HeapBlock *>(bytes - heap_header)->used = false;
    // Coalesce adjacent free blocks.
    for (std::size_t offset = 0; offset + heap_header <= heap_storage.size();) {
        auto *block = heap_block(offset);
        const std::size_t next = offset + heap_header + block->size;
        if (!block->used && next + heap_header <= heap_storage.size() && !heap_block(next)->used) {
            block->size += heap_header + heap_block(next)->size;
            continue;
        }
        offset = next;
    }
}

// --------------------------------------------------------------- C library
void *kf_psx_memcpy(void *destination, const void *source, unsigned long size) {
    return std::memmove(destination, source, size);
}
void *kf_psx_memset(void *destination, int value, unsigned long size) {
    return std::memset(destination, value, size);
}
int kf_psx_rand(void) {
    random_state = random_state * 1103515245u + 12345u;
    return static_cast<int>((random_state >> 16) & 0x7fff);
}
void kf_psx_srand(unsigned int seed) { random_state = seed; }
int kf_psx_printf(const char *format, ...) {
    va_list arguments;
    va_start(arguments, format);
    const int result = std::vprintf(format, arguments);
    va_end(arguments);
    return result;
}
void kf_psx_exit(int status) { std::exit(status); }
char *kf_psx_strcpy(char *destination, const char *source) { return std::strcpy(destination, source); }
char *kf_psx_strcat(char *destination, const char *source) { return std::strcat(destination, source); }
int kf_psx_strncmp(const char *left, const char *right, unsigned long count) {
    return std::strncmp(left, right, count);
}
int kf_psx_abs(int value) { return value < 0 ? -value : value; }
int kf_psx_atoi(const char *string) { return std::atoi(string); }

// ----------------------------------------------- memory card (BIOS devices)
// The card is a directory of save files; device names select no hardware.
void InitCARD(long) {}
long StartCARD(void) { return 1; }
long StopCARD(void) { return 1; }
void ChangeClearPAD(long) {}
void _bu_init(void) {}
long _card_info(long) {
    event_deliver(0xf4000001, 0x0004);
    return 1;
}

long kf_psx_delete(const char *name);
struct DIRENTRY *kf_psx_nextfile(struct DIRENTRY *entry);

long kf_psx_open(const char *name, long mode) {
    std::string path;
    if (!card_path(name, path))
        return -1;
    for (std::size_t handle = 0; handle < card_files.size(); ++handle) {
        auto &file = card_files[handle];
        if (file.stream)
            continue;
        if (mode & card_create) {
            // The block count in the high half sizes the new file.
            const long blocks = std::max(1L, (mode >> 16) & 0xffff);
            if (std::FILE *existing = std::fopen(path.c_str(), "rb")) {
                std::fclose(existing);
                return -1;
            }
            file.stream = std::fopen(path.c_str(), "w+b");
            if (!file.stream)
                return -1;
            const std::vector<char> zero(static_cast<std::size_t>(blocks) * card_block_bytes, 0);
            std::fwrite(zero.data(), 1, zero.size(), file.stream);
            std::fflush(file.stream);
            std::rewind(file.stream);
        } else {
            file.stream = std::fopen(path.c_str(), (mode & card_write) ? "r+b" : "rb");
            if (!file.stream)
                return -1;
        }
        return static_cast<long>(handle);
    }
    return -1;
}

long kf_psx_close(long handle) {
    if (handle < 0 || static_cast<std::size_t>(handle) >= card_files.size() || !card_files[handle].stream)
        return -1;
    std::fflush(card_files[handle].stream);
    std::fclose(card_files[handle].stream);
    card_files[handle].stream = nullptr;
    return handle;
}

long kf_psx_lseek(long handle, long offset, long origin) {
    if (handle < 0 || static_cast<std::size_t>(handle) >= card_files.size() || !card_files[handle].stream)
        return -1;
    if (std::fseek(card_files[handle].stream, offset, origin == 1 ? SEEK_CUR : origin == 2 ? SEEK_END : SEEK_SET))
        return -1;
    return std::ftell(card_files[handle].stream);
}

long kf_psx_read(long handle, void *buffer, long length) {
    if (handle < 0 || static_cast<std::size_t>(handle) >= card_files.size() || !card_files[handle].stream ||
        length < 0)
        return -1;
    return static_cast<long>(std::fread(buffer, 1, static_cast<std::size_t>(length), card_files[handle].stream));
}

long kf_psx_write(long handle, const void *buffer, long length) {
    if (handle < 0 || static_cast<std::size_t>(handle) >= card_files.size() || !card_files[handle].stream ||
        length < 0)
        return -1;
    const auto written = std::fwrite(buffer, 1, static_cast<std::size_t>(length), card_files[handle].stream);
    std::fflush(card_files[handle].stream);
    return static_cast<long>(written);
}

long kf_psx_erase(const char *name) { return kf_psx_delete(name); }

long kf_psx_delete(const char *name) {
    std::string path;
    return card_path(name, path) && std::remove(path.c_str()) == 0 ? 1 : 0;
}

long kf_psx_format(const char *) {
    // Formatting would erase every save in the directory; keep files intact.
    warn_once("memory card formatting (saves are kept)");
    return 1;
}

struct DIRENTRY *kf_psx_firstfile(const char *pattern, struct DIRENTRY *entry) {
    card_listing.clear();
    card_listing_index = 0;
    std::string directory_pattern;
    if (!card_path(pattern, directory_pattern))
        return nullptr;
    if (DIR *directory = opendir(saves_directory.c_str())) {
        while (const dirent *item = readdir(directory)) {
            const std::string name = item->d_name;
            if (name.size() >= sizeof entry->name || name.rfind("BISLPS", 0) != 0)
                continue;
            struct stat status {};
            if (stat((saves_directory + "/" + name).c_str(), &status) == 0 && S_ISREG(status.st_mode))
                card_listing.push_back({name, static_cast<long>(status.st_size)});
        }
        closedir(directory);
    }
    std::sort(card_listing.begin(), card_listing.end());
    return kf_psx_nextfile(entry);
}

struct DIRENTRY *kf_psx_nextfile(struct DIRENTRY *entry) {
    if (card_listing_index >= card_listing.size())
        return nullptr;
    const auto &item = card_listing[card_listing_index++];
    std::memset(entry, 0, sizeof *entry);
    std::strncpy(entry->name, item.first.c_str(), sizeof entry->name - 1);
    entry->size = item.second;
    entry->attr = 0x51;
    return entry;
}
}
