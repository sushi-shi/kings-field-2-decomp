#include <kf/game/callback.h>
#include <kf/game/card.h>
#include <kf/game/menu.h>
#include <kf/game/player.h>
#include <kf/lib/address.h>
#include <psyq/pad.h>

RODATA(0x80011098, 0x1c)

ADDRESS(0x8001876c, 0x284)
s32 menu_run_root_controller(void)
{
    s32 cursor = 0;
    s32 confirmed = 0;
    s32 result = -99;
    s32 selection = -1;
    s32 choice_result;
    s32 frame;
    u32 buttons;

    menu_enter_display_state(1);
    for (frame = 0; frame < 2; frame++) {
        menu_frame_begin();
        menu_draw_player_status();
        menu_draw_window(0, 8, cursor, confirmed);
        menu_present_frame();
    }
    menu_play_sound_cue(16);
    input_wait_release();

    for (;;) {
        if (selection != -1 || result != -99)
            input_wait_release();

        switch (selection) {
        case 0:
            choice_result = menu_item_selection_controller();
            goto selection_result;
        case 1:
            choice_result = menu_choose_magic_action();
            goto selection_result;
        case 2:
            menu_equipment_list_controller();
            break;
        case 3:
            menu_show_combat_attributes();
            break;
        case 4:
            menu_item_use_controller();
            break;
        case 5:
            choice_result = menu_run_card_choice();
selection_result:
            result = choice_result;
            if (choice_result == -1)
                result = -99;
            break;
        case 6:
            menu_options_controller();
            break;
        }

        if (result != -99)
            break;

        cursor = menu_poll_choice_input(cursor, 7, &selection, &confirmed, &result);
        buttons = PadRead(1);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            if ((buttons & PADR1) != 0 && (buttons & PADL1) != 0)
                menu_draw_location_number();
            menu_draw_player_status();
            menu_draw_window(0, 8, cursor, confirmed);
            menu_present_frame();
        }
    }

    if (result == -1)
        menu_play_sound_cue(0);
    if (result == -3)
        menu_exit_display_state(1);
    else
        menu_exit_display_state(0);
    if (result != -1 && result != -3 && (result & 0x1000) != 0) {
        player_select_magic_action(result & 0xfff);
        result = -1;
    }
    return result;
}

ADDRESS(0x800189f0, 0xd8)
void menu_draw_location_number(void)
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
    map_layer = player_state.map_layer_index;
    grid_z = camera_z >> 11;
    prefix = state_8017d118.current_map_region_id * 100000
           + map_layer * 10000
           + (camera_x >> 11) * 100;
    value = prefix + grid_z;
    menu_format_number(value, 6, 1, 0, row.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &row);
}
