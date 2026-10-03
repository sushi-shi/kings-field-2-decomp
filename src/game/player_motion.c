#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/lib/null.h>
#include <kf/game/audio.h>
#include <kf/game/collision_cache.h>
#include <kf/game/effect.h>
#include <kf/game/player.h>
#include <psyq/sdk.h>

RODATA(0x80011298, 0x64)

ADDRESS(0x8002722c, 0x2c0)
void player_select_magic_action(s32 magic_id)
{
    KfMagicRecord *record;
    u16 mp_cost;

    if (player_state.queued_magic_action.magic_id != KF_PLAYER_MAGIC_ACTION_NONE ||
        magic_id == KF_PLAYER_MAGIC_ACTION_NONE) {
        return;
    }

    record = &effect_state.magic_records[magic_id];
    if (player_state.vitals.current_mp < record->mp_cost) {
        return;
    }

    if (player_state.equipped_weapon_id == 12 && magic_id < 11) {
        if (magic_id >= 7) {
            return;
        }
    }
    if (player_state.equipped_body_id == 31 && magic_id >= 11) {
        if (magic_id < 13) {
            return;
        }
        if (magic_id < 20) {
            if (magic_id >= 18) {
                return;
            }
        }
    }

    switch (magic_id - 14) {
    case 0:
    case 2:
    case 5:
        break;
    case 1:
        if (player_state.defense_boost_timer != 0) {
            return;
        }
        break;
    case 3:
        if (player_state.attack_boost_timer != 0) {
            return;
        }
        break;
    case 4:
        player_state.magic_tint_phase_limit = 900;
        player_state.vitals.current_mp -= record->mp_cost;
        return;
    default:
        goto charge_gate;
    }
    player_state.queued_magic_action.casts_remaining = 1;
    player_state.queued_magic_action.repeat_interval = 1;

charge_gate:
    if (player_state.magic_charge < KF_PLAYER_CHARGE_FULL) {
        return;
    }
    player_state.magic_charge = 0;
    mp_cost = record->mp_cost;
    player_state.queued_magic_action.magic_id = magic_id;
    player_state.magic_origin_offset.vx = -200;
    player_state.magic_origin_offset.vy = 200;
    player_state.magic_origin_offset.vz = 400;
    player_state.queued_magic_action.countdown = 1;
    player_state.vitals.current_mp -= mp_cost;

    switch (magic_id) {
    case 10:
        player_state.magic_origin_offset.vx = 0;
        player_state.magic_origin_offset.vy = -512;
        player_state.magic_origin_offset.vz = 2000;
        /* Retail falls through to the shared action-byte stores. */
    case 1:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 11:
    case 18:
        player_state.queued_magic_action.casts_remaining = 1;
        player_state.queued_magic_action.repeat_interval = 1;
        break;
    case 12:
        player_state.magic_origin_offset.vx = -200;
        player_state.queued_magic_action.casts_remaining = 5;
        player_state.queued_magic_action.repeat_interval = 2;
        break;
    case 9:
        player_state.queued_magic_action.casts_remaining = 6;
        player_state.queued_magic_action.repeat_interval = 1;
        break;
    case 0:
    case 2:
        player_state.queued_magic_action.casts_remaining = 1;
        player_state.queued_magic_action.repeat_interval = 1;
        player_state.magic_origin_offset.vz = 0;
        player_state.magic_origin_offset.vy = 0;
        player_state.magic_origin_offset.vx = 0;
        break;
    case 13:
        player_state.queued_magic_action.casts_remaining = 7;
        player_state.queued_magic_action.repeat_interval = 1;
        break;
    case 3:
        player_state.queued_magic_action.casts_remaining = 6;
        player_state.queued_magic_action.repeat_interval = 2;
        break;
    }

    player_state.selected_magic_record = record;
}

enum {
    PLAYER_MOVE_COLLISION_MODE = KF_COLLISION_QUERY_SHAPES |
                                 KF_COLLISION_QUERY_ACTORS |
                                 KF_COLLISION_QUERY_MAP_OBJECTS,
    PLAYER_MOVE_SLIDE_RADIUS = 880,
    PLAYER_MOVE_DEFLECTION_ANGLE = 32,
    PLAYER_MOVE_STEP = 22,
    PLAYER_MOVE_STEP_UP_TOLERANCE = 1280,
    PLAYER_MOVE_COLLISION_RETRY_LIMIT = 2
};

