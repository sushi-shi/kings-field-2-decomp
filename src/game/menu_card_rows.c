#include <kf/lib/address.h>
#include <kf/lib/types.h>

extern s32 func_800228c8(const char *filename, s32 *experience, s32 *level,
    s32 *slot_id);

ADDRESS(0x8001af30, 0x100)
s32 func_8001af30(const u8 *card_entries, s16 *glyph_rows,
    s32 *experience_values, u8 *levels, s32 *slot_ids)
{
    s32 count = 0;
    s32 index;
    s32 experience;
    s32 level;
    s32 slot_id;

    for (index = 0; index < 15; index++) {
        if (func_800228c8((const char *)card_entries,
            &experience, &level, &slot_id) == 0) {
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
        card_entries += 40;
    }
    return count;
}
