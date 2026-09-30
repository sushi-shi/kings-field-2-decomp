#include <kf/lib/address.h>
#include <kf/game/card.h>
#include <kf/game/menu.h>
#include <psyq/kernel.h>
#include <psyq/pad.h>

extern void func_8001c550(KfMenuGlyphString *rows);
extern void func_8001ba80(KfMenuGlyphString *rows);
extern void func_8001bb94(KfMenuGlyphString *rows);
extern s32 func_8001b834(void);
extern s32 func_800226ec(struct DIRENTRY *entries, s32 *matching_count);

ADDRESS(0x8001b554, 0x2e0)
s32 func_8001b554(void)
{
    struct DIRENTRY entries[15];
    KfMenuGlyphString rows[4];
    s32 matching_count;
    s32 cursor = 0;
    s32 confirmed = 0;
    s32 result = -99;
    s32 selection = -1;
    s32 card_full;
    s32 probe;
    s32 buttons;
    s32 frame;

    func_80021c8c(0);
    func_8001c550(rows);
    func_8001b030(9, rows, 2, 70, 87, 178, 66, 2, 0);
    memory_card_start();
    probe = memory_card_probe_temporary_file();
    if (probe != 0 && probe != 2) {
        func_8001ba80(rows);
        func_8001b030(9, rows, 3, 50, 87, 220, 66, 2, 0);
        input_wait_release();
        while (PadRead(1) == 0) {}
        input_wait_release();
        goto no_file;
    }

    card_full = func_800226ec(entries, &matching_count);
    if (matching_count == 0) {
        if (card_full == 1) {
            func_8001bb94(rows);
            func_8001b030(9, rows, 4, 70, 87, 178, 96, 2, 0);
            input_wait_release();
            while (PadRead(1) == 0) {}
            input_wait_release();
        }
no_file:
        memory_card_stop();
        func_80021e00(0);
        return -1;
    }

    func_80022300(16);
    input_wait_release();
    for (;;) {
        if (selection != -1)
            input_wait_release();
        switch (selection) {
        case 0:
            result = -1;
            break;
        case 1:
            result = func_8001b834();
            if (result == -1) {
                result = -99;
                selection = -1;
            }
            break;
        }
        if (result != -99)
            break;

        func_800223cc();
        buttons = input_read_mark_active();
        if ((buttons & 0x1000) || (buttons & 0x4000)) {
            menu_cursor_animation_direction = 0;
            func_80022300(16);
            if (cursor == 0)
                cursor = 1;
            else
                cursor = 0;
        } else if (buttons & 0x20) {
            func_80022300(17);
            confirmed = 1;
            selection = cursor;
        }
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            menu_draw_window(6, 2, cursor, confirmed);
            menu_present_frame();
        }
    }
    memory_card_stop();
    func_80021e00(0);
    return result;
}