ADDRESS(0x800274ec, 0x43c)
s32 player_move_horizontal(s32 heading, s32 distance)
{
    s32 dx = (-rsin(heading) * distance) >> 12;
    s32 dz = (rcos(heading) * distance) >> 12;
    s32 initial_dx = dx;
    s32 initial_dz = dz;
    VECTOR next;
    s32 flags;
    s32 angle;
    s32 radius;
    s32 slide_distance;
    s32 result = 0;
    s32 collision_retry = 0;
    s32 slide_attempted = 0;
    s32 diagonal_retry = 0;
    s32 high_collision;
    SVECTOR delta;
    s32 diagonal_kind;

    for (;;) {
        next.vx = player_state.camera_position.vx + dx;
        next.vz = player_state.camera_position.vz + dz;
        flags = collision_query_world(next.vx, player_state.camera_position.vy, next.vz,
                              KF_PLAYER_COLLISION_RADIUS, KF_PLAYER_HEIGHT,
                              PLAYER_MOVE_COLLISION_MODE);
        if (flags == 0) {
        accept_position:
            player_state.camera_position.vx = next.vx;
            player_state.camera_position.vz = next.vz;
            player_state.map_layer_index = KF_COLLISION_CACHE_LAYER;
            result = 1;
            break;
        }

        high_collision = 0;
        if ((flags & ~(KF_COLLISION_HIT_AXIS | KF_COLLISION_HIT_FLOOR)) == 0) {
            s32 collision_height = KF_COLLISION_CACHE_RESULT;
            high_collision = 1;
            if (collision_height + PLAYER_MOVE_STEP_UP_TOLERANCE >= player_state.camera_position.vy
                && player_state.death_state == 0
                && (KF_COLLISION_CACHE_HEIGHT_LIMIT - collision_height)
                       < -KF_PLAYER_HEIGHT) {
                goto accept_position;
            }
        }

        if (flags & (KF_COLLISION_HIT_ACTOR | KF_COLLISION_HIT_MAP_OBJECT)) {
            collision_retry++;
            if (collision_retry == PLAYER_MOVE_COLLISION_RETRY_LIMIT) {
                break;
            }
            collision_cache_load_hit_bounds();
            delta.vx = (u16)KF_COLLISION_CACHE_POSITION.vx
                     - (u16)player_state.camera_position.vx;
            delta.vz = (u16)KF_COLLISION_CACHE_POSITION.vz
                     - (u16)player_state.camera_position.vz;
            angle = vector_xz_to_angle(delta.vx, delta.vz);
            angle = angle_mod_delta_le_half_turn(heading, angle)
                ? angle + (KF_ANGLE_HALF_TURN - PLAYER_MOVE_DEFLECTION_ANGLE)
                : angle + (KF_ANGLE_HALF_TURN + PLAYER_MOVE_DEFLECTION_ANGLE);
            angle &= KF_ANGLE_WRAP_MASK;
            radius = KF_COLLISION_CACHE_RADIUS + PLAYER_MOVE_SLIDE_RADIUS;
            delta.vx = (-rsin(angle) * radius) >> 12;
            delta.vz = (rcos(angle) * radius) >> 12;
            next.vx = KF_COLLISION_CACHE_POSITION.vx + delta.vx;
            next.vz = KF_COLLISION_CACHE_POSITION.vz + delta.vz;
            dx = next.vx - player_state.camera_position.vx;
            dz = next.vz - player_state.camera_position.vz;
            continue;
        }

        if (!slide_attempted) {
            slide_distance = distance - PLAYER_MOVE_STEP;
            if (slide_distance >= 0) {
                VECTOR *camera = &player_state.camera_position;
                do {
                    next.vx = camera->vx
                           + ((-rsin(heading) * slide_distance) >> 12);
                    next.vz = camera->vz
                           + ((rcos(heading) * slide_distance) >> 12);
                    if (collision_query_world(next.vx, camera->vy,
                                       next.vz, KF_PLAYER_COLLISION_RADIUS,
                                       KF_PLAYER_HEIGHT, PLAYER_MOVE_COLLISION_MODE) == 0) {
                        camera->vx = next.vx;
                        camera->vz = next.vz;
                        break;
                    }
                    slide_distance -= PLAYER_MOVE_STEP;
                } while (slide_distance >= 0);
            }
            slide_attempted = 1;
        }

        if (high_collision || (flags & KF_COLLISION_HIT_AXIS)) {
        axis_retry:
            if (dx != 0) {
                dx = 0;
                continue;
            }
            if (dz != 0) {
                dz = 0;
                dx = initial_dx;
                continue;
            }
        }
        if (flags & KF_COLLISION_HIT_DIAGONAL) {
            if (diagonal_retry) {
                goto axis_retry;
            } else {
                diagonal_retry = 1;
                diagonal_kind =
                    ((KfMapOccupancyLayer *)KF_COLLISION_CACHE_SHAPE)->quarter_turns & 3;
                if (diagonal_kind == 0 || diagonal_kind == 2) {
                    dx = (initial_dx + initial_dz) >> 1;
                    dz = dx;
                } else {
                    dx = (initial_dx - initial_dz) >> 1;
                    dz = -dx;
                }
                continue;
            }
        }
        result = 0;
        break;
    }
    player_state.frame_displacement.vx = dx;
    player_state.frame_displacement.vz = dz;
    return result;
}

