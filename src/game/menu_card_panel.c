#include <kf/lib/address.h>
#include <kf/game/card.h>
#include <kf/game/menu.h>
#include <kf/game/player.h>
#include <psyq/pad.h>

ADDRESS(0x8001b030, 0x11c)
void menu_show_dialog_panel(s32 panel, const KfMenuGlyphString *rows, s32 count,
    s32 detail0, s32 detail1, s32 detail2, s32 detail3, s32 detail4,
    s32 detail5)
{
    s32 frame = 0;
    const KfMenuGlyphString *title = &menu_window_layouts[1].rows[panel];
    const KfMenuGlyphString *current;
    s32 row;

    do {
        menu_frame_begin();
        if (panel < 6) {
            menu_blit_sprite_translucent(&menu_sprite_defs[5], &title->position);
            menu_draw_string(&menu_sprite_defs[1], title);
        }
        current = rows;
        for (row = 0; row < count; row++, current++)
            menu_draw_string(&menu_sprite_defs[1], current);
        menu_draw_nine_slice_panel(detail0, detail1, detail2, detail3, detail4, detail5);
        menu_present_frame();
        frame++;
    } while (frame < 2);
}

ADDRESS(0x8001b14c, 0x190)
s32 menu_prompt_two_option(void)
{
    KfMenuGlyphString labels[2];
    s32 choice;
    s32 selected;
    s32 result;
    s32 current;
    s32 frame;
    s32 cursor_frame;

    current = 0;
    choice = 0;
    result = -99;
    selected = -1;
    labels[0].position.x = 101;
    labels[0].position.y = 123;
    labels[0].glyphs.codes[0] = 89;
    labels[0].glyphs.codes[1] = 65;
    labels[0].glyphs.codes[2] = -1;
    labels[1].position.x = 101;
    labels[1].position.y = 149;
    labels[1].glyphs.codes[0] = 65;
    labels[1].glyphs.codes[1] = 65;
    labels[1].glyphs.codes[2] = 67;
    labels[1].glyphs.codes[3] = -1;

    for (;;) {
        if (selected != -1 || result != -99)
            input_wait_release();

        switch (selected) {
        case 0:
            result = 0;
            break;
        case 1:
            result = -1;
            break;
        }

        if (result != -99)
            break;
        current = menu_poll_choice_input(current, 1, &selected, &choice, &result);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            cursor_frame = menu_cursor_animation_frame;
            menu_cursor_animation_frame = 0;
            menu_draw_window(1, 3, 1, 1);
            menu_cursor_animation_frame = cursor_frame;
            menu_draw_two_option(&labels[0], &labels[1], current, choice);
            menu_present_frame();
        }
    }
    return result;
}
enum {
    KF_MENU_OPTION_COUNT = 6,
    KF_MENU_OPTION_CANCEL_ROW = 6,
    KF_MENU_OPTION_PENDING = -99
};

ADDRESS(0x8001b2dc, 0x278)
void menu_options_controller(void)
{
    u8 selected[KF_MENU_OPTION_COUNT];
    KfMenuGlyphString labels[2];
    s32 choice = 0;
    s32 result = KF_MENU_OPTION_PENDING;
    s32 last_row = KF_MENU_OPTION_COUNT;
    s32 confirmed;
    s32 frame;
    u32 buttons;

    labels[0].position.x = 180;
    labels[0].position.y = 45;
    labels[0].glyphs.codes[0] = 240;
    labels[0].glyphs.codes[1] = 241;
    labels[0].glyphs.codes[2] = KF_MENU_TEXT_END;
    labels[1].position.x = 254;
    labels[1].position.y = 45;
    labels[1].glyphs.codes[0] = 242;
    labels[1].glyphs.codes[1] = 243;
    labels[1].glyphs.codes[2] = 244;
    labels[1].glyphs.codes[3] = KF_MENU_TEXT_END;

    selected[0] = player_state.audio_effects_enabled;
    selected[1] = player_state.audio_music_enabled;
    selected[2] = player_state.unknown_c9[0];
    selected[3] = player_state.unknown_c9[1];
    selected[4] = player_state.unknown_c9[2];
    selected[5] = player_state.unknown_c9[3];

    for (;;) {
        if (result != KF_MENU_OPTION_PENDING) {
            input_wait_release();
            break;
        }

        input_wait_brief_release();
        buttons = input_read_mark_active();
        confirmed = 0;
        if (buttons & PADLup) {
            menu_cursor_animation_direction = 0;
            menu_play_sound_cue(16);
            if (choice != 0)
                choice--;
            else
                choice = last_row;
        } else if (buttons & PADLdown) {
            menu_cursor_animation_direction = 0;
            menu_play_sound_cue(16);
            if (choice != last_row)
                choice++;
            else
                choice = 0;
        } else if ((buttons & PADRright) || (buttons & PADLright)
            || (buttons & PADLleft)) {
            if (choice < last_row) {
                menu_play_sound_cue(17);
                confirmed = 1;
                selected[choice] = selected[choice] == 0;
            } else if (buttons & PADRright) {
                menu_play_sound_cue(17);
                result = -1;
                confirmed = 1;
            }
        } else if (buttons & PADRdown) {
            menu_play_sound_cue(18);
            result = -1;
        }

        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            menu_draw_window(2, 7, choice, confirmed);
            func_8001f798(&labels[0], &labels[1], selected);
            menu_present_frame();
        }
    }

    player_state.audio_effects_enabled = selected[0];
    player_state.audio_music_enabled = selected[1];
    player_state.unknown_c9[0] = selected[2];
    player_state.unknown_c9[1] = selected[3];
    player_state.unknown_c9[2] = selected[4];
    player_state.unknown_c9[3] = selected[5];
}
