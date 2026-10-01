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
| GAME.EXE | 32/59 | 21,055/88,376 (23.8%) | unresolved data symbols | unavailable |
| OPEN.EXE | 3/9 | 84/111,252 (0.1%) | linked, 186,368 B | 125,832/186,368 differ (46 header; 125,786 load) |
| END.EXE | 2/8 | 56/107,104 (0.1%) | linked, 172,032 B | 140,791/174,080 differ (46 header; 140,745 load) |

The GAME strict count incorporates the separately verified
`game.resource_startup` literal extent correction and targeted
`game.return_stub_18764` artifact rebuild. The source-claim fractions are
coverage of the loaded-data census, which also includes SDK data and unresolved
gaps; they do not measure reachable data or overall correctness. The current
strict gate has 40 divergent data-owning units: PSX `main`, 27 GAME units,
six OPEN units, and six END units. New source-backed BSS owners increased the
denominator to 77 units; the lower exact fraction does not undo exact function
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
The display source now defines the complete two-buffer primitive memory
(`0x32000` bytes) and typed graphics runtime (`0x17cf0` bytes). The event
counter source defines its startup-cleared `0x78` byte array, and the resource
transition request source defines the seven-word callback state. Their
function listings retain the prior verdicts: display 10/10 identical, event
counter 3/3 identical, and transition request 71.6% WIP. Strict data reports
COMMON allocation for all four BSS objects; it also reports incompatible
section bases for the three nonadjacent display claims. These are source-backed
owners, not strict-data closures. The original event-counter and transition
request TU boundaries remain unproven; the current modules are WIP owners
supported by their central update roles and complete retail extents.
Two adjacent loaded menu arrays now have typed initializers in the window
drawing source: 20 six-halfword sprite descriptors (`0xf0` bytes) and eight
window layouts containing signed glyph indices and `-1` row terminators
(`0x9a0` bytes). The whole `0xa90`-byte `.data` unit is strict-data exact.
Its one function retains the pre-existing 83.5% stack-frame residue, so the
data result does not imply code closure or original TU-boundary proof.
Four supported menu display-state globals are now defined where the menu
setup/restore functions use them: the saved music byte, frame upload pointer
and rectangle, and two saved primitive-buffer records. One of the two
functions remains exact; the other retains its previously documented 97.2%
stack-frame residue. Their retail BSS addresses are noncontiguous, and the
compiler emits COMMON requests, rounding the one-byte and four-byte objects
to eight bytes. The concurrent complete map-object state owner also remains
a strict BSS placement WIP.
The item-model loader owns three adjacent zero-loaded preview controls: two
SDK `SVECTOR` values and their signed rotation step. The retail 20-byte span
is all zero, and explicit source initializers produce a strict-exact `.data`
unit while both loader functions retain identical listings. A supported
eight-byte effect-scatter motion vector was also defined in its central
source; its target BSS placement remains open under COMMON allocation.
The two cursor animation words are zero-loaded, and their central frame
updater now owns an exact eight-byte `.data` section with both functions
unchanged. The item-model allocation flag is also a zero-loaded word owned by
its exact loader/releaser; adding it preserves the full 24-byte `.data`
contents but makes that unit a strict placement WIP because the flag and
preview vectors have noncontiguous retail addresses. A separate 32-byte
motion table in the map-object action source matches retail loaded data.
The menu transition source now owns the only referenced transfer `RECT`:
retail halfwords decode as `{320, 0, 320, 240}`. Its eight initialized bytes
match strictly; two of the unit's three functions remain identical while the
unchanged fade renderer remains a 44.8% WIP.
The central effect-reset source now defines the complete startup-cleared
`0x2a8c`-byte effect state. Its three function listings remain identical, and
the native symbol resolves, but GCC requests `0x2a90` bytes of COMMON rather
than the retail fixed BSS extent.

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
| GAME.EXE | 556 | 13 | `current_poly_ft4`, `render_mask_scan_state`, `DAT_8006d694` |
| OPEN.EXE | 0 | 0 | Native link completes with ten C units |
| END.EXE | 0 | 0 | Native link completes with eight C units |

Every distinct unresolved GAME name has a row in
`config/retail/data_identities.tsv`; no unresolved function name remains.
The GAME diagnostic count includes repeated references and does not count
missing definitions. The preceding 2,780/39 GAME snapshot fell to 556/13
as the player equipment's exact 20-byte loaded table, the complete graphics,
counter, callback, audio, actor, event, map-object, memory-card and menu BSS
objects, and the exact loaded menu and preview arrays were defined. `current_poly_ft4` remains unresolved
because its defining owner is not supported yet. The card-buffer pointer
`memory_card_buffer` also remains unresolved after a tentative definition
changed an exact retail relocation and was reverted. OPEN and END passed
through intermediate states with
20/17, then 13/5, then zero unresolved names as their source-backed data
owners were added. A small `DATA_AT()` claim form mirrors `ADDRESS_AT()` so
one shared movie/display/audio source can retain image-qualified retail data
addresses without duplicating definitions. Its parser, manifest filter, and
compiler size probe all select the claims for the current image.

The four 32-bit memory-card event handles are proven at zero-loaded retail
addresses separated by four-byte gaps, but the containing object and gap
ownership remain unresolved. A probe defining them individually in the
card-event unit failed target delinking because its claims did not describe
the physical gap after that unit's earlier loaded-slot byte. The probe was
fully reverted; their linker names remain unresolved until the containing
source/data layout is proved.

The local evidence files are ignored build products:
`build/link/{psx,game,open,end}/build.json` and each overlay's `LINK.TXT`.
This note does not claim historical linker option attribution;
`/n1024` only raises the pinned linker's module table for the current
reconstruction.
