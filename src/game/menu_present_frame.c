#include <kf/lib/address.h>
#include <kf/game/graphics.h>
#include <kf/game/menu.h>

ADDRESS(0x80021be0, 0xac)
void menu_present_frame(void)
{
    DrawSync(0);
    VSync(0);
    PutDrawEnv(&game_graphics_runtime.display_draw_environments[
        game_graphics_runtime.display_state.buffer_index]);
    PutDispEnv(&game_graphics_runtime.display_disp_environments[
        game_graphics_runtime.display_state.buffer_index]);
    LoadImage(&menu_frame_upload_rect, menu_frame_upload_pixels);
    DrawOTag(game_graphics_runtime.display_state.ordering_table
        + (KF_GAME_ORDERING_TABLE_LENGTH - 1));
}
