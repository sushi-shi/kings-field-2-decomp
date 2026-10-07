#include "runtime.h"

#include <kf/platform/host.h>
#include <kf/psx/sdk.h>

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <thread>

// Vertical-blank timing and digital pads on the shared host clock.

namespace kf::psx {
namespace {
std::uint64_t last_vblank;
std::uint64_t vblank_count;

// KF_PAD_SCRIPT="TICK:MASK,..." holds hexadecimal pad masks from each listed
// 60 Hz host-clock tick, for repeatable diagnostic runs without a keyboard.
u_long scripted_pad() {
    static const char *script = std::getenv("KF_PAD_SCRIPT");
    if (!script)
        return 0;
    u_long mask = 0;
    const char *cursor = script;
    while (*cursor) {
        char *end = nullptr;
        const auto frame = std::strtoull(cursor, &end, 10);
        if (!end || *end != ':')
            break;
        const auto value = std::strtoul(end + 1, &end, 16);
        if (frame > kf::host_clock_tick())
            break;
        mask = value;
        cursor = *end == ',' ? end + 1 : end;
        if (!*end)
            break;
    }
    return mask;
}
unsigned pad_reads_since_vblank;
constexpr unsigned busy_pad_read_threshold = 256;
}
void note_vblank_wait() { pad_reads_since_vblank = 0; }
}

using namespace kf::psx;

extern "C" {
int VSync(int mode) {
    if (mode < 0)
        return static_cast<int>(kf::host_clock_tick());
    if (mode == 1)
        return 0;
    const unsigned frames = mode == 0 ? 1u : static_cast<unsigned>(mode);
    gpu_present();
    sound_update();
    // Wait for the requested number of vertical blanks after the previous wait.
    const auto now = kf::host_clock_tick();
    auto target = last_vblank + frames;
    if (target <= now || target > now + frames)
        target = now + 1;
    kf::host_wait_until_tick(target);
    last_vblank = target;
    ++vblank_count;
    interrupt_raise(event_class_vsync_counter, event_spec_interrupt);
    interrupts_dispatch();
    note_vblank_wait();
    return static_cast<int>(target);
}

void ResetCallback(void) {}

u_long PadInit(long) { return 1; }

u_long PadRead(long) {
    // Some original waits spin on the pad without a vertical-blank wait.
    if (++pad_reads_since_vblank > busy_pad_read_threshold)
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    const u_long buttons = kf::host_read_pad() | scripted_pad();
    static const bool trace = std::getenv("KF_TRACE") != nullptr;
    static u_long previous;
    if (trace && buttons != previous)
        std::fprintf(stderr, "kf2: pad %04lx\n", buttons);
    previous = buttons;
    return buttons;
}

void PadStop(void) {}
}
