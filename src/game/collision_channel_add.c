#include <kf/lib/address.h>
#include <kf/game/graphics.h>

ADDRESS(0x80031634, 0x94)
void func_80031634(s32 first, s32 second, s32 third, s32 scale)
{
    game_graphics_runtime.unknown_14cc5++;
    game_graphics_runtime.unknown_14cc6 += (first * scale) >> 12;
    game_graphics_runtime.unknown_14cc8 += (second * scale) >> 12;
    game_graphics_runtime.unknown_14cca += (third * scale) >> 12;
}
