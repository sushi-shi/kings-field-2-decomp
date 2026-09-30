#include <kf/lib/address.h>
#include <kf/game/player.h>

enum {
    KF_PLAYER_STATUS_CAP = 64,
    KF_PLAYER_STATUS_FIRST = 1,
    KF_PLAYER_STATUS_SECOND = 2,
    KF_PLAYER_STATUS_THIRD = 4
};

ADDRESS(0x800247e4, 0xc4)
void player_cap_status_components(u32 mask)
{
    if (mask & KF_PLAYER_STATUS_FIRST) {
        if (player_state.curse_strength >= KF_PLAYER_STATUS_CAP + 1) {
            player_state.curse_strength = KF_PLAYER_STATUS_CAP;
        }
        player_state.unknown_58 = 0;
    }
    if (mask & KF_PLAYER_STATUS_SECOND) {
        if (player_state.unknown_5a >= KF_PLAYER_STATUS_CAP + 1) {
            player_state.unknown_5a = KF_PLAYER_STATUS_CAP;
        }
        player_state.unknown_5c = 0;
    }
    if (mask & (KF_PLAYER_STATUS_FIRST | KF_PLAYER_STATUS_SECOND)) {
        player_state.unknown_54 = 0;
    }
    if (mask & (KF_PLAYER_STATUS_FIRST | KF_PLAYER_STATUS_THIRD)) {
        if (player_state.unknown_5e >= KF_PLAYER_STATUS_CAP + 1) {
            player_state.unknown_5e = KF_PLAYER_STATUS_CAP;
        }
    }
    if (mask & KF_PLAYER_STATUS_THIRD) {
        player_state.unknown_60 = 0;
    }
}
