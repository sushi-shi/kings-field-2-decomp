#include <kf/lib/address.h>
#include <kf/lib/types.h>
#include <kf/game/card.h>
#include <psyq/kernel.h>

typedef char kf_card_directory_entry_size[sizeof(struct DIRENTRY) == 40 ? 1 : -1];

ADDRESS(0x8001af30, 0x100)
s32 func_8001af30(const struct DIRENTRY *card_entries, s16 *glyph_rows,
    s32 *experience_values, u8 *levels, s32 *slot_ids)
{
    s32 count = 0;
    s32 index;
    s32 experience;
    s32 level;
    s32 slot_id;

    for (index = 0; index < 15; index++) {
        if (func_800228c8(card_entries->name,
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
        ++card_entries;
    }
    return count;
}
