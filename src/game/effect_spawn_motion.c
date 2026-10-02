#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/collision_cache.h>
#include <kf/game/effect.h>
#include <psyq/libc.h>

ADDRESS(0x80041e94, 0x298)
void effect_spawn_motion(KfEffectRecord *record, s32 position_mode,
                   s32 motion_mode, s32 arg3, s32 arg4, s32 arg5,
                   s32 arg6, s32 arg7, ...)
{
    s32 *args;
    SVECTOR motion;
    VECTOR position_delta;
    VECTOR transformed;

    /* O32 places the remaining effect-mode operands after arg7. */
    args = &arg7;
    if (position_mode == -1) {
        position_delta.vx = position_delta.vy = position_delta.vz = 0;
    } else {
        effect_sample_rotated_vertex(record, position_mode, &position_delta,
                      (const SVECTOR *)&record->scale_x);
    }

    switch (motion_mode) {
    case -1:
        effect_sample_rotated_vertex(record, args[2], &transformed,
                      (const SVECTOR *)args[3]);
        copyVector(&motion, &transformed);
        motion_mode = args[1];
        goto add_direction;
    case -2:
        motion.vx = (rand() * args[1] >> 15) + (u16)args[2];
        motion.vy = (rand() * args[3] >> 15) + (u16)args[4];
        motion.vz = (rand() * args[5] >> 15) + (u16)args[6];
        goto spawn;
    case -3:
        motion.vx = motion.vy = motion.vz = 0;
        motion_mode = args[1];
        goto add_direction;
    default:
        break;
    }

    pitch_yaw_to_forward_vector((const struct KfEulerAngles *)&record->rotation,
                                &motion);
    vector3s_scale_shift12(30, &motion);
    motion.vx = -(u16)motion.vx;
    motion.vy = -(u16)motion.vy;
    motion.vz = -(u16)motion.vz;

add_direction:
    motion.vx = (u16)motion.vx + ((record->direction.vx * motion_mode) >> 12);
    motion.vy = (u16)motion.vy + ((record->direction.vy * motion_mode) >> 12);
    motion.vz = (u16)motion.vz + ((record->direction.vz * motion_mode) >> 12);

spawn:
    addVector(&position_delta, &record->position);
    effect_construct_record(10, 0, 101, &position_delta, &motion,
                  arg3, arg4, arg5, arg6, arg7);
}

ADDRESS(0x8004212c, 0x16c)
s32 effect_scatter_lower_bound(const VECTOR *origin, s32 count, s32 spread,
                  s32 scale_x, s32 scale_z, s32 variation)
{
    s32 lower_bound = KF_COLLISION_CACHE_LOWER_BOUND;
    s32 offset_x = 0;
    s32 offset_z = 0;

    if (origin->vy >= lower_bound) {
        if (origin->vy > lower_bound + 500) {
            return 0;
        }
        count--;
        if (count != -1) {
            do {
            VECTOR position;
            SVECTOR direction;
            s32 magnitude;

            position.vx = origin->vx + offset_x;
            position.vz = origin->vz + offset_z;
            position.vy = KF_COLLISION_CACHE_LOWER_BOUND;
            magnitude = func_800157f8(variation) + 4096;
            effect_construct_record(10, 0, 0x66, &position, &direction,
                          scale_x * magnitude >> 12,
                          scale_z * magnitude >> 12);

            count--;
            offset_x = (rand() * spread >> 14) - spread;
            offset_z = (rand() * spread >> 14) - spread;
            } while (count != -1);
        }
        return 1;
    }
    return 0;
}
