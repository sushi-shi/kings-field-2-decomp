#include <kf/lib/address.h>
#include <kf/game/card.h>
#include <kf/game/menu.h>
#include <psyq/pad.h>

extern void func_8001fc94(void *list_state, s32 render_mode);

ADDRESS(0x8001f8b8, 0x2d4)
s32 func_8001f8b8(void *list_state, s32 label_kind,
    s32 render_mode, s32 item_id)
{
    KfMenuGlyphString labels[2];
    s32 choice = 0;
    s32 result = -99;
    s32 confirmed;
    s32 frame;
    u32 buttons;

    labels[0].position.x = menu_window_layouts[1].rows[0].position.x;
    labels[0].position.y = menu_window_layouts[1].rows[0].position.y;
    labels[1].position.x = menu_window_layouts[1].rows[1].position.x;
    labels[1].position.y = menu_window_layouts[1].rows[1].position.y;

    if (label_kind == 0) {
        labels[0].glyphs.codes[0] = 114;
        labels[0].glyphs.codes[1] = 66;
        labels[0].glyphs.codes[2] = -1;
    } else if (label_kind == 1) {
        labels[0].glyphs.codes[0] = 117;
        labels[0].glyphs.codes[1] = 82;
        labels[0].glyphs.codes[2] = 106;
        labels[0].glyphs.codes[3] = -1;
    } else if (label_kind == 2) {
        labels[0].glyphs.codes[0] = 89;
        labels[0].glyphs.codes[1] = 65;
        labels[0].glyphs.codes[2] = -1;
    } else if (label_kind == 3) {
        labels[0].glyphs.codes[0] = 116;
        labels[0].glyphs.codes[1] = 66;
        labels[0].glyphs.codes[2] = -1;
    } else if (label_kind == 4) {
        labels[0].glyphs.codes[0] = 115;
        labels[0].glyphs.codes[1] = 106;
        labels[0].glyphs.codes[2] = -1;
    } else if (label_kind == 5) {
        labels[0].glyphs.codes[0] = 112;
        labels[0].glyphs.codes[1] = 113;
        labels[0].glyphs.codes[2] = -1;
    } else if (label_kind == 6) {
        labels[0].glyphs.codes[0] = 44;
        labels[0].glyphs.codes[1] = 45;
        labels[0].glyphs.codes[2] = 0x1013;
        labels[0].glyphs.codes[3] = -1;
    } else if (label_kind == 7) {
        labels[0].glyphs.codes[0] = 13;
        labels[0].glyphs.codes[1] = 45;
        labels[0].glyphs.codes[2] = 0x101b;
        labels[0].glyphs.codes[3] = -1;
    } else if (label_kind == 8) {
        labels[0].glyphs.codes[0] = 0x104;
        labels[0].glyphs.codes[1] = 71;
        labels[0].glyphs.codes[2] = -1;
    } else if (label_kind == 9) {
        labels[0].glyphs.codes[0] = 0x105;
        labels[0].glyphs.codes[1] = 0x106;
        labels[0].glyphs.codes[2] = -1;
    } else {
        labels[0].glyphs.codes[0] = 207;
        labels[0].glyphs.codes[1] = 106;
        labels[0].glyphs.codes[2] = -1;
    }

    if (label_kind == 2) {
        labels[1].glyphs.codes[0] = 65;
        labels[1].glyphs.codes[1] = 65;
        labels[1].glyphs.codes[2] = 67;
    } else {
        labels[1].glyphs.codes[0] = 99;
        labels[1].glyphs.codes[1] = 97;
        labels[1].glyphs.codes[2] = 106;
    }
    labels[1].glyphs.codes[3] = -1;

    for (;;) {
        if (result != -99) {
            input_wait_release();
            return result;
        }
        func_800223cc();
        buttons = input_read_mark_active();
        confirmed = 0;
        if ((buttons & PADLup) || (buttons & PADLdown)) {
            menu_cursor_animation_direction = 0;
            func_80022300(16);
            if (choice != 0)
                choice = 0;
            else
                choice = 1;
        } else if (buttons & PADRright) {
            func_80022300(17);
            confirmed = 1;
            result = -choice;
        } else if (buttons & PADRdown) {
            func_80022300(18);
            result = -1;
        }

        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            func_8002083c((u8)item_id);
            func_8001fc94(list_state, render_mode);
            menu_draw_two_option(&labels[0], &labels[1], choice, confirmed);
            menu_present_frame();
        }
    }
}
