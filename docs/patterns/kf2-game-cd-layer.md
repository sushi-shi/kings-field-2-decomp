# GAME.EXE: CD archive layer, profile and residues (SLPS-00069)

The first GAME units reconstruct the consumers of the `CD\COM\*.T` archives:
the word/halfword copies, the BCD location helpers, the entry checksum and the
archive/file reader (`0x800180b4..0x800185c0`). 16 of 17 functions match.

## Profile: `-mcpu=r2000`, unlike OPEN and END

GAME fits `probe-gcc257-o2-g0` (KF1's GAME profile), not the plain profile
that fits OPEN and END. The discriminating instruction is the `andi` that
truncates a second `u16` parameter: retail keeps it before the first
dependent `lw`, the plain R3000 model moves it into that load's delay slot.
Under r2000 `cd_archive_open` rises from 93.1% to 99.75% and nothing regresses.

Controls on the same preprocessed unit: the Psy-Q 3.0 kit's GCC 2.4.1 plus
ASPSX 2.08 produces exactly the 2.5.7 probe's plain-profile words, and the
kit's GCC 2.6.0 is worse on every function except `cd_archive_entry_size`.

## Source shapes that decided the match

| Function | Retail evidence | Source |
| --- | --- | --- |
| `cd_archive_entry_size` | `offsets[entry]` is loaded before `offsets[entry + 1]` | `u32 start = offsets[entry]; u32 end = offsets[entry + 1]; return (end - start) << 11;` |
| `cd_archive_read` | `&cd_archives[slot]` is computed once, then both the extent and the table come from it | a `KfCdArchive *archive` local |
| `cd_read_sectors` | `attempt = 0` is scheduled before `failed = 1` | initialized declarations in that order |
| `cd_sectors_corrupt` | `xor` takes the sum first | `return sum != data[last];` |

The three whole-file loaders (`cd_extent_load`, `cd_file_load`,
`cd_file_load_into`) are never called. The delinker classifies their code as
`instruction-word`, not reachable code, so their `jal` rows needed manual
review before the delinked target object carried relocations.

## Residues

- `cd_file_load_into`: retail keeps 8 unused bytes after its `CdlFILE`. OPEN
  and END's `cd_file_load_into` declare `CdlLOC start` there for their own
  seek-back; GAME's copy reads through `cd_read_sectors` but keeps the
  declaration, which reproduces the frame (evidence-gated leftover; see
  `kf2-end-template-residue.md`).
- `cd_archive_open`: retail has 64 unused bytes between the 2048-byte
  sector-0 buffer and the `CdlFILE`. No sibling shows a construct for them;
  unattributed, 99.75%.

The archive entry checksum and the whole `.T` format are documented with the
codec in `tools/README.md`.
