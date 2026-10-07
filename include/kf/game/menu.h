#ifndef KF_GAME_MENU_H
#define KF_GAME_MENU_H

#include <kf/lib/bool.h>
#include <kf/lib/offsetof.h>
#include <kf/lib/types.h>
#include <psyq/sdk.h>

struct DIRENTRY;

enum {
    KF_MENU_GLYPHS_PER_ROW = 12,
    KF_MENU_WINDOW_ROW_CAPACITY = 10,
    KF_MENU_WINDOW_COUNT = 8,
    KF_MENU_LIST_TITLE_COPY_GLYPHS = 10,
    KF_MENU_SPRITE_COUNT = 20
};

enum {
    KF_MENU_SPRITE_NUMBER_ATLAS = 0,
    KF_MENU_SPRITE_GLYPH_ATLAS = 1,
    KF_MENU_SPRITE_SELECTION_CURSOR = 2,
    KF_MENU_SPRITE_OPTION_BACKGROUND = 3,
    KF_MENU_SPRITE_OPTION_HIGHLIGHT = 4,
    KF_MENU_SPRITE_PANEL_BACKGROUND = 5,
    KF_MENU_SPRITE_CONFIRMED_ROW = 6,
    KF_MENU_SPRITE_LIST_TOP = 7,
    KF_MENU_SPRITE_LIST_ROW = 8,
    KF_MENU_SPRITE_LIST_BOTTOM = 9,
    KF_MENU_SPRITE_LIST_SELECTED_ROW = 10,
    KF_MENU_SPRITE_PANEL_TOP_LEFT = 11
};

/* Menu sound cues; each nonzero cue is also the sound ID it keys on. */
enum {
    KF_MENU_SOUND_NONE = 0,
    KF_MENU_SOUND_ITEM_USED = 13,
    KF_MENU_SOUND_CURSOR = 16,
    KF_MENU_SOUND_CONFIRM = 17,
    KF_MENU_SOUND_CANCEL = 18
};

enum {
    KF_MENU_CHOICE_ACCEPT = 0
};

enum {
    KF_MENU_ROOT_ITEM_SELECTION = 0,
    KF_MENU_ROOT_MAGIC_ACTION = 1,
    KF_MENU_ROOT_EQUIPMENT = 2,
    KF_MENU_ROOT_COMBAT_ATTRIBUTES = 3,
    KF_MENU_ROOT_ITEM_USE = 4,
    KF_MENU_ROOT_MEMORY_CARD = 5,
    KF_MENU_ROOT_OPTIONS = 6,
    KF_MENU_ROOT_ENTRY_COUNT = 7,
    KF_MENU_ROOT_CANCEL_ROW = KF_MENU_ROOT_ENTRY_COUNT,
    KF_MENU_ROOT_WINDOW_ROWS = KF_MENU_ROOT_ENTRY_COUNT + 1
};

/* Signed menu controls and the root controller's encoded magic action. */
enum {
    KF_MENU_RESULT_PENDING = -99,
    KF_MENU_RESULT_GAME_LOADED = -3,
    KF_MENU_RESULT_CANCELLED = -1,
    KF_MENU_SELECTION_NONE = -1,
    KF_MENU_MAGIC_ACTION_TAG = 0x1000,
    KF_MENU_MAGIC_ACTION_ID_MASK = 0x0fff
};

enum {
    KF_MENU_TRANSLUCENT_SPRITE_OFFSET = 6,
    KF_MENU_WIDGET_OT_DEPTH = 20,
    KF_MENU_CONTENT_OT_DEPTH = 10
};

enum {
    KF_MENU_TEXT_END = -1,
    KF_MENU_TEXT_GLYPH_MASK = 0x0fff,
    KF_MENU_TEXT_DAKUTEN = 0x1000,
    KF_MENU_TEXT_HANDAKUTEN = 0x2000,
    KF_MENU_FONT_COLUMNS = 16,
    KF_MENU_FONT_CELL_WIDTH = 15,
    KF_MENU_FONT_CELL_HEIGHT = 15,
    KF_MENU_DAKUTEN_U = 210,
    KF_MENU_HANDAKUTEN_U = 225,
    KF_MENU_KANA_MARK_V = 30,
    KF_MENU_GLYPH_ADVANCE = 14,
    KF_MENU_DIGIT_ADVANCE = 7,
    KF_MENU_NUMBER_COLUMN_ROWS = 11,
    KF_MENU_NUMBER_COLUMN_WIDTH = 7
};

