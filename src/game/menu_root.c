#include <kf/game/callback.h>
#include <kf/lib/null.h>
#include <kf/lib/bool.h>
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
#include <kf/lib/types.h>
#include <kf/game/audio.h>
#include <psyq/audio.h>
#include <psyq/cd.h>
#include <psyq/kernel.h>
#include <kf/game/tmd.h>
#include <kf/game/pool.h>

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

typedef char kf_card_directory_entry_size[sizeof(struct DIRENTRY) == 40 ? 1 : -1];

enum {
    CARD_CHOICE_EXIT = -2,
    CARD_WRITE_IO_FAILURE = 1,
    CARD_MENU_ROW_CAPACITY = KF_CARD_DIRECTORY_CAPACITY / KF_CARD_FILE_BLOCKS + 1,
    CARD_MENU_VISIBLE_ROWS = 6,
    CARD_MENU_LIST_Y = 0x83,
    CARD_PROBE_TEMPORARY_FILE_CREATE_FAILURE = 2,
    CARD_MENU_NO_PREVIEW_ITEM = 0xff,
    CARD_MENU_NO_LEVEL = 0xff,
    CARD_MENU_NEW_SLOT = 0xff
};


/* The templates copied here use four, five, or six 16-bit glyph codes. */
static inline void menu_copy_prefix8(s16 *destination, const s16 *source)
{
    memcpy(destination, source, 8);
}

static inline void menu_copy_prefix10(s16 *destination, const s16 *source)
{
    memcpy(destination, source, 10);
}

static inline void menu_copy_prefix12(s16 *destination, const s16 *source)
{
    memcpy(destination, source, 12);
}
/* Unreferenced return stub; original role and TU owner remain unresolved. */
ADDRESS(0x80018764, 0x8)
void func_80018764(void)
{
}

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
    image = memory_allocate(cd_archive_entry_extent(KF_RESOURCE_ARCHIVE_ITEM, entry, NULL));
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

        menu_update_list_input(&menu.list, NULL, &mode, &result);
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

        menu_update_list_input(&menu.list, NULL, &mode, &result);
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

        menu_update_list_input(&menu.list, NULL, &mode, &result);
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

ADDRESS(0x8001aa9c, 0x1e4)
s32 menu_run_card_choice(void)
{
    KfMenuGlyphString labels[2];
    s32 cursor = 0;
    s32 confirmed = 0;
    s32 result = KF_MENU_RESULT_PENDING;
    s32 selection = KF_MENU_SELECTION_NONE;
    s32 frame;
    s32 volume;

    for (;;) {
        if (selection != KF_MENU_SELECTION_NONE || result != KF_MENU_RESULT_PENDING)
            input_wait_release();

        switch (selection) {
        case 0:
            result = menu_card_load_browser();
            if (result == 0)
                result = KF_MENU_RESULT_GAME_LOADED;
            break;
        case 1:
            result = menu_prompt_two_option();
            if (result == 0)
                result = CARD_CHOICE_EXIT;
            break;
        }

        if (selection != KF_MENU_SELECTION_NONE && result == KF_MENU_RESULT_CANCELLED)
            result = KF_MENU_RESULT_PENDING;

        if (result != KF_MENU_RESULT_PENDING)
            break;

        cursor = menu_poll_choice_input(cursor, 2, &selection, &confirmed, &result);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            menu_draw_window(1, 3, cursor, confirmed);
            menu_present_frame();
        }
    }

    if (result == CARD_CHOICE_EXIT) {
        u32 cd_result;

        CdControl(CdlStop, NULL, (u8 *)&cd_result);
        volume = 60;
        menu_prepare_card_exit_rows(labels);
        for (;;) {
            menu_show_dialog_panel(5, labels, 2, 70, 87, 178, 66, 2, 0);
            if (volume > 0) {
                volume--;
                SsSeqSetVol(audio_state.sequence_id, volume, volume);
            }
        }
    }
    return result;
}

ADDRESS(0x8001ac80, 0x2b0)
s32 menu_card_load_browser(void)
{
    struct DIRENTRY entries[KF_CARD_DIRECTORY_CAPACITY];
    KfCardMenuList menu;
    KfCardSlotGlyphRow glyph_rows[CARD_MENU_ROW_CAPACITY];
    s32 experience_values[CARD_MENU_ROW_CAPACITY];
    u8 levels[CARD_MENU_ROW_CAPACITY];
    s32 slot_ids[CARD_MENU_ROW_CAPACITY];
    KfMenuGlyphString dialog_rows[3];
    s32 matching_count;
    s32 mode = 0;
    s32 result = KF_MENU_RESULT_PENDING;
    s32 count;
    s32 read_result;
    s32 frame;

    menu_prepare_card_browser_rows(dialog_rows);
    menu_show_dialog_panel(4, dialog_rows, 2, 70, 87, 178, 66, 2, 0);
    memory_card_start();
    memory_card_scan_save_entries(entries, &matching_count);
    count = menu_card_build_slot_rows(entries, glyph_rows[0].codes,
        experience_values, levels, slot_ids);
    menu_list_init(&menu.list, 1, 0);
    menu.list.visible_rows = CARD_MENU_VISIBLE_ROWS;
    menu.list.list_y = CARD_MENU_LIST_Y;
    menu.list.entry_count = count;
    menu.rows = glyph_rows;
    menu.levels = levels;
    menu.experience_values = experience_values;

    for (;;) {
        if (mode != 0 || result != KF_MENU_RESULT_PENDING)
            input_wait_release();

        if (mode == 1) {
            result = menu_preview_choice(&menu, 6, 8, CARD_MENU_NO_PREVIEW_ITEM);
            if (result == KF_MENU_RESULT_CANCELLED)
                result = KF_MENU_RESULT_PENDING;
            else
                result = slot_ids[menu.list.selected_index];
        }
        if (result != KF_MENU_RESULT_PENDING)
            break;

        menu_update_list_input(&menu.list, NULL, &mode, &result);
        if (mode == 1)
            menu_play_sound_cue(16);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            menu_render_list(&menu, 8);
            menu_present_frame();
        }
    }

    if (result != KF_MENU_RESULT_CANCELLED) {
        menu_prepare_card_read_row(dialog_rows);
        menu_show_dialog_panel(4, dialog_rows, 1, 70, 87, 178, 66, 2, 0);
        read_result = memory_card_read_slot(result);
        if (read_result != 0) {
            menu_prepare_card_read_failure_rows(dialog_rows, read_result);
            menu_show_dialog_panel(4, dialog_rows, 3, 70, 87, 178, 81, 2, 0);
            input_wait_release();
            while (PadRead(1) == 0) {}
            input_wait_release();
            result = KF_MENU_RESULT_CANCELLED;
        } else {
            result = 0;
        }
    }
    memory_card_stop();
    return result;
}

ADDRESS(0x8001af30, 0x100)
s32 menu_card_build_slot_rows(const struct DIRENTRY *card_entries, s16 *glyph_rows,
    s32 *experience_values, u8 *levels, s32 *slot_ids)
{
    s32 count = 0;
    s32 index;
    s32 experience;
    s32 level;
    s32 slot_id;

    for (index = 0; index < KF_CARD_DIRECTORY_CAPACITY; index++) {
        if (memory_card_read_slot_summary(card_entries->name,
            &experience, &level, &slot_id) == KF_FALSE) {
            *glyph_rows++ = 0x1012;
            *glyph_rows++ = 0x2d;
            *glyph_rows++ = 0xf;
            *glyph_rows++ = slot_id + 229;
            *glyph_rows = -1;
            glyph_rows += 6;
            *experience_values++ = experience;
            *levels++ = level;
            *slot_ids++ = slot_id;
            count++;
        }
        ++card_entries;
    }
    return count;
}

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
    result = KF_MENU_RESULT_PENDING;
    selected = KF_MENU_SELECTION_NONE;
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
        if (selected != KF_MENU_SELECTION_NONE || result != KF_MENU_RESULT_PENDING)
            input_wait_release();

        switch (selected) {
        case 0:
            result = 0;
            break;
        case 1:
            result = KF_MENU_RESULT_CANCELLED;
            break;
        }

        if (result != KF_MENU_RESULT_PENDING)
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
    KF_MENU_OPTION_EFFECTS_ROW = 0,
    KF_MENU_OPTION_MUSIC_ROW = 1,
    KF_MENU_OPTION_GAUGES_ROW = 2,
    KF_MENU_OPTION_COMPASS_ROW = 3,
    KF_MENU_OPTION_ITEM_PREVIEW_ROW = 4,
    KF_MENU_OPTION_WALKING_BOB_ROW = 5,
    KF_MENU_OPTION_COUNT = 6,
    KF_MENU_OPTION_CANCEL_ROW = KF_MENU_OPTION_COUNT
};

ADDRESS(0x8001b2dc, 0x278)
void menu_options_controller(void)
{
    u8 selected[KF_MENU_OPTION_COUNT];
    KfMenuGlyphString labels[2];
    s32 choice = 0;
    s32 result = KF_MENU_RESULT_PENDING;
    s32 last_row = KF_MENU_OPTION_CANCEL_ROW;
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

    selected[KF_MENU_OPTION_EFFECTS_ROW] = player_state.audio_effects_enabled;
    selected[KF_MENU_OPTION_MUSIC_ROW] = player_state.audio_music_enabled;
    selected[KF_MENU_OPTION_GAUGES_ROW] = player_state.hud_gauges_enabled;
    selected[KF_MENU_OPTION_COMPASS_ROW] = player_state.compass_enabled;
    selected[KF_MENU_OPTION_ITEM_PREVIEW_ROW] = player_state.item_preview_enabled;
    selected[KF_MENU_OPTION_WALKING_BOB_ROW] = player_state.walking_bob_enabled;

    for (;;) {
        if (result != KF_MENU_RESULT_PENDING) {
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
                result = KF_MENU_RESULT_CANCELLED;
                confirmed = 1;
            }
        } else if (buttons & PADRdown) {
            menu_play_sound_cue(18);
            result = KF_MENU_RESULT_CANCELLED;
        }

        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            menu_draw_window(2, 7, choice, confirmed);
            menu_draw_options_rows(&labels[0], &labels[1], selected);
            menu_present_frame();
        }
    }

    player_state.audio_effects_enabled = selected[KF_MENU_OPTION_EFFECTS_ROW];
    player_state.audio_music_enabled = selected[KF_MENU_OPTION_MUSIC_ROW];
    player_state.hud_gauges_enabled = selected[KF_MENU_OPTION_GAUGES_ROW];
    player_state.compass_enabled = selected[KF_MENU_OPTION_COMPASS_ROW];
    player_state.item_preview_enabled = selected[KF_MENU_OPTION_ITEM_PREVIEW_ROW];
    player_state.walking_bob_enabled = selected[KF_MENU_OPTION_WALKING_BOB_ROW];
}

