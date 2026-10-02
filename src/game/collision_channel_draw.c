#include <kf/lib/address.h>
#include <kf/game/graphics.h>

ADDRESS(0x800314fc, 0x138)
void render_accumulated_color_overlay(void)
{
    if (game_graphics_runtime.unknown_14cc5 != 0) {
        func_800311b0(0, 0, 0x140, 0xf0,
                      0x80, 0xd0, 0xf, 0xf, 1, 0x37, 0x7bdc,
                      *(s16 *)&game_graphics_runtime.unknown_14cc6 /
                          game_graphics_runtime.unknown_14cc5,
                      *(s16 *)&game_graphics_runtime.unknown_14cc8 /
                          game_graphics_runtime.unknown_14cc5,
                      *(s16 *)&game_graphics_runtime.unknown_14cca /
                          game_graphics_runtime.unknown_14cc5,
                      0x40);
    }
}
