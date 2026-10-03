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
#include <kf/game/tmd.h>

enum { KF_MAP_CELL_SHIFT = 11 };

ADDRESS(0x80031fa0, 0x68)
KfAssetHeader *resource_registry_get(u16 index)
{
    KfAssetHeader *asset = game_graphics_runtime.asset_registry_entries[index];

    if (index < 104) {
        return asset;
    }
    if (asset != 0 && (u8)(memory_block_kind((u8 *)asset) - 1) < 2) {
        return asset;
    }
    return 0;
}

ADDRESS(0x80032008, 0x38)
void resource_tmd_read_complete(u8 *data)
{
    KfAssetHeader *asset = (KfAssetHeader *)data;

    tmd_prepare_primitive_indices((KfTmdHeader *)(data + asset->tmd_data_offset));
    memory_block_set_kind(data, 2);
}

ADDRESS(0x80032040, 0x70)
u32 map_cell_layer_mask(const VECTOR *position)
{
    s32 z = (position->vz >> KF_MAP_CELL_SHIFT) + game_graphics_runtime.render_state.cell_origin_z;
    s32 x;

    if ((u32)z >= KF_MAP_CELL_GRID_SIDE) {
        return 0;
    }
    x = (position->vx >> KF_MAP_CELL_SHIFT) + game_graphics_runtime.render_state.cell_origin_x;
    if ((u32)x >= KF_MAP_CELL_GRID_SIDE) {
        return 0;
    }
    return game_graphics_runtime.render_grid.map_cell_layer_masks[z][x];
}

ADDRESS(0x800320b0, 0xc4)
u32 map_cell_layer_mask_radius(const VECTOR *position, s32 radius)
{
    s32 span = (s32)((u32)radius << 1);
    u8 mask = 0;
    s32 z = (position->vz >> KF_MAP_CELL_SHIFT) + game_graphics_runtime.render_state.cell_origin_z - radius;
    s32 x0 = (position->vx >> KF_MAP_CELL_SHIFT) + game_graphics_runtime.render_state.cell_origin_x - radius;
    s32 row_offset = z * KF_MAP_CELL_GRID_SIDE;
    const u8 *row = &game_graphics_runtime.render_grid.map_cell_layer_masks[0][0] + row_offset;
    s32 row_count = span;

    do {
        if (row_offset >= 0 && (u32)row_offset < sizeof(game_graphics_runtime.render_grid.map_cell_layer_masks)) {
            s32 x = x0;
            s32 column_count = span;

            do {
                if (x >= 0 && (u32)x < KF_MAP_CELL_GRID_SIDE) {
                    mask |= row[x];
                }
                x++;
                column_count--;
            } while (column_count != -1);
        }
        row += KF_MAP_CELL_GRID_SIDE;
        row_offset += KF_MAP_CELL_GRID_SIDE;
        row_count--;
    } while (row_count != -1);

    return mask;
}

ADDRESS(0x80032174, 0x64)
s32 map_cell_visible(const VECTOR *position, s32 radius_x, s32 radius_z)
{
    s32 z = position->vz >> KF_MAP_CELL_SHIFT;
    s32 x;
    s32 visible = 0;

    if (game_graphics_runtime.render_state.view_cell_z
        < (s32)((u32)z - (u32)radius_z)) {
        goto done;
    }
    if ((s32)((u32)z + (u32)radius_z)
        < game_graphics_runtime.render_state.view_cell_z) {
        goto done;
    }

    x = position->vx >> KF_MAP_CELL_SHIFT;
    if (game_graphics_runtime.render_state.view_cell_x
        < (s32)((u32)x - (u32)radius_x)) {
        goto done;
    }
    visible = (s32)((u32)x + (u32)radius_x)
        >= game_graphics_runtime.render_state.view_cell_x;

done:
    return visible;
}

ADDRESS(0x800321d8, 0x9c)
void resource_tmd_queue_read(s32 archive_slot, s32 entry, s32 registry_index)
{
    u32 size = cd_archive_entry_size(archive_slot, entry);
    u8 *block;

    block = memory_arena_allocate_block(KF_GAME_RESOURCE_ARENA_BASE, size,
        (u8 **)&game_graphics_runtime.asset_registry_entries[registry_index]);
    if (block != 0) {
        memory_block_set_kind(block, 3);
        memory_block_set_tag(block, registry_index);
        /* Kind 0x10 completion passes the destination, not the request. */
        cd_archive_queue_read(archive_slot, entry, (u_long *)block,
            (KfCdRequestCallback)resource_tmd_read_complete);
    }
}

