#include <kf/lib/address.h>
#include <kf/game/collision_cache.h>
#include <kf/game/effect.h>


ADDRESS(0x8003fa68, 0x12c)
s32 func_8003fa68(const VECTOR *position, s32 arg1, s32 angle)
{
    KfEffectRecord *record = effect_state.current_record;
    s32 y;
    s32 result;

    if (record->cooldown == 0) {
        y = position->vy + ((angle & 0xfff) >> 1);
        switch (record->type & 7) {
        case 2:
            result = collision_query_world(position->vx, y, position->vz, arg1, angle, 0x31);
            break;
        case 1:
            result = collision_query_world(position->vx, y, position->vz, arg1, angle, 0xa1);
            break;
        case 3:
            result = collision_query_world(position->vx, y, position->vz, arg1, angle, 0xb1);
            break;
        case 4:
            result = collision_query_world(position->vx, y, position->vz, arg1, angle, 1);
            break;
        }
    } else {
        record->cooldown--;
        result = 0;
    }
    return result;
}
