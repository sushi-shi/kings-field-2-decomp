//! Opt-in checks against locally extracted SLPS-00069 resources.
//!
//! Run with `KF_RETAIL_DIR` pointing at the extracted retail disc directory
//! (the one holding `GAME.EXE` and `CD/COM`):
//! `cargo test --test retail_corpus -- --ignored`.

use std::fs;
use std::path::PathBuf;

use kf_codec::chunked::Cursor;
use kf_codec::sector_archive::{self, Archive, SECTOR_SIZE};

fn com_dir() -> PathBuf {
    PathBuf::from(
        std::env::var_os("KF_RETAIL_DIR")
            .expect("set KF_RETAIL_DIR to the extracted retail disc directory"),
    )
    .join("CD/COM")
}

/// Archive name, entry count, sector count, and the number of non-empty
/// entries whose checksum holds and does not hold on the SLPS-00069 disc.
const ARCHIVES: [(&str, u16, u16, usize, usize); 7] = [
    ("FDAT.T", 70, 971, 45, 0),
    ("ITEM.T", 548, 5425, 403, 0),
    ("MO.T", 448, 2780, 405, 0),
    ("RTIM.T", 75, 2102, 0, 72),
    ("RTMD.T", 9, 529, 9, 0),
    ("TALK.T", 755, 6444, 379, 0),
    ("VAB.T", 331, 1840, 66, 56),
];

#[test]
#[ignore = "requires proprietary SLPS-00069 files via KF_RETAIL_DIR"]
fn every_archive_parses_and_reencodes_byte_exact() {
    for (name, count, sectors, _, _) in ARCHIVES {
        let path = com_dir().join(name);
        let bytes = fs::read(&path).unwrap();
        let archive = Archive::parse(&bytes).unwrap();
        assert_eq!(archive.count(), count, "{name}");
        assert_eq!(archive.sector_count(), sectors, "{name}");
        assert_eq!(bytes.len(), usize::from(sectors) * SECTOR_SIZE, "{name}");

        let entries: Vec<&[u8]> = archive
            .entries()
            .map(|entry| {
                entry
                    .unwrap_or_else(|error| panic!("{name}: {error}"))
                    .bytes
            })
            .collect();
        let mut encoded = vec![0; sector_archive::encoded_len(&entries).unwrap()];
        sector_archive::encode_into(&entries, &mut encoded).unwrap();
        assert!(encoded == bytes, "{name}: re-encoded archive differs");
    }
}

#[test]
#[ignore = "requires proprietary SLPS-00069 files via KF_RETAIL_DIR"]
fn common_tables_are_nine_chunks() {
    let bytes = fs::read(com_dir().join("FDAT.T")).unwrap();
    let archive = Archive::parse(&bytes).unwrap();
    let entry = archive.entry(48).unwrap();
    let mut cursor = Cursor::new(entry.bytes);
    let sizes: Vec<usize> = (0..9)
        .map(|_| cursor.next_chunk().unwrap().unwrap().payload.len())
        .collect();
    assert_eq!(
        sizes,
        [7680, 1232, 2048, 1200, 1664, 2816, 2560, 6280, 83540]
    );
    assert!(sizes.iter().all(|size| size % 4 == 0));
}

#[test]
#[ignore = "requires proprietary SLPS-00069 files via KF_RETAIL_DIR"]
fn checksummed_entries_are_the_ones_the_game_verifies() {
    for (name, _, _, holds, fails) in ARCHIVES {
        let bytes = fs::read(com_dir().join(name)).unwrap();
        let archive = Archive::parse(&bytes).unwrap();
        let (mut ok, mut bad) = (0, 0);
        for entry in archive.entries() {
            let entry = entry.unwrap();
            if entry.is_empty() {
                continue;
            }
            if sector_archive::checksum_holds(entry.bytes) {
                ok += 1;
            } else {
                bad += 1;
            }
        }
        assert_eq!((ok, bad), (holds, fails), "{name}");
    }
}
