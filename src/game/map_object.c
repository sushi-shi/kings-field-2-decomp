#include <stdarg.h>
#include <kf/lib/null.h>
#include <kf/lib/address.h>
#include <kf/game/callback.h>
#include <kf/game/cd.h>
#include <kf/game/map_cell.h>
#include <kf/game/map_cell_pattern.h>
#include <kf/game/map_object.h>
#include <kf/game/player.h>
#include <psyq/libc.h>
#include <kf/lib/math.h>
#include <kf/game/animation.h>

RODATA(0x80011484, 0x494)

ADDRESS(0x80035504, 0x30)
KfAudioPlaybackResult map_object_play_spatial_sound(KfMapObject *object, s32 sound)
{
    return audio_play_spatial_default_range(sound, &object->position, 120, 0);
}

ADDRESS(0x80035534, 0x5c)
void map_object_pool_reset(void)
{
    KfMapObject *object;
    u16 remaining;

    object = map_object_state.objects;
    remaining = KF_MAP_OBJECT_CAPACITY - 1;
    do {
        object->object_id = KF_OBJECT_NONE;
        object->action = KF_MAP_OBJECT_OP_NONE;
        object->tail.reset_words[2] = 0;
        object->tail.reset_words[1] = 0;
        object->tail.reset_words[0] = 0;
        object++;
    } while (remaining-- != 0);

    map_object_state.placement_drop_sequence = 0;
    map_object_state.definition_drop_sequence = 0;
    map_object_state.spawn_sequence_pool_15e = 0;
}

ADDRESS(0x80035590, 0x48)
void map_object_reset(KfMapObject *object)
{
    object->asset_clip_selector = KF_MAP_OBJECT_STATIC_OBJECT_ZERO;
    object->phase_q12 = 0;
    object->render_queue_mode = KF_RENDER_QUEUE_TEXTURED;
    object->layer_mask = KF_MAP_LAYER_NONE;
    object->rotation.vz = 0;
    object->rotation.vx = 0;
    object->rotation.vy = 0;
    object->scale.vx = object->scale.vy = object->scale.vz = KF_FIXED12_ONE;
    object->action = KF_MAP_OBJECT_OP_NONE;
    object->render_depth_offset = 0;
    object->lighting_override_index = KF_LIGHTING_NONE;
    object->lighting_blend_q12 = 0;
}

ADDRESS(0x800355d8, 0xd4)
void map_object_set_property(s32 index, KfMapObjectProperty property, ...)
{
    KfMapObject *object;
    KfMapObjectTemplate *object_template;
    va_list arguments;

    if (index == KF_MAP_OBJECT_INDEX_NONE) {
        return;
    }

    object = &map_object_state.objects[index];
    object_template = &map_object_state.templates[KF_ENUM_ENCODE(u16, object->object_id)];
    va_start(arguments, property);
    switch (property) {
    case KF_MAP_OBJECT_PROPERTY_CLEAR_LAYER_AND_STATE:
        object->layer_mask = KF_MAP_LAYER_NONE;
        object->tail.fields.unknown_38 = 0;
        if (object_template->kind == 0x10) {
            object->rotation.vz = 0x400;
        }
        break;
    case KF_MAP_OBJECT_PROPERTY_SET_LAYER_MASK:
        object->layer_mask = KF_ENUM_DECODE(KfMapLayerMask, va_arg(arguments, u8));
        break;
    case KF_MAP_OBJECT_PROPERTY_ARM_EVENT:
        object->tail.fields.unknown_38 = 0xff;
        break;
    case KF_MAP_OBJECT_PROPERTY_SET_RENDER_DEPTH:
        object->render_depth_offset = va_arg(arguments, u16);
        break;
    }
    va_end(arguments);
}

ADDRESS(0x800356ac, 0xf4)
void map_object_set_cell_marker(KfMapObject *object, KfMapCellMarkerMode mode, u8 marker)
{
    KfMapOccupancyCell *cell;
    u8 *cell_marker;

    if (mode == KF_MAP_CELL_MARKER_PLACE && player_state.map_marker_visual_effect_timer == 0) {
        s32 cell_z = object->position.vz >> 11;
        s32 cell_x = object->position.vx >> 11;
        KfMapOccupancyCell *row = bss_801c7540.map_cells[cell_z];
        cell = &row[cell_x];
        cell_marker = &cell->layer[0].object_index;
        if (object->layer_mask != KF_MAP_LAYER_FIRST) {
            cell_marker = &cell->layer[1].object_index;
        }
        *cell_marker = marker;
        object->scale.vx = object->scale.vy = object->scale.vz = 0;
    } else {
        s32 cell_z = object->position.vz >> 11;
        s32 cell_x = object->position.vx >> 11;
        KfMapOccupancyCell *row = bss_801c7540.map_cells[cell_z];
        cell = &row[cell_x];
        cell_marker = &cell->layer[0].object_index;
        if (object->layer_mask != KF_MAP_LAYER_FIRST) {
            cell_marker = &cell->layer[1].object_index;
        }
        *cell_marker = KF_MAP_CELL_MARKER_CLEARED;
        object->scale.vx = object->scale.vy = object->scale.vz = KF_FIXED12_ONE;
    }
}

