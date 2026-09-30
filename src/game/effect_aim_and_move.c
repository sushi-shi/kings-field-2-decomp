#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/actor.h>
#include <kf/game/effect.h>
#include <kf/game/player.h>

extern KfActor *func_8003a778(const VECTOR *position, s16 yaw, s16 pitch,
                               s32 max_distance, s32 filter_a, s32 filter_b,
                               s32 *distance, s32 flags);

ADDRESS(0x8004195c, 0x1b8)
s32 func_8004195c(s32 max_length, s32 scale, s32 turn_step,
                  s32 probe_radius, s32 probe_angle, s32 proximity,
                  s32 close_scale, s32 target_filter)
{
    KfEffectRecord *record = effect_state.current_record;
    VECTOR target_position;
    struct KfEulerAngles target_angles;
    SVECTOR motion;
    s32 distance;
    KfActor *target;

    if (!(record->type & KF_EFFECT_USE_PLAYER_MAGIC)) {
        goto player_target;
    }
    target = func_8003a778(&record->position, record->rotation.vy,
                           record->rotation.vx, 24000, target_filter,
                           target_filter, &distance, 0);
    if (target == 0) {
        goto move;
    }
    target_position.vx = target->position.vx;
    target_position.vy = target->position.vy - (target->unknown_1e >> 1);
    target_position.vz = target->position.vz;

aim:
    func_800154fc(target_position.vx - record->position.vx,
                  target_position.vy - record->position.vy,
                  target_position.vz - record->position.vz,
                  &target_angles);
    record->rotation.vx = angle_approach(record->rotation.vx,
                                        target_angles.x, turn_step);
    record->rotation.vy = angle_approach(record->rotation.vy,
                                        target_angles.y, turn_step);
    goto move;

player_target:
    distance = player_distance_to_point_in_cone(
        &record->position, record->rotation.vy, 24000, 0x1000);
    if (distance == -1) {
        goto move;
    }
    target_position.vx = player_state.camera_position.vx;
    target_position.vy = player_state.camera_position.vy - 1600;
    target_position.vz = player_state.camera_position.vz;
    goto aim;

move:
    pitch_yaw_to_forward_vector((const struct KfEulerAngles *)&record->rotation,
                                &motion);
    if (distance >= 0 && distance <= proximity) {
        scale = close_scale;
    }
    return func_8004177c(scale, max_length, probe_radius, probe_angle, &motion);
}
