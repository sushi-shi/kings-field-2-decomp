#include <kf/lib/address.h>
#include <kf/game/card.h>
#include <kf/game/effect.h>
#include <kf/game/event_counter.h>
#include <kf/game/menu.h>
#include <kf/game/player.h>
#include <psyq/libc.h>

RODATA(0x800110b8, 0x4c)

enum {
    MENU_CATEGORY_WEAPON = 0,
    MENU_CATEGORY_PRIMARY_MAGIC = 1,
    MENU_CATEGORY_ARM = 2,
    MENU_CATEGORY_HEAD = 3,
    MENU_CATEGORY_BODY = 4,
    MENU_CATEGORY_LEG = 5,
    MENU_CATEGORY_SHIELD = 6,
    MENU_CATEGORY_ACCESSORY = 7,
    MENU_CATEGORY_EXTRA = 8,
    MENU_CATEGORY_SECONDARY_SHORTCUT = 9,
    MENU_CATEGORY_COUNT = 10
};

ADDRESS(0x80019ac4, 0x220)
void menu_equipment_list_controller(void)
{
    KfMenuRenderList menu;
    KfMenuLabelSuffix initial_rows[MENU_CATEGORY_COUNT];
    KfMenuLabelSuffix current_rows[MENU_CATEGORY_COUNT];
    s32 mode = 0;
    s32 result = KF_MENU_RESULT_PENDING;
    s32 frame;
    u32 choice;

    memcpy(initial_rows, menu_equipment_labels_64910, sizeof initial_rows);
    menu_build_equipped_label_rows(current_rows);
    menu_list_init(&menu.list, 0, 2);
    menu.list.entry_count = MENU_CATEGORY_COUNT;
    menu.list.visible_rows = MENU_CATEGORY_COUNT;
    menu.row_glyphs = initial_rows[0].codes;
    menu.detail_rows = current_rows;
    menu.list.list_y = 39;

    for (;;) {
        if (mode != 0 || result != KF_MENU_RESULT_PENDING)
            input_wait_release();

        if (mode == 1) {
            choice = menu.list.selected_index;
            if (choice == MENU_CATEGORY_PRIMARY_MAGIC)
                menu_choose_primary_magic_shortcut();
            else if (choice == MENU_CATEGORY_SECONDARY_SHORTCUT)
                menu_item_magic_controller();
            else
                menu_equipment_category_controller(choice);
            menu_build_equipped_label_rows(current_rows);
        }

        if (result != KF_MENU_RESULT_PENDING)
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
    u8 selected[MENU_CATEGORY_COUNT];
    u8 *entry;
    s32 i;

    selected[MENU_CATEGORY_WEAPON] = player_state.equipped_weapon_id;
    selected[MENU_CATEGORY_PRIMARY_MAGIC] = player_state.primary_magic_shortcut_id;
    selected[MENU_CATEGORY_ARM] = player_state.equipped_arm_id;
    selected[MENU_CATEGORY_HEAD] = player_state.equipped_head_id;
    selected[MENU_CATEGORY_BODY] = player_state.equipped_body_id;
    selected[MENU_CATEGORY_LEG] = player_state.equipped_leg_id;
    selected[MENU_CATEGORY_SHIELD] = player_state.equipped_shield_id;
    selected[MENU_CATEGORY_ACCESSORY] = player_state.equipped_accessory_id;
    selected[MENU_CATEGORY_EXTRA] = player_state.equipped_extra_id;
    if (player_state.secondary_magic_shortcut_id == KF_EQUIPMENT_NONE)
        selected[MENU_CATEGORY_SECONDARY_SHORTCUT] = player_state.secondary_item_shortcut_id;
    else
        selected[MENU_CATEGORY_SECONDARY_SHORTCUT] = player_state.secondary_magic_shortcut_id;

    entry = selected;
    for (i = 0; i < MENU_CATEGORY_COUNT; rows++, i++, entry++) {
        u32 id = *entry;

        if (id == KF_EQUIPMENT_NONE)
            goto missing;
        if (i == MENU_CATEGORY_PRIMARY_MAGIC)
            goto extra;
        if (i != MENU_CATEGORY_SECONDARY_SHORTCUT)
            goto base;
        if (player_state.secondary_magic_shortcut_id == KF_EQUIPMENT_NONE)
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
    s32 result = KF_MENU_RESULT_PENDING;
    s32 first;
    s32 last;
    s32 count;
    s32 frame;
    u8 selected_item;
    u8 other_slot_item_id;

    switch (category) {
    case MENU_CATEGORY_WEAPON:
        first = 0;
        last = 20;
        break;
    case MENU_CATEGORY_ARM:
        first = 34;
        last = 40;
        break;
    case MENU_CATEGORY_HEAD:
        first = 21;
        last = 27;
        break;
    case MENU_CATEGORY_BODY:
        first = 28;
        last = 33;
        break;
    case MENU_CATEGORY_LEG:
        first = 41;
        last = 46;
        break;
    case MENU_CATEGORY_SHIELD:
        first = 47;
        last = 52;
        break;
    case MENU_CATEGORY_ACCESSORY:
    case MENU_CATEGORY_EXTRA:
        other_slot_item_id = category == MENU_CATEGORY_ACCESSORY ? player_state.equipped_extra_id
                                        : player_state.equipped_accessory_id;
        first = 53;
        if (other_slot_item_id != KF_EQUIPMENT_NONE)
            game_counter_bytes[other_slot_item_id]--;
        last = 59;
        break;
    }

    count = menu_collect_masked_item_rows(game_counter_bytes, rows, values, item_ids,
        first, last);
    memcpy(rows[count].codes, menu_none_option_glyphs, sizeof menu_none_option_glyphs);
    values[count] = 0xff;
    item_ids[count] = 0xff;
    count++;

    menu_list_init(&menu.list, 0, 2);
    menu.list.entry_count = count;
    menu.rows = rows;
    menu.values = values;
    menu.list.glyphs_per_entry = KF_MENU_GLYPHS_PER_ROW;

    if (menu.list.entry_count != 0
        && menu_load_item_model(item_ids[menu.list.selected_index]) != 0)
        return;

    for (;;) {
        if (selection != 0 || result != KF_MENU_RESULT_PENDING)
            input_wait_release();

        if (selection == 1) {
            result = menu_preview_choice(&menu, 5, 5, selected_item);
            if (result == KF_MENU_RESULT_CANCELLED)
                result = KF_MENU_RESULT_PENDING;
            else
                result = selected_item;
        }

        if (result != KF_MENU_RESULT_PENDING)
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
    if (result != KF_MENU_RESULT_CANCELLED) {
        switch (category) {
        case MENU_CATEGORY_WEAPON:
            player_equip_weapon((u8)result);
            break;
        case MENU_CATEGORY_ARM:
            player_set_equipment_slot((u8)result, KF_EQUIPMENT_SLOT_ARM);
            break;
        case MENU_CATEGORY_HEAD:
            player_set_equipment_slot((u8)result, KF_EQUIPMENT_SLOT_HEAD);
            break;
        case MENU_CATEGORY_BODY:
            player_set_equipment_slot((u8)result, KF_EQUIPMENT_SLOT_BODY);
            break;
        case MENU_CATEGORY_LEG:
            player_set_equipment_slot((u8)result, KF_EQUIPMENT_SLOT_LEG);
            break;
        case MENU_CATEGORY_SHIELD:
            player_set_equipment_slot((u8)result, KF_EQUIPMENT_SLOT_SHIELD);
            break;
        case MENU_CATEGORY_ACCESSORY:
            player_set_equipment_slot((u8)result, KF_EQUIPMENT_SLOT_ACCESSORY);
            break;
        case MENU_CATEGORY_EXTRA:
            player_set_equipment_slot((u8)result, KF_EQUIPMENT_SLOT_EXTRA);
            break;
        }
    }

    if ((u32)(category - MENU_CATEGORY_ACCESSORY) < 2 &&
        other_slot_item_id != KF_EQUIPMENT_NONE)
        game_counter_bytes[other_slot_item_id]++;
}

ADDRESS(0x8001a2f4, 0x1fc)
void menu_choose_primary_magic_shortcut(void)
{
    KfMagicMenuList menu;
    KfMenuGlyphRow rows[20];
    s32 values[20];
    u8 indices[20];
    s32 mode = 0;
    s32 result = KF_MENU_RESULT_PENDING;
    s32 count;
    s32 frame;

    count = menu_collect_available_magic_rows(effect_state.magic_records, rows, values, indices, 0, 13);
    memcpy(rows[count].codes, menu_none_option_glyphs, sizeof menu_none_option_glyphs);
    values[count] = -1;
    indices[count] = 0xff;
    count++;

    menu_list_init(&menu.list, 0, 2);
    menu.list.entry_count = count;
    menu.list.visible_rows = 6;
    menu.rows = rows;
    menu.values = values;
    menu.list.list_y = 0x83;
    menu.list.glyphs_per_entry = KF_MENU_GLYPHS_PER_ROW;

    for (;;) {
        if (mode != 0 || result != KF_MENU_RESULT_PENDING)
            input_wait_release();

        if (mode == 1) {
            result = menu_preview_choice(&menu, 5, 4, 0xff);
            if (result == KF_MENU_RESULT_CANCELLED)
                result = KF_MENU_RESULT_PENDING;
            else
                result = indices[menu.list.selected_index];
        }

        if (result != KF_MENU_RESULT_PENDING)
            break;

        menu_update_list_input(&menu.list, 0, &mode, &result);
        if (mode == 1)
            menu_play_sound_cue(17);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            menu_render_list(&menu, 4);
            menu_present_frame();
        }
    }

    if (result != KF_MENU_RESULT_CANCELLED)
        player_set_primary_magic_shortcut_id(result);
}

ADDRESS(0x8001a4f0, 0x30c)
void menu_item_magic_controller(void)
{
    KfMenuRenderList menu;
    KfMenuGlyphRow rows[74];
    u8 counts[74];
    s32 numbers[74];
    u8 item_ids[74];
    u8 magic_ids[74];
    s32 selection = 0;
    s32 result = KF_MENU_RESULT_PENDING;
    s32 count;
    s32 index;
    s32 frame;
    u8 selected_item;
    u8 saved_count_60;
    u8 saved_count_61;

    for (index = 0; index < 74; index++) {
        counts[index] = 0xff;
        numbers[index] = -1;
        item_ids[index] = 0xff;
        magic_ids[index] = 0xff;
    }

    saved_count_60 = game_counter_bytes[0x60];
    saved_count_61 = game_counter_bytes[0x61];
    game_counter_bytes[0x60] = 0;
    game_counter_bytes[0x61] = 0;
    count = menu_collect_masked_item_rows(game_counter_bytes, rows, counts, item_ids, 70, 116);
    game_counter_bytes[0x60] = saved_count_60;
    game_counter_bytes[0x61] = saved_count_61;

    count += menu_collect_available_magic_rows(effect_state.magic_records, &rows[count],
        &numbers[count], &magic_ids[count], 0, 19);
    memcpy(rows[count].codes, menu_none_option_glyphs, sizeof menu_none_option_glyphs);
    count++;

    menu_list_init(&menu.list, 0, 2);
    menu.list.entry_count = count;
    menu.row_glyphs = rows[0].codes;
    menu.byte_values = counts;
    menu.number_values = numbers;
    menu.list.glyphs_per_entry = KF_MENU_GLYPHS_PER_ROW;

    if (menu.list.entry_count != 0
        && menu_load_item_model(item_ids[menu.list.selected_index]) != 0)
        return;

    for (;;) {
        if (selection != 0 || result != KF_MENU_RESULT_PENDING)
            input_wait_release();

        if (selection == 1) {
            result = menu_preview_choice(&menu, 5, 16, selected_item);
            if (result == KF_MENU_RESULT_CANCELLED)
                result = KF_MENU_RESULT_PENDING;
            else
                result = menu.list.selected_index;
        }

        if (result != KF_MENU_RESULT_PENDING)
            break;

        menu_update_list_input(&menu.list, item_ids, &selection, &result);
        selected_item = item_ids[menu.list.selected_index];
        if (selection == 1)
            menu_play_sound_cue(17);

        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            if (menu.list.entry_count != 0)
                menu_update_item_preview(selected_item);
            menu_render_list(&menu, 16);
            menu_present_frame();
        }
    }

    if (result != KF_MENU_RESULT_CANCELLED) {
        player_set_secondary_magic_shortcut_id(magic_ids[result]);
        player_set_secondary_item_shortcut_id(item_ids[result]);
    }
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
DATA(0x800649ec, 0x8)
s16 menu_none_option_glyphs[4] = {89, 4172, 76, -1};

ADDRESS(0x8001a7fc, 0x9c)
void menu_show_combat_attributes(void)
{
    s32 current = KF_MENU_RESULT_PENDING;
    s32 previous = KF_MENU_RESULT_PENDING;
    s32 frame;

    for (;;) {
        if (current != previous) {
            input_wait_release();
            return;
        }
        input_wait_brief_release();
        if (input_read_mark_active()) {
            menu_play_sound_cue(18);
            current = KF_MENU_RESULT_CANCELLED;
        }
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            menu_draw_combat_attributes();
            menu_present_frame();
        }
    }
}

ADDRESS(0x8001a898, 0x204)
void menu_item_use_controller(void)
{
    KfItemMenuList menu;
    KfMenuGlyphRow rows[120];
    u8 values[120];
    u8 indices[120];
    s32 mode = 0;
    s32 result = KF_MENU_RESULT_PENDING;
    s32 count;
    s32 frame;
    u8 selected_item;

    count = menu_collect_available_item_rows(game_counter_bytes, rows, values, indices, 0, 119);
    menu_list_init(&menu.list, 0, 4);
    menu.list.entry_count = count;
    menu.rows = rows;
    menu.values = values;
    menu.list.glyphs_per_entry = KF_MENU_GLYPHS_PER_ROW;
    if (menu.list.entry_count != 0
        && menu_load_item_model(indices[menu.list.selected_index]) != 0)
        return;

    for (;;) {
        if (mode != 0 || result != KF_MENU_RESULT_PENDING)
            input_wait_release();

        if (mode == 1) {
            result = menu_preview_choice(&menu, 1, 7,
                indices[menu.list.selected_index]);
            if (result == KF_MENU_RESULT_CANCELLED)
                result = KF_MENU_RESULT_PENDING;
            else
                result = selected_item;
        }

        if (result != KF_MENU_RESULT_PENDING)
            break;

        menu_update_list_input(&menu.list, indices, &mode, &result);
        selected_item = indices[menu.list.selected_index];
        if (mode == 1)
            menu_play_sound_cue(17);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            if (menu.list.entry_count != 0)
                menu_update_item_preview(selected_item);
            menu_render_list(&menu, 7);
            menu_present_frame();
        }
    }

    menu_release_item_model();
    if (result != KF_MENU_RESULT_CANCELLED)
        game_counter_bytes[result]--;
}