ADDRESS(0x800357a0, 0xf4)
void map_object_refresh_cell_markers(KfMapCellMarkerMode mode)
{
    KfMapObject *object;
    KfMapObjectTemplate *object_template;
    s32 index;

    object = map_object_state.objects;
    for (index = KF_MAP_OBJECT_CAPACITY; index != 0; object++, index--) {
        if (object->action != KF_MAP_OBJECT_OP_5) {
            if (object->action != KF_MAP_OBJECT_OP_81) {
                continue;
            }
            if (object->tail.fields.unknown_3a.bytes.high == 0xff) {
                continue;
            }
            object_template = &map_object_state.templates[KF_ENUM_ENCODE(u16, object->object_id)];
            map_object_set_cell_marker(object, mode,
                                       object_template->params.marker.marker_action_51);
        } else {
            if (object->tail.marker.marker_id == 0xfe) {
                continue;
            }
            /* The marker mode doubles as hide (0) / restore-layer (1). */
            map_object_set_property(object->tail.linked_property.linked_object_index,
                                    KF_ENUM_DECODE(KfMapObjectProperty, KF_ENUM_VALUE(mode)),
                                    object->layer_mask);
            object_template = &map_object_state.templates[KF_ENUM_ENCODE(u16, object->object_id)];
            map_object_set_cell_marker(object, mode,
                                       object_template->params.marker.marker_action_05);
        }
    }
}

