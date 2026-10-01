#include <kf/lib/address.h>
#include <kf/game/callback.h>
#include <kf/game/map_cell_pattern.h>
#include <kf/game/map_object.h>

extern KfMapCellPattern map_object_cell_patterns[9][3];
extern void func_80034f90(s32 mode, s32 world_x, s32 world_z, s32 angle,
                          const KfMapCellPattern *patterns, s32 variant_index,
                          s32 layer_flag);
extern void func_80035194(u32 layer_select, s32 source_x, s32 source_z,
                          s32 destination_x, s32 destination_z, s32 width,
                          s32 height, s32 rotation, u32 field_mask);
extern s32 func_8002b9d4(s32 x, s32 y, s32 z, s32 radius, s32 height, s32 mode);

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

        /* The remaining bounded action arms have retail case targets, but
         * their side effects are not yet reconstructed in this WIP body. */
        case 4: case 5: case 8: case 9:
        case 15: case 16: case 17: case 18: case 19:
        case 22: case 34: case 81: case 83: case 84:
        case 88: case 89: case 96: case 97: case 98:
        case 224: case 225:
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
