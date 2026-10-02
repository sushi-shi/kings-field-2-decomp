#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/lib/null.h>
#include <kf/game/collision_cache.h>
#include <kf/game/player.h>


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

    flags = collision_query_world(next.vx, next.vy, next.vz, 800, 1700, 49);
    if (flags == 0) {
    accept:
        if (player_state.reaction.damage.rotation.vy >= 160
            && KF_COLLISION_CACHE_RESULT - player_state.camera_position.vy > 32000) {
            player_death_begin(NULL);
            player_state.unknown_d1[4] = 1;
        }
        player_state.camera_position.vx = next.vx;
        player_state.camera_position.vy = next.vy;
        player_state.camera_position.vz = next.vz;
        player_state.reaction.damage.rotation.vy += 32;
        goto accepted;
    }

    next.vy = player_state.camera_position.vy;
    flags = collision_query_world(next.vx, next.vy, next.vz, 800, 1700, 49);
    if (flags == 0) {
        player_state.reaction.damage.rotation.vy = 1;
        flags = collision_query_world(next.vx, next.vy, next.vz, 800, 1700, 49);
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

    if (flags & -6) {
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
    func_80023384();
    return 0;
}
