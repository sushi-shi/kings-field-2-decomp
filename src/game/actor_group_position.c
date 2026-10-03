#include <kf/lib/address.h>
#include <kf/game/actor.h>
#include <kf/game/animation.h>
#include <kf/game/player.h>

ADDRESS(0x8003c000, 0x10c)
s32 actor_sample_rotated_animation_vertex(KfActor *actor, s32 vertex_index, VECTOR *output)
{
    struct KfEulerAngles rotation;
    SVECTOR offset;

    if (animation_sample_vertex(actor->unknown_01 + 128, actor->unknown_0c,
                      actor->animation_phase, vertex_index, &offset) != 0) {
        offset.vx = 0;
        offset.vy = -(s32)actor->unknown_1e >> 1;
        offset.vz = -(s32)actor->unknown_1c;
    } else {
        offset.vx = ((s32)offset.vx * (s16)actor->unknown_48) >> KF_FIXED12_BITS;
        offset.vy = ((s32)offset.vy * (s16)actor->unknown_4a.value) >> KF_FIXED12_BITS;
        offset.vz = ((s32)offset.vz * (s16)actor->unknown_4c) >> KF_FIXED12_BITS;
    }
    rotation.x = actor->rotation.x;
    rotation.y = actor->rotation.y + KF_ANGLE_HALF_TURN;
    rotation.z = actor->rotation.z;
    vector_rotate_yxz(&rotation, &offset, output);
    return 1;
}

ADDRESS(0x8003c10c, 0x114)
VECTOR *actor_resolve_group_position(KfActor *actor, VECTOR *output)
{
    KfTargetGroup *group;
    switch (actor->unknown_28 & 3) {
    case 0:
        return &actor->position;
    case 1:
        group = &actor_state.target_groups[actor->group_index];
        setVector(output, actor->position.vx + group->unknown_0c,
                  actor->position.vy + group->unknown_0e,
                  actor->position.vz + group->unknown_10);
        return output;
    case 2:
        group = &actor_state.target_groups[actor->group_index];
        vector_rotate_yxz(&actor->rotation,
                          (SVECTOR *)&group->unknown_0c, output);
        addVector(output, &actor->position);
        return output;
    }
    /* Retail leaves the return register unspecified for mode 3. */
}

ADDRESS(0x8003c220, 0x1c0)
void actor_update_motion_animation(s32 first, s32 reverse, s32 forward, s32 fast,
                   s32 slow, s32 phase_step)
{
    KfActor *actor = actor_state.current;
    s32 selected = first;
    s32 magnitude = 0;
    s16 motion = (s16)actor->unknown_58;

    if (motion > 0) {
        selected = forward;
        magnitude = motion;
    } else if (motion < 0) {
        selected = reverse;
        magnitude = -motion;
    }
    if (actor->unknown_52 >= 11) {
        if (magnitude < actor->unknown_52) {
            selected = fast;
        }
    } else if (actor->unknown_52 < 10) {
        if (magnitude < -actor->unknown_52) {
            selected = slow;
        }
    }

    if (actor->unknown_0c == first) {
        actor_advance_animation_wrapped(actor, phase_step);
        if (actor_animation_crossed_phase(actor, 0)) {
            actor_set_animation_if_changed(selected);
        }
    } else if (actor->unknown_0c == reverse) {
        if (actor->animation_phase < KF_ANGLE_HALF_TURN) {
            actor_advance_animation_clamped(actor, phase_step);
        } else if (selected != actor->unknown_0c) {
            actor_advance_animation_clamped(actor, phase_step);
            if (actor->animation_phase >= KF_ACTOR_ANIMATION_PHASE_MAX) {
                actor_set_animation(selected);
            }
        }
    } else if (actor->unknown_0c == forward) {
        if (actor->animation_phase < KF_ANGLE_HALF_TURN) {
            actor_advance_animation_clamped(actor, phase_step);
        } else if (selected != actor->unknown_0c) {
            actor_advance_animation_clamped(actor, phase_step);
            if (actor->animation_phase >= KF_ACTOR_ANIMATION_PHASE_MAX) {
                actor_set_animation(selected);
            }
        }
    } else if (actor->unknown_0c == fast) {
        if (actor->animation_phase < KF_ANGLE_HALF_TURN) {
            actor_advance_animation_clamped(actor, phase_step);
        } else if (selected != actor->unknown_0c) {
            actor_advance_animation_clamped(actor, phase_step);
            if (actor->animation_phase >= KF_ACTOR_ANIMATION_PHASE_MAX) {
                actor_set_animation(selected);
            }
        }
    } else if (actor->unknown_0c == slow) {
        if (actor->animation_phase < KF_ANGLE_HALF_TURN) {
            actor_advance_animation_clamped(actor, phase_step);
        } else if (selected != actor->unknown_0c) {
            actor_advance_animation_clamped(actor, phase_step);
            if (actor->animation_phase >= KF_ACTOR_ANIMATION_PHASE_MAX) {
                actor_set_animation(selected);
            }
        }
    }
}

ADDRESS(0x8003c3e0, 0x234)
s32 actor_compute_target_direction(KfActor *actor, const VECTOR *origin, s32 step,
                  const VECTOR *target, SVECTOR *direction,
                  s32 pitch_override, u16 yaw_limit, s32 iterations)
{
    VECTOR position = *origin;
    struct KfEulerAngles angles;
    s32 pitch = pitch_override;
    s32 yaw_fraction;
    s32 yaw_error;
    s32 pitch_error;
    s32 distance;
    s32 target_y;

    for (;;) {
        target_y = target->vy + 1600;
        vector_displacement_to_pitch_yaw(position.vx - target->vx,
                      position.vy - target_y,
                      position.vz - target->vz, &angles);
        yaw_error = ((s16)angles.y - (s16)actor->rotation.y) & KF_ANGLE_WRAP_MASK;
        yaw_fraction = yaw_error << KF_FIXED12_BITS;
        if (yaw_error >= KF_ANGLE_HALF_TURN) {
            yaw_error = KF_ANGLE_FULL_TURN - yaw_error;
            yaw_fraction = yaw_error << KF_FIXED12_BITS;
        }
        yaw_fraction /= (s16)yaw_limit;
        if (yaw_fraction > KF_FIXED12_ONE) {
            yaw_fraction = KF_FIXED12_ONE;
        }
        angles.y = angle_lerp_shortest_q12(angles.y, actor->rotation.y, yaw_fraction);

        if ((s16)pitch == -1) {
            pitch_error = ((s16)angles.x - (s16)actor->rotation.x) & KF_ANGLE_WRAP_MASK;
            if (pitch_error >= KF_ANGLE_HALF_TURN) {
                pitch_error = KF_ANGLE_FULL_TURN - pitch_error;
            }
            angles.x = angle_lerp_shortest_q12(angles.x, actor->rotation.x, pitch_error);
        } else {
            angles.x = pitch;
        }

        pitch_yaw_to_forward_vector(&angles, direction);
        vector3s_scale_shift12(step, direction);
        if (--iterations == 0) {
            break;
        }

        distance = fixed_vector3_length(target->vx - position.vx,
                                        target->vy - position.vy,
                                        target->vz - position.vz);
        vector_add_scaled_delta(origin, &player_state.frame_displacement,
                      distance / step, &position);
    }
    return angles.y;
}
