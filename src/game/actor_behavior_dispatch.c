#include <kf/lib/address.h>
#include <kf/game/actor.h>
#include <kf/game/audio.h>
#include <kf/game/callback.h>
#include <kf/game/map_cell.h>
#include <kf/game/map_object.h>
#include <kf/game/player.h>
#include <psyq/libc.h>

extern void func_8002b73c(s32 x, s32 z, s32 radius, s32 amount);
extern s32 func_8002b9d4(s32 x, s32 y, s32 z, s32 radius, s32 height,
                         s32 mode);
extern void func_8003d0e8(KfActor *actor);
extern s32 func_8003b9a4(s32 decay, s32 target);
extern s32 func_8003b33c(SVECTOR *motion);
extern s32 func_8003bae4(s16 angle, s32 speed, s32 step, s32 target);
extern s32 func_8003b520(s32 mode, s32 target_x, s32 target_y,
                         s32 target_z, s32 trajectory_parameter,
                         s32 trajectory_speed);
extern void func_8003b5bc(void);
extern s32 func_8003a614(s32 minimum_distance, s32 maximum_distance,
                          s32 y_offset, s32 angle_tolerance, u16 damage0,
                          u16 damage1, u16 damage2, u16 damage3);
extern s32 func_8003bcd0(s16 angle, s32 speed, s32 range, s32 step,
                         s32 mode, s32 target);
extern s32 func_8003bf74(const struct KfEulerAngles *angles, s32 speed,
                         s32 range, s32 step, s32 mode, s32 target);
extern s32 func_8003bd40(s32 world_x, s32 world_z, s32 speed, s32 range,
                         s16 reference_angle, s32 step, s32 mode, s32 target);
extern void func_8003bba0(KfActor *actor, s32 target_angle, s32 max_speed,
                          s32 acceleration);
extern void actor_reset_target_and_reselect(void);
extern void func_8003c220(s32 first, s32 reverse, s32 forward, s32 fast,
                          s32 slow, s32 phase_step);
extern void func_8003c614(s32 kind, s32 effect_id, s32 position_mode, ...);

RODATA(0x800120d8, 0x3c4)

