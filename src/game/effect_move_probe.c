#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/effect.h>

ADDRESS(0x8004177c, 0x1e0)
s32 func_8004177c(s32 scale, s32 max_length, s32 probe_radius, s32 probe_angle,
                  SVECTOR *motion)
{
    KfEffectRecord *record = effect_state.current_record;
    s32 length;

    vector3s_scale_shift12(scale, motion);
    addVector(&record->direction, motion);
    length = fixed_vector3_length(record->direction.vx, record->direction.vy,
                                  record->direction.vz);
    if (length > max_length) {
        record->direction.vx = record->direction.vx * max_length / length;
        record->direction.vy = record->direction.vy * max_length / length;
        record->direction.vz = record->direction.vz * max_length / length;
    }
    addVector(&record->position, &record->direction);
    if (probe_angle == -1) {
        return 0;
    }
    return -!!func_8003fa68(&record->position, probe_radius, probe_angle);
}
