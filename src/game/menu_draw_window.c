#include <kf/lib/address.h>
#include <kf/game/menu.h>

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

ADDRESS(0x8001fb8c, 0x108)
void menu_draw_window(s32 window_kind, s32 count, s32 highlight, s32 confirmation)
{
    const KfMenuWindowLayout *layout = &menu_window_layouts[window_kind];
    const KfMenuGlyphString *row = &layout->rows[0];
    s32 index;

    if (layout->title.position.x != 0) {
        menu_blit_sprite_translucent(&menu_sprite_defs[5], &layout->title.position);
        menu_draw_string(&menu_sprite_defs[1], &layout->title);
    }
    if (count > 0) {
        index = 0;
        do {
            if (index == highlight && confirmation == KF_MENU_CONFIRM_REQUESTED)
                menu_blit_sprite_translucent(&menu_sprite_defs[6], &row->position);
            else
                menu_blit_sprite_translucent(&menu_sprite_defs[5], &row->position);
            if (index == highlight)
                menu_blit_sprite(&menu_sprite_defs[2], &row->position);
            menu_draw_string(&menu_sprite_defs[1], row);
            row++;
            index++;
        } while (index < count);
    }
}