ADDRESS(0x80035894, 0x7e4)
void map_object_initialize_from_placements(const KfMapObjectPlacement *placements)
{
    KfMapObject *object = map_object_state.objects;
    u32 frame_count = cd_state.frame_count;
    s32 index;

    for (index = KF_MAP_OBJECT_PLACED_COUNT - 1; index != -1; placements++, object++, index--) {
        const KfMapObjectTemplate *object_template;
        KfMapOccupancyCell *cell;
        KfMapOccupancyCell *row;
        KfMapOccupancyLayer *layer;

        if (placements->object_id == KF_OBJECT_PLACEMENT_NONE) {
            object->object_id = KF_OBJECT_NONE;
        } else {
            object->object_id = placements->object_id;
        }
        object->action = KF_MAP_OBJECT_OP_NONE;
        if (object->object_id == KF_OBJECT_NONE) {
            continue;
        }

        object_template = &map_object_state.templates[KF_ENUM_ENCODE(u16, object->object_id)];
        object->action_timer = 0;
        object->asset_clip_selector = KF_MAP_OBJECT_STATIC_OBJECT_ZERO;
        object->phase_q12 = 0;
        object->render_queue_mode = KF_RENDER_QUEUE_TEXTURED;
        object->rotation.vz = 0;
        object->rotation.vx = 0;
        object->rotation.vy = -(s32)placements->rotation_y & 0xfff;
        object->scale.vz = KF_FIXED12_ONE;
        object->scale.vy = KF_FIXED12_ONE;
        object->scale.vx = KF_FIXED12_ONE;
        object->layer_mask = placements->layer_mask;
        object->collision_flags = object_template->collision_flags;
        object->render_depth_offset = object_template->initial_render_depth_offset;
        object->lighting_override_index = KF_LIGHTING_NONE;
        object->lighting_blend_q12 = 0;
        if (object->collision_flags & 0x20) {
            object->render_queue_mode = KF_RENDER_QUEUE_TEXTURED_UNBIASED;
        }
        object->collision_height = object_template->interaction_height;
        object->position.vx = ((u32)placements->region_x << 11) + placements->local_x;
        object->position.vz = ((u32)placements->region_z << 11) + placements->local_z;
        row = bss_801c7540.map_cells[placements->region_z];
        cell = &row[placements->region_x];
        layer = cell->layer;
        if (object->layer_mask != KF_MAP_LAYER_FIRST) {
            layer++;
        }
        object->position.vy = placements->height - ((s32)layer->elevation << 7);
        object->tail.placement.copy_words = placements->tail_words;
        memset((void *)&object->extra_40, 0xff, sizeof object->extra_40);

        if (object_template->collision_radius != 0) {
            map_cell_add_layer_occupancy(object->position.vx, object->position.vz,
                          object_template->collision_radius, 1);
        }

        switch (object_template->collision_kind) {
        case KF_MAP_OBJECT_OP_64:
            if (object_template->kind != 0x20) {
                if (object->tail.initial_rotation.rotation_x_code != 0xff) {
                    object->rotation.vx =
                        object->tail.initial_rotation.rotation_x_code << 6;
                }
                if (object->tail.initial_rotation.rotation_y_code != 0xff) {
                    object->rotation.vy =
                        object->tail.initial_rotation.rotation_y_code << 6;
                }
                if (object->tail.initial_rotation.rotation_z_code != 0xff) {
                    object->rotation.vz =
                        object->tail.initial_rotation.rotation_z_code << 6;
                }
                if (object->tail.fields.unknown_39 == 0) {
                    object->action = KF_MAP_OBJECT_OP_16;
                    object->render_queue_mode = KF_RENDER_QUEUE_BLEND_ADD;
                    object->lighting_override_index = KF_LIGHTING_EFFECT;
                    object->lighting_blend_q12 = KF_FIXED12_ONE;
                    object->extra_40.bob_base_y = object->position.vy;
                }
            }
            break;
        case KF_MAP_OBJECT_OP_2:
            object->action = KF_MAP_OBJECT_OP_2;
            object->asset_clip_selector = 0;
            map_cell_apply_rotated_pattern(object->layer_mask, object->position.vx,
                          object->position.vz, object->rotation.vy,
                          map_object_cell_patterns[8], 0, 0xff);
            break;
        case KF_MAP_OBJECT_OP_HINGE:
            map_cell_copy_rotated_fields(object->layer_mask,
                          object->tail.cell_copy.source_x + 2,
                          object->tail.cell_copy.source_z,
                          object->tail.cell_copy.destination_x,
                          object->tail.cell_copy.destination_z,
                          2, 2, 0, 0x2d);
            object->action = KF_MAP_OBJECT_OP_HINGE;
            object->action_timer = 2;
            object->extra_40.hinge.progress_ticks = 999;
            object->extra_40.hinge.base_yaw = object->rotation.vy;
            map_cell_add_layer_occupancy(object->position.vx, object->position.vz, 3000, 1);
            break;
        case KF_MAP_OBJECT_OP_3: {
            object->asset_clip_selector = 0;
            map_cell_copy_rotated_fields(object->layer_mask,
                          object->tail.cell_copy.source_x +
                              object_template->params.marker.cell_width * 2,
                          object->tail.cell_copy.source_z,
                          object->tail.cell_copy.destination_x,
                          object->tail.cell_copy.destination_z,
                          object_template->params.marker.cell_width,
                          object_template->params.marker.cell_height,
                          object->rotation.vy, 0x2d);
            object->action = KF_MAP_OBJECT_OP_3;
            map_cell_add_layer_occupancy(object->position.vx, object->position.vz, 0x1130, 1);
            break;
        }
        case KF_MAP_OBJECT_OP_83:
            object->asset_clip_selector = 0;
            object->action = KF_MAP_OBJECT_OP_83;
            object->action_timer = 9;
            break;
        case KF_MAP_OBJECT_OP_8:
            object->action = KF_MAP_OBJECT_OP_8;
            break;
        case KF_MAP_OBJECT_OP_22:
            object->action = KF_MAP_OBJECT_OP_22;
            break;
        case KF_MAP_OBJECT_OP_5:
            object->action = KF_MAP_OBJECT_OP_5;
            object->asset_clip_selector = 0;
            map_object_set_cell_marker(object, KF_MAP_CELL_MARKER_PLACE, object_template->params.marker.marker_action_05);
            break;
        case KF_MAP_OBJECT_OP_RESOURCE_TRIGGER:
            object->layer_mask = KF_MAP_LAYER_NONE;
            object->action = KF_MAP_OBJECT_OP_RESOURCE_TRIGGER;
            object->position.vx -= placements->local_x;
            object->position.vz -= placements->local_z;
            object->position.vy -= placements->height;
            if (placements->local_x == 0xff) {
                object->extra_40.resource_offsets.offset_y = 0x7f;
                object->extra_40.resource_offsets.offset_z = 0x7f;
                object->extra_40.resource_offsets.offset_x = 0x7f;
            } else {
                object->extra_40.resource_offsets.offset_x =
                    (s8)placements->local_x - (s8)placements->region_x;
                object->extra_40.resource_offsets.offset_z =
                    (s8)placements->local_z - (s8)placements->region_z;
                object->extra_40.resource_offsets.offset_y =
                    (s8)placements->height -
                    (s8)(-object->position.vy >> 7);
            }
            break;
        case KF_MAP_OBJECT_OP_34:
            object->asset_clip_selector = 0;
            object->phase_q12 = 0;
            object->action = KF_MAP_OBJECT_OP_34;
            object->extra_40.bytes[0] = 0;
            break;
        case KF_MAP_OBJECT_OP_REGION_TRIGGER:
            object->layer_mask = KF_MAP_LAYER_NONE;
            object->action = KF_MAP_OBJECT_OP_REGION_TRIGGER;
            object->extra_40.bytes[0] = 0;
            break;
        case KF_MAP_OBJECT_OP_13:
            if (object->tail.initial_rotation.rotation_x_code != 0xff) {
                object->rotation.vx =
                    object->tail.initial_rotation.rotation_x_code << 6;
            }
            if (object->tail.initial_rotation.rotation_y_code != 0xff) {
                object->rotation.vy =
                    object->tail.initial_rotation.rotation_y_code << 6;
            }
            if (object->tail.initial_rotation.rotation_z_code != 0xff) {
                object->rotation.vz =
                    object->tail.initial_rotation.rotation_z_code << 6;
            }
            break;
        case KF_MAP_OBJECT_OP_AMBIENT_SOUND:
            object->layer_mask = KF_MAP_LAYER_NONE;
            object->action = KF_MAP_OBJECT_OP_AMBIENT_SOUND;
            object->extra_40.next_sound_frame = frame_count +
                object->tail.ambient_sound.repeat_delay_units * 6;
            break;
        case KF_MAP_OBJECT_OP_ANIMATED_MODEL:
            object->action = KF_MAP_OBJECT_OP_ANIMATED_MODEL;
            break;
        case KF_MAP_OBJECT_OP_84:
            object->asset_clip_selector = 0;
            object->extra_40.saved_layer.layer_mask = object->layer_mask;
            object->layer_mask = KF_MAP_LAYER_BOTH;
            object->action = KF_MAP_OBJECT_OP_84;
            map_cell_apply_rotated_pattern(object->extra_40.saved_layer.layer_mask, object->position.vx,
                          object->position.vz, object->rotation.vy,
                          map_object_cell_patterns[
                              object_template->params.pattern.pattern_pair_index * 2 +
                              (object->tail.action_84_pattern.pattern_flags & 1)],
                          0, 0);
            object->scale.vz = 0;
            object->scale.vy = 0;
            object->scale.vx = 0;
            break;
        case KF_MAP_OBJECT_OP_81:
            object->asset_clip_selector = 0;
            object->action = KF_MAP_OBJECT_OP_81;
            object->extra_40.bytes[0] = 0;
            if (object->tail.fields.unknown_3a.bytes.high != 0xff) {
                map_object_set_cell_marker(object, KF_MAP_CELL_MARKER_PLACE,
                                           object_template->params.marker.marker_action_51);
            }
            break;
        case KF_MAP_OBJECT_OP_88:
            object->asset_clip_selector = 0;
            object->action = KF_MAP_OBJECT_OP_88;
            object->action_timer = 1;
            break;
        case KF_MAP_OBJECT_OP_89: {
            KfMapOccupancyCell *kind59_row;
            KfMapOccupancyCell *kind59_cell;
            KfMapOccupancyLayer *kind59_layer;

            object->action = KF_MAP_OBJECT_OP_89;
            object->render_queue_mode = KF_RENDER_QUEUE_BLEND_ADD;
            object->lighting_override_index = KF_LIGHTING_PRESET_42;
            object->lighting_blend_q12 = KF_FIXED12_ONE;
            object->position.vy += 0x100;
            kind59_row = bss_801c7540.map_cells[object->position.vz >> 11];
            kind59_cell = &kind59_row[object->position.vx >> 11];
            kind59_layer = kind59_cell->layer;
            if (object->layer_mask != KF_MAP_LAYER_FIRST) {
                kind59_layer++;
            }
            kind59_layer->collision_shape_id = 0x75;
            object->extra_40.layer_fade.original_layer_mask = object->layer_mask;
            object->layer_mask = KF_MAP_LAYER_NONE;
            break;
        }
        case KF_MAP_OBJECT_OP_11:
        case KF_MAP_OBJECT_OP_20:
            object->layer_mask = KF_MAP_LAYER_NONE;
            break;
        case KF_MAP_OBJECT_OP_19:
            object->action = KF_MAP_OBJECT_OP_19;
            break;
        case KF_MAP_OBJECT_OP_48:
            object->action = KF_MAP_OBJECT_OP_48;
            object->render_queue_mode = KF_RENDER_QUEUE_BLEND_ADD;
            object->position.vy -=
                (u8)object->tail.fields.spawn_sequence * 0x100;
            break;
        case KF_MAP_OBJECT_OP_18:
            object->action = KF_MAP_OBJECT_OP_18;
            object->extra_40.next_sound_frame = frame_count + 30;
            break;
        case KF_MAP_OBJECT_OP_15:
            object->action = KF_MAP_OBJECT_OP_15;
            break;
        case KF_MAP_OBJECT_OP_17:
            object->action = KF_MAP_OBJECT_OP_17;
            break;
        case KF_MAP_OBJECT_OP_9:
            object->action = KF_MAP_OBJECT_OP_9;
            object->extra_40.saved_layer.layer_mask = object->layer_mask;
            break;
        case KF_MAP_OBJECT_OP_21:
            object->action = KF_MAP_OBJECT_OP_9;
            /* Fall through: this kind saves the original layer. */
        case KF_MAP_OBJECT_OP_226:
            object->extra_40.saved_layer.layer_mask = object->layer_mask;
            object->layer_mask = KF_MAP_LAYER_NONE;
            break;
        default:
            ((void (*)(KfMapObject *, const KfMapObjectTemplate *))
                 resource_state.active_table[8])(object, object_template);
            break;
        case KF_MAP_OBJECT_OP_33:
        case KF_MAP_OBJECT_OP_NONE:
            break;
        }
        if (object->object_id == KF_OBJECT_163) {
            object->render_queue_mode = KF_RENDER_QUEUE_BLEND_ADD_QUARTER;
        }
    }
}

