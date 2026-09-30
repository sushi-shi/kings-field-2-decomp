#include <kf/game/graphics.h>
#include <kf/lib/address.h>

ADDRESS(0x800314d4, 0x28)
void func_800314d4(u8 control, u8 red, u8 green, u8 blue)
{
    game_graphics_runtime.unknown_14cc1 = control;
    game_graphics_runtime.unknown_14cc2[0] = red;
    game_graphics_runtime.unknown_14cc2[1] = green;
    game_graphics_runtime.unknown_14cc2[2] = blue;
}