ADDRESS(0x8003d184, 0x248c)
void func_8003d184(void)
{
    KfActor *actor = actor_state.current;
    KfTargetGroup *group = actor_state.active_group;
    KfTargetCandidate *target = actor->target;
    u16 trigger;
    s32 interval;

    actor_state.unknown_93a4 = (actor->unknown_28 & 4) == 0 ? 0x93 : 3;
    func_8002b73c(actor->position.vx, actor->position.vz,
                   actor->unknown_1c, -1);

    if (target->unknown_04 != 0xff) {
        trigger = target->unknown_0a;
        interval = trigger & 0x3fff;
        switch (trigger & 0xc000) {
        case 0:
            if (actor_animation_crossed_phase(actor, interval)) {
                goto play_sound;
            }
            break;
        case 0x8000:
            if ((rand() >> 3) < interval) {
                goto play_sound;
            }
            break;
        case 0x4000:
            if ((interval * actor_state.unknown_93b8 / 3) % interval ==
                (s32)actor_state.unknown_93c4 % interval) {
                goto play_sound;
            }
            break;
        }
    }
    goto dispatch_action;

play_sound:
    func_8003d0e8(actor);

dispatch_action:

    switch (actor->target_type) {
    case 2:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            func_800397d8(target->unknown_01[0]);
        }
        if (actor->animation_phase < 0x800 ||
            (actor->unknown_28 & 0x800) == 0) {
            actor_advance_animation_clamped(actor, target->unknown_08);
        }
        if (actor->animation_phase > 0xffe) {
            actor_reset_target_and_reselect();
        }
        func_8003b9a4(group->unknown_01[2] * 2, 10);
        break;
    case 3:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            func_800397d8(target->unknown_01[0]);
        }
        actor_advance_animation_clamped(actor, target->unknown_08);
        if (actor_animation_crossed_phase(actor, target->word_18.value)) {
            func_8003a614(0, target->word_0e.bytes.low,
                           target->word_0e.bytes.high,
                           target->word_10.bytes.fallback_offset,
                           target->word_12.value, target->word_14.value,
                           target->word_16.value,
                           target->word_10.bytes.unknown_11);
        }
        if (actor->animation_phase >= 0xfff) {
            actor_reset_target_and_reselect();
        }
        func_8003b9a4(target->word_0c.bytes.high, 10);
        break;
    case 0:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            func_80039804(target->unknown_01[0]);
        }
        actor_advance_animation_wrapped(actor, target->unknown_08);
        if (angle_within_tolerance(actor->animation_phase, 0,
                                   target->unknown_08 - 1)) {
            actor->unknown_0f = 0xf1;
        }
        func_8003b9a4(group->unknown_01[2], 10);
        break;
    case 1:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf1;
            func_80039804(target->unknown_01[0]);
            actor->unknown_64 = rand() >> 3;
        } else if (func_8003bcd0(actor->unknown_64,
                                  target->word_0c.value, target->word_0e.value,
                                  group->unknown_01[2],
                                  group->unknown_01[3], 5) != 0 ||
                   (rand() >> 5) < target->word_10.bytes.fallback_offset) {
            actor->unknown_64 = rand() >> 3;
        }
        actor_advance_animation_wrapped(actor, target->unknown_08);
        break;
    case 12:
    case 16: {
        s32 motion_flags;

        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf1;
            actor->tail_72.angles.x = 0;
            actor->tail_72.angles.y = 0;
            actor->tail_72.angles.z = 0;
            actor->tail_72.motion.baseline = (u16)actor->unknown_26 +
                func_8002b67c(actor->unknown_06,
                    actor->unknown_24 + (actor->unknown_07[1] << 11),
                    actor->unknown_22 + (actor->unknown_07[0] << 11),
                    actor->unknown_1c, actor->unknown_1e);
            func_80039804(target->unknown_01[0]);
            func_8003b5bc();
            motion_flags = 3;
        } else {
            motion_flags = func_8003bf74(
                &actor->tail_72.angles, target->word_0c.value,
                target->word_0e.value,
                group->unknown_01[2],
                group->unknown_01[3], interval & 1);
            if ((rand() >> 5) < target->word_12.bytes.marker_state) {
                motion_flags |= 1;
            }
            if ((rand() >> 5) < target->word_12.bytes.marker_state) {
                motion_flags |= 2;
            }
            if (actor->position.vy <
                actor->tail_72.motion.baseline + target->word_10.value) {
                actor->unknown_52 -= target->word_12.bytes.unknown_12;
            } else if (actor->position.vy >
                       actor->tail_72.motion.baseline - target->word_10.value) {
                actor->unknown_52 += target->word_12.bytes.unknown_12;
            }
        }
        if (motion_flags & 1) {
            actor->tail_72.angles.y = rand() >> 3;
        }
        if (motion_flags & 2) {
            actor->tail_72.angles.x = (rand() >> 5) - 512;
        }
        if (actor->target_type == 16) {
            func_8003c220(target->unknown_01[0], target->word_14.bytes[0],
                           target->word_14.bytes[1], target->word_16.bytes.low,
                           target->word_16.bytes.high, target->unknown_08);
        } else {
            actor_advance_animation_wrapped(actor, target->unknown_08);
        }
        break;
    }
    case 5: {
        s32 distance;
        s32 angle;
        s32 mode;
        s32 motion_flags;

        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf1;
            func_80039804(target->unknown_01[0]);
            actor->state_70.bytes.low =
                actor->previous_target_type == 4 ||
                actor->previous_target_type == 18 ||
                actor->previous_target_type == 23 ||
                actor->previous_target_type == 24;
            actor->state_70.bytes.high = 0;
        }
        if (rand() < 6000) {
            distance = fixed_vector2_length(
                player_state.camera_position.vx - actor->position.vx,
                player_state.camera_position.vz - actor->position.vz);
            if (distance >= target->word_16.value) {
                actor->state_70.bytes.low = 0;
            } else if (distance > target->word_14.value) {
                actor->state_70.bytes.low = 1;
            }
        }
        angle = vector_xz_to_angle(
            player_state.camera_position.vx - actor->position.vx,
            player_state.camera_position.vz - actor->position.vz);
        actor->unknown_64 = angle;
        mode = actor->state_70.bytes.high == 0 ? 5 : 33;
        if (actor->state_70.bytes.low == 0) {
            motion_flags = func_8003bcd0((s16)angle,
                target->word_0c.value, target->word_0e.value,
                group->unknown_01[2],
                group->unknown_01[3], mode);
        } else {
            motion_flags = func_8003bae4((s16)angle + 2048,
                target->word_0c.value,
                group->unknown_01[2], mode);
        }
        if (motion_flags & 0x100) {
            if (actor->state_70.bytes.high == 0 &&
                rand() < target->word_18.value) {
                actor->state_70.bytes.high = 1;
            }
        } else {
            actor->state_70.bytes.high = 0;
            if (motion_flags != 0) {
                actor->state_70.bytes.low = actor->state_70.bytes.low == 0;
            }
        }
        actor_advance_animation_wrapped(actor, target->unknown_08);
        break;
    }
    case 13:
    case 17: {
        s32 delta_x;
        s32 delta_y;
        s32 delta_z;
        s32 distance;
        struct KfEulerAngles opposite;

        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            func_80039804(target->unknown_01[0]);
            actor->state_70.signed_state = 0;
        }
        delta_x = player_state.camera_position.vx - actor->position.vx;
        delta_y = player_state.camera_position.vy - actor->position.vy - 1600;
        delta_z = player_state.camera_position.vz - actor->position.vz;
        distance = fixed_vector3_length(delta_x, delta_y, delta_z);
        func_800154fc(delta_x, delta_y, delta_z, &actor->tail_72.angles);
        if (actor->tail_72.angles.x >= 3585) {
            actor->tail_72.angles.x = 3584;
        } else if (actor->tail_72.angles.x <= 512) {
            actor->tail_72.angles.x = 512;
        }
        if (distance >= target->word_16.value) {
            actor->state_70.signed_state = 0;
            actor->unknown_0f = 0xf1;
        } else if (distance <= target->word_14.value) {
            actor->state_70.signed_state = 1;
            actor->unknown_0f = 0xf0;
        }
        if (actor->state_70.signed_state == 0) {
            func_8003bf74(&actor->tail_72.angles, target->word_0c.value,
                          target->word_0e.value,
                          group->unknown_01[2],
                          group->unknown_01[3], 17);
        } else {
            opposite.x = -512;
            opposite.y = actor->tail_72.angles.y + 2048;
            opposite.z = 0;
            func_8003bf74(&opposite, target->word_0c.value,
                          target->word_0e.value,
                          group->unknown_01[2],
                          group->unknown_01[3], 17);
        }
        if (actor->target_type == 17) {
            func_8003c220(target->unknown_01[0], target->word_18.bytes.low,
                           target->word_18.bytes.high, target->word_1a.bytes[0],
                           target->word_1a.bytes[1], target->unknown_08);
        } else {
            actor_advance_animation_wrapped(actor, target->unknown_08);
        }
        break;
    }
    case 9:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            actor->state_70.signed_state = 0;
            func_800397d8(target->word_14.bytes[0]);
        }
        switch (actor->state_70.signed_state) {
        case 0:
            if (func_8003bae4(actor->rotation.y + 2048,
                              target->word_12.value,
                              group->unknown_01[2], 5) == 0) {
                actor_advance_animation_wrapped(actor, target->unknown_08);
                if (fixed_vector2_length(
                        player_state.camera_position.vx - actor->position.vx,
                        player_state.camera_position.vz - actor->position.vz)
                    < target->word_0e.value) {
                    break;
                }
            }
            actor->state_70.signed_state = 1;
            func_800397d8(target->unknown_01[0]);
            break;
        case 1:
            actor_advance_animation_clamped(actor, target->word_16.bytes.low);
            if (actor->animation_phase >= 0xfff) {
                if ((s16)func_8003b520(
                        1, player_state.camera_position.vx,
                        player_state.camera_position.vy - 500,
                        player_state.camera_position.vz,
                        target->word_16.bytes.high,
                        target->word_10.value) <= 0) {
                    actor->state_70.signed_state = 3;
                    func_800397d8(target->word_14.bytes[1]);
                    break;
                }
                actor->state_70.signed_state = 2;
            }
            func_8003b9a4(group->unknown_01[2] * 2, 10);
            break;
        case 2:
            if (actor->unknown_0d == 0) {
                actor->state_70.signed_state = 3;
                func_800397d8(target->word_14.bytes[1]);
            } else {
                func_8003bcd0(actor->rotation.y, actor->unknown_68, 0,
                              actor->unknown_68,
                              group->unknown_01[3], 10);
            }
            break;
        case 3:
            actor_advance_animation_clamped(actor, target->word_16.bytes.low);
            if (actor->animation_phase >= 0xfff) {
                actor_reset_target_and_reselect();
            }
            func_8003b9a4(actor->unknown_68 >> 4, 10);
            break;
        }
        break;
    case 10: {
        s32 random_value;

        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf1;
            func_80039804(target->unknown_01[0]);
            func_8003b5bc();
        }
        random_value = rand();
        if (random_value < 2048) {
            actor->unknown_50 += target->word_10.value;
            if (actor->unknown_50 > target->word_0c.value) {
                actor->unknown_50 = target->word_0c.value;
            }
        } else if (random_value < 4096) {
            actor->unknown_50 -= target->word_10.value;
            if (actor->unknown_50 < -(s32)target->word_0c.value) {
                actor->unknown_50 = -target->word_0c.value;
            }
        }
        random_value = rand();
        if (random_value < 2048) {
            actor->unknown_54 += target->word_10.value;
            if (actor->unknown_54 > target->word_0c.value) {
                actor->unknown_54 = target->word_0c.value;
            }
        } else if (random_value < 4096) {
            actor->unknown_54 -= target->word_10.value;
            if (actor->unknown_54 < -(s32)target->word_0c.value) {
                actor->unknown_54 = -target->word_0c.value;
            }
        }
        random_value = rand();
        if (random_value < 2048) {
            actor->unknown_52 += target->word_10.value;
            if (actor->unknown_52 > target->word_0c.value) {
                actor->unknown_52 = target->word_0c.value;
            }
        } else if (random_value < 4096) {
            actor->unknown_52 -= target->word_10.value;
            if (actor->unknown_52 < -(s32)target->word_0c.value) {
                actor->unknown_52 = -target->word_0c.value;
            }
        }
        func_8003b33c((SVECTOR *)&actor->unknown_50);
        actor_advance_animation_wrapped(actor, target->unknown_08);
        func_8003bba0(actor, vector_xz_to_angle(actor->unknown_50,
                                                actor->unknown_54),
                        target->word_0e.value,
                        group->unknown_01[3]);
        break;
    }
    case 4:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            func_800397d8(target->unknown_01[0]);
        }
        actor_advance_animation_clamped(actor, target->unknown_08);
        if (actor_animation_crossed_phase(actor, target->word_18.value)) {
            func_8003a614(0, target->word_0e.bytes.low,
                           target->word_0e.bytes.high,
                           target->word_10.bytes.fallback_offset,
                           target->word_12.value, target->word_14.value,
                           target->word_16.value,
                           target->word_10.bytes.unknown_11);
        }
        if (actor->animation_phase >= 0xfff) {
            actor_reset_target_and_reselect();
        }
        func_8003b9a4(target->word_0c.bytes.high, 10);
        break;
    case 23:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            actor->state_70.signed_state = target->word_18.value;
            func_800397d8(target->unknown_01[0]);
        }
        actor_advance_animation_clamped(actor, target->unknown_08);
        if (actor_animation_crossed_phase(actor, actor->state_70.signed_state)) {
            actor->state_70.signed_state = (u16)actor->state_70.signed_state + target->unknown_28;
            if (target->word_26.unsigned_value < actor->state_70.signed_state) {
                actor->state_70.signed_state = 0;
            }
            func_8003a614(0, target->word_0e.bytes.low,
                           target->word_0e.bytes.high,
                           target->word_10.bytes.fallback_offset,
                           target->word_12.value, target->word_14.value,
                           target->word_16.value,
                           target->word_10.bytes.unknown_11 | 0x80);
        }
        if (target->unknown_2a != 0 &&
            actor_animation_crossed_phase(actor, target->unknown_2a)) {
            func_8003a614(0, target->word_1c.bytes[0],
                           target->word_1c.bytes[1],
                           target->word_1e.bytes[0], target->unknown_20,
                           target->unknown_22, target->word_24.unsigned_value,
                           target->word_1e.bytes[1]);
        }
        if (actor->animation_phase >= 0xfff) {
            actor_reset_target_and_reselect();
        }
        func_8003b9a4(target->word_0c.bytes.high, 10);
        break;
    case 24: {
        s32 speed;
        s32 step;
        s32 angle;
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            func_800397d8(target->unknown_01[0]);
        }
        actor_advance_animation_clamped(actor, target->unknown_08);
        if (actor->animation_phase >= target->unknown_22) {
            speed = 0;
            step = target->word_26.signed_value;
        } else if (actor->animation_phase >= target->unknown_20) {
            speed = target->word_1c.value;
            step = target->word_24.signed_value;
        } else {
            speed = target->word_1c.value;
            step = target->word_0c.bytes.high;
        }
        angle = vector_xz_to_angle(
            player_state.camera_position.vx - actor->position.vx,
            player_state.camera_position.vz - actor->position.vz);
        actor->unknown_64 = angle;
        func_8003bcd0(actor->unknown_64, speed,
                      target->word_1e.value, step,
                      group->unknown_01[3], 4);
        if (actor_animation_crossed_phase(actor, target->word_18.value)) {
            func_8003a614(0, target->word_0e.bytes.low,
                           target->word_0e.bytes.high,
                           target->word_10.bytes.fallback_offset,
                           target->word_12.value, target->word_14.value,
                           target->word_16.value,
                           target->word_10.bytes.unknown_11);
        }
        if (actor->animation_phase >= 0xfff) {
            actor_reset_target_and_reselect();
        }
        break;
    }
    case 11: {
        s32 collision;
        s32 stage;
        struct KfEulerAngles toward_player;
        SVECTOR forward;
        SVECTOR outer;

        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            func_80039804(target->unknown_01[0]);
            func_8003b5bc();
            actor->state_70.signed_state = 0;
        }
        stage = actor->state_70.signed_state;
        switch (stage) {
        case 0:
            actor_advance_animation_clamped(actor, target->unknown_08);
            if (actor->animation_phase >= 0xfff) {
                func_800154fc(
                    player_state.camera_position.vx - actor->position.vx,
                    player_state.camera_position.vy - actor->position.vy,
                    player_state.camera_position.vz - actor->position.vz,
                    &toward_player);
                pitch_yaw_to_forward_vector(&toward_player,
                                            &actor->tail_72.direction);
                actor->state_70.signed_state = 1;
            }
            break;
        case 1:
            forward = actor->tail_72.direction;
            outer = forward;
            vector3s_scale_shift12(target->word_14.value, &forward);
            vector3s_scale_shift12(target->word_18.value, &outer);
            actor->unknown_50 = value_approach(actor->unknown_50,
                                                forward.vx, outer.vx);
            actor->unknown_52 = value_approach(actor->unknown_52,
                                                forward.vy, outer.vy);
            actor->unknown_54 = value_approach(actor->unknown_54,
                                                forward.vz, outer.vz);
            collision = func_8003b33c((SVECTOR *)&actor->unknown_50);
            if (collision != 0) {
                if (collision & 0x80) {
                    func_800248a8(target->word_0e.value,
                                   target->word_10.value,
                                   target->word_12.value,
                                   target->word_0c.bytes.high,
                                   0, 0, 0, 0, 0, 0x1000, 10,
                                   &actor->position);
                }
                actor->state_70.signed_state = 2;
            }
            actor->rotation.x = (actor->rotation.x + 256) & 0xfff;
            break;
        case 2:
            actor->rotation.x = (actor->rotation.x + 256) & 0xfff;
            if (actor->rotation.x < 256) {
                actor->rotation.x = 0;
                actor->state_70.signed_state = 3;
            }
            break;
        case 3:
            actor_advance_animation_clamped(actor, -target->unknown_08);
            if (actor->animation_phase == 0) {
                actor_reset_target_and_reselect();
            }
            break;
        }
        if (stage == 0 || stage == 2 || stage == 3) {
            func_8003b33c((SVECTOR *)&actor->unknown_50);
        }
        func_8003bba0(actor, vector_xz_to_angle(actor->unknown_50,
                                                actor->unknown_54),
                        target->word_16.value,
                        group->unknown_01[3]);
        break;
    }
    case 18: {
        s32 delta_x;
        s32 delta_y;
        s32 delta_z;

        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            func_80039804(target->unknown_01[0]);
        }
        delta_x = player_state.camera_position.vx - actor->position.vx;
        delta_y = player_state.camera_position.vy - actor->position.vy - 1600;
        delta_z = player_state.camera_position.vz - actor->position.vz;
        fixed_vector3_length(delta_x, delta_y, delta_z);
        func_800154fc(delta_x, delta_y, delta_z, &actor->tail_72.angles);
        func_8003bf74(&actor->tail_72.angles, target->word_1c.value,
                      target->word_1e.value,
                      target->word_0c.bytes.high,
                      group->unknown_01[3], 6);
        actor_advance_animation_clamped(actor, target->unknown_08);
        if (actor_animation_crossed_phase(actor, target->word_18.value)) {
            func_8003a614(0, target->word_0e.bytes.low,
                           target->word_0e.bytes.high,
                           target->word_10.bytes.fallback_offset,
                           target->word_12.value, target->word_14.value,
                           target->word_16.value,
                           target->word_10.bytes.unknown_11);
        }
        if (actor->animation_phase >= 0xfff) {
            actor_reset_target_and_reselect();
        }
        break;
    }
    case 14:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf1;
            actor->state_70.signed_state = 0;
        }
        switch (actor->state_70.signed_state) {
        case 0:
            if ((s16)actor_state.other_actor->unknown_58 > 0) {
                func_800397d8(target->word_0c.bytes.high);
                actor->state_70.signed_state = 1;
            } else if ((s16)actor_state.other_actor->unknown_58 < 0) {
                func_800397d8(target->word_0c.bytes.low);
                actor->state_70.signed_state = 2;
            }
            break;
        case 1:
            if (actor->animation_phase < 2048) {
                actor->animation_phase += 128;
            } else if ((s16)actor_state.other_actor->unknown_58 <= 0) {
                actor->state_70.signed_state = 3;
            }
            break;
        case 2:
            if (actor->animation_phase < 2048) {
                actor->animation_phase += 128;
            } else if ((s16)actor_state.other_actor->unknown_58 >= 0) {
                actor->state_70.signed_state = 3;
            }
            break;
        case 3:
            actor->animation_phase -= 128;
            if (actor->animation_phase == 0) {
                actor->state_70.signed_state = 0;
            }
            break;
        }
        break;
    case 15:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            func_80039804(target->unknown_01[0]);
        }
        func_8003bcd0(actor->unknown_64,
                      target->word_0c.value, target->word_0e.value,
                      group->unknown_01[2],
                      group->unknown_01[3], 5);
        actor_advance_animation_clamped(actor, target->unknown_08);
        if (actor->animation_phase >= 0xfff) {
            actor_reset_target_and_reselect();
        }
        break;
    case 19:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            actor->state_70.signed_state = 0;
            func_80039804(target->unknown_01[0]);
        }
        switch (actor->state_70.signed_state) {
        case 0:
            actor_advance_animation_clamped(actor, target->unknown_08);
            if (actor->animation_phase >= target->word_10.value) {
                actor->state_70.signed_state = 16;
            }
            break;
        case 16:
            if (player_state.weapon_attack_phase == -1) {
                actor->state_70.signed_state = 32;
            }
            break;
        case 32:
            actor_advance_animation_clamped(actor, target->unknown_08);
            if (actor->animation_phase >= 0xfff) {
                actor_reset_target_and_reselect();
            }
            break;
        }
        func_8003b9a4(group->unknown_01[3], 10);
        break;
    case 20:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            actor->state_70.signed_state = target->word_12.value;
            func_80039804(target->unknown_01[0]);
        }
        if (func_8003bae4(actor->rotation.y + 2048,
                          target->word_10.value, 1000, 5) != 0) {
            actor->state_70.signed_state = 1;
        }
        actor_advance_animation_wrapped(actor, target->unknown_08);
        if (actor->animation_phase >= 0xfff - target->unknown_08) {
            actor->state_70.signed_state--;
            if (actor->state_70.signed_state == 0) {
                actor->animation_phase = 0xfff;
                actor_reset_target_and_reselect();
            }
        }
        break;
    case 21:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            actor->state_70.signed_state = 0;
            func_800397d8(target->unknown_01[0]);
            actor->unknown_1c = target->word_14.value;
            actor->unknown_1e = target->word_16.value;
        }
        switch (actor->state_70.signed_state) {
        case 0:
            if (fixed_vector2_length(player_state.camera_position.vx - actor->position.vx,
                                     player_state.camera_position.vz - actor->position.vz)
                < target->word_0c.value) {
                actor->state_70.signed_state = 1;
            }
            break;
        case 1:
            actor_advance_animation_clamped(actor, target->unknown_08);
            actor->unknown_1c = func_8001584c(target->word_14.value,
                group->unknown_12, actor->animation_phase);
            actor->unknown_1e = func_8001584c(target->word_16.value,
                group->unknown_14, actor->animation_phase);
            if (actor->animation_phase >= 0xfff) {
                actor->unknown_1c = group->unknown_12;
                actor->unknown_1e = group->unknown_14;
                actor->state_70.signed_state = 2;
                actor->tail_72.signed_state = target->word_10.value;
                func_800397d8(target->word_18.bytes.low);
            }
            if (actor->animation_phase >= target->word_12.value) {
                s32 saved_flags = actor_state.unknown_93a4;
                actor_state.unknown_93a4 &= ~0x20;
                func_8003bcd0(actor->unknown_64,
                               target->word_0e.value, 0, 0xff, 0, 5);
                actor_state.unknown_93a4 = saved_flags;
            }
            break;
        case 2: {
            s32 saved_flags = actor_state.unknown_93a4;
            actor_state.unknown_93a4 &= ~0x20;
            func_8003bcd0(actor->unknown_64,
                           target->word_0e.value, 0, 0xff, 0, 5);
            actor->tail_72.unsigned_state--;
            if ((s16)actor->tail_72.unsigned_state == -1) {
                actor_reset_target_and_reselect();
            }
            actor_state.unknown_93a4 = saved_flags;
            actor_advance_animation_wrapped(actor, target->word_18.bytes.high);
            break;
        }
        }
        break;
    case 22:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            func_800397d8(target->unknown_01[0]);
        }
        actor_advance_animation_clamped(actor, target->unknown_08);
        if (actor->animation_phase >= 0xfff) {
            actor_reset_target_and_reselect();
        }
        func_8003b9a4(group->unknown_01[3], 10);
        break;
    case 26:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            actor->state_70.signed_state = 0;
            func_800397d8(target->unknown_01[0]);
            actor->unknown_48 = 0;
            actor->unknown_4a.value = 0;
            actor->unknown_4c = 0;
        }
        switch (actor->state_70.signed_state) {
        case 0: {
            KfMapObject *object = &map_object_state.objects[actor->unknown_20];
            if (object->action_timer < 2 || object->rotation.vx <= 0 ||
                object->rotation.vx >= 3072) {
                break;
            }
            actor->state_70.signed_state = 1;
            actor->tail_72.signed_state = 8;
            actor->unknown_1c = group->unknown_12;
            actor->position.vy += 2048;
            actor->unknown_1e = group->unknown_14;
            actor->unknown_48 = group->unknown_32;
            actor->unknown_4a.value = group->unknown_32;
            actor->unknown_4c = group->unknown_32;
            actor->rotation.y = object->rotation.vy;
            actor->unknown_0d = 0;
        }
            /* fall through */
        case 1:
            actor->position.vy -= 256;
            actor->tail_72.unsigned_state--;
            if ((s16)actor->tail_72.unsigned_state == 0) {
                actor_reset_target_and_reselect();
            }
            break;
        }
        break;
    case 25: {
        const KfTargetCandidateAction25 *script =
            (const KfTargetCandidateAction25 *)target;
        const u16 *cursor;
        s32 repeat;

        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            func_800397d8(target->unknown_01[0]);
            actor->state_70.signed_state = target->word_0e.value;
            actor->tail_72.script.word_index = 0;
            actor->tail_72.script.unknown_74 = 0;
            if (target->unknown_05[2] == 1) {
                actor->unknown_0d = 16;
            }
        }
        actor_advance_animation_clamped(actor, target->unknown_08);
        if (actor_animation_crossed_phase(actor,
                                          actor->state_70.signed_state)) {
            actor->state_70.signed_state =
                (u16)actor->state_70.signed_state + target->word_12.value;
            repeat = 1;
            if (target->word_10.value < actor->state_70.signed_state) {
                actor->state_70.signed_state = 0;
            }
            cursor = script->stream + (s16)actor->tail_72.script.word_index;
            for (;;) {
                u16 opcode = *cursor++;
                u16 index = actor->tail_72.script.word_index;
                actor->tail_72.script.word_index = index + 1;

                switch (opcode) {
                case 0x8000:
                    cursor = script->stream;
                    actor->tail_72.script.word_index = 0;
                    continue;
                case 0x8001:
                    repeat = *cursor++;
                    actor->tail_72.script.word_index = index + 2;
                    continue;
                case 0x8003: {
                    u16 skip = *cursor;
                    cursor += skip + 1;
                    continue;
                }
                case 0x8002: {
                    s16 x = *cursor++;
                    s16 y = *cursor++;
                    s16 z = *cursor++;
                    actor->tail_72.script.word_index = index + 4;
                    func_8003c614(target->word_0c.bytes.low,
                                   target->word_18.value, -1, x, y, z, cursor);
                    break;
                }
                case 0x8004: {
                    u16 first = *cursor++;
                    u16 second = *cursor++;
                    u16 third = *cursor++;
                    actor->tail_72.script.word_index = index + 4;
                    func_8003c614(target->word_0c.bytes.low,
                                   target->word_18.value, -2,
                                   first, second, third);
                    break;
                }
                default:
                    func_8003c614(target->word_0c.bytes.low,
                                   target->word_18.value, opcode,
                                   cursor + repeat - 1,
                                   (s16)actor->tail_72.script.unknown_74);
                    break;
                }
                if (--repeat == 0) {
                    break;
                }
            }
            actor->tail_72.script.unknown_74++;
        }
        if (actor->animation_phase >= 0xfff) {
            actor_reset_target_and_reselect();
        }
        func_8003b9a4(group->unknown_01[2], 10);
        break;
    }
    case 27:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf1;
            actor->state_70.signed_state = 0;
            actor->tail_72.signed_state = -1;
            func_800397d8(target->word_16.bytes.low);
        }
        switch (actor->state_70.signed_state) {
        case 0:
            if (target->word_0e.value == 0) {
                actor->state_70.signed_state = 1;
            } else {
                actor->tail_72.signed_state = func_8003bd40(
                    actor->unknown_24 + (actor->unknown_07[1] << 11),
                    actor->unknown_22 + (actor->unknown_07[0] << 11),
                    target->word_0e.value, target->word_10.value,
                    actor->tail_72.signed_state,
                    group->unknown_01[2],
                    group->unknown_01[3], 5);
                if (actor->tail_72.signed_state == -1) {
                    actor->state_70.signed_state = 1;
                }
            }
            actor_advance_animation_wrapped(actor, target->word_16.bytes.high);
            break;
        case 1:
            func_8003bba0(actor, actor->unknown_20, target->word_10.value,
                          group->unknown_01[3]);
            if (actor->rotation.y == actor->unknown_20) {
                func_800397d8(target->unknown_01[0]);
                actor->state_70.signed_state = 2;
            }
            break;
        case 2: {
            KfTargetCandidate *next_target;
            actor_advance_animation_clamped(actor, target->unknown_08);
            actor->unknown_1c = func_8001584c(
                group->unknown_12,
                target->word_12.value, actor->animation_phase);
            actor->unknown_1e = func_8001584c(
                group->unknown_14,
                target->word_14.value, actor->animation_phase);
            if (actor->animation_phase >= 0xfff) {
                actor->unknown_1c = target->word_12.value;
                actor->unknown_1e = target->word_14.value;
                next_target = actor_find_target_of_type(group, 21);
                if (next_target != 0) {
                    actor_set_target(actor, next_target);
                } else {
                    actor_reset_target_and_reselect();
                }
            }
            break;
        }
        }
        break;
    case 29:
        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            func_800397d8(target->unknown_01[0]);
            func_8003b5bc();
            actor->tail_72.signed_state = 16;
        }
        actor->rotation.x += 128;
        actor->rotation.y += 64;
        actor->position.vy -= 256;
        actor->tail_72.unsigned_state--;
        if ((s16)actor->tail_72.unsigned_state == 0) {
            actor_reset_target_and_reselect();
        }
        break;
    case 28: {
        s32 collision;

        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            func_800397d8(target->unknown_01[0]);
            func_8003b5bc();
            audio_play_spatial_default_range(0x1b, &actor->position, 120, 0);
        }
        actor->rotation.x += 128;
        actor->rotation.y += 64;
        collision = func_8003b33c((SVECTOR *)&actor->unknown_50);
        if (collision != 0) {
            if (collision & 0x80) {
                func_800248a8(target->word_0e.value, target->word_10.value,
                               target->word_12.value, target->word_0c.value,
                               target->word_14.value, target->word_16.value,
                               target->word_18.value, target->word_1a.value,
                               target->word_1c.value, 0x1000, 10, &actor->position);
            }
            actor_select_target_type_in_own_group(actor, 3);
            actor->unknown_50 = 0;
            actor->unknown_52 = 0;
            actor->unknown_54 = 0;
            actor->unknown_0d = 16;
        }
        break;
    }
    case 30: {
        VECTOR next;
        s32 collision;

        if (actor->unknown_0f == 0) {
            actor->unknown_0f = 0xf0;
            func_800397d8(target->unknown_01[0]);
            func_8003b5bc();
        }
        next.vx = actor->position.vx + actor->unknown_50;
        next.vy = actor->position.vy + actor->unknown_52;
        next.vz = actor->position.vz + actor->unknown_54;
        collision = func_8002b9d4(
            next.vx, next.vy, next.vz, actor->unknown_1c,
            actor->unknown_1e | ((actor->unknown_28 & 0xc000) << 16),
            actor_state.unknown_93a4);
        if (collision & 0x80) {
            func_800248a8(target->word_0e.value, target->word_10.value,
                           target->word_12.value, target->word_0c.value,
                           target->word_14.value, target->word_16.value,
                           target->word_18.value, target->word_1a.value,
                           target->word_1c.value, 0x1000, 10, &actor->position);
        }
        if (collision == 0 || (collision & 0x80)) {
            actor->position.vx = next.vx;
            actor->position.vy = next.vy;
            actor->position.vz = next.vz;
        } else {
            actor->unknown_50 = 0;
            actor->unknown_52 = 0;
            actor->unknown_54 = 0;
        }
        actor_advance_animation_clamped(actor, target->unknown_08);
        if (actor->animation_phase >= 0xfff) {
            actor_select_target_type_in_own_group(actor, 3);
            actor->unknown_0d = 16;
        }
        break;
    }
    case 240:
        func_8003b5bc();
        break;
    case 6:
    case 7:
    case 8:
        state_8017d118.active_table[17]();
        break;
    default:
        if (actor->target_type >= 31 && actor->target_type != 240) {
            state_8017d118.active_table[17]();
        }
        break;
    }
}