ADDRESS(0x8001b554, 0x2e0)
s32 menu_card_browser(void)
{
    struct DIRENTRY entries[KF_CARD_DIRECTORY_CAPACITY];
    KfMenuGlyphString rows[4];
    s32 matching_count;
    s32 cursor = 0;
    s32 confirmed = 0;
    s32 result = KF_MENU_RESULT_PENDING;
    s32 selection = KF_MENU_SELECTION_NONE;
    b32 card_full;
    s32 probe;
    s32 buttons;
    s32 frame;

    menu_enter_display_state(0);
    menu_prepare_card_browser_rows(rows);
    menu_show_dialog_panel(9, rows, 2, 70, 87, 178, 66, 2, 0);
    memory_card_start();
    probe = memory_card_probe_temporary_file();
    if (probe != 0 && probe != CARD_PROBE_TEMPORARY_FILE_CREATE_FAILURE) {
        menu_build_card_probe_error_rows(rows);
        menu_show_dialog_panel(9, rows, 3, 50, 87, 220, 66, 2, 0);
        input_wait_release();
        while (PadRead(1) == 0) {}
        input_wait_release();
        goto no_file;
    }

    card_full = memory_card_scan_save_entries(entries, &matching_count);
    if (matching_count == 0) {
        if (card_full == KF_TRUE) {
            menu_build_card_full_rows(rows);
            menu_show_dialog_panel(9, rows, 4, 70, 87, 178, 96, 2, 0);
            input_wait_release();
            while (PadRead(1) == 0) {}
            input_wait_release();
        }
no_file:
        memory_card_stop();
        menu_exit_display_state(0);
        return KF_MENU_RESULT_CANCELLED;
    }

    menu_play_sound_cue(16);
    input_wait_release();
    for (;;) {
        if (selection != KF_MENU_SELECTION_NONE)
            input_wait_release();
        switch (selection) {
        case 0:
            result = KF_MENU_RESULT_CANCELLED;
            break;
        case 1:
            result = menu_card_load_slot_browser();
            if (result == KF_MENU_RESULT_CANCELLED) {
                result = KF_MENU_RESULT_PENDING;
                selection = KF_MENU_SELECTION_NONE;
            }
            break;
        }
        if (result != KF_MENU_RESULT_PENDING)
            break;

        input_wait_brief_release();
        buttons = input_read_mark_active();
        if ((buttons & PADLup) || (buttons & PADLdown)) {
            menu_cursor_animation_direction = 0;
            menu_play_sound_cue(16);
            if (cursor == 0)
                cursor = 1;
            else
                cursor = 0;
        } else if (buttons & PADRright) {
            menu_play_sound_cue(17);
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
    menu_exit_display_state(0);
    return result;
}

ADDRESS(0x8001b834, 0x24c)
s32 menu_card_load_slot_browser(void)
{
    struct DIRENTRY entries[KF_CARD_DIRECTORY_CAPACITY];
    KfCardMenuList menu;
    KfCardSlotGlyphRow glyph_rows[CARD_MENU_ROW_CAPACITY];
    s32 experience_values[CARD_MENU_ROW_CAPACITY];
    u8 levels[CARD_MENU_ROW_CAPACITY];
    s32 slot_ids[CARD_MENU_ROW_CAPACITY];
    KfMenuGlyphString dialog_rows[3];
    s32 matching_count;
    s32 mode = 0;
    s32 result = KF_MENU_RESULT_PENDING;
    s32 count;
    s32 read_result;
    s32 frame;

    memory_card_scan_save_entries(entries, &matching_count);
    count = menu_card_build_slot_rows(entries, glyph_rows[0].codes,
        experience_values, levels, slot_ids);
    menu_list_init(&menu.list, 1, 0);
    menu.list.visible_rows = CARD_MENU_VISIBLE_ROWS;
    menu.list.list_y = CARD_MENU_LIST_Y;
    menu.list.entry_count = count;
    menu.rows = glyph_rows;
    menu.levels = levels;
    menu.experience_values = experience_values;

    for (;;) {
        if (mode != 0 || result != KF_MENU_RESULT_PENDING)
            input_wait_release();

        if (mode == 1) {
            result = menu_preview_choice(&menu, 6, 8, CARD_MENU_NO_PREVIEW_ITEM);
            if (result == KF_MENU_RESULT_CANCELLED)
                result = KF_MENU_RESULT_PENDING;
            else
                result = slot_ids[menu.list.selected_index];
        }
        if (result != KF_MENU_RESULT_PENDING)
            break;

        menu_update_list_input(&menu.list, NULL, &mode, &result);
        if (mode == 1)
            menu_play_sound_cue(16);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            menu_render_list(&menu, 8);
            menu_present_frame();
        }
    }

    if (result != KF_MENU_RESULT_CANCELLED) {
        menu_prepare_card_read_row(dialog_rows);
        menu_show_dialog_panel(9, dialog_rows, 1, 70, 87, 178, 66, 2, 0);
        read_result = memory_card_read_slot(result);
        if (read_result != 0) {
            menu_prepare_card_read_failure_rows(dialog_rows, read_result);
            menu_show_dialog_panel(9, dialog_rows, 3, 70, 87, 178, 81, 2, 0);
            input_wait_release();
            while (PadRead(1) == 0) {}
            input_wait_release();
        }
    }
    return result;
}

ADDRESS(0x8001ba80, 0x114)
void menu_build_card_probe_error_rows(KfMenuGlyphString *row)
{
    row->position.x = 70;
    row->position.y = 105;
    memcpy(row->glyphs.codes, menu_label_suffixes[5].codes, sizeof(KfMenuLabelSuffix));
    row->glyphs.codes[8] = 84;
    row->glyphs.codes[9] = 65;
    row->glyphs.codes[10] = 83;
    row->glyphs.codes[11] = -1;
    row++;
    row->position.x = 70;
    row->position.y = 120;
    row->glyphs.codes[0] = 0x1008;
    row->glyphs.codes[1] = 45;
    row->glyphs.codes[2] = 32;
    row->glyphs.codes[3] = 88;
    row->glyphs.codes[4] = 13;
    row->glyphs.codes[5] = 45;
    row->glyphs.codes[6] = 0x101b;
    row->glyphs.codes[7] = 0x1045;
    row->glyphs.codes[8] = -1;
    row++;
    row->position.x = 182;
    row->position.y = 120;
    row->glyphs.codes[0] = 0x1052;
    row->glyphs.codes[1] = 70;
    row->glyphs.codes[2] = 94;
    row->glyphs.codes[3] = 77;
    row->glyphs.codes[4] = 103;
    row->glyphs.codes[5] = -1;
}

ADDRESS(0x8001bb94, 0x168)
void menu_build_card_full_rows(KfMenuGlyphString *row)
{
    row->position.x = 104;
    row->position.y = 105;
    memcpy(row->glyphs.codes, menu_label_suffixes[5].codes, sizeof(KfMenuLabelSuffix));
    row++;
    row->position.x = 104;
    row->position.y = 120;
    row->glyphs.codes[0] = 65;
    row->glyphs.codes[1] = 59;
    row->glyphs.codes[2] = 0x2059;
    row->glyphs.codes[3] = 65;
    row->glyphs.codes[4] = 0x1052;
    row->glyphs.codes[5] = 76;
    row->glyphs.codes[6] = -1;
    row++;
    row->position.x = 104;
    row->position.y = 135;
    row->glyphs.codes[0] = 73;
    row->glyphs.codes[1] = 88;
    row->glyphs.codes[2] = 5;
    row->glyphs.codes[3] = 45;
    row->glyphs.codes[4] = 0x1013;
    row->glyphs.codes[5] = 85;
    row->glyphs.codes[6] = 89;
    row->glyphs.codes[7] = -1;
    row++;
    row->position.x = 104;
    row->position.y = 150;
    memcpy(row->glyphs.codes, menu_label_suffixes[2].codes, sizeof(KfMenuLabelSuffix));
}

ADDRESS(0x8001bcfc, 0x26c)
void menu_card_save_browser(void)
{
    struct DIRENTRY entries[KF_CARD_DIRECTORY_CAPACITY];
    KfCardMenuList menu;
    KfCardSlotGlyphRow glyph_rows[CARD_MENU_ROW_CAPACITY];
    s32 experience_values[CARD_MENU_ROW_CAPACITY];
    u8 levels[CARD_MENU_ROW_CAPACITY];
    s32 slot_ids[CARD_MENU_ROW_CAPACITY];
    KfMenuGlyphString dialog_rows[2];
    s32 matching_count;
    s32 mode = 0;
    s32 result = KF_MENU_RESULT_PENDING;
    s32 count;
    s32 frame;

    menu_enter_display_state(1);
    menu_prepare_card_browser_rows(dialog_rows);
    menu_draw_card_dialog_rows(dialog_rows, 2, 70, 87, 178, 66, 2, 0);
    memory_card_start();
    memory_card_probe_temporary_file();
    memory_card_scan_save_entries(entries, &matching_count);
    count = menu_card_build_slot_rows(entries, glyph_rows[0].codes,
        experience_values, levels, slot_ids);
    glyph_rows[count].codes[0] = 0xe0;
    glyph_rows[count].codes[1] = 0xe1;
    glyph_rows[count].codes[2] = 0xe2;
    glyph_rows[count].codes[3] = -1;
    experience_values[count] = -1;
    levels[count] = CARD_MENU_NO_LEVEL;
    slot_ids[count] = CARD_MENU_NEW_SLOT;
    count++;

    menu_list_init(&menu.list, 1, 3);
    menu.list.visible_rows = CARD_MENU_VISIBLE_ROWS;
    menu.list.list_y = CARD_MENU_LIST_Y;
    menu.list.entry_count = count;
    menu.rows = glyph_rows;
    menu.levels = levels;
    menu.experience_values = experience_values;
    menu_play_sound_cue(16);
    input_wait_release();

    for (;;) {
        if (mode != 0 || result != KF_MENU_RESULT_PENDING)
            input_wait_release();
        if (mode == 1) {
            result = menu_preview_choice(&menu, 7, 9, CARD_MENU_NO_PREVIEW_ITEM);
            if (result == KF_MENU_RESULT_CANCELLED)
                result = KF_MENU_RESULT_PENDING;
            else
                result = menu.list.selected_index;
        }
        if (result != KF_MENU_RESULT_PENDING)
            break;

        menu_update_list_input(&menu.list, NULL, &mode, &result);
        if (mode == 1)
            menu_play_sound_cue(16);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            menu_render_list(&menu, 9);
            menu_present_frame();
        }
    }

    if (result != KF_MENU_RESULT_CANCELLED)
        menu_card_save_slot(slot_ids[result]);
    memory_card_stop();
    menu_exit_display_state(0);
}

ADDRESS(0x8001bf68, 0x1c4)
void menu_card_save_slot(s32 slot)
{
    KfMenuGlyphString rows[4];
    s32 probe = memory_card_probe_temporary_file();
    s32 result;

    if (probe != 0) {
        if (probe != CARD_PROBE_TEMPORARY_FILE_CREATE_FAILURE) {
            menu_prepare_card_io_error_rows(rows);
            menu_draw_card_dialog_rows(rows, 3, 70, 87, 178, 81, 2, 0);
            input_wait_release();
            while (PadRead(1) == 0) {}
            goto wait_release;
        }
        if (menu_confirm_card_format(1) == 0) {
            memory_card_format();
            goto write_file;
        }
        menu_prepare_card_format_declined_rows(rows);
        menu_draw_card_dialog_rows(rows, 3, 70, 87, 192, 66, 2, 0);
        input_wait_release();
        while (PadRead(1) == 0) {}
        goto wait_release;
    }

write_file:
    menu_prepare_card_write_rows(rows);
    menu_draw_card_dialog_rows(rows, 2, 70, 87, 178, 66, 2, 0);
    result = memory_card_write_slot(slot);
    if (result == 0)
        return;
    if (result == CARD_WRITE_IO_FAILURE)
        menu_prepare_card_io_error_rows(rows);
    else
        menu_prepare_card_write_full_rows(rows);
    menu_draw_card_dialog_rows(rows, 3, 70, 87, 178, 81, 2, 0);
    input_wait_release();
    while (PadRead(1) == 0) {}

wait_release:
    input_wait_release();
}

ADDRESS(0x8001c12c, 0x424)
s32 menu_confirm_card_format(s32 kind)
{
    KfMenuGlyphString labels[7];
    s32 cursor = 0;
    s32 confirmed = 0;
    s32 result = KF_MENU_RESULT_PENDING;
    s32 selection = KF_MENU_SELECTION_NONE;
    s32 frame;

    labels[0].position.x = menu_window_layouts[1].rows[0].position.x;
    labels[0].position.y = menu_window_layouts[1].rows[0].position.y;
    labels[0].glyphs.codes[0] = 89;
    labels[0].glyphs.codes[1] = 65;
    labels[0].glyphs.codes[2] = -1;
    labels[1].position.x = menu_window_layouts[1].rows[1].position.x;
    labels[1].position.y = menu_window_layouts[1].rows[1].position.y;
    labels[1].glyphs.codes[0] = 65;
    labels[1].glyphs.codes[1] = 65;
    labels[1].glyphs.codes[2] = 67;
    labels[1].glyphs.codes[3] = -1;

    if (kind == 1) {
        labels[2].position.x = 90;
        labels[2].position.y = 110;
        memcpy(labels[2].glyphs.codes, menu_label_suffixes[5].codes, sizeof(KfMenuLabelSuffix));
        labels[3].position.x = 90;
        labels[3].position.y = 125;
        memcpy(labels[3].glyphs.codes, menu_label_suffixes[6].codes, sizeof(KfMenuLabelSuffix));
        labels[4].position.x = 174;
        labels[4].position.y = 125;
        memcpy(labels[4].glyphs.codes, menu_label_suffixes[7].codes, sizeof(KfMenuLabelSuffix));
        labels[5].position.x = 90;
        labels[5].position.y = 140;
        memcpy(labels[5].glyphs.codes, menu_label_suffixes[6].codes, sizeof(KfMenuLabelSuffix));
        labels[6].position.x = 174;
        labels[6].position.y = 140;
        memcpy(labels[6].glyphs.codes, menu_label_suffixes[8].codes, sizeof(KfMenuLabelSuffix));
    }

    for (;;) {
        if (selection != KF_MENU_SELECTION_NONE || result != KF_MENU_RESULT_PENDING)
            input_wait_release();
        switch (selection) {
        case 0:
            result = 0;
            break;
        case 1:
            result = KF_MENU_RESULT_CANCELLED;
            break;
        }
        if (result != KF_MENU_RESULT_PENDING)
            break;

        cursor = menu_poll_choice_input(cursor, 1, &selection, &confirmed, &result);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            menu_draw_two_option(&labels[0], &labels[1], cursor, confirmed);
            menu_blit_sprite_translucent(&menu_sprite_defs[5],
                &menu_window_layouts[1].rows[3].position);
            menu_draw_string(&menu_sprite_defs[1], &menu_window_layouts[1].rows[3]);
            if (kind == 1) {
                menu_draw_string(&menu_sprite_defs[1], &labels[2]);
                menu_draw_string(&menu_sprite_defs[1], &labels[3]);
                menu_draw_string(&menu_sprite_defs[1], &labels[4]);
                menu_draw_string(&menu_sprite_defs[1], &labels[5]);
                menu_draw_string(&menu_sprite_defs[1], &labels[6]);
                menu_draw_nine_slice_panel(70, 92, 220, 81, 2, 0);
            }
            menu_present_frame();
        }
    }
    return result;
}


ADDRESS(0x8001c550, 0xdc)
void menu_prepare_card_browser_rows(KfMenuGlyphString *row)
{
    row->position.x = 90;
    row->position.y = 105;
    memcpy(row->glyphs.codes, menu_label_suffixes[0].codes,
        sizeof menu_label_suffixes[0]);
    row++;
    row->position.x = 90;
    row->position.y = 120;
    memcpy(row->glyphs.codes, menu_label_suffixes[1].codes,
        sizeof menu_label_suffixes[1]);
    row->glyphs.codes[10] = -1;
}

