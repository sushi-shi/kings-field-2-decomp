#include <kf/game/player.h>
#include <kf/lib/address.h>

void func_800192ac(void);
void func_800192dc(void);

ADDRESS(0x80019240, 0x6c)
void func_80019240(void)
{
    if (player_state.unknown_60 > 0)
        player_state.unknown_60 = 0;
    if (player_state.unknown_5e > 64)
        player_state.unknown_5e = 64;
    player_state.unknown_54 = 0;
    func_800192dc();
    func_800192ac();
}

ADDRESS(0x800192ac, 0x30)
void func_800192ac(void)
{
    if (player_state.unknown_5a > 64) {
        player_state.unknown_5a = 64;
        player_state.unknown_5c = 0;
    }
}

ADDRESS(0x800192dc, 0x30)
void func_800192dc(void)
{
    if (player_state.curse_strength > 64) {
        player_state.curse_strength = 64;
        player_state.unknown_58 = 0;
    }
}
