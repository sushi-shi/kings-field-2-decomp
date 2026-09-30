#include <kf/lib/address.h>
#include <kf/game/menu.h>

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
    s32 i = 0;
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
