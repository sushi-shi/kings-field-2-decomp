#include <kf/lib/address.h>
#include <kf/game/collision_cache.h>
#include <kf/game/effect.h>


ADDRESS(0x8003fa68, 0x12c)
s32 effect_probe_collision_by_type(const VECTOR *position, s32 radius,
    s32 height_flags)
{
    KfEffectRecord *record = effect_state.current_record;
    s32 y;
    s32 result;

    if (record->cooldown == 0) {
        y = position->vy + ((height_flags & 0xfff) >> 1);
        switch (record->type & 7) {
        case 2:
            result = collision_query_world(position->vx, y, position->vz, radius, height_flags, 0x31);
            break;
        case 1:
            result = collision_query_world(position->vx, y, position->vz, radius, height_flags, 0xa1);
            break;
        case 3:
            result = collision_query_world(position->vx, y, position->vz, radius, height_flags, 0xb1);
            break;
        case 4:
            result = collision_query_world(position->vx, y, position->vz, radius, height_flags, 1);
            break;
        }
    } else {
        record->cooldown--;
        result = 0;
    }
    return result;
}