enum {
    COLLISION_DEPTH_ARM_HEIGHT = 200,
    COLLISION_DEPTH_DEATH_LIMIT = 32000,
    PLAYER_MOTION_COLLISION_MASK = KF_COLLISION_QUERY_SHAPES |
        KF_COLLISION_QUERY_ACTORS | KF_COLLISION_QUERY_MAP_OBJECTS,
    PLAYER_LANDING_SOUND_ID = 12,
    PLAYER_LANDING_SOUND_MIN_MAGNITUDE = 320,
    PLAYER_LANDING_SOUND_MAX_EXCESS = 896,
    PLAYER_LANDING_SOUND_BASE_VOLUME = 32
};

ADDRESS(0x80027928, 0x60)
void player_check_fall_death(void)
{
    if (player_state.vertical_velocity >= COLLISION_DEPTH_ARM_HEIGHT
        && (KF_COLLISION_CACHE_RESULT - player_state.camera_position.vy)
               > COLLISION_DEPTH_DEATH_LIMIT) {
        player_death_begin(NULL);
        player_state.fatal_fall_latch = 1;
    }
}

ADDRESS(0x80027988, 0x44)
void player_play_landing_sound(s32 magnitude)
{
    s32 volume = magnitude;

    if (volume >= PLAYER_LANDING_SOUND_MIN_MAGNITUDE) {
        volume -= PLAYER_LANDING_SOUND_MIN_MAGNITUDE;
        if (volume > PLAYER_LANDING_SOUND_MAX_EXCESS) {
            volume = PLAYER_LANDING_SOUND_MAX_EXCESS;
        }
        audio_play_sound(PLAYER_LANDING_SOUND_ID,
                         (volume >> 3) + PLAYER_LANDING_SOUND_BASE_VOLUME);
    }
}

