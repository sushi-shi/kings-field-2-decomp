#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/collision_cache.h>
#include <kf/game/effect.h>
#include <psyq/libc.h>

DATA(0x801c7068, 0x8)
SVECTOR effect_collision_motion_step;

ADDRESS(0x80042298, 0x18c)
s32 effect_collision_step(s32 radius, s32 angle, s32 step)
{
    KfEffectRecord *record = effect_state.current_record;
    VECTOR previous;
    s32 result;
    u8 next_kind;

    addVector(&record->position, &record->direction);
    result = func_8003fa68(&record->position, radius, angle);
    if (record->unknown_0d != 0) {
        effect_collision_motion_step.vx = record->direction.vx >> 1;
        effect_collision_motion_step.vy = record->direction.vy >> 1;
        effect_collision_motion_step.vz = record->direction.vz >> 1;
        if (result == 0) {
            previous.vx = record->position.vx - effect_collision_motion_step.vx;
            previous.vy = record->position.vy - effect_collision_motion_step.vy;
            previous.vz = record->position.vz - effect_collision_motion_step.vz;
            result = func_8003fa68(&previous, radius, angle);
            if (result != 0) {
                copyVector(&effect_collision_motion_step, &record->direction);
            }
        }
    }
    next_kind = 2;
    if (KF_COLLISION_CACHE_LAYER == 0) {
        next_kind = 1;
    }
    record->unknown_0a = next_kind;
    record->rotation.vz = ((u16)record->rotation.vz - step) & KF_ANGLE_WRAP_MASK;
    return result;
}

ADDRESS(0x80042424, 0xcc)
void effect_collision_backtrack(void)
{
    KfEffectRecord *record = effect_state.current_record;

    if (record->unknown_0d != 0) {
        record->position.vx -= effect_collision_motion_step.vx;
        record->position.vy -= effect_collision_motion_step.vy;
        record->position.vz -= effect_collision_motion_step.vz;
    } else {
        record->position.vx -= record->direction.vx;
        record->position.vy -= record->direction.vy;
        record->position.vz -= record->direction.vz;
    }
    record->position.vx -= record->direction.vx;
    record->position.vy -= record->direction.vy;
    record->position.vz -= record->direction.vz;
}

ADDRESS(0x800424f0, 0x160)
void effect_spawn_radial_ring(s32 count, s32 radius, s32 vertical_angle, s32 arg3)
{
    KfEffectRecord *record = effect_state.current_record;
    SVECTOR direction;
    VECTOR position;
    s32 angle = rand();
    s32 angle_step = KF_ANGLE_FULL_TURN / count;

    position.vx = record->position.vx - record->direction.vx;
    position.vy = record->position.vy - record->direction.vy;
    position.vz = record->position.vz - record->direction.vz;
    direction.vy = vertical_angle;
    count--;
    while (count != -1) {
        direction.vx = (rcos(angle) * radius) >> KF_FIXED12_BITS;
        direction.vz = (rsin(angle) * radius) >> KF_FIXED12_BITS;
        angle += angle_step;
        count--;
        effect_construct_record(10, record->type | 3, 8, &position, &direction,
                      effect_state.current_index, arg3);
    }
}
