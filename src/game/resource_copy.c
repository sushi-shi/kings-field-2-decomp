#include <kf/lib/address.h>
#include <kf/game/memory.h>

ADDRESS(0x800171c8, 0x30)
const u32 *resource_copy_words(u32 *destination, const u32 *source, u32 word_count)
{
    while (word_count-- != 0) {
        *destination++ = *source++;
    }
    return source;
}

ADDRESS(0x800171f8, 0x30)
const u16 *resource_copy_halfwords(u16 *destination, const u16 *source, u32 halfword_count)
{
    while (halfword_count-- != 0) {
        *destination++ = *source++;
    }
    return source;
}
