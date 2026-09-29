//! Sector-indexed `COM\*.T` archives opened by `func_800184d0` and read by
//! `func_800182d0` in `GAME.EXE`.
//!
//! Sector 0 starts with a little-endian `u16` entry count followed by
//! `count + 1` little-endian `u16` sector offsets; the rest of the sector is
//! zero. Entry `i` occupies sectors `offsets[i]..offsets[i + 1]` of the file,
//! so an entry with equal bounds is empty and the final offset is the file's
//! sector count. The game reads only sector 0 when opening an archive and
//! copies the table from it, then reads whole sectors per entry.
//!
//! Entries read through `func_800182d0` end in a checksum word: the last
//! word equals [`CHECKSUM_SEED`] plus the wrapping sum of every preceding
//! little-endian word, and the game rereads the entry until it holds. Entries
//! streamed through other paths (all of `RTIM.T`, the VB bodies in `VAB.T`)
//! carry no checksum.
//!
//! Retail checks neither the entry index nor the table's fit in sector 0;
//! this reader rejects both instead of reading outside the archive.

use core::fmt;

use crate::Sink;

pub const SECTOR_SIZE: usize = 2048;
pub const COUNT_SIZE: usize = 2;
pub const OFFSET_SIZE: usize = 2;
/// Initial value of the per-entry checksum (`cd_sectors_corrupt` @`0x80017d00`).
pub const CHECKSUM_SEED: u32 = 0x1234_5678;
/// Largest count whose `count + 1` offsets still fit in sector 0.
pub const MAX_ENTRIES: u16 = ((SECTOR_SIZE - COUNT_SIZE) / OFFSET_SIZE - 1) as u16;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum ArchiveError {
    TruncatedTable {
        need: usize,
        available: usize,
    },
    TableOutsideFirstSector {
        count: u16,
    },
    IndexOutOfRange {
        index: usize,
        count: u16,
    },
    DescendingOffsets {
        index: usize,
        start: u16,
        end: u16,
    },
    TruncatedEntry {
        index: usize,
        end: usize,
        available: usize,
    },
    TooManyEntries {
        count: usize,
    },
    ArchiveTooLarge {
        sectors: usize,
    },
    OutputFull {
        need: usize,
        have: usize,
    },
}

impl fmt::Display for ArchiveError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match *self {
            Self::TruncatedTable { need, available } => {
                write!(f, "archive table needs {need} bytes, {available} available")
            }
            Self::TableOutsideFirstSector { count } => {
                write!(
                    f,
                    "archive table for {count} entries does not fit in sector 0"
                )
            }
            Self::IndexOutOfRange { index, count } => {
                write!(f, "archive entry {index} is outside {count} entries")
            }
            Self::DescendingOffsets { index, start, end } => write!(
                f,
                "archive entry {index} starts at sector {start} after its end {end}"
            ),
            Self::TruncatedEntry {
                index,
                end,
                available,
            } => write!(
                f,
                "archive entry {index} ends at byte {end}, {available} bytes available"
            ),
            Self::TooManyEntries { count } => {
                write!(f, "{count} entries exceed the sector-0 table")
            }
            Self::ArchiveTooLarge { sectors } => {
                write!(f, "{sectors} sectors exceed a 16-bit sector offset")
            }
            Self::OutputFull { need, have } => {
                write!(f, "output buffer holds {have} bytes, need {need}")
            }
        }
    }
}

impl core::error::Error for ArchiveError {}

/// One archive entry: its sector bounds and whole-sector payload.
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub struct Entry<'a> {
    pub index: usize,
    pub start_sector: u16,
    pub end_sector: u16,
    pub bytes: &'a [u8],
}

impl Entry<'_> {
    pub const fn sector_count(&self) -> u16 {
        self.end_sector - self.start_sector
    }

    pub const fn is_empty(&self) -> bool {
        self.start_sector == self.end_sector
    }
}

#[derive(Debug, Clone, Copy)]
pub struct Archive<'a> {
    bytes: &'a [u8],
    count: u16,
}

impl<'a> Archive<'a> {
    pub fn parse(bytes: &'a [u8]) -> Result<Self, ArchiveError> {
        let Some(count) = bytes.get(..COUNT_SIZE) else {
            return Err(ArchiveError::TruncatedTable {
                need: COUNT_SIZE,
                available: bytes.len(),
            });
        };
        let count = u16::from_le_bytes([count[0], count[1]]);
        if count > MAX_ENTRIES {
            return Err(ArchiveError::TableOutsideFirstSector { count });
        }
        let need = table_size(count);
        if bytes.len() < need {
            return Err(ArchiveError::TruncatedTable {
                need,
                available: bytes.len(),
            });
        }
        Ok(Self { bytes, count })
    }

    pub const fn count(&self) -> u16 {
        self.count
    }

    /// Sector offset `index` of the table, `0..=count`.
    pub fn offset(&self, index: usize) -> Option<u16> {
        if index > usize::from(self.count) {
            return None;
        }
        let at = COUNT_SIZE + index * OFFSET_SIZE;
        Some(u16::from_le_bytes([self.bytes[at], self.bytes[at + 1]]))
    }

