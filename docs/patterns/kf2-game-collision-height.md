# GAME collision height wrappers

The contiguous GAME run `0x8002b604`–`0x8002c670` now has one C owner,
`game.collision_height_wrappers`. The first helper preserves five arguments,
samples the map at Y minus 1280 through `0x8002a988`, calls the five-argument
collision dispatcher `0x8002aaa4`, and returns the signed result at
`0x801d8d50`. The second helper indexes the ten-byte map-cell grid by
arithmetic-shifted X/Z coordinates, selects elevation byte +1 or +6 according
to the unsigned kind byte, records the selected layer and negative elevation
scaled by 128, calls the same dispatcher, and returns that result. The source
keeps the existing occupancy writer and exact probe-offset wrapper in address
order. Direct calls, the internal jump, and eight HI16/LO16 BSS/grid pairs
were reviewed from retail instructions.

The next helper, `0x8002b874`, snapshots collision position and dimensions
from the player camera, a selected actor, or a selected map object and its
template. `0x8002b9d4` dispatches the grid, actor, map-object, and player
collision channels according to mode bits and returns their combined flags.
Its sixth argument is the mode and its fifth argument is masked to 28 bits
for the later scans. The `0x40` actor scan contributes a hit bit but clears
the cached actor index afterward, as the retail stores show. The `0x8002b7f8`
wrapper returns the collision dispatcher's value, which its `0x8002b9d4`
caller consumes; its exact listing is preserved with the corrected signature.
All direct calls, internal jumps, and BSS/state HI16/LO16 pairs in the added
functions have been reviewed.

The same unit now includes the six default-row collision helpers
`0x8002bc18`–`0x8002bfd4`, the mask-segment rasterizer at `0x8002bfd4`,
and the three mask-fill helpers `0x8002c170`–`0x8002c424`. This follows the
continuous retail address run and removes three Psy-Q link modules. The
source-owned 80-row default table remains byte-exact at 3520/3520. Focused
comparison preserves all nine exact function listings in the combined unit;
strict objdiff reports 9/17 at 100% after the rasterizer claim. The prior
seven WIPs retain their scores, with no normalized function or data regression
from the merge.

The immediately following `0x8002c424` raster scan now has a semantic C claim
in the same continuous unit. Its six arguments are two mask sample offsets,
the map step, a signed window step, mask stride, and count, as shown by eight
calls from `0x8002c670`. It reads the existing scan-state bytes at
`0x801b5a78/79`; the earlier `0x801b5a64/65` reading was incorrect. Its
source models the two occupancy-layer checks, 24-by-24 window bounds, 80-by-80
map bounds, cursor update, and final state writes. The BSS allocation owner
of the shared `0x801b5a70` state is still unresolved, and `0x8002c670`
has a separate WIP source claim. The new rasterizer was initially WIP at
48.9932% strict objdiff;
focused comparison reports 23/19 CFG blocks and 14/12 branches, with the
first control difference at the pre-loop count check. Its direct BSS
HI16/LO16 pairs and internal jumps have been reviewed. Retail carries two
neighbor-mask cursors independently and skips advancing the second cursor
when a valid second layer has neither requested bit. The initial source
advanced both cursors unconditionally. The revised source keeps that
conditional update and tests the two mask bytes separately; focused CFG
improves from 23/19 blocks and 14/12 branches to 23/22 and 14/14. Strict
objdiff improves from 48.9932% to 55.789116%; all nine exact neighbors are
preserved. A direct-global spelling raised the focused listing from 30.5% to
36.5% but lowered strict objdiff to 52.77551% without adding a source-level
fact, so the typed local scan-state view was restored. The GAME target relink accepts this
unit; global closure is currently limited by three unrelated switch-table
addends and incomplete known-reference ownership.

Strict objdiff reports `0x8002b604` at 100%, `0x8002b67c` at
94.895836%, `0x8002b73c` at its preserved 98.40426% WIP, and `0x8002b7f8`
at 100%. The third-argument Z coordinate to `0x8002a988` was proven by its
callee body and by the incoming `$a2` preserved across the call; spelling that
argument in source resolved the first helper's saved-register residue. The
second retains an elevation register and global-height reload difference and
remains WIP. The exact probe-offset wrapper remained identical after the merge.
`0x8002b874` is 91.5% strict with branch and load scheduling residue;
`0x8002b9d4` is 89.4% strict with 23/23 CFG blocks and 12/12 branches but
different block successors and register scheduling. Both are WIP.

