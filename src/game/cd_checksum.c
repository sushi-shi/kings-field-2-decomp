#include <kf/lib/address.h>
#include <kf/game/cd.h>

/* Nonzero unless the last word of DATA equals KF_CD_CHECKSUM_SEED plus the
 * sum of every preceding word. */
ADDRESS(0x80017d00, 0x54)
s32 cd_sectors_corrupt(u32 *data, s32 sector_count)
{
    u32 sum = KF_CD_CHECKSUM_SEED;
    s32 last = sector_count * KF_CD_SECTOR_WORDS - 1;
    u32 *word = data;
    s32 remaining;

    for (remaining = last - 1; remaining != -1; remaining--) {
        sum += *word++;
    }
    return sum != data[last];
}
