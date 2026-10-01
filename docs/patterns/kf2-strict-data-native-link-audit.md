# Strict data and native link audit

The 2026-10-01 audit uses the hash-checked Japanese `SLPS-00069` retail EXEs,
the pinned Psy-Q shell, `kf verify data --detail --coverage`, and native
`kf link` for each image. It runs no repository tests or lint. The KF2 data
verifier and executable byte comparator are byte-identical to the corresponding
KF1 master scripts, so no verifier port is needed. The data gate compares full
initialized section extents and bytes, ordered relocation referents and REL
addends, BSS allocation class and named layout, and retail placement. A
matching data-owning unit is narrower than whole-image data reconstruction.

| Image | Strict data-owning units | Source claims / loaded-data census | Native EXE | Exact retail-vs-candidate bytes |
| --- | ---: | ---: | --- | ---: |
| PSX.EXE | 0/1 | 73/1,555 (4.7%) | linked, 4,096 B | 277/4,096 differ (43 header; 234 load) |
| GAME.EXE | 30/47 | 18,264/88,376 (20.7%) | unresolved data symbols | unavailable |
| OPEN.EXE | 3/9 | 84/111,252 (0.1%) | linked, 186,368 B | 125,832/186,368 differ (46 header; 125,786 load) |
| END.EXE | 2/8 | 56/107,104 (0.1%) | linked, 172,032 B | 140,791/174,080 differ (46 header; 140,745 load) |

The GAME strict count incorporates the separately verified
`game.resource_startup` literal extent correction and targeted
`game.return_stub_18764` artifact rebuild. The source-claim fractions are
coverage of the loaded-data census, which also includes SDK data and unresolved
gaps; they do not measure reachable data or overall correctness. The current
strict gate has 30 divergent data-owning units: PSX `main`, 17 GAME units,
six OPEN units, and six END units. New source-backed BSS owners increased the
denominator to 65 units; the lower exact fraction does not undo exact function
listings. The GAME check reports zero missing artifacts.
The PSX `.data` referent now matches after load-data carving passes its owning
module's RODATA range to the delinker. The sole PSX data residue is
`overlay_header`: retail target storage is 60-byte `.bss`, while the pinned
compiler emits a 64-byte COMMON request for the authentic 60-byte SDK
`struct EXEC`. Controlled `= {0}` and `-fno-common` probes both moved it into
`.data`, growing that section from 12 to 72 bytes, so neither change was kept.
GAME's remaining divergences are chiefly switch-table `.text` addends and
section extents; `audio_runtime` and the newly defined CD-state BSS, plus
`map_object_action_update` read-only ownership, remain open. The CD source
defines the complete `cd_state` and eight-record `cd_archives` objects at
their curated addresses and sizes. Its focused comparison retains 56 of 57
identical function listings, with the same pre-existing allocation-register
residue in the other function. The compiler emits `cd_state` as a 680-byte
COMMON request for the retail 676-byte BSS object. The neighboring
`cd_stream_work_buffer` stays undefined: its four-byte candidate identity is
only a first-word marker, while source proves at least `0xfa04` readable
bytes and does not establish its complete extent.

The OPEN/END globals use the curated widths, storage classes, names, and
owners in their loader, audio, display, and shared movie-stream sources.
Focused comparisons retained every function listing in those changed units.
The strict gate correctly leaves their BSS as WIP: GCC emits tentative
definitions as COMMON requests instead of fixed `.bss`, often rounding a
2- or 4-byte symbol to eight bytes. The two zero-loaded movie flags also
have a four-byte unclassified retail gap between them, whereas C emits them
contiguously in one `.data` section. Neither padding nor individual linker
placements were introduced to hide those differences.

This COMMON residue is not resolved by a general source-independent compiler
switch found in this audit. The native compiler emits `.comm` requests for
truthful tentative definitions, and the ELF reader intentionally preserves
them as unplaced `SHN_COMMON`; the native linker allocates the final BSS
segment only when forming the EXE. The existing `-fno-common` and explicit
zero-initializer PSX controls moved the 60-byte `struct EXEC` into `.data`,
while the established G8 profile changes small-data instruction selection.
No per-symbol section attribute or forced initializer was retained.

The current native PSX candidate has the same 4,096-byte file extent as
retail. Its first differing file byte is at offset 8 in the header; its first
load-payload difference is at offset `0x84c`, VA `0x8001004c`. Of the 2,048
load bytes, 234 differ. OPEN has the same file extent as retail, with its first
load difference at `0x80011020`. END is 2,048 bytes shorter than retail and
also first differs in its load at `0x80011020`. No current native GAME
candidate EXE exists, so an exact GAME byte-difference count is unavailable.
An aligned-island heuristic or the retail-target round trip is not a candidate
EXE comparison.

## Linker inputs and residual ownership

The pinned Psy-Q 3.0 `LIBC.LIB` has archive symbols and retail signature
matches for `bcopy`, `printf`, and related libc functions in all three
overlays; `LIBCARD.LIB` supplies the GAME card entry points. Both archives
were absent from the native overlay link inputs. Adding `LIBC.LIB` to all
overlays and `LIBCARD.LIB` to GAME reduced OPEN's diagnostics from 545 to 462
and END's from 241 to 188. GAME's first failure was different: with 193
source modules, PSYLINK 1.29 stopped at `LIBGTE.LIB` with `Too many modules
to link`. Its embedded usage advertises `/n value` as the maximum
object-module setting. A scratch re-link of the same compiled inputs with
`/n1024` cleared that limit; the normal GAME link now reaches the unresolved
symbol pass without altering source objects.

| Image | Current native linker diagnostics | Distinct unresolved names | Largest repeated unresolved names |
| --- | ---: | ---: | --- |
| GAME.EXE | 2,780 | 39 | `game_graphics_runtime`, `state_8017d118`, and 37 other curated data names |
| OPEN.EXE | 0 | 0 | Native link completes with ten C units |
| END.EXE | 0 | 0 | Native link completes with eight C units |

Every distinct unresolved GAME name has a row in
`config/retail/data_identities.tsv`; no unresolved function name remains.
The GAME diagnostic count includes repeated references and does not count
missing definitions. OPEN and END passed through intermediate states with
20/17, then 13/5, then zero unresolved names as their source-backed data
owners were added. A small `DATA_AT()` claim form mirrors `ADDRESS_AT()` so
one shared movie/display/audio source can retain image-qualified retail data
addresses without duplicating definitions. Its parser, manifest filter, and
compiler size probe all select the claims for the current image.

The local evidence files are ignored build products:
`build/link/{psx,game,open,end}/build.json` and each overlay's `LINK.TXT`.
This note does not claim historical linker option attribution;
`/n1024` only raises the pinned linker's module table for the current
reconstruction.
