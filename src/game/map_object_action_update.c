#include <kf/lib/address.h>
#include <kf/lib/null.h>
#include <kf/game/cd.h>
#include <kf/game/graphics.h>
#include <kf/game/player.h>
#include <kf/game/callback.h>
#include <kf/game/collision_cache.h>
#include <kf/game/audio.h>
#include <kf/game/event_state.h>
#include <kf/game/event_counter.h>
#include <kf/game/effect.h>
#include <kf/game/map_cell.h>
#include <kf/game/map_cell_pattern.h>
#include <kf/game/map_object.h>
#include <kf/game/resources.h>
#include <kf/lib/math.h>
#include <kf/lib/offsetof.h>
#include <psyq/sdk.h>
#include <psyq/libc.h>

DATA(0x8006d6e4, 0x8, ".sdata")
SVECTOR map_object_motion_action15_start_offset = {0, -1424, 0, 0};
DATA(0x8006d6ec, 0x8, ".sdata")
SVECTOR map_object_motion_action15_end_offset = {0, -912, 0, 0};
DATA(0x8006d6f4, 0x8, ".sdata")
SVECTOR map_object_motion_action17_start_offset = {0, -100, 300, 0};
DATA(0x8006d6fc, 0x8, ".sdata")
SVECTOR map_object_motion_action17_end_offset = {0, 0, 64, 0};

/* The three adjacent retail tables dispatch actions and subactions. */
RODATA(0x8001191c, 0x3bc)

enum {
    FRAME_COLOR_LEVELS = 256,
    FRAME_COLOR_MAX = FRAME_COLOR_LEVELS - 1,
    MAP_OBJECT_EVENT_TRIGGERED = 0xfe,
    MAP_OBJECT_RENDERED_PREVIOUS_FRAME = 0x80,
    MAP_OBJECT_CELL_PATTERN_SWITCH_PHASE = 0xc18,
    MAP_OBJECT_CELL_COPY_FIELDS = KF_MAP_CELL_COPY_OBJECT_INDEX |
                                  KF_MAP_CELL_COPY_ROTATED_ORIENTATION |
                                  KF_MAP_CELL_COPY_COLLISION_SHAPE |
                                  KF_MAP_CELL_COPY_LIGHTING_BIT_40
};

ADDRESS(0x80036e24, 0xb0)
void render_frames_with_color_overlay(s32 mode, s32 phase, s32 last_phase,
    s32 step)
{
    VECTOR position;
    SVECTOR angles;
    s32 end = last_phase;
    s32 increment = step;
    s32 level = phase;

    for (;;) {
        s32 brightness = (level * level) >> 16;

        if (brightness >= FRAME_COLOR_LEVELS) {
            brightness = FRAME_COLOR_MAX;
        }
        render_set_color_overlay(mode, brightness, brightness, brightness);
        cd_request_service_vab();
        cd_request_service_stream();
        player_get_camera_pose(&position, &angles);
        render_game_frame(&position, &angles);
        if (level == end) {
            break;
        }
        level += increment;
    }
}

