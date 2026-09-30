#include <stdarg.h>

#include <kf/lib/address.h>
#include <kf/game/map_object.h>
#include <kf/game/player.h>

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
        cell = bss_801c7540.map_cells[object->position.vz >> 11];
        cell += object->position.vx >> 11;
        cell_marker = &cell->layer[0].object_index;
        if (object->unknown_00 != 1) {
            cell_marker = &cell->layer[1].object_index;
        }
        *cell_marker = marker;
        object->scale.vx = object->scale.vy = object->scale.vz = 0;
    } else {
        cell = bss_801c7540.map_cells[object->position.vz >> 11];
        cell += object->position.vx >> 11;
        cell_marker = &cell->layer[0].object_index;
        if (object->unknown_00 != 1) {
            cell_marker = &cell->layer[1].object_index;
        }
        *cell_marker = 0xfe;
        object->scale.vx = object->scale.vy = object->scale.vz = 0x1000;
    }
}

ADDRESS(0x800357a0, 0xf4)
void func_800357a0(s32 mode)
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
