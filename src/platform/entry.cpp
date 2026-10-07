#include <kf/platform/host.h>

#include "../runtime/runtime.h"

#include <SDL3/SDL.h>

#include <csignal>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#if defined(__linux__) && !defined(__EMSCRIPTEN__)
#include <execinfo.h>
#include <ucontext.h>
#include <unistd.h>

// KF_WATCHDOG=SECONDS prints the main thread's stack and exits when it expires;
// a diagnostic for stalls in unconverted library paths.
static void watchdog_expired(int signal) {
    void *frames[64];
    const int count = backtrace(frames, 64);
    const char message[] = "kf2: watchdog expired or fatal signal\n";
    (void)signal;
    (void)!write(2, message, sizeof message - 1);
    backtrace_symbols_fd(frames, count, 2);
    _exit(3);
}

// Reports the faulting instruction and the return address at the stack top; a
// jump through a bad function pointer leaves no unwindable frame.
static void fault_report(int, siginfo_t *info, void *context) {
    char line[160];
#if defined(__i386__)
    const auto *registers = static_cast<ucontext_t *>(context)->uc_mcontext.gregs;
    const auto *stack = reinterpret_cast<const unsigned *>(registers[REG_ESP]);
    const int length = std::snprintf(line, sizeof line, "kf2: fault at %p eip=%08x stack top=%08x %08x\n",
                                     info->si_addr, static_cast<unsigned>(registers[REG_EIP]), stack[0], stack[1]);
#else
    (void)context;
    const int length = std::snprintf(line, sizeof line, "kf2: fault at %p\n", info->si_addr);
#endif
    (void)!write(2, line, static_cast<std::size_t>(length));
    watchdog_expired(SIGSEGV);
}

static void start_watchdog() {
    static char alternate_stack[1 << 16];
    stack_t stack{};
    stack.ss_sp = alternate_stack;
    stack.ss_size = sizeof alternate_stack;
    sigaltstack(&stack, nullptr);
    struct sigaction action {};
    action.sa_sigaction = fault_report;
    action.sa_flags = SA_ONSTACK | SA_SIGINFO;
    sigaction(SIGSEGV, &action, nullptr);
    std::signal(SIGBUS, watchdog_expired);
    std::signal(SIGILL, watchdog_expired);
    std::signal(SIGFPE, watchdog_expired);
    if (const char *seconds = std::getenv("KF_WATCHDOG")) {
        std::signal(SIGALRM, watchdog_expired);
        alarm(static_cast<unsigned>(std::atoi(seconds)));
    }
}
#else
static void start_watchdog() {}
#endif

namespace kf::psx {
void set_saves_directory(const char *directory);
}

int main(int argc, char **argv) {
    const char *disc = std::getenv("KF_DISC");
    const char *saves = nullptr;
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--disc") == 0 && i + 1 < argc)
            disc = argv[++i];
        else if (std::strcmp(argv[i], "--saves") == 0 && i + 1 < argc)
            saves = argv[++i];
        else {
            const bool help = std::strcmp(argv[i], "--help") == 0 || std::strcmp(argv[i], "-h") == 0;
            std::fprintf(help ? stdout : stderr,
                         "Usage: kings-field-2 [--disc IMAGE.cue|IMAGE.bin|IMAGE.iso] [--saves DIRECTORY]\n"
                         "The disc defaults to KF_DISC.\n");
            return help ? 0 : 1;
        }
    }
    if (!disc) {
        std::fprintf(stderr, "Supply your King's Field II (SLPS-00069) disc with --disc or KF_DISC.\n");
        return 1;
    }
    if (!kf::psx::disc_open(disc))
        return 1;
    if (!saves) {
        // The default card directory follows the platform's per-user data location.
        if (char *preference = SDL_GetPrefPath("KingsField2", "SLPS00069")) {
            static std::string directory;
            directory = std::string(preference) + "card";
            SDL_free(preference);
            saves = directory.c_str();
        } else {
            saves = "saves";
        }
    }
    kf::psx::set_saves_directory(saves);
    std::printf("kf2: memory card directory %s\n", saves);
    if (!kf::host_start("King's Field II"))
        return 1;
    start_watchdog();
    kf::psx::programs_capture_initial_state();
    kf::psx::programs_run_boot();
    kf::host_shutdown();
    return 0;
}
