#include <kf/lib/address.h>
#include <kf/game/graphics.h>

ADDRESS(0x80031414, 0xc0)
void render_color_overlay(void)
{
    if (game_graphics_runtime.unknown_14cc1 != 0xff) {
        func_800311b0(0, 0, 0x140, 0xf0,
                      0x80, 0xd0, 0xf, 0xf, 1,
                      ((game_graphics_runtime.unknown_14cc1 & 3) << 5) | 0x17,
                      0x7bdc,
                      game_graphics_runtime.unknown_14cc2[0],
                      game_graphics_runtime.unknown_14cc2[1],
                      game_graphics_runtime.unknown_14cc2[2],
                      (game_graphics_runtime.unknown_14cc1 & 0x80) ? 1 : 0x40);
    }
}
