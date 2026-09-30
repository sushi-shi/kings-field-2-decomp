#include <kf/lib/address.h>
#include <kf/lib/math.h>
#include <kf/game/actor.h>
#include <kf/game/player.h>

ADDRESS(0x800396c4, 0x4c)
void actor_select_target_for_player_distance(void)
{
    KfActor *actor = actor_state.current;
    s32 distance = fixed_vector2_length(
        actor->position.vx - player_state.camera_position.vx,
        actor->position.vz - player_state.camera_position.vz);

    actor_select_best_target(distance);
}