ADDRESS(0x800279cc, 0x5ac)
void player_update_vertical_motion(void)
{
    s32 next_y;
    s32 height_difference;
    s32 collision_flags;
    s32 impact;
    s32 bob;
    s32 movement_speed;
    const s32 *floor_result;

    collision_probe_floor_height(player_state.camera_position.vx,
                  player_state.camera_position.vy,
                  player_state.camera_position.vz, KF_PLAYER_COLLISION_RADIUS, KF_PLAYER_HEIGHT);
    player_state.frame_displacement.vy = 0;

    switch (player_state.vertical_motion_state) {
    case KF_PLAYER_VERTICAL_GROUNDED:
        break;

    case KF_PLAYER_VERTICAL_FALLING:
        player_check_fall_death();
        next_y = player_state.camera_position.vy + player_state.vertical_velocity;
        player_state.camera_position.vy = next_y;
        player_state.frame_displacement.vy = player_state.vertical_velocity;
        player_state.vertical_velocity += 40;
        if (KF_COLLISION_CACHE_RESULT + 100 < next_y) {
            player_state.camera_position.vy = KF_COLLISION_CACHE_RESULT;
            player_state.vertical_motion_state = KF_PLAYER_VERTICAL_GROUNDED;
        }
        goto finish;

    case KF_PLAYER_VERTICAL_STEP_UP:
        player_check_fall_death();
        player_state.camera_position.vy += player_state.vertical_velocity;
        if (player_state.camera_position.vy <= KF_COLLISION_CACHE_RESULT
            || player_state.vertical_velocity >= 0) {
            if (KF_COLLISION_CACHE_RESULT
                < player_state.camera_position.vy - player_state.vertical_velocity) {
                player_state.camera_position.vy = KF_COLLISION_CACHE_RESULT;
            }
            player_state.vertical_motion_state = KF_PLAYER_VERTICAL_GROUNDED;
        }
        player_state.frame_displacement.vy = player_state.vertical_velocity;
        player_state.vertical_velocity += 5;
        goto finish;

    case KF_PLAYER_VERTICAL_DEEP_FALL:
        player_check_fall_death();
        next_y = player_state.camera_position.vy + player_state.vertical_velocity;
        collision_flags = collision_query_world(player_state.camera_position.vx, next_y,
                                         player_state.camera_position.vz, KF_PLAYER_COLLISION_RADIUS,
                                         KF_PLAYER_HEIGHT, PLAYER_MOTION_COLLISION_MASK);
        if (collision_flags == 0) {
            player_state.frame_displacement.vy = player_state.vertical_velocity;
            player_state.vertical_velocity += 40;
            player_state.vertical_motion_pitch_offset = player_state.vertical_velocity >> 1;
            player_state.camera_position.vy = next_y;
            goto finish;
        }
        if (player_state.vertical_velocity < 0) {
            player_state.vertical_velocity = 0;
            goto finish;
        }
        player_play_landing_sound(player_state.vertical_velocity);
        if (player_state.vertical_velocity >= 480) {
            impact = (player_state.vertical_velocity * player_state.vertical_velocity) >> 12;
            player_apply_damage_reaction(NULL, (impact * impact * impact) / 0x1ccf0, 0);
        }
        if ((collision_flags & KF_COLLISION_HIT_FLOOR) != 0) {
            player_state.camera_position.vy = KF_COLLISION_CACHE_RESULT;
        } else {
            collision_cache_load_hit_bounds();
            next_y = KF_COLLISION_CACHE_POSITION.vy
                   - KF_COLLISION_CACHE_INTERACTION_HEIGHT - 1;
            if (collision_query_world(player_state.camera_position.vx, next_y,
                               player_state.camera_position.vz, KF_PLAYER_COLLISION_RADIUS,
                               KF_PLAYER_HEIGHT, PLAYER_MOTION_COLLISION_MASK) == 0) {
                player_state.camera_position.vy = next_y;
            }
        }
        player_state.landing_vertical_offset = 1;
        player_state.vertical_motion_state = KF_PLAYER_VERTICAL_LANDING;
        /* Enter the landing response in the same frame. */
        goto landing;

    case KF_PLAYER_VERTICAL_LANDING:
landing:
        if (player_state.landing_vertical_offset > 0) {
            player_state.landing_vertical_offset += player_state.vertical_velocity >> 2;
        }
        bob = player_state.vertical_motion_pitch_offset;
        player_state.vertical_velocity -= 100;
        if (bob > 0) {
            if (player_state.vertical_velocity > 0) {
                player_state.vertical_motion_pitch_offset = bob + 10;
            } else {
                player_state.vertical_motion_pitch_offset = bob - 30;
            }
        }
        if (player_state.landing_vertical_offset <= 0
            && player_state.vertical_motion_pitch_offset <= 0) {
            player_state.vertical_motion_state = KF_PLAYER_VERTICAL_GROUNDED;
            player_state.vertical_velocity = 0;
            player_state.landing_vertical_offset = 0;
            player_state.vertical_motion_pitch_offset = 0;
        }
        break;

    default:
        goto finish;
    }

    floor_result = &KF_COLLISION_CACHE_RESULT;
    height_difference = *floor_result - player_state.camera_position.vy;
    if (height_difference < 0) {
        if (height_difference >= -256) {
            if (height_difference < -128) {
                player_state.camera_position.vy -= 128;
            } else {
                player_state.camera_position.vy = *floor_result;
            }
            goto finish;
        }
        if (height_difference >= -512) {
            player_state.camera_position.vy -= 256;
            goto finish;
        }
        movement_speed = player_state.movement_speed.signed_value;
        player_state.vertical_motion_state = KF_PLAYER_VERTICAL_STEP_UP;
        player_state.vertical_velocity = movement_speed > 200 ? -300 : -150;
    } else {
        if (height_difference <= 0) {
            goto finish;
        }
        if (collision_query_world(player_state.camera_position.vx,
                           player_state.camera_position.vy + 1,
                           player_state.camera_position.vz, KF_PLAYER_COLLISION_RADIUS,
                           KF_PLAYER_HEIGHT, PLAYER_MOTION_COLLISION_MASK) != 0) {
            goto finish;
        }
        if (height_difference <= 256) {
            if (height_difference >= 129) {
                player_state.camera_position.vy += 128;
            } else {
                player_state.camera_position.vy = *floor_result;
            }
            goto finish;
        }
        if (height_difference <= 512) {
            player_state.camera_position.vy += 256;
            goto finish;
        }
        player_state.vertical_motion_state = height_difference > 1024
            ? KF_PLAYER_VERTICAL_DEEP_FALL : KF_PLAYER_VERTICAL_FALLING;
        player_state.vertical_velocity = 40;
    }
    player_state.landing_vertical_offset = 0;
    player_state.vertical_motion_pitch_offset = 0;

finish:
    if (player_state.vertical_motion_state == KF_PLAYER_VERTICAL_GROUNDED) {
        if (player_state.walking_bob_enabled != 0) {
            player_state.walking_bob_phase =
                (player_state.walking_bob_phase + player_state.movement_speed.unsigned_value)
                & KF_ANGLE_WRAP_MASK;
            bob = rsin(player_state.walking_bob_phase) >> 5;
            if (bob < 0) {
                bob = -bob;
            }
            player_state.camera_vertical_offset = bob - (bob >> 2);
        } else {
            player_state.camera_vertical_offset = 0;
        }
    }
    player_update_collision_bounds();
}

