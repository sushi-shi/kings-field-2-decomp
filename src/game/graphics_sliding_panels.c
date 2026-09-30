#include <kf/lib/address.h>
#include <kf/game/graphics.h>
#include <kf/game/player.h>

ADDRESS(0x800312f4, 0x90)
void func_800312f4(void)
{
    s32 y = player_state.unknown_120;

    if (y < 240) {
        y += 120;
        if (y < 0) {
            y = 0;
        }
        func_800311b0(0, y, 320, 240, 128, 192, 15, 15, 1, 55,
                      0x7bdc, 60, 60, 60, 64);
    }
}

ADDRESS(0x80031384, 0x90)
void func_80031384(void)
{
    s32 y = player_state.unknown_124;

    if (y < 240) {
        y += 120;
        if (y < 0) {
            y = 0;
        }
        func_800311b0(0, y, 320, 240, 144, 192, 15, 15, 1, 55,
                      0x7bdc, 60, 60, 60, 64);
    }
}
