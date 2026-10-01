#ifndef KF_GAME_RENDER_MASK_H
#define KF_GAME_RENDER_MASK_H

#include <kf/lib/types.h>

/* The observed span is written by 0x8002c670 while it scans a 24-by-24
 * window over the two-layer map occupancy grid. */
typedef struct KfRenderMaskScanState {
    s32 first_layer_byte_offset;
    s32 second_layer_byte_offset;
    u8 first_layer_mask;
    u8 second_layer_mask;
    u8 unknown_0a[2];
    s32 map_x;
    s32 map_z;
    s32 window_x;
    s32 window_z;
    u8 *mask_cursor;
} KfRenderMaskScanState;

typedef char kf_render_mask_scan_state_size[
    sizeof(KfRenderMaskScanState) == 0x20 ? 1 : -1];
typedef char kf_render_mask_scan_cursor_offset[
    (u32)&((KfRenderMaskScanState *)0)->mask_cursor == 0x1c ? 1 : -1];

typedef struct KfCollisionMaskPoint {
    s32 x;
    s32 z;
} KfCollisionMaskPoint;

extern KfRenderMaskScanState render_mask_scan_state;

#endif
