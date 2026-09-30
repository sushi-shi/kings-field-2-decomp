#include <kf/lib/address.h>
#include <kf/game/graphics.h>
#include <kf/game/menu.h>

enum {
    KF_MENU_UPLOAD_SECOND_BUFFER_Y = 240,
    KF_MENU_CURSOR_FRAME_COUNT = 8
};

ADDRESS(0x80021a60, 0x8)
void func_80021a60(void)
{
}

ADDRESS(0x80021a68, 0x178)
void menu_frame_begin(void)
{
    game_graphics_runtime.display_state.buffer_index =
        game_graphics_runtime.display_state.buffer_index == 0;
    game_graphics_runtime.display_state.primitive_buffer =
        &game_graphics_runtime.display_state.primitive_buffers[
            game_graphics_runtime.display_state.buffer_index];
    game_graphics_runtime.display_state.ordering_table =
        game_graphics_runtime.display_state.ordering_tables[
            game_graphics_runtime.display_state.buffer_index].entries;
    ClearOTagR(game_graphics_runtime.display_state.ordering_table,
        KF_GAME_ORDERING_TABLE_LENGTH);
    game_graphics_runtime.display_state.primitive_buffer->cursor =
        game_graphics_runtime.display_state.primitive_buffer->start;
    current_poly_ft4 = (POLY_FT4 *)
        game_graphics_runtime.display_state.primitive_buffer->cursor;

    if (game_graphics_runtime.display_state.buffer_index == 1) {
        menu_frame_upload_rect.x = 0;
        menu_frame_upload_rect.y = KF_MENU_UPLOAD_SECOND_BUFFER_Y;
    } else {
        menu_frame_upload_rect.x = 0;
        menu_frame_upload_rect.y = 0;
    }

    if (menu_cursor_animation_direction == 0) {
        menu_cursor_animation_frame++;
    } else if (menu_cursor_animation_direction == 1) {
        menu_cursor_animation_frame--;
    }
    if (menu_cursor_animation_frame >= KF_MENU_CURSOR_FRAME_COUNT) {
        menu_cursor_animation_frame = KF_MENU_CURSOR_FRAME_COUNT - 1;
        menu_cursor_animation_direction = 1;
    }
    if (menu_cursor_animation_frame < 0) {
        menu_cursor_animation_frame = 0;
        menu_cursor_animation_direction = -1;
    }
}
