#include <kf/game/callback.h>
#include <kf/game/card.h>
#include <kf/game/menu.h>
#include <kf/game/player.h>
#include <kf/lib/address.h>
#include <psyq/pad.h>
#include <kf/game/event_counter.h>
#include <kf/game/cd.h>
#include <kf/game/graphics.h>
#include <kf/game/memory.h>
#include <kf/game/map_cell.h>
#include <kf/game/resources.h>
#include <psyq/sdk.h>
#include <psyq/libc.h>
#include <kf/game/effect.h>
#include <kf/lib/math.h>

RODATA(0x80011098, 0x6c)

enum {
    MENU_STATUS_RELIEF_CAP = 64,
    MENU_LOCATION_REGION_PLACE = 100000,
    MENU_LOCATION_LAYER_PLACE = 10000,
    MENU_LOCATION_CELL_X_PLACE = 100,
    MENU_MAP_ITEM_FIRST = 67,
    MENU_ITEM_LAST_GLYPH_INDEX = 119,
    MENU_MAGIC_ACTION_FIRST = 14,
    MENU_MAGIC_ACTION_LAST = 19,
    MENU_MAGIC_ACTION_VISIBLE_ROWS = MENU_MAGIC_ACTION_LAST - MENU_MAGIC_ACTION_FIRST + 1,
    MENU_MAP_ARCHIVE_FIRST_ENTRY = 480,
    MENU_MAP_ARCHIVE_ENTRIES_PER_ITEM = 8,
    MENU_MAP_FACING_TILE_COUNT = 8,
    MENU_MAP_FACING_TILE_SHIFT = 9,
    MENU_MAP_FACING_TILE_HALF_ANGLE = 1 << (MENU_MAP_FACING_TILE_SHIFT - 1),
    MENU_MAP_FACING_TILE_U_STRIDE = 16,
    MENU_EFFECT_RELIEVE_AILMENTS = 71,
    MENU_EFFECT_RESTORE_MP_40 = 72,
    MENU_EFFECT_RAISE_BASE_MAGIC = 73,
    MENU_EFFECT_RESTORE_HP_40 = 74,
    MENU_EFFECT_CURE_POISON_RESTORE_HP_15 = 75,
    MENU_EFFECT_FULL_RESTORE = 76,
    MENU_EFFECT_RESTORE_HP_100 = 77,
    MENU_EFFECT_RESTORE_MP_50 = 78,
    MENU_EFFECT_CLEAR_AILMENTS = 79,
    MENU_EFFECT_MIXED_RESTORE = 80,
    MENU_EFFECT_ITEM_COUNT = MENU_EFFECT_MIXED_RESTORE - MENU_EFFECT_RELIEVE_AILMENTS + 1
};