ADDRESS(0x8001c62c, 0x144)
void menu_prepare_card_io_error_rows(KfMenuGlyphString *row)
{
    row->position.x = 90;
    row->position.y = 105;
    memcpy(row->glyphs.codes, menu_label_suffixes[2].codes,
        sizeof menu_label_suffixes[2]);
    row++;
    row->position.x = 90;
    row->position.y = 120;
    memcpy(row->glyphs.codes, menu_label_suffixes[3].codes,
        sizeof menu_label_suffixes[3]);
    row++;
    row->position.x = 90;
    row->position.y = 135;
    memcpy(row->glyphs.codes, menu_label_suffixes[4].codes,
        sizeof menu_label_suffixes[4]);
    row->glyphs.codes[10] = -1;
}

ADDRESS(0x8001c770, 0x140)
void menu_prepare_card_format_declined_rows(KfMenuGlyphString *row)
{
    row->position.x = 90;
    row->position.y = 105;
    memcpy(row->glyphs.codes, menu_label_suffixes[6].codes,
        sizeof menu_label_suffixes[6]);
    row++;
    row->position.x = 174;
    row->position.y = 105;
    memcpy(row->glyphs.codes, menu_label_suffixes[9].codes,
        sizeof menu_label_suffixes[9]);
    row++;
    row->position.x = 90;
    row->position.y = 120;
    memcpy(row->glyphs.codes, menu_label_suffixes[2].codes,
        sizeof menu_label_suffixes[2]);
}

ADDRESS(0x8001c8b0, 0x144)
void menu_prepare_card_write_full_rows(KfMenuGlyphString *row)
{
    row->position.x = 102;
    row->position.y = 105;
    memcpy(row->glyphs.codes, menu_label_suffixes[2].codes,
        sizeof menu_label_suffixes[2]);
    row++;
    row->position.x = 102;
    row->position.y = 120;
    memcpy(row->glyphs.codes, menu_label_suffixes[0].codes,
        sizeof menu_label_suffixes[0]);
    row++;
    row->position.x = 102;
    row->position.y = 135;
    memcpy(row->glyphs.codes, menu_label_suffixes[12].codes,
        sizeof menu_label_suffixes[12]);
    row->glyphs.codes[10] = -1;
}

ADDRESS(0x8001c9f4, 0xe0)
void menu_prepare_card_write_rows(KfMenuGlyphString *row)
{
    row->position.x = 102;
    row->position.y = 105;
    memcpy(row->glyphs.codes, menu_label_suffixes[0].codes,
        sizeof menu_label_suffixes[0]);
    row->glyphs.codes[7] = 85;
    row++;
    row->position.x = 102;
    row->position.y = 120;
    memcpy(row->glyphs.codes, menu_label_suffixes[11].codes,
        sizeof menu_label_suffixes[11]);
}

ADDRESS(0x8001cad4, 0x70)
void menu_prepare_card_read_row(KfMenuGlyphString *row)
{
    row->position.x = 102;
    row->position.y = 112;
    memcpy(row->glyphs.codes, menu_label_suffixes[13].codes,
        sizeof menu_label_suffixes[13]);
}

ADDRESS(0x8001cb44, 0x190)
void menu_prepare_card_read_failure_rows(KfMenuGlyphString *row, s32 kind)
{
    row->position.x = 90;
    row->position.y = 105;
    if (kind == 1) {
        memcpy(row->glyphs.codes, menu_label_suffixes[10].codes,
            sizeof menu_label_suffixes[10]);
    } else {
        row->glyphs.codes[0] = 0x1012;
        row->glyphs.codes[1] = 45;
        row->glyphs.codes[2] = 15;
        row->glyphs.codes[3] = 3;
        row->glyphs.codes[4] = 40;
        row->glyphs.codes[5] = 45;
        row->glyphs.codes[6] = -1;
    }
    row++;
    row->position.x = 90;
    row->position.y = 120;
    memcpy(row->glyphs.codes, menu_label_suffixes[3].codes,
        sizeof menu_label_suffixes[3]);
    row++;
    row->position.x = 90;
    row->position.y = 135;
    memcpy(row->glyphs.codes, menu_label_suffixes[4].codes,
        sizeof menu_label_suffixes[4]);
    row->glyphs.codes[10] = -1;
}

ADDRESS(0x8001ccd4, 0xdc)
void menu_prepare_card_exit_rows(KfMenuGlyphString *row)
{
    row->position.x = 90;
    row->position.y = 105;
    memcpy(row->glyphs.codes, menu_label_suffixes[14].codes,
        sizeof menu_label_suffixes[14]);
    row++;
    row->position.x = 90;
    row->position.y = 120;
    memcpy(row->glyphs.codes, menu_label_suffixes[15].codes,
        sizeof menu_label_suffixes[15]);
    row->glyphs.codes[10] = -1;
}

ADDRESS(0x8001cdb0, 0x108)
void menu_draw_card_dialog_rows(const KfMenuGlyphString *rows, s32 count,
    s32 x, s32 y, s32 width, s32 height, s32 overlap_x, s32 overlap_y)
{
    const KfMenuGlyphString *current;
    s32 frame;
    s32 row;

    for (frame = 0; frame < 2; frame++) {
        menu_frame_begin();
        menu_blit_sprite_translucent(&menu_sprite_defs[5],
            &menu_window_layouts[1].rows[3].position);
        menu_draw_string(&menu_sprite_defs[1], &menu_window_layouts[1].rows[3]);
        current = rows;
        for (row = 0; row < count; row++, current++)
            menu_draw_string(&menu_sprite_defs[1], current);
        menu_draw_nine_slice_panel(x, y, width, height, overlap_x, overlap_y);
        menu_present_frame();
    }
}


DATA(0x80063e80, 0xf0)
KfMenuSpriteDef menu_sprite_defs[KF_MENU_SPRITE_COUNT] = {
    {0x1d, 0x7f64, 240, 0, 7, 14},
    {0x1d, 0x7f64, 0, 0, 15, 15},
    {0x1c, 0x7ee4, 128, 128, 14, 14},
    {0x1e, 0x7fa4, 160, 0, 54, 24},
    {0x1e, 0x7fa4, 160, 32, 54, 24},
    {0x1e, 0x7fa4, 0, 0, 124, 24},
    {0x1e, 0x7fa4, 0, 32, 124, 24},
    {0x1e, 0x7fa4, 0, 64, 236, 6},
    {0x1e, 0x7fa4, 0, 72, 236, 14},
    {0x1e, 0x7fa4, 0, 104, 236, 6},
    {0x1e, 0x7fa4, 0, 88, 236, 14},
    {0x1e, 0x7fa4, 0, 120, 33, 33},
    {0x1e, 0x7fa4, 40, 120, 28, 33},
    {0x1e, 0x7fa4, 72, 120, 33, 33},
    {0x1e, 0x7fa4, 0, 160, 33, 28},
    {0x1e, 0x7fa4, 40, 160, 28, 28},
    {0x1e, 0x7fa4, 72, 160, 33, 28},
    {0x1e, 0x7fa4, 0, 192, 33, 33},
    {0x1e, 0x7fa4, 40, 192, 28, 33},
    {0x1e, 0x7fa4, 72, 192, 33, 33},
};
DATA(0x80063f70, 0x9a0)
KfMenuWindowLayout menu_window_layouts[KF_MENU_WINDOW_COUNT] = {
    {
        {{0, 0}, {{-1}}},
        {
            {{31, 19}, {{0, 1, 18, 32, 109, 114, 66, -1}}},
            {{31, 45}, {{120, 121, 109, 114, 66, -1}}},
            {{31, 71}, {{112, 113, -1}}},
            {{31, 97}, {{137, 138, 57, 122, 208, -1}}},
            {{31, 123}, {{117, 82, 106, -1}}},
            {{31, 149}, {{11, 12, 18, 32, -1}}},
            {{31, 175}, {{4, 8219, 11, 56, 39, -1}}},
            {{31, 201}, {{98, 4179, 106, -1}}},
        },
    },
    {
        {{17, 19}, {{11, 12, 18, 32, -1}}},
        {
            {{31, 45}, {{44, 45, 4115, -1}}},
            {{31, 71}, {{4104, 45, 32, 109, 99, 97, 106, -1}}},
            {{31, 97}, {{98, 4179, 106, -1}}},
            {{17, 19}, {{13, 45, 4123, -1}}},
            {{17, 19}, {{44, 45, 4115, -1}}},
            {{17, 19}, {{4104, 45, 32, 109, 99, 97, 106, -1}}},
        },
    },
    {
        {{17, 19}, {{4, 8219, 11, 56, 39, -1}}},
        {
            {{31, 45}, {{213, 214, 215, -1}}},
            {{31, 71}, {{215, 216, -1}}},
            {{31, 97}, {{133, 134, 217, 218, -1}}},
            {{31, 123}, {{219, 220, 217, 218, -1}}},
            {{31, 149}, {{0, 1, 18, 32, 217, 218, -1}}},
            {{31, 175}, {{238, 239, 213, 214, -1}}},
            {{31, 201}, {{98, 4179, 106, -1}}},
        },
    },
    {
        {{17, 19}, {{116, 66, 57, 115, 106, -1}}},
        {
            {{31, 45}, {{116, 66, -1}}},
            {{31, 71}, {{115, 106, -1}}},
            {{31, 97}, {{98, 4179, 106, -1}}},
        },
    },
    {
        {{17, 19}, {{164, 206, -1}}},
        {
            {{31, 45}, {{116, 66, -1}}},
            {{31, 71}, {{256, 257, 109, 207, 106, -1}}},
            {{31, 97}, {{98, 4179, 106, -1}}},
        },
    },
    {
        {{17, 19}, {{0, -1}}},
        {
            {{31, 45}, {{258, 259, -1}}},
            {{31, 71}, {{141, 142, 206, -1}}},
        },
    },
    {
        {{0, 0}, {{-1}}},
        {
            {{98, 94}, {{89, 4171, 97, 106, -1}}},
            {{98, 120}, {{44, 45, 4115, 76, 106, -1}}},
        },
    },
    {0},
};

