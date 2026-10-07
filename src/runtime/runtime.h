#ifndef KF_RUNTIME_RUNTIME_H
#define KF_RUNTIME_RUNTIME_H

// Internal interfaces of the transitional PlayStation library runtime.

#include <kf/platform/types.h>

#include <cstddef>

namespace kf::psx {
enum class Program : u8 { Psx, Open, Game, End, Count };

// Captures each program's initialized data before any program runs.
void programs_capture_initial_state();
// Runs PSX.EXE's main; it loads and executes the other programs through Exec.
void programs_run_boot();

bool disc_open(const char *path);
bool disc_read_sector(u32 lba, u8 *destination);
// Looks up an ISO9660 path such as "\\OP\\OP.D;1" (separators may be / or \).
bool disc_find(const char *path, u32 *lba, u32 *size);

// Hardware-style events: raising queues a delivery for each open, enabled
// matching event; delivery happens at safe points (vertical blank and when
// leaving a critical section), never in the middle of unrelated game code.
inline constexpr unsigned long event_class_cdrom = 0xf0000003;
inline constexpr unsigned long event_class_vsync_counter = 0xf2000003;
inline constexpr unsigned long event_class_card = 0xf4000001;
inline constexpr long event_spec_interrupt = 0x0002;
inline constexpr long event_spec_cd_complete = 0x0020;
inline constexpr long event_spec_cd_data_ready = 0x0040;
void interrupt_raise(unsigned long descriptor, long spec);
void interrupts_dispatch();
bool interrupt_enabled(unsigned long descriptor, long spec);
// Continues an interrupt-driven sector stream; returns false when idle.
bool cd_service_stream();

// Called from VSync: presents the display area and advances the frame clock.
void gpu_present();
void sound_update();

[[noreturn]] void unimplemented(const char *function);
void warn_once(const char *function);
}

#endif // KF_RUNTIME_RUNTIME_H
