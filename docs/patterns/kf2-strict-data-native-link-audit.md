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
| GAME.EXE | 31/44 | 18,264/88,376 (20.7%) | unresolved data symbols | unavailable |
| OPEN.EXE | 4/4 | 76/111,252 (0.1%) | unresolved data symbols | unavailable |
| END.EXE | 2/4 | 48/107,104 (0.0%) | unresolved data symbols | unavailable |

The GAME strict count incorporates the separately verified
`game.resource_startup` literal extent correction and targeted
`game.return_stub_18764` artifact rebuild. The source-claim fractions are
coverage of the loaded-data census, which also includes SDK data and unresolved
gaps; they do not measure reachable data or overall correctness. The remaining
16 divergent data-owning units are PSX `main`, 13 GAME units, and two END units.
The PSX unit
differs in one `.data` relocation referent and in `overlay_header` storage:
the retail target carries a 60-byte `.bss` allocation, while the current
compiler emits a 64-byte COMMON request. This is an allocation-model issue;
changing the source to force a score without proving the original declaration
would lose that evidence. GAME's remaining divergences are chiefly switch-table
`.text` addends and section extents; `audio_runtime` BSS and
`map_object_action_update` read-only ownership remain open. Five newly
defined END globals in `end.main` and `end.audio` preserve all three focused
function listings, but the compiler emits 8-byte COMMON requests for their
retail 2- or 4-byte BSS identities. Their strict data mismatch remains WIP;
four audio identities also occupy widely separated retail addresses and do
not have one defensible contiguous source-section base.

The current native PSX candidate has the same 4,096-byte file extent as
retail. Its first differing file byte is at offset 8 in the header; its first
load-payload difference is at offset `0x84c`, VA `0x8001004c`. Of the 2,048
load bytes, 234 differ. No current native GAME, OPEN, or END candidate EXE
exists, so no exact byte-difference count can be stated for those images.
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
| GAME.EXE | 5,762 | 46 | `player_state` (2,590), `game_graphics_runtime` (624), `state_8017d118` (352) |
| OPEN.EXE | 462 | 20 | `current_poly_ft4` (198), `dec` (68), `display_buffers` (64) |
| END.EXE | 150 | 12 | `dec` (68), `display_buffers` (34), `display_current` (22) |

Every distinct unresolved name has a row in
`config/retail/data_identities.tsv`; there is no unresolved function name in
these transcripts. They remain undefined in source because the owning module,
initialized bytes or BSS reservation, and whole-image allocation placement
must be reconstructed. The diagnostic counts include repeated references to
the same names and are not counts of missing source definitions.

The END source pass defined `ending_data` in its loader unit and the four
audio pointers/IDs in the unit that assigns them, with retail-backed widths,
storage, and identity names. It reduced END's distinct unresolved names from
17 to 12, and its diagnostics from 188 to 150. The strict END data gate moved
from 3/3 to 2/4 because current compiler COMMON requests do not prove fixed
retail BSS placement. No attributes, padding, or linker equates were added to
manufacture a data match.

The local evidence files are ignored build products:
`build/link/{psx,game,open,end}/build.json` and each failed overlay's
`LINK.TXT`. This note does not claim historical linker option attribution;
`/n1024` only raises the pinned linker's module table for the current
reconstruction.
