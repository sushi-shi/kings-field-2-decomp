#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/actor.h>
#include <kf/game/asset.h>
#include <kf/game/audio.h>
#include <kf/game/cd.h>
#include <kf/game/effect.h>
#include <kf/game/graphics.h>
#include <kf/game/map_object.h>
#include <kf/game/memory.h>
#include <kf/game/player.h>
#include <kf/game/render_mask.h>
#include <kf/game/resources.h>

extern void func_80031850(u8 layer, u16 asset_index, const VECTOR *position,
                           const struct KfEulerAngles *rotation,
                           const SVECTOR *scale, KfPoolRecord **cache,
                           MATRIX *world_matrix, u16 clip, u16 phase,
                           u8 lighting_override, s16 lighting_blend,
                           u8 render_mode, s32 depth);
extern void func_80031d8c(s32 asset_index, const struct KfEulerAngles *rotation,
                           KfPoolRecord **cache, s32 clip, u16 phase,
                           s32 blend_mode, s32 lighting_flags, s16 depth);
extern s32 func_80036ad8(s32 x, s32 z, s32 width, s32 depth, s32 height);

extern MATRIX render_world_identity_matrix;

/* The two flag ranges are consumed as byte arrays by the resource updaters. */
ADDRESS(0x8003247c, 0xb70)
void func_8003247c(void)
{
    struct KfEulerAngles rotation;
    VECTOR actor_position;
    u8 tmd_flags[320];
    /* The second stack region spans 320 bytes; the VAB updater reads 64. */
    u8 vab_flags[320];
    KfActor *actor;
    KfMapObject *object;
    KfEffectRecord *effect;
    KfMapPlacedEntry *placed;
    s32 frame;
    s32 i;

    repeat_store_word((u32 *)tmd_flags, 0, 32);
    repeat_store_word((u32 *)vab_flags, 0, 16);
    actor = actor_state.actors;
    for (i = 0; i < KF_ACTOR_CAPACITY; i++, actor++) {
        u8 visible;
        u8 layer;
        KfTargetGroup *group;
        const VECTOR *position;
        MATRIX *world_matrix;

        if (actor->lifecycle != 1) {
            continue;
        }
        layer = actor->unknown_03;
        if (actor->unknown_28 & 0x2000) {
            layer |= 0x20;
        }
        if (actor->unknown_28 & 0x80000) {
            visible = map_cell_layer_mask_radius(&actor->position, 3) & actor->unknown_03;
        } else {
            visible = map_cell_layer_mask(&actor->position) & layer;
        }
        if (visible == 0) {
            continue;
        }
        if (resource_registry_get(actor->unknown_01 + 0x80) != 0) {
            position = func_8003c10c(actor, &actor_position);
            world_matrix = &game_graphics_runtime.render_state.view_matrix;
            if (actor->unknown_28 & 0x20) {
                rotation.x = 0;
                rotation.y = 0;
                rotation.z = 0;
                position = &actor->position;
                world_matrix = &render_world_identity_matrix;
            } else {
                rotation.x = actor->rotation.x;
                rotation.y = actor->rotation.y + 0x800;
                rotation.z = actor->rotation.z;
            }
            func_80031850(actor->unknown_03, actor->unknown_01 + 0x80,
                           position, &rotation, (SVECTOR *)&actor->unknown_48,
                           &actor->animation_cache, world_matrix,
                           actor->unknown_0c, actor->animation_phase,
                           actor->unknown_14, actor->unknown_16,
                           actor->unknown_13, (s8)actor->unknown_15);
        }
        group = &actor_state.target_groups[actor->group_index];
        vab_flags[group->unknown_07[0]] = 1;
        vab_flags[group->unknown_07[1]] = 1;
        tmd_flags[actor->unknown_01] = 1;
    }
    resource_tmd_update_range(0, 0, 0x80, 0x80, tmd_flags);
    resource_vab_update_range(4, 0x20, 2, 0x40, vab_flags);

    frame = cd_state.frame_count;
    repeat_store_word((u32 *)tmd_flags, 0, 80);
    repeat_store_word((u32 *)vab_flags, 0, 16);
    object = map_object_state.objects;
    for (i = 0; i < KF_MAP_OBJECT_CAPACITY; i++, object++) {
        u8 visibility;
        s32 object_index;

        if (object->object_id == KF_MAP_OBJECT_ID_NONE) {
            continue;
        }
        object->collision_flags &= 0x7f;
        if ((s8)object->action == 0x1f) {
            s32 sound;
            s32 distance;
            s32 radius;
            s32 volume;

            if (func_80036ad8(object->position.vx >> 11,
                              object->position.vz >> 11,
                              (s8)object->tail.fields.unknown_38,
                              object->tail.fields.unknown_39, 0x8000) == 0) {
                object->extra_40.raw = frame +
                    object->tail.fields.unknown_3e.value * 6;
                continue;
            }
            sound = object->tail.fields.unknown_3a.bytes.low;
            if ((u16)(audio_state.voices.params[sound].vab_slot_index - 0x42) < 0x40) {
                /* This update starts at VAB slot 0x42. */
                vab_flags[audio_state.voices.params[sound].vab_slot_index - 0x42] = 1;
            }
            if ((s32)(object->extra_40.raw - frame) < 0) {
                object->extra_40.raw = frame +
                    object->tail.fields.unknown_3e.value * 6;
                distance = player_state.camera_position.vx -
                    (object->tail.fields.unknown_38 * 0x400 + object->position.vx);
                if (distance < 0) distance = -distance;
                volume = object->tail.fields.unknown_38 * 0x400 - distance;
                distance = player_state.camera_position.vz -
                    (object->tail.fields.unknown_39 * 0x400 + object->position.vz);
                if (distance < 0) distance = -distance;
                distance = object->tail.fields.unknown_39 * 0x400 - distance;
                if (distance < volume) volume = distance;
                radius = (u8)object->tail.fields.spawn_sequence << 11;
                if (volume < radius) {
                    if (radius == 0) continue;
                    volume = object->tail.fields.unknown_3a.bytes.high * volume / radius;
                }
                if (object->tail.fields.spawn_sequence & 0x100) {
                    distance = player_state.camera_position.vy - object->position.vy;
                    if (distance < 0) distance = -distance;
                    volume -= object->tail.fields.unknown_3a.bytes.high * distance >> 13;
                }
                if (volume > 19) {
                    audio_play_sound(sound, volume);
                }
            }
        } else if ((s8)object->action == -16) {
            if (map_cell_visible(&object->position,
                                 (s8)object->tail.fields.unknown_38,
                                 object->tail.fields.unknown_39) != 0 &&
                (object->unknown_00 & render_mask_scan_state.first_layer_mask)) {
                if (resource_registry_get(object->object_id + 0x100) != 0) {
                    func_80031d8c(object->object_id + 0x100,
                                   (const struct KfEulerAngles *)&object->rotation,
                                   (KfPoolRecord **)&object->tail,
                                   object->unknown_01, object->unknown_0a,
                                   (s8)object->tail.fields.spawn_sequence,
                                   object->tail.fields.unknown_3a.bytes.high,
                                   0x1fff - object->tail.fields.unknown_3a.bytes.low);
                    object->collision_flags |= 0x80;
                }
                tmd_flags[object->object_id] = 1;
            }
        } else {
            u8 render_mode;
            if (object->collision_flags & 2) {
                visibility = map_cell_layer_mask_radius(&object->position,
                    map_object_state.templates[object->object_id].marker_action_05);
            } else {
                visibility = map_cell_layer_mask(&object->position);
            }
            if ((visibility & object->unknown_00) == 0) continue;
            object_index = object->object_id;
            tmd_flags[object_index] = 1;
            vab_flags[map_object_state.templates[object_index].unknown_02[0]] = 1;
            if (resource_registry_get(object_index + 0x100) != 0) {
                render_mode = object->unknown_02;
                if (object->collision_flags & 1) {
                    render_mode = (visibility & 0x80) ? 0xfe : 0xff;
                }
                rotation.x = object->rotation.vx;
                rotation.y = object->rotation.vy + 0x800;
                rotation.z = object->rotation.vz;
                func_80031850(object->unknown_00, object_index + 0x100,
                               &object->position, &rotation, &object->scale,
                               (KfPoolRecord **)&object->tail,
                               &game_graphics_runtime.render_state.view_matrix,
                               object->unknown_01, object->unknown_0a,
                               object->unknown_05, object->unknown_10,
                               render_mode,
                               (s16)object->unknown_0e);
                object->collision_flags |= 0x80;
            }
        }
    }
    resource_tmd_update_range(0, 0x80, 0x100, 0x140, tmd_flags);
    resource_vab_update_range(4, 0x60, 0x42, 0x40, vab_flags);

    effect = effect_state.records;
    for (i = 0; i < KF_EFFECT_CAPACITY; i++, effect++) {
        MATRIX *world_matrix;
        const struct KfEulerAngles *angles;

        if (effect->type == KF_EFFECT_SLOT_FREE ||
            (effect->unknown_08 & 3) == 0) continue;
        if ((effect->unknown_08 & 3) != 2 &&
            (map_cell_layer_mask(&effect->position) & effect->unknown_0a) == 0) continue;
        switch (effect->unknown_08 & 12) {
        case 0:
            rotation.x = effect->rotation.vx;
            rotation.y = effect->rotation.vy + 0x800;
            rotation.z = effect->rotation.vz;
            angles = &rotation;
            world_matrix = &game_graphics_runtime.render_state.view_matrix;
            break;
        case 4:
            angles = (const struct KfEulerAngles *)&effect->rotation;
            world_matrix = &render_world_identity_matrix;
            break;
        case 8:
            angles = (const struct KfEulerAngles *)&effect->rotation;
            world_matrix = &game_graphics_runtime.render_state.pitch_matrix;
            break;
        case 12:
            func_80031850(effect->unknown_0a, effect->render_id + 0x28,
                           &effect->position,
                           (const struct KfEulerAngles *)&effect->rotation,
                           (SVECTOR *)&effect->scale_x,
                           (KfPoolRecord **)&effect->direction, 0,
                           effect->animation_clip, effect->unknown_12,
                           effect->unknown_0c, effect->unknown_10,
                           effect->unknown_09, 0x14);
            continue;
        default:
            continue;
        }
        func_80031850(effect->unknown_0a, effect->render_id + 0x28,
                       &effect->position, angles, (SVECTOR *)&effect->scale_x,
                       (KfPoolRecord **)&effect->direction, world_matrix,
                       effect->animation_clip, effect->unknown_12,
                       effect->unknown_0c, effect->unknown_10,
                       effect->unknown_09, -60);
    }

    rotation.x = 0;
    rotation.y = 0;
    rotation.z = 0;
    placed = game_graphics_runtime.map_placed_entries;
    for (i = 0; i < KF_MAP_PLACED_ENTRY_COUNT; i++, placed++) {
        u8 visibility;
        if (placed->id == 0xffff) continue;
        visibility = map_cell_layer_mask(&placed->position);
        if (visibility & placed->layer) {
            func_80031850(placed->layer, placed->id + 0x28,
                           &placed->position, &rotation, 0, 0,
                           &game_graphics_runtime.render_state.pitch_matrix,
                           placed->frame_index + 0x80, 0, 0x46,
                           0x1000, 1, 0);
        }
        if (placed->frame_period != 0 &&
            game_graphics_runtime.map_placed_frame_counter % placed->frame_period == 0) {
            placed->frame_index++;
            if (placed->frame_index >= placed->frame_count) placed->frame_index = 0;
        }
    }
    game_graphics_runtime.map_placed_frame_counter++;
}

DATA(0x80063dcc, 0x20)
MATRIX render_world_identity_matrix = {
    {{0x1000, 0, 0}, {0, 0x1000, 0}, {0, 0, 0x1000}},
    0,
    {0, 0, 0}
};