ADDRESS(0x8001876c, 0x284)
s32 menu_run_root_controller(void)
{
    s32 cursor = 0;
    s32 confirmed = 0;
    s32 result = KF_MENU_RESULT_PENDING;
    s32 selection = KF_MENU_SELECTION_NONE;
    s32 choice_result;
    s32 frame;
    u32 buttons;

    menu_enter_display_state(1);
    for (frame = 0; frame < 2; frame++) {
        menu_frame_begin();
        menu_draw_player_status();
        menu_draw_window(0, KF_MENU_ROOT_WINDOW_ROWS, cursor, confirmed);
        menu_present_frame();
    }
    menu_play_sound_cue(16);
    input_wait_release();

    for (;;) {
        if (selection != KF_MENU_SELECTION_NONE || result != KF_MENU_RESULT_PENDING)
            input_wait_release();

        switch (selection) {
        case KF_MENU_ROOT_ITEM_SELECTION:
            choice_result = menu_item_selection_controller();
            goto selection_result;
        case KF_MENU_ROOT_MAGIC_ACTION:
            choice_result = menu_choose_magic_action();
            goto selection_result;
        case KF_MENU_ROOT_EQUIPMENT:
            menu_equipment_list_controller();
            break;
        case KF_MENU_ROOT_COMBAT_ATTRIBUTES:
            menu_show_combat_attributes();
            break;
        case KF_MENU_ROOT_ITEM_USE:
            menu_item_use_controller();
            break;
        case KF_MENU_ROOT_MEMORY_CARD:
            choice_result = menu_run_card_choice();
selection_result:
            result = choice_result;
            if (choice_result == KF_MENU_RESULT_CANCELLED)
                result = KF_MENU_RESULT_PENDING;
            break;
        case KF_MENU_ROOT_OPTIONS:
            menu_options_controller();
            break;
        }

        if (result != KF_MENU_RESULT_PENDING)
            break;

        cursor = menu_poll_choice_input(cursor, KF_MENU_ROOT_CANCEL_ROW,
            &selection, &confirmed, &result);
        buttons = PadRead(1);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            if ((buttons & PADR1) != 0 && (buttons & PADL1) != 0)
                menu_draw_location_number();
            menu_draw_player_status();
            menu_draw_window(0, KF_MENU_ROOT_WINDOW_ROWS, cursor, confirmed);
            menu_present_frame();
        }
    }

    if (result == KF_MENU_RESULT_CANCELLED)
        menu_play_sound_cue(0);
    if (result == KF_MENU_RESULT_GAME_LOADED)
        menu_exit_display_state(1);
    else
        menu_exit_display_state(0);
    if (result != KF_MENU_RESULT_CANCELLED &&
        result != KF_MENU_RESULT_GAME_LOADED &&
        (result & KF_MENU_MAGIC_ACTION_TAG) != 0) {
        player_select_magic_action(result & KF_MENU_MAGIC_ACTION_ID_MASK);
        result = KF_MENU_RESULT_CANCELLED;
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
    grid_z = camera_z >> KF_MAP_CELL_POSITION_SHIFT;
    prefix = state_8017d118.current_map_region_id * MENU_LOCATION_REGION_PLACE
           + map_layer * MENU_LOCATION_LAYER_PLACE
           + (camera_x >> KF_MAP_CELL_POSITION_SHIFT) * MENU_LOCATION_CELL_X_PLACE;
    value = prefix + grid_z;
    menu_format_number(value, 6, 1, 0, row.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &row);
}

ADDRESS(0x80018ac8, 0x240)
s32 menu_item_selection_controller(void)
{
    KfItemMenuList menu;
    KfMenuGlyphRow rows[54];
    u8 values[56];
    u8 indices[56];
    s32 mode = 0;
    s32 result = KF_MENU_RESULT_PENDING;
    s32 count;
    s32 frame;
    u8 selected_item;

    count = menu_collect_masked_item_rows(game_counter_bytes, rows, values, indices,
                                          MENU_MAP_ITEM_FIRST, MENU_ITEM_LAST_GLYPH_INDEX);
    menu_list_init(&menu.list, 0, 0);
    menu.list.entry_count = count;
    menu.rows = rows;
    menu.values = values;
    menu.list.glyphs_per_entry = KF_MENU_GLYPHS_PER_ROW;
    if (menu.list.entry_count != 0
        && menu_load_item_model(indices[menu.list.selected_index]) != 0)
        return KF_MENU_RESULT_CANCELLED;

    for (;;) {
        if (mode != 0 || result != KF_MENU_RESULT_PENDING)
            input_wait_release();

        if (mode == 1) {
            result = menu_preview_choice(&menu, 0, 1, selected_item);
            if (result == KF_MENU_RESULT_CANCELLED)
                result = KF_MENU_RESULT_PENDING;
            else
                result = selected_item;
        }

        if (result != KF_MENU_RESULT_PENDING)
            break;

        menu_update_list_input(&menu.list, indices, &mode, &result);
        selected_item = indices[menu.list.selected_index];
        if (mode == 1) {
            menu_play_sound_cue(17);
            if (selected_item == MENU_MAP_ITEM_FIRST
                || selected_item == MENU_MAP_ITEM_FIRST + 1
                || selected_item == MENU_MAP_ITEM_FIRST + 2) {
                menu_release_item_model();
                menu_show_map_preview(selected_item);
                menu_play_sound_cue(18);
                if (menu_load_item_model(selected_item) != 0)
                    return KF_MENU_RESULT_CANCELLED;
                mode = 0;
            }
        }
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            if (menu.list.entry_count != 0)
                menu_update_item_preview(selected_item);
            menu_render_list(&menu, 1);
            menu_present_frame();
        }
    }

    menu_release_item_model();
    if ((u32)(result - MENU_EFFECT_RELIEVE_AILMENTS) < MENU_EFFECT_ITEM_COUNT)
        menu_apply_item_effect(result);
    return result;
}