typedef struct KfMenuPoint {
    s16 x;
    s16 y;
} KfMenuPoint;

typedef struct KfMenuSpriteDef {
    u16 tpage;
    u16 clut;
    u16 u;
    u16 v;
    s16 width;
    s16 height;
} KfMenuSpriteDef;

typedef struct KfMenuGlyphRow {
    s16 codes[KF_MENU_GLYPHS_PER_ROW];
} KfMenuGlyphRow;

typedef struct KfMenuLabelSuffix {
    s16 codes[10];
} KfMenuLabelSuffix;

typedef struct KfMenuGlyphString {
    KfMenuPoint position;
    KfMenuGlyphRow glyphs;
} KfMenuGlyphString;

struct KfMagicRecord;

typedef struct KfMenuWindowLayout {
    KfMenuGlyphString title;
    KfMenuGlyphString rows[KF_MENU_WINDOW_ROW_CAPACITY];
} KfMenuWindowLayout;

/* The initialized prefix; callers supply row and quantity pointers later. */
typedef struct KfMenuList {
    KfMenuGlyphString title;
    u8 list_x;
    u8 list_y;
    u8 entry_count;
    u8 visible_rows;
    u8 scroll_offset;
    u8 selected_index;
    u8 cursor_row;
    u8 glyphs_per_entry;
} KfMenuList;

/* Item lists keep byte values beside their glyph rows. */
typedef struct KfItemMenuList {
    KfMenuList list;
    KfMenuGlyphRow *rows;
    u8 unknown_28[4];
    u8 *values;
    s32 *prices;
} KfItemMenuList;

/* Card catalogue rows carry ten glyph codes; the renderer advances 20 bytes. */
typedef struct KfCardSlotGlyphRow {
    s16 codes[10];
} KfCardSlotGlyphRow;

typedef struct KfCardMenuList {
    KfMenuList list;
    KfCardSlotGlyphRow *rows;
    u8 unknown_28[4];
    u8 *levels;
    s32 *experience_values;
} KfCardMenuList;

/* Selection menus extend the initialized list prefix with row/value storage. */
typedef struct KfMagicMenuList {
    KfMenuList list;
    KfMenuGlyphRow *rows;
    u8 unknown_28[8];
    s32 *values;
} KfMagicMenuList;

/* The renderer reads these payload slots according to its mode. Row width
 * comes from glyphs_per_entry, so card and item rows share this view. */
typedef struct KfMenuRenderList {
    KfMenuList list;
    const s16 *row_glyphs;
    const KfMenuLabelSuffix *detail_rows;
    const u8 *byte_values;
    const s32 *number_values;
} KfMenuRenderList;

typedef char kf_menu_glyph_string_size[sizeof(KfMenuGlyphString) == 28 ? 1 : -1];
typedef char kf_menu_label_suffix_size[sizeof(KfMenuLabelSuffix) == 20 ? 1 : -1];
typedef char kf_menu_sprite_def_size[sizeof(KfMenuSpriteDef) == 12 ? 1 : -1];
typedef char kf_menu_window_layout_size[sizeof(KfMenuWindowLayout) == 308 ? 1 : -1];
typedef char kf_menu_list_prefix_size[sizeof(KfMenuList) == 36 ? 1 : -1];
typedef char kf_item_menu_list_size[sizeof(KfItemMenuList) == 52 ? 1 : -1];
typedef char kf_item_menu_list_rows_offset[offsetof(KfItemMenuList, rows) == 0x24 ? 1 : -1];
typedef char kf_item_menu_list_values_offset[offsetof(KfItemMenuList, values) == 0x2c ? 1 : -1];
typedef char kf_item_menu_list_prices_offset[offsetof(KfItemMenuList, prices) == 0x30 ? 1 : -1];
typedef char kf_card_slot_glyph_row_size[sizeof(KfCardSlotGlyphRow) == 20 ? 1 : -1];
typedef char kf_card_menu_list_size[sizeof(KfCardMenuList) == 52 ? 1 : -1];
typedef char kf_card_menu_list_rows_offset[offsetof(KfCardMenuList, rows) == 0x24 ? 1 : -1];
typedef char kf_card_menu_list_levels_offset[offsetof(KfCardMenuList, levels) == 0x2c ? 1 : -1];
typedef char kf_card_menu_list_experience_values_offset[
    offsetof(KfCardMenuList, experience_values) == 0x30 ? 1 : -1];
