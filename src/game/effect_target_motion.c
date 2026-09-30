#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/effect.h>

ADDRESS(0x80041b14, 0x1bc)
s32 func_80041b14(const VECTOR *target, s32 max_length, s32 scale,
                  s32 settle_distance, s32 min_distance, s32 probe_radius,
                  s32 probe_angle)
{
    KfEffectRecord *record = effect_state.current_record;
    VECTOR delta;
    SVECTOR motion;
    s32 length;

    delta.vx = target->vx - record->position.vx;
    delta.vy = target->vy - record->position.vy;
    delta.vz = target->vz - record->position.vz;
    length = fixed_vector3_length(delta.vx, delta.vy, delta.vz);
    if (length <= min_distance) {
        return -2;
    }
    if (length <= settle_distance) {
        record->direction.vx = func_8001584c(0, record->direction.vx, 0xc00);
        record->direction.vy = func_8001584c(0, record->direction.vy, 0xc00);
        record->direction.vz = func_8001584c(0, record->direction.vz, 0xc00);
    }
    motion.vx = (delta.vx << KF_FIXED12_BITS) / length;
    motion.vy = (delta.vy << KF_FIXED12_BITS) / length;
    motion.vz = (delta.vz << KF_FIXED12_BITS) / length;
    return func_8004177c(scale, max_length, probe_radius, probe_angle, &motion);
}