ADDRESS(0x80018d08, 0xe4)
s32 menu_collect_masked_item_rows(const u8 *mask, KfMenuGlyphRow *rows,
    u8 *values, u8 *indices, s32 first, s32 last)
{
    const KfMenuGlyphRow *glyph = &menu_glyph_rows[first];
    const u8 *entry = mask + first;
    s32 index;
    s32 count = 0;

    last++;
    index = first;
    while (index < last) {
        if (*entry != 0) {
            *rows++ = *glyph;
            *values = *entry;
            *indices = index;
            values++;
            indices++;
            count++;
        }
        entry++;
        glyph++;
        index++;
    }
    return count;
}

ADDRESS(0x80018dec, 0x1a0)
s32 menu_collect_available_item_rows(const u8 *mask, KfMenuGlyphRow *rows,
    u8 *values, u8 *indices, s32 first, s32 last)
{
    const KfMenuGlyphRow *glyph = &menu_glyph_rows[first];
    const u8 *entry = mask + first;
    s32 index;
    s32 count = 0;

    last++;
    index = first;
    while (index < last) {
        if (*entry != 0) {
            *values = *entry;
            if (index == player_state.equipped_weapon_id
                || index == player_state.equipped_head_id
                || index == player_state.equipped_body_id
                || index == player_state.equipped_arm_id
                || index == player_state.equipped_leg_id
                || index == player_state.equipped_shield_id
                || index == player_state.equipped_accessory_id
                || index == player_state.equipped_extra_id
                || index == player_state.secondary_item_shortcut_id)
                --*values;
            if (*values != 0) {
                *rows = *glyph;
                *indices = index;
                rows++;
                values++;
                indices++;
                count++;
            }
        }
        entry++;
        glyph++;
        index++;
    }
    return count;
}

ADDRESS(0x80018f8c, 0x2b4)
void menu_apply_item_effect(s32 item_id)
{
    u16 current_hp;

    if (game_counter_bytes[item_id] == 0)
        return;

    if (item_id == MENU_EFFECT_RELIEVE_AILMENTS) {
        if (player_state.paralysis_timer > 0)
            player_state.paralysis_timer = 0;
        if (player_state.slow_timer >= MENU_STATUS_RELIEF_CAP + 1)
            player_state.slow_timer = MENU_STATUS_RELIEF_CAP;
        player_cap_curse_strength();
        player_cap_darkness_phase();
    } else if (item_id == MENU_EFFECT_RESTORE_MP_40) {
        player_state.vitals.current_mp += 40;
    } else if (item_id == MENU_EFFECT_RAISE_BASE_MAGIC) {
        player_state.base_magic++;
        player_recalculate_combat_stats();
    } else if (item_id == MENU_EFFECT_RESTORE_HP_40) {
        player_state.vitals.current_hp += 40;
    } else if (item_id == MENU_EFFECT_CURE_POISON_RESTORE_HP_15) {
        current_hp = player_state.vitals.current_hp;
        player_state.poison_timer = 0;
        player_state.vitals.current_hp = current_hp + 15;
    } else if (item_id == MENU_EFFECT_FULL_RESTORE) {
        player_state.vitals.current_hp = player_state.vitals.maximum_hp;
        player_state.vitals.current_mp = player_state.vitals.maximum_mp;
        player_clear_and_cap_status_effects();
    } else if (item_id == MENU_EFFECT_RESTORE_HP_100) {
        player_state.vitals.current_hp += 100;
    } else if (item_id == MENU_EFFECT_RESTORE_MP_50) {
        player_state.vitals.current_mp += 50;
    }

    if (item_id == MENU_EFFECT_CLEAR_AILMENTS) {
        player_clear_and_cap_status_effects();
    } else if (item_id == MENU_EFFECT_MIXED_RESTORE) {
        player_state.vitals.current_hp += 100;
        player_state.vitals.current_mp += 50;
        player_clear_and_cap_status_effects();
    }

    if (player_state.vitals.current_hp > player_state.vitals.maximum_hp)
        player_state.vitals.current_hp = player_state.vitals.maximum_hp;
    if (player_state.vitals.current_mp > player_state.vitals.maximum_mp)
        player_state.vitals.current_mp = player_state.vitals.maximum_mp;

    game_counter_bytes[item_id]--;
    if ((u32)(item_id - MENU_EFFECT_RESTORE_HP_100) < 2 ||
        (u32)(item_id - MENU_EFFECT_CLEAR_AILMENTS) < 2)
        game_counter_bytes[0x52]++;
    menu_play_sound_cue(13);
}

