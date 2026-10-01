#include <kf/lib/address.h>
#include <kf/game/actor.h>
#include <kf/game/audio.h>
#include <kf/game/effect.h>
#include <kf/game/player.h>
#include <psyq/libc.h>

extern s32 func_8003c000(KfActor *actor, s32 vertex_index, VECTOR *output);
extern s32 func_8003c3e0(KfActor *actor, const VECTOR *origin, s32 step,
                         const VECTOR *target, SVECTOR *direction,
                         s32 pitch_override, u16 yaw_limit, s32 iterations);

RODATA(0x80011ee8, 0x1ec)

/* The script gives either three signed coordinates, two vertex indices and a
 * blend fraction, or one vertex index. The final pointer is used by the
 * coordinate form when an effect kind consumes an extra script halfword. */
ADDRESS(0x8003c614, 0xa70)
void func_8003c614(s32 kind, s32 effect_id, s32 position_mode,
                   s32 first, s32 second, s32 third, const u16 *script)
{
    KfActor *current = actor_state.current;
    const VECTOR *player = &player_state.camera_position;
    const u16 *parameters = (const u16 *)first;
    VECTOR offset;
    VECTOR position;
    VECTOR target;
    VECTOR predicted;
    SVECTOR rotated;
    SVECTOR direction;
    SVECTOR motion;
    struct KfEulerAngles angles;
    KfEffectRecord *effect;
    KfActor *spawned;
    s32 travel_time;
    s32 trajectory_angle;
    s32 distance;
    s32 count;
    u16 group_index;

    if (position_mode == -1) {
        parameters = script;
        rotated.vx = first;
        rotated.vy = second;
        rotated.vz = third;
        vector_rotate_yxz(&current->rotation, &rotated, &offset);
    } else if (position_mode == -2) {
        func_8003c000(current, first, &target);
        func_8003c000(current, second, &offset);
        predicted.vx = func_8001584c(player->vx,
            ((offset.vx - target.vx) << 8) + current->position.vx, third);
        predicted.vy = func_8001584c(player->vy,
            ((offset.vy - target.vy) << 8) + current->position.vy, third);
        predicted.vz = func_8001584c(player->vz,
            ((offset.vz - target.vz) << 8) + current->position.vz, third);
    } else {
        func_8003c000(current, position_mode, &offset);
    }

    position.vx = current->position.vx + offset.vx;
    position.vy = current->position.vy + offset.vy;
    position.vz = current->position.vz + offset.vz;

    switch (kind) {
    case 1:
    case 0x1c:
        func_8003c3e0(current, player, 500, &position, &direction, -1, 0x400, 1);
        func_80040308(effect_id, 0x23, kind, &position, &direction);
        break;
    case 2:
        func_80040308(effect_id, 0x23, kind, &position, &direction,
                      0x1000, 0x100, 0x1000);
        break;
    case 4:
        func_8003c3e0(current, player, 800, &position, &direction, -1, 0x400, 1);
        func_80040308(effect_id, 0x23, kind, &position, &direction);
        break;
    case 7:
    case 0x20:
        audio_play_spatial_default_range(0x23, &position, 0x6e, 0);
        /* fall through */
    case 0x7b:
        if (kind == 0x7b && second == 0) {
            audio_play_spatial_default_range(0x23, &position, 0x6e, 0);
        }
        func_8003c3e0(current, player, 500, &position, &direction, -1, 0x400, 1);
        effect = func_80040308(effect_id, 0x23,
                                kind == 0x7b ? 0x20 : kind, &position, &direction);
        if (effect != 0) effect->cooldown = 3;
        break;
    case 9:
    case 0x21:
    case 0x16:
        func_8003c3e0(current, player, 400, &position, &direction, -1, 0x400, 1);
        func_80040308(effect_id, 0x23, kind, &position, &direction);
        break;
    case 0xc:
        func_800154fc(predicted.vx - position.vx,
                      predicted.vy - position.vy,
                      predicted.vz - position.vz,
                      &angles);
        pitch_yaw_to_forward_vector(&angles, &direction);
        vector3s_scale_shift12(20, &direction);
        func_80040308(effect_id, 0x23, kind, &position, &direction,
                      &angles, 500, 0x3c, 0x80, 0x50, 0x8c);
        break;
    case 0x17:
        func_80040308(effect_id, 0x23, kind, &position, 0,
                      actor_state.unknown_93b8, position_mode, parameters[2]);
        break;
    case 0x18:
        func_8003c3e0(current, player, 250, &position, &direction, -1, 0x400, 1);
        func_80040308(effect_id, 0x23, kind, &position, &direction);
        break;
    case 0x1a:
    case 0x1b:
        func_8003c3e0(current, player, 300, &position, &direction, -1, 0x400, 1);
        func_80040308(effect_id, 0x23, kind, &position, &direction);
        break;
    case 0x1d:
    case 0x1f:
        target = *player;
        for (count = 6; count != 0; count--) {
            distance = fixed_vector2_length(target.vx - position.vx,
                                            target.vz - position.vz);
            if (func_80015918(0, distance,
                    position.vy + 1400 - target.vy, 10, 800,
                    &travel_time, &trajectory_angle) != 0) {
                trajectory_angle = 0x100;
            }
            if (count != 1) {
                func_80015ce0(player, (SVECTOR *)&player_state.unknown_e8,
                              travel_time >> 6, &target);
            }
        }
        motion.vx = trajectory_angle;
        motion.vy = func_8003c3e0(current, &target, 800, &position,
                                  &direction, trajectory_angle, 0xc00, 1);
        motion.vz = 0;
        effect = func_80040308(effect_id, 0x23, kind, &position, &direction, &motion);
        if (effect != 0) {
            effect->updates_remaining = 0x32;
            effect->phase = 0;
            *(u16 *)&effect->unknown_3c[4] = position.vy;
        }
        break;
    case 0x28:
        func_800154fc(player->vx - position.vx,
                      player->vy - (position.vy + 1600),
                      player->vz - position.vz,
                      &angles);
        pitch_yaw_to_forward_vector(&angles, &direction);
        vector3s_scale_shift12(1000, &direction);
        position.vx += direction.vx;
        position.vy += direction.vy;
        position.vz += direction.vz;
        func_80040308(effect_id, 0x23, kind, &position, &direction, &angles);
        break;
    case 0x6c:
        pitch_yaw_to_forward_vector(&current->rotation, &direction);
        vector3s_scale_shift12(550, &direction);
        effect = func_80040308(effect_id, 0x23, 7, &position, &direction);
        if (effect != 0) effect->cooldown = 5;
        break;
    case 0x6e:
    case 0x70:
        group_index = parameters[2];
        spawned = actor_pool_find_free();
        if (spawned != 0) {
            func_8003c3e0(current, player, kind == 0x6e ? 400 : 250,
                          &position, &direction, -1, 0x400, 1);
            spawned->slot_state = 5;
            spawned->group_index = group_index;
            spawned->unknown_04 = 0;
            spawned->unknown_05 = 0;
            spawned->unknown_06 = current->unknown_06;
            spawned->lifecycle = 1;
            spawned->unknown_28 = actor_state.target_groups[group_index].unknown_34;
            spawned->unknown_15 = actor_state.target_groups[group_index].unknown_09;
            spawned->position = position;
            spawned->position.vy += kind == 0x6e ? 4096 :
                actor_state.target_groups[group_index].unknown_14 >> 1;
            actor_initialize_from_group(spawned);
            *(SVECTOR *)&spawned->unknown_50 = direction;
            *(SVECTOR *)&spawned->rotation = *(SVECTOR *)&current->rotation;
            actor_select_target_type_in_own_group(spawned,
                                                   kind == 0x6e ? 0x1d : 0x1e);
        }
        break;
    case 0x78:
        position.vx = (rand() >> 2) + player->vx - 4096;
        position.vz = (rand() >> 2) + player->vz - 4096;
        position.vy = player->vy - 5000;
        func_80040308(effect_id, 0x23, kind, &position, 0);
        func_80040308(effect_id, 0x23, kind, &position, 0);
        break;
    case 0x79:
        func_8003c3e0(current, player, 600, &position, &direction, -1, 0x400, 1);
        fixed_vector3_length(position.vx - player->vx,
                             position.vy - player->vy,
                             position.vz - player->vz);
        func_80040308(effect_id, 0x23, kind, &position, &direction);
        break;
    }
}
