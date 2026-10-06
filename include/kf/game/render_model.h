#ifndef KF_GAME_RENDER_MODEL_H
#define KF_GAME_RENDER_MODEL_H

#include <kf/lib/types.h>
#include <kf/game/pool.h>
#include <psyq/sdk.h>

enum { KF_RENDER_MODEL_ROW_COUNT = 15 };

/* Fourteen live initialized rows followed by a full-width 0xff sentinel. */
typedef struct KfRenderModelRow {
    u8 state;
    u8 animation_clip;
    u8 lighting_index;
    u16 asset_id;
    u16 animation_phase;
    SVECTOR scale;
    SVECTOR translation;
    SVECTOR rotation;
    KfPoolRecord *animation_state;
} KfRenderModelRow;

typedef char kf_render_model_row_size[sizeof(KfRenderModelRow) == 36 ? 1 : -1];
typedef char kf_render_model_asset_id_offset[
    (u32)&((KfRenderModelRow *)0)->asset_id == 4 ? 1 : -1];
typedef char kf_render_model_rotation_offset[
    (u32)&((KfRenderModelRow *)0)->rotation == 24 ? 1 : -1];
typedef char kf_render_model_animation_state_offset[
    (u32)&((KfRenderModelRow *)0)->animation_state == 32 ? 1 : -1];

extern KfRenderModelRow render_model_rows[KF_RENDER_MODEL_ROW_COUNT];
extern s32 render_model_yaw_smoothing_accumulator;
extern MATRIX render_world_identity_matrix;

struct KfEulerAngles;
void render_world_model(u8 layer, u16 asset_index, const VECTOR *position,
                   const struct KfEulerAngles *rotation, const SVECTOR *scale,
                   KfPoolRecord **cache, MATRIX *world_matrix, u16 clip,
                   u16 phase, u8 lighting_override, s16 lighting_blend,
                   u8 render_mode, s32 depth);
void render_animated_object(s32 asset_index, const struct KfEulerAngles *rotation,
                   KfPoolRecord **cache, u16 clip, u16 phase,
                   s32 blend_mode, s32 lighting_flags, s16 depth);

#endif
