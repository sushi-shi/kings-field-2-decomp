//! Host transport for testing the allocation-free codec library.
//!
//! Requests and responses use a little-endian block count followed by
//! `(u32 length, bytes)` for each block. Only this host executable uses std.

use std::io::{self, Read, Write};

fn decode_blocks(bytes: &[u8]) -> Result<Vec<Vec<u8>>, String> {
    let mut at = 0usize;
    let mut word = || -> Result<usize, String> {
        let end = at.checked_add(4).ok_or("frame offset overflow")?;
        let value = bytes.get(at..end).ok_or("truncated frame count")?;
        at = end;
        Ok(u32::from_le_bytes(value.try_into().unwrap()) as usize)
    };
    let count = word()?;
    if count > 128 {
        return Err("too many frame blocks".into());
    }
    let mut result = Vec::with_capacity(count);
    for _ in 0..count {
        let header_end = at.checked_add(4).ok_or("frame offset overflow")?;
        let header = bytes.get(at..header_end).ok_or("truncated block length")?;
        let size = u32::from_le_bytes(header.try_into().unwrap()) as usize;
        let end = header_end.checked_add(size).ok_or("block size overflow")?;
        result.push(
            bytes
                .get(header_end..end)
                .ok_or("truncated block")?
                .to_vec(),
        );
        at = end;
    }
    if at != bytes.len() {
        return Err("trailing bytes after request".into());
    }
    Ok(result)
}

fn encode_blocks(blocks: &[Vec<u8>]) -> io::Result<()> {
    let mut out = io::stdout().lock();
    out.write_all(&(blocks.len() as u32).to_le_bytes())?;
    for block in blocks {
        let length = u32::try_from(block.len()).map_err(io::Error::other)?;
        out.write_all(&length.to_le_bytes())?;
        out.write_all(block)?;
    }
    Ok(())
}

fn word(block: &[u8], what: &str) -> Result<u32, String> {
    block
        .try_into()
        .map(u32::from_le_bytes)
        .map_err(|_| format!("{what} must be a little-endian u32"))
}

fn execute(operation: &str, blocks: Vec<Vec<u8>>) -> Result<Vec<Vec<u8>>, String> {
    use kf_codec::cd_location::CdLocation;
    use kf_codec::sector_archive::{self, Archive};

    match operation {
        // Sector 0 of a .T archive -> the copied offset table and the byte
        // size the game allocates for it.
        "archive-open" => {
            let [sector] = &blocks[..] else {
                return Err("archive-open expects sector 0".into());
            };
            let archive = Archive::parse(sector).map_err(|error| error.to_string())?;
            let entries = usize::from(archive.count()) + 1;
            let mut table = Vec::with_capacity(entries * 2);
            for index in 0..entries {
                table.extend_from_slice(&archive.offset(index).unwrap().to_le_bytes());
            }
            let size = u32::try_from(table.len()).map_err(|error| error.to_string())?;
            Ok(vec![table, size.to_le_bytes().to_vec()])
        }
        // Archive bytes, u32 entry index and the archive's CdlLOC -> the
        // entry, the location it is read from, its sector count, and whether
        // its checksum holds.
        "archive-read" => {
            let [archive, entry, base] = &blocks[..] else {
                return Err("archive-read expects archive, entry, base location".into());
            };
            let archive = Archive::parse(archive).map_err(|error| error.to_string())?;
            let index = word(entry, "entry")? as usize;
            let entry = archive.entry(index).map_err(|error| error.to_string())?;
            let base: [u8; CdLocation::BYTE_SIZE] = base[..]
                .try_into()
                .map_err(|_| "base location must be four bytes")?;
            let location = CdLocation::from_bytes(base).add(u32::from(entry.start_sector));
            Ok(vec![
                entry.bytes.to_vec(),
                location.to_bytes().to_vec(),
                u32::from(entry.sector_count()).to_le_bytes().to_vec(),
                vec![u8::from(sector_archive::checksum_holds(entry.bytes))],
            ])
        }
        _ => Err(format!("unknown codec operation {operation:?}")),
    }
}

fn main() {
    let result = (|| -> Result<(), String> {
        let operation = std::env::args().nth(1).ok_or("missing codec operation")?;
        let mut request = Vec::new();
        io::stdin()
            .take(64 * 1024 * 1024 + 1)
            .read_to_end(&mut request)
            .map_err(|error| error.to_string())?;
        if request.len() > 64 * 1024 * 1024 {
            return Err("request exceeds 64 MiB".into());
        }
        let response = execute(&operation, decode_blocks(&request)?)?;
        encode_blocks(&response).map_err(|error| error.to_string())
    })();
    if let Err(error) = result {
        eprintln!("kf-codec-oracle: {error}");
        std::process::exit(1);
    }
}
