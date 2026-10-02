#include <stdarg.h>

#include <kf/lib/address.h>
#include <kf/game/callback.h>
#include <kf/game/cd.h>
#include <kf/game/map_cell.h>
#include <kf/game/map_cell_pattern.h>
#include <kf/game/map_object.h>
#include <kf/game/player.h>
#include <psyq/libc.h>

enum {
    KF_MAP_OBJECT_LOAD_COUNT = 0x15e
};

RODATA(0x80011484, 0x3f8)

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
        object->object_id = KF_MAP_OBJECT_ID_NONE;
        object->action = KF_MAP_OBJECT_ACTION_NONE;
        object->tail.reset_words[2] = 0;
        object->tail.reset_words[1] = 0;
        object->tail.reset_words[0] = 0;
        object++;
    } while (remaining-- != 0);

    map_object_state.unknown_8742 = 0;
    map_object_state.unknown_8740 = 0;
    map_object_state.unknown_873e = 0;
}

ADDRESS(0x80035590, 0x48)
void map_object_reset(KfMapObject *object)
{
    object->unknown_01 = 0x80;
    object->unknown_0a = 0;
    object->unknown_02 = 0xff;
    object->unknown_00 = 0;
    object->rotation.vz = 0;
    object->rotation.vx = 0;
    object->rotation.vy = 0;
    object->scale.vx = object->scale.vy = object->scale.vz = 0x1000;
    object->action = KF_MAP_OBJECT_ACTION_NONE;
    object->unknown_0e = 0;
    object->unknown_05 = 0xff;
    object->unknown_10 = 0;
}

ADDRESS(0x800355d8, 0xd4)
void map_object_set_property(s32 index, s32 property, ...)
{
    KfMapObject *object;
    KfMapObjectTemplate *template;
    va_list arguments;

    if (index == 0xffff) {
        return;
    }

    object = &map_object_state.objects[index];
    template = &map_object_state.templates[object->object_id];
    va_start(arguments, property);
    switch (property) {
    case 0:
        object->unknown_00 = 0;
        object->tail.fields.unknown_38 = 0;
        if (template->kind == 0x10) {
            object->rotation.vz = 0x400;
        }
        break;
    case 1:
        object->unknown_00 = va_arg(arguments, u8);
        break;
    case 2:
        object->tail.fields.unknown_38 = 0xff;
        break;
    case 3:
        object->unknown_0e = va_arg(arguments, u16);
        break;
    }
    va_end(arguments);
}

