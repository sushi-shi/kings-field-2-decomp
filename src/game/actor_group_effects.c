#include <kf/lib/address.h>
#include <kf/game/actor.h>
#include <kf/game/audio.h>
#include <kf/game/effect.h>
#include <kf/game/player.h>
#include <psyq/libc.h>
#include <stdarg.h>

extern s32 func_8003c000(KfActor *actor, s32 vertex_index, VECTOR *output);
extern s32 func_8003c3e0(KfActor *actor, const VECTOR *origin, s32 step,
                         const VECTOR *target, SVECTOR *direction,
                         s32 pitch_override, u16 yaw_limit, s32 iterations);

RODATA(0x80011ee8, 0x1ec)

/* The script gives either three signed coordinates, two vertex indices and a
 * blend fraction, or one vertex index. The final pointer is used by the
 * coordinate form when an effect kind consumes an extra script halfword. */
ADDRESS(0x8003c614, 0xa70)
void func_8003c614(s32 kind, s32 effect_id, s32 position_mode, ...)
{
    va_list arguments;
    KfActor *current = actor_state.current;
    const VECTOR *player = &player_state.camera_position;
    const u16 *parameters;
    s32 first;
    s32 second;
    s32 third;
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
    KfTargetGroup *group;

    va_start(arguments, position_mode);
    if (position_mode == -1) {
        /* Each coordinate occupies an O32 word slot but is read as u16. */
        rotated.vx = *(const u16 *)arguments;
        (void)va_arg(arguments, s32);
        rotated.vy = *(const u16 *)arguments;
        (void)va_arg(arguments, s32);
        rotated.vz = *(const u16 *)arguments;
        (void)va_arg(arguments, s32);
        vector_rotate_yxz(&current->rotation, &rotated, &offset);
    } else if (position_mode == -2) {
        first = va_arg(arguments, s32);
        func_8003c000(current, first, &target);
        second = va_arg(arguments, s32);
        func_8003c000(current, second, &offset);
        third = va_arg(arguments, s32);
        predicted.vx = func_8001584c(player->vx,
            (s32)((((u32)offset.vx - (u32)target.vx) << 8) +
                  (u32)current->position.vx), third);
        predicted.vy = func_8001584c(player->vy,
            (s32)((((u32)offset.vy - (u32)target.vy) << 8) +
                  (u32)current->position.vy), third);
        predicted.vz = func_8001584c(player->vz,
            (s32)((((u32)offset.vz - (u32)target.vz) << 8) +
                  (u32)current->position.vz), third);
    } else {
        func_8003c000(current, position_mode, &offset);
    }

    position.vx = current->position.vx + offset.vx;
    position.vy = current->position.vy + offset.vy;
    position.vz = current->position.vz + offset.vz;

    switch (kind) {
    case 0x7b:
        kind = 0x20;
        if (((const s32 *)arguments)[1] != 0) {
            goto target_effect;
        }
        /* fall through */
    case 7:
    case 0x20:
        audio_play_spatial_default_range(0x23, &position, 0x6e, 0);
    target_effect:
        func_8003c3e0(current, player, 500, &position, &direction, -1, 0x400, 1);
        effect = func_80040308(effect_id, 0x23, kind, &position, &direction);
        if (effect != 0) effect->cooldown = 3;
        break;
    case 0x79:
        func_8003c3e0(current, player, 600, &position, &direction, -1, 0x400, 1);
        distance = fixed_vector3_length(position.vx - player->vx,
                                        position.vy - player->vy,
                                        position.vz - player->vz);
        travel_time = (distance - 2000) / 600;
        if (travel_time < 0) travel_time = 0;
        goto simple_direction_effect;
    case 4:
        func_8003c3e0(current, player, 800, &position, &direction, -1, 0x400, 1);
        travel_time = -1;
    simple_direction_effect:
        func_80040308(effect_id, 0x23, kind, &position, &direction,
                      travel_time, 0x400, 1);
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
    case 9:
    case 0x21:
        func_8003c3e0(current, player, 400, &position, &direction, -1, 0x400, 1);
        travel_time = 0xfe;
        goto simple_direction_effect;
    case 0x18:
        func_8003c3e0(current, player, 250, &position, &direction, -1, 0x400, 1);
        travel_time = -1;
        goto simple_direction_effect;
    case 2:
        func_80040308(effect_id, 0x23, kind, &position, &direction,
                      0x1000, 0x100, 0x1000);
        break;
    case 0x16:
        func_8003c3e0(current, player, 400, &position, &direction, -1, 0x400, 1);
        travel_time = -1;
        goto simple_direction_effect;
    case 0x17:
        parameters = va_arg(arguments, const u16 *);
        func_80040308(effect_id, 0x23, kind, &position, 0,
                      actor_state.unknown_93b8, position_mode, parameters[2]);
        break;
    case 0x6c:
        pitch_yaw_to_forward_vector(&current->rotation, &direction);
        vector3s_scale_shift12(550, &direction);
        effect = func_80040308(effect_id, 0x23, 7, &position, &direction);
        if (effect != 0) effect->cooldown = 5;
        break;
    case 1:
    case 0x1c:
        func_8003c3e0(current, player, 500, &position, &direction, -1, 0x400, 1);
        travel_time = -1;
        goto simple_direction_effect;
    case 0x1a:
    case 0x1b:
        func_8003c3e0(current, player, 300, &position, &direction, -1, 0x400, 1);
        travel_time = -1;
        goto simple_direction_effect;
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
    case 0x78:
        position.vx = (rand() >> 2) + player->vx - 4096;
        position.vz = (rand() >> 2) + player->vz - 4096;
        position.vy = player->vy - 5000;
        func_80040308(effect_id, 0x23, kind, &position, 0);
        func_80040308(effect_id, 0x23, kind, &position, 0);
        break;
    case 0x6e:
        parameters = va_arg(arguments, const u16 *);
        group_index = parameters[2];
        spawned = actor_pool_find_free();
        if (spawned != 0) {
            func_8003c3e0(current, player, 400,
                          &position, &direction, -1, 0x400, 1);
            spawned->slot_state = 5;
            spawned->group_index = group_index;
            spawned->unknown_04 = 0;
            spawned->unknown_05 = 0;
            spawned->unknown_03 = current->unknown_06;
            spawned->lifecycle = 1;
            group = &actor_state.target_groups[group_index];
            spawned->unknown_28 = group->unknown_34;
            spawned->unknown_15 = group->unknown_09;
            spawned->position.vx = position.vx;
            spawned->position.vy = position.vy;
            spawned->position.vz = position.vz;
            spawned->position.vy += 4096;
            actor_initialize_from_group(spawned);
            *(SVECTOR *)&spawned->unknown_50 = direction;
            *(SVECTOR *)&spawned->rotation = *(SVECTOR *)&current->rotation;
            actor_select_target_type_in_own_group(spawned, 0x1d);
        }
        break;
    case 0x70:
        parameters = va_arg(arguments, const u16 *);
        group_index = parameters[2];
        spawned = actor_pool_find_free();
        if (spawned != 0) {
            func_8003c3e0(current, player, 250,
                          &position, &direction, -1, 0x400, 1);
            spawned->slot_state = 5;
            spawned->group_index = group_index;
            spawned->unknown_04 = 0;
            spawned->unknown_05 = 0;
            spawned->unknown_03 = current->unknown_06;
            spawned->lifecycle = 1;
            group = &actor_state.target_groups[group_index];
            spawned->unknown_28 = group->unknown_34;
            spawned->unknown_15 = group->unknown_09;
            spawned->position.vx = position.vx;
            spawned->position.vy = position.vy;
            spawned->position.vz = position.vz;
            spawned->position.vy += group->unknown_14 >> 1;
            actor_initialize_from_group(spawned);
            *(SVECTOR *)&spawned->unknown_50 = direction;
            *(SVECTOR *)&spawned->rotation = *(SVECTOR *)&current->rotation;
            actor_select_target_type_in_own_group(spawned, 0x1e);
        }
        break;
    case 0x1d:
    case 0x1f:
        target = *player;
        count = 6;
        do {
            distance = fixed_vector2_length(target.vx - position.vx,
                                            target.vz - position.vz);
            if (func_80015918(0, distance,
                    position.vy + 1400 - target.vy, 10, 800,
                    &travel_time, &trajectory_angle) != 0) {
                trajectory_angle = 0x100;
            }
            count--;
            if (count != 0) {
                func_80015ce0(player, (SVECTOR *)&player_state.unknown_e8,
                              travel_time >> 6, &target);
            }
        } while (count != 0);
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
    }
    va_end(arguments);
}
