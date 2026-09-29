#ifndef KF_OVERLAY_H
#define KF_OVERLAY_H
#include <kf/lib/types.h>

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

#endif