ADDRESS(0x80036078, 0x118)
s32 map_object_find_collision_at_point(s32 x, s32 y, s32 z, s32 radius, s32 point_height)
{
    KfMapObject *object = map_object_state.objects;
    s32 index;

    for (index = 0; index < KF_MAP_OBJECT_CAPACITY; index++, object++) {
        if (object->object_id == KF_OBJECT_NONE) {
            continue;
        }
        if (object->layer_mask == KF_MAP_LAYER_NONE) {
            continue;
        }
        if (object == map_object_state.current_collision_object) {
            continue;
        }
        if (map_object_state.templates[KF_ENUM_ENCODE(u16, object->object_id)].collision_radius == 0) {
            continue;
        }
        if (vector_distance_to_point(
                &object->position, x, y, z,
                map_object_state.templates[KF_ENUM_ENCODE(u16, object->object_id)].collision_radius + radius,
                object->collision_height, point_height) != -1) {
            return index;
        }
    }
    return -1;
}

enum { MAP_OBJECT_HINGE_INTERACTION_FORWARD = 0x700 };

ADDRESS(0x80036190, 0x22c)
s32 map_object_find_interaction_target(s32 first_index, const VECTOR *position, s32 radius,
                   s32 point_height, s32 angle, s32 tolerance)
{
    KfMapObject *object;
    KfMapObjectTemplate *object_template;
    SVECTOR forward;
    VECTOR rotated;
    MATRIX rotation;
    s16 index;
    s32 direction;

    object = &map_object_state.objects[first_index];
    index = first_index;
    for (; index < KF_MAP_OBJECT_CAPACITY; index++, object++) {
        if (object->object_id == KF_OBJECT_NONE) {
            continue;
        }
        object_template = &map_object_state.templates[KF_ENUM_ENCODE(u16, object->object_id)];
        if (object_template->collision_kind == KF_MAP_OBJECT_OP_HINGE) {
            forward.vx = MAP_OBJECT_HINGE_INTERACTION_FORWARD;
            forward.vy = 0;
            forward.vz = 0;
            matrix_set_rotation_y(object->rotation.vy, &rotation);
            ApplyMatrix(&rotation, &forward, &rotated);
            rotated.vx += position->vx;
            rotated.vz += position->vz;
            if (vector_distance_to_point(&object->position,
                                         rotated.vx, position->vy, rotated.vz,
                                         object_template->interaction_radius + radius,
                                         object->collision_height, point_height) == -1) {
                continue;
            }
            direction = vector_xz_to_angle(
                object->position.vx - rotated.vx,
                object->position.vz - rotated.vz);
            if (!angle_within_tolerance(angle, direction, tolerance)) {
                continue;
            }
        } else {
            if (vector_distance_to_point(&object->position,
                                         position->vx, position->vy, position->vz,
                                         object_template->interaction_radius + radius,
                                         object_template->interaction_height, point_height) == -1) {
                continue;
            }
            if (!(object->collision_flags & KF_MAP_OBJECT_INTERACTION_ANY_ANGLE)) {
                direction = vector_xz_to_angle(
                    object->position.vx - position->vx,
                    object->position.vz - position->vz);
                if (!angle_within_tolerance(angle, direction, tolerance)) {
                    continue;
                }
            }
        }
        return index;
    }
    return -1;
}