ADDRESS(0x80019240, 0x6c)
void player_clear_and_cap_status_effects(void)
{
    if (player_state.paralysis_timer > 0)
        player_state.paralysis_timer = 0;
    if (player_state.slow_timer > MENU_STATUS_RELIEF_CAP)
        player_state.slow_timer = MENU_STATUS_RELIEF_CAP;
    player_state.poison_timer = 0;
    player_cap_curse_strength();
    player_cap_darkness_phase();
}

ADDRESS(0x800192ac, 0x30)
void player_cap_darkness_phase(void)
{
    if (player_state.darkness_phase > MENU_STATUS_RELIEF_CAP) {
        player_state.darkness_phase = MENU_STATUS_RELIEF_CAP;
        player_state.darkness_phase_limit = 0;
    }
}

ADDRESS(0x800192dc, 0x30)
void player_cap_curse_strength(void)
{
    if (player_state.curse_strength > MENU_STATUS_RELIEF_CAP) {
        player_state.curse_strength = MENU_STATUS_RELIEF_CAP;
        player_state.curse_phase_limit = 0;
    }
}

ADDRESS(0x8001930c, 0x528)
void menu_show_map_preview(s32 menu_code)
{
    u32 entry;
    u32 map_index;
    u32 map_offset;
    u8 *image;
    s32 frame;
    s32 facing_tile;
    s32 u0;

    map_index = (menu_code - MENU_MAP_ITEM_FIRST) & 0xff;
    map_offset = state_8017d118.current_map_region_id + MENU_MAP_ARCHIVE_FIRST_ENTRY;
    entry = map_index * MENU_MAP_ARCHIVE_ENTRIES_PER_ITEM + map_offset;
    image = memory_allocate(cd_archive_entry_extent(KF_RESOURCE_ARCHIVE_ITEM, entry, 0));
    cd_archive_read(KF_RESOURCE_ARCHIVE_ITEM, entry, (u_long *)image);
    tim_upload_images(image);

    for (frame = 0; frame < 2; frame++) {
        menu_frame_begin();

        primitive_buffer_begin_poly_ft4();
        setRGB0(current_poly_ft4, 0x7f, 0x7f, 0x7f);
        SetSemiTrans((void *)current_poly_ft4, 1);
        current_poly_ft4->tpage = 0x1f;
        current_poly_ft4->clut = 0x7fe4;
        setXYWH(current_poly_ft4, 0x3c, 0x14, 200, 200);
        setUVWH(current_poly_ft4, 0, 0, 200, 200);
        primitive_buffer_commit_poly_ft4(10);

        primitive_buffer_begin_poly_ft4();
        setRGB0(current_poly_ft4, 0x7f, 0x7f, 0x7f);
        SetSemiTrans((void *)current_poly_ft4, 1);
        current_poly_ft4->tpage = 0x1c;
        current_poly_ft4->clut = 0x7d25;
        setXYWH(current_poly_ft4,
            player_state.camera_position.vx / 819 + 52,
            212 - player_state.camera_position.vz / 819,
            15, 15);

        facing_tile = ((player_state.camera_rotation.angles[1] & KF_ANGLE_WRAP_MASK)
                       + MENU_MAP_FACING_TILE_HALF_ANGLE) >> MENU_MAP_FACING_TILE_SHIFT;
        if (facing_tile == MENU_MAP_FACING_TILE_COUNT)
            facing_tile = 0;
        u0 = facing_tile * MENU_MAP_FACING_TILE_U_STRIDE
             - MENU_MAP_FACING_TILE_COUNT * MENU_MAP_FACING_TILE_U_STRIDE;
        setUVWH(current_poly_ft4, u0, 0x90, 15, 15);
        primitive_buffer_commit_poly_ft4(9);

        menu_draw_nine_slice_panel(0x36, 0xe, 0xd4, 0xd4, 2, 2);
        menu_present_frame();
    }

    input_wait_release();
    while (PadRead(1) == 0) {}
    input_wait_release();
    memory_free(image);
}

