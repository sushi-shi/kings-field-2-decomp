#include <kf/game/event_stream.h>
#include <kf/game/map_object.h>
#include <kf/lib/address.h>

ADDRESS(0x80046700, 0x8c)
void func_80046700(KfEventObjectView *event, s32 object_id)
{
    KfMapObject *object = map_object_effect_pool_acquire(0x17c, 0x10, -1);

    event->effect_object_index = (object - map_object_state.objects) - 0x7c;
    object->object_id = object_id;
    object->tail.fields.unknown_38 = 0;
}
