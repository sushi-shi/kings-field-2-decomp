# GAME resource codecs

`kf-codec` is a dependency-free `#![no_std]` library with no allocator, built
the same way as King's Field's: readers borrow resource bytes, writers use
caller-owned buffers, and multi-byte fields have explicit byte order. Each
format is transcribed from its retail `GAME.EXE` consumer, not from the
matching C reconstruction, so it can serve as an independent implementation in
differential tests.

| Module | Format | Retail consumer |
| --- | --- | --- |
| `sector_archive` | `CD\COM\*.T`: `u16` count, `count + 1` `u16` sector offsets in sector 0, whole-sector entries; checksummed entries end in `0x12345678` plus the sum of the preceding words | `cd_archive_open` @`0x800184d0`, `cd_archive_read` @`0x800182d0`, `cd_sectors_corrupt` @`0x80017d00` |
| `cd_location` | BCD `CdlLOC` arithmetic, counted from 00:00:00 without LIBCD's 150-sector lead-in | `cd_location_add` @`0x80017bf0` and its helpers |
| `chunked` | `u32` length-prefixed chunks; `FDAT.T` entry 48 holds nine common tables | `func_80015d58` |

The seven `.T` archives hold every GAME resource: TIM and TMD (`ITEM`, `TALK`),
VAB headers, bodies and SEQ (`VAB`), and game-specific motion (`MO`), map
(`FDAT`, `RTIM`, `RTMD`) data still to be decoded.

```sh
nix develop --command cargo test --offline --manifest-path tools/Cargo.toml

# Requires the extracted, hash-verified SLPS-00069 disc directory
# (the one holding GAME.EXE and CD/COM):
KF_RETAIL_DIR=/path/to/disc nix develop --command \
  cargo test --offline --manifest-path tools/Cargo.toml --test retail_corpus -- --ignored
```

The corpus test re-encodes all seven archives byte for byte, checks the
common-table chunk sizes, and checks that exactly the entries the game reads
through `cd_archive_read` carry a valid checksum (`RTIM.T` and the VAB bodies
are streamed without one).

## Three-way oracles

As in King's Field, each codec is compared with its retail consumer and with
the reconstructed C for that consumer, both executed by `scripts/kf/parser_machine.py`
under Unicorn with the same declared services:

```sh
# Requires `kf init --retail-dir` with the extracted SLPS-00069 disc directory
# (it holds both the executables and CD/COM), or --retail-dir:
nix develop --command python -m scripts.kf.codec_oracle
nix develop --command python -m scripts.kf.sector_archive_oracle
```

`sector_archive_oracle` opens all seven archives and reads every checksummed
entry (plus one corrupted-first-read control per archive) through retail
`cd_archive_open`/`cd_archive_read` and their reconstructed C closures.
It compares the archive slot, the copied offset table, destination bytes and
the ordered CD, event and allocation services with each other and with the
Rust codec. CD, BIOS and heap services are deterministic Python hooks, not
emulation of the drive; they are declared in the oracle's docstring.

King's Field II GAME uses all 2 MiB of RAM (`.bss` to `0x801da018`, heap to
`0x801f8000`), so the harness places candidate code, hook stubs and its return
sentinel in a harness-only window at `0x80200000`.

Malformed-input checks keep the API bounded: retail
trusts the entry index and the table's fit in sector 0, and rejecting such
input is not a claim to reproduce retail's unchecked access. No proprietary
resource bytes are stored in this workspace.
