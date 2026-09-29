#ifndef KF_OVERLAY_H
#define KF_OVERLAY_H
#include <kf/lib/types.h>
#include <kf/lib/memory_layout.h>

/* Byte at 0x800102f0 through which an overlay names the next program for the
 * PSX.EXE loader (0 = OPEN, 1 = GAME, 2 = END). Each program reaches it
 * through its own initialized pointer; how the original spelled the address
 * is unresolved. */
enum {
    KF_OVERLAY_OPEN = 0,
    KF_OVERLAY_GAME = 1,
    KF_OVERLAY_END = 2
};

extern u8 *overlay_next_request;

/* Boundaries supplied by the program's link layout (config/link/overlay_bounds.asm). */
extern u8 BSS_START[];
extern u8 BSS_END[];

enum {
    OVERLAY_STACK_BYTES = 0x8000
};

#define OVERLAY_RAM_END (0x80000000u + KF_MAIN_RAM_BYTES)
#define OVERLAY_STACK_BOTTOM (OVERLAY_RAM_END - OVERLAY_STACK_BYTES)

#endif