ADDRESS(0x80019834, 0x19c)
s32 menu_choose_magic_action(void)
{
    KfMagicMenuList menu;
    KfMenuGlyphRow rows[20];
    s32 values[20];
    u8 indices[20];
    s32 mode = 0;
    s32 result = KF_MENU_RESULT_PENDING;
    s32 count;
    s32 frame;

    count = menu_collect_available_magic_rows(effect_state.magic_records, rows, values, indices,
                                              MENU_MAGIC_ACTION_FIRST, MENU_MAGIC_ACTION_LAST);
    menu_list_init(&menu.list, 0, 1);
    menu.list.entry_count = count;
    menu.list.visible_rows = MENU_MAGIC_ACTION_VISIBLE_ROWS;
    menu.rows = rows;
    menu.values = values;
    menu.list.list_y = 0x83;
    menu.list.glyphs_per_entry = KF_MENU_GLYPHS_PER_ROW;

    for (;;) {
        if (mode != 0 || result != KF_MENU_RESULT_PENDING)
            input_wait_release();

        if (mode == 1) {
            result = menu_preview_choice(&menu, 0, 2, 0xff);
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
            menu_render_list(&menu, 2);
            menu_present_frame();
        }
    }

    if (result != KF_MENU_RESULT_CANCELLED)
        result |= KF_MENU_MAGIC_ACTION_TAG;
    return result;
}

