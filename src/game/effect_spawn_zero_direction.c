#include <kf/lib/address.h>
#include <kf/game/collision_cache.h>
#include <kf/game/effect.h>
#include <psyq/libc.h>

DATA(0x8006d708, 0x8)
static SVECTOR effect_zero_direction = {0, 0, 0, 0};

ADDRESS(0x80041d7c, 0x90)
void func_80041d7c(KfEffectRecord *record, s32 mode)
{
    VECTOR position;
    SVECTOR direction;
    KfEffectRecord *spawned;

    func_800401b4(record, mode, &position, (const SVECTOR *)&record->scale_x);
    direction.vx = record->rotation.vx;
    direction.vy = record->rotation.vy + (rand() >> 7) - 128;
    direction.vz = record->rotation.vz;
    spawned = func_80040308(10, record->type, 100, &position,
                            &effect_zero_direction, &direction);
    if (spawned != 0) {
        spawned->phase = 2;
    }
}

ADDRESS(0x80041e0c, 0x88)
s32 func_80041e0c(const VECTOR *position, s32 arg1, s32 arg2, s32 vertical_window)
{
    s32 lower_bound = KF_COLLISION_CACHE_LOWER_BOUND;
    VECTOR spawn_position;
    SVECTOR direction;

    if (position->vy < lower_bound) {
        return 0;
    }
    if (position->vy > vertical_window + lower_bound) {
        return 0;
    }

    spawn_position.vx = position->vx;
    spawn_position.vz = position->vz;
    spawn_position.vy = lower_bound;
    func_80040308(10, 0, 0x66, &spawn_position, &direction, arg1, arg2);
    return 1;
}
