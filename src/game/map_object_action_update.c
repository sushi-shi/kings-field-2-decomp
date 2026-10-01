#include <kf/lib/address.h>
#include <kf/game/callback.h>
#include <kf/game/cd.h>
#include <kf/game/audio.h>
#include <kf/game/event_state.h>
#include <kf/game/graphics.h>
#include <kf/game/map_cell.h>
#include <kf/game/map_cell_pattern.h>
#include <kf/game/map_object.h>
#include <kf/game/player.h>
#include <kf/lib/math.h>
#include <psyq/sdk.h>

extern KfMapCellPattern map_object_cell_patterns[9][3];
extern void func_80034f90(s32 mode, s32 world_x, s32 world_z, s32 angle,
                          const KfMapCellPattern *patterns, s32 variant_index,
                          s32 layer_flag);
extern void func_80035194(u32 layer_select, s32 source_x, s32 source_z,
                          s32 destination_x, s32 destination_z, s32 width,
                          s32 height, s32 rotation, u32 field_mask);
extern s32 func_8002b9d4(s32 x, s32 y, s32 z, s32 radius, s32 height, s32 mode);
extern s32 func_80036ad8(s32 x, s32 z, s32 width, s32 depth, s32 height);
extern void func_80016260(u8 first, u8 second, u8 third, u8 fourth,
                           u8 fifth, s8 offset_x, s8 offset_z, s8 offset_y);
extern s32 func_80036b68(KfMapObject *source, KfMapObject *target,
                          SVECTOR *start_offset, SVECTOR *end_offset,
                          s32 brighten, s32 duration);
