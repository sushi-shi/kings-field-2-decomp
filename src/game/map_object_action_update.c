#include <kf/lib/address.h>
#include <kf/game/callback.h>
#include <kf/game/cd.h>
#include <kf/game/collision_cache.h>
#include <kf/game/audio.h>
#include <kf/game/event_state.h>
#include <kf/game/event_counter.h>
#include <kf/game/effect.h>
#include <kf/game/graphics.h>
#include <kf/game/map_cell.h>
#include <kf/game/map_cell_pattern.h>
#include <kf/game/map_object.h>
#include <kf/game/player.h>
#include <kf/game/resources.h>
#include <kf/lib/math.h>
#include <psyq/sdk.h>

DATA(0x8006d6e4, 0x8)
SVECTOR DAT_8006d6e4 = {0, -1424, 0, 0};
DATA(0x8006d6ec, 0x8)
SVECTOR DAT_8006d6ec = {0, -912, 0, 0};
DATA(0x8006d6f4, 0x8)
SVECTOR DAT_8006d6f4 = {0, -100, 300, 0};
DATA(0x8006d6fc, 0x8)
SVECTOR DAT_8006d6fc = {0, 0, 64, 0};

/* The three adjacent retail tables dispatch actions and subactions. */
RODATA(0x8001191c, 0x3bc)

