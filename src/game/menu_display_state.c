#include <kf/lib/address.h>
#include <kf/game/audio.h>
#include <kf/game/graphics.h>
#include <kf/game/menu.h>
#include <kf/game/player.h>
#include <kf/game/pool.h>
#include <psyq/audio.h>

ADDRESS(0x80021c8c, 0x174)
void func_80021c8c(s32 mode)
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
void func_80021e00(s32 stop_sequence)
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
