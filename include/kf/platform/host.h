#ifndef KF_PLATFORM_HOST_H
#define KF_PLATFORM_HOST_H

#include <kf/platform/types.h>

namespace kf {
struct Renderer;

bool host_start(const char *title);
void host_shutdown();
// Services window, input and audio events. Never re-enters game code.
void host_poll();
std::uint64_t host_clock_ns();
// 60 Hz ticks of the shared host clock; focus pauses are excluded.
std::uint64_t host_clock_tick();
void host_wait_until_tick(std::uint64_t deadline);
void host_wait_frame();
// Held PlayStation digital pad buttons, active-high, in the documented bit layout.
u32 host_read_pad();
Renderer *host_renderer();
// Shows the renderer's current display area and swaps the window.
void host_present();
[[noreturn]] void host_fail(const char *message);
}

#endif // KF_PLATFORM_HOST_H
