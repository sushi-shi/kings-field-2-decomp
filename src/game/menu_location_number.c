#include <kf/game/callback.h>
#include <kf/game/card.h>
#include <kf/game/menu.h>
#include <kf/game/player.h>
#include <kf/lib/address.h>
#include <psyq/pad.h>

RODATA(0x80011098, 0x1c)

ADDRESS(0x8001876c, 0x284)
s32 func_8001876c(void)
{
    s32 cursor = 0;
    s32 confirmed = 0;
    s32 result = -99;
    s32 selection = -1;
    s32 choice_result;
    s32 frame;
    u32 buttons;

    func_80021c8c(1);
    for (frame = 0; frame < 2; frame++) {
        menu_frame_begin();
        func_8001e94c();
        menu_draw_window(0, 8, cursor, confirmed);
        menu_present_frame();
    }
    func_80022300(16);
    input_wait_release();

    for (;;) {
        if (selection != -1 || result != -99)
            input_wait_release();

        switch (selection) {
        case 0:
            choice_result = func_80018ac8();
            goto selection_result;
        case 1:
            choice_result = func_80019834();
            goto selection_result;
        case 2:
            func_80019ac4();
            break;
        case 3:
            func_8001a7fc();
            break;
        case 4:
            func_8001a898();
            break;
        case 5:
            choice_result = func_8001aa9c();
selection_result:
            result = choice_result;
            if (choice_result == -1)
                result = -99;
            break;
        case 6:
            func_8001b2dc();
            break;
        }

        if (result != -99)
            break;

        cursor = func_8001e378(cursor, 7, &selection, &confirmed, &result);
        buttons = PadRead(1);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            if ((buttons & PADR1) != 0 && (buttons & PADL1) != 0)
                func_800189f0();
            func_8001e94c();
            menu_draw_window(0, 8, cursor, confirmed);
            menu_present_frame();
        }
    }

    if (result == -1)
        func_80022300(0);
    if (result == -3)
        func_80021e00(1);
    else
        func_80021e00(0);
    if (result != -1 && result != -3 && (result & 0x1000) != 0) {
        func_8002722c(result & 0xfff);
        result = -1;
    }
    return result;
}

ADDRESS(0x800189f0, 0xd8)
void func_800189f0(void)
{
    KfMenuGlyphString row;
    s32 camera_x;
    s32 camera_z;
    s32 grid_z;
    s32 prefix;
    u16 map_layer;
    s32 value;

    row.position.x = 252;
    row.position.y = 205;
    camera_x = player_state.camera_position.vx;
    camera_z = player_state.camera_position.vz;
    map_layer = player_state.unknown_128;
    grid_z = camera_z >> 11;
    prefix = state_8017d118.unknown_09[0] * 100000
           + map_layer * 10000
           + (camera_x >> 11) * 100;
    value = prefix + grid_z;
    menu_format_number(value, 6, 1, 0, row.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &row);
}
