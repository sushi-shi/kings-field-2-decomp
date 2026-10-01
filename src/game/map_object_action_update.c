#include <kf/lib/address.h>
#include <kf/game/callback.h>
#include <kf/game/cd.h>
#include <kf/game/event_state.h>
#include <kf/game/map_cell_pattern.h>
#include <kf/game/map_object.h>
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

        case 18:
            if ((s32)(object->extra_40.raw - cd_state.frame_count) < 0) {
                object->extra_40.raw = cd_state.frame_count + 30;
                map_object_play_spatial_sound(object, 0xee);
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

        /* The remaining bounded action arms have retail case targets, but
         * their side effects are not yet reconstructed in this WIP body. */
        case 4: case 8:
        case 17: case 19:
        case 22: case 34: case 81: case 83: case 84:
        case 89: case 96: case 97: case 98:
        case 225:
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
