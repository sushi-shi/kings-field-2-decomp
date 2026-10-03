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
#include <kf/game/render_model.h>
#include <kf/game/resources.h>

/* The two flag ranges are consumed as byte arrays by the resource updaters. */
ADDRESS(0x8003247c, 0xb70)
void render_scene_and_update_resources(void)
{
    struct KfEulerAngles rotation;
    VECTOR actor_position;
    u8 tmd_flags[320];
    /* The second stack region spans 320 bytes; the VAB updater reads 64. */
    u8 vab_flags[320];
    KfActor *actor;
    const VECTOR *actor_position_ptr;
    KfMapObject *object;
    KfEffectRecord *effect;
    KfPoolRecord **effect_cache;
    SVECTOR *effect_scale_ptr;
    const struct KfEulerAngles *effect_rotation_ptr;
    KfMapPlacedEntry *placed;
    const VECTOR *camera_position;
    s32 frame;
    s16 remaining;

    repeat_store_word((u32 *)tmd_flags, 0, 32);
    repeat_store_word((u32 *)vab_flags, 0, 16);
    actor = actor_state.actors;
    actor_position_ptr = &actor->position;
    remaining = KF_ACTOR_CAPACITY - 1;
    while (remaining != -1) {
        u32 layer;
        KfTargetGroup *group;
        const VECTOR *position;

        if (actor->lifecycle != 1) {
            goto actor_next;
        }
        if (actor->unknown_28 & 0x2000) {
            layer = actor->current_map_layer | 0x20;
        } else {
            layer = actor->current_map_layer;
        }
        if (actor->unknown_28 & 0x80000) goto actor_radius_check;
        if ((map_cell_layer_mask(actor_position_ptr) & layer) == 0) goto actor_next;
actor_visible:
        if (resource_registry_get(actor->definition_id + 0x80) != 0) {
            position = actor_resolve_group_position(actor, &actor_position);
            if (actor->unknown_28 & 0x20) {
                rotation.z = 0;
                rotation.y = 0;
                rotation.x = 0;
                position = actor_position_ptr;
                render_world_model(actor->current_map_layer, actor->definition_id + 0x80,
                               position, &rotation, (SVECTOR *)&actor->unknown_48,
                               &actor->animation_cache, &render_world_identity_matrix,
                               actor->animation_id, actor->animation_phase,
                               actor->lighting_override, actor->lighting_blend,
                               actor->render_mode, (s8)actor->render_depth);
            } else {
                rotation.x = actor->rotation.x;
                rotation.y = actor->rotation.y + 0x800;
                rotation.z = actor->rotation.z;
                render_world_model(actor->current_map_layer, actor->definition_id + 0x80,
                               position, &rotation, (SVECTOR *)&actor->unknown_48,
                               &actor->animation_cache,
                               &game_graphics_runtime.render_state.view_matrix,
                               actor->animation_id, actor->animation_phase,
                               actor->lighting_override, actor->lighting_blend,
                               actor->render_mode, (s8)actor->render_depth);
            }
        }
        group = &actor_state.target_groups[actor->group_index];
        vab_flags[group->vab_resource_indices[0]] = 1;
        vab_flags[group->vab_resource_indices[1]] = 1;
        tmd_flags[actor->definition_id] = 1;
        goto actor_next;
actor_radius_check:
        if (map_cell_layer_mask_radius(actor_position_ptr, 3) &
            actor->current_map_layer) goto actor_visible;
actor_next:
        actor_position_ptr = (const VECTOR *)((const u8 *)actor_position_ptr +
                                               sizeof *actor);
        actor++;
        remaining--;
    }
    resource_tmd_update_range(0, 0, 0x80, 0x80, tmd_flags);
    resource_vab_update_range(4, 0x20, 2, 0x40, vab_flags);

    frame = cd_state.frame_count;
    camera_position = &player_state.camera_position;
    repeat_store_word((u32 *)tmd_flags, 0, 80);
    repeat_store_word((u32 *)vab_flags, 0, 16);
    object = map_object_state.objects;
    remaining = KF_MAP_OBJECT_CAPACITY - 1;
    while (remaining != -1) {
        u32 visibility;
        s32 object_index;

        if (object->object_id == KF_MAP_OBJECT_ID_NONE) {
            goto map_object_next;
        }
        object->collision_flags &= 0x7f;
        if (object->action == 0x1f) goto map_sound_action;
        if (object->action != 0xf0) goto map_ordinary_object;
        if (map_cell_visible(&object->position,
                             object->tail.animated.radius_x,
                             object->tail.animated.radius_z) != 0 &&
            (object->layer_mask & render_mask_scan_state.first_layer_mask)) {
            if (resource_registry_get(object->object_id + 0x100) != 0) {
                render_animated_object(object->object_id + 0x100,
                               (const struct KfEulerAngles *)&object->rotation,
                               &object->tail.animated.animation_cache,
                               object->asset_clip_selector, object->phase_q12,
                               object->tail.animated.blend_mode,
                               object->tail.animated.lighting_flags,
                               0x1fff - object->tail.animated.depth_code);
                object->collision_flags |= 0x80;
            }
            tmd_flags[object->object_id] = 1;
        }
        goto map_object_next;
map_sound_action: {
            s32 sound;
            s32 distance;
            s32 radius;
            s32 volume;

            if (player_camera_within_map_region(object->position.vx >> 11,
                              object->position.vz >> 11,
                              object->tail.fields.unknown_38,
                              object->tail.fields.unknown_39, 0x8000) == 0)
                goto map_sound_outside;
            sound = object->tail.fields.unknown_3a.bytes.low;
            if ((u16)(audio_state.voices.params[sound].vab_slot_index - 0x42) < 0x40) {
                /* This update starts at VAB slot 0x42. */
                vab_flags[audio_state.voices.params[sound].vab_slot_index - 0x42] = 1;
            }
            if ((s32)(object->extra_40.next_sound_frame - frame) < 0) {
                object->extra_40.next_sound_frame = frame +
                    object->tail.fields.unknown_3e.value * 6;
                distance = camera_position->vx -
                    (object->tail.fields.unknown_38 * 0x400 + object->position.vx);
                if (distance < 0) distance = -distance;
                volume = object->tail.fields.unknown_38 * 0x400 - distance;
                distance = camera_position->vz -
                    (object->tail.fields.unknown_39 * 0x400 + object->position.vz);
                if (distance < 0) distance = -distance;
                distance = object->tail.fields.unknown_39 * 0x400 - distance;
                if (distance < volume) volume = distance;
                radius = object->tail.spawn_bytes.spawn_sequence.low << 11;
                if (volume >= radius) {
                    volume = object->tail.fields.unknown_3a.bytes.high;
                } else {
                    if (radius == 0) goto map_object_next;
                    volume = object->tail.fields.unknown_3a.bytes.high * volume / radius;
                }
                if (object->tail.spawn_bytes.spawn_sequence.high & 1) {
                    distance = camera_position->vy - object->position.vy;
                    if (distance < 0) distance = -distance;
                    volume -= object->tail.fields.unknown_3a.bytes.high * distance >> 13;
                }
                if (volume > 19) {
                    audio_play_sound(sound, volume);
                }
            }
            goto map_object_next;
map_sound_outside:
            object->extra_40.next_sound_frame = frame +
                object->tail.fields.unknown_3e.value * 6;
            goto map_object_next;
        }
map_ordinary_object: {
            u8 render_mode;
            KfMapObjectTemplate *object_template;
            if (object->collision_flags & 2) goto map_radius_check;
            visibility = map_cell_layer_mask(&object->position);
            if ((visibility & object->layer_mask) == 0) goto map_object_next;
            object_template = &map_object_state.templates[object->object_id];
map_ordinary_visible:
            object_index = object->object_id;
            tmd_flags[object_index] = 1;
            vab_flags[object_template->vab_resource_index] = 1;
            if (resource_registry_get(object_index + 0x100) != 0) {
                rotation.x = object->rotation.vx;
                rotation.y = object->rotation.vy + 0x800;
                rotation.z = object->rotation.vz;
                render_mode = object->render_queue_mode;
                if (object->collision_flags & 1) {
                    render_mode = (visibility & 0x80) ? 0xfe : 0xff;
                }
                render_world_model(object->layer_mask, object_index + 0x100,
                               &object->position, &rotation, &object->scale,
                               (KfPoolRecord **)&object->tail,
                               &game_graphics_runtime.render_state.view_matrix,
                               object->asset_clip_selector, object->phase_q12,
                               object->lighting_override_index, object->lighting_blend_q12,
                               render_mode,
                               (s16)object->render_depth_offset);
                object->collision_flags |= 0x80;
            }
            goto map_object_next;
map_radius_check:
            object_template = &map_object_state.templates[object->object_id];
            visibility = map_cell_layer_mask_radius(&object->position,
                object_template->marker_action_05);
            if (visibility & object->layer_mask) goto map_ordinary_visible;
        }
map_object_next:
        object++;
        remaining--;
    }
    resource_tmd_update_range(0, 0x80, 0x100, 0x140, tmd_flags);
    resource_vab_update_range(4, 0x60, 0x42, 0x40, vab_flags);

    effect = effect_state.records;
    effect_cache = (KfPoolRecord **)&effect->unknown_3c[0];
    effect_scale_ptr = (SVECTOR *)&effect->scale_x;
    effect_rotation_ptr = (const struct KfEulerAngles *)&effect->rotation;
    remaining = KF_EFFECT_CAPACITY - 1;
    while (remaining != -1) {
        if (effect->type == KF_EFFECT_SLOT_FREE ||
            (effect->render_flags & 3) == 0) goto effect_next;
        if ((effect->render_flags & 3) != 2 &&
            (map_cell_layer_mask(&effect->position) & effect->map_layer_mask) == 0)
            goto effect_next;
        switch (effect->render_flags & 12) {
        case 0:
            rotation.x = effect->rotation.vx;
            rotation.y = effect->rotation.vy + 0x800;
            rotation.z = effect->rotation.vz;
            render_world_model(effect->map_layer_mask, effect->render_id + 0x28,
                           &effect->position, &rotation, effect_scale_ptr,
                           effect_cache,
                           &game_graphics_runtime.render_state.view_matrix,
                           effect->animation_clip, effect->animation_phase_q12,
                           effect->lighting_override_index, effect->lighting_blend_q12,
                           effect->render_queue_mode, -60);
            break;
        case 4:
            render_world_model(effect->map_layer_mask, effect->render_id + 0x28,
                           &effect->position, effect_rotation_ptr,
                           effect_scale_ptr, effect_cache,
                           &render_world_identity_matrix,
                           effect->animation_clip, effect->animation_phase_q12,
                           effect->lighting_override_index, effect->lighting_blend_q12,
                           effect->render_queue_mode, -60);
            break;
        case 8:
            render_world_model(effect->map_layer_mask, effect->render_id + 0x28,
                           &effect->position, effect_rotation_ptr,
                           effect_scale_ptr, effect_cache,
                           &game_graphics_runtime.render_state.pitch_matrix,
                           effect->animation_clip, effect->animation_phase_q12,
                           effect->lighting_override_index, effect->lighting_blend_q12,
                           effect->render_queue_mode, -60);
            break;
        case 12:
            render_world_model(effect->map_layer_mask, effect->render_id + 0x28,
                           &effect->position,
                           effect_rotation_ptr,
                           effect_scale_ptr,
                           effect_cache, 0,
                           effect->animation_clip, effect->animation_phase_q12,
                           effect->lighting_override_index, effect->lighting_blend_q12,
                           effect->render_queue_mode, 0x14);
            break;
        }
effect_next:
        effect++;
        effect_cache = (KfPoolRecord **)((u8 *)effect_cache + sizeof *effect);
        effect_scale_ptr = (SVECTOR *)((u8 *)effect_scale_ptr + sizeof *effect);
        effect_rotation_ptr = (const struct KfEulerAngles *)(
            (const u8 *)effect_rotation_ptr + sizeof *effect);
        remaining--;
    }

    placed = game_graphics_runtime.map_placed_entries;
    rotation.z = 0;
    rotation.y = 0;
    rotation.x = 0;
    remaining = KF_MAP_PLACED_ENTRY_COUNT - 1;
    while (remaining != -1) {
        u32 visibility;
        if (placed->id != 0xffff) {
            visibility = map_cell_layer_mask(&placed->position);
            if (visibility & placed->layer) {
                render_world_model(placed->layer, placed->id + 0x28,
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
        placed++;
        remaining--;
    }
    game_graphics_runtime.map_placed_frame_counter++;
}

DATA(0x80063dcc, 0x20)
MATRIX render_world_identity_matrix = {
    {{0x1000, 0, 0}, {0, 0x1000, 0}, {0, 0, 0x1000}},
    0,
    {0, 0, 0}
};
