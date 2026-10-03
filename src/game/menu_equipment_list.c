#include <kf/lib/address.h>
#include <kf/game/event_counter.h>
#include <kf/game/menu.h>
#include <kf/game/player.h>
#include <psyq/libc.h>

RODATA(0x800110b8, 0x4c)

typedef struct KfMenuEquipmentList {
    KfMenuList list;
    KfMenuLabelSuffix *initial_rows;
    KfMenuLabelSuffix *current_rows;
    u8 unknown_2c[8];
} KfMenuEquipmentList;

typedef char kf_menu_equipment_list_size[sizeof(KfMenuEquipmentList) == 52 ? 1 : -1];

ADDRESS(0x80019ac4, 0x220)
void menu_equipment_list_controller(void)
{
    KfMenuEquipmentList menu;
    KfMenuLabelSuffix initial_rows[10];
    KfMenuLabelSuffix current_rows[10];
    s32 mode = 0;
    s32 result = -99;
    s32 frame;
    u32 choice;

    memcpy(initial_rows, menu_equipment_labels_64910, sizeof initial_rows);
    menu_build_equipped_label_rows(current_rows);
    menu_list_init(&menu.list, 0, 2);
    menu.list.entry_count = 10;
    menu.list.visible_rows = 10;
    menu.initial_rows = initial_rows;
    menu.current_rows = current_rows;
    menu.list.list_y = 39;

    for (;;) {
        if (mode != 0 || result != -99)
            input_wait_release();

        if (mode == 1) {
            choice = menu.list.selected_index;
            if (choice == 1)
                menu_choose_primary_magic_shortcut();
            else if (choice == 9)
                menu_item_magic_controller();
            else
                menu_equipment_category_controller(choice);
            menu_build_equipped_label_rows(current_rows);
        }

        if (result != -99)
            break;

        menu_update_list_input(&menu.list, 0, &mode, &result);
        if (mode == 1)
            menu_play_sound_cue(17);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            menu_render_list(&menu, 3);
            menu_present_frame();
        }
    }
}


ADDRESS(0x80019ce4, 0x1f0)
void menu_build_equipped_label_rows(KfMenuLabelSuffix *rows)
{
    u8 selected[10];
    u8 *entry;
    s32 i;

    selected[0] = player_state.equipped_weapon_id;
    selected[1] = player_state.primary_magic_shortcut_id;
    selected[2] = player_state.equipped_arm_id;
    selected[3] = player_state.equipped_head_id;
    selected[4] = player_state.equipped_body_id;
    selected[5] = player_state.equipped_leg_id;
    selected[6] = player_state.equipped_shield_id;
    selected[7] = player_state.equipped_accessory_id;
    selected[8] = player_state.equipped_extra_id;
    if (player_state.secondary_magic_shortcut_id == 0xff)
        selected[9] = player_state.secondary_item_shortcut_id;
    else
        selected[9] = player_state.secondary_magic_shortcut_id;

    entry = selected;
    for (i = 0; i < 10; rows++, i++, entry++) {
        u32 id = *entry;

        if (id == 0xff)
            goto missing;
        if (i == 1)
            goto extra;
        if (i != 9)
            goto base;
        if (player_state.secondary_magic_shortcut_id == 0xff)
            goto base;
    extra:
        *rows = *(const KfMenuLabelSuffix *)menu_glyph_rows_extra[id].codes;
        goto next;
    base:
        *rows = *(const KfMenuLabelSuffix *)menu_glyph_rows[id].codes;
        goto next;
    missing:
        rows->codes[0] = -1;
    next:;
    }
}

ADDRESS(0x80019ed4, 0x420)
void menu_equipment_category_controller(s32 category)
{
    KfItemMenuList menu;
    KfMenuGlyphRow rows[28];
    u8 values[32];
    u8 item_ids[32];
    s32 selection = 0;
    s32 result = -99;
    s32 first;
    s32 last;
    s32 count;
    s32 frame;
    u8 selected_item;
    u8 equipped_id;

    switch (category) {
    case 0:
        first = 0;
        last = 20;
        break;
    case 2:
        first = 34;
        last = 40;
        break;
    case 3:
        first = 21;
        last = 27;
        break;
    case 4:
        first = 28;
        last = 33;
        break;
    case 5:
        first = 41;
        last = 46;
        break;
    case 6:
        first = 47;
        last = 52;
        break;
    case 7:
    case 8:
        equipped_id = category == 7 ? player_state.equipped_extra_id
                                    : player_state.equipped_accessory_id;
        first = 53;
        if (equipped_id != 0xff)
            game_counter_bytes[equipped_id]--;
        last = 59;
        break;
    }

    count = menu_collect_masked_item_rows(game_counter_bytes, rows, values, item_ids,
        first, last);
    memcpy(rows[count].codes, menu_row_prefix_649ec, sizeof menu_row_prefix_649ec);
    values[count] = 0xff;
    item_ids[count] = 0xff;
    count++;

    menu_list_init(&menu.list, 0, 2);
    menu.list.entry_count = count;
    menu.rows = rows;
    menu.values = values;
    menu.list.glyphs_per_entry = 12;

    if (menu.list.entry_count != 0
        && menu_load_item_model(item_ids[menu.list.selected_index]) != 0)
        return;

    for (;;) {
        if (selection != 0 || result != -99)
            input_wait_release();

        if (selection == 1) {
            result = menu_preview_choice(&menu, 5, 5, selected_item);
            if (result == -1)
                result = -99;
            else
                result = selected_item;
        }

        if (result != -99)
            break;

        menu_update_list_input(&menu.list, item_ids, &selection, &result);
        selected_item = item_ids[menu.list.selected_index];
        if (selection == 1)
            menu_play_sound_cue(17);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            if (menu.list.entry_count != 0)
                menu_update_item_preview(selected_item);
            menu_render_list(&menu, 5);
            menu_present_frame();
        }
    }

    menu_release_item_model();
    if (result != -1) {
        switch (category) {
        case 0:
            player_equip_weapon((u8)result);
            break;
        case 2:
            player_set_equipment_slot((u8)result, 4);
            break;
        case 3:
            player_set_equipment_slot((u8)result, 0);
            break;
        case 4:
            player_set_equipment_slot((u8)result, 1);
            break;
        case 5:
            player_set_equipment_slot((u8)result, 2);
            break;
        case 6:
            player_set_equipment_slot((u8)result, 3);
            break;
        case 7:
            player_set_equipment_slot((u8)result, 5);
            break;
        case 8:
            player_set_equipment_slot((u8)result, 6);
            break;
        }
    }

    if ((u32)(category - 7) < 2 && equipped_id != 0xff)
        game_counter_bytes[equipped_id]++;
}

DATA(0x80064910, 0xc8)
KfMenuLabelSuffix menu_equipment_labels_64910[10] = {
    {{118, 119, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{120, 121, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{122, 123, 57, 124, -1, 0, 0, 0, 0, 0}},
    {{122, 123, 57, 125, -1, 0, 0, 0, 0, 0}},
    {{122, 123, 57, 126, -1, 0, 0, 0, 0, 0}},
    {{122, 123, 57, 127, -1, 0, 0, 0, 0, 0}},
    {{122, 123, 57, 128, -1, 0, 0, 0, 0, 0}},
    {{0, 1, 18, 32, 230, -1, 0, 0, 0, 0}},
    {{0, 1, 18, 32, 231, -1, 0, 0, 0, 0}},
    {{58, 4125, 15, 39, -1, 0, 0, 0, 0, 0}},
};