ADDRESS(0x800363bc, 0x20)
void map_object_start_action_if_idle(KfMapObject *object, KfMapObjectOperation action)
{
    if (object->action == KF_MAP_OBJECT_OP_NONE) {
        object->action = action;
        object->action_timer = KF_MAP_OBJECT_ACTION_TIMER_INIT;
    }
}

ADDRESS(0x800363dc, 0x88)
KfMapObject *map_object_effect_pool_acquire(s32 first_index, s32 count, s32 sequence)
{
    KfMapObject *object = &map_object_state.objects[first_index];
    KfMapObject *oldest = NULL;
    s32 oldest_age = 0;
    s32 age;

    do {
        if (object->object_id == KF_OBJECT_NONE) {
            return object;
        }
        if (sequence != -1) {
            age = sequence - object->tail.fields.spawn_sequence;
            if (age < 0) {
                age += KF_MAP_OBJECT_SPAWN_SEQUENCE_MODULUS;
            }
            if (oldest_age < age) {
                oldest = object;
                oldest_age = age;
            }
        }
        object++;
    } while (--count != 0);
    return oldest;
}

ADDRESS(0x80036464, 0x174)
void map_object_spawn_effect(u8 source, KF_ENUM_PARAM(KfObjectId, u8) object_id, const VECTOR *position,
                             s32 height_offset)
{
    KfMapObject *object;
    KfMapObjectTemplate *object_template;

    if (source == KF_MAP_OBJECT_DROP_FROM_PLACEMENT) {
        object = map_object_effect_pool_acquire(KF_MAP_OBJECT_PLACEMENT_DROP_FIRST,
                                                KF_MAP_OBJECT_EFFECT_POOL_SIZE,
                                                map_object_state.placement_drop_sequence);
        object->tail.fields.spawn_sequence = map_object_state.placement_drop_sequence++;
        object->object_id = object_id;
    } else {
        object = map_object_effect_pool_acquire(KF_MAP_OBJECT_DEFINITION_DROP_FIRST,
                                                KF_MAP_OBJECT_EFFECT_POOL_SIZE,
                                                map_object_state.definition_drop_sequence);
        object->tail.fields.spawn_sequence = map_object_state.definition_drop_sequence++;
        object->object_id = object_id;
    }
    map_object_reset(object);
    object_template = &map_object_state.templates[KF_ENUM_ENCODE(u16, object->object_id)];
    object->position.vx = position->vx;
    object->position.vy = height_offset + position->vy;
    object->position.vz = position->vz;
    object->rotation.vy = rand() >> KF_RANDOM_ANGLE_SHIFT;

    switch (object_template->kind) {
    case 0x10:
    case 0x13:
    case 0x16:
        map_object_start_action_if_idle(object, KF_MAP_OBJECT_OP_FALL_AND_TIP);
        break;
    case 0x17:
        map_object_start_action_if_idle(object, KF_MAP_OBJECT_OP_FALL_AND_SPIN);
        break;
    case 0x11:
    case 0x12:
    case 0x14:
    case 0x15:
    case 0x18:
    case 0x19:
    case 0x20:
        map_object_start_action_if_idle(object, KF_MAP_OBJECT_OP_BOUNCE);
        if (object_id == KF_OBJECT_103) {
            object->rotation.pad = 0x400;
        } else {
            object->rotation.pad = 0;
        }
        break;
    }
    object->tail.motion.motion_velocity.value = 0;
    object->tail.fields.unknown_38 = 0xff;
}