typedef char kf_magic_menu_list_size[sizeof(KfMagicMenuList) == 52 ? 1 : -1];
typedef char kf_magic_menu_list_rows_offset[offsetof(KfMagicMenuList, rows) == 0x24 ? 1 : -1];
typedef char kf_magic_menu_list_values_offset[offsetof(KfMagicMenuList, values) == 0x30 ? 1 : -1];
typedef char kf_menu_render_list_size[sizeof(KfMenuRenderList) == 52 ? 1 : -1];
typedef char kf_menu_render_row_offset[offsetof(KfMenuRenderList, row_glyphs) == 0x24 ? 1 : -1];
typedef char kf_menu_render_detail_offset[offsetof(KfMenuRenderList, detail_rows) == 0x28 ? 1 : -1];
typedef char kf_menu_render_byte_offset[offsetof(KfMenuRenderList, byte_values) == 0x2c ? 1 : -1];
typedef char kf_menu_render_number_offset[offsetof(KfMenuRenderList, number_values) == 0x30 ? 1 : -1];

extern KfMenuWindowLayout menu_window_layouts[KF_MENU_WINDOW_COUNT];
extern KfMenuLabelSuffix menu_header_labels[12];
extern KfMenuLabelSuffix menu_label_suffixes[16];
extern KfMenuSpriteDef menu_sprite_defs[KF_MENU_SPRITE_COUNT];
extern s32 menu_cursor_animation_frame;
extern s32 menu_cursor_animation_direction;
extern b32 menu_item_model_allocation_pending;
/* Shared item quantity; original containing data object is unresolved. */
extern s32 menu_item_quantity;
extern KfMenuGlyphRow menu_glyph_rows[120];
extern KfMenuGlyphRow menu_glyph_rows_extra[20];
extern KfMenuLabelSuffix menu_equipment_category_labels[10];
extern KfMenuLabelSuffix menu_none_option_glyphs;
extern u8 menu_item_mask_pages[6][120];
void menu_build_equipped_label_rows(KfMenuLabelSuffix *rows);
extern u16 menu_item_code_primary[6][120];
extern u16 menu_item_code_secondary[5][120];

void menu_list_init(KfMenuList *list, s32 window_kind, s32 row);
u32 menu_update_list_input(KfMenuList *list, const u8 *item_ids,
    b32 *confirmed, s32 *result);
s32 menu_preview_choice(const KfMenuList *list, s32 label_kind,
    s32 render_mode, u8 item_id);
void menu_show_map_preview(s32 menu_code);
s32 menu_card_browser(void);
/* Menu modes reinterpret the four payload words after the common list prefix. */
void menu_render_list(const KfMenuList *menu, s32 render_mode);
void menu_blit_sprite(const KfMenuSpriteDef *sprite, const KfMenuPoint *position);
void menu_blit_sprite_fixed_clut(const KfMenuSpriteDef *sprite, const KfMenuPoint *position);
void menu_blit_sprite_translucent(const KfMenuSpriteDef *sprite, const KfMenuPoint *position);
void menu_draw_string(const KfMenuSpriteDef *font, const KfMenuGlyphString *string);
void menu_draw_number(const KfMenuSpriteDef *font, const KfMenuGlyphString *string);
void menu_update_item_preview(s32 item_id);
void menu_draw_status_counters(s32 kind);
void menu_render_item_model(void);
void menu_present_frame(void);
void menu_frame_begin(void);
void menu_render_list_mode_8_9_noop(void);
void menu_format_number(s32 value, s32 count, s32 padding_mode, s32 style, s16 *out);
void menu_draw_two_option(const KfMenuGlyphString *accept_label,
    const KfMenuGlyphString *decline_label, s32 selected_choice, b32 confirmation);
