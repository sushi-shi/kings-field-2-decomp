#include <kf/lib/address.h>
#include <kf/lib/math.h>
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

    if (player_state.queued_magic_action.magic_id != 0xff || magic_id == 0xff) {
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
    if (player_state.magic_charge < 5000) {
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
    PLAYER_MOVE_RADIUS = 800,
    PLAYER_MOVE_HEIGHT = 1700,
    PLAYER_MOVE_COLLISION_MODE = KF_COLLISION_QUERY_SHAPES |
                                 KF_COLLISION_QUERY_ACTORS |
                                 KF_COLLISION_QUERY_MAP_OBJECTS,
    PLAYER_MOVE_SLIDE_RADIUS = 880,
    PLAYER_MOVE_DEFLECTION_ANGLE = 32,
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
    SVECTOR delta;
    s32 diagonal_kind;

    for (;;) {
        next.vx = player_state.camera_position.vx + dx;
        next.vz = player_state.camera_position.vz + dz;
        flags = collision_query_world(next.vx, player_state.camera_position.vy, next.vz,
                              PLAYER_MOVE_RADIUS, PLAYER_MOVE_HEIGHT,
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
        if ((flags & -6) == 0) {
            s32 collision_height = KF_COLLISION_CACHE_RESULT;
            high_collision = 1;
            if (collision_height + 1280 >= player_state.camera_position.vy
                && player_state.death_state == 0
                && (KF_COLLISION_CACHE_HEIGHT_LIMIT - collision_height)
                       < -PLAYER_MOVE_HEIGHT) {
                goto accept_position;
            }
        }

        if (flags & (KF_COLLISION_HIT_ACTOR | KF_COLLISION_HIT_MAP_OBJECT)) {
            collision_retry++;
            if (collision_retry == 2) {
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
                                       next.vz, PLAYER_MOVE_RADIUS,
                                       PLAYER_MOVE_HEIGHT, PLAYER_MOVE_COLLISION_MODE) == 0) {
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