ADDRESS(0x80036ed4, 0x1df4)
void map_object_update_actions(void)
{
    KfMapObject *object = map_object_state.objects;
    s32 remaining = KF_MAP_OBJECT_CAPACITY;

    do {
        KfMapObjectTemplate *object_template;
        if (object->action != KF_MAP_OBJECT_OP_NONE) {
            map_object_state.current_collision_object = object;
            object_template = &map_object_state.templates[KF_ENUM_ENCODE(u16, object->object_id)];
            map_object_state.current_template = object_template;

            switch (object->action) {
        case KF_MAP_OBJECT_OP_2:
            switch (object->action_timer) {
            case 1:
                if (object->phase_q12 == 0) {
                    map_object_play_spatial_sound(object, object_template->params.marker.sound_id);
                }
                object->phase_q12 += 72;
                if (object->phase_q12 == MAP_OBJECT_CELL_PATTERN_SWITCH_PHASE) {
                    map_cell_apply_rotated_pattern(object->layer_mask, object->position.vx,
                                  object->position.vz, object->rotation.vy,
                                  map_object_cell_patterns[8], 1, 0xff);
                }
                if (object->phase_q12 > 0xfff) {
                    object->phase_q12 = 0xfff;
                    object->action_timer = 2;
                }
                break;
            case 20:
                if (collision_query_world(object->position.vx, object->position.vy,
                                   object->position.vz, 0x700, 0xc80,
                                   KF_COLLISION_QUERY_ACTORS_INCLUDE_TYPE3 | KF_COLLISION_QUERY_PLAYER) == KF_COLLISION_HIT_NONE) {
                    map_cell_apply_rotated_pattern(object->layer_mask, object->position.vx,
                                  object->position.vz, object->rotation.vy,
                                  map_object_cell_patterns[8], 0, 0xff);
                    object->action_timer = 21;
                    map_object_play_spatial_sound(object, object_template->params.marker.sound_id);
                }
                break;
            case 21:
                object->phase_q12 -= 72;
                if ((s16)object->phase_q12 <= 0) {
                    object->phase_q12 = 0;
                    object->action_timer = 0;
                }
                break;
            case 0:
                break;
            default:
                object->action_timer++;
                break;
            }
            break;

        case KF_MAP_OBJECT_OP_3: {
            switch (object->action_timer) {
            case 0: {
                u8 phase_byte = object->tail.marker.marker_id;
                if ((u8)(phase_byte + 0x6a) < 0x31 && (phase_byte & 1)) {
                    object->action_timer = 1;
                }
                break;
            }
            case 1:
                if (object->phase_q12 == 0) {
                    map_cell_copy_rotated_fields(object->layer_mask,
                                  object->tail.cell_copy.source_x +
                                      object_template->params.marker.cell_width,
                                  object->tail.cell_copy.source_z,
                                  object->tail.cell_copy.destination_x,
                                  object->tail.cell_copy.destination_z,
                                  object_template->params.marker.cell_width,
                                  object_template->params.marker.cell_height, object->rotation.vy,
                                  MAP_OBJECT_CELL_COPY_FIELDS);
                    map_object_play_spatial_sound(object, object_template->params.marker.sound_id);
                }
                object->phase_q12 += 72;
                if (object->phase_q12 == MAP_OBJECT_CELL_PATTERN_SWITCH_PHASE) {
                    map_cell_copy_rotated_fields(object->layer_mask,
                                  object->tail.cell_copy.source_x,
                                  object->tail.cell_copy.source_z,
                                  object->tail.cell_copy.destination_x,
                                  object->tail.cell_copy.destination_z,
                                  object_template->params.marker.cell_width,
                                  object_template->params.marker.cell_height, object->rotation.vy,
                                  MAP_OBJECT_CELL_COPY_FIELDS);
                }
                if (object->phase_q12 > 0xfff) {
                    object->phase_q12 = 0xfff;
                    object->action_timer = 2;
                }
                break;
            case 20: {
                u8 phase_byte = object->tail.marker.marker_id;
                if (((u8)(phase_byte + 0x6a) > 0x30 || !(phase_byte & 1)) &&
                    collision_query_world(object->position.vx, object->position.vy,
                                   object->position.vz, 0x1130, 0xc80,
                                   KF_COLLISION_QUERY_ACTORS_INCLUDE_TYPE3 | KF_COLLISION_QUERY_PLAYER) == KF_COLLISION_HIT_NONE) {
                    object->action_timer = 21;
                    map_cell_copy_rotated_fields(object->layer_mask,
                                  object->tail.cell_copy.source_x +
                                      object_template->params.marker.cell_width,
                                  object->tail.cell_copy.source_z,
                                  object->tail.cell_copy.destination_x,
                                  object->tail.cell_copy.destination_z,
                                  object_template->params.marker.cell_width,
                                  object_template->params.marker.cell_height, object->rotation.vy,
                                  MAP_OBJECT_CELL_COPY_FIELDS);
                    map_object_play_spatial_sound(object, object_template->params.marker.sound_id);
                }
                break;
            }
            case 21:
                object->phase_q12 -= 72;
                if ((s16)object->phase_q12 <= 0) {
                    object->phase_q12 = 0;
                    object->action_timer = 0;
                    map_cell_copy_rotated_fields(object->layer_mask,
                                  object->tail.cell_copy.source_x +
                                      object_template->params.marker.cell_width * 2,
                                  object->tail.cell_copy.source_z,
                                  object->tail.cell_copy.destination_x,
                                  object->tail.cell_copy.destination_z,
                                  object_template->params.marker.cell_width,
                                  object_template->params.marker.cell_height, object->rotation.vy,
                                  MAP_OBJECT_CELL_COPY_FIELDS);
                }
                break;
            default:
                object->action_timer++;
                break;
            }
            break;
        }

        case KF_MAP_OBJECT_OP_HINGE:
            if (object->action_timer != 0) {
                KfMapObject *linked;
                s16 previous;
                if (object->collision_flags & MAP_OBJECT_RENDERED_PREVIOUS_FRAME) {
                    s32 bearing = vector_xz_to_angle(
                        player_state.camera_position.vx - object->position.vx,
                        player_state.camera_position.vz - object->position.vz);
                    if ((u32)((bearing - object->extra_40.hinge.base_yaw) & 0xfff) <= 0x800) {
                        object->render_depth_offset = -200;
                    } else {
                        object->render_depth_offset = 0xf0;
                    }
                }
                if (object->tail.cell_copy.linked_object_index != 0xff) {
                    linked = &map_object_state.objects[object->tail.cell_copy.linked_object_index];
                    if (linked->collision_flags & MAP_OBJECT_RENDERED_PREVIOUS_FRAME) {
                        s32 bearing = vector_xz_to_angle(
                            player_state.camera_position.vx - linked->position.vx,
                            player_state.camera_position.vz - linked->position.vz);
                        if ((u32)((bearing - object->extra_40.hinge.base_yaw) & 0xfff) <= 0x800) {
                            linked->render_depth_offset = 0xf0;
                        } else {
                            linked->render_depth_offset = -200;
                        }
                    }
                } else {
                    linked = NULL;
                }
                if (object->action_timer == 1) {
                    object->action_timer = 2;
                    object->extra_40.hinge.progress_ticks = 0;
                    map_object_play_spatial_sound(object, object_template->params.marker.sound_id);
                }
                previous = object->extra_40.hinge.progress_ticks++;
                if (previous < 32) {
                    object->asset_clip_selector = KF_ANIMATION_CLIP_STATIC_OBJECT_SECOND;
                    object->rotation.vy += 32;
                    if (linked != NULL) {
                        linked->asset_clip_selector = KF_ANIMATION_CLIP_STATIC_OBJECT_SECOND;
                        linked->rotation.vy -= 32;
                    }
                    if (previous == 24) {
                        map_cell_copy_rotated_fields(object->layer_mask,
                                      object->tail.cell_copy.source_x,
                                      object->tail.cell_copy.source_z,
                                      object->tail.cell_copy.destination_x,
                                      object->tail.cell_copy.destination_z,
                                      2, 2, 0, MAP_OBJECT_CELL_COPY_FIELDS);
                    } else if (previous == 31) {
                        object->extra_40.hinge.progress_ticks = 0x118;
                    }
                } else if (previous >= 300) {
                    if (previous < 332) {
                        if (previous == 300) {
                            SVECTOR offset;
                            VECTOR target;
                            MATRIX rotation;
                            offset.vx = -1792;
                            offset.vy = 0;
                            offset.vz = 0;
                            matrix_set_rotation_y((s16)object->extra_40.hinge.base_yaw, &rotation);
                            ApplyMatrix(&rotation, &offset, &target);
                            target.vx += object->position.vx;
                            target.vz += object->position.vz;
                            if (collision_query_world(target.vx, object->position.vy, target.vz,
                                               3000, object->collision_height,
                                               KF_COLLISION_QUERY_ACTORS_INCLUDE_TYPE3 | KF_COLLISION_QUERY_PLAYER) != KF_COLLISION_HIT_NONE) {
                                object->extra_40.hinge.progress_ticks = 300;
                                break;
                            }
                            map_cell_copy_rotated_fields(object->layer_mask,
                                          object->tail.cell_copy.source_x + 2,
                                          object->tail.cell_copy.source_z,
                                          object->tail.cell_copy.destination_x,
                                          object->tail.cell_copy.destination_z,
                                          2, 2, 0, MAP_OBJECT_CELL_COPY_FIELDS);
                            map_object_play_spatial_sound(object,
                                                          object_template->params.marker.sound_id);
                        }
                        object->rotation.vy -= 32;
                        if (linked != NULL) {
                            linked->rotation.vy += 32;
                        }
                    } else {
                        object->action_timer = 0;
                        object->asset_clip_selector = KF_ANIMATION_CLIP_STATIC_OBJECT_FIRST;
                        object->render_depth_offset = -50;
                        if (linked != NULL) {
                            linked->asset_clip_selector = KF_ANIMATION_CLIP_STATIC_OBJECT_FIRST;
                            linked->render_depth_offset = -50;
                        }
                    }
                }
            }
            break;

        case KF_MAP_OBJECT_OP_8:
            switch (object->action_timer) {
            case 0:
                if (object->tail.marker.marker_id == MAP_OBJECT_EVENT_TRIGGERED) {
                    object->rotation.vx = 0xa00;
                    object->action_timer = 3;
                } else {
                    map_object_set_property(object->tail.linked_property.linked_object_index, 0);
                    map_object_set_property(object->tail.linked_property.linked_object_index, 3, 0);
                    object->action_timer = 1;
                }
                break;
            case 1:
                if (object->tail.marker.marker_id == MAP_OBJECT_EVENT_TRIGGERED) {
                    object->extra_40.angular_velocity_x = -16;
                    object->action_timer = 2;
                    map_object_set_property(object->tail.linked_property.linked_object_index,
                                            1, object->layer_mask);
                    map_object_play_spatial_sound(object, object_template->params.marker.sound_id);
                }
                break;
            case 2: {
                s32 velocity = angle_velocity_step(0xa00, object->rotation.vx,
                                                    object->extra_40.angular_velocity_x, 8, 4);
                s32 angle = ((u16)object->rotation.vx + velocity) & 0xfff;
                object->extra_40.angular_velocity_x = velocity;
                object->rotation.vx = angle;
                if (angle < 0xc00) {
                    map_object_set_property(object->tail.linked_property.linked_object_index, 2);
                }
                if (object->extra_40.angular_velocity_x == 0 && object->rotation.vx == 0xa00) {
                    object->action_timer = 3;
                }
                break;
            }
            }
            break;

        case KF_MAP_OBJECT_OP_22:
            switch (object->action_timer) {
            case 0:
                if (object->tail.marker.marker_id == MAP_OBJECT_EVENT_TRIGGERED) {
                    struct KfVecXZi displacement;
                    object->rotation.vx = 0xd44;
                    angle_to_forward_xz(object->rotation.vy + 0x800, &displacement);
                    vector2i_scale_shift11(0xa0, &displacement);
                    object->position.vx += displacement.x;
                    object->position.vz += displacement.z;
                    object->action_timer = 3;
                } else {
                    map_object_set_property(object->tail.linked_property.linked_object_index, 0);
                    map_object_set_property(object->tail.linked_property.linked_object_index, 3, 0);
                    object->action_timer = 1;
                }
                break;
            case 1:
                if (object->tail.marker.marker_id == MAP_OBJECT_EVENT_TRIGGERED) {
                    object->extra_40.movement_frames_left = 16;
                    object->action_timer = 2;
                    map_object_set_property(object->tail.linked_property.linked_object_index,
                                            1, object->layer_mask);
                    map_object_play_spatial_sound(object, object_template->params.marker.sound_id);
                }
                break;
            case 2: {
                struct KfVecXZi displacement;
                angle_to_forward_xz(object->rotation.vy + 0x800, &displacement);
                vector2i_scale_shift11(10, &displacement);
                object->position.vx += displacement.x;
                object->position.vz += displacement.z;
                if (--object->extra_40.movement_frames_left == 0) {
                    map_object_set_property(object->tail.linked_property.linked_object_index, 2);
                    object->action_timer = 3;
                }
                break;
            }
            case 3:
                object->rotation.vx = angle_approach(object->rotation.vx, 0xd44, 0x32);
                if (object->rotation.vx == 0xd44) {
                    object->action_timer = 4;
                }
                break;
            }
            break;

        case KF_MAP_OBJECT_OP_5:
            switch (object->action_timer) {
            case 0: {
                u16 linked_index = object->tail.linked_property.linked_object_index;
                if (linked_index != KF_MAP_OBJECT_INDEX_NONE) {
                    KfMapObject *linked = &map_object_state.objects[linked_index];
                    linked->render_depth_offset += 200;
                }
                if (object->tail.marker.marker_id == MAP_OBJECT_EVENT_TRIGGERED) {
                    map_object_set_cell_marker(object, 1,
                                               object_template->params.marker.marker_action_05);
                    object->phase_q12 = 0xfff;
                    object->action_timer = 3;
                } else {
                    map_object_set_property(object->tail.linked_property.linked_object_index,
                                            0);
                    object->action_timer = 1;
                }
                break;
            }
            case 1:
                if (object->tail.marker.marker_id == MAP_OBJECT_EVENT_TRIGGERED) {
                    object->action_timer = 2;
                    map_object_set_property(object->tail.linked_property.linked_object_index,
                                            1, object->layer_mask);
                    map_object_play_spatial_sound(object, object_template->params.marker.sound_id);
                    map_object_set_cell_marker(object, 1,
                                               object_template->params.marker.marker_action_05);
                }
                break;
            case 2:
                object->phase_q12 += 128;
                if (object->phase_q12 >= 0xfff) {
                    object->phase_q12 = 0xfff;
                    map_object_set_property(object->tail.linked_property.linked_object_index, 2);
                    object->action_timer = 3;
                }
                break;
            }
            break;

        case KF_MAP_OBJECT_OP_83:
            switch (object->action_timer) {
            case 1:
                map_object_play_spatial_sound(object, object_template->params.marker.sound_id);
                switch (object->tail.action_83.transition_mode) {
                case 0:
                case 1:
                    object->action_timer = 2;
                    break;
                case 2:
                    object->action_timer = 2;
                    object->tail.action_83.transition_mode = 3;
                    break;
                case 3:
                    object->action_timer = 3;
                    object->tail.action_83.transition_mode = 2;
                    break;
                default:
                    break;
                }
                break;
            case 2:
                object->phase_q12 += 256;
                if (object->phase_q12 < 0x1000) {
                    break;
                }
                object->phase_q12 = 0xfff;
            action83_complete:
                map_object_apply_marker_signal(object->tail.action_83.completion_marker);
                switch (object->tail.action_83.transition_mode) {
                case 0:
                    object->action_timer = 99;
                    break;
                case 1:
                    map_object_play_spatial_sound(object, object_template->params.marker.sound_id);
                    object->action_timer = 4;
                    break;
                case 2:
                case 3:
                    object->action_timer = 0;
                    break;
                }
                break;
            case 3:
                object->phase_q12 -= 256;
                if ((s16)object->phase_q12 > 0) {
                    break;
                }
                object->phase_q12 = 0;
                goto action83_complete;
            case 4:
                object->phase_q12 -= 256;
                if ((s16)object->phase_q12 <= 0) {
                    object->action_timer = 99;
                }
                break;
            case 9:
                if (object->tail.action_83.transition_mode == 3) {
                    object->phase_q12 = 0xfff;
                }
                object->action_timer = 0;
                break;
            default:
                break;
            }
            break;

        case KF_MAP_OBJECT_OP_88:
            switch (object->action_timer) {
            case 1:
                switch (object->tail.action_88_cell_copy.transition_mode) {
                case 0:
                    object->action_timer = 2;
                    map_object_play_spatial_sound(object, 0x44);
                    break;
                case 1:
                    object->action_timer = 3;
                    map_object_play_spatial_sound(object, 0x44);
                    break;
                default:
                    break;
                }
                map_cell_copy_rotated_fields(object->layer_mask,
                              object->tail.action_88_cell_copy.source_x +
                                  object->tail.action_88_cell_copy.width,
                              object->tail.action_88_cell_copy.source_z,
                              object->tail.action_88_cell_copy.destination_x,
                              object->tail.action_88_cell_copy.destination_z,
                              object->tail.action_88_cell_copy.width,
                              object->tail.action_88_cell_copy.height,
                              object->rotation.vy, MAP_OBJECT_CELL_COPY_FIELDS);
                break;
            case 2:
                object->phase_q12 -= 64;
                if ((s16)object->phase_q12 <= 0) {
                    object->phase_q12 = 0;
                    object->action_timer = 99;
                    map_cell_copy_rotated_fields(object->layer_mask,
                                  object->tail.action_88_cell_copy.source_x,
                                  object->tail.action_88_cell_copy.source_z,
                                  object->tail.action_88_cell_copy.destination_x,
                                  object->tail.action_88_cell_copy.destination_z,
                                  object->tail.action_88_cell_copy.width,
                                  object->tail.action_88_cell_copy.height,
                                  object->rotation.vy, MAP_OBJECT_CELL_COPY_FIELDS);
                }
                break;
            case 3:
                object->phase_q12 += 64;
                if (object->phase_q12 >= 0x1000) {
                    object->phase_q12 = 0xfff;
                    object->action_timer = 99;
                }
                break;
            }
            break;

        case KF_MAP_OBJECT_OP_89:
            switch (object->action_timer) {
            case 1:
                object->extra_40.layer_fade.delay_frames_left = object->tail.action_89_layer_fade.delay_frames;
                object->action_timer = 2;
                object->layer_mask = object->extra_40.layer_fade.original_layer_mask;
                break;
            case 2: {
                u16 previous = object->extra_40.layer_fade.delay_frames_left;
                object->extra_40.layer_fade.delay_frames_left = previous - 1;
                if (previous == 0) {
                    KfMapOccupancyCell *row =
                        bss_801c7540.map_cells[object->position.vz >> 11];
                    KfMapOccupancyCell *cell =
                        &row[object->position.vx >> 11];
                    KfMapOccupancyLayer *layer = &cell->layer[0];
                    object->action_timer = 3;
                    if (object->layer_mask != 1) {
                        layer = &cell->layer[1];
                    }
                    layer->collision_shape_id = 0x74;
                    map_object_play_spatial_sound(object, 0xe3);
                }
                break;
            }
            case 3:
                object->lighting_blend_q12 -= 256;
                if ((s16)object->lighting_blend_q12 <= 0) {
                    object->lighting_blend_q12 = 0;
                    object->action_timer = 4;
                    object->extra_40.layer_fade.delay_frames_left = object->tail.fields.spawn_sequence;
                }
                break;
            case 4: {
                u16 previous = object->extra_40.layer_fade.delay_frames_left;
                object->extra_40.layer_fade.delay_frames_left = previous - 1;
                if (previous == 0) {
                    map_object_play_spatial_sound(object, 0xe3);
                    object->action_timer = 5;
                }
                break;
            }
            case 5:
                object->lighting_blend_q12 += 256;
                if (object->lighting_blend_q12 >= 0x1000) {
                    KfMapOccupancyCell *row =
                        bss_801c7540.map_cells[object->position.vz >> 11];
                    KfMapOccupancyCell *cell =
                        &row[object->position.vx >> 11];
                    KfMapOccupancyLayer *layer = &cell->layer[0];
                    object->lighting_blend_q12 = 0x1000;
                    object->action_timer = 0;
                    if (object->layer_mask != 1) {
                        layer = &cell->layer[1];
                    }
                    layer->collision_shape_id = 0x75;
                    object->layer_mask = 0;
                }
                break;
            default:
                break;
            }
            break;

        case KF_MAP_OBJECT_OP_16:
            object->rotation.vy += 128;
            object->position.vy = object->extra_40.bob_base_y +
                                  (rsin(object->rotation.vy) >> 6);
            break;

        case KF_MAP_OBJECT_OP_84:
            switch (object->action_timer) {
            case 0: {
                s32 depth = (-(s32)object->tail.action_84_pattern.depth_code) * 128;
                s32 center_x = object->tail.action_84_pattern.center_x;
                s32 width = object->tail.action_84_pattern.region_width;
                s32 center_z = object->tail.action_84_pattern.center_z;
                s32 height = object->tail.action_84_pattern.region_depth;
                s32 source_x = center_x - ((width - 1) >> 1);
                s32 source_z = center_z - ((height - 1) >> 1);
                if (player_camera_within_map_region(source_x, source_z, width, height, depth) ||
                    object->tail.action_84_pattern.marker_id == 0xff) {
                    map_cell_apply_rotated_pattern(object->extra_40.saved_layer.layer_mask, object->position.vx,
                                  object->position.vz, object->rotation.vy,
                                  map_object_cell_patterns[
                                      object_template->params.pattern.pattern_pair_index * 2 +
                                      (object->tail.action_84_pattern.pattern_flags & 1)],
                                  1, 0x80);
                    object->action_timer = 1;
                    object->scale.vz = 0x1000;
                    object->scale.vy = 0x1000;
                    object->scale.vx = 0x1000;
                }
                break;
            }
            case 1:
                object->phase_q12 += 256;
                if (object->phase_q12 >= 0xfff) {
                    object->asset_clip_selector = KF_ANIMATION_CLIP_SECOND;
                    object->phase_q12 = 0;
                    if (object->tail.action_84_pattern.pattern_flags & 2) {
                        object->action_timer = 99;
                        object->tail.action_84_pattern.marker_id = 0xff;
                    } else {
                        object->action_timer = 2;
                    }
                }
                break;
            case 32:
                object->phase_q12 += 128;
                if (object->phase_q12 >= 0xfff) {
                    object->asset_clip_selector = KF_ANIMATION_CLIP_FIRST;
                    object->phase_q12 = 0;
                    object->action_timer = 0;
                    map_cell_apply_rotated_pattern(object->extra_40.saved_layer.layer_mask, object->position.vx,
                                  object->position.vz, object->rotation.vy,
                                  map_object_cell_patterns[
                                      object_template->params.pattern.pattern_pair_index * 2 +
                                      (object->tail.action_84_pattern.pattern_flags & 1)],
                                  0, 0);
                    object->scale.vz = 0;
                    object->scale.vy = 0;
                    object->scale.vx = 0;
                }
                break;
            case 99:
                break;
            default:
                object->action_timer++;
                break;
            }
            break;

        case KF_MAP_OBJECT_OP_RESOURCE_TRIGGER:
            if (player_camera_within_map_region(object->position.vx >> 11,
                               object->position.vz >> 11,
                               object->tail.resource_trigger.region_width,
                               object->tail.resource_trigger.region_depth,
                               object->position.vy)) {
                resource_request_transition(object->tail.resource_trigger.resource_selectors[0],
                              object->tail.resource_trigger.resource_selectors[1],
                              object->tail.resource_trigger.resource_selectors[2],
                              object->tail.resource_trigger.resource_selectors[3],
                              object->tail.resource_trigger.resource_selectors[4],
                              object->extra_40.resource_offsets.offset_x,
                              object->extra_40.resource_offsets.offset_z,
                              object->extra_40.resource_offsets.offset_y);
            }
            break;

        case KF_MAP_OBJECT_OP_9: {
            u16 linked_index = object->tail.linked_property.linked_object_index;
            if (linked_index != KF_MAP_OBJECT_INDEX_NONE) {
                KfMapObject *linked = &map_object_state.objects[linked_index];
                if (linked->object_id != KF_OBJECT_NONE) {
                    linked->layer_mask = 0;
                    linked->tail.fields.unknown_38 = 0;
                }
            }
            object->action = KF_MAP_OBJECT_OP_NONE;
            break;
        }

        case KF_MAP_OBJECT_OP_81: {
            s32 increment = object->tail.collision_probe.phase_step_code * 4;

            switch (object->action_timer) {
            case 0:
                if (object->tail.collision_probe.marker_trigger_state == 0) {
                    object->action_timer = 2;
                    object->phase_q12 = 0;
                    object->asset_clip_selector = KF_ANIMATION_CLIP_SECOND;
                    object->phase_q12 = 0xfff;
                } else if (object->tail.collision_probe.camera_region_width != 0xfe &&
                           (object->tail.collision_probe.camera_region_width == 0xff ||
                            player_camera_within_map_region(object->position.vx >> 11,
                                            object->position.vz >> 11,
                                            object->tail.collision_probe.camera_region_width,
                                            object->tail.collision_probe.camera_region_depth,
                                            object->position.vy))) {
                    object->action_timer = 1;
                    object->phase_q12 = 0;
                    object->asset_clip_selector = KF_ANIMATION_CLIP_FIRST;
                    object->extra_40.bytes[0] = 0;
                    map_object_play_spatial_sound(object,
                                                  object_template->params.collision.sound_id);
                }
                break;
            case 1:
                if (object->tail.collision_probe.camera_region_width != 0xff) {
                    if (object->phase_q12 == 0) {
                        map_object_set_cell_marker(object, 1,
                            object_template->params.collision.marker_action_51);
                        map_object_play_spatial_sound(object,
                                                      object_template->params.collision.sound_id);
                    } else if (object->phase_q12 >= 1500 &&
                               object->phase_q12 < 1500 + increment) {
                        map_object_play_spatial_sound(object, 0x4e);
                    }
                }
                if (object->tail.collision_probe.marker_trigger_state == 0 &&
                    object->phase_q12 >= 0x1000 - increment) {
                    object->action_timer = 2;
                    object->phase_q12 = 0;
                    object->asset_clip_selector = KF_ANIMATION_CLIP_SECOND;
                    break;
                }
                object->phase_q12 += increment;
                if (object->phase_q12 >= 0xfff) {
                    if (object->tail.collision_probe.camera_region_width != 0xff) {
                        object->phase_q12 = 0;
                        object->action_timer = 0;
                        map_object_set_cell_marker(object, 0,
                            object_template->params.collision.marker_action_51);
                        break;
                    }
                    map_object_play_spatial_sound(object,
                                                  object_template->params.collision.sound_id);
                    object->phase_q12 &= 0xfff;
                }
                {
                    VECTOR position;
                    u16 vertex_index = object_template->params.collision.vertex_index;
                    u16 reach;
                    u16 height;
                    KF_ENUM_PARAM(KfCollisionHitFlags, s32) kind;
                    map_object_sample_world_vertex(object, vertex_index, &position);
                    reach = object_template->params.collision.reach;
                    height = object_template->params.collision.height;
                    kind = collision_query_world(position.vx, position.vy, position.vz,
                                         reach, height,
                                         KF_COLLISION_QUERY_ACTORS | KF_COLLISION_QUERY_PLAYER);
                    if (kind == KF_COLLISION_HIT_NONE) {
                        goto clear_action_trigger;
                    }
                    if (object->extra_40.bytes[0] == 0) {
                        object->extra_40.bytes[0] = 1;
                        effect_dispatch_magic_impact(kind, KF_ACTOR_DAMAGE_FROM_HAZARD, 5000, 5,
                                      object->tail.collision_probe.damage_multiplier_tenths,
                                      object_template->params.collision.impact_magic_values[0],
                                      object_template->params.collision.impact_magic_values[1],
                                      object_template->params.collision.impact_magic_values[2],
                                      object_template->params.collision.impact_magic_values[3],
                                      0, 0, 0, 0, 0, &position);
                    }
                }
                break;
            case 2:
                object->phase_q12 += 64;
                if (object->phase_q12 >= 0xfff) {
                    object->phase_q12 = 0xfff;
                    if (object->tail.collision_probe.marker_trigger_state == 0xff) {
                        object->asset_clip_selector = KF_ANIMATION_CLIP_THIRD;
                        object->phase_q12 = 0;
                        object->action_timer = 3;
                    }
                }
                break;
            case 3:
                object->phase_q12 += 64;
                if (object->phase_q12 >= 0xfff) {
                    object->action_timer = 0;
                }
                break;
            }
            break;
        }

        case KF_MAP_OBJECT_OP_15: {
            KfMapObject *target = &map_object_state.objects[380 + object->tail.event_effect.effect_object_index];
            if (object->action_timer != 0 && target->object_id == KF_OBJECT_NONE) {
                u32 sentinel_offset;
                switch (object->tail.event_effect.pending_event_command) {
                case KF_OBJECT_114:
                    sentinel_offset = offsetof(KfEventControlFields, object_slots[0].object_index);
                    break;
                case KF_OBJECT_115:
                    sentinel_offset = offsetof(KfEventControlFields, object_slots[1].object_index);
                    break;
                case KF_OBJECT_116:
                    sentinel_offset = offsetof(KfEventControlFields, object_slots[2].object_index);
                    break;
                default:
                    goto no_sentinel;
                }
                ((KfEventControlObjectSlot *)&event_state.control.bytes[sentinel_offset])->object_index = 0xffff;
            no_sentinel:
                object->tail.event_effect.pending_event_command = KF_OBJECT_NONE;
                object->action_timer = 0;
            }
            map_object_step_offset_motion(object, target, &map_object_motion_action15_start_offset, &map_object_motion_action15_end_offset, KF_TRUE, 32);
            break;
        }

        case KF_MAP_OBJECT_OP_17: {
            KfMapObject *target = &map_object_state.objects[380 + object->tail.event_effect.effect_object_index];
            if (object->action_timer != 0 && target->object_id == KF_OBJECT_NONE) {
                KfMapObject *linked = &map_object_state.objects[object->tail.event_effect.linked_object_index];
                object->tail.event_effect.pending_event_command = KF_OBJECT_NONE;
                object->action_timer = 0;
                linked->tail.fields.unknown_38 &= ~object->tail.event_effect.linked_object_flag_mask;
            }
            if (map_object_step_offset_motion(object, target, &map_object_motion_action17_start_offset,
                              &map_object_motion_action17_end_offset, KF_FALSE, 20)) {
                KfMapObject *linked = &map_object_state.objects[object->tail.event_effect.linked_object_index];
                linked->tail.fields.unknown_38 |= object->tail.event_effect.linked_object_flag_mask;
            }
            break;
        }

        case KF_MAP_OBJECT_OP_18:
            if ((s32)(object->extra_40.next_sound_frame - cd_state.frame_count) < 0) {
                object->extra_40.next_sound_frame = cd_state.frame_count + 30;
                map_object_play_spatial_sound(object, 0xee);
            }
            break;

        case KF_MAP_OBJECT_OP_19: {
            KfMapObject *linked = &map_object_state.objects[object->tail.scale_link.linked_object_index];
            switch (object->action_timer) {
            case 0:
                map_object_sample_world_vertex(object, 2, &linked->position);
                if (linked->position.vy != object->position.vy) {
                    s16 scale;
                    linked->rotation = object->rotation;
                    if (object->tail.scale_link.scale_step_code == 0xff) {
                        goto start_action_19;
                    }
                    linked->object_id = KF_ITEM_FULL_RESTORE;
                    linked->action = KF_MAP_OBJECT_OP_NONE;
                    linked->position.vy += 300;
                    linked->layer_mask = object->layer_mask;
                    linked->tail.fields.unknown_38 = 0;
                    scale = object->tail.scale_link.scale_step_code << 5;
                    linked->scale.vz = scale;
                    linked->scale.vy = scale;
                    linked->scale.vx = scale;
                    object->action_timer = 1;
                }
                break;
            case 1: {
                s32 chance = game_counter_bytes[KF_ENUM_ENCODE(u8, KF_ITEM_FULL_RESTORE)];
                if (chance < 16 && rand() >= chance * 2048) {
                    s16 scale = linked->scale.vz + 1;
                    linked->scale.vz = scale;
                    linked->scale.vy = scale;
                    linked->scale.vx = scale;
                    object->tail.scale_link.scale_step_code = (u16)linked->scale.vz >> 5;
                    if (linked->scale.vx < 0x1000) {
                        break;
                    }
                    goto start_action_19;
                }
                break;
            }
        start_action_19:
            object->tail.scale_link.scale_step_code = 0xff;
            linked->tail.fields.unknown_38 = 0xff;
            map_object_start_action_if_idle(linked, KF_MAP_OBJECT_OP_BOUNCE);
            object->action_timer = 2;
            break;
            case 2:
                if (linked->object_id == KF_OBJECT_NONE) {
                    object->tail.scale_link.scale_step_code = 0;
                }
                break;
            }
            break;
        }

        case KF_MAP_OBJECT_OP_FALL_AND_TIP:
            switch (object->action_timer) {
            case 0: {
                s32 floor_y = collision_probe_floor_height(object->position.vx, object->position.vy,
                                             object->position.vz, object_template->collision_radius,
                                             object_template->interaction_height);
                object->layer_mask = KF_COLLISION_CACHE_LAYER == 0 ? 1 : 2;
                object->position.vy += object->tail.motion.motion_velocity.signed_value;
                object->tail.motion.motion_velocity.value += 20;
                if (object->position.vy >= floor_y) {
                    object->position.vy = floor_y;
                    object->tail.motion.motion_velocity.value = 16;
                    object->action_timer = 1;
                }
                break;
            }
            case 1:
                object->rotation.vz += object->tail.motion.motion_velocity.value;
                object->tail.motion.motion_velocity.value += 16;
                if (object->rotation.vz >= 0x400) {
                    object->rotation.vz = 0x400;
                    object->action_timer = 99;
                }
                break;
            }
            break;

        case KF_MAP_OBJECT_OP_FALL_AND_SPIN:
            if (object->action_timer == 0) {
                s32 floor_y = collision_probe_floor_height(object->position.vx, object->position.vy,
                                             object->position.vz, object_template->collision_radius,
                                             object_template->interaction_height);
                object->layer_mask = KF_COLLISION_CACHE_LAYER == 0 ? 1 : 2;
                object->position.vy += 20;
                object->rotation.vy = (object->rotation.vy + 0x100) & KF_ANGLE_WRAP_MASK;
                if (object->position.vy >= floor_y) {
                    object->position.vy = floor_y;
                    object->action_timer = 99;
                }
            }
            break;

        case KF_MAP_OBJECT_OP_BOUNCE:
            if (object->action_timer < 2) {
                s32 floor_y = collision_probe_floor_height(object->position.vx, object->position.vy,
                                             object->position.vz, object_template->collision_radius,
                                             object_template->interaction_height);
                s16 velocity;
                object->layer_mask = KF_COLLISION_CACHE_LAYER == 0 ? 1 : 2;
                object->position.vy += object->tail.motion.motion_velocity.signed_value;
                effect_spawn_at_lower_bound(&object->position, KF_FIXED12_ONE, 6000, 300);
                object->rotation.vx = (object->rotation.vx +
                    (object->action_timer == 0 ? 0xa0 : -0xa0)) & KF_ANGLE_WRAP_MASK;
                object->tail.motion.motion_velocity.value += 30;
                velocity = object->tail.motion.motion_velocity.signed_value;
                if (velocity >= 0 && object->position.vy >= floor_y) {
                    if (velocity < 80) {
                        object->position.vy = floor_y;
                        object->action_timer = 2;
                    } else {
                        object->tail.motion.motion_velocity.value = -(velocity >> 1);
                        object->action_timer = object->action_timer == 0 ? 1 : 0;
                    }
                }
            } else if (object->action_timer == 2) {
                object->rotation.vx = angle_approach(object->rotation.vx,
                                                     object->rotation.pad, 0xa0);
                if (object->rotation.vx == object->rotation.pad) {
                    object->action_timer = 99;
                }
            }
            break;

        case KF_MAP_OBJECT_OP_REGION_TRIGGER:
            if (player_camera_within_map_region(object->position.vx >> 11,
                               object->position.vz >> 11,
                               object->tail.region_action.region_width,
                               object->tail.region_action.region_depth,
                               object->position.vy)) {
                if (object->extra_40.bytes[0] == 0) {
                    if (!(object->tail.region_action.operation_flags & 0x80)) {
                        object->extra_40.bytes[0] = 1;
                    }
                    switch (object->tail.region_action.operation_flags & 0x0f) {
                    case 0:
                        ((void (*)(KfMapObject *))resource_state.active_table[3])(object);
                        break;
                    case 1:
                        map_object_apply_marker_signal(object->tail.region_action.operand);
                        break;
                    case 2:
                        event_state.control.bytes[0x40 + object->tail.region_action.operand] =
                            object->tail.region_action.assigned_value;
                        break;
                    }
                }
            } else {
            clear_action_trigger:
                object->extra_40.bytes[0] = 0;
            }
            break;

        case KF_MAP_OBJECT_OP_34:
            if (player_camera_within_map_region(object->tail.transition.region_x,
                               object->tail.transition.region_z,
                               object->tail.transition.region_width,
                               object->tail.transition.region_depth,
                               object->position.vy)) {
                if (object->extra_40.bytes[0] == 0) {
                    KfMapObject *candidate = map_object_state.objects;
                    s32 count = KF_MAP_OBJECT_CAPACITY;
                    audio_play_sound(0x14, 0x6e);
                    render_frames_with_color_overlay(1, 0, 0x1000, 0x100);
                    do {
                        if (candidate->action == KF_MAP_OBJECT_OP_34) {
                            candidate->extra_40.bytes[0] = 1;
                        }
                        candidate++;
                    } while (--count != 0);
                    map_cell_add_layer_occupancy(player_state.camera_position.vx,
                                   player_state.camera_position.vz, 800, -1);
                    player_state.camera_position.vx =
                        object->tail.transition.destination_cell_x * 0x800 + 0x400;
                    player_state.camera_position.vz =
                        object->tail.transition.destination_cell_z * 0x800 + 0x400;
                    player_state.map_layer_index =
                        object->tail.transition.destination_layer_code == 1 ? 0 : 5;
                    player_state.camera_rotation_target.angles[1] =
                        -((u32)object->tail.transition.destination_yaw_code * 16);
                    player_sync_position_to_map();
                    player_state.camera_rotation.angles[1] =
                        player_state.camera_rotation_target.angles[1] +
                        player_state.reaction_rotation_offset[1] +
                        player_state.view_rotation_offset.components[1] +
                        player_state.camera_yaw_roll_offsets[0];
                    render_frames_with_color_overlay(1, 0x1000, 0, -0x100);
                    render_set_color_overlay(0xff, 0, 0, 0);
                }
            } else {
                object->extra_40.bytes[0] = 0;
            }
            object->phase_q12 = (object->phase_q12 + 64) & 0xfff;
            break;

        default:
            ((void (*)(void))resource_state.active_table[9])();
            break;
            }
        }
        object++;
    } while (--remaining != 0);

    map_object_state.current_collision_object = NULL;
}