ADDRESS(0x800199d0, 0xf4)
s32 menu_collect_available_magic_rows(const KfMagicRecord *records,
    KfMenuGlyphRow *rows, s32 *values, u8 *indices, s32 first, s32 last)
{
    const KfMenuGlyphRow *glyph = &menu_glyph_rows_extra[first];
    const KfMagicRecord *entry = &records[first];
    s32 count = 0;
    s32 index;

    last++;
    index = first;
    while (index < last) {
        if (entry->menu_available == 1) {
            *rows++ = *glyph;
            *values = entry->mp_cost;
            *indices = index;
            values++;
            indices++;
            count++;
        }
        entry++;
        glyph++;
        index++;
    }
    return count;
}




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

    memcpy(initial_rows, menu_equipment_category_labels, sizeof initial_rows);
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

        for (index = 0; index < 2; index++) {
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
KfMenuLabelSuffix menu_equipment_category_labels[10] = {
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

DATA(0x80064c30, 0xb40)
KfMenuGlyphRow menu_glyph_rows[120] = {
    {{4111, 4101, 45, -1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{11, 56, 45, 19, 14, 45, 4115, -1, 0, 0, 0, 0}},
    {{20, 1, 19, 14, 45, 4115, -1, 0, 0, 0, 0, 0}},
    {{34, 45, 21, 39, 4103, 12, 15, 45, -1, 0, 0, 0}},
    {{4121, 19, 42, 25, 39, 30, 45, -1, 0, 0, 0, 0}},
    {{4121, 12, 15, 45, 4115, 14, 45, 4115, -1, 0, 0, 0}},
    {{7, 43, 13, 39, 19, 0, 53, 7, 12, -1, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{27, 43, 1, 32, 14, 45, 4115, -1, 0, 0, 0, 0}},
    {{160, 161, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{12, 8217, 1, 4111, 45, -1, 0, 0, 0, 0, 0, 0}},
    {{0, 1, 12, 4123, 43, 45, 4115, -1, 0, 0, 0, 0}},
    {{33, 43, 42, 57, 2, 42, 88, 129, -1, 0, 0, 0}},
    {{32, 45, 39, 40, 1, 19, 14, 45, 4115, -1, 0, 0}},
    {{4111, 45, 7, 57, 12, 43, 1, 35, 45, -1, 0, 0}},
    {{4125, 2, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 42, 4121, 43, 12, 19, -1, 0, 0, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 1, 0, 39, 30, 12, 7, -1, 0, 0, 0, 0}},
    {{20, 1, 19, 28, 42, 32, -1, 0, 0, 0, 0, 0}},
    {{4103, 43, 45, 19, 28, 42, 32, -1, 0, 0, 0, 0}},
    {{4123, 40, 53, 4115, 7, 40, 2, 39, -1, 0, 0, 0}},
    {{162, 163, 88, 150, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{33, 43, 42, 57, 2, 42, 88, 150, -1, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{4123, 43, 12, 19, 8219, 43, 45, 19, -1, 0, 0, 0}},
    {{20, 1, 19, 8219, 43, 45, 19, -1, 0, 0, 0, 0}},
    {{0, 1, 12, 0, 45, 30, 45, -1, 0, 0, 0, 0}},
    {{211, 212, 88, 157, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{33, 43, 42, 57, 2, 42, 88, 157, -1, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{43, 4106, 45, 11, 45, 42, 4115, -1, 0, 0, 0, 0}},
    {{40, 45, 4107, 11, 45, 42, 4115, -1, 0, 0, 0, 0}},
    {{32, 45, 39, 4101, 45, 4115, -1, 0, 0, 0, 0, 0}},
    {{7, 41, 12, 15, 42, 4101, 45, 4115, -1, 0, 0, 0}},
    {{12, 5, 42, 11, 45, 42, 4115, -1, 0, 0, 0, 0}},
    {{33, 43, 42, 57, 2, 42, 88, 124, -1, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 1, 0, 39, 4103, 44, 45, 4123, -1, 0, 0, 0}},
    {{12, 19, 45, 39, 25, 39, 4115, -1, 0, 0, 0, 0}},
    {{11, 42, 4121, 45, 0, 45, 32, -1, 0, 0, 0, 0}},
    {{4114, 45, 34, 39, 25, 39, 4115, -1, 0, 0, 0, 0}},
    {{42, 1, 20, 12, 4103, 44, 45, 4123, -1, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 1, 0, 39, 4123, 45, 17, -1, 0, 0, 0, 0}},
    {{43, 53, 4103, 4101, 45, 4111, 45, -1, 0, 0, 0, 0}},
    {{11, 42, 4121, 45, 4123, 45, 17, -1, 0, 0, 0, 0}},
    {{4114, 12, 2, 52, 45, 5, 45, -1, 0, 0, 0, 0}},
    {{42, 1, 20, 12, 4123, 45, 17, -1, 0, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{204, 88, 127, 156, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{11, 45, 12, 88, 177, -1, 0, 0, 0, 0, 0, 0}},
    {{33, 43, 42, 57, 2, 42, 88, 127, 156, -1, 0, 0}},
    {{168, 205, 88, 158, 156, -1, 0, 0, 0, 0, 0, 0}},
    {{246, 81, 247, 248, 249, 88, 250, 156, -1, 0, 0, 0}},
    {{251, 83, 252, 88, 120, 253, 72, -1, 0, 0, 0, 0}},
    {{189, 254, 88, 158, 156, -1, 0, 0, 0, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{165, 166, 88, 168, 169, -1, 0, 0, 0, 0, 0, 0}},
    {{141, 142, 159, 105, 88, 168, 169, -1, 0, 0, 0, 0}},
    {{181, 149, 88, 168, 169, -1, 0, 0, 0, 0, 0, 0}},
    {{6, 39, 5, -1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{4123, 40, 53, 4115, 12, 19, 45, 39, -1, 0, 0, 0}},
    {{32, 45, 39, 12, 19, 45, 39, -1, 0, 0, 0, 0}},
    {{4098, 48, 45, 4111, 1, 19, -1, 0, 0, 0, 0, 0}},
    {{143, 144, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{136, 145, 75, 144, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{148, 149, 144, 88, 151, -1, 0, 0, 0, 0, 0, 0}},
    {{146, 147, 88, 141, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{146, 147, 88, 141, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{146, 147, 88, 141, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{146, 147, 88, 141, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{136, 88, 141, -1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{141, 142, 88, 173, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{11, 45, 12, 88, 196, -1, 0, 0, 0, 0, 0, 0}},
    {{171, 88, 172, -1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{174, 175, 88, 176, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{11, 45, 12, 88, 191, -1, 0, 0, 0, 0, 0, 0}},
    {{199, 120, 88, 200, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{188, 88, 111, -1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{74, 74, 99, 70, 88, 167, -1, 0, 0, 0, 0, 0}},
    {{190, 88, 7, 41, 12, 15, 42, -1, 0, 0, 0, 0}},
    {{141, 88, 7, 41, 12, 15, 42, -1, 0, 0, 0, 0}},
    {{187, 88, 7, 41, 12, 15, 42, -1, 0, 0, 0, 0}},
    {{188, 88, 7, 41, 12, 15, 42, -1, 0, 0, 0, 0}},
    {{189, 88, 7, 41, 12, 15, 42, -1, 0, 0, 0, 0}},
    {{4111, 45, 7, 7, 41, 12, 15, 42, -1, 0, 0, 0}},
    {{141, 142, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{141, 142, 88, 69, 72, 104, -1, 0, 0, 0, 0, 0}},
    {{31, 11, 37, 2, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{3, 42, 27, 88, 164, -1, 0, 0, 0, 0, 0, 0}},
    {{165, 166, 88, 164, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{194, 195, 88, 164, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{178, 88, 164, -1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{182, 183, 88, 164, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{201, 149, 88, 164, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{148, 88, 186, -1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{120, 192, 193, 88, 164, -1, 0, 0, 0, 0, 0, 0}},
    {{210, 88, 164, -1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{209, 88, 164, -1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{184, 185, 88, 164, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{223, 110, 88, 127, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{152, 88, 4104, 45, 19, -1, 0, 0, 0, 0, 0, 0}},
    {{153, 88, 4104, 45, 19, -1, 0, 0, 0, 0, 0, 0}},
    {{154, 155, 88, 4104, 45, 19, -1, 0, 0, 0, 0, 0}},
    {{152, 88, 164, -1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{153, 88, 164, -1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{154, 155, 88, 164, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{141, 142, 88, 135, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{3, 42, 27, 88, 135, -1, 0, 0, 0, 0, 0, 0}},
    {{40, 12, 19, 4104, 45, 19, -1, 0, 0, 0, 0, 0}},
};

DATA(0x80065770, 0x1e0)
KfMenuGlyphRow menu_glyph_rows_extra[20] = {
    {{2, 52, 45, 15, 45, 27, 52, 45, 42, -1, 0, 0}},
    {{12, 19, 45, 39, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{0, 45, 12, 2, 51, 45, 4123, -1, 0, 0, 0, 0}},
    {{33, 18, 4, -1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{2, 49, 39, 4115, 5, 53, 15, 45, -1, 0, 0, 0}},
    {{0, 1, 12, 12, 19, 45, 32, -1, 0, 0, 0, 0}},
    {{27, 41, 45, 4108, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{27, 48, 1, 0, 45, 4125, 45, 42, -1, 0, 0, 0}},
    {{27, 48, 1, 0, 45, 2, 52, 45, 42, -1, 0, 0}},
    {{27, 48, 1, 0, 45, 12, 19, 45, 32, -1, 0, 0}},
    {{27, 43, 1, 32, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{40, 1, 19, 21, 39, 4103, 4125, 42, 19, -1, 0, 0}},
    {{27, 40, 53, 11, 55, -1, 0, 0, 0, 0, 0, 0}},
    {{11, 45, 12, -1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{4114, 49, 12, 8221, 1, 4110, 39, -1, 0, 0, 0, 0}},
    {{43, 4107, 12, 19, 27, 48, 1, 0, -1, 0, 0, 0}},
    {{0, 45, 12, 26, 45, 42, -1, 0, 0, 0, 0, 0}},
    {{31, 10, 1, 42, 11, 45, 42, 4115, -1, 0, 0, 0}},
    {{40, 1, 19, -1, 0, 0, 0, 0, 0, 0, 0, 0}},
    {{4123, 43, 12, -1, 0, 0, 0, 0, 0, 0, 0, 0}},
};

DATA(0x80065950, 0x2d0)
u8 menu_item_mask_pages[6][120] = {
    {
        0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
        0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    },
    {
        0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
        0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    },
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    },
    {
        0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0,
        0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    },
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
    },
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    },
};

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
