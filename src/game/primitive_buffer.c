#include <kf/lib/address.h>
#include <kf/game/graphics.h>
#include <kf/game/menu.h>

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
