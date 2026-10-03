#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/actor.h>
#include <kf/game/effect.h>
#include <kf/game/player.h>

ADDRESS(0x8004177c, 0x1e0)
s32 effect_move_probe(s32 scale, s32 max_length, s32 probe_radius, s32 probe_angle,
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

ADDRESS(0x8004195c, 0x1b8)
s32 effect_aim_and_move(s32 max_length, s32 scale, s32 turn_step,
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
    target = actor_find_best_in_cone(&record->position, record->rotation.vy,
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
    if (distance != -1) {
        target_position.vx = player_state.camera_position.vx;
        target_position.vy = player_state.camera_position.vy - 1600;
        target_position.vz = player_state.camera_position.vz;
        goto aim;
    }

move:
    pitch_yaw_to_forward_vector((const struct KfEulerAngles *)&record->rotation,
                                &motion);
    if (distance >= 0 && distance <= proximity) {
        scale = close_scale;
    }
    return effect_move_probe(scale, max_length, probe_radius, probe_angle, &motion);
}

ADDRESS(0x80041b14, 0x1bc)
s32 effect_target_motion(const VECTOR *target, s32 max_length, s32 scale,
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
        record->direction.vx = fixed_lerp_q12(0, record->direction.vx, 0xc00);
        record->direction.vy = fixed_lerp_q12(0, record->direction.vy, 0xc00);
        record->direction.vz = fixed_lerp_q12(0, record->direction.vz, 0xc00);
    }
    motion.vx = (delta.vx << KF_FIXED12_BITS) / length;
    motion.vy = (delta.vy << KF_FIXED12_BITS) / length;
    motion.vz = (delta.vz << KF_FIXED12_BITS) / length;
    return effect_move_probe(scale, max_length, probe_radius, probe_angle, &motion);
}
