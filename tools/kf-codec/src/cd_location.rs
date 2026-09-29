//! BCD CD-ROM locations as computed by `GAME.EXE`'s own helpers:
//! `cd_location_to_sector` @`0x80017aec`, `cd_sector_to_location`
//! @`0x80017b6c` and `cd_location_add` @`0x80017bf0`.
//!
//! Unlike LIBCD's `CdPosToInt`, these count sectors from 00:00:00 with no
//! 150-sector lead-in. `track` is always written as zero.

pub const SECTORS_PER_SECOND: u32 = 75;
pub const SECTORS_PER_MINUTE: u32 = 60 * SECTORS_PER_SECOND;

/// `CdlLOC`: minute, second and sector in BCD, then the track byte.
#[derive(Debug, Clone, Copy, PartialEq, Eq, Default)]
pub struct CdLocation {
    pub minute: u8,
    pub second: u8,
    pub sector: u8,
    pub track: u8,
}

/// `cd_bcd_to_int` @`0x80017aa0`.
pub const fn bcd_to_int(bcd: u8) -> u32 {
    (bcd >> 4) as u32 * 10 + (bcd & 0x0f) as u32
}

/// `cd_int_to_bcd` @`0x80017ac0`: the byte store keeps only the low eight
/// bits of `value / 10 << 4 | value % 10` for the truncated `u8` input.
pub const fn int_to_bcd(value: u8) -> u8 {
    let value = value as u32;
    (((value / 10) << 4) | (value % 10)) as u8
}

impl CdLocation {
    pub const BYTE_SIZE: usize = 4;

    pub const fn from_bytes(bytes: [u8; Self::BYTE_SIZE]) -> Self {
        Self {
            minute: bytes[0],
            second: bytes[1],
            sector: bytes[2],
            track: bytes[3],
        }
    }

    pub const fn to_bytes(self) -> [u8; Self::BYTE_SIZE] {
        [self.minute, self.second, self.sector, self.track]
    }

    pub const fn to_sector(self) -> u32 {
        bcd_to_int(self.minute)
            .wrapping_mul(SECTORS_PER_MINUTE)
            .wrapping_add(bcd_to_int(self.second).wrapping_mul(SECTORS_PER_SECOND))
            .wrapping_add(bcd_to_int(self.sector))
    }

    /// Minute, second and sector of `sector`, each truncated to a byte before
    /// BCD conversion as the retail helper's `u8` parameter does.
    pub const fn from_sector(sector: u32) -> Self {
        let rest = sector % SECTORS_PER_MINUTE;
        Self {
            minute: int_to_bcd((sector / SECTORS_PER_MINUTE) as u8),
            second: int_to_bcd((rest / SECTORS_PER_SECOND) as u8),
            sector: int_to_bcd((rest % SECTORS_PER_SECOND) as u8),
            track: 0,
        }
    }

    pub const fn add(self, sector_offset: u32) -> Self {
        Self::from_sector(self.to_sector().wrapping_add(sector_offset))
    }
}