enum { KF_MAP_OBJECT_SCATTER_RADIUS = 600 };



ADDRESS(0x800365d8, 0x124)
void map_object_spawn_scattered_effect(u16 effect_id, const VECTOR *origin,
                                       s32 height_offset)
{
    KfMapObject *object;
    u16 sequence;
    u16 angle;

    object = map_object_effect_pool_acquire(KF_MAP_OBJECT_SCATTER_POOL_FIRST,
                                            KF_MAP_OBJECT_EFFECT_POOL_SIZE,
                                            map_object_state.spawn_sequence_pool_15e);
    map_object_reset(object);
    sequence = map_object_state.spawn_sequence_pool_15e;
    map_object_state.spawn_sequence_pool_15e = sequence + 1;
    object->tail.scattered_effect.spawn_sequence = sequence;
    object->object_id = KF_OBJECT_70;
    object->tail.scattered_effect.effect_id = effect_id;
    angle = (u16)(rand() >> KF_RANDOM_ANGLE_SHIFT);
    object->position.vx = origin->vx +
        ((rsin(angle) * KF_MAP_OBJECT_SCATTER_RADIUS) >> 12);
    object->position.vy = origin->vy + height_offset;
    object->position.vz = origin->vz +
        ((rcos(angle) * KF_MAP_OBJECT_SCATTER_RADIUS) >> 12);
    object->rotation.vy = rand() >> KF_RANDOM_ANGLE_SHIFT;
    object->tail.fields.unknown_38 = 0xff;
    map_object_start_action_if_idle(object, KF_MAP_OBJECT_OP_BOUNCE);
    object->tail.motion.motion_velocity.signed_value = -120;
}

ADDRESS(0x800366fc, 0x1b8)
void map_object_apply_marker_signal(u8 identifier)
{
    KfMapObject *object = map_object_state.objects;
    u16 remaining;

    if (identifier == 0xff) {
        return;
    }
    remaining = KF_MAP_OBJECT_CAPACITY - 1;
    do {
        switch (object->action) {
        case KF_MAP_OBJECT_OP_80:
        case KF_MAP_OBJECT_OP_84:
        case KF_MAP_OBJECT_OP_95:
        case KF_MAP_OBJECT_OP_162:
        case KF_MAP_OBJECT_OP_163:
            if (object->tail.marker.marker_id == identifier) {
                object->tail.marker.marker_id = 0xff;
            }
            break;
        case KF_MAP_OBJECT_OP_88:
            if (object->tail.action_88_cell_copy.marker_id == identifier) {
                object->action_timer = 1;
                object->tail.action_88_cell_copy.transition_mode =
                    object->tail.action_88_cell_copy.transition_mode == 0;
            }
            break;
        case KF_MAP_OBJECT_OP_81:
            if (object->tail.action_51_marker.marker_id == identifier) {
                object->tail.collision_probe.marker_trigger_state =
                    object->tail.collision_probe.marker_trigger_state == 0 ? 0xff : 0;
            }
            break;
        case KF_MAP_OBJECT_OP_89:
            if (object->tail.action_89_layer_fade.marker_id == identifier) {
                object->action_timer = 1;
            }
            break;
        case KF_MAP_OBJECT_OP_2:
        case KF_MAP_OBJECT_OP_3:
        case KF_MAP_OBJECT_OP_HINGE:
            if ((u8)(identifier + 106) < 49) {
                if ((object->tail.marker.marker_id & 0xfe) == identifier) {
                    object->tail.marker.marker_id ^= 1;
                }
            } else {
                u8 marker = object->tail.marker.marker_id;

                if (marker == identifier) {
                    if (marker >= 200) {
                        object->tail.marker.marker_id = 0xff;
                    } else if (object->action_timer == 0) {
                        object->action_timer = 1;
                        if (marker >= 100) {
                            object->tail.marker.marker_id = 0xff;
                        }
                    }
                }
            }
            break;
        }
        object++;
    } while (remaining-- != 0);
}