    /// Sector count of the archive according to its final offset.
    pub fn sector_count(&self) -> u16 {
        self.offset(usize::from(self.count)).unwrap_or(0)
    }

    pub fn entry(&self, index: usize) -> Result<Entry<'a>, ArchiveError> {
        let (Some(start), Some(end)) = (self.offset(index), self.offset(index + 1)) else {
            return Err(ArchiveError::IndexOutOfRange {
                index,
                count: self.count,
            });
        };
        if end < start {
            return Err(ArchiveError::DescendingOffsets { index, start, end });
        }
        let from = usize::from(start) * SECTOR_SIZE;
        let to = usize::from(end) * SECTOR_SIZE;
        let Some(bytes) = self.bytes.get(from..to) else {
            return Err(ArchiveError::TruncatedEntry {
                index,
                end: to,
                available: self.bytes.len(),
            });
        };
        Ok(Entry {
            index,
            start_sector: start,
            end_sector: end,
            bytes,
        })
    }

    pub fn entries(&self) -> Entries<'a> {
        Entries {
            archive: *self,
            next: 0,
        }
    }
}

/// Wrapping sum of [`CHECKSUM_SEED`] and every word before the last.
///
/// `bytes` must be a whole number of words; the game always passes whole
/// sectors. Returns `None` for an empty or partial-word slice.
pub fn checksum(bytes: &[u8]) -> Option<u32> {
    if bytes.is_empty() || bytes.len() % 4 != 0 {
        return None;
    }
    let body = &bytes[..bytes.len() - 4];
    Some(body.chunks_exact(4).fold(CHECKSUM_SEED, |sum, word| {
        sum.wrapping_add(u32::from_le_bytes([word[0], word[1], word[2], word[3]]))
    }))
}

/// Whether the last word holds the checksum of the words before it.
pub fn checksum_holds(bytes: &[u8]) -> bool {
    match checksum(bytes) {
        Some(sum) => bytes[bytes.len() - 4..] == sum.to_le_bytes(),
        None => false,
    }
}

/// Writes the checksum of every preceding word into the last word.
pub fn seal_checksum(bytes: &mut [u8]) -> bool {
    match checksum(bytes) {
        Some(sum) => {
            let end = bytes.len();
            bytes[end - 4..].copy_from_slice(&sum.to_le_bytes());
            true
        }
        None => false,
    }
}

pub struct Entries<'a> {
    archive: Archive<'a>,
    next: usize,
}

impl<'a> Iterator for Entries<'a> {
    type Item = Result<Entry<'a>, ArchiveError>;

    fn next(&mut self) -> Option<Self::Item> {
        if self.next >= usize::from(self.archive.count) {
            return None;
        }
        let entry = self.archive.entry(self.next);
        self.next += 1;
        Some(entry)
    }
}

const fn table_size(count: u16) -> usize {
    COUNT_SIZE + (count as usize + 1) * OFFSET_SIZE
}

const fn sectors_for(len: usize) -> usize {
    len.div_ceil(SECTOR_SIZE)
}

/// Placeholder that `encode_into` replaces with the measured sizes.
const FULL: ArchiveError = ArchiveError::OutputFull { need: 0, have: 0 };

fn encode(entries: &[&[u8]], sink: &mut Sink<'_>) -> Result<(), ArchiveError> {
    let count = u16::try_from(entries.len())
        .ok()
        .filter(|&count| count <= MAX_ENTRIES)
        .ok_or(ArchiveError::TooManyEntries {
            count: entries.len(),
        })?;
    if !sink.extend(&count.to_le_bytes()) {
        return Err(FULL);
    }
    let mut sector = 1usize;
    for offset in core::iter::once(0).chain(entries.iter().map(|entry| sectors_for(entry.len()))) {
        sector += offset;
        let value =
            u16::try_from(sector).map_err(|_| ArchiveError::ArchiveTooLarge { sectors: sector })?;
        if !sink.extend(&value.to_le_bytes()) {
            return Err(FULL);
        }
    }
    if !sink.zeros(SECTOR_SIZE - table_size(count)) {
        return Err(FULL);
    }
    for entry in entries {
        let padding = sectors_for(entry.len()) * SECTOR_SIZE - entry.len();
        if !sink.extend(entry) || !sink.zeros(padding) {
            return Err(FULL);
        }
    }
    Ok(())
}

/// Encoded size of an archive whose entries are zero-padded to whole sectors.
pub fn encoded_len(entries: &[&[u8]]) -> Result<usize, ArchiveError> {
    let mut sink = Sink::Count(0);
    encode(entries, &mut sink)?;
    Ok(sink.len())
}

/// Writes the sector-0 table and every entry, zero-padding each to whole
/// sectors. Entries taken from a retail archive keep their own padding, so
/// re-encoding them reproduces the archive byte for byte.
pub fn encode_into(entries: &[&[u8]], output: &mut [u8]) -> Result<usize, ArchiveError> {
    let have = output.len();
    let mut sink = Sink::Write {
        bytes: output,
        at: 0,
    };
    match encode(entries, &mut sink) {
        Ok(()) => Ok(sink.len()),
        Err(ArchiveError::OutputFull { .. }) => Err(ArchiveError::OutputFull {
            need: encoded_len(entries)?,
            have,
        }),
        Err(error) => Err(error),
    }
}