void menu_draw_window(s32 window_kind, s32 count, s32 highlight, b32 confirmation);
void menu_show_combat_attributes(void);
s32 menu_collect_masked_item_rows(const u8 *mask, KfMenuGlyphRow *rows,
    u8 *values, u8 *indices, s32 first, s32 last);
s32 menu_collect_available_item_rows(const u8 *mask, KfMenuGlyphRow *rows,
    u8 *values, u8 *indices, s32 first, s32 last);
void menu_apply_item_effect(s32 item_id);
s32 menu_item_selection_controller(void);
s32 menu_choose_magic_action(void);
void menu_equipment_list_controller(void);
void menu_item_use_controller(void);
s32 menu_run_card_choice(void);
void menu_options_controller(void);
void menu_draw_player_status(void);
void menu_draw_location_number(void);
s32 menu_collect_available_magic_rows(const struct KfMagicRecord *records,
    KfMenuGlyphRow *rows, s32 *values, u8 *indices, s32 first, s32 last);
void menu_fill_item_counts_and_prices(const u8 *source, u8 *counts, s32 *prices,
    const u8 *indices, s32 first, s32 last, s32 group);
void menu_fill_item_prices(s32 *prices, const u8 *indices,
    s32 first, s32 last, s32 group);
s32 menu_card_build_slot_rows(const struct DIRENTRY *card_entries, s16 *glyph_rows,
    s32 *experience_values, u8 *levels, s32 *slot_ids);
void menu_show_dialog_panel(s32 panel, const KfMenuGlyphString *rows, s32 count,
    s32 detail0, s32 detail1, s32 detail2, s32 detail3, s32 detail4,
    s32 detail5);
void menu_build_card_probe_error_rows(KfMenuGlyphString *rows);
void menu_build_card_full_rows(KfMenuGlyphString *rows);
s32 menu_card_load_slot_browser(void);
void menu_prepare_card_browser_rows(KfMenuGlyphString *rows);
void menu_draw_nine_slice_panel(s32 x, s32 y, s32 width, s32 height,
    s32 overlap_x, s32 overlap_y);
void menu_enter_display_state(s32 mode);
void menu_exit_display_state(b32 stop_sequence);
s32 menu_load_item_model(u8 item_id);
void menu_release_item_model(void);
void menu_play_sound_cue(s32 cue);
void input_wait_brief_release(void);
s32 menu_poll_choice_input(s32 index, s32 last, s32 *selection, b32 *confirmed,
    s32 *cancelled);
void menu_card_save_browser(void);
void menu_item_buy_sell_controller(s32 kind);
s32 menu_choose_inventory_item(void);
void menu_item_trade_controller(void);
void menu_item_stock_choice_controller(void);
s32 menu_run_root_controller(void);
void player_clear_and_cap_status_effects(void);
void player_cap_darkness_phase(void);
void player_cap_curse_strength(void);
void menu_equipment_category_controller(s32 category);
void menu_choose_primary_magic_shortcut(void);
void menu_item_magic_controller(void);
s32 menu_card_load_browser(void);
s32 menu_prompt_two_option(void);
void menu_card_save_slot(s32 slot);
s32 menu_confirm_card_format(s32 kind);
void menu_prepare_card_io_error_rows(KfMenuGlyphString *rows);
void menu_prepare_card_format_declined_rows(KfMenuGlyphString *rows);
void menu_prepare_card_write_full_rows(KfMenuGlyphString *rows);
void menu_prepare_card_write_rows(KfMenuGlyphString *rows);
void menu_prepare_card_read_row(KfMenuGlyphString *rows);
void menu_prepare_card_read_failure_rows(KfMenuGlyphString *rows, s32 kind);
void menu_prepare_card_exit_rows(KfMenuGlyphString *rows);
void menu_draw_card_dialog_rows(const KfMenuGlyphString *rows, s32 count,
    s32 x, s32 y, s32 width, s32 height, s32 overlap_x, s32 overlap_y);
void menu_item_buy_controller(s32 kind);
void menu_item_sell_controller(s32 kind);
void menu_buy_masked_stock_items(void);
void menu_buy_owned_items(void);
void menu_draw_combat_attributes(void);
void menu_draw_options_rows(KfMenuGlyphString *left, KfMenuGlyphString *right,
    const u8 *selected);

#endif