ADDRESS(0x800356ac, 0xf4)
void map_object_set_cell_marker(KfMapObject *object, s32 mode, u8 marker)
{
    KfMapOccupancyCell *cell;
    u8 *cell_marker;

    if (mode == 0 && player_state.unknown_6a == 0) {
        s32 cell_z = object->position.vz >> 11;
        s32 cell_x = object->position.vx >> 11;
        KfMapOccupancyCell *row = bss_801c7540.map_cells[cell_z];
        cell = &row[cell_x];
        cell_marker = &cell->layer[0].object_index;
        if (object->unknown_00 != 1) {
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
        if (object->unknown_00 != 1) {
            cell_marker = &cell->layer[1].object_index;
        }
        *cell_marker = 0xfe;
        object->scale.vx = object->scale.vy = object->scale.vz = 0x1000;
    }
}

ADDRESS(0x800357a0, 0xf4)
void map_object_refresh_cell_markers(s32 mode)
{
    KfMapObject *object;
    KfMapObjectTemplate *template;
    s32 index;

    object = map_object_state.objects;
    for (index = KF_MAP_OBJECT_CAPACITY; index != 0; object++, index--) {
        if (object->action != 5) {
            if (object->action != 0x51) {
                continue;
            }
            if (object->tail.fields.unknown_3a.bytes.high == 0xff) {
                continue;
            }
            template = &map_object_state.templates[object->object_id];
            map_object_set_cell_marker(object, mode, template->marker_action_51);
        } else {
            if (object->tail.fields.unknown_38 == 0xfe) {
                continue;
            }
            map_object_set_property(object->tail.fields.unknown_3a.value, mode, object->unknown_00);
            template = &map_object_state.templates[object->object_id];
            map_object_set_cell_marker(object, mode, template->marker_action_05);
        }
    }
}

ADDRESS(0x80035894, 0x7e4)
void map_object_initialize_from_placements(const KfMapObjectPlacement *placements)
{
    KfMapObject *object = map_object_state.objects;
    u32 frame_count = cd_state.frame_count;
    s32 index;

    for (index = KF_MAP_OBJECT_LOAD_COUNT - 1; index != -1; placements++, object++, index--) {
        const KfMapObjectTemplate *template;
        KfMapOccupancyCell *cell;
        KfMapOccupancyCell *row;
        KfMapOccupancyLayer *layer;

        if (placements->object_id == 0xffff) {
            object->object_id = KF_MAP_OBJECT_ID_NONE;
        } else {
            object->object_id = placements->object_id;
        }
        object->action = KF_MAP_OBJECT_ACTION_NONE;
        if (object->object_id == KF_MAP_OBJECT_ID_NONE) {
            continue;
        }

        template = &map_object_state.templates[object->object_id];
        object->action_timer = 0;
        object->unknown_01 = 0x80;
        object->unknown_0a = 0;
        object->unknown_02 = 0xff;
        object->rotation.vz = 0;
        object->rotation.vx = 0;
        object->rotation.vy = -(s32)placements->rotation_y & 0xfff;
        object->scale.vz = 0x1000;
        object->scale.vy = 0x1000;
        object->scale.vx = 0x1000;
        object->unknown_00 = placements->layer;
        object->collision_flags = template->unknown_02[1];
        object->unknown_0e = template->unknown_0a;
        object->unknown_05 = 0xff;
        object->unknown_10 = 0;
        if (object->collision_flags & 0x20) {
            object->unknown_02 = 0x80;
        }
        object->collision_height = template->interaction_height;
        object->position.vx = ((u32)placements->region_x << 11) + placements->local_x;
        object->position.vz = ((u32)placements->region_z << 11) + placements->local_z;
        row = bss_801c7540.map_cells[placements->region_z];
        cell = &row[placements->region_x];
        layer = cell->layer;
        if (object->unknown_00 != 1) {
            layer++;
        }
        object->position.vy = placements->height - ((s32)layer->elevation << 7);
        object->tail.placement.copy_words = placements->tail_words;
        memset(&object->extra_40, 0xff, sizeof object->extra_40);

        if (template->collision_radius != 0) {
            func_8002b73c(object->position.vx, object->position.vz,
                          template->collision_radius, 1);
        }

        switch (template->collision_kind) {
        case 0x40:
            if (template->kind != 0x20) {
                if (object->tail.fields.unknown_3a.bytes.low != 0xff) {
                    object->rotation.vx =
                        (u8)object->tail.fields.unknown_3a.bytes.low << 6;
                }
                if (object->tail.fields.unknown_3a.bytes.high != 0xff) {
                    object->rotation.vy =
                        (u8)object->tail.fields.unknown_3a.bytes.high << 6;
                }
                if ((u8)object->tail.fields.spawn_sequence != 0xff) {
                    object->rotation.vz =
                        (u8)object->tail.fields.spawn_sequence << 6;
                }
                if (object->tail.fields.unknown_39 == 0) {
                    object->action = 0x10;
                    object->unknown_02 = 1;
                    object->unknown_05 = 0x44;
                    object->unknown_10 = 0x1000;
                    object->extra_40.raw = object->position.vy;
                }
            }
            break;
        case 2:
            object->action = 2;
            object->unknown_01 = 0;
            func_80034f90(object->unknown_00, object->position.vx,
                          object->position.vz, object->rotation.vy,
                          map_object_cell_patterns[8], 0, 0xff);
            break;
        case 4:
            func_80035194(object->unknown_00,
                          (u8)object->tail.fields.unknown_3a.bytes.high + 2,
                          (u8)object->tail.fields.spawn_sequence,
                          object->tail.fields.unknown_39,
                          object->tail.fields.unknown_3a.bytes.low,
                          2, 2, 0, 0x2d);
            object->action = 4;
            object->action_timer = 2;
            object->extra_40.halfwords[0] = 999;
            object->extra_40.halfwords[1] = object->rotation.vy;
            func_8002b73c(object->position.vx, object->position.vz, 3000, 1);
            break;
        case 3:
            object->unknown_01 = 0;
            func_80035194(object->unknown_00,
                          (u8)object->tail.fields.unknown_3a.bytes.high +
                              template->unknown_0d[0] * 2,
                          (u8)object->tail.fields.spawn_sequence,
                          object->tail.fields.unknown_39,
                          object->tail.fields.unknown_3a.bytes.low,
                          template->unknown_0d[0], template->unknown_0d[1],
                          object->rotation.vy, 0x2d);
            object->action = 3;
            func_8002b73c(object->position.vx, object->position.vz, 0x1130, 1);
            break;
        case 0x53:
            object->unknown_01 = 0;
            object->action = 0x53;
            object->action_timer = 9;
            break;
        case 8:
            object->action = 8;
            break;
        case 0x16:
            object->action = 0x16;
            break;
        case 5:
            object->action = 5;
            object->unknown_01 = 0;
            map_object_set_cell_marker(object, 0, template->marker_action_05);
            break;
        case 0xe0:
            object->unknown_00 = 0;
            object->action = 0xe0;
            object->position.vx -= placements->local_x;
            object->position.vz -= placements->local_z;
            object->position.vy -= placements->height;
            if (placements->local_x == 0xff) {
                object->extra_40.bytes[2] = 0x7f;
                object->extra_40.bytes[1] = 0x7f;
                object->extra_40.bytes[0] = 0x7f;
            } else {
                object->extra_40.bytes[0] =
                    (s8)placements->local_x - (s8)placements->region_x;
                object->extra_40.bytes[1] =
                    (s8)placements->local_z - (s8)placements->region_z;
                object->extra_40.bytes[2] =
                    (s8)placements->height -
                    (s8)(-object->position.vy >> 7);
            }
            break;
        case 0x22:
            object->unknown_01 = 0;
            object->unknown_0a = 0;
            object->action = 0x22;
            object->extra_40.bytes[0] = 0;
            break;
        case 0xe1:
            object->unknown_00 = 0;
            object->action = 0xe1;
            object->extra_40.bytes[0] = 0;
            break;
        case 0xd:
            if (object->tail.fields.unknown_3a.bytes.low != 0xff) {
                object->rotation.vx =
                    (u8)object->tail.fields.unknown_3a.bytes.low << 6;
            }
            if (object->tail.fields.unknown_3a.bytes.high != 0xff) {
                object->rotation.vy =
                    (u8)object->tail.fields.unknown_3a.bytes.high << 6;
            }
            if ((u8)object->tail.fields.spawn_sequence != 0xff) {
                object->rotation.vz =
                    (u8)object->tail.fields.spawn_sequence << 6;
            }
            break;
        case 0x1f:
            object->unknown_00 = 0;
            object->action = 0x1f;
            object->extra_40.raw = frame_count +
                object->tail.fields.unknown_3e.value * 6;
            break;
        case 0xf0:
            object->action = 0xf0;
            break;
        case 0x54:
            object->unknown_01 = 0;
            object->extra_40.bytes[0] = object->unknown_00;
            object->unknown_00 = 3;
            object->action = 0x54;
            func_80034f90(object->extra_40.bytes[0], object->position.vx,
                          object->position.vz, object->rotation.vy,
                          map_object_cell_patterns[
                              template->unknown_0d[1] * 2 +
                              (object->tail.fields.unknown_3e.bytes.low & 1)],
                          0, 0);
            object->scale.vz = 0;
            object->scale.vy = 0;
            object->scale.vx = 0;
            break;
        case 0x51:
            object->unknown_01 = 0;
            object->action = 0x51;
            object->extra_40.bytes[0] = 0;
            if (object->tail.fields.unknown_3a.bytes.high != 0xff) {
                map_object_set_cell_marker(object, 0,
                                           template->marker_action_51);
            }
            break;
        case 0x58:
            object->unknown_01 = 0;
            object->action = 0x58;
            object->action_timer = 1;
            break;
        case 0x59: {
            KfMapOccupancyCell *kind59_row;
            KfMapOccupancyCell *kind59_cell;
            KfMapOccupancyLayer *kind59_layer;

            object->action = 0x59;
            object->unknown_02 = 1;
            object->unknown_05 = 0x42;
            object->unknown_10 = 0x1000;
            object->position.vy += 0x100;
            kind59_row = bss_801c7540.map_cells[object->position.vz >> 11];
            kind59_cell = &kind59_row[object->position.vx >> 11];
            kind59_layer = kind59_cell->layer;
            if (object->unknown_00 != 1) {
                kind59_layer++;
            }
            kind59_layer->unknown_03 = 0x75;
            object->extra_40.bytes[2] = object->unknown_00;
            object->unknown_00 = 0;
            break;
        }
        case 0xb:
        case 0x14:
            object->unknown_00 = 0;
            break;
        case 0x13:
            object->action = 0x13;
            break;
        case 0x30:
            object->action = 0x30;
            object->unknown_02 = 1;
            object->position.vy -=
                (u8)object->tail.fields.spawn_sequence * 0x100;
            break;
        case 0x12:
            object->action = 0x12;
            object->extra_40.raw = frame_count + 30;
            break;
        case 0xf:
            object->action = 0xf;
            break;
        case 0x11:
            object->action = 0x11;
            break;
        case 9:
            object->action = 9;
            object->extra_40.bytes[0] = object->unknown_00;
            break;
        case 0x15:
            object->action = 9;
            /* Fall through: this kind saves the original layer. */
        case 0xe2:
            object->extra_40.bytes[0] = object->unknown_00;
            object->unknown_00 = 0;
            break;
        default:
            state_8017d118.active_table[8](object, template);
            break;
        case 0x21:
        case 0xff:
            break;
        }
        if (object->object_id == 0xa3) {
            object->unknown_02 = 3;
        }
    }
}

DATA(0x80067890, 0x10e)
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

DATA(0x801749d0, 0x8744)
KfMapObjectStateGame map_object_state;