enum {
    KF_MENU_UPLOAD_SECOND_BUFFER_Y = 240,
    KF_MENU_CURSOR_FRAME_COUNT = 8
};
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
DATA(0x80064a00, 0xf0)
KfMenuLabelSuffix menu_header_labels[12] = {
    {{130, 131, 132, -1, 0, 0, 0, 0, 0, 0}},
    {{43, 4124, 42, -1, 0, 0, 0, 0, 0, 0}},
    {{137, 138, 139, -1, 0, 0, 0, 0, 0, 0}},
    {{122, 208, 139, -1, 0, 0, 0, 0, 0, 0}},
    {{133, 134, -1, 0, 0, 0, 0, 0, 0}},
    {{8217, 40, 40, 1, 4108, -1, 0, 0, 0, 0}},
    {{255, 255, 5, 45, 12, -1, 0, 0, 0, 0}},
    {{255, 255, 12, 44, 45, -1, 0, 0, 0, 0}},
    {{255, 8221, 1, 4110, 39, -1, 0, 0, 0, 0}},
    {{255, 255, 4111, 45, 7, -1, 0, 0, 0, 0}},
    {{255, 255, 255, 197, 198, -1, 0, 0, 0, 0}},
    {{4105, 45, 42, 4115, -1, 0, 0, 0, 0}},
};
DATA(0x80064af0, 0x140)
KfMenuLabelSuffix menu_label_suffixes[16] = {
    {{33, 34, 41, 45, 5, 45, 4115, 88, -1, 0}},
    {{16, 51, 53, 7, 109, 75, 82, 65, 94, 76}},
    {{13, 45, 4123, 4178, 70, 94, 77, 103, -1, 0}},
    {{33, 34, 41, 45, 5, 45, 4115, 109, -1, 0}},
    {{16, 51, 53, 7, 75, 82, 71, 4175, 74, 65}},
    {{33, 34, 41, 45, 5, 45, 4115, 4165, -1, 0}},
    {{27, 52, 45, 30, 53, 19, -1, 0, 0, 0}},
    {{74, 107, 82, 65, 94, 77, 103, -1, 0, 0}},
    {{75, 94, 76, 69, -1, 0, 0, 0, 0, 0}},
    {{109, 75, 84, 65, 83, -1, 0, 0, 0, 0}},
    {{44, 45, 4115, 4178, 70, 94, 77, 103, -1, 0}},
    {{13, 45, 4123, 75, 82, 65, 94, 76, -1, 0}},
    {{267, 268, 4165, 79, 105, 94, 77, 103, -1, 0}},
    {{44, 45, 4115, 75, 82, 65, 94, 76, -1, 0}},
    {{269, 270, 109, 86, 65, 82, -1, 0, 0, 0}},
    {{161, 271, 109, 263, 59, 82, 71, 4175, 74, 65}}
};
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
DATA(0x80065c20, 0x5a0)
u16 menu_item_code_primary[6][120] = {
    {
        50000, 50000, 50000, 990, 50000, 2850, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 1830, 50000, 50000, 50000, 50000, 50000, 680, 2950,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 780,
        50000, 50000, 50000, 50000, 50000, 50000, 1200, 3400, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 320,
        50000, 50000, 23, 48, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
    },
    {
        50000, 180, 640, 50000, 50000, 50000, 4300, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 200, 780, 50000,
        50000, 50000, 50000, 50000, 330, 950, 50000, 50000, 50000, 50000, 150, 830,
        50000, 50000, 50000, 50000, 50000, 270, 50000, 3900, 50000, 50000, 50000, 350,
        1800, 4000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 330,
        650, 17700, 20, 40, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
    },
    {
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 180, 50000,
        2380, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 8250, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 6300, 50000, 50000, 50000,
        50000, 50000, 18, 33, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 4800, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
    },
    {
        50000, 50000, 50000, 50000, 50000, 3250, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 2020, 50000, 50000, 50000, 50000, 50000, 50000, 3600,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 1000,
        50000, 19800, 50000, 50000, 50000, 50000, 50000, 3400, 50000, 50000, 50000, 50000,
        1600, 4200, 50000, 23800, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 28, 52, 2800, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        15000, 50000, 50000, 2800, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
    },
    {
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 2, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 1, 50000, 50000,
    },
    {
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
        50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 8350, 1530,
        13500, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000, 50000,
    },
};
DATA(0x800661c0, 0x4b0)
u16 menu_item_code_secondary[5][120] = {
    {
        70, 120, 520, 750, 2000, 2100, 2700, 0, 0, 13500, 12800, 13000,
        15000, 26000, 28500, 30000, 1750, 2800, 0, 0, 0, 130, 550, 2000,
        14800, 16500, 23500, 0, 130, 570, 18500, 14500, 25000, 0, 50, 500,
        1300, 18000, 14300, 28000, 0, 150, 880, 1750, 12750, 21000, 0, 210,
        1500, 3500, 13300, 20000, 0, 1880, 2500, 8230, 9100, 11000, 6200, 3350,
        0, 0, 0, 0, 0, 0, 0, 4700, 3700, 6280, 0, 250,
        600, 4830, 8, 13, 1600, 700, 700, 700, 700, 700, 700, 3280,
        1030, 6000, 3300, 1800, 40000, 36500, 8500, 8500, 8500, 8500, 8500, 13500,
        530, 25, 0, 4500, 6500, 9750, 100, 280, 2400, 11000, 6000, 900,
        1900, 18000, 0, 2400, 2400, 2400, 1800, 1800, 1800, 20, 130, 0,
    },
    {
        80, 130, 450, 800, 1900, 1900, 2100, 0, 0, 7300, 8500, 13600,
        14300, 24800, 24800, 25000, 1630, 3200, 0, 0, 0, 140, 600, 2000,
        17000, 16000, 20000, 0, 230, 480, 12700, 14800, 20000, 0, 80, 450,
        1430, 15100, 14300, 20000, 0, 180, 1000, 2850, 11800, 21000, 0, 210,
        1500, 3000, 12700, 19800, 0, 1430, 1300, 7850, 8770, 11200, 7300, 3280,
        0, 0, 0, 0, 0, 0, 0, 4800, 3300, 6300, 0, 250,
        580, 4300, 12, 16, 1800, 800, 800, 800, 800, 800, 800, 2200,
        950, 5800, 2800, 1800, 38000, 3200, 7000, 7000, 7000, 7000, 8270, 13500,
        550, 26, 0, 350, 5220, 8500, 200, 350, 2000, 8640, 5880, 950,
        1880, 18000, 0, 2400, 2400, 2400, 1800, 1800, 1800, 20, 130, 0,
    },
    {
        80, 140, 480, 550, 1450, 1300, 1730, 0, 0, 4700, 7000, 7800,
        8150, 8500, 8500, 8330, 1780, 3500, 0, 0, 0, 160, 580, 2150,
        9200, 6380, 7700, 0, 250, 600, 4500, 4850, 7600, 0, 100, 580,
        1500, 4000, 3000, 5500, 0, 200, 650, 2230, 1200, 4700, 0, 180,
        1230, 3550, 1250, 5100, 0, 1700, 500, 5200, 1380, 1500, 5500, 3600,
        0, 0, 0, 0, 0, 0, 0, 3200, 4500, 1200, 0, 250,
        580, 2450, 13, 25, 2000, 950, 950, 950, 950, 950, 950, 1100,
        370, 1500, 1850, 600, 1330, 5110, 5800, 5800, 5800, 5800, 5980, 4800,
        500, 25, 0, 10, 4830, 6150, 550, 500, 1800, 3500, 4300, 1020,
        2530, 8320, 0, 2250, 2250, 2250, 1700, 1700, 1700, 20, 130, 0,
    },
    {
        60, 150, 600, 880, 1600, 1780, 2550, 0, 0, 8500, 11000, 11500,
        16500, 22000, 25300, 28800, 1700, 2800, 0, 0, 0, 200, 480, 2000,
        12200, 14300, 19800, 0, 100, 350, 13800, 15000, 21000, 0, 50, 370,
        1150, 16200, 14300, 22000, 0, 150, 730, 1630, 12750, 17700, 0, 250,
        1340, 3300, 13300, 16000, 0, 1570, 2500, 9650, 9100, 12000, 6200, 3350,
        0, 0, 0, 0, 0, 0, 0, 4700, 5200, 7850, 0, 250,
        435, 5120, 5, 10, 1600, 530, 530, 530, 530, 530, 530, 8200,
        950, 6650, 3300, 1800, 36000, 3850, 8500, 8500, 8500, 8500, 8500, 22750,
        530, 25, 0, 5000, 6500, 10800, 100, 250, 2400, 11000, 6350, 850,
        1760, 18000, 0, 2400, 2400, 2400, 1800, 1800, 1800, 20, 130, 0,
    },
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 5000, 6500, 11300, 760, 580, 3800, 16200, 7800, 1400,
        10500, 22900, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    },
};
DATA(0x8006d68c, 0x4)
s32 menu_item_model_allocation_pending = 0;
DATA(0x8006d690, 0x4)
s32 input_press_pending = 0;
DATA(0x8006d694, 0x4)
s32 menu_item_quantity = 1;
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
DATA(0x8006da00, 0x8)
SVECTOR menu_item_preview_translation = {0};
DATA(0x8006da08, 0x8)
SVECTOR menu_item_preview_rotation = {0};
DATA(0x8006da10, 0x4)
s32 menu_item_preview_rotation_step = 0;
DATA(0x8006dbe8, 0x18)
KfPrimitiveBuffer menu_saved_primitive_buffers[KF_DISPLAY_BUFFER_COUNT];

ADDRESS(0x8001ceb8, 0x178)
void menu_item_buy_sell_controller(s32 kind)
{
    s32 cursor = 0;
    s32 confirmed = 0;
    s32 result = -99;
    s32 selection = -1;
    s32 frame;

    menu_enter_display_state(1);
    for (frame = 0; frame < 2; frame++) {
        menu_frame_begin();
        menu_draw_window(3, 3, cursor, confirmed);
        menu_present_frame();
    }
    menu_play_sound_cue(16);
    input_wait_release();
    for (;;) {
        if (selection != -1 || result != -99)
            input_wait_release();

        switch (selection) {
        case 0:
            menu_item_buy_controller(kind);
            break;
        case 1:
            menu_item_sell_controller(kind);
            break;
        }

        if (result != -99)
            break;
        cursor = menu_poll_choice_input(cursor, 2, &selection, &confirmed, &result);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            menu_draw_window(3, 3, cursor, confirmed);
            menu_present_frame();
        }
    }
    menu_exit_display_state(0);
}

ADDRESS(0x8001d030, 0x310)
void menu_item_buy_controller(s32 kind)
{
    KfItemMenuList menu;
    KfMenuGlyphRow rows[40];
    u8 values[40];
    u32 codes[40];
    u8 indices[40];
    s32 selection = 0;
    s32 result = -99;
    s32 count;
    s32 frame;
    s32 cost;
    u8 selected_item;
    u8 *counters = game_counter_bytes;

    if (counters[0x33] == 0)
        menu_item_mask_pages[3][51] = 1;
    else
        menu_item_mask_pages[3][51] = 0;
    count = menu_collect_masked_item_rows(menu_item_mask_pages[kind], rows, values, indices,
        0, 119);
    menu_fill_item_counts_and_prices(counters, values, codes, indices, 0, count, kind);
    menu_list_init(&menu.list, 3, 0);
    menu.list.entry_count = count;
    menu.rows = rows;
    menu.values = values;
    menu.codes = codes;
    menu.list.glyphs_per_entry = 12;

    if (menu_load_item_model(indices[menu.list.selected_index]) != 0)
        return;

    for (;;) {
        if (selection != 0 || result != -99)
            input_wait_release();

        if (selection == 1) {
            result = menu_preview_choice(&menu, 3, 10, selected_item);
            if (result == -1)
                result = -99;
            else
                result = selected_item;
        }
        if (result != -99)
            break;

        menu_update_list_input(&menu.list, indices, &selection, &result);
        selected_item = indices[menu.list.selected_index];
        if (selection == 1) {
            if (player_state.gold < (u32)((s32)menu.codes[menu.list.selected_index]
                    * menu_item_quantity)
                    || counters[selected_item] + menu_item_quantity >= 100) {
                menu_play_sound_cue(18);
                selection = 0;
            } else {
                menu_play_sound_cue(17);
            }
        }

        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            if (menu.list.entry_count != 0)
                menu_update_item_preview(selected_item);
            menu_render_list(&menu, 10);
            menu_present_frame();
        }
    }

    menu_release_item_model();
    if (result != -1) {
        cost = (s32)menu.codes[menu.list.selected_index] * menu_item_quantity;
        player_state.gold -= cost;
        counters[result] += (u8)menu_item_quantity;
    }
}

ADDRESS(0x8001d340, 0x74)
void menu_fill_item_counts_and_prices(const u8 *source, u8 *counts, u32 *prices,
    const u8 *indices, s32 first, s32 last, s32 group)
{
    const u16 *page;

    if (first < last) {
        const u16 (*pages)[120] = menu_item_code_primary;
        page = pages[group];
        do {
            *counts++ = source[*indices];
            *prices++ = page[*indices++];
            first++;
        } while (first < last);
    }
}

ADDRESS(0x8001d3b4, 0x2a0)
void menu_item_sell_controller(s32 kind)
{
    KfItemMenuList menu;
    KfMenuGlyphRow rows[120];
    u8 values[120];
    u32 codes[120];
    u8 indices[120];
    s32 selection = 0;
    s32 result = -99;
    s32 count;
    s32 frame;
    s32 payment;
    u8 selected_item;

    count = menu_collect_available_item_rows(game_counter_bytes, rows, values, indices, 0, 119);
    menu_fill_item_prices(codes, indices, 0, count, kind);
    menu_list_init(&menu.list, 3, 1);
    menu.list.entry_count = count;
    menu.rows = rows;
    menu.values = values;
    menu.codes = codes;
    menu.list.glyphs_per_entry = 12;

    if (menu.list.entry_count != 0
            && menu_load_item_model(indices[menu.list.selected_index]) != 0)
        return;

    for (;;) {
        if (selection != 0 || result != -99)
            input_wait_release();

        if (selection == 1) {
            result = menu_preview_choice(&menu, 4, 11, selected_item);
            if (result == -1)
                result = -99;
            else
                result = selected_item;
        }
        if (result != -99)
            break;

        menu_update_list_input(&menu.list, indices, &selection, &result);
        selected_item = indices[menu.list.selected_index];
        if (selection == 1) {
            if (values[menu.list.selected_index] < menu_item_quantity) {
                menu_play_sound_cue(18);
                selection = 0;
            } else {
                menu_play_sound_cue(17);
            }
        }

        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            if (menu.list.entry_count != 0)
                menu_update_item_preview(selected_item);
            menu_render_list(&menu, 11);
            menu_present_frame();
        }
    }

    menu_release_item_model();
    if (result != -1) {
        payment = (s32)menu.codes[menu.list.selected_index] * menu_item_quantity;
        player_state.gold += payment;
        game_counter_bytes[result] -= (u8)menu_item_quantity;
    }
}

ADDRESS(0x8001d654, 0x54)
void menu_fill_item_prices(u32 *prices, const u8 *indices, s32 first, s32 last, s32 group)
{
    const u16 *page;

    if (first < last) {
        const u16 (*pages)[120] = menu_item_code_secondary;
        page = pages[group];
        do {
            *prices++ = page[*indices++];
            first++;
        } while (first < last);
    }
}