ADDRESS(0x80032274, 0xf0)
void resource_vab_update_range(s32 archive_slot, s32 entry, s32 vab_slot,
    s32 count, u8 *flags)
{
    s32 end = count + entry;

    while (entry < end) {
        KfAudioVabSlot *slot = &audio_state.vab_slots[vab_slot];
        KfAudioVabStreamSlot *state = slot->stream_slot;

        if (*flags++) {
            if (state == 0) {
                audio_queue_vab_stream(archive_slot, entry, vab_slot);
            } else if (state->state == 2) {
                state->state = 1;
            }
        } else if (state != 0 && state->state == 1) {
            state->state = 2;
        }
        entry++;
        vab_slot++;
    }
}

ADDRESS(0x80032364, 0x118)
void resource_tmd_update_range(s32 archive_slot, s32 entry, s32 registry_index,
    s32 count, u8 *flags)
{
    s32 end = count + entry;

    while (entry < end) {
        u8 *block;

        if (*flags++) {
            block = (u8 *)game_graphics_runtime.asset_registry_entries[registry_index];
            if (block == 0) {
                resource_tmd_queue_read(archive_slot, entry, registry_index);
            } else if (memory_block_kind(block) == 1) {
                memory_block_set_kind(block, 2);
            }
        } else {
            block = (u8 *)game_graphics_runtime.asset_registry_entries[registry_index];
            if (block != 0 && memory_block_kind(block) != 3) {
                memory_block_set_kind(block, 1);
            }
        }
        entry++;
        registry_index++;
    }
}
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
                               position, &rotation, (SVECTOR *)&actor->model_scale_x,
                               &actor->animation_cache, &render_world_identity_matrix,
                               actor->animation_id, actor->animation_phase,
                               actor->lighting_override, actor->lighting_blend,
                               actor->render_mode, (s8)actor->render_depth);
            } else {
                rotation.x = actor->rotation.x;
                rotation.y = actor->rotation.y + 0x800;
                rotation.z = actor->rotation.z;
                render_world_model(actor->current_map_layer, actor->definition_id + 0x80,
                               position, &rotation, (SVECTOR *)&actor->model_scale_x,
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
                              object->tail.ambient_sound.region_width,
                              object->tail.ambient_sound.region_depth, 0x8000) == 0)
                goto map_sound_outside;
            sound = object->tail.ambient_sound.sound_id;
            if ((u16)(audio_state.voices.params[sound].vab_slot_index - 0x42) < 0x40) {
                /* This update starts at VAB slot 0x42. */
                vab_flags[audio_state.voices.params[sound].vab_slot_index - 0x42] = 1;
            }
            if ((s32)(object->extra_40.next_sound_frame - frame) < 0) {
                object->extra_40.next_sound_frame = frame +
                    object->tail.ambient_sound.repeat_delay_units * 6;
                distance = camera_position->vx -
                    (object->tail.ambient_sound.region_width * 0x400 + object->position.vx);
                if (distance < 0) distance = -distance;
                volume = object->tail.ambient_sound.region_width * 0x400 - distance;
                distance = camera_position->vz -
                    (object->tail.ambient_sound.region_depth * 0x400 + object->position.vz);
                if (distance < 0) distance = -distance;
                distance = object->tail.ambient_sound.region_depth * 0x400 - distance;
                if (distance < volume) volume = distance;
                radius = object->tail.ambient_sound.audible_radius_code << 11;
                if (volume >= radius) {
                    volume = object->tail.ambient_sound.maximum_volume;
                } else {
                    if (radius == 0) goto map_object_next;
                    volume = object->tail.ambient_sound.maximum_volume * volume / radius;
                }
                if (object->tail.ambient_sound.vertical_attenuation_flags & 1) {
                    distance = camera_position->vy - object->position.vy;
                    if (distance < 0) distance = -distance;
                    volume -= object->tail.ambient_sound.maximum_volume * distance >> 13;
                }
                if (volume > 19) {
                    audio_play_sound(sound, volume);
                }
            }
            goto map_object_next;
map_sound_outside:
            object->extra_40.next_sound_frame = frame +
                object->tail.ambient_sound.repeat_delay_units * 6;
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
    effect_cache = &((KfEffectCacheTail *)effect->unknown_3c)->animation_cache;
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