ADDRESS(0x80027f78, 0x2ac)
s32 player_move_reaction_with_collision(void)
{
    VECTOR next;
    s32 flags;
    s32 length;
    s32 remaining;
    s32 minimum_length;
    SVECTOR *motion;

    next.vx = player_state.camera_position.vx + player_state.reaction.damage.rotation.vx;
    next.vy = player_state.camera_position.vy + player_state.reaction.damage.rotation.vy;
    next.vz = player_state.camera_position.vz + player_state.reaction.damage.rotation.vz;

    flags = collision_query_world(next.vx, next.vy, next.vz,
                                  KF_PLAYER_COLLISION_RADIUS, KF_PLAYER_HEIGHT,
                                  PLAYER_MOTION_COLLISION_MASK);
    if (flags == 0) {
    accept:
        if (player_state.reaction.damage.rotation.vy >= 160
            && KF_COLLISION_CACHE_RESULT - player_state.camera_position.vy
                   > COLLISION_DEPTH_DEATH_LIMIT) {
            player_death_begin(NULL);
            player_state.fatal_fall_latch = 1;
        }
        player_state.camera_position.vx = next.vx;
        player_state.camera_position.vy = next.vy;
        player_state.camera_position.vz = next.vz;
        player_state.reaction.damage.rotation.vy += 32;
        goto accepted;
    }

    next.vy = player_state.camera_position.vy;
    flags = collision_query_world(next.vx, next.vy, next.vz,
                                  KF_PLAYER_COLLISION_RADIUS, KF_PLAYER_HEIGHT,
                                  PLAYER_MOTION_COLLISION_MASK);
    if (flags == 0) {
        player_state.reaction.damage.rotation.vy = 1;
        flags = collision_query_world(next.vx, next.vy, next.vz,
                                      KF_PLAYER_COLLISION_RADIUS, KF_PLAYER_HEIGHT,
                                      PLAYER_MOTION_COLLISION_MASK);
        if (flags == 0) {
            minimum_length = 32;
        scale_motion:
            motion = &player_state.reaction.damage.rotation;
            length = fixed_vector2_length(motion->vx, motion->vz);
            remaining = length - minimum_length;
            if (length <= minimum_length) {
                motion->vz = 0;
                motion->vx = 0;
                goto exhausted;
            }
            motion->vx = (motion->vx * remaining) / length;
            motion->vz = (motion->vz * remaining) / length;
            goto accept;
        }
    }

    if (flags & ~(KF_COLLISION_HIT_AXIS | KF_COLLISION_HIT_FLOOR)) {
        return 1;
    }
    if (KF_COLLISION_CACHE_RESULT + 256 < player_state.camera_position.vy) {
        return 1;
    }
    next.vy = KF_COLLISION_CACHE_RESULT;
    minimum_length = 56;
    goto scale_motion;

exhausted:
    return 1;

accepted:
    player_update_collision_bounds();
    return 0;
}