ADDRESS(0x8001d6a8, 0x228)
s32 menu_choose_inventory_item(void)
{
    KfItemMenuList menu;
    KfMenuGlyphRow rows[120];
    u8 values[120];
    u8 indices[120];
    s32 mode = 0;
    s32 result = -99;
    s32 count;
    u8 selected_item;
    s32 frame;

    menu_enter_display_state(1);
    count = menu_collect_masked_item_rows(game_counter_bytes, rows, values, indices, 0, 119);
    menu_list_init(&menu.list, 5, 0);
    menu.list.entry_count = count;
    menu.rows = rows;
    menu.values = values;
    menu.list.glyphs_per_entry = 12;
    selected_item = indices[menu.list.selected_index];
    if (menu_load_item_model(selected_item) != 0)
        return -1;

    for (frame = 0; frame < 2; frame++) {
        menu_frame_begin();
        if (menu.list.entry_count != 0)
            menu_update_item_preview((u8)selected_item);
        menu_render_list(&menu, 12);
        menu_present_frame();
    }
    menu_play_sound_cue(16);
    input_wait_release();

    for (;;) {
        if (mode != 0 || result != -99)
            input_wait_release();

        if (mode == 1) {
            result = menu_preview_choice(&menu, 8, 12, (u8)selected_item);
            if (result == -1)
                result = -99;
            else
                result = selected_item;
        }
        if (result != -99)
            break;

        menu_update_list_input(&menu.list, indices, &mode, &result);
        selected_item = indices[menu.list.selected_index];
        if (mode == 1)
            menu_play_sound_cue(17);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            if (menu.list.entry_count != 0)
                menu_update_item_preview((u8)selected_item);
            menu_render_list(&menu, 12);
            menu_present_frame();
        }
    }

    menu_release_item_model();
    menu_exit_display_state(0);
    return result;
}
ADDRESS(0x8001d8d0, 0x394)
void menu_item_trade_controller(void)
{
    KfItemMenuList menu;
    KfMenuGlyphRow rows[40];
    u8 values[40];
    u32 codes[40];
    u8 indices[40];
    u8 *counters = game_counter_bytes;
    s32 selection = 0;
    s32 result = -99;
    s32 count;
    s32 frame;
    s32 cost;
    u8 selected_item;

    menu_enter_display_state(1);
    count = menu_collect_masked_item_rows(menu_item_mask_pages[4], rows, values, indices,
        0, 119);
    menu_fill_item_counts_and_prices(counters, values, codes, indices, 0, count, 4);
    menu_list_init(&menu.list, 5, 1);
    menu.list.entry_count = count;
    menu.rows = rows;
    menu.values = values;
    menu.codes = codes;
    menu.list.glyphs_per_entry = 12;

    selected_item = indices[menu.list.selected_index];
    if (menu_load_item_model(selected_item) != 0)
        return;

    for (frame = 0; frame < 2; frame++) {
        menu_frame_begin();
        if (menu.list.entry_count != 0)
            menu_update_item_preview(selected_item);
        menu_render_list(&menu, 15);
        menu_present_frame();
    }
    menu_play_sound_cue(16);
    input_wait_release();

    for (;;) {
        if (selection != 0 || result != -99)
            input_wait_release();

        if (selection == 1) {
            result = menu_preview_choice(&menu, 9, 15, selected_item);
            if (result == -1)
                result = -99;
            else
                result = selected_item;
        }
        if (result != -99)
            break;

        menu_update_list_input(&menu.list, indices, &selection, &result);
        selected_item = indices[menu.list.selected_index];
        if (selection == 1) {
            cost = (s32)menu.codes[menu.list.selected_index] * menu_item_quantity;
            if (counters[96] < cost
                    || (selected_item == 117
                        ? counters[117] + menu_item_quantity * 10 >= 100
                        : counters[selected_item] + menu_item_quantity >= 100)) {
                menu_play_sound_cue(18);
                selection = 0;
            } else {
                menu_play_sound_cue(17);
            }
        }

        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            if (menu.list.entry_count != 0)
                menu_update_item_preview(selected_item);
            menu_render_list(&menu, 15);
            menu_present_frame();
        }
    }

    menu_release_item_model();
    if (result != -1) {
        cost = (s32)menu.codes[menu.list.selected_index] * menu_item_quantity;
        counters[96] -= cost;
        if (result == 117)
            counters[117] += menu_item_quantity * 10;
        else
            counters[result] += (u8)menu_item_quantity;
    }
    menu_exit_display_state(0);
}
ADDRESS(0x8001dc64, 0x16c)
void menu_item_stock_choice_controller(void)
{
    s32 cursor = 0;
    s32 confirmed = 0;
    s32 result = -99;
    s32 selection = -1;
    s32 frame;

    menu_enter_display_state(1);
    for (frame = 0; frame < 2; frame++) {
        menu_frame_begin();
        menu_draw_window(4, 3, cursor, confirmed);
        menu_present_frame();
    }
    menu_play_sound_cue(16);
    input_wait_release();
    for (;;) {
        if (selection != -1 || result != -99)
            input_wait_release();

        switch (selection) {
        case 0:
            menu_buy_masked_stock_items();
            break;
        case 1:
            menu_buy_owned_items();
            break;
        }

        if (result != -99)
            break;
        cursor = menu_poll_choice_input(cursor, 2, &selection, &confirmed, &result);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            menu_draw_window(4, 3, cursor, confirmed);
            menu_present_frame();
        }
    }
    menu_exit_display_state(0);
}

ADDRESS(0x8001ddd0, 0x2d8)
void menu_buy_masked_stock_items(void)
{
    KfItemMenuList menu;
    KfMenuGlyphRow rows[40];
    u8 values[40];
    u32 codes[40];
    u8 indices[40];
    u8 *counters = game_counter_bytes;
    s32 selection = 0;
    s32 result = -99;
    s32 count;
    s32 frame;
    s32 cost;
    u8 selected_item;

    count = menu_collect_masked_item_rows(menu_item_mask_pages[5], rows, values, indices,
        0, 119);
    menu_fill_item_counts_and_prices(counters, values, codes, indices, 0, count, 5);
    menu_list_init(&menu.list, 4, 0);
    menu.list.entry_count = count;
    menu.rows = rows;
    menu.values = values;
    menu.codes = codes;
    menu.list.glyphs_per_entry = 12;

    if (menu_load_item_model(indices[menu.list.selected_index]) != 0)
        return;

    for (;;) {
        if (selection != 0 || result != -99)
            input_wait_release();

        if (selection == 1) {
            result = menu_preview_choice(&menu, 3, 13, selected_item);
            if (result == -1)
                result = -99;
            else
                result = selected_item;
        }
        if (result != -99)
            break;

        menu_update_list_input(&menu.list, indices, &selection, &result);
        selected_item = indices[menu.list.selected_index];
        if (selection == 1) {
            if (player_state.gold < (u32)((s32)menu.codes[menu.list.selected_index]
                    * menu_item_quantity)
                    || counters[selected_item] + menu_item_quantity >= 100) {
                menu_play_sound_cue(18);
                selection = 0;
            } else {
                menu_play_sound_cue(17);
            }
        }

        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            if (menu.list.entry_count != 0)
                menu_update_item_preview(selected_item);
            menu_render_list(&menu, 13);
            menu_present_frame();
        }
    }

    menu_release_item_model();
    if (result != -1) {
        cost = (s32)menu.codes[menu.list.selected_index] * menu_item_quantity;
        player_state.gold -= cost;
        counters[result] += (u8)menu_item_quantity;
    }
}

ADDRESS(0x8001e0a8, 0x2d0)
void menu_buy_owned_items(void)
{
    KfItemMenuList menu;
    KfMenuGlyphRow rows[40];
    u8 values[40];
    u32 codes[40];
    u8 indices[40];
    u8 *counters = game_counter_bytes;
    s32 selection = 0;
    s32 result = -99;
    s32 count;
    s32 frame;
    s32 cost;
    u8 selected_item;

    count = menu_collect_masked_item_rows(counters, rows, values, indices, 99, 109);
    menu_fill_item_prices(codes, indices, 0, count, 4);
    menu_list_init(&menu.list, 4, 1);
    menu.list.entry_count = count;
    menu.rows = rows;
    menu.values = values;
    menu.codes = codes;
    menu.list.glyphs_per_entry = 12;

    if (menu_load_item_model(indices[menu.list.selected_index]) != 0)
        return;

    for (;;) {
        if (selection != 0 || result != -99)
            input_wait_release();

        if (selection == 1) {
            result = menu_preview_choice(&menu, 10, 14, selected_item);
            if (result == -1)
                result = -99;
            else
                result = selected_item;
        }
        if (result != -99)
            break;

        menu_update_list_input(&menu.list, indices, &selection, &result);
        selected_item = indices[menu.list.selected_index];
        if (selection == 1) {
            if (player_state.gold < (u32)((s32)menu.codes[menu.list.selected_index]
                    * menu_item_quantity)
                    || counters[selected_item] + menu_item_quantity >= 100) {
                menu_play_sound_cue(18);
                selection = 0;
            } else {
                menu_play_sound_cue(17);
            }
        }

        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            if (menu.list.entry_count != 0)
                menu_update_item_preview(selected_item);
            menu_render_list(&menu, 14);
            menu_present_frame();
        }
    }

    menu_release_item_model();
    if (result != -1) {
        cost = (s32)menu.codes[menu.list.selected_index] * menu_item_quantity;
        player_state.gold -= cost;
        counters[result] += (u8)menu_item_quantity;
    }
}
ADDRESS(0x8001e378, 0x10c)
s32 menu_poll_choice_input(s32 index, s32 last, s32 *selection, s32 *confirmed,
    s32 *cancelled)
{
    u32 buttons;

    *selection = -1;
    *confirmed = 0;
    input_wait_brief_release();
    buttons = input_read_mark_active();
    if (buttons & PADLup) {
        menu_cursor_animation_direction = 0;
        menu_play_sound_cue(16);
        if (index != 0)
            index--;
        else
            index = last;
    } else if (buttons & PADLdown) {
        menu_cursor_animation_direction = 0;
        menu_play_sound_cue(16);
        if (index != last)
            index++;
        else
            index = 0;
    } else if (buttons & PADRright) {
        menu_play_sound_cue(17);
        *confirmed = 1;
        if (index < last)
            *selection = index;
        else
            *cancelled = -1;
    } else if (buttons & PADRdown) {
        menu_play_sound_cue(18);
        *cancelled = -1;
    }
    return index;
}

ADDRESS(0x8001e484, 0x4c8)
u32 menu_update_list_input(KfMenuList *list, const u8 *item_ids,
    s32 *selection, s32 *result)
{
    u32 buttons;

    *selection = 0;
    input_wait_brief_release();
    buttons = input_read_mark_active();

    if (list->entry_count == 0) {
        if (buttons != 0) {
            menu_play_sound_cue(18);
            *result = -1;
        }
    } else if (buttons & PADLup) {
        menu_play_sound_cue(16);
        if (list->selected_index != 0) {
            list->selected_index--;
            if (list->cursor_row == 0)
                list->scroll_offset--;
            else
                list->cursor_row--;
        } else {
            list->selected_index = list->entry_count - 1;
            if (list->entry_count < list->visible_rows) {
                list->scroll_offset = 0;
                list->cursor_row = list->entry_count - 1;
            } else {
                list->scroll_offset = list->entry_count - list->visible_rows;
                list->cursor_row = list->visible_rows - 1;
            }
        }
        if (item_ids != NULL && menu_load_item_model(item_ids[list->selected_index]) != 0)
            *result = -1;
    } else if (buttons & PADLdown) {
        menu_play_sound_cue(16);
        if (list->selected_index < list->entry_count - 1) {
            list->selected_index++;
            if (list->cursor_row == list->visible_rows - 1)
                list->scroll_offset++;
            else
                list->cursor_row++;
        } else {
            list->selected_index = 0;
            list->scroll_offset = 0;
            list->cursor_row = 0;
        }
        if (item_ids != NULL && menu_load_item_model(item_ids[list->selected_index]) != 0)
            *result = -1;
    } else if (buttons & PADLright) {
        if (menu_item_quantity < 99) {
            menu_play_sound_cue(16);
            menu_item_quantity++;
        }
    } else if (buttons & PADLleft) {
        if (menu_item_quantity > 1) {
            menu_play_sound_cue(16);
            menu_item_quantity--;
        }
    } else if (buttons & PADRright) {
        *selection = 1;
    } else if (buttons & PADRdown) {
        menu_play_sound_cue(18);
        *result = -1;
    }

    if (buttons & PADselect) {
        if (buttons & PADR1) {
            input_press_pending = 0;
            menu_item_preview_rotation.vx += 16;
        }
        if (buttons & PADR2) {
            input_press_pending = 0;
            menu_item_preview_rotation.vx -= 16;
        }
        if (buttons & PADL1) {
            input_press_pending = 0;
            menu_item_preview_rotation.vz += 16;
        }
        if (buttons & PADL2) {
            input_press_pending = 0;
            menu_item_preview_rotation.vz -= 16;
        }
        if (buttons & PADRup) {
            input_press_pending = 0;
            menu_item_preview_rotation_step++;
        }
        if (buttons & PADRleft) {
            input_press_pending = 0;
            menu_item_preview_rotation_step--;
        }
    }

    if (buttons & PADstart) {
        if (buttons & PADR1) {
            input_press_pending = 0;
            menu_item_preview_translation.vx += 16;
        }
        if (buttons & PADR2) {
            input_press_pending = 0;
            menu_item_preview_translation.vx -= 16;
        }
        if (buttons & PADL1) {
            input_press_pending = 0;
            menu_item_preview_translation.vy += 16;
        }
        if (buttons & PADL2) {
            input_press_pending = 0;
            menu_item_preview_translation.vy -= 16;
        }
        if (buttons & PADRup) {
            input_press_pending = 0;
            menu_item_preview_translation.vz += 16;
        }
        if (buttons & PADRleft) {
            if (menu_item_preview_translation.vz > 500)
                menu_item_preview_translation.vz -= 16;
            input_press_pending = 0;
        }
    }

    return buttons;
}

