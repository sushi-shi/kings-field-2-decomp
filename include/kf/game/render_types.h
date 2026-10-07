#ifndef KF_GAME_RENDER_TYPES_H
#define KF_GAME_RENDER_TYPES_H

#include <kf/lib/types.h>
#include <kf/lib/enum.h>

/* Selectors shared by actor, effect, map-object and HUD model records and the
 * render_world_model queue. */

/* render_world_model's queue selector. 0xff and 0xfe pick the textured and
 * clipping enqueues; 0x80 is textured without the below-view depth bias.
 * Other values reach render_enqueue_blended_tmd, which shifts them into the
 * GPU tpage semi-transparency field (ABR, bits 5-6). */
KF_ENUM_BEGIN(KfRenderQueueMode, u8)
    KF_RENDER_QUEUE_BLEND_AVERAGE = 0,
    KF_RENDER_QUEUE_BLEND_ADD = 1,
    KF_RENDER_QUEUE_BLEND_SUBTRACT = 2,
    KF_RENDER_QUEUE_BLEND_ADD_QUARTER = 3,
    KF_RENDER_QUEUE_TEXTURED_UNBIASED = 0x80,
    KF_RENDER_QUEUE_CLIPPED = 0xfe,
    KF_RENDER_QUEUE_TEXTURED = 0xff
KF_ENUM_END(KfRenderQueueMode)

/* Row of game_graphics_runtime.collision_rows (colour matrix, light matrix,
 * fog and back colour). Map-cell layers select rows 0..0x3f through their low
 * six bits; the fixed rows from 0x40 light HUD models, effects, actors and
 * map-placed models. NONE disables an object's override blend. Rows without a
 * distinctive consumer keep their encoding as a WIP name. */
KF_ENUM_BEGIN(KfLightingIndex, u8)
    KF_LIGHTING_HUD_COMPASS = 0x40,
    KF_LIGHTING_HUD = 0x41,
    KF_LIGHTING_PRESET_42 = 0x42,
    KF_LIGHTING_SCALED_EFFECT = 0x43,
    KF_LIGHTING_EFFECT = 0x44,
    KF_LIGHTING_MAP_PLACED = 0x46,
    KF_LIGHTING_ACTOR_DEFAULT = 0x47,
    KF_LIGHTING_PRESET_48 = 0x48,
    KF_LIGHTING_PRESET_49 = 0x49,
    KF_LIGHTING_NONE = 0xff
KF_ENUM_END(KfLightingIndex)

#endif
