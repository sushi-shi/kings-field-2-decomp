#include <kf/lib/address.h>
#include <kf/game/player.h>

ADDRESS(0x800251f0, 0x44)
void player_clear_motion(void)
{
    player_state.yaw_step = 0;
    player_state.pitch_step = 0;
    player_state.movement_speed = 0;
    player_state.forward_velocity = 0;
    player_state.strafe_velocity = 0;
    player_state.unknown_140 &= KF_PLAYER_MOTION_FLAGS_KEPT;
}