ADDRESS(0x8001e94c, 0x6bc)
void menu_draw_player_status(void)
{
    KfMenuGlyphString heading;
    KfMenuGlyphString amount;
    s32 row_step = 23;

    heading.position.x = 168;
    heading.position.y = 30;
    menu_copy_prefix8(heading.glyphs.codes, menu_header_labels[0].codes);
    menu_draw_string(&menu_sprite_defs[1], &heading);
    amount.position.x = heading.position.x + 77;
    amount.position.y = heading.position.y;
    menu_format_number(player_state.experience, 7, 0, 0, amount.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &amount);

    heading.position.y += row_step;
    menu_copy_prefix8(heading.glyphs.codes, menu_header_labels[1].codes);
    menu_draw_string(&menu_sprite_defs[1], &heading);
    amount.position.y += row_step;
    menu_format_number(player_state.level, 7, 0, 0, amount.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &amount);

    heading.glyphs.codes[0] = 227;
    heading.glyphs.codes[1] = 229;
    heading.glyphs.codes[2] = -1;
    heading.position.y += row_step;
    menu_draw_string(&menu_sprite_defs[1], &heading);
    amount.position.x = heading.position.x + 63;
    amount.position.y += row_step;
    menu_format_number(player_state.vitals.current_hp, 4, 0, 0, amount.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &amount);
    amount.glyphs.codes[0] = 20;
    amount.glyphs.codes[1] = -1;
    amount.position.x += 28;
    menu_draw_number(&menu_sprite_defs[0], &amount);
    amount.position.x += 7;
    menu_format_number(player_state.vitals.maximum_hp, 4, 0, 0, amount.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &amount);

    heading.glyphs.codes[0] = 228;
    heading.position.y += row_step;
    menu_draw_string(&menu_sprite_defs[1], &heading);
    amount.position.x = heading.position.x + 63;
    amount.position.y += row_step;
    menu_format_number(player_state.vitals.current_mp, 4, 0, 0, amount.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &amount);
    amount.glyphs.codes[0] = 20;
    amount.glyphs.codes[1] = -1;
    amount.position.x += 28;
    menu_draw_number(&menu_sprite_defs[0], &amount);
    amount.position.x += 7;
    menu_format_number(player_state.vitals.maximum_mp, 4, 0, 0, amount.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &amount);

    heading.glyphs.codes[0] = 140;
    heading.glyphs.codes[1] = 139;
    heading.glyphs.codes[2] = -1;
    heading.position.y += row_step;
    menu_draw_string(&menu_sprite_defs[1], &heading);
    amount.position.x = heading.position.x + 84;
    amount.position.y = heading.position.y;
    menu_format_number(player_state.physical_power, 6, 0, 0, amount.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &amount);

    heading.glyphs.codes[0] = 120;
    heading.position.y += row_step;
    menu_draw_string(&menu_sprite_defs[1], &heading);
    amount.position.x = heading.position.x + 84;
    amount.position.y = heading.position.y;
    menu_format_number(player_state.magic, 6, 0, 0, amount.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &amount);

    heading.position.y += row_step;
    menu_copy_prefix8(heading.glyphs.codes, menu_header_labels[4].codes);
    menu_draw_string(&menu_sprite_defs[1], &heading);
    amount.position.x = heading.position.x + 56;
    amount.position.y += row_step;
    if (player_state.paralysis_timer != 0)
        menu_copy_prefix12(amount.glyphs.codes, menu_header_labels[5].codes);
    else if (player_state.curse_strength != 0)
        menu_copy_prefix12(amount.glyphs.codes, menu_header_labels[6].codes);
    else if (player_state.slow_timer != 0)
        menu_copy_prefix12(amount.glyphs.codes, menu_header_labels[7].codes);
    else if (player_state.poison_timer != 0)
        menu_copy_prefix12(amount.glyphs.codes, menu_header_labels[8].codes);
    else if (player_state.darkness_phase != 0)
        menu_copy_prefix12(amount.glyphs.codes, menu_header_labels[9].codes);
    else
        menu_copy_prefix12(amount.glyphs.codes, menu_header_labels[10].codes);
    menu_draw_string(&menu_sprite_defs[1], &amount);

    heading.position.y += row_step;
    menu_copy_prefix10(heading.glyphs.codes, menu_header_labels[11].codes);
    menu_draw_string(&menu_sprite_defs[1], &heading);
    amount.position.x = heading.position.x + 77;
    amount.position.y += row_step;
    menu_format_number(player_state.gold, 7, 0, 0, amount.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &amount);
    menu_draw_nine_slice_panel(161, 14, 140, 205, 1, 2);
}

