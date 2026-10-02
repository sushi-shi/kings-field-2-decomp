#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/player.h>

enum {
    PLAYER_YAW_ACCEL_SHIFT = 2,
    PLAYER_PITCH_STEP = 3,
    PLAYER_PITCH_STEP_LIMIT = 32,
    PLAYER_CAMERA_PITCH_LIMIT = 700
};

ADDRESS(0x80028224, 0x2f8)
void player_update_camera_rotation(void)
{
    if (player_state.flags_140.low & 0x8000) {
        player_state.yaw_step += player_state.turn_step_limit >> PLAYER_YAW_ACCEL_SHIFT;
        if (player_state.yaw_step > player_state.turn_step_limit) {
            player_state.yaw_step = player_state.turn_step_limit;
        }
    } else if (player_state.flags_140.low & 0x2000) {
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

    if (player_state.flags_140.low & 2) {
        player_state.pitch_step += PLAYER_PITCH_STEP;
        if (player_state.pitch_step > PLAYER_PITCH_STEP_LIMIT) {
            player_state.pitch_step = PLAYER_PITCH_STEP_LIMIT;
        }
    } else if (player_state.flags_140.low & 1) {
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

    if (player_state.flags_140.low & 0x1000) {
        forward = player_state.forward_velocity + (player_state.movement_step_limit >> 2);
        if (forward > player_state.movement_step_limit) {
            player_state.forward_velocity = player_state.movement_step_limit;
        } else {
            player_state.forward_velocity = forward;
        }
    } else if (player_state.flags_140.low & 0x4000) {
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

    if (player_state.flags_140.low & 8) {
        strafe = player_state.strafe_velocity + (player_state.movement_step_limit >> 2);
        if (strafe > player_state.movement_step_limit) {
            player_state.strafe_velocity = player_state.movement_step_limit;
        } else {
            player_state.strafe_velocity = strafe;
        }
    } else if (player_state.flags_140.low & 4) {
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
        player_state.unknown_ec = 0;
        player_state.unknown_e8 = 0;
    }
}
