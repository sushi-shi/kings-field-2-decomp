#include <kf/lib/address.h>
#include <kf/game/menu.h>
#include <psyq/libc.h>

ADDRESS(0x8001ba80, 0x114)
void func_8001ba80(KfMenuGlyphString *row)
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
void func_8001bb94(KfMenuGlyphString *row)
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