ADDRESS(0x80036ed4, 0x1df4)
void func_80036ed4(void)
{
    KfMapObject *object = map_object_state.objects;
    s32 remaining = KF_MAP_OBJECT_CAPACITY;

    do {
        KfMapObjectTemplate *template;
        if (object->action != KF_MAP_OBJECT_ACTION_NONE) {
            map_object_state.current_collision_object = object;
            template = &map_object_state.templates[object->object_id];
            map_object_state.current_template = template;

            switch (object->action) {
        case 2:
            switch (object->action_timer) {
            case 1:
                if (object->unknown_0a == 0) {
                    map_object_play_spatial_sound(object, template->unknown_0d[2]);
                }
                object->unknown_0a += 72;
                if (object->unknown_0a == 0xc18) {
                    func_80034f90(object->unknown_00, object->position.vx,
                                  object->position.vz, object->rotation.vy,
                                  map_object_cell_patterns[8], 1, 0xff);
                }
                if (object->unknown_0a > 0xfff) {
                    object->unknown_0a = 0xfff;
                    object->action_timer = 2;
                }
                break;
            case 20:
                if (collision_query_world(object->position.vx, object->position.vy,
                                   object->position.vz, 0x700, 0xc80, 0xc0) == 0) {
                    func_80034f90(object->unknown_00, object->position.vx,
                                  object->position.vz, object->rotation.vy,
                                  map_object_cell_patterns[8], 0, 0xff);
                    object->action_timer = 21;
                    map_object_play_spatial_sound(object, template->unknown_0d[2]);
                }
                break;
            case 21:
                object->unknown_0a -= 72;
                if ((s16)object->unknown_0a <= 0) {
                    object->unknown_0a = 0;
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

        case 3:
            switch (object->action_timer) {
            case 0: {
                u8 phase_byte = object->tail.fields.unknown_38;
                if ((u8)(phase_byte + 0x6a) < 0x31 && (phase_byte & 1)) {
                    object->action_timer = 1;
                }
                break;
            }
            case 1:
                if (object->unknown_0a == 0) {
                    func_80035194(object->unknown_00,
                                  object->tail.fields.unknown_3a.bytes.high +
                                      template->unknown_0d[0],
                                  object->tail.spawn_bytes.spawn_sequence.low,
                                  object->tail.fields.unknown_39,
                                  object->tail.fields.unknown_3a.bytes.low,
                                  template->unknown_0d[0],
                                  template->unknown_0d[1], object->rotation.vy, 0x2d);
                    map_object_play_spatial_sound(object, template->unknown_0d[2]);
                }
                object->unknown_0a += 72;
                if (object->unknown_0a == 0xc18) {
                    func_80035194(object->unknown_00,
                                  object->tail.fields.unknown_3a.bytes.high,
                                  object->tail.spawn_bytes.spawn_sequence.low,
                                  object->tail.fields.unknown_39,
                                  object->tail.fields.unknown_3a.bytes.low,
                                  template->unknown_0d[0],
                                  template->unknown_0d[1], object->rotation.vy, 0x2d);
                }
                if (object->unknown_0a > 0xfff) {
                    object->unknown_0a = 0xfff;
                    object->action_timer = 2;
                }
                break;
            case 20: {
                u8 phase_byte = object->tail.fields.unknown_38;
                if (((u8)(phase_byte + 0x6a) > 0x30 || !(phase_byte & 1)) &&
                    collision_query_world(object->position.vx, object->position.vy,
                                   object->position.vz, 0x1130, 0xc80, 0xc0) == 0) {
                    object->action_timer = 21;
                    func_80035194(object->unknown_00,
                                  object->tail.fields.unknown_3a.bytes.high +
                                      template->unknown_0d[0],
                                  object->tail.spawn_bytes.spawn_sequence.low,
                                  object->tail.fields.unknown_39,
                                  object->tail.fields.unknown_3a.bytes.low,
                                  template->unknown_0d[0],
                                  template->unknown_0d[1], object->rotation.vy, 0x2d);
                    map_object_play_spatial_sound(object, template->unknown_0d[2]);
                }
                break;
            }
            case 21:
                object->unknown_0a -= 72;
                if ((s16)object->unknown_0a <= 0) {
                    object->unknown_0a = 0;
                    object->action_timer = 0;
                    func_80035194(object->unknown_00,
                                  object->tail.fields.unknown_3a.bytes.high +
                                      template->unknown_0d[0] * 2,
                                  object->tail.spawn_bytes.spawn_sequence.low,
                                  object->tail.fields.unknown_39,
                                  object->tail.fields.unknown_3a.bytes.low,
                                  template->unknown_0d[0],
                                  template->unknown_0d[1], object->rotation.vy, 0x2d);
                }
                break;
            default:
                object->action_timer++;
                break;
            }
            break;

        case 4:
            if (object->action_timer != 0) {
                KfMapObject *linked;
                s16 previous;
                if (object->collision_flags & 0x80) {
                    s32 bearing = vector_xz_to_angle(
                        player_state.camera_position.vx - object->position.vx,
                        player_state.camera_position.vz - object->position.vz);
                    if ((u32)((bearing - object->extra_40.halfwords[1]) & 0xfff) <= 0x800) {
                        object->unknown_0e = -200;
                    } else {
                        object->unknown_0e = 0xf0;
                    }
                }
                if (object->tail.spawn_bytes.spawn_sequence.high != 0xff) {
                    linked = &map_object_state.objects[object->tail.spawn_bytes.spawn_sequence.high];
                    if (linked->collision_flags & 0x80) {
                        s32 bearing = vector_xz_to_angle(
                            player_state.camera_position.vx - linked->position.vx,
                            player_state.camera_position.vz - linked->position.vz);
                        if ((u32)((bearing - object->extra_40.halfwords[1]) & 0xfff) <= 0x800) {
                            linked->unknown_0e = 0xf0;
                        } else {
                            linked->unknown_0e = -200;
                        }
                    }
                } else {
                    linked = 0;
                }
                if (object->action_timer == 1) {
                    object->action_timer = 2;
                    object->extra_40.halfwords[0] = 0;
                    map_object_play_spatial_sound(object, template->unknown_0d[2]);
                }
                previous = object->extra_40.halfwords[0]++;
                if (previous < 32) {
                    object->unknown_01 = 0x81;
                    object->rotation.vy += 32;
                    if (linked != 0) {
                        linked->unknown_01 = 0x81;
                        linked->rotation.vy -= 32;
                    }
                    if (previous == 24) {
                        func_80035194(object->unknown_00,
                                      object->tail.fields.unknown_3a.bytes.high,
                                      object->tail.spawn_bytes.spawn_sequence.low,
                                      object->tail.fields.unknown_39,
                                      object->tail.fields.unknown_3a.bytes.low,
                                      2, 2, 0, 0x2d);
                    } else if (previous == 31) {
                        object->extra_40.halfwords[0] = 0x118;
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
                            matrix_set_rotation_y((s16)object->extra_40.halfwords[1], &rotation);
                            ApplyMatrix(&rotation, &offset, &target);
                            target.vx += object->position.vx;
                            target.vz += object->position.vz;
                            if (collision_query_world(target.vx, object->position.vy, target.vz,
                                               3000, object->collision_height, 0xc0)) {
                                object->extra_40.halfwords[0] = 300;
                                break;
                            }
                            func_80035194(object->unknown_00,
                                          object->tail.fields.unknown_3a.bytes.high + 2,
                                          object->tail.spawn_bytes.spawn_sequence.low,
                                          object->tail.fields.unknown_39,
                                          object->tail.fields.unknown_3a.bytes.low,
                                          2, 2, 0, 0x2d);
                            map_object_play_spatial_sound(object, template->unknown_0d[2]);
                        }
                        object->rotation.vy -= 32;
                        if (linked != 0) {
                            linked->rotation.vy += 32;
                        }
                    } else {
                        object->action_timer = 0;
                        object->unknown_01 = 0x80;
                        object->unknown_0e = -50;
                        if (linked != 0) {
                            linked->unknown_01 = 0x80;
                            linked->unknown_0e = -50;
                        }
                    }
                }
            }
            break;

        case 8:
            switch (object->action_timer) {
            case 0:
                if (object->tail.fields.unknown_38 == 0xfe) {
                    object->rotation.vx = 0xa00;
                    object->action_timer = 3;
                } else {
                    map_object_set_property(object->tail.fields.unknown_3a.value, 0);
                    map_object_set_property(object->tail.fields.unknown_3a.value, 3, 0);
                    object->action_timer = 1;
                }
                break;
            case 1:
                if (object->tail.fields.unknown_38 == 0xfe) {
                    object->extra_40.signed_halfwords[0] = -16;
                    object->action_timer = 2;
                    map_object_set_property(object->tail.fields.unknown_3a.value,
                                            1, object->unknown_00);
                    map_object_play_spatial_sound(object, template->unknown_0d[2]);
                }
                break;
            case 2: {
                s32 velocity = angle_velocity_step(0xa00, object->rotation.vx,
                                                    object->extra_40.signed_halfwords[0], 8, 4);
                s32 angle = ((u16)object->rotation.vx + velocity) & 0xfff;
                object->extra_40.signed_halfwords[0] = velocity;
                object->rotation.vx = angle;
                if (angle < 0xc00) {
                    map_object_set_property(object->tail.fields.unknown_3a.value, 2);
                }
                if (object->extra_40.signed_halfwords[0] == 0 && object->rotation.vx == 0xa00) {
                    object->action_timer = 3;
                }
                break;
            }
            }
            break;

        case 22:
            switch (object->action_timer) {
            case 0:
                if (object->tail.fields.unknown_38 == 0xfe) {
                    struct KfVecXZi displacement;
                    object->rotation.vx = 0xd44;
                    angle_to_forward_xz(object->rotation.vy + 0x800, &displacement);
                    vector2i_scale_shift11(0xa0, &displacement);
                    object->position.vx += displacement.x;
                    object->position.vz += displacement.z;
                    object->action_timer = 3;
                } else {
                    map_object_set_property(object->tail.fields.unknown_3a.value, 0);
                    map_object_set_property(object->tail.fields.unknown_3a.value, 3, 0);
                    object->action_timer = 1;
                }
                break;
            case 1:
                if (object->tail.fields.unknown_38 == 0xfe) {
                    object->extra_40.halfwords[0] = 16;
                    object->action_timer = 2;
                    map_object_set_property(object->tail.fields.unknown_3a.value,
                                            1, object->unknown_00);
                    map_object_play_spatial_sound(object, template->unknown_0d[2]);
                }
                break;
            case 2: {
                struct KfVecXZi displacement;
                angle_to_forward_xz(object->rotation.vy + 0x800, &displacement);
                vector2i_scale_shift11(10, &displacement);
                object->position.vx += displacement.x;
                object->position.vz += displacement.z;
                if (--object->extra_40.halfwords[0] == 0) {
                    map_object_set_property(object->tail.fields.unknown_3a.value, 2);
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

        case 5:
            switch (object->action_timer) {
            case 0: {
                u16 linked_index = object->tail.fields.unknown_3a.value;
                if (linked_index != 0xffff) {
                    KfMapObject *linked = &map_object_state.objects[linked_index];
                    linked->unknown_0e += 200;
                }
                if (object->tail.fields.unknown_38 == 0xfe) {
                    map_object_set_cell_marker(object, 1, template->marker_action_05);
                    object->unknown_0a = 0xfff;
                    object->action_timer = 3;
                } else {
                    map_object_set_property(object->tail.fields.unknown_3a.value,
                                            0);
                    object->action_timer = 1;
                }
                break;
            }
            case 1:
                if (object->tail.fields.unknown_38 == 0xfe) {
                    object->action_timer = 2;
                    map_object_set_property(object->tail.fields.unknown_3a.value,
                                            1, object->unknown_00);
                    map_object_play_spatial_sound(object, template->unknown_0d[2]);
                    map_object_set_cell_marker(object, 1, template->marker_action_05);
                }
                break;
            case 2:
                object->unknown_0a += 128;
                if (object->unknown_0a >= 0xfff) {
                    object->unknown_0a = 0xfff;
                    map_object_set_property(object->tail.fields.unknown_3a.value, 2);
                    object->action_timer = 3;
                }
                break;
            }
            break;

        case 83:
            switch (object->action_timer) {
            case 1:
                map_object_play_spatial_sound(object, template->unknown_0d[2]);
                switch (object->tail.fields.unknown_38) {
                case 0:
                case 1:
                    object->action_timer = 2;
                    break;
                case 2:
                    object->action_timer = 2;
                    object->tail.fields.unknown_38 = 3;
                    break;
                case 3:
                    object->action_timer = 3;
                    object->tail.fields.unknown_38 = 2;
                    break;
                default:
                    break;
                }
                break;
            case 2:
                object->unknown_0a += 256;
                if (object->unknown_0a < 0x1000) {
                    break;
                }
                object->unknown_0a = 0xfff;
            action83_complete:
                map_object_apply_marker_signal(object->tail.fields.unknown_39);
                switch (object->tail.fields.unknown_38) {
                case 0:
                    object->action_timer = 99;
                    break;
                case 1:
                    map_object_play_spatial_sound(object, template->unknown_0d[2]);
                    object->action_timer = 4;
                    break;
                case 2:
                case 3:
                    object->action_timer = 0;
                    break;
                }
                break;
            case 3:
                object->unknown_0a -= 256;
                if ((s16)object->unknown_0a > 0) {
                    break;
                }
                object->unknown_0a = 0;
                goto action83_complete;
            case 4:
                object->unknown_0a -= 256;
                if ((s16)object->unknown_0a <= 0) {
                    object->action_timer = 99;
                }
                break;
            case 9:
                if (object->tail.fields.unknown_38 == 3) {
                    object->unknown_0a = 0xfff;
                }
                object->action_timer = 0;
                break;
            default:
                break;
            }
            break;

        case 88:
            switch (object->action_timer) {
            case 1:
                switch (object->tail.fields.unknown_38) {
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
                func_80035194(object->unknown_00,
                              object->tail.spawn_bytes.spawn_sequence.low +
                                  object->tail.fields.unknown_3e.bytes.low,
                              object->tail.spawn_bytes.spawn_sequence.high,
                              object->tail.fields.unknown_3a.bytes.low,
                              object->tail.fields.unknown_3a.bytes.high,
                              object->tail.fields.unknown_3e.bytes.low,
                              object->tail.fields.unknown_3e.bytes.high,
                              object->rotation.vy, 0x2d);
                break;
            case 2:
                object->unknown_0a -= 64;
                if ((s16)object->unknown_0a <= 0) {
                    object->unknown_0a = 0;
                    object->action_timer = 99;
                    func_80035194(object->unknown_00,
                                  object->tail.spawn_bytes.spawn_sequence.low,
                                  object->tail.spawn_bytes.spawn_sequence.high,
                                  object->tail.fields.unknown_3a.bytes.low,
                                  object->tail.fields.unknown_3a.bytes.high,
                                  object->tail.fields.unknown_3e.bytes.low,
                                  object->tail.fields.unknown_3e.bytes.high,
                                  object->rotation.vy, 0x2d);
                }
                break;
            case 3:
                object->unknown_0a += 64;
                if (object->unknown_0a >= 0x1000) {
                    object->unknown_0a = 0xfff;
                    object->action_timer = 99;
                }
                break;
            }
            break;

        case 89:
            switch (object->action_timer) {
            case 1:
                object->extra_40.halfwords[0] = object->tail.fields.unknown_3a.value;
                object->action_timer = 2;
                object->unknown_00 = object->extra_40.bytes[2];
                break;
            case 2: {
                u16 previous = object->extra_40.halfwords[0];
                object->extra_40.halfwords[0] = previous - 1;
                if (previous == 0) {
                    KfMapOccupancyCell *row =
                        bss_801c7540.map_cells[object->position.vz >> 11];
                    KfMapOccupancyCell *cell =
                        &row[object->position.vx >> 11];
                    KfMapOccupancyLayer *layer = &cell->layer[0];
                    object->action_timer = 3;
                    if (object->unknown_00 != 1) {
                        layer = &cell->layer[1];
                    }
                    layer->unknown_03 = 0x74;
                    map_object_play_spatial_sound(object, 0xe3);
                }
                break;
            }
            case 3:
                object->unknown_10 -= 256;
                if ((s16)object->unknown_10 <= 0) {
                    object->unknown_10 = 0;
                    object->action_timer = 4;
                    object->extra_40.halfwords[0] = object->tail.fields.spawn_sequence;
                }
                break;
            case 4: {
                u16 previous = object->extra_40.halfwords[0];
                object->extra_40.halfwords[0] = previous - 1;
                if (previous == 0) {
                    map_object_play_spatial_sound(object, 0xe3);
                    object->action_timer = 5;
                }
                break;
            }
            case 5:
                object->unknown_10 += 256;
                if (object->unknown_10 >= 0x1000) {
                    KfMapOccupancyCell *row =
                        bss_801c7540.map_cells[object->position.vz >> 11];
                    KfMapOccupancyCell *cell =
                        &row[object->position.vx >> 11];
                    KfMapOccupancyLayer *layer = &cell->layer[0];
                    object->unknown_10 = 0x1000;
                    object->action_timer = 0;
                    if (object->unknown_00 != 1) {
                        layer = &cell->layer[1];
                    }
                    layer->unknown_03 = 0x75;
                    object->unknown_00 = 0;
                }
                break;
            default:
                break;
            }
            break;

        case 16:
            object->rotation.vy += 128;
            object->position.vy = object->extra_40.raw +
                                  (rsin((s16)object->rotation.vy) >> 6);
            break;

        case 84:
            switch (object->action_timer) {
            case 0: {
                s32 depth = (-(s32)object->tail.spawn_bytes.spawn_sequence.high) * 128;
                s32 center_x = object->tail.fields.unknown_39;
                s32 width = object->tail.fields.unknown_3a.bytes.high;
                s32 center_z = object->tail.fields.unknown_3a.bytes.low;
                s32 height = object->tail.spawn_bytes.spawn_sequence.low;
                s32 source_x = center_x - ((width - 1) >> 1);
                s32 source_z = center_z - ((height - 1) >> 1);
                if (player_camera_within_map_region(source_x, source_z, width, height, depth) ||
                    object->tail.fields.unknown_38 == 0xff) {
                    func_80034f90(object->extra_40.bytes[0], object->position.vx,
                                  object->position.vz, object->rotation.vy,
                                  map_object_cell_patterns[
                                      template->unknown_0d[1] * 2 +
                                      (object->tail.fields.unknown_3e.bytes.low & 1)],
                                  1, 0x80);
                    object->action_timer = 1;
                    object->scale.vz = 0x1000;
                    object->scale.vy = 0x1000;
                    object->scale.vx = 0x1000;
                }
                break;
            }
            case 1:
                object->unknown_0a += 256;
                if (object->unknown_0a >= 0xfff) {
                    object->unknown_01 = 1;
                    object->unknown_0a = 0;
                    if (object->tail.fields.unknown_3e.bytes.low & 2) {
                        object->action_timer = 99;
                        object->tail.fields.unknown_38 = 0xff;
                    } else {
                        object->action_timer = 2;
                    }
                }
                break;
            case 32:
                object->unknown_0a += 128;
                if (object->unknown_0a >= 0xfff) {
                    object->unknown_01 = 0;
                    object->unknown_0a = 0;
                    object->action_timer = 0;
                    func_80034f90(object->extra_40.bytes[0], object->position.vx,
                                  object->position.vz, object->rotation.vy,
                                  map_object_cell_patterns[
                                      template->unknown_0d[1] * 2 +
                                      (object->tail.fields.unknown_3e.bytes.low & 1)],
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

        case 224:
            if (player_camera_within_map_region(object->position.vx >> 11,
                               object->position.vz >> 11,
                               object->tail.fields.unknown_38,
                               object->tail.fields.unknown_39,
                               object->position.vy)) {
                func_80016260(object->tail.fields.unknown_3a.bytes.low,
                              object->tail.fields.unknown_3a.bytes.high,
                              object->tail.spawn_bytes.spawn_sequence.low,
                              object->tail.spawn_bytes.spawn_sequence.high,
                              object->tail.fields.unknown_3e.bytes.low,
                              (s8)object->extra_40.bytes[0],
                              (s8)object->extra_40.bytes[1],
                              (s8)object->extra_40.bytes[2]);
            }
            break;

        case 9: {
            u16 linked_index = object->tail.fields.unknown_3a.value;
            if (linked_index != 0xffff) {
                KfMapObject *linked = &map_object_state.objects[linked_index];
                if (linked->object_id != 0xff) {
                    linked->unknown_00 = 0;
                    linked->tail.fields.unknown_38 = 0;
                }
            }
            object->action = KF_MAP_OBJECT_ACTION_NONE;
            break;
        }

        case 81: {
            s32 increment = object->tail.fields.unknown_3a.bytes.low * 4;

            switch (object->action_timer) {
            case 0:
                if (object->tail.fields.unknown_38 == 0) {
                    object->action_timer = 2;
                    object->unknown_0a = 0;
                    object->unknown_01 = 1;
                    object->unknown_0a = 0xfff;
                } else if (object->tail.fields.unknown_3a.bytes.high != 0xfe &&
                           (object->tail.fields.unknown_3a.bytes.high == 0xff ||
                            player_camera_within_map_region(object->position.vx >> 11,
                                            object->position.vz >> 11,
                                            object->tail.fields.unknown_3a.bytes.high,
                                            object->tail.spawn_bytes.spawn_sequence.low,
                                            object->position.vy))) {
                    object->action_timer = 1;
                    object->unknown_0a = 0;
                    object->unknown_01 = 0;
                    object->extra_40.bytes[0] = 0;
                    map_object_play_spatial_sound(object, template->unknown_0d[9]);
                }
                break;
            case 1:
                if (object->tail.fields.unknown_3a.bytes.high != 0xff) {
                    if (object->unknown_0a == 0) {
                        map_object_set_cell_marker(object, 1,
                                                   template->marker_action_51);
                        map_object_play_spatial_sound(object, template->unknown_0d[9]);
                    } else if (object->unknown_0a >= 1500 &&
                               object->unknown_0a < 1500 + increment) {
                        map_object_play_spatial_sound(object, 0x4e);
                    }
                }
                if (object->tail.fields.unknown_38 == 0 &&
                    object->unknown_0a >= 0x1000 - increment) {
                    object->action_timer = 2;
                    object->unknown_0a = 0;
                    object->unknown_01 = 1;
                    break;
                }
                object->unknown_0a += increment;
                if (object->unknown_0a >= 0xfff) {
                    if (object->tail.fields.unknown_3a.bytes.high != 0xff) {
                        object->unknown_0a = 0;
                        object->action_timer = 0;
                        map_object_set_cell_marker(object, 0,
                                                   template->marker_action_51);
                        break;
                    }
                    map_object_play_spatial_sound(object, template->unknown_0d[9]);
                    object->unknown_0a &= 0xfff;
                }
                {
                    const KfMapObjectTemplatePoseView *pose_template =
                        (const KfMapObjectTemplatePoseView *)template;
                    VECTOR position;
                    u16 vertex_index = (u16)pose_template->height_offset;
                    u16 reach;
                    u16 height;
                    s32 kind;
                    map_object_sample_world_vertex(object, vertex_index, &position);
                    reach = (u16)pose_template->depth_offset;
                    height = pose_template->unknown_10;
                    kind = collision_query_world(position.vx, position.vy, position.vz,
                                         reach, height, 0x90);
                    if (kind == 0) {
                        goto clear_action_trigger;
                    } else if (object->extra_40.bytes[0] == 0) {
                        object->extra_40.bytes[0] = 1;
                        effect_dispatch_magic_impact(kind, 0x20, 5000, 5,
                                      object->tail.fields.unknown_39,
                                      template->unknown_0d[5], template->unknown_0d[6],
                                      template->unknown_0d[7], template->unknown_0d[8],
                                      0, 0, 0, 0, 0, &position);
                    }
                }
                break;
            case 2:
                object->unknown_0a += 64;
                if (object->unknown_0a >= 0xfff) {
                    object->unknown_0a = 0xfff;
                    if (object->tail.fields.unknown_38 == 0xff) {
                        object->unknown_01 = 2;
                        object->unknown_0a = 0;
                        object->action_timer = 3;
                    }
                }
                break;
            case 3:
                object->unknown_0a += 64;
                if (object->unknown_0a >= 0xfff) {
                    object->action_timer = 0;
                }
                break;
            }
            break;
        }

        case 15: {
            KfMapObject *target = &map_object_state.objects[380 + object->tail.fields.unknown_39];
            if (object->action_timer != 0 && target->object_id == 0xff) {
                u32 sentinel_offset;
                switch (object->tail.fields.unknown_38) {
                case 0x72:
                    sentinel_offset = (u32)&((KfEventControlFields *)0)->sentinels.unknown_00;
                    break;
                case 0x73:
                    sentinel_offset = (u32)&((KfEventControlFields *)0)->sentinels.unknown_04;
                    break;
                case 0x74:
                    sentinel_offset = (u32)&((KfEventControlFields *)0)->sentinels.unknown_08;
                    break;
                default:
                    goto no_sentinel;
                }
                *(u16 *)&event_state.control.bytes[sentinel_offset] = 0xffff;
            no_sentinel:
                object->tail.fields.unknown_38 = 0xff;
                object->action_timer = 0;
            }
            map_object_step_offset_motion(object, target, &DAT_8006d6e4, &DAT_8006d6ec, 1, 32);
            break;
        }

        case 17: {
            KfMapObject *target = &map_object_state.objects[380 + object->tail.fields.unknown_39];
            if (object->action_timer != 0 && target->object_id == 0xff) {
                KfMapObject *linked = &map_object_state.objects[object->tail.fields.unknown_3a.bytes.high];
                object->tail.fields.unknown_38 = 0xff;
                object->action_timer = 0;
                linked->tail.fields.unknown_38 &= ~object->tail.fields.unknown_3a.bytes.low;
            }
            if (map_object_step_offset_motion(object, target, &DAT_8006d6f4,
                              &DAT_8006d6fc, 0, 20)) {
                KfMapObject *linked = &map_object_state.objects[object->tail.fields.unknown_3a.bytes.high];
                linked->tail.fields.unknown_38 |= object->tail.fields.unknown_3a.bytes.low;
            }
            break;
        }

        case 18:
            if ((s32)(object->extra_40.raw - cd_state.frame_count) < 0) {
                object->extra_40.raw = cd_state.frame_count + 30;
                map_object_play_spatial_sound(object, 0xee);
            }
            break;

        case 19: {
            KfMapObject *linked = &map_object_state.objects[object->tail.fields.unknown_3a.value];
            switch (object->action_timer) {
            case 0:
                map_object_sample_world_vertex(object, 2, &linked->position);
                if (linked->position.vy != object->position.vy) {
                    s16 scale;
                    linked->rotation = object->rotation;
                    if (object->tail.fields.unknown_38 == 0xff) {
                        goto start_action_19;
                    }
                    linked->object_id = 0x4c;
                    linked->action = KF_MAP_OBJECT_ACTION_NONE;
                    linked->position.vy += 300;
                    linked->unknown_00 = object->unknown_00;
                    linked->tail.fields.unknown_38 = 0;
                    scale = object->tail.fields.unknown_38 << 5;
                    linked->scale.vz = scale;
                    linked->scale.vy = scale;
                    linked->scale.vx = scale;
                    object->action_timer = 1;
                }
                break;
            case 1: {
                s32 chance = game_counter_bytes[0x4c];
                if (chance < 16 && rand() >= chance * 2048) {
                    s16 scale = (u16)linked->scale.vz + 1;
                    linked->scale.vz = scale;
                    linked->scale.vy = scale;
                    linked->scale.vx = scale;
                    object->tail.fields.unknown_38 = (u16)linked->scale.vz >> 5;
                    if (linked->scale.vx < 0x1000) {
                        break;
                    }
                    goto start_action_19;
                }
                break;
            }
        start_action_19:
            object->tail.fields.unknown_38 = 0xff;
            linked->tail.fields.unknown_38 = 0xff;
            map_object_start_action_if_idle(linked, 0x62);
            object->action_timer = 2;
            break;
            case 2:
                if (linked->object_id == KF_MAP_OBJECT_ID_NONE) {
                    object->tail.fields.unknown_38 = 0;
                }
                break;
            }
            break;
        }

        case 96:
            switch (object->action_timer) {
            case 0: {
                s32 floor_y = func_8002b604(object->position.vx, object->position.vy,
                                             object->position.vz, template->collision_radius,
                                             template->interaction_height);
                object->unknown_00 = KF_COLLISION_CACHE_LAYER == 0 ? 1 : 2;
                object->position.vy += object->tail.fields.unknown_3e.signed_value;
                object->tail.fields.unknown_3e.value += 20;
                if (object->position.vy >= floor_y) {
                    object->position.vy = floor_y;
                    object->tail.fields.unknown_3e.value = 16;
                    object->action_timer = 1;
                }
                break;
            }
            case 1:
                object->rotation.vz += object->tail.fields.unknown_3e.value;
                object->tail.fields.unknown_3e.value += 16;
                if (object->rotation.vz >= 0x400) {
                    object->rotation.vz = 0x400;
                    object->action_timer = 99;
                }
                break;
            }
            break;

        case 97:
            if (object->action_timer == 0) {
                s32 floor_y = func_8002b604(object->position.vx, object->position.vy,
                                             object->position.vz, template->collision_radius,
                                             template->interaction_height);
                object->unknown_00 = KF_COLLISION_CACHE_LAYER == 0 ? 1 : 2;
                object->position.vy += 20;
                object->rotation.vy = (object->rotation.vy + 0x100) & 0xfff;
                if (object->position.vy >= floor_y) {
                    object->position.vy = floor_y;
                    object->action_timer = 99;
                }
            }
            break;

        case 98:
            if (object->action_timer < 2) {
                s32 floor_y = func_8002b604(object->position.vx, object->position.vy,
                                             object->position.vz, template->collision_radius,
                                             template->interaction_height);
                s16 velocity;
                object->unknown_00 = KF_COLLISION_CACHE_LAYER == 0 ? 1 : 2;
                object->position.vy += object->tail.fields.unknown_3e.signed_value;
                effect_spawn_at_lower_bound(&object->position, 0x1000, 6000, 300);
                {
                    s32 base_angle = object->rotation.vx;
                    s32 angle = base_angle - 0xa0;
                    if (object->action_timer == 0) {
                        angle = base_angle + 0xa0;
                    }
                    object->rotation.vx = angle & 0xfff;
                }
                object->tail.fields.unknown_3e.value += 30;
                velocity = object->tail.fields.unknown_3e.signed_value;
                if (velocity >= 0 && object->position.vy >= floor_y) {
                    if (velocity < 80) {
                        object->position.vy = floor_y;
                        object->action_timer = 2;
                    } else {
                        object->tail.fields.unknown_3e.value = -(velocity >> 1);
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

        case 225:
            if (player_camera_within_map_region(object->position.vx >> 11,
                               object->position.vz >> 11,
                               object->tail.fields.unknown_38,
                               object->tail.fields.unknown_39,
                               object->position.vy)) {
                if (object->extra_40.bytes[0] == 0) {
                    if (!(object->tail.fields.unknown_3a.bytes.low & 0x80)) {
                        object->extra_40.bytes[0] = 1;
                    }
                    switch (object->tail.fields.unknown_3a.bytes.low & 0x0f) {
                    case 0:
                        ((void (*)(KfMapObject *))state_8017d118.active_table[3])(object);
                        break;
                    case 1:
                        map_object_apply_marker_signal(object->tail.fields.unknown_3a.bytes.high);
                        break;
                    case 2:
                        event_state.control.bytes[0x40 + object->tail.fields.unknown_3a.bytes.high] =
                            object->tail.spawn_bytes.spawn_sequence.low;
                        break;
                    }
                }
            } else {
            clear_action_trigger:
                object->extra_40.bytes[0] = 0;
            }
            break;

        case 34:
            if (player_camera_within_map_region(object->tail.fields.unknown_38,
                               object->tail.fields.unknown_39,
                               object->tail.fields.unknown_3a.bytes.low,
                               object->tail.fields.unknown_3a.bytes.high,
                               object->position.vy)) {
                if (object->extra_40.bytes[0] == 0) {
                    KfMapObject *candidate = map_object_state.objects;
                    s32 count = KF_MAP_OBJECT_CAPACITY;
                    audio_play_sound(0x14, 0x6e);
                    func_80036e24(1, 0, 0x1000, 0x100);
                    do {
                        if (candidate->action == 34) {
                            candidate->extra_40.bytes[0] = 1;
                        }
                        candidate++;
                    } while (--count != 0);
                    func_8002b73c(player_state.camera_position.vx,
                                   player_state.camera_position.vz, 800, -1);
                    player_state.camera_position.vx =
                        object->tail.spawn_bytes.spawn_sequence.low * 0x800 + 0x400;
                    player_state.camera_position.vz =
                        object->tail.spawn_bytes.spawn_sequence.high * 0x800 + 0x400;
                    player_state.unknown_128 =
                        object->tail.fields.unknown_3e.bytes.low == 1 ? 0 : 5;
                    player_state.camera_rotation_target.angles[1] =
                        -((u32)object->tail.fields.unknown_3e.bytes.high * 16);
                    player_sync_position_to_map();
                    player_state.camera_rotation.angles[1] =
                        player_state.camera_rotation_target.angles[1] +
                        player_state.unknown_100[1] +
                        player_state.unknown_108.components[1] +
                        player_state.unknown_110[1];
                    func_80036e24(1, 0x1000, 0, -0x100);
                    render_set_color_overlay(0xff, 0, 0, 0);
                }
            } else {
                object->extra_40.bytes[0] = 0;
            }
            object->unknown_0a = (object->unknown_0a + 64) & 0xfff;
            break;

        default:
            ((void (*)(void))state_8017d118.active_table[9])();
            break;
            }
        }
        object++;
    } while (--remaining != 0);

    map_object_state.current_collision_object = 0;
}
