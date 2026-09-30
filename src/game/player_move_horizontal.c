#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/collision_cache.h>
#include <kf/game/player.h>
#include <psyq/sdk.h>

extern s32 func_8002b9d4(s32 x, s32 y, s32 z, s32 radius, s32 height, s32 mode);
extern void func_8002b874(void);

enum {
    PLAYER_MOVE_RADIUS = 800,
    PLAYER_MOVE_HEIGHT = 1700,
    PLAYER_MOVE_COLLISION_MODE = 49,
    PLAYER_MOVE_SLIDE_RADIUS = 880,
    PLAYER_MOVE_STEP = 22
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
    s32 delta_x;
    s32 delta_z;
    s32 diagonal_kind;

    for (;;) {
        next.vx = player_state.camera_position.vx + dx;
        next.vz = player_state.camera_position.vz + dz;
        flags = func_8002b9d4(next.vx, player_state.camera_position.vy, next.vz,
                              PLAYER_MOVE_RADIUS, PLAYER_MOVE_HEIGHT,
                              PLAYER_MOVE_COLLISION_MODE);
        if (flags == 0) {
        accept_position:
            player_state.camera_position.vx = next.vx;
            player_state.camera_position.vz = next.vz;
            player_state.unknown_128 = KF_COLLISION_CACHE_LAYER;
            result = 1;
            break;
        }

        high_collision = 0;
        if (flags & -6) {
            s32 collision_height = KF_COLLISION_CACHE_RESULT;
            high_collision = 1;
            if (collision_height + 1280 >= player_state.camera_position.vy
                && player_state.death_state == 0 && (flags & 0x30) == 0
                       /* The cache/equipment boundary remains provisional. */
                       && (*(s32 *)((u8 *)&bss_801c7540 + 0x11814)
                           - collision_height) < -PLAYER_MOVE_HEIGHT) {
                goto accept_position;
            }
        }

        if (flags & 0x30) {
            collision_retry++;
            if (collision_retry == 2) {
                break;
            }
            func_8002b874();
            delta_x = (s16)(KF_COLLISION_CACHE_POSITION.vx - player_state.camera_position.vx);
            delta_z = (s16)(KF_COLLISION_CACHE_POSITION.vz - player_state.camera_position.vz);
            angle = vector_xz_to_angle(delta_x, delta_z);
            if (angle_mod_delta_le_half_turn(heading, angle)) {
                angle += 2016;
            } else {
                angle += 2080;
            }
            angle &= KF_ANGLE_WRAP_MASK;
            radius = KF_COLLISION_CACHE_RADIUS + PLAYER_MOVE_SLIDE_RADIUS;
            next.vx = KF_COLLISION_CACHE_POSITION.vx + ((-rsin(angle) * radius) >> 12);
            next.vz = KF_COLLISION_CACHE_POSITION.vz + ((rcos(angle) * radius) >> 12);
            dx = next.vx - player_state.camera_position.vx;
            dz = next.vz - player_state.camera_position.vz;
            continue;
        }

        if (!slide_attempted) {
            slide_distance = distance - PLAYER_MOVE_STEP;
            if (slide_distance >= 0) {
                do {
                    next.vx = player_state.camera_position.vx
                           + ((-rsin(heading) * slide_distance) >> 12);
                    next.vz = player_state.camera_position.vz
                           + ((rcos(heading) * slide_distance) >> 12);
                    if (func_8002b9d4(next.vx, player_state.camera_position.vy,
                                       next.vz, PLAYER_MOVE_RADIUS,
                                       PLAYER_MOVE_HEIGHT, PLAYER_MOVE_COLLISION_MODE) == 0) {
                        player_state.camera_position.vx = next.vx;
                        player_state.camera_position.vz = next.vz;
                        break;
                    }
                    slide_distance -= PLAYER_MOVE_STEP;
                } while (slide_distance >= 0);
            }
            slide_attempted = 1;
        }

        if (!high_collision && (flags & 1)) {
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
        if (flags & 2) {
            if (diagonal_retry) {
                if (dx != 0) {
                    dx = 0;
                    continue;
                }
                if (dz != 0) {
                    dz = 0;
                    dx = initial_dx;
                    continue;
                }
            } else {
                diagonal_retry = 1;
                diagonal_kind = KF_COLLISION_CACHE_SHAPE[2] & 3;
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
        break;
    }
    player_state.unknown_e8 = dx;
    player_state.unknown_ec = dz;
    return result;
}
