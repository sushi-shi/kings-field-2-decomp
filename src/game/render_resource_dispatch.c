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
    s16 remaining;

    repeat_store_word((u32 *)tmd_flags, 0, 32);
    repeat_store_word((u32 *)vab_flags, 0, 16);
    actor = actor_state.actors;
    for (remaining = KF_ACTOR_CAPACITY - 1;
         remaining != -1; remaining--, actor++) {
        u32 layer;
        KfTargetGroup *group;
        const VECTOR *actor_position_ptr;
        const VECTOR *position;
        MATRIX *world_matrix;

        actor_position_ptr = &actor->position;
        if (actor->lifecycle != 1) {
            continue;
        }
        if (actor->unknown_28 & 0x2000) {
            layer = actor->unknown_03 | 0x20;
        } else {
            layer = actor->unknown_03;
        }
        if (actor->unknown_28 & 0x80000) goto actor_radius_check;
        if ((map_cell_layer_mask(actor_position_ptr) & layer) == 0) continue;
actor_visible:
        if (resource_registry_get(actor->unknown_01 + 0x80) != 0) {
            position = func_8003c10c(actor, &actor_position);
            if (actor->unknown_28 & 0x20) {
                rotation.z = 0;
                rotation.y = 0;
                rotation.x = 0;
                position = actor_position_ptr;
                world_matrix = &render_world_identity_matrix;
            } else {
                rotation.x = actor->rotation.x;
                rotation.y = actor->rotation.y + 0x800;
                rotation.z = actor->rotation.z;
                world_matrix = &game_graphics_runtime.render_state.view_matrix;
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
        continue;
actor_radius_check:
        if (map_cell_layer_mask_radius(actor_position_ptr, 3) &
            actor->unknown_03) goto actor_visible;
    }
    resource_tmd_update_range(0, 0, 0x80, 0x80, tmd_flags);
    resource_vab_update_range(4, 0x20, 2, 0x40, vab_flags);

    frame = cd_state.frame_count;
    repeat_store_word((u32 *)tmd_flags, 0, 80);
    repeat_store_word((u32 *)vab_flags, 0, 16);
    object = map_object_state.objects;
    for (remaining = KF_MAP_OBJECT_CAPACITY - 1;
         remaining != -1; remaining--, object++) {
        u32 visibility;
        s32 object_index;

        if (object->object_id == KF_MAP_OBJECT_ID_NONE) {
            continue;
        }
        object->collision_flags &= 0x7f;
        if (object->action == 0x1f) goto map_sound_action;
        if (object->action != 0xf0) goto map_ordinary_object;
        if (map_cell_visible(&object->position,
                             object->tail.fields.unknown_38,
                             object->tail.fields.unknown_39) != 0 &&
            (object->unknown_00 & render_mask_scan_state.first_layer_mask)) {
            if (resource_registry_get(object->object_id + 0x100) != 0) {
                func_80031d8c(object->object_id + 0x100,
                               (const struct KfEulerAngles *)&object->rotation,
                               (KfPoolRecord **)&object->tail,
                               object->unknown_01, object->unknown_0a,
                               (u8)object->tail.fields.spawn_sequence,
                               object->tail.fields.unknown_3a.bytes.high,
                               0x1fff - object->tail.fields.unknown_3a.bytes.low);
                object->collision_flags |= 0x80;
            }
            tmd_flags[object->object_id] = 1;
        }
        continue;
map_sound_action: {
            s32 sound;
            s32 distance;
            s32 radius;
            s32 volume;

            if (func_80036ad8(object->position.vx >> 11,
                              object->position.vz >> 11,
                              object->tail.fields.unknown_38,
                              object->tail.fields.unknown_39, 0x8000) == 0)
                goto map_sound_outside;
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
                radius = object->tail.spawn_bytes.spawn_sequence.low << 11;
                if (volume >= radius) {
                    volume = object->tail.fields.unknown_3a.bytes.high;
                } else {
                    if (radius == 0) continue;
                    volume = object->tail.fields.unknown_3a.bytes.high * volume / radius;
                }
                if (object->tail.spawn_bytes.spawn_sequence.high & 1) {
                    distance = player_state.camera_position.vy - object->position.vy;
                    if (distance < 0) distance = -distance;
                    volume -= object->tail.fields.unknown_3a.bytes.high * distance >> 13;
                }
                if (volume > 19) {
                    audio_play_sound(sound, volume);
                }
            }
            continue;
map_sound_outside:
            object->extra_40.raw = frame +
                object->tail.fields.unknown_3e.value * 6;
            continue;
        }
map_ordinary_object: {
            u8 render_mode;
            KfMapObjectTemplate *object_template;
            if (object->collision_flags & 2) goto map_radius_check;
            visibility = map_cell_layer_mask(&object->position);
            if ((visibility & object->unknown_00) == 0) continue;
            object_template = &map_object_state.templates[object->object_id];
map_ordinary_visible:
            object_index = object->object_id;
            tmd_flags[object_index] = 1;
            vab_flags[object_template->unknown_02[0]] = 1;
            if (resource_registry_get(object_index + 0x100) != 0) {
                rotation.x = object->rotation.vx;
                rotation.y = object->rotation.vy + 0x800;
                rotation.z = object->rotation.vz;
                render_mode = object->unknown_02;
                if (object->collision_flags & 1) {
                    render_mode = (visibility & 0x80) ? 0xfe : 0xff;
                }
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
            continue;
map_radius_check:
            object_template = &map_object_state.templates[object->object_id];
            visibility = map_cell_layer_mask_radius(&object->position,
                object_template->marker_action_05);
            if (visibility & object->unknown_00) goto map_ordinary_visible;
        }
    }
    resource_tmd_update_range(0, 0x80, 0x100, 0x140, tmd_flags);
    resource_vab_update_range(4, 0x60, 0x42, 0x40, vab_flags);

    effect = effect_state.records;
    for (remaining = KF_EFFECT_CAPACITY - 1;
         remaining != -1; remaining--, effect++) {
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

    rotation.z = 0;
    rotation.y = 0;
    rotation.x = 0;
    placed = game_graphics_runtime.map_placed_entries;
    for (remaining = KF_MAP_PLACED_ENTRY_COUNT - 1;
         remaining != -1; remaining--, placed++) {
        u32 visibility;
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
