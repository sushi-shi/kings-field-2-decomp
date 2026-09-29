#include <kf/lib/address.h>
#include <kf/game/cd.h>

enum {
    CD_SECTORS_PER_SECOND = 75,
    CD_SECTORS_PER_MINUTE = 60 * CD_SECTORS_PER_SECOND
};

ADDRESS(0x80017aa0, 0x20)
s32 cd_bcd_to_int(u8 bcd)
{
    return (bcd >> 4) * 10 + (bcd & 0xf);
}

ADDRESS(0x80017ac0, 0x2c)
u32 cd_int_to_bcd(u8 value)
{
    return (value / 10) << 4 | value % 10;
}

/* Absolute sector of a BCD location, without CdPosToInt's 150-sector lead-in. */
ADDRESS(0x80017aec, 0x80)
u32 cd_location_to_sector(CdlLOC *location)
{
    return cd_bcd_to_int(location->minute) * CD_SECTORS_PER_MINUTE
        + cd_bcd_to_int(location->second) * CD_SECTORS_PER_SECOND
        + cd_bcd_to_int(location->sector);
}

ADDRESS(0x80017b6c, 0x84)
void cd_sector_to_location(CdlLOC *location, u32 sector)
{
    u32 rest;

    location->minute = cd_int_to_bcd(sector / CD_SECTORS_PER_MINUTE);
    rest = sector % CD_SECTORS_PER_MINUTE;
    location->second = cd_int_to_bcd(rest / CD_SECTORS_PER_SECOND);
    location->sector = cd_int_to_bcd(rest % CD_SECTORS_PER_SECOND);
    location->track = 0;
}

ADDRESS(0x80017bf0, 0x3c)
void cd_location_add(CdlLOC *base, u32 sector_offset, CdlLOC *result)
{
    cd_sector_to_location(result, cd_location_to_sector(base) + sector_offset);
}