extern void func_800366fc(u8 identifier);
extern void func_80036e24(s32 mode, s32 phase, s32 last_phase, s32 step);
extern SVECTOR DAT_8006d6e4[4];

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
            if (object->action_timer == 1) {
                if (object->unknown_0a == 0) {
                    map_object_play_spatial_sound(object, template->unknown_0d[2]);
                }
                object->unknown_0a += 72;
                if (object->unknown_0a == 0xc18) {
                    func_80034f90(object->unknown_00, object->position.vx,
                                  object->position.vz, object->rotation.pad,
                                  map_object_cell_patterns[8], 1, 0xff);
                }
                if (object->unknown_0a > 0xfff) {
                    object->unknown_0a = 0xfff;
                    object->action_timer = 2;
                }
            } else if (object->action_timer == 20) {
                if (func_8002b9d4(object->position.vx, object->position.vy,
                                   object->position.vz, 0x700, 0xc80, 0xc0) == 0) {
                    func_80034f90(object->unknown_00, object->position.vx,
                                  object->position.vz, object->rotation.pad,
                                  map_object_cell_patterns[8], 0, 0xff);
                    object->action_timer = 21;
                    map_object_play_spatial_sound(object, template->unknown_0d[2]);
                }
            } else if (object->action_timer == 21) {
                object->unknown_0a -= 72;
                if ((s16)object->unknown_0a <= 0) {
                    object->unknown_0a = 0;
                    object->action_timer = 0;
                }
            } else if (object->action_timer != 0) {
                object->action_timer++;
            }
            break;

        case 3:
            if (object->action_timer == 1) {
                if (object->unknown_0a == 0) {
                    func_80035194(object->unknown_00,
                                  object->tail.fields.unknown_3a.bytes.high +
                                      template->unknown_0d[0],
                                  (s8)object->tail.fields.spawn_sequence,
                                  object->tail.fields.unknown_39,
                                  (s8)object->tail.fields.unknown_3a.bytes.low,
                                  template->unknown_0d[0],
                                  template->unknown_0d[1], object->rotation.pad, 0x2d);
                    map_object_play_spatial_sound(object, template->unknown_0d[2]);
                }
                object->unknown_0a += 72;
                if (object->unknown_0a == 0xc18) {
                    func_80035194(object->unknown_00,
                                  object->tail.fields.unknown_3a.bytes.high,
                                  (s8)object->tail.fields.spawn_sequence,
                                  object->tail.fields.unknown_39,
                                  (s8)object->tail.fields.unknown_3a.bytes.low,
                                  template->unknown_0d[0],
                                  template->unknown_0d[1], object->rotation.pad, 0x2d);
                }
                if (object->unknown_0a > 0xfff) {
                    object->unknown_0a = 0xfff;
                    object->action_timer = 2;
                }
            } else if (object->action_timer == 0) {
                u8 phase_byte = object->tail.fields.unknown_38;
                if ((u8)(phase_byte + 0x6a) < 0x31 && (phase_byte & 1)) {
                    object->action_timer = 1;
                }
            } else if (object->action_timer == 20) {
                u8 phase_byte = object->tail.fields.unknown_38;
                if (((u8)(phase_byte + 0x6a) > 0x30 || !(phase_byte & 1)) &&
                    func_8002b9d4(object->position.vx, object->position.vy,
                                   object->position.vz, 0x1130, 0xc80, 0xc0) == 0) {
                    object->action_timer = 21;
                    func_80035194(object->unknown_00,
                                  object->tail.fields.unknown_3a.bytes.high +
                                      template->unknown_0d[0],
                                  (s8)object->tail.fields.spawn_sequence,
                                  object->tail.fields.unknown_39,
                                  (s8)object->tail.fields.unknown_3a.bytes.low,
                                  template->unknown_0d[0],
                                  template->unknown_0d[1], object->rotation.pad, 0x2d);
                    map_object_play_spatial_sound(object, template->unknown_0d[2]);
                }
            } else if (object->action_timer == 21) {
                object->unknown_0a -= 72;
                if ((s16)object->unknown_0a <= 0) {
                    object->unknown_0a = 0;
                    object->action_timer = 0;
                    func_80035194(object->unknown_00,
                                  object->tail.fields.unknown_3a.bytes.high +
                                      template->unknown_0d[0] * 2,
                                  (s8)object->tail.fields.spawn_sequence,
                                  object->tail.fields.unknown_39,
                                  (s8)object->tail.fields.unknown_3a.bytes.low,
                                  template->unknown_0d[0],
                                  template->unknown_0d[1], object->rotation.pad, 0x2d);
                }
            } else {
                object->action_timer++;
            }
            break;

        case 5:
            if (object->action_timer == 0) {
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
                    map_object_set_property(linked_index, 0);
                    object->action_timer = 1;
                }
            } else if (object->action_timer == 1) {
                if (object->tail.fields.unknown_38 == 0xfe) {
                    object->action_timer = 2;
                    map_object_set_property(object->tail.fields.unknown_3a.value,
                                            1, object->unknown_00);
                    map_object_play_spatial_sound(object, template->unknown_0d[2]);
                    map_object_set_cell_marker(object, 1, template->marker_action_05);
                }
            } else if (object->action_timer == 2) {
                object->unknown_0a += 128;
                if (object->unknown_0a >= 0xfff) {
                    object->unknown_0a = 0xfff;
                    map_object_set_property(object->tail.fields.unknown_3a.value, 2);
                    object->action_timer = 3;
                }
            }
            break;

        case 8:
            if (object->action_timer == 0) {
                if (object->tail.fields.unknown_38 == 0xfe) {
                    object->rotation.vx = 0xa00;
                    object->action_timer = 3;
                } else {
                    map_object_set_property(object->tail.fields.unknown_3a.value, 0);
                    map_object_set_property(object->tail.fields.unknown_3a.value, 3, 0);
                    object->action_timer = 1;
                }
            } else if (object->action_timer == 1) {
                if (object->tail.fields.unknown_38 == 0xfe) {
                    object->extra_40.halfwords[0] = (u16)-16;
                    object->action_timer = 2;
                    map_object_set_property(object->tail.fields.unknown_3a.value,
                                            1, object->unknown_00);
                    map_object_play_spatial_sound(object, template->unknown_0d[2]);
                }
            } else if (object->action_timer == 2) {
                s32 velocity = angle_velocity_step(0xa00, object->rotation.vx,
                                                    (s16)object->extra_40.halfwords[0], 8, 4);
                object->extra_40.halfwords[0] = velocity;
                object->rotation.vx = (object->rotation.vx + velocity) & 0xfff;
                if ((u16)object->rotation.vx < 0xc00) {
                    map_object_set_property(object->tail.fields.unknown_3a.value, 2);
                }
                if ((s16)object->extra_40.halfwords[0] == 0 && object->rotation.vx == 0xa00) {
                    object->action_timer = 3;
                }
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

        case 15: {
            KfMapObject *target = &map_object_state.objects[380 + object->tail.fields.unknown_39];
            if (object->action_timer != 0 && target->object_id == 0xff) {
                switch (object->tail.fields.unknown_38) {
                case 0x72:
                    event_state.control.fields.sentinels.unknown_00 = 0xffff;
                    break;
                case 0x73:
                    event_state.control.fields.sentinels.unknown_04 = 0xffff;
                    break;
                case 0x74:
                    event_state.control.fields.sentinels.unknown_08 = 0xffff;
                    break;
                }
                object->tail.fields.unknown_38 = 0xff;
                object->action_timer = 0;
            }
            func_80036b68(object, target, &DAT_8006d6e4[0], &DAT_8006d6e4[1], 1, 32);
            break;
        }

        case 16:
            object->rotation.pad += 128;
            object->position.vy = object->extra_40.raw +
                                  (rsin((s16)object->rotation.pad) >> 6);
            break;

        case 17: {
            KfMapObject *target = &map_object_state.objects[380 + object->tail.fields.unknown_39];
            KfMapObject *linked = &map_object_state.objects[object->tail.fields.unknown_3a.bytes.high];
            u8 marker_mask = object->tail.fields.unknown_3a.bytes.low;
            if (object->action_timer != 0 && target->object_id == 0xff) {
                object->tail.fields.unknown_38 = 0xff;
                object->action_timer = 0;
                linked->tail.fields.unknown_38 &= ~marker_mask;
            }
            if (func_80036b68(object, target, &DAT_8006d6e4[2],
                              &DAT_8006d6e4[3], 0, 20)) {
                linked->tail.fields.unknown_38 |= marker_mask;
            }
            break;
        }

        case 18:
            if ((s32)(object->extra_40.raw - cd_state.frame_count) < 0) {
                object->extra_40.raw = cd_state.frame_count + 30;
                map_object_play_spatial_sound(object, 0xee);
            }
            break;

        case 22:
            if (object->action_timer == 0) {
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
            } else if (object->action_timer == 1) {
                if (object->tail.fields.unknown_38 == 0xfe) {
                    object->extra_40.halfwords[0] = 16;
                    object->action_timer = 2;
                    map_object_set_property(object->tail.fields.unknown_3a.value,
                                            1, object->unknown_00);
                    map_object_play_spatial_sound(object, template->unknown_0d[2]);
                }
            } else if (object->action_timer == 2) {
                struct KfVecXZi displacement;
                angle_to_forward_xz(object->rotation.vy + 0x800, &displacement);
                vector2i_scale_shift11(10, &displacement);
                object->position.vx += displacement.x;
                object->position.vz += displacement.z;
                object->extra_40.halfwords[0]--;
                if (object->extra_40.halfwords[0] == 0) {
                    map_object_set_property(object->tail.fields.unknown_3a.value, 2);
                    object->action_timer = 3;
                }
            } else if (object->action_timer == 3) {
                object->rotation.vx = angle_approach(object->rotation.vx, 0xd44, 0x32);
                if (object->rotation.vx == 0xd44) {
                    object->action_timer = 4;
                }
            }
            break;

        case 34:
            if (!func_80036ad8(object->tail.fields.unknown_38,
                                object->tail.fields.unknown_39,
                                object->tail.fields.unknown_3a.bytes.low,
                                object->tail.fields.unknown_3a.bytes.high,
                                object->position.vy)) {
                object->extra_40.bytes[0] = 0;
            } else if (object->extra_40.bytes[0] == 0) {
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
                    (u8)object->tail.fields.spawn_sequence * 0x800 + 0x400;
                player_state.camera_position.vz =
                    (u8)(object->tail.fields.spawn_sequence >> 8) * 0x800 + 0x400;
                player_state.unknown_128 =
                    object->tail.fields.unknown_3e.bytes.low == 1 ? 0 : 5;
                player_state.camera_rotation_target.angles[1] =
                    -(u8)object->tail.fields.unknown_3e.bytes.high * 16;
                player_sync_position_to_map();
                player_state.camera_rotation.angles[1] =
                    player_state.camera_rotation_target.angles[1] +
                    player_state.unknown_100[1] +
                    player_state.unknown_108.components[1] +
                    player_state.unknown_110[1];
                func_80036e24(1, 0x1000, 0, -0x100);
                func_800314d4(0xff, 0, 0, 0);
            }
            object->unknown_0a = (object->unknown_0a + 64) & 0xfff;
            break;

        case 83:
            if (object->action_timer == 1) {
                map_object_play_spatial_sound(object, template->unknown_0d[2]);
                if (object->tail.fields.unknown_38 == 2) {
                    object->action_timer = 2;
                    object->tail.fields.unknown_38 = 3;
                } else if (object->tail.fields.unknown_38 == 3) {
                    object->action_timer = 3;
                    object->tail.fields.unknown_38 = 2;
                } else if (object->tail.fields.unknown_38 < 2) {
                    object->action_timer = 2;
                }
            } else if (object->action_timer == 2 || object->action_timer == 3) {
                if (object->action_timer == 2) {
                    object->unknown_0a += 256;
                    if (object->unknown_0a < 0x1000) {
                        break;
                    }
                    object->unknown_0a = 0xfff;
                } else {
                    object->unknown_0a -= 256;
                    if ((s16)object->unknown_0a > 0) {
                        break;
                    }
                    object->unknown_0a = 0;
                }
                func_800366fc(object->tail.fields.unknown_39);
                if (object->tail.fields.unknown_38 == 0) {
                    object->action_timer = 99;
                } else if (object->tail.fields.unknown_38 == 1) {
                    map_object_play_spatial_sound(object, template->unknown_0d[2]);
                    object->action_timer = 4;
                } else if (object->tail.fields.unknown_38 < 4) {
                    object->action_timer = 0;
                }
            } else if (object->action_timer == 4) {
                object->unknown_0a -= 256;
                if ((s16)object->unknown_0a <= 0) {
                    object->action_timer = 99;
                }
            } else if (object->action_timer == 9 &&
                       object->tail.fields.unknown_38 == 3) {
                object->unknown_0a = 0xfff;
            }
            break;

        case 84:
            if (object->action_timer == 0) {
                s32 width = object->tail.fields.unknown_3a.bytes.low;
                s32 height = (u8)object->tail.fields.spawn_sequence;
                s32 source_x = object->tail.fields.unknown_39 - ((width - 1) >> 1);
                s32 source_z = object->tail.fields.unknown_3a.bytes.high - ((height - 1) >> 1);
                s32 depth = -((s32)(u8)(object->tail.fields.spawn_sequence >> 8) * 128);
                if (func_80036ad8(source_x, source_z, width, height, depth) ||
                    object->tail.fields.unknown_38 == 0xff) {
                    s32 pattern_index = template->unknown_0d[1] * 2 +
                                        (object->tail.fields.unknown_3e.bytes.low & 1);
                    func_80034f90(object->extra_40.bytes[0], object->position.vx,
                                  object->position.vz, object->rotation.vy,
                                  map_object_cell_patterns[pattern_index], 1, 0x80);
                    object->action_timer = 1;
                    object->scale.vx = 0x1000;
                    object->scale.vy = 0x1000;
                    object->scale.vz = 0x1000;
                } else {
                    object->action_timer++;
                }
            } else if (object->action_timer == 1) {
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
            } else if (object->action_timer == 32) {
                object->unknown_0a += 128;
                if (object->unknown_0a >= 0xfff) {
                    s32 pattern_index = template->unknown_0d[1] * 2 +
                                        (object->tail.fields.unknown_3e.bytes.low & 1);
                    object->unknown_01 = 0;
                    object->unknown_0a = 0;
                    object->action_timer = 0;
                    func_80034f90(object->extra_40.bytes[0], object->position.vx,
                                  object->position.vz, object->rotation.vy,
                                  map_object_cell_patterns[pattern_index], 0, 0);
                    object->scale.vx = 0;
                    object->scale.vy = 0;
                    object->scale.vz = 0;
                }
            } else if (object->action_timer != 99) {
                object->action_timer++;
            }
            break;

        case 88:
            if (object->action_timer == 1) {
                if (object->tail.fields.unknown_38 == 0) {
                    object->action_timer = 2;
                    map_object_play_spatial_sound(object, 0x44);
                } else if (object->tail.fields.unknown_38 == 1) {
                    object->action_timer = 3;
                    map_object_play_spatial_sound(object, 0x44);
                }
                func_80035194(object->unknown_00,
                              (u8)object->tail.fields.spawn_sequence +
                                  object->tail.fields.unknown_3e.bytes.low,
                              (u8)(object->tail.fields.spawn_sequence >> 8),
                              object->tail.fields.unknown_3a.bytes.low,
                              object->tail.fields.unknown_3a.bytes.high,
                              object->tail.fields.unknown_3e.bytes.low,
                              object->tail.fields.unknown_3e.bytes.high,
                              object->rotation.pad, 0x2d);
            } else if (object->action_timer == 2) {
                object->unknown_0a -= 64;
                if ((s16)object->unknown_0a <= 0) {
                    object->unknown_0a = 0;
                    object->action_timer = 99;
                    func_80035194(object->unknown_00,
                                  (u8)object->tail.fields.spawn_sequence,
                                  (u8)(object->tail.fields.spawn_sequence >> 8),
                                  object->tail.fields.unknown_3a.bytes.low,
                                  object->tail.fields.unknown_3a.bytes.high,
                                  object->tail.fields.unknown_3e.bytes.low,
                                  object->tail.fields.unknown_3e.bytes.high,
                                  object->rotation.pad, 0x2d);
                }
            } else if (object->action_timer == 3) {
                object->unknown_0a += 64;
                if (object->unknown_0a >= 0x1000) {
                    object->unknown_0a = 0xfff;
                    object->action_timer = 99;
                }
            }
            break;

        case 89:
            if (object->action_timer == 1) {
                object->extra_40.halfwords[0] = object->tail.fields.unknown_3a.value;
                object->action_timer = 2;
                object->unknown_00 = object->extra_40.bytes[2];
            } else if (object->action_timer == 2) {
                u16 previous = object->extra_40.halfwords[0];
                object->extra_40.halfwords[0] = previous - 1;
                if (previous == 0) {
                    KfMapOccupancyCell *cell =
                        &bss_801c7540.map_cells[object->position.vz >> 11]
                                                  [object->position.vx >> 11];
                    object->action_timer = 3;
                    cell->layer[object->unknown_00 == 1 ? 0 : 1].unknown_03 = 0x74;
                    map_object_play_spatial_sound(object, 0xe3);
                }
            } else if (object->action_timer == 3) {
                object->unknown_10 -= 256;
                if ((s16)object->unknown_10 <= 0) {
                    object->unknown_10 = 0;
                    object->action_timer = 4;
                    object->extra_40.halfwords[0] = object->tail.fields.spawn_sequence;
                }
            } else if (object->action_timer == 4) {
                u16 previous = object->extra_40.halfwords[0];
                object->extra_40.halfwords[0] = previous - 1;
                if (previous == 0) {
                    map_object_play_spatial_sound(object, 0xe3);
                    object->action_timer = 5;
                }
            } else if (object->action_timer == 5) {
                object->unknown_10 += 256;
                if (object->unknown_10 >= 0x1000) {
                    KfMapOccupancyCell *cell =
                        &bss_801c7540.map_cells[object->position.vz >> 11]
                                                  [object->position.vx >> 11];
                    object->unknown_10 = 0x1000;
                    object->action_timer = 0;
                    cell->layer[object->unknown_00 == 1 ? 0 : 1].unknown_03 = 0x75;
                    object->unknown_00 = 0;
                }
            }
            break;

        case 224:
            if (func_80036ad8(object->position.vx >> 11,
                               object->position.vz >> 11,
                               object->tail.fields.unknown_38,
                               object->tail.fields.unknown_39,
                               object->position.vy)) {
                func_80016260(object->tail.fields.unknown_3a.bytes.low,
                              object->tail.fields.unknown_3a.bytes.high,
                              (u8)object->tail.fields.spawn_sequence,
                              (u8)(object->tail.fields.spawn_sequence >> 8),
                              object->tail.fields.unknown_3e.bytes.low,
                              (s8)object->extra_40.bytes[0],
                              (s8)object->extra_40.bytes[1],
                              (s8)object->extra_40.bytes[2]);
            }
            break;

        case 225:
            if (!func_80036ad8(object->position.vx >> 11,
                                object->position.vz >> 11,
                                object->tail.fields.unknown_38,
                                object->tail.fields.unknown_39,
                                object->position.vy)) {
                object->extra_40.bytes[2] = 0;
            } else if (object->extra_40.bytes[2] == 0) {
                if (!(object->tail.fields.unknown_3a.bytes.low & 0x80)) {
                    object->extra_40.bytes[2] = 1;
                }
                switch (object->tail.fields.unknown_3a.bytes.low & 0x0f) {
                case 0:
                    ((void (*)(KfMapObject *))state_8017d118.active_table[3])(object);
                    break;
                case 1:
                    func_800366fc(object->tail.fields.unknown_3a.bytes.high);
                    break;
                case 2:
                    event_state.control.bytes[0x40 + object->tail.fields.unknown_3a.bytes.high] =
                        (u8)object->tail.fields.spawn_sequence;
                    break;
                }
            }
            break;

        /* The remaining bounded action arms have retail case targets, but
         * their side effects are not yet reconstructed in this WIP body. */
        case 4:
        case 19:
        case 81:
        case 96: case 97: case 98:
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
