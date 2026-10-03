#ifndef KF_GAME_MENU_H
#define KF_GAME_MENU_H

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
    KF_MENU_SPRITE_OPTION_HIGHLIGHT = 4
};

enum {
    KF_MENU_CHOICE_ACCEPT = 0,
    KF_MENU_CONFIRM_REQUESTED = 1
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

enum {
    KF_MENU_MODEL_RELEASED = 0,
    KF_MENU_MODEL_ALLOCATED = 1
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
    u32 *codes;
} KfItemMenuList;

/* Card catalogue rows carry ten glyph codes; the renderer advances 20 bytes. */
typedef struct KfCardSlotGlyphRow {
    s16 codes[10];
} KfCardSlotGlyphRow;

typedef struct KfCardMenuList {
    KfMenuList list;
    KfCardSlotGlyphRow *rows;
    u8 unknown_28[4];
    u8 *values;
    s32 *codes;
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
typedef char kf_item_menu_list_rows_offset[(u32)&((KfItemMenuList *)0)->rows == 0x24 ? 1 : -1];
typedef char kf_item_menu_list_values_offset[(u32)&((KfItemMenuList *)0)->values == 0x2c ? 1 : -1];
typedef char kf_item_menu_list_codes_offset[(u32)&((KfItemMenuList *)0)->codes == 0x30 ? 1 : -1];
typedef char kf_card_slot_glyph_row_size[sizeof(KfCardSlotGlyphRow) == 20 ? 1 : -1];
typedef char kf_card_menu_list_size[sizeof(KfCardMenuList) == 52 ? 1 : -1];
typedef char kf_card_menu_list_rows_offset[(u32)&((KfCardMenuList *)0)->rows == 0x24 ? 1 : -1];
typedef char kf_card_menu_list_values_offset[(u32)&((KfCardMenuList *)0)->values == 0x2c ? 1 : -1];
typedef char kf_card_menu_list_codes_offset[(u32)&((KfCardMenuList *)0)->codes == 0x30 ? 1 : -1];
typedef char kf_magic_menu_list_size[sizeof(KfMagicMenuList) == 52 ? 1 : -1];
typedef char kf_magic_menu_list_rows_offset[(u32)&((KfMagicMenuList *)0)->rows == 0x24 ? 1 : -1];
typedef char kf_magic_menu_list_values_offset[(u32)&((KfMagicMenuList *)0)->values == 0x30 ? 1 : -1];
typedef char kf_menu_render_list_size[sizeof(KfMenuRenderList) == 52 ? 1 : -1];
typedef char kf_menu_render_row_offset[(u32)&((KfMenuRenderList *)0)->row_glyphs == 0x24 ? 1 : -1];
typedef char kf_menu_render_detail_offset[(u32)&((KfMenuRenderList *)0)->detail_rows == 0x28 ? 1 : -1];
typedef char kf_menu_render_byte_offset[(u32)&((KfMenuRenderList *)0)->byte_values == 0x2c ? 1 : -1];
typedef char kf_menu_render_number_offset[(u32)&((KfMenuRenderList *)0)->number_values == 0x30 ? 1 : -1];

extern KfMenuWindowLayout menu_window_layouts[KF_MENU_WINDOW_COUNT];
extern KfMenuLabelSuffix menu_header_labels[12];
extern KfMenuLabelSuffix menu_label_suffixes[16];
extern KfMenuSpriteDef menu_sprite_defs[KF_MENU_SPRITE_COUNT];
extern s32 menu_cursor_animation_frame;
extern s32 menu_cursor_animation_direction;
extern u_long *menu_frame_upload_pixels;
extern RECT menu_frame_upload_rect;
extern s32 menu_item_model_allocation_pending;
/* Shared item quantity; original containing data object is unresolved. */
extern s32 DAT_8006d694;
extern SVECTOR menu_item_preview_translation;
extern SVECTOR menu_item_preview_rotation;
extern s32 menu_item_preview_rotation_step;
extern KfMenuGlyphRow menu_glyph_rows[120];
extern KfMenuGlyphRow menu_glyph_rows_extra[20];
extern KfMenuLabelSuffix menu_equipment_labels_64910[10];
extern s16 menu_row_prefix_649ec[4];
extern u8 menu_item_mask_pages[6][120];
void func_80019ce4(KfMenuLabelSuffix *rows);
extern u16 menu_item_code_primary[6][120];
extern u16 menu_item_code_secondary[5][120];

void menu_list_init(KfMenuList *list, s32 window_kind, s32 row);
u32 menu_update_list_input(KfMenuList *list, const u8 *item_ids,
    s32 *selection, s32 *result);
s32 menu_preview_choice(void *list_state, s32 label_kind,
    s32 render_mode, s32 item_id);
void menu_show_map_preview(s32 menu_code);
s32 menu_card_browser(void);
/* Menu modes reinterpret the four payload words after the common list prefix. */
void func_8001fc94(const void *list_state, s32 render_mode);
void menu_blit_sprite(const KfMenuSpriteDef *sprite, const KfMenuPoint *position);
void menu_blit_sprite_fixed_clut(const KfMenuSpriteDef *sprite, const KfMenuPoint *position);
void menu_blit_sprite_translucent(const KfMenuSpriteDef *sprite, const KfMenuPoint *position);
void menu_draw_string(const KfMenuSpriteDef *font, const KfMenuGlyphString *string);
void menu_draw_number(const KfMenuSpriteDef *font, const KfMenuGlyphString *string);
void func_8002083c(s32 item_id);
void func_80020990(s32 kind);
void menu_render_item_model(void);
void menu_present_frame(void);
void menu_frame_begin(void);
void func_80021a60(void);
void menu_format_number(s32 value, s32 count, s32 padding_mode, s32 style, s16 *out);
void menu_draw_two_option(const KfMenuGlyphString *accept_label,
    const KfMenuGlyphString *decline_label, s32 selected_choice, s32 confirmation);
void menu_draw_window(s32 window_kind, s32 count, s32 highlight, s32 confirmation);
void func_8001a7fc(void);
s32 func_80018d08(const u8 *mask, KfMenuGlyphRow *rows,
    u8 *values, u8 *indices, s32 first, s32 last);
s32 func_80018dec(const u8 *mask, KfMenuGlyphRow *rows,
    u8 *values, u8 *indices, s32 first, s32 last);
void func_80018f8c(s32 item_id);
s32 func_80018ac8(void);
s32 func_80019834(void);
void func_80019ac4(void);
void func_8001a898(void);
s32 func_8001aa9c(void);
void func_8001b2dc(void);
void func_8001e94c(void);
void func_800189f0(void);
s32 func_800199d0(const struct KfMagicRecord *records,
    KfMenuGlyphRow *rows, s32 *values, u8 *indices, s32 first, s32 last);
void menu_fill_item_counts_and_prices(const u8 *source, u8 *counts, u32 *prices,
    const u8 *indices, s32 first, s32 last, s32 group);
void menu_fill_item_prices(u32 *prices, const u8 *indices,
    s32 first, s32 last, s32 group);
s32 menu_card_build_slot_rows(const struct DIRENTRY *card_entries, s16 *glyph_rows,
    s32 *experience_values, u8 *levels, s32 *slot_ids);
void func_8001b030(s32 panel, const KfMenuGlyphString *rows, s32 count,
    s32 detail0, s32 detail1, s32 detail2, s32 detail3, s32 detail4,
    s32 detail5);
void func_8001ba80(KfMenuGlyphString *rows);
void func_8001bb94(KfMenuGlyphString *rows);
s32 menu_card_load_slot_browser(void);
void func_8001c550(KfMenuGlyphString *rows);
void func_800217f0(s32 x, s32 y, s32 width, s32 height,
    s32 overlap_x, s32 overlap_y);
void menu_enter_display_state(s32 mode);
void menu_exit_display_state(s32 stop_sequence);
s32 menu_load_item_model(u8 item_id);
void menu_release_item_model(void);
void func_80022300(s32 cue);
void func_800223cc(void);
s32 menu_poll_choice_input(s32 index, s32 last, s32 *selection, s32 *confirmed,
    s32 *cancelled);
void menu_card_save_browser(void);
void func_8001ceb8(s32 kind);
s32 menu_choose_inventory_item(void);
void func_8001d8d0(void);
void func_8001dc64(void);
s32 func_8001876c(void);
void func_80019240(void);
void func_800192ac(void);
void func_800192dc(void);
void func_80019ed4(s32 category);
void func_8001a2f4(void);
void func_8001a4f0(void);
s32 menu_card_load_browser(void);
s32 func_8001b14c(void);
void menu_card_save_slot(s32 slot);
s32 menu_confirm_card_format(s32 kind);
void func_8001c62c(KfMenuGlyphString *rows);
void func_8001c770(KfMenuGlyphString *rows);
void func_8001c8b0(KfMenuGlyphString *rows);
void func_8001c9f4(KfMenuGlyphString *rows);
void func_8001cad4(KfMenuGlyphString *rows);
void func_8001cb44(KfMenuGlyphString *rows, s32 kind);
void func_8001ccd4(KfMenuGlyphString *rows);
void func_8001cdb0(const KfMenuGlyphString *rows, s32 count,
    s32 x, s32 y, s32 width, s32 height, s32 overlap_x, s32 overlap_y);
void menu_item_buy_controller(s32 kind);
void menu_item_sell_controller(s32 kind);
void func_8001ddd0(void);
void func_8001e0a8(void);
void func_8001f008(void);
void func_8001f798(KfMenuGlyphString *left, KfMenuGlyphString *right,
    const u8 *selected);

#endif