ADDRESS(0x800368b4, 0x90)
s32 map_object_check_and_consume_marker(KfMapObject *object, s32 marker)
{
    switch (object->action) {
    case KF_MAP_OBJECT_OP_2:
    case KF_MAP_OBJECT_OP_3:
    case KF_MAP_OBJECT_OP_HINGE:
        if (object->action_timer != 0) {
            return 0;
        }
        /* Fall through to the active marker check. */
    case KF_MAP_OBJECT_OP_5:
    case KF_MAP_OBJECT_OP_8:
    case KF_MAP_OBJECT_OP_22:
        if (object->tail.marker.marker_id >= 0xfe) {
            return 2;
        }
        if (object->tail.marker.marker_id == marker) {
            object->tail.marker.marker_id = 0xff;
            return 1;
        }
        return 3;
    case KF_MAP_OBJECT_OP_15:
    case KF_MAP_OBJECT_OP_17:
        return 4;
    default:
        return 0;
    }
}

ADDRESS(0x80036944, 0x74)
void map_object_process_marker_all(u8 marker)
{
    KfMapObject *object = map_object_state.objects;
    u16 remaining;

    if (marker == 0xff) {
        return;
    }
    remaining = KF_MAP_OBJECT_CAPACITY - 1;
    do {
        map_object_check_and_consume_marker(object, marker);
        object++;
    } while (remaining-- != 0);
}

ADDRESS(0x800369b8, 0x120)
void map_object_sample_world_vertex(KfMapObject *object, s32 vertex_index, VECTOR *result)
{
    struct KfEulerAngles angles;
    SVECTOR vertex;

    animation_sample_vertex(
        KF_ENUM_ENCODE(u16, object->object_id) + 0x100, object->asset_clip_selector, object->phase_q12,
        vertex_index, &vertex);

    vertex.vx = (vertex.vx * object->scale.vx) >> 12;
    vertex.vy = (vertex.vy * object->scale.vy) >> 12;
    vertex.vz = (vertex.vz * object->scale.vz) >> 12;

    angles.x = object->rotation.vx;
    angles.y = object->rotation.vy + 0x800;
    angles.z = object->rotation.vz;
    vector_rotate_yxz(&angles, &vertex, result);

    addVector(result, &object->position);
}

ADDRESS(0x80036ad8, 0x90)
b32 player_camera_within_map_region(s32 x, s32 z, s32 width, s32 depth, s32 height)
{
    s32 camera_x = player_state.camera_position.vx >> 11;
    s32 camera_z = player_state.camera_position.vz >> 11;

    if (camera_x < x || camera_x >= x + width ||
        camera_z < z || camera_z >= z + depth) {
        goto outside;
    }
    if (height == KF_MAP_REGION_HEIGHT_ANY) {
        return KF_TRUE;
    }
    if (height + 2048 < player_state.camera_position.vy ||
        player_state.camera_position.vy < height - 3200) {
        goto outside;
    }
    return KF_TRUE;
outside:
    return KF_FALSE;
}

enum {
    KF_MAP_OBJECT_MOTION_COMPLETE_ACTION = 0x63,
    KF_MAP_OBJECT_MOTION_STEP = 204,
    KF_MAP_OBJECT_MOTION_LIMIT = 0xfff
};

ADDRESS(0x80036b68, 0x2bc)
b32 map_object_step_offset_motion(KfMapObject *source, KfMapObject *target,
                   SVECTOR *start_offset, SVECTOR *end_offset,
                   b32 brighten, s32 duration)
{
    VECTOR start_position;
    VECTOR end_position;
    s32 fraction;

    switch (source->action_timer) {
    case 0:
        if (source->tail.event_effect.pending_event_command == KF_OBJECT_NONE) {
            return KF_FALSE;
        }
        if (target->tail.fields.unknown_38 == 0) {
            source->extra_40.offset_motion.elapsed_frames = 0;
        } else {
            source->extra_40.offset_motion.elapsed_frames = duration - 1;
        }
        source->action_timer = 1;
        map_object_reset(target);
        target->action = KF_MAP_OBJECT_OP_OFFSET_MOTION;
        target->layer_mask = source->layer_mask;
        if (brighten) {
            target->asset_clip_selector = 0;
        } else {
            target->asset_clip_selector = KF_MAP_OBJECT_STATIC_OBJECT_ZERO;
        }
        vector_rotate_yxz((const struct KfEulerAngles *)&source->rotation,
                          start_offset, &target->position);
        addVector(&target->position, &source->position);
        target->rotation.vy = source->rotation.vy;
        target->extra_40.object_index = source - map_object_state.objects;
        break;
    case 1:
        break;
    default:
        return KF_FALSE;
    }

    if (brighten) {
        target->phase_q12 += KF_MAP_OBJECT_MOTION_STEP;
        if (target->phase_q12 >= 0x1000) {
            target->phase_q12 = KF_MAP_OBJECT_MOTION_LIMIT;
        }
    }
    vector_rotate_yxz((const struct KfEulerAngles *)&source->rotation,
                      start_offset, &start_position);
    vector_rotate_yxz((const struct KfEulerAngles *)&source->rotation,
                      end_offset, &end_position);
    source->extra_40.offset_motion.elapsed_frames++;
    fraction = ((s32)source->extra_40.offset_motion.elapsed_frames << 12) / duration;
    target->position.vx = source->position.vx +
        fixed_lerp_q12(start_position.vx, end_position.vx, fraction);
    target->position.vy = source->position.vy +
        fixed_lerp_q12(start_position.vy, end_position.vy, fraction);
    target->position.vz = source->position.vz +
        fixed_lerp_q12(start_position.vz, end_position.vz, fraction);
    if (source->extra_40.offset_motion.elapsed_frames >= duration) {
        if (brighten) {
            target->phase_q12 = KF_MAP_OBJECT_MOTION_LIMIT;
        }
        map_object_play_spatial_sound(target, 0x42);
        target->tail.fields.unknown_38 = 0xff;
        source->action_timer = KF_MAP_OBJECT_MOTION_COMPLETE_ACTION;
        return KF_TRUE;
    }
    return KF_FALSE;
}

