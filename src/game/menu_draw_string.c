#include <kf/lib/address.h>
#include <kf/game/graphics.h>
#include <kf/game/menu.h>

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
