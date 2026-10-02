#include <kf/lib/address.h>
#include <kf/game/actor.h>
#include <kf/game/callback.h>
#include <kf/game/player.h>
#include <psyq/libc.h>

typedef s32 (*KfCandidateScoreCallback)(KfTargetCandidate *target,
                                         s32 player_distance);

RODATA(0x80011cd8, 0x20c)

ADDRESS(0x80039108, 0x4c0)
s32 func_80039108(KfTargetCandidate *target, s32 player_distance)
{
    KfActor *actor = actor_state.current;
    s32 score;
    s32 angle;

    if (target->type == 0xff) {
        return 0;
    }

    score = 0;

    switch (target->type) {
    case 5:
    case 13:
        if (target == actor->target) {
            if (target->word_12.value < player_distance) {
                goto done;
            }
            score = func_800157ac(target->unknown_01[2]);
            goto done;
        } else if (target->word_10.value >= player_distance) {
            score = func_800157ac(target->unknown_01[1]);
        }
        switch (actor->target_type) {
        case 4:
        case 18:
        case 23:
        case 24:
        case 132:
            score *= 2;
            break;
        }
        goto done;

    case 9:
        if (target->word_0c.value < player_distance) {
            goto done;
        }
        if ((u32)(player_state.camera_position.vy - actor->position.vy + 1023) < 2047 &&
            rand() >= 4096) {
            goto done;
        }
        angle = vector_xz_to_angle(player_state.camera_position.vx - actor->position.vx,
                                   player_state.camera_position.vz - actor->position.vz);
        if (!angle_within_tolerance(actor->rotation.y, angle, 0x140)) {
            goto done;
        }
        goto score_target;

    case 4:
    case 18:
    case 23:
    case 24:
    case 132:
        if ((actor->unknown_28 & 0x100) || target->word_1a.value < player_distance ||
            !func_80015574(actor->position.vy, actor->unknown_1e,
                            player_state.camera_position.vy + 200, 0x834)) {
            goto done;
        }
        angle = vector_xz_to_angle(player_state.camera_position.vx - actor->position.vx,
                                   player_state.camera_position.vz - actor->position.vz);
        if (!angle_within_tolerance(actor->rotation.y, angle,
                                    target->word_10.bytes.fallback_offset << 4)) {
            goto done;
        }
        if (target == actor->target) {
            score = func_800157ac(target->unknown_01[2]);
            goto done;
        }
        score = target->unknown_01[1];
        {
            s32 near_distance = target->word_0e.bytes.low << 6;
            s32 middle_distance = (near_distance + target->word_1a.value) >> 1;
            if (player_distance >= middle_distance) {
                score -= score / 3;
            } else if (player_distance >= near_distance) {
                score -= score / 6;
            }
        }
        score = func_800157ac(score);
        goto done;

    case 25:
        if ((actor->unknown_28 & 0x100) || target->word_16.value < player_distance ||
            target->word_14.value > player_distance) {
            goto done;
        }
        angle = vector_xz_to_angle(player_state.camera_position.vx - actor->position.vx,
                                   player_state.camera_position.vz - actor->position.vz);
        if (angle_within_tolerance(actor->rotation.y, angle,
                                   target->word_0c.bytes.high << 4)) {
            break;
        }
        goto done;

    case 11:
        if (target->word_1a.value < player_distance) {
            goto done;
        }
        angle = vector_xz_to_angle(player_state.camera_position.vx - actor->position.vx,
                                   player_state.camera_position.vz - actor->position.vz);
        if (angle_within_tolerance(actor->rotation.y, angle, 0x140)) {
            break;
        }
        goto done;

    case 19:
    case 20:
        if (target->word_0e.value < player_distance || player_state.weapon_attack_phase == -1) {
            goto done;
        }
        angle = vector_xz_to_angle(player_state.camera_position.vx - actor->position.vx,
                                   player_state.camera_position.vz - actor->position.vz);
        if (angle_within_tolerance(actor->rotation.y, angle,
                                   target->word_0c.bytes.low << 5)) {
            break;
        }
        goto done;

    case 112:
        score = -1;
        goto done;
    case 27:
        if (player_distance >= target->word_0c.value) {
            break;
        }
        /* Fall through to the zero-score cases. */
    case 2:
    case 3:
    case 22:
        score = 0;
        goto done;
    default:
        if (target->type >= 128 &&
            !((KfCandidateScoreCallback)state_8017d118.active_table[16])(
                target, player_distance)) {
            goto done;
        }
        break;
    }

score_target:
    if (target == actor->target) {
        score = func_800157ac(target->unknown_01[2]);
    } else {
        score = func_800157ac(target->unknown_01[1]);
    }
done:
    return score;
}
