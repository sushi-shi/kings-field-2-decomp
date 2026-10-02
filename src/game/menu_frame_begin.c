#include <kf/lib/address.h>
#include <kf/game/audio.h>
#include <kf/game/graphics.h>
#include <kf/game/menu.h>
#include <kf/game/player.h>
#include <kf/game/pool.h>
#include <psyq/audio.h>

enum {
    KF_MENU_UPLOAD_SECOND_BUFFER_Y = 240,
    KF_MENU_CURSOR_FRAME_COUNT = 8
};

DATA(0x8006d698, 0x4)
s32 menu_cursor_animation_frame = 0;

DATA(0x8006d69c, 0x4)
s32 menu_cursor_animation_direction = 0;

DATA(0x8006d9e0, 0x4)
POLY_FT4 *current_poly_ft4;

DATA(0x8006d9e8, 0x1)
u8 menu_saved_music_enabled;

DATA(0x8006d9f0, 0x4)
u_long *menu_frame_upload_pixels;

DATA(0x8006d9f8, 0x8)
RECT menu_frame_upload_rect;

DATA(0x8006dbe8, 0x18)
KfPrimitiveBuffer menu_saved_primitive_buffers[KF_DISPLAY_BUFFER_COUNT];

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

ADDRESS(0x80021c8c, 0x174)
void menu_enter_display_state(s32 mode)
{
    pool_release_all();
    game_graphics_runtime.display_draw_environments[0].isbg = 0;
    game_graphics_runtime.display_draw_environments[0].dfe = 0;
    game_graphics_runtime.display_draw_environments[1].isbg = 0;
    game_graphics_runtime.display_draw_environments[1].dfe = 0;

    menu_saved_primitive_buffers[0] =
        game_graphics_runtime.display_state.primitive_buffers[0];
    menu_saved_primitive_buffers[1] =
        game_graphics_runtime.display_state.primitive_buffers[1];
    game_graphics_runtime.display_state.primitive_buffers[0].end =
        game_graphics_runtime.display_state.primitive_buffers[0].start + 0x6400;
    game_graphics_runtime.display_state.primitive_buffers[1].start =
        game_graphics_runtime.display_state.primitive_buffers[0].end;
    game_graphics_runtime.display_state.primitive_buffers[1].end =
        game_graphics_runtime.display_state.primitive_buffers[1].start + 0x6400;
    menu_frame_upload_pixels = (u_long *)
        game_graphics_runtime.display_state.primitive_buffers[1].end;

    if (game_graphics_runtime.display_state.buffer_index == 1) {
        menu_frame_upload_rect.x = 0;
        menu_frame_upload_rect.y = 240;
    } else {
        menu_frame_upload_rect.x = 0;
        menu_frame_upload_rect.y = 0;
    }
    menu_frame_upload_rect.w = 320;
    menu_frame_upload_rect.h = 240;
    StoreImage(&menu_frame_upload_rect, menu_frame_upload_pixels);
    DrawSync(0);

    menu_saved_music_enabled = player_state.audio_music_enabled;
    if (player_state.audio_music_enabled == 1 && audio_state.sequence_active == 1)
        SsSeqPause(audio_state.sequence_id);
}

ADDRESS(0x80021e00, 0x110)
void menu_exit_display_state(s32 stop_sequence)
{
    u8 music_enabled;

    game_graphics_runtime.display_state.primitive_buffers[0] =
        menu_saved_primitive_buffers[0];
    game_graphics_runtime.display_state.primitive_buffers[1] =
        menu_saved_primitive_buffers[1];
    game_graphics_runtime.display_draw_environments[0].isbg = 1;
    game_graphics_runtime.display_draw_environments[0].dfe = 1;
    game_graphics_runtime.display_draw_environments[1].isbg = 1;
    game_graphics_runtime.display_draw_environments[1].dfe = 1;

    if (stop_sequence == 1) {
        audio_stop_sequence();
    } else {
        music_enabled = player_state.audio_music_enabled;
        if (music_enabled != menu_saved_music_enabled) {
            if (music_enabled == 0)
                audio_stop_sequence();
            else
                audio_start_sequence();
        } else if (music_enabled == 1 && audio_state.sequence_active == 1) {
            SsSeqReplay(audio_state.sequence_id);
        }
    }
}
