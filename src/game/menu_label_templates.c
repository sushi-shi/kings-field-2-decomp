#include <kf/lib/address.h>
#include <kf/game/card.h>
#include <kf/game/menu.h>
#include <psyq/libc.h>

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

ADDRESS(0x8001c550, 0xdc)
void func_8001c550(KfMenuGlyphString *row)
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
void func_8001c62c(KfMenuGlyphString *row)
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
void func_8001c770(KfMenuGlyphString *row)
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
void func_8001c8b0(KfMenuGlyphString *row)
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
void func_8001c9f4(KfMenuGlyphString *row)
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
void func_8001cad4(KfMenuGlyphString *row)
{
    row->position.x = 102;
    row->position.y = 112;
    memcpy(row->glyphs.codes, menu_label_suffixes[13].codes,
        sizeof menu_label_suffixes[13]);
}

ADDRESS(0x8001cb44, 0x190)
void func_8001cb44(KfMenuGlyphString *row, s32 kind)
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
void func_8001ccd4(KfMenuGlyphString *row)
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
void func_8001cdb0(const KfMenuGlyphString *rows, s32 count,
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
        func_800217f0(x, y, width, height, overlap_x, overlap_y);
        menu_present_frame();
    }
}

ADDRESS(0x8001ceb8, 0x178)
void func_8001ceb8(s32 kind)
{
    s32 cursor = 0;
    s32 confirmed = 0;
    s32 result = -99;
    s32 selection = -1;
    s32 frame;

    func_80021c8c(1);
    for (frame = 0; frame < 2; frame++) {
        menu_frame_begin();
        menu_draw_window(3, 3, cursor, confirmed);
        menu_present_frame();
    }
    func_80022300(16);
    input_wait_release();
    for (;;) {
        if (selection != -1 || result != -99)
            input_wait_release();

        switch (selection) {
        case 0:
            func_8001d030(kind);
            break;
        case 1:
            func_8001d3b4(kind);
            break;
        }

        if (result != -99)
            break;
        cursor = func_8001e378(cursor, 2, &selection, &confirmed, &result);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            menu_draw_window(3, 3, cursor, confirmed);
            menu_present_frame();
        }
    }
    func_80021e00(0);
}