ADDRESS(0x8001f008, 0x790)
void menu_draw_combat_attributes(void)
{
    KfMenuGlyphString label;
    KfMenuGlyphString number;

    label.position.x = 24;
    label.position.y = 28;
    menu_copy_prefix8(label.glyphs.codes, menu_header_labels[2].codes);
    menu_draw_string(&menu_sprite_defs[1], &label);

    label.glyphs.codes[0] = 255;
    label.glyphs.codes[1] = 263;
    label.glyphs.codes[2] = 106;
    label.glyphs.codes[3] = -1;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.x = label.position.x + 84;
    number.position.y = label.position.y;
    menu_format_number(player_state.attack_components[0], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.glyphs.codes[1] = 264;
    label.glyphs.codes[2] = 81;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.y = label.position.y;
    menu_format_number(player_state.attack_components[1], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.glyphs.codes[1] = 265;
    label.glyphs.codes[2] = 76;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.y = label.position.y;
    menu_format_number(player_state.attack_components[2], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.glyphs.codes[1] = 266;
    label.glyphs.codes[2] = 88;
    label.glyphs.codes[3] = 120;
    label.glyphs.codes[4] = 121;
    label.glyphs.codes[5] = -1;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.y = label.position.y;
    menu_format_number(player_state.attack_components[3], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.glyphs.codes[1] = 170;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.y = label.position.y;
    menu_format_number(player_state.attack_components[4], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.glyphs.codes[1] = 187;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.y = label.position.y;
    menu_format_number(player_state.attack_components[5], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.glyphs.codes[1] = 188;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.y = label.position.y;
    menu_format_number(player_state.attack_components[6], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.glyphs.codes[1] = 141;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.y = label.position.y;
    menu_format_number(player_state.attack_components[7], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.position.x = 168;
    label.position.y = 28;
    menu_copy_prefix8(label.glyphs.codes, menu_header_labels[3].codes);
    menu_draw_string(&menu_sprite_defs[1], &label);

    label.glyphs.codes[0] = 255;
    label.glyphs.codes[1] = 263;
    label.glyphs.codes[2] = 106;
    label.glyphs.codes[3] = -1;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.x = label.position.x + 84;
    number.position.y = label.position.y;
    menu_format_number(player_state.combat_components[0], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.glyphs.codes[1] = 264;
    label.glyphs.codes[2] = 81;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.y = label.position.y;
    menu_format_number(player_state.combat_components[1], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.glyphs.codes[1] = 265;
    label.glyphs.codes[2] = 76;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.y = label.position.y;
    menu_format_number(player_state.combat_components[2], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.glyphs.codes[1] = 136;
    label.glyphs.codes[2] = -1;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.y = label.position.y;
    menu_format_number(player_state.combat_components[3], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.glyphs.codes[1] = 120;
    label.glyphs.codes[2] = 88;
    label.glyphs.codes[3] = 120;
    label.glyphs.codes[4] = 121;
    label.glyphs.codes[5] = -1;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.y = label.position.y;
    menu_format_number(player_state.combat_components[4], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.glyphs.codes[1] = 170;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.y = label.position.y;
    menu_format_number(player_state.combat_components[5], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.glyphs.codes[1] = 187;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.y = label.position.y;
    menu_format_number(player_state.combat_components[6], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.glyphs.codes[1] = 188;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.y = label.position.y;
    menu_format_number(player_state.combat_components[7], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    label.glyphs.codes[1] = 141;
    label.position.y += 18;
    menu_draw_string(&menu_sprite_defs[1], &label);
    number.position.y = label.position.y;
    menu_format_number(player_state.combat_components[8], 6, 0, 0,
        number.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &number);

    menu_draw_nine_slice_panel(17, 14, 140, 205, 1, 2);
    menu_draw_nine_slice_panel(161, 14, 140, 205, 1, 2);
}

ADDRESS(0x8001f798, 0x120)
void menu_draw_options_rows(KfMenuGlyphString *left, KfMenuGlyphString *right,
    const u8 *selected)
{
    s32 left_y = left->position.y;
    s32 right_y = right->position.y;
    s32 row;

    row = 0;
    do {
        if (*selected == 1) {
            menu_blit_sprite_fixed_clut(&menu_sprite_defs[2], &left->position);
            menu_blit_sprite_translucent(&menu_sprite_defs[4], &left->position);
            menu_blit_sprite_translucent(&menu_sprite_defs[3], &right->position);
        } else {
            menu_blit_sprite_fixed_clut(&menu_sprite_defs[2], &right->position);
            menu_blit_sprite_translucent(&menu_sprite_defs[3], &left->position);
            menu_blit_sprite_translucent(&menu_sprite_defs[4], &right->position);
        }
        menu_draw_string(&menu_sprite_defs[1], left);
        menu_draw_string(&menu_sprite_defs[1], right);
        left->position.y += 26;
        right->position.y += 26;
        selected++;
        row++;
    } while (row < 6);
    left->position.y = left_y;
    right->position.y = right_y;
}

ADDRESS(0x8001f8b8, 0x2d4)
s32 menu_preview_choice(void *list_state, s32 label_kind,
    s32 render_mode, u8 item_id)
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
        goto labels_ready;
    finished:
        input_wait_release();
        return result;
    } else {
        labels[1].glyphs.codes[0] = 99;
        labels[1].glyphs.codes[1] = 97;
        labels[1].glyphs.codes[2] = 106;
    }
labels_ready:
    labels[1].glyphs.codes[3] = -1;

    for (;;) {
        if (result != -99) {
            goto finished;
        }
        input_wait_brief_release();
        buttons = input_read_mark_active();
        confirmed = 0;
        if ((buttons & PADLup) || (buttons & PADLdown)) {
            menu_cursor_animation_direction = 0;
            menu_play_sound_cue(16);
            if (choice != 0)
                choice = 0;
            else
                choice = 1;
        } else if (buttons & PADRright) {
            menu_play_sound_cue(17);
            confirmed = 1;
            result = -choice;
        } else if (buttons & PADRdown) {
            menu_play_sound_cue(18);
            result = -1;
        }

        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            menu_update_item_preview((u8)item_id);
            menu_render_list(list_state, render_mode);
            menu_draw_two_option(&labels[0], &labels[1], choice, confirmed);
            menu_present_frame();
        }
    }
}

ADDRESS(0x8001fb8c, 0x108)
void menu_draw_window(s32 window_kind, s32 count, s32 highlight, s32 confirmation)
{
    const KfMenuWindowLayout *layout = &menu_window_layouts[window_kind];
    const KfMenuGlyphString *row = &layout->rows[0];
    s32 index;

    if (layout->title.position.x != 0) {
        menu_blit_sprite_translucent(&menu_sprite_defs[KF_MENU_SPRITE_PANEL_BACKGROUND], &layout->title.position);
        menu_draw_string(&menu_sprite_defs[1], &layout->title);
    }
    if (count > 0) {
        index = 0;
        do {
            if (index == highlight && confirmation == KF_MENU_CONFIRM_REQUESTED)
                menu_blit_sprite_translucent(&menu_sprite_defs[KF_MENU_SPRITE_CONFIRMED_ROW], &row->position);
            else
                menu_blit_sprite_translucent(&menu_sprite_defs[KF_MENU_SPRITE_PANEL_BACKGROUND], &row->position);
            if (index == highlight)
                menu_blit_sprite(&menu_sprite_defs[2], &row->position);
            menu_draw_string(&menu_sprite_defs[1], row);
            row++;
            index++;
        } while (index < count);
    }
}
ADDRESS(0x8001fc94, 0xab4)
void menu_render_list(const void *list_state, s32 render_mode)
{
    const KfMenuRenderList *view = list_state;
    const KfMenuList *list = &view->list;
    const KfMenuSpriteDef *sprite;
    const s16 *row_glyphs = view->row_glyphs;
    const s16 *detail_codes = (const s16 *)view->detail_rows;
    const s32 *number_values = view->number_values;
    const u8 *byte_values = view->byte_values;
    KfMenuGlyphString text;
    s16 *out_codes;
    s32 row;
    s32 code;
    s32 value;
    s32 y;

    if (list->title.position.x != 0) {
        menu_blit_sprite_translucent(&menu_sprite_defs[KF_MENU_SPRITE_PANEL_BACKGROUND], &list->title.position);
        menu_draw_string(&menu_sprite_defs[1], &list->title);
    }

    detail_codes += list->scroll_offset * 10;
    row_glyphs += list->scroll_offset * list->glyphs_per_entry;
    number_values += list->scroll_offset;
    byte_values += list->scroll_offset;

    {
        for (row = 0; row < list->visible_rows && row < list->entry_count; row++) {
            text.position.x = list->list_x + 5;
            text.position.y = list->list_y + 5 + row * 14;
            out_codes = text.glyphs.codes;
            for (code = 0; code < list->glyphs_per_entry; code++)
                *out_codes++ = *row_glyphs++;
            menu_draw_string(&menu_sprite_defs[1], &text);

            if (render_mode == 3) {
                text.position.x += 84;
                out_codes = text.glyphs.codes;
                for (code = 0; code < 10; code++)
                    *out_codes++ = *detail_codes++;
                *out_codes = -1;
                menu_draw_string(&menu_sprite_defs[1], &text);
            }

            if (((u32)render_mode - 10u) < 6u && render_mode != 12) {
                value = *number_values;
                text.position.x += 140;
                if (render_mode == 15)
                    menu_format_number(value, 6, 0, 6, text.glyphs.codes);
                else
                    menu_format_number(value, 6, 0, 2, text.glyphs.codes);
                menu_draw_number(&menu_sprite_defs[0], &text);
                number_values++;
            }

            if (render_mode == 2 || render_mode == 4) {
                value = *number_values++;
                if (value != -1) {
                    text.position.x += 189;
                    menu_format_number(value, 3, 0, 3, text.glyphs.codes);
                    menu_draw_number(&menu_sprite_defs[0], &text);
                }
            }

            if (((u32)render_mode - 8u) < 2u) {
                value = *number_values++;
                if (value != -1) {
                    text.position.x += 98;
                    menu_format_number(value, 7, 0, 4, text.glyphs.codes);
                    menu_draw_number(&menu_sprite_defs[0], &text);
                }
                value = *byte_values++;
                if (value != 0xff) {
                    text.position.x += 91;
                    menu_format_number(value, 3, 0, 5, text.glyphs.codes);
                    menu_draw_number(&menu_sprite_defs[0], &text);
                }
            }

            if (render_mode != 2 && render_mode != 3 && render_mode != 4
                && ((u32)render_mode - 8u) >= 2u && render_mode != 16) {
                value = *byte_values++;
                if (value != 0xff) {
                    text.position.x = list->list_x + 208;
                    menu_format_number(value, 2, 0, 1, text.glyphs.codes);
                    menu_draw_number(&menu_sprite_defs[0], &text);
                }
            }
            if (render_mode == 16) {
                value = *byte_values++;
                if (value != 0xff) {
                    text.position.x += 203;
                    menu_format_number(value, 2, 0, 1, text.glyphs.codes);
                    menu_draw_number(&menu_sprite_defs[0], &text);
                }
                value = *number_values++;
                if (value != -1) {
                    text.position.x += 189;
                    menu_format_number(value, 3, 0, 3, text.glyphs.codes);
                    menu_draw_number(&menu_sprite_defs[0], &text);
                }
            }
        }
    }

    sprite = &menu_sprite_defs[KF_MENU_SPRITE_LIST_TOP];
    primitive_buffer_begin_poly_ft4();
    setRGB0(current_poly_ft4, 255, 255, 255);
    current_poly_ft4->tpage = sprite->tpage;
    current_poly_ft4->clut = sprite->clut;
    setXYWH(current_poly_ft4, list->list_x, list->list_y,
        sprite->width, sprite->height);
    setUVWH(current_poly_ft4, sprite->u, sprite->v,
        sprite->width, sprite->height);
    SetSemiTrans((void *)current_poly_ft4, 1);
    primitive_buffer_commit_poly_ft4(KF_MENU_WIDGET_OT_DEPTH);

    row = 0;
    if (row < list->visible_rows) {
        y = 0;
        do {
            sprite = row == list->cursor_row
                ? &menu_sprite_defs[KF_MENU_SPRITE_LIST_SELECTED_ROW]
                : &menu_sprite_defs[KF_MENU_SPRITE_LIST_ROW];
            row++;
            primitive_buffer_begin_poly_ft4();
            setRGB0(current_poly_ft4, 255, 255, 255);
            current_poly_ft4->tpage = sprite->tpage;
            current_poly_ft4->clut = sprite->clut;
            setXYWH(current_poly_ft4, list->list_x, list->list_y + y + 5,
                sprite->width, sprite->height);
            setUVWH(current_poly_ft4, sprite->u, sprite->v,
                sprite->width, sprite->height);
            SetSemiTrans((void *)current_poly_ft4, 1);
            primitive_buffer_commit_poly_ft4(KF_MENU_WIDGET_OT_DEPTH);
            y += 14;
        } while (row < list->visible_rows);
    }

    sprite = &menu_sprite_defs[KF_MENU_SPRITE_LIST_BOTTOM];
    primitive_buffer_begin_poly_ft4();
    setRGB0(current_poly_ft4, 255, 255, 255);
    current_poly_ft4->tpage = sprite->tpage;
    current_poly_ft4->clut = sprite->clut;
    setXYWH(current_poly_ft4, list->list_x,
        list->list_y + list->visible_rows * 14 + 5,
        sprite->width, sprite->height);
    setUVWH(current_poly_ft4, sprite->u, sprite->v,
        sprite->width, sprite->height);
    SetSemiTrans((void *)current_poly_ft4, 1);
    primitive_buffer_commit_poly_ft4(KF_MENU_WIDGET_OT_DEPTH);

    if (((u32)render_mode - 10u) < 2u || ((u32)render_mode - 13u) < 2u)
        menu_draw_status_counters(1);
    else if (render_mode == 15)
        menu_draw_status_counters(3);
    if (((u32)render_mode - 8u) < 2u)
        menu_render_list_mode_8_9_noop();
}
enum { KF_PREVIEW_ANGLE_MASK = 0xfff };

ADDRESS(0x80020748, 0xf4)
void menu_draw_two_option(const KfMenuGlyphString *accept_label,
    const KfMenuGlyphString *decline_label, s32 selected_choice, s32 confirmation)
{
    if (selected_choice == KF_MENU_CHOICE_ACCEPT) {
        menu_blit_sprite(&menu_sprite_defs[KF_MENU_SPRITE_SELECTION_CURSOR],
            &accept_label->position);
    } else {
        menu_blit_sprite(&menu_sprite_defs[KF_MENU_SPRITE_SELECTION_CURSOR],
            &decline_label->position);
    }
    if (confirmation == KF_MENU_CONFIRM_REQUESTED) {
        if (selected_choice == KF_MENU_CHOICE_ACCEPT) {
            menu_blit_sprite_translucent(&menu_sprite_defs[KF_MENU_SPRITE_OPTION_HIGHLIGHT],
                &accept_label->position);
            menu_blit_sprite_translucent(&menu_sprite_defs[KF_MENU_SPRITE_OPTION_BACKGROUND],
                &decline_label->position);
        } else {
            menu_blit_sprite_translucent(&menu_sprite_defs[KF_MENU_SPRITE_OPTION_BACKGROUND],
                &accept_label->position);
            menu_blit_sprite_translucent(&menu_sprite_defs[KF_MENU_SPRITE_OPTION_HIGHLIGHT],
                &decline_label->position);
        }
    } else {
        menu_blit_sprite_translucent(&menu_sprite_defs[KF_MENU_SPRITE_OPTION_BACKGROUND],
            &accept_label->position);
        menu_blit_sprite_translucent(&menu_sprite_defs[KF_MENU_SPRITE_OPTION_BACKGROUND],
            &decline_label->position);
    }
    menu_draw_string(&menu_sprite_defs[KF_MENU_SPRITE_GLYPH_ATLAS], accept_label);
    menu_draw_string(&menu_sprite_defs[KF_MENU_SPRITE_GLYPH_ATLAS], decline_label);
}

ADDRESS(0x8002083c, 0x154)
void menu_update_item_preview(s32 item_id)
{
    MATRIX rotation;
    MATRIX light;
    MATRIX lit;
    MATRIX color;

    if (player_state.item_preview_enabled == 0 || (u8)item_id == 0xff) {
        return;
    }

    rotation.t[0] = menu_item_preview_translation.vx;
    rotation.t[1] = menu_item_preview_translation.vy;
    rotation.t[2] = menu_item_preview_translation.vz;
    menu_item_preview_rotation.vy += menu_item_preview_rotation_step;
    menu_item_preview_rotation.vx &= KF_PREVIEW_ANGLE_MASK;
    menu_item_preview_rotation.vy &= KF_PREVIEW_ANGLE_MASK;
    menu_item_preview_rotation.vz &= KF_PREVIEW_ANGLE_MASK;
    RotMatrix(&menu_item_preview_rotation, &rotation);

    light.m[0][0] = 0x800;
    light.m[0][1] = 0x800;
    light.m[0][2] = -0x800;
    light.m[1][0] = 0;
    light.m[1][1] = 0;
    light.m[1][2] = -0x800;
    light.m[2][0] = 0x1000;
    light.m[2][1] = 0;
    light.m[2][2] = -0x800;
    MulMatrix0(&light, &rotation, &lit);
    SetLightMatrix(&lit);

    color.m[0][0] = 0x1000;
    color.m[0][1] = 0x1000;
    color.m[0][2] = 0x1000;
    color.m[1][0] = 0x1000;
    color.m[1][1] = 0x1000;
    color.m[1][2] = 0x1000;
    color.m[2][0] = 0x1000;
    color.m[2][1] = 0x1000;
    color.m[2][2] = 0x1000;
    SetColorMatrix(&color);
    SetRotMatrix(&rotation);
    SetTransMatrix(&rotation);
    menu_render_item_model();
    current_poly_ft4 = (POLY_FT4 *)game_graphics_runtime.display_state.primitive_buffer->cursor;
}

ADDRESS(0x80020990, 0x1c0)
void menu_draw_status_counters(s32 kind)
{
    KfMenuGlyphString heading;
    KfMenuGlyphString amount;

    heading.position.x = 183;
    heading.position.y = 33;
    if (kind == 3) {
        heading.glyphs.codes[0] = 7;
        heading.glyphs.codes[1] = 41;
        heading.glyphs.codes[2] = 12;
        heading.glyphs.codes[3] = 15;
        heading.glyphs.codes[4] = 42;
        heading.glyphs.codes[5] = KF_MENU_TEXT_END;
    } else {
        heading.glyphs.codes[0] = 221;
        heading.glyphs.codes[1] = 222;
        heading.glyphs.codes[2] = 209;
        heading.glyphs.codes[3] = KF_MENU_TEXT_END;
    }
    menu_blit_sprite_translucent(&menu_sprite_defs[KF_MENU_SPRITE_PANEL_BACKGROUND], &heading.position);
    menu_draw_string(&menu_sprite_defs[1], &heading);

    amount.position.x = heading.position.x + 56;
    amount.position.y = heading.position.y;
    if (kind == 3)
        menu_format_number(game_counter_bytes[0x60], 7, 0, 6, amount.glyphs.codes);
    else
        menu_format_number(player_state.gold, 7, 0, 2, amount.glyphs.codes);
    menu_draw_number(&menu_sprite_defs[0], &amount);

    if (kind != 2) {
        heading.position.x = 183;
        heading.position.y = 61;
        heading.glyphs.codes[0] = 202;
        heading.glyphs.codes[1] = 203;
        heading.glyphs.codes[2] = KF_MENU_TEXT_END;
        menu_blit_sprite_translucent(&menu_sprite_defs[KF_MENU_SPRITE_PANEL_BACKGROUND], &heading.position);
        menu_draw_string(&menu_sprite_defs[1], &heading);
        amount.position.x = heading.position.x + 91;
        amount.position.y = heading.position.y;
        menu_format_number(menu_item_quantity, 2, 0, 1, amount.glyphs.codes);
        menu_draw_number(&menu_sprite_defs[0], &amount);
    }
}
ADDRESS(0x80020b50, 0x1d0)
void menu_blit_sprite_translucent(
    const KfMenuSpriteDef *sprite, const KfMenuPoint *position)
{
    primitive_buffer_begin_poly_ft4();
    setRGB0(current_poly_ft4, 0xff, 0xff, 0xff);
    current_poly_ft4->tpage = sprite->tpage;
    current_poly_ft4->clut = sprite->clut;
    setXYWH(current_poly_ft4,
        position->x - KF_MENU_TRANSLUCENT_SPRITE_OFFSET,
        position->y - KF_MENU_TRANSLUCENT_SPRITE_OFFSET,
        sprite->width, sprite->height);
    setUVWH(current_poly_ft4, sprite->u, sprite->v, sprite->width, sprite->height);
    SetSemiTrans((void *)current_poly_ft4, 1);
    primitive_buffer_commit_poly_ft4(KF_MENU_WIDGET_OT_DEPTH);
}

enum {
    KF_MENU_CURSOR_COLOR = 0x40,
    KF_MENU_CURSOR_LEFT_MARGIN = 8,
    KF_MENU_CURSOR_TEXTURE_PAGE_BASE = 500,
    KF_MENU_CURSOR_CLUT_BASE = 36
};

ADDRESS(0x80020d20, 0x1d8)
void menu_blit_sprite(const KfMenuSpriteDef *sprite, const KfMenuPoint *position)
{
    s32 frame = menu_cursor_animation_frame;

    primitive_buffer_begin_poly_ft4();
    setRGB0(current_poly_ft4,
        KF_MENU_CURSOR_COLOR, KF_MENU_CURSOR_COLOR, KF_MENU_CURSOR_COLOR);
    current_poly_ft4->tpage = sprite->tpage;
    current_poly_ft4->clut = ((frame + KF_MENU_CURSOR_TEXTURE_PAGE_BASE) << 6)
        + KF_MENU_CURSOR_CLUT_BASE;
    setXYWH(current_poly_ft4,
        position->x - (sprite->width + KF_MENU_CURSOR_LEFT_MARGIN),
        position->y, sprite->width, sprite->height);
    setUVWH(current_poly_ft4,
        sprite->u + (frame << 4), sprite->v, sprite->width, sprite->height);
    primitive_buffer_commit_poly_ft4(KF_MENU_WIDGET_OT_DEPTH);
}

enum {
    KF_MENU_FIXED_CURSOR_COLOR = 0x38,
    KF_MENU_FIXED_CURSOR_CLUT = 0x7d24,
    KF_MENU_FIXED_CURSOR_LEFT_MARGIN = 7
};

ADDRESS(0x80020ef8, 0x1b4)
void menu_blit_sprite_fixed_clut(
    const KfMenuSpriteDef *sprite, const KfMenuPoint *position)
{
    primitive_buffer_begin_poly_ft4();
    setRGB0(current_poly_ft4,
        KF_MENU_FIXED_CURSOR_COLOR,
        KF_MENU_FIXED_CURSOR_COLOR,
        KF_MENU_FIXED_CURSOR_COLOR);
    current_poly_ft4->tpage = sprite->tpage;
    current_poly_ft4->clut = KF_MENU_FIXED_CURSOR_CLUT;
    setXYWH(current_poly_ft4,
        position->x - (sprite->width + KF_MENU_FIXED_CURSOR_LEFT_MARGIN),
        position->y, sprite->width, sprite->height);
    setUVWH(current_poly_ft4,
        sprite->u, sprite->v, sprite->width, sprite->height);
    primitive_buffer_commit_poly_ft4(KF_MENU_WIDGET_OT_DEPTH);
}
static inline void menu_begin_text_glyph(
    const KfMenuSpriteDef *font, const KfMenuGlyphString *string, s32 x_offset)
{
    primitive_buffer_begin_poly_ft4();
    current_poly_ft4->tpage = font->tpage;
    current_poly_ft4->clut = font->clut;
    setXYWH(current_poly_ft4, string->position.x + x_offset,
        string->position.y, font->width, font->height);
}

ADDRESS(0x800210ac, 0x464)
void menu_draw_string(const KfMenuSpriteDef *font, const KfMenuGlyphString *string)
{
    const s16 *code = string->glyphs.codes;
    s32 i;
    s32 x_offset;

    for (i = 0; *code != KF_MENU_TEXT_END; code++, i++) {
        u32 glyph;

        x_offset = i * KF_MENU_GLYPH_ADVANCE;
        menu_begin_text_glyph(font, string, x_offset);
        glyph = *code & KF_MENU_TEXT_GLYPH_MASK;
        setUVWH(current_poly_ft4,
            (glyph % KF_MENU_FONT_COLUMNS) * KF_MENU_FONT_CELL_WIDTH,
            (glyph / KF_MENU_FONT_COLUMNS) * KF_MENU_FONT_CELL_HEIGHT,
            font->width, font->height);
        primitive_buffer_commit_poly_ft4(KF_MENU_CONTENT_OT_DEPTH);

        if (*code & KF_MENU_TEXT_DAKUTEN) {
            menu_begin_text_glyph(font, string, x_offset);
            setUVWH(current_poly_ft4, KF_MENU_DAKUTEN_U, KF_MENU_KANA_MARK_V,
                font->width, font->height);
            primitive_buffer_commit_poly_ft4(KF_MENU_CONTENT_OT_DEPTH);
        }

        if (*code & KF_MENU_TEXT_HANDAKUTEN) {
            menu_begin_text_glyph(font, string, x_offset);
            setUVWH(current_poly_ft4, KF_MENU_HANDAKUTEN_U, KF_MENU_KANA_MARK_V,
                font->width, font->height);
            primitive_buffer_commit_poly_ft4(KF_MENU_CONTENT_OT_DEPTH);
        }
    }
}

ADDRESS(0x80021510, 0x2e0)
void menu_draw_number(const KfMenuSpriteDef *font, const KfMenuGlyphString *string)
{
    const s16 *code;
    s32 x_offset;

    code = string->glyphs.codes;
    if (*code == KF_MENU_TEXT_END) {
        return;
    }
    x_offset = 0;
    do {
        primitive_buffer_begin_poly_ft4();
        current_poly_ft4->tpage = font->tpage;
        current_poly_ft4->clut = font->clut;
        setXYWH(current_poly_ft4,
            string->position.x + x_offset, string->position.y,
            font->width, font->height);
        if (*code < KF_MENU_NUMBER_COLUMN_ROWS) {
            setUVWH(current_poly_ft4,
                font->u, *code * KF_MENU_FONT_CELL_HEIGHT,
                font->width, font->height);
        } else {
            setUVWH(current_poly_ft4,
                (u8)(font->u + KF_MENU_NUMBER_COLUMN_WIDTH),
                (*code - KF_MENU_NUMBER_COLUMN_ROWS) * KF_MENU_FONT_CELL_HEIGHT,
                font->width, font->height);
        }
        primitive_buffer_commit_poly_ft4(KF_MENU_CONTENT_OT_DEPTH);
        code++;
        x_offset += KF_MENU_DIGIT_ADVANCE;
    } while (*code != KF_MENU_TEXT_END);
}
ADDRESS(0x800217f0, 0x270)
void menu_draw_nine_slice_panel(s32 x, s32 y, s32 width, s32 height,
    s32 overlap_x, s32 overlap_y)
{
    enum { MENU_PANEL_BASE_SPAN = 94 };
    const KfMenuSpriteDef *tile;
    u16 clut;
    s32 row;
    s32 column;
    s32 tile_index;
    s32 draw_x;
    s32 draw_y;
    s32 draw_width;
    s32 draw_height;

    width -= MENU_PANEL_BASE_SPAN;
    height -= MENU_PANEL_BASE_SPAN;
    tile_index = 9;
    tile = &menu_sprite_defs[KF_MENU_SPRITE_PANEL_TOP_LEFT];
    draw_y = y;
    for (row = 0; row < 3; row++) {
        draw_height = tile->height;
        if (row == 1) {
            draw_y += 33;
            draw_height += height + overlap_y;
        } else if (row == 2) {
            draw_y += 28 + height;
        }
        draw_x = x;
        for (column = 0; column < 3; column++) {
            draw_width = tile->width;
            if (column == 1) {
                draw_x += 33;
                draw_width += width + overlap_x;
            } else if (column == 2) {
                draw_x += 28 + width;
            }
            primitive_buffer_begin_poly_ft4();
            setRGB0(current_poly_ft4, 0xff, 0xff, 0xff);
            SetSemiTrans((void *)current_poly_ft4, 1);
            current_poly_ft4->tpage = tile->tpage;
            clut = tile->clut;
            setXYWH(current_poly_ft4, draw_x, draw_y, draw_width, draw_height);
            current_poly_ft4->clut = clut;
            setUVWH(current_poly_ft4, tile->u, tile->v, tile->width, tile->height);
            primitive_buffer_commit_poly_ft4(KF_MENU_WIDGET_OT_DEPTH);
            tile_index++;
            tile = &menu_sprite_defs[tile_index + 2];
        }
    }
}

ADDRESS(0x80021a60, 0x8)
void menu_render_list_mode_8_9_noop(void)
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

enum {
    KF_MENU_PRIMITIVE_BRIGHTNESS = 0x68
};

ADDRESS(0x80021f10, 0x50)
void primitive_buffer_begin_poly_ft4(void)
{
    SetPolyFT4(current_poly_ft4);
    setRGB0(current_poly_ft4,
        KF_MENU_PRIMITIVE_BRIGHTNESS,
        KF_MENU_PRIMITIVE_BRIGHTNESS,
        KF_MENU_PRIMITIVE_BRIGHTNESS);
}

ADDRESS(0x80021f60, 0x50)
void primitive_buffer_commit_poly_ft4(s32 depth)
{
    AddPrim(&game_graphics_runtime.display_state.ordering_table[depth], current_poly_ft4);
    current_poly_ft4++;
    game_graphics_runtime.display_state.primitive_buffer->cursor = (u8 *)current_poly_ft4;
}

ADDRESS(0x80021fb0, 0xa8)
void menu_list_init(KfMenuList *list, s32 window_kind, s32 row)
{
    KfMenuGlyphString *source;
    s16 *title_codes;
    s16 *source_codes;
    s32 i;

    list->title.position.x = 17;
    list->title.position.y = 19;
    title_codes = list->title.glyphs.codes;
    source = &menu_window_layouts[window_kind].rows[row];
    source_codes = source->glyphs.codes;
    for (i = 0; i < KF_MENU_LIST_TITLE_COPY_GLYPHS; i++) {
        *title_codes++ = *source_codes++;
    }
    list->list_x = 0x2a;
    list->list_y = 0x9f;
    list->entry_count = 0;
    list->visible_rows = 4;
    list->scroll_offset = 0;
    list->selected_index = 0;
    list->cursor_row = 0;
    list->glyphs_per_entry = 10;
}

enum {
    KF_MENU_FORMAT_BLANK = 10,
    KF_MENU_FORMAT_STYLE_SINGLE_PREFIX = 1,
    KF_MENU_FORMAT_STYLE_TRAILING_13 = 2,
    KF_MENU_FORMAT_STYLE_PAIR_15_16 = 3,
    KF_MENU_FORMAT_STYLE_TRIPLE_12_18_16 = 4,
    KF_MENU_FORMAT_STYLE_PAIR_14_17 = 5,
    KF_MENU_FORMAT_STYLE_TRAILING_11 = 6
};

ADDRESS(0x80022058, 0x190)
void menu_format_number(s32 value, s32 count, s32 padding_mode, s32 style, s16 *out)
{
    s32 i;
    s16 blank;
    s16 *cursor;

    if ((u32)(style - 1) < 2 || style == KF_MENU_FORMAT_STYLE_TRAILING_11) {
        count++;
    } else if (style == KF_MENU_FORMAT_STYLE_PAIR_15_16
        || style == KF_MENU_FORMAT_STYLE_PAIR_14_17) {
        count += 2;
    } else if (style == KF_MENU_FORMAT_STYLE_TRIPLE_12_18_16) {
        count += 3;
    }

    i = 0;
    blank = padding_mode == 0 ? KF_MENU_FORMAT_BLANK : 0;
    if (count > 0) {
        cursor = out;
        do {
            *cursor++ = blank;
            i++;
        } while (i < count);
    }
    out[count] = KF_MENU_TEXT_END;

    if (style == KF_MENU_FORMAT_STYLE_SINGLE_PREFIX) {
        out[0] = 19;
    } else if (style == KF_MENU_FORMAT_STYLE_TRAILING_13) {
        out[count - 1] = 13;
        count--;
    } else if (style == KF_MENU_FORMAT_STYLE_PAIR_15_16) {
        out[0] = 15;
        out[1] = 16;
    } else if (style == KF_MENU_FORMAT_STYLE_TRIPLE_12_18_16) {
        out[0] = 12;
        out[1] = 18;
        out[2] = 16;
    } else if (style == KF_MENU_FORMAT_STYLE_PAIR_14_17) {
        out[0] = 14;
        out[1] = 17;
    } else if (style == KF_MENU_FORMAT_STYLE_TRAILING_11) {
        out[count - 1] = 11;
        count--;
    }

    for (i = count - 1; i >= 0; i--) {
        out[i] = value % 10;
        value /= 10;
        if (value == 0) {
            i = -1;
        }
    }
}

ADDRESS(0x800221e8, 0xd4)
s32 menu_load_item_model(u8 item_id)
{
    u8 *allocation;

    menu_item_quantity = 1;
    if (player_state.item_preview_enabled == 0)
        return 0;

    menu_release_item_model();
    if (item_id != 0xff) {
        allocation = memory_allocate(cd_archive_entry_extent(6, item_id, NULL));
        cd_archive_read(6, item_id, (u_long *)allocation);
        tmd_register(KF_TMD_SLOT_MENU_ITEM, (KfTmdHeader *)allocation);
        menu_item_model_allocation_pending = KF_MENU_MODEL_ALLOCATED;
    }
    setVector(&menu_item_preview_translation, 0, 0, 1000);
    setVector(&menu_item_preview_rotation, 0, 0, 0);
    menu_item_preview_rotation_step = 4;
    return 0;
}

ADDRESS(0x800222bc, 0x44)
void menu_release_item_model(void)
{
    if (menu_item_model_allocation_pending == KF_MENU_MODEL_ALLOCATED) {
        memory_free((u8 *)game_graphics_runtime.tmd_state.slots[KF_TMD_SLOT_MENU_ITEM]);
        menu_item_model_allocation_pending = KF_MENU_MODEL_RELEASED;
    }
}
ADDRESS(0x80022300, 0x94)
void menu_play_sound_cue(s32 cue)
{
    if (cue == 16) {
        audio_key_on(16, 48, 48, 0);
        SsSeqCalledTbyT();
    } else if ((u32)cue - 17u < 2u) {
        audio_key_on(cue, 48, 48, 0);
        SsSeqCalledTbyT();
    } else if (cue == 13) {
        audio_key_on(13, 0, 58, 0);
        VSync(0);
        VSync(0);
        audio_key_on(13, 58, 0, 0);
    }
}

ADDRESS(0x80022394, 0x38)
u32 input_read_mark_active(void)
{
    u32 buttons = PadRead(1);
    if (buttons != 0) {
        input_press_pending = 1;
    }
    return buttons;
}

ADDRESS(0x800223cc, 0x6c)
void input_wait_brief_release(void)
{
    s32 polls;

    if (input_press_pending == 1) {
        input_press_pending = 0;
        for (polls = 0; PadRead(1) != 0;) {
            if (polls++ < 6) {
                VSync(0);
            } else {
                menu_cursor_animation_frame = 0;
                break;
            }
        }
    }
}