enum {
    PLAYER_YAW_ACCEL_SHIFT = 2,
    PLAYER_PITCH_STEP = 3,
    PLAYER_PITCH_STEP_LIMIT = 32,
    PLAYER_CAMERA_PITCH_LIMIT = 700
};

ADDRESS(0x80028224, 0x2f8)
void player_update_camera_rotation(void)
{
    if (player_state.flags_140.low & PADLleft) {
        player_state.yaw_step += player_state.turn_step_limit >> PLAYER_YAW_ACCEL_SHIFT;
        if (player_state.yaw_step > player_state.turn_step_limit) {
            player_state.yaw_step = player_state.turn_step_limit;
        }
    } else if (player_state.flags_140.low & PADLright) {
        player_state.yaw_step -= player_state.turn_step_limit >> PLAYER_YAW_ACCEL_SHIFT;
        if (player_state.yaw_step < -player_state.turn_step_limit) {
            player_state.yaw_step = -player_state.turn_step_limit;
        }
    } else if (player_state.yaw_step > 0) {
        player_state.yaw_step -= player_state.turn_step_limit >> PLAYER_YAW_ACCEL_SHIFT;
        if (player_state.yaw_step < 0) {
            player_state.yaw_step = 0;
        }
    } else if (player_state.yaw_step < 0) {
        player_state.yaw_step += player_state.turn_step_limit >> PLAYER_YAW_ACCEL_SHIFT;
        if (player_state.yaw_step > 0) {
            player_state.yaw_step = 0;
        }
    }

    player_state.camera_rotation_target.angles[1] =
        (player_state.camera_rotation_target.angles[1] + player_state.yaw_step)
        & KF_ANGLE_WRAP_MASK;

    if (player_state.flags_140.low & PADR2) {
        player_state.pitch_step += PLAYER_PITCH_STEP;
        if (player_state.pitch_step > PLAYER_PITCH_STEP_LIMIT) {
            player_state.pitch_step = PLAYER_PITCH_STEP_LIMIT;
        }
    } else if (player_state.flags_140.low & PADL2) {
        player_state.pitch_step -= PLAYER_PITCH_STEP;
        if (player_state.pitch_step < -PLAYER_PITCH_STEP_LIMIT) {
            player_state.pitch_step = -PLAYER_PITCH_STEP_LIMIT;
        }
    } else if (player_state.pitch_step > 0) {
        player_state.pitch_step -= PLAYER_PITCH_STEP;
        if (player_state.pitch_step < 0) {
            player_state.pitch_step = 0;
        }
    } else if (player_state.pitch_step < 0) {
        player_state.pitch_step += PLAYER_PITCH_STEP;
        if (player_state.pitch_step > 0) {
            player_state.pitch_step = 0;
        }
    }

    if (player_state.pitch_step > 0) {
        if (angle_mod_delta_le_half_turn(
                (player_state.camera_rotation_target.angles[0] =
                    (player_state.camera_rotation_target.angles[0] + player_state.pitch_step)
                    & KF_ANGLE_WRAP_MASK),
                PLAYER_CAMERA_PITCH_LIMIT)) {
            player_state.camera_rotation_target.angles[0] = PLAYER_CAMERA_PITCH_LIMIT;
        }
    } else if (player_state.pitch_step < 0) {
        if (angle_mod_delta_le_half_turn(
                -PLAYER_CAMERA_PITCH_LIMIT,
                (player_state.camera_rotation_target.angles[0] =
                    (player_state.camera_rotation_target.angles[0] + player_state.pitch_step)
                    & KF_ANGLE_WRAP_MASK))) {
            player_state.camera_rotation_target.angles[0] = -PLAYER_CAMERA_PITCH_LIMIT;
        }
    }
}

