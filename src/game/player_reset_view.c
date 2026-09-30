#include <kf/lib/address.h>
#include <kf/game/player.h>

enum {
    PLAYER_VIEW_SCALE_INITIAL = 0x1000,
    PLAYER_VIEW_TIMER_INITIAL = 10000
};

ADDRESS(0x80023484, 0xec)
void player_reset_view(void)
{
    player_state.unknown_134 = 0;
    player_state.unknown_136 = 0;
    player_state.unknown_138 = 0;
    player_state.unknown_ce[2] = 0;
    player_state.unknown_13a = 0;
    player_state.death_state = 0;
    player_state.unknown_110[2] = 0;
    player_state.unknown_110[1] = 0;
    player_state.unknown_110[0] = 0;
    player_state.unknown_108[2] = 0;
    player_state.unknown_108[1] = 0;
    player_state.unknown_108[0] = 0;
    player_state.unknown_100[2] = 0;
    player_state.unknown_100[1] = 0;
    player_state.unknown_100[0] = 0;
    player_state.camera_rotation = player_state.camera_rotation_target;
    player_state.unknown_0e = 0;
    player_state.unknown_0c[1] = 0;
    player_state.damage_scale = PLAYER_VIEW_SCALE_INITIAL;
    player_state.unknown_ce[3] = 0xff;
    player_state.unknown_120 = PLAYER_VIEW_TIMER_INITIAL;
    player_state.unknown_124 = PLAYER_VIEW_TIMER_INITIAL;
}