Two controlled declaration-order probes for `0x8002b73c` retained its
semantics but reduced focused similarity from the current listing. The
current source preserves the retail call-free CFG and memory accesses; its
remaining register-lifetime residue has no attributed compiler mechanism.

The cache at `0x801d8d40`–`0x801d8d84` lies inside the complete
startup-cleared `bss_801c7540` claim. It overlaps the provisional 20-record
equipment view. Retail equipment pointer construction has no proven upper
item-ID bound, so source uses temporary interior accesses and does not define
an overlapping global or assert an 18-record boundary. The SDK-looking
`0x80058298` leaf was separately identified as LIBSND `SpuVmDamperOff` and
excluded from game reconstruction.

| GAME address | Verdict | Remaining evidence |
| --- | --- | --- |
| `0x8002b604` | Exact, 100% | Five-argument height wrapper. |
| `0x8002b67c` | WIP | Elevation load and global-height reload differ. |
| `0x8002b73c` | WIP | Grid footprint CFG agrees; register lifetimes differ. |
| `0x8002b7f8` | Exact, 100% | Probe-offset wrapper. |
| `0x8002b874` | WIP | Actor/object radius and height load schedule differs. |
| `0x8002b9d4` | WIP | Collision channel CFG and result scheduling differ. |
| `0x8002bc18` | Exact, 100% | Default row helper. |
| `0x8002bd3c` | Exact, 100% | Default row helper. |
| `0x8002bdbc` | Exact, 100% | Default row helper. |
| `0x8002be9c` | Exact, 100% | Default row helper. |
| `0x8002bf38` | Exact, 100% | Default row helper. |
| `0x8002bfac` | Exact, 100% | Default row helper. |
| `0x8002bfd4` | WIP | Mask-segment rasterizer control and data placement differ. |
| `0x8002c170` | WIP | Scan CFG agrees; pointer and state registers differ. |
| `0x8002c1d4` | Exact, 100% | Row-fill caller. |
| `0x8002c290` | WIP | Mask-fill control and cursor scheduling differ. |
| `0x8002c424` | WIP, 55.789116% | Neighbor-mask cursors now follow retail; 23/22 CFG blocks and 14/14 branches. |

## Collision-cache and mask continuation

A fresh GAME retail pass covered `0x8002a988`, the eight non-exact bodies in
the contiguous collision-height unit, and the map-object dispatcher at
`0x80036ed4`. The sampler's shared layer-selection control flow is now strict
exact. The height unit's nine exact neighboring functions and its 3520-byte
default table remain exact. All ten bodies are game-specific map/collision
operations; the selected retail bodies contain no string references or
vendored-library signature.
The first WIP percentages below are retained from that strict report;
the later mask-scan rows state focused listing results because full-image
certification is pending.

| GAME address | Retained verdict | Evidence or first unresolved residue |
| --- | --- | --- |
| `0x8002a988` | Exact, 100% (284/284) | Retail's backward alternate-layer branch and shared write blocks now match; fallback data remains 10/10. |
| `0x8002b67c` | WIP, 94.895836% | Correct four-block CFG and referents; retail reloads cached height as call argument after storing it, while compiled C carries the elevation value. |
| `0x8002b73c` | WIP, 98.404260% | Correct eight-block footprint loop, four branches, and map-grid reference; row pointer and loop-index registers differ. |
| `0x8002b874` | WIP, 91.5% | Player/actor/map-object snapshot is modeled, but the actor/object radius and interaction-height load order and common-tail schedule differ. |
| `0x8002b9d4` | WIP, 89.4% | Grid, actor, map-object, and player call set is modeled; 23-block CFG has a differing actor-scan successor and result lifetime. |
| `0x8002bfd4` | WIP, 54.155340% | Both mask-segment axes are modeled; coordinate and render-grid address scheduling diverges before the raster loops. |
| `0x8002c170` | WIP, 89.8% | Eleven-block mask-row scan and bounds agree; pointer and state registers differ. |
| `0x8002c290` | Focused listing SAME; strict certification pending | The two-layer mask update now matches in the focused object; the paired mask sweep remains WIP. |
| `0x8002c424` | WIP, 73.5% focused listing | The second-neighbor cursor advance and empty-count setup follow retail; target/compiled CFG has 23/22 blocks and the remaining register and address schedule is unresolved. |
| `0x8002c670` | WIP, 75.8% focused listing | Eleven target and compiled blocks, four branches, and one return agree. Retail stores the selected collision-cache layer at scan-state +0 and its alternate `5 - layer` at +4; the first-lighting access uses the retail BSS+4 field-base referent. Mask-byte addressing and traversal scheduling remain different. |
| `0x80036ed4` | WIP, unclaimed | The 0x1df4-byte no-argument map-object dispatcher has three bounded indirect jumps. Their raw pointer-table extents and in-function targets are established; case reachability and callback targets remain unresolved. |