ADDRESS(0x8002851c, 0x460)
void player_update_horizontal_motion(void)
{
    s16 forward;
    s16 strafe;
    s32 forward_square;
    s32 strafe_square;
    s16 magnitude;

    if (player_state.flags_140.low & PADLup) {
        forward = player_state.forward_velocity + (player_state.movement_step_limit >> 2);
        if (forward > player_state.movement_step_limit) {
            player_state.forward_velocity = player_state.movement_step_limit;
        } else {
            player_state.forward_velocity = forward;
        }
    } else if (player_state.flags_140.low & PADLdown) {
        forward = player_state.forward_velocity - (player_state.movement_step_limit >> 2);
        if (forward >= -player_state.movement_step_limit) {
            player_state.forward_velocity = forward;
        } else {
            player_state.forward_velocity = -player_state.movement_step_limit;
        }
    } else if (player_state.forward_velocity > 0) {
        player_state.forward_velocity -= player_state.movement_step_limit >> 3;
        if (player_state.forward_velocity < 0) {
            player_state.forward_velocity = 0;
        }
    } else if (player_state.forward_velocity < 0) {
        player_state.forward_velocity += player_state.movement_step_limit >> 3;
        if (player_state.forward_velocity > 0) {
            player_state.forward_velocity = 0;
        }
    }

    if (player_state.flags_140.low & PADR1) {
        strafe = player_state.strafe_velocity + (player_state.movement_step_limit >> 2);
        if (strafe > player_state.movement_step_limit) {
            player_state.strafe_velocity = player_state.movement_step_limit;
        } else {
            player_state.strafe_velocity = strafe;
        }
    } else if (player_state.flags_140.low & PADL1) {
        strafe = player_state.strafe_velocity - (player_state.movement_step_limit >> 2);
        if (strafe >= -player_state.movement_step_limit) {
            player_state.strafe_velocity = strafe;
        } else {
            player_state.strafe_velocity = -player_state.movement_step_limit;
        }
    } else if (player_state.strafe_velocity > 0) {
        player_state.strafe_velocity -= player_state.movement_step_limit >> 2;
        if (player_state.strafe_velocity < 0) {
            player_state.strafe_velocity = 0;
        }
    } else if (player_state.strafe_velocity < 0) {
        player_state.strafe_velocity += player_state.movement_step_limit >> 2;
        if (player_state.strafe_velocity > 0) {
            player_state.strafe_velocity = 0;
        }
    }

    strafe_square = player_state.strafe_velocity;
    strafe_square *= strafe_square;
    forward_square = player_state.forward_velocity;
    forward_square *= forward_square;
    magnitude = SquareRoot0(strafe_square + forward_square);
    if (magnitude == 0) {
        forward = 0;
        strafe = 0;
    } else {
        strafe = strafe_square / magnitude;
        if (player_state.strafe_velocity < 0) {
            strafe = -(strafe_square / magnitude);
        }
        forward = forward_square / magnitude;
        if (player_state.forward_velocity < 0) {
            forward = -(forward_square / magnitude);
        }
    }

    player_state.movement_speed.unsigned_value = SquareRoot0(strafe * strafe + forward * forward);
    if (forward >= 0) {
        player_move_horizontal((s16)player_state.camera_rotation_target.angles[1], forward);
    } else {
        player_move_horizontal(
            ((s16)player_state.camera_rotation_target.angles[1] + KF_ANGLE_HALF_TURN)
                & KF_ANGLE_WRAP_MASK,
            -forward);
    }
    if (strafe > 0) {
        player_move_horizontal(
            ((s16)player_state.camera_rotation_target.angles[1] - KF_ANGLE_QUARTER_TURN)
                & KF_ANGLE_WRAP_MASK,
            strafe);
    } else if (strafe < 0) {
        player_move_horizontal(
            ((s16)player_state.camera_rotation_target.angles[1] + KF_ANGLE_QUARTER_TURN)
                & KF_ANGLE_WRAP_MASK,
            -strafe);
    } else {
        player_state.frame_displacement.vz = 0;
        player_state.frame_displacement.vx = 0;
    }
}
