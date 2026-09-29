use kf_codec::sector_archive::{self, Archive, ArchiveError, MAX_ENTRIES, SECTOR_SIZE};

/// Archive with sector offsets `offsets` and `sectors` total sectors, each
/// payload sector filled with its sector number.
fn archive(offsets: &[u16], sectors: usize) -> Vec<u8> {
    let mut bytes = vec![0; sectors * SECTOR_SIZE];
    let count = u16::try_from(offsets.len() - 1).unwrap();
    bytes[..2].copy_from_slice(&count.to_le_bytes());
    for (index, offset) in offsets.iter().enumerate() {
        bytes[2 + index * 2..4 + index * 2].copy_from_slice(&offset.to_le_bytes());
    }
    for sector in 1..sectors {
        bytes[sector * SECTOR_SIZE..(sector + 1) * SECTOR_SIZE].fill(sector as u8);
    }
    bytes
}

#[test]
fn entries_span_whole_sectors_between_offsets() {
    let bytes = archive(&[1, 3, 3, 4], 4);
    let parsed = Archive::parse(&bytes).unwrap();
    assert_eq!(parsed.count(), 3);
    assert_eq!(parsed.sector_count(), 4);

    let entries: Vec<_> = parsed.entries().map(Result::unwrap).collect();
    assert_eq!(entries.len(), 3);
    assert_eq!((entries[0].start_sector, entries[0].end_sector), (1, 3));
    assert_eq!(entries[0].bytes.len(), 2 * SECTOR_SIZE);
    assert_eq!(entries[0].bytes[0], 1);
    assert_eq!(entries[0].bytes[SECTOR_SIZE], 2);
    assert!(entries[1].is_empty());
    assert_eq!(entries[1].bytes.len(), 0);
    assert_eq!(entries[2].sector_count(), 1);
    assert_eq!(entries[2].bytes[0], 3);
}

#[test]
fn rejects_indices_the_game_would_not_bound() {
    let bytes = archive(&[1, 2], 2);
    let parsed = Archive::parse(&bytes).unwrap();
    assert_eq!(
        parsed.entry(1),
        Err(ArchiveError::IndexOutOfRange { index: 1, count: 1 })
    );
}

#[test]
fn rejects_descending_and_truncated_entries() {
    let bytes = archive(&[2, 1], 3);
    assert_eq!(
        Archive::parse(&bytes).unwrap().entry(0),
        Err(ArchiveError::DescendingOffsets {
            index: 0,
            start: 2,
            end: 1
        })
    );

    let mut bytes = archive(&[1, 3], 3);
    bytes.truncate(2 * SECTOR_SIZE + 5);
    assert_eq!(
        Archive::parse(&bytes).unwrap().entry(0),
        Err(ArchiveError::TruncatedEntry {
            index: 0,
            end: 3 * SECTOR_SIZE,
            available: 2 * SECTOR_SIZE + 5
        })
    );
}

#[test]
fn table_must_fit_in_the_sector_the_game_reads() {
    let mut bytes = vec![0; SECTOR_SIZE];
    bytes[..2].copy_from_slice(&(MAX_ENTRIES + 1).to_le_bytes());
    assert_eq!(
        Archive::parse(&bytes).unwrap_err(),
        ArchiveError::TableOutsideFirstSector {
            count: MAX_ENTRIES + 1
        }
    );
    assert_eq!(
        Archive::parse(&[3, 0, 1, 0]).unwrap_err(),
        ArchiveError::TruncatedTable {
            need: 10,
            available: 4
        }
    );
}

#[test]
fn encoding_retail_shaped_entries_is_byte_exact() {
    let bytes = archive(&[1, 3, 3, 4], 4);
    let parsed = Archive::parse(&bytes).unwrap();
    let entries: Vec<&[u8]> = parsed.entries().map(|entry| entry.unwrap().bytes).collect();

    let mut encoded = vec![0; sector_archive::encoded_len(&entries).unwrap()];
    assert_eq!(
        sector_archive::encode_into(&entries, &mut encoded).unwrap(),
        bytes.len()
    );
    assert_eq!(encoded, bytes);
}

#[test]
fn encoder_pads_partial_sectors_with_zeros() {
    let entries: [&[u8]; 2] = [b"TIM", b""];
    let len = sector_archive::encoded_len(&entries).unwrap();
    assert_eq!(len, 2 * SECTOR_SIZE);

    let mut encoded = vec![0xff; len];
    sector_archive::encode_into(&entries, &mut encoded).unwrap();
    let parsed = Archive::parse(&encoded).unwrap();
    assert_eq!(parsed.count(), 2);
    let first = parsed.entry(0).unwrap();
    assert_eq!(&first.bytes[..3], b"TIM");
    assert!(first.bytes[3..].iter().all(|&byte| byte == 0));
    assert!(parsed.entry(1).unwrap().is_empty());
    assert!(encoded[8..SECTOR_SIZE].iter().all(|&byte| byte == 0));
}

#[test]
fn writer_reports_the_required_size() {
    let entries: [&[u8]; 1] = [b"x"];
    let mut output = [0; SECTOR_SIZE];
    assert_eq!(
        sector_archive::encode_into(&entries, &mut output),
        Err(ArchiveError::OutputFull {
            need: 2 * SECTOR_SIZE,
            have: SECTOR_SIZE
        })
    );
}

#[test]
fn checksum_is_the_seeded_sum_of_every_word_but_the_last() {
    let mut entry = vec![0u8; SECTOR_SIZE];
    entry[..8].copy_from_slice(&[1, 0, 0, 0, 0xff, 0xff, 0xff, 0xff]);
    assert!(!sector_archive::checksum_holds(&entry));
    assert!(sector_archive::seal_checksum(&mut entry));
    // 0x12345678 + 1 + 0xffffffff wraps to 0x12345678.
    assert_eq!(entry[SECTOR_SIZE - 4..], 0x1234_5678u32.to_le_bytes());
    assert!(sector_archive::checksum_holds(&entry));

    entry[100] ^= 1;
    assert!(!sector_archive::checksum_holds(&entry));
    assert_eq!(sector_archive::checksum(&[]), None);
    assert_eq!(sector_archive::checksum(&[0; 6]), None);
}
