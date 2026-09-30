#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/map_object.h>

enum {
    KF_MAP_OBJECT_MOTION_ACTION = 0x70,
    KF_MAP_OBJECT_MOTION_COMPLETE_ACTION = 0x63,
    KF_MAP_OBJECT_MOTION_STEP = 204,
    KF_MAP_OBJECT_MOTION_LIMIT = 0xfff
};

ADDRESS(0x80036b68, 0x2bc)
s32 func_80036b68(KfMapObject *source, KfMapObject *target,
                   SVECTOR *start_offset, SVECTOR *end_offset,
                   s32 brighten, s32 duration)
{
    VECTOR start_position;
    VECTOR end_position;
    s32 fraction;

    switch (source->action_timer) {
    case 0:
        if (source->tail.fields.unknown_38 == 0xff) {
            return 0;
        }
        if (target->tail.fields.unknown_38 == 0) {
            source->extra_40.bytes[0] = 0;
        } else {
            source->extra_40.bytes[0] = duration - 1;
        }
        source->action_timer = 1;
        map_object_reset(target);
        target->action = KF_MAP_OBJECT_MOTION_ACTION;
        target->unknown_00 = source->unknown_00;
        if (brighten) {
            target->unknown_01 = 0;
        } else {
            target->unknown_01 = 0x80;
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
        return 0;
    }

    if (brighten) {
        target->unknown_0a += KF_MAP_OBJECT_MOTION_STEP;
        if (target->unknown_0a >= 0x1000) {
            target->unknown_0a = KF_MAP_OBJECT_MOTION_LIMIT;
        }
    }
    vector_rotate_yxz((const struct KfEulerAngles *)&source->rotation,
                      start_offset, &start_position);
    vector_rotate_yxz((const struct KfEulerAngles *)&source->rotation,
                      end_offset, &end_position);
    source->extra_40.bytes[0]++;
    fraction = ((s32)source->extra_40.bytes[0] << 12) / duration;
    target->position.vx = source->position.vx +
        func_8001584c(start_position.vx, end_position.vx, fraction);
    target->position.vy = source->position.vy +
        func_8001584c(start_position.vy, end_position.vy, fraction);
    target->position.vz = source->position.vz +
        func_8001584c(start_position.vz, end_position.vz, fraction);
    if (source->extra_40.bytes[0] >= duration) {
        if (brighten) {
            target->unknown_0a = KF_MAP_OBJECT_MOTION_LIMIT;
        }
        map_object_play_spatial_sound(target, 0x42);
        target->tail.fields.unknown_38 = 0xff;
        source->action_timer = KF_MAP_OBJECT_MOTION_COMPLETE_ACTION;
        return 1;
    }
    return 0;
}