At `0x80036f70`, the dispatcher subtracts two from its opcode and rejects
values above `0xdf`; its `jr` at `0x80036f98` indexes the 224 pointer words
from `0x8001191c` through `0x80011c98`. Every Japanese retail pointer lands
inside `0x80036ed4`–`0x80038cc7`; 201 point to the default block at
`0x80038c5c`. A second `jr` at `0x80037a18` subtracts one from a subaction,
rejects values above eight, and indexes nine adjacent pointer words from
`0x80011c9c` through `0x80011cbc`. The primary opcode `0x53` enters this
secondary switch. All nine pointers target dispatcher blocks. The primary
opcode `0x59` enters a third `jr` at `0x80037cdc`; it subtracts one from
another subaction,
rejects values above four, and indexes five pointers at `0x80011cc4`
through `0x80011cd4`. All five target dispatcher block heads. The zero
word at `0x80011cc0` separates the latter two tables, and the distinct
actor table begins at `0x80011cd8`.
The primary table has nondefault opcode entries at `0x02`–`0x05`, `0x08`,
`0x09`, `0x0f`–`0x13`, `0x16`, `0x22`, `0x51`, `0x53`, `0x54`, `0x58`,
`0x59`, `0x60`–`0x62`, `0xe0`, and `0xe1`.
All 24 unique first-table, six unique second-table, and five third-table
targets are block heads, with none in a delay slot. These bounds establish
table extents without proving every case reachable or the source owner.
The direct `lui`/`addiu` base pairs at `0x80036f84/88`, `0x80037a04/08`,
and `0x80037cc8/cc` are reviewed; their indirect case-pointer
rows remain candidate relocations.

The sole proven direct caller is `game_main_loop` at `0x800138d8`; it sets no
arguments for this call and ignores its result. The dispatcher reads no
incoming argument register and prepares no return value, supporting a `void`
no-argument identity. The default case at `0x80038c5c` loads
`state_8017d118.active_table` at BSS `0x8017d124`, then calls table entry
`+0x24` (index 9) through `jalr`. Another case at `0x80038a9c` reads the
same active table, then calls entry `+0x0c` (index 3) with the current map
object pointer. Both direct BSS address pairs are reviewed. The callback
table's storage owner is resolved, but the selected function pointers remain
indirect and have no proved callee identities.

Two other dispatcher cases pass ordered start/end motion offsets to the exact
`func_80036b68` helper. Four direct address pairs cover one 0x20-byte load
datum at `0x8006d6e4`, now typed as SDK `SVECTOR[4]`. Japanese retail bytes
decode to `(0,-1424,0,0)`, `(0,-912,0,0)`, `(0,-100,300,0)`, and
`(0,0,64,0)`; the next curated row begins at `0x8006d704`. The defining TU and
original linkage remain unproved, so its identity stays address-derived and
the giant dispatcher remains unclaimed.

The provisional `bss_801c7540.map_cells[88][80]` spans `0x11300` bytes, while
`0x8002aaa4` directly reads a shape-table base at BSS offset `0x10000`.
This overlap does not yet prove that the map grid has only 80 rows:
`0x80035894` passes an unguarded byte-valued source row to `0x80035194`,
and `0x80036ed4` has additional rectangle-copy calls. Rows beyond 79 may
share or repurpose the region. Keep the grid extent and shape-bank owner WIP
until those coordinate ranges and all consumers are established.

The strict `game.collision_grid_sample` report relinked 142/142 GAME target
units. Overall edge-check remains open on three unrelated `.rodata` addends
and incomplete known-reference ownership. The raw-backed dispatcher tables
above do not resolve those unrelated edge gaps, and no cache boundary was
guessed from the overlapping views.