DATA(0x80067890, 0x10e, ".data")
KfMapCellPattern map_object_cell_patterns
    [KF_MAP_OBJECT_PATTERN_GROUPS][KF_MAP_OBJECT_PATTERN_ROWS] = {
    {
        { {{0x01, 0x01, 0x14, 0x01}, {0x68, 0x69, 0xfe, 0xfe}}, 0, 0 },
        { {{0x01, 0x01, 0x14, 0x01}, {0x68, 0x69, 0xfe, 0xfe}}, -1, 0 },
        { {{0xff, 0xff, 0xff, 0xff}, {0xff, 0xff, 0xff, 0xff}}, 0, 0 },
    },
    {
        { {{0x01, 0x6b, 0x14, 0x01}, {0x68, 0x69, 0xfe, 0xfe}}, 0, 0 },
        { {{0x01, 0x6b, 0x14, 0x01}, {0x68, 0x69, 0xfe, 0xfe}}, -1, 0 },
        { {{0xff, 0xff, 0xff, 0xff}, {0xff, 0xff, 0xff, 0xff}}, 0, 0 },
    },
    {
        { {{0x01, 0x01, 0x15, 0x17}, {0x68, 0x69, 0xfe, 0xfe}}, 0, 0 },
        { {{0x01, 0x01, 0x15, 0x17}, {0x68, 0x69, 0xfe, 0xfe}}, -1, 0 },
        { {{0xff, 0xff, 0xff, 0xff}, {0xff, 0xff, 0xff, 0xff}}, 0, 0 },
    },
    {
        { {{0x01, 0x6b, 0x15, 0x17}, {0x68, 0x69, 0xfe, 0xfe}}, 0, 0 },
        { {{0x01, 0x6b, 0x15, 0x17}, {0x68, 0x69, 0xfe, 0xfe}}, -1, 0 },
        { {{0xff, 0xff, 0xff, 0xff}, {0xff, 0xff, 0xff, 0xff}}, 0, 0 },
    },
    {
        { {{0x00, 0x00, 0x18, 0x00}, {0xaa, 0xab, 0xfe, 0xfe}}, 0, 0 },
        { {{0x00, 0x00, 0x18, 0x00}, {0xaa, 0xab, 0xfe, 0xfe}}, -1, 0 },
        { {{0xff, 0xff, 0xff, 0xff}, {0xff, 0xff, 0xff, 0xff}}, 0, 0 },
    },
    {
        { {{0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}}, 0, 0 },
        { {{0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}}, 0, 0 },
        { {{0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}}, 0, 0 },
    },
    {
        { {{0x00, 0x00, 0x19, 0x16}, {0xaa, 0xab, 0xfe, 0xfe}}, 0, 0 },
        { {{0x00, 0x00, 0x19, 0x16}, {0xaa, 0xab, 0xfe, 0xfe}}, -1, 0 },
        { {{0xff, 0xff, 0xff, 0xff}, {0xff, 0xff, 0xff, 0xff}}, 0, 0 },
    },
    {
        { {{0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}}, 0, 0 },
        { {{0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}}, 0, 0 },
        { {{0x00, 0x00, 0x00, 0x00}, {0x00, 0x00, 0x00, 0x00}}, 0, 0 },
    },
    {
        { {{0x01, 0xfe, 0xff, 0xff}, {0x3c, 0xfe, 0xff, 0xff}}, 0, 0 },
        { {{0x01, 0xfe, 0xff, 0xff}, {0x3c, 0xfe, 0xff, 0xff}}, -1, 0 },
        { {{0xff, 0xff, 0xff, 0xff}, {0xff, 0xff, 0xff, 0xff}}, 0, 0 },
    },
};

DATA(0x801749d0, 0x8744, ".bss")
KfMapObjectStateGame map_object_state;
