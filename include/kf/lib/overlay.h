#ifndef KF_OVERLAY_H
#define KF_OVERLAY_H
#include <kf/lib/types.h>
#include <kf/lib/memory_layout.h>

enum {
    KF_OVERLAY_OPEN = 0,
    KF_OVERLAY_GAME = 1,
    KF_OVERLAY_END = 2
};

extern u8 *overlay_next_request;

extern u8 BSS_END[];

enum {
    OVERLAY_STACK_BYTES = 0x8000
};

#define OVERLAY_RAM_END (0x80000000u + KF_MAIN_RAM_BYTES)
#define OVERLAY_STACK_BOTTOM (OVERLAY_RAM_END - OVERLAY_STACK_BYTES)

#endif
