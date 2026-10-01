# GAME collision height wrappers

The contiguous GAME run `0x8002b604`–`0x8002c66f` now has one C owner,
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
neighbor-mask cursors independently. When a valid second layer has neither
requested bit, the branch-delay slot advances the second cursor once before
bypassing the shared increment; the first cursor advances at the common tail.
The revised source uses a separate jump for that path and tests the two mask
bytes separately; focused CFG
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
| `0x8002b874` | WIP, 85.8% focused listing | Player/actor/map-object snapshot is modeled, but the actor/object radius and interaction-height load order and common-tail schedule differ. |
| `0x8002b9d4` | Prior strict WIP, 89.4% | Grid, actor, map-object, and player call set was modeled; the prior actor-scan successor difference is corrected in the focused pass below, while result lifetime remains unresolved. |
| `0x8002bfd4` | Prior strict WIP, 54.155340% | Both mask-segment axes are modeled; the focused branch correction below has not been recertified strictly. |
| `0x8002c170` | WIP, 67.9% focused listing | Eleven-block mask-row scan, unsigned 24-cell bound, byte comparison, and signed step agree; pointer and state register lifetimes differ. |
| `0x8002c290` | Focused listing SAME; strict certification pending | The two-layer mask update now matches in the focused object; the paired mask sweep remains WIP. |
| `0x8002c424` | WIP, 73.5% focused listing | The second-neighbor cursor advance and empty-count setup follow retail; target/compiled CFG has 23/22 blocks and the remaining register and address schedule is unresolved. |
| `0x8002c670` | WIP, 76.4% focused listing | Eleven target and compiled blocks, four branches, and one return agree. Retail stores the selected collision-cache layer at scan-state +0 and its alternate `5 - layer` at +4. The current typed center-cell expression forms the first lighting address differently from retail's BSS+4 field-base referent; mask traversal scheduling also differs. |
| `0x80036ed4` | WIP, 27.7% focused listing | The 0x1df4-byte no-argument map-object dispatcher has a source claim and three bounded indirect jumps. Its 956-byte table section and 238 relocation referents are present, while case-pointer addends and text layout differ; indirect callback targets remain unresolved. |

The current 27-function focused cohort comprises the 17 collision-height
wrappers, two map-cell pattern helpers, six map-object reset helpers, the
placement initializer, and the mask sweep. Its retained listings are 15 SAME
and 12 DIFF; no new strict 100% certification is claimed.

The retained `0x8002b9d4` actor branch now uses one common reset of
`COLLISION_CACHE_ACTOR_INDEX` after its optional mode-`0x40` scan, matching
retail's `0x8002bb44`/`0x8002bb48` join. An isolated source probe changed
the focused listing from 58.0% to 59.4% and made the 23/23-block CFG's
known successor lists agree by block order. The shared focused build keeps
10/17 SAME collision-height listings; the other six WIP neighbors and
`0x8002b9d4` remain non-exact. The earlier strict report row above is
historical and has not been recertified under the user's focused-build-only
constraint.

In the mask-segment rasterizer `0x8002bfd4`, retail assigns the positive X
step in the nonnegative-delta arm and jumps over it from the negative arm.
The source now uses that explicit `else` after setting the negative step.
Its focused listing improves from 16.0% to 18.3%, with target/compiled CFG
moving from 22/21 to 22/22 blocks and known successors agreeing by block
order. The 10/17 SAME sibling listings remain unchanged; the rasterizer's
coordinate and grid-address schedule is still WIP.

The later source-backed truncation of both grid subscripts to `u16` and the
retail-shaped positive Z-step assignment raise the current focused listing
to 35.5%, still DIFF. A fresh focused build keeps 10/17 SAME siblings.

Retail `0x8002c670` forms a saved pointer to the scan state's +0x0c field
and uses it for several relative stores. An isolated C probe that changed
all state accesses to one local `KfRenderMaskScanState *` produced a larger
frame and 46.3% focused similarity; it was discarded. Relative retail
addressing alone does not establish the original C pointer spelling.
Its seven-pair pitch interpolation uses the source-owned
`DAT_80067874` table; raw `0x8002c6a8/6ac` constructs that exact load
address and is now a reviewed direct pair. The focused listing remains
75.8% with identical diff output after this relocation correction.

The adjacent map-object focused controls remain stable: `0x80036078`,
`0x800363bc`, `0x800363dc`, `0x800365d8`, `0x800366fc`, `0x800368b4`,
`0x800369b8`, `0x80036ad8`, and `0x80036b68` all have identical focused
listings. `0x80036190` remains WIP at 82.9% focused similarity: its
return/loop successor layout and index lifetime differ. Removing its
separate entry guard in an isolated source probe produced 82.6% and moved
the loop farther from retail, so the source was retained. `0x80036464`
remains WIP at 92.8%, with saved-register and initial-store scheduling
differences. These focused verdicts are listing checks, not strict objdiff
closure.

The two branches of `map_object_set_cell_marker` each load the object's Z
and X coordinates before forming the map-cell address in retail. A temporary
source probe with explicit coordinate locals preserved all five SAME
listings in `game.map_object_reset`, but left this function at 55.9% and
changed only its arithmetic/address schedule. No source change was retained;
the duplicated marker writes and scale updates still agree with retail.

The map-placed expansion at `0x80034818` is also focused-listing SAME. It
expands 128 typed 24-byte destination records, sampling elevation through
the existing `0x8002b67c` collision helper; its source and layout were not
changed in this pass.

The 0x74-byte gap `0x80036944`–`0x800369b7` is executable code, not the
seeded unclassified data/string split. Raw words show a 40-byte frame,
saved `$ra` and `$s0`–`$s3`, one return at `0x800369b0`, and a 396-object
loop that passes each 68-byte object and the low input byte to
`0x800368b4` at `0x80036980`. Input byte `0xff` skips the loop. The
four-byte string seed at `0x8003697b` intersects the little-endian bytes
of `move a0,s1` and the following `jal`; it is a false string detection.
No aligned `j/jal` or 32-bit pointer word in GAME.EXE targets `0x80036944`,
and no matching low-immediate address constructor was found. The entry's
caller ABI and return use remain unproved. The contiguous `0x74`-byte
function boundary and address-derived `func_80036944` identity are curated;
the three overlapping false data/string seeds were removed. A later source
claim emits 116/116 identical function bytes in isolated direct objdiff; its
`u8` parameter and `void` return remain candidate without an incoming
caller. The preceding `0x800368b4` returns at `0x8003693c` with its
`0x80036940` delay slot; the following claimed function starts exactly at
`0x800369b8`. A safe GAME one-VA carve admits the complete target with no
withheld function, but withholds its direct `jal` at `0x80036980` as
`non-reachable-code-channel`: no static caller establishes reachability.
The same `0x800368b4` callee has three proven direct calls from event
controller `0x8004678c` at `0x8004684c`, `0x800469d4`, and `0x80046d50`.
Those callers use its result to select event paths; `0x80036944` discards
that result on every loop iteration. This constrains the callee, but does
not supply an incoming signature or return consumer for `0x80036944`.
At this function's epilogue, the marker-`0xff` fast path leaves `$v0=0xff`
and the completed loop leaves `$v0=0` from its count check. Those residual
register values do not prove an integer return without a caller.

KF1's `map_object_pool_trigger_link(u8)` and `map_object_pool_clear_link(u8)`
use the same 396-record/count-down loop idiom; this supports the loop model
and byte-width hypothesis, not a semantic name or a KF2 callsite.

The related map-object band `0x80034f90`–`0x80036ed3` had 17 remaining
candidate direct `j/jal` rows. All 17 retail words encode their curated
opcode class and target. Sixteen are in identified reachable functions;
their internal jumps land on CFG block heads and their external calls name
the decoded callees, so those rows are now reviewed. The seventeenth is
the `0x80036980` call in the newly bounded but caller-unresolved
`0x80036944` function and remains candidate. Focused controls after the
review retain 5/6 identical listings in `game.map_object_reset`, 1/1 in
`game.map_object_collision_query`, and 2/2 in
`game.map_object_vertex_world`; the sole reset-unit WIP is the
historical `map_object_set_cell_marker` at 55.9% focused similarity.

A later raw-word pass checked all 54 still-candidate `mips26` rows in
`0x8002aaa4`–`0x8002ce2b`: every word has the curated `j`/`jal` opcode and
target, and each site lies in an identified function. A safe one-VA carve of
`0x8002c670` already materializes its decoded direct calls (82 target
relocations, none withheld), so no candidate tier was promoted merely to
change the focused listing. The `0x80035194` rectangle copier's apparent
extra zero-width branch is an instruction-order difference: retail also
checks `width - 1 == -1` at each row's inner-loop entry. Its source and
the 92.5% `0x80034f90` neighbor therefore remain unchanged in this pass.
Focused `0x8002c424` and `0x8002c670` listings remain 73.5% and 75.8%; the
adjacent spawn/scatter three-function and vertex-world two-function units
remain all SAME listings. These are focused listing verdicts, not strict
closure.
Direct target/compiled disassembly of `0x8002c424` accounts for its 23/22
block count: retail's two first-layer-mask branches enter one instruction
apart, while the compiled object schedules the common shift in their delay
slots and merges the entries. The second-neighbor failure advances its
cursor in a jump delay slot in both objects, skipping the ordinary advance.
The source-level scan behavior is unchanged by this block-count difference.

The exact `0x8002bc18` copy uses `lui t1,0x8006; addiu t1,0x6ab4` to form
the source-owned `collision_default_rows` base. The raw pair, its
`0x80066ab4` target, and the 80-record copy establish a reviewed direct
relocation. A safe one-VA carve retains the pair, and the focused
collision-height unit remains 10/17 SAME listings. A separate apparent
`rcos` xref at `0x8005df68/6c` forms `0x8006749c + angle * 2` only for
angles 3072–4095, actually reading `0x80068c9c`–`0x8006949a`. Its base
literal happens to fall within `collision_default_rows`; the dynamic reads
do not, so it is not evidence to change that datum's owner.

Fresh direct comparison keeps `0x8002b67c` WIP at 94.895836% (87.1%
focused). Its five-argument kind/cell wrapper has the same 4/4 CFG blocks,
single branch, occupancy-cell address, unsigned elevation loads, cache
writes, and `0x8002aaa4` call. The first differences choose `$a1` instead
of retail `$v0` for the elevation and omit a later cache-height reload and
its HI16/LO16 pair after the preceding store. The shared C still references
that cache field; no source-visible alias or intervening call supports
forcing another memory read. Ten other functions in the focused
seventeen-function collision unit remain exact.

The placement helpers also retain their focused WIP verdicts:
`0x80034f90` is 92.5% with equivalent 10-byte pattern writes but a
different saved-register assignment; `0x80035194` is 58.8% with a 40-byte
retail frame versus a 32-byte compiled frame and unresolved register
lifetimes across the rotated rectangle copy; and `0x80035894` is 92.7%
with matching record initialization but different early store and
80-column address arithmetic schedules. No ownership or width correction
was supported by these differences.

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
table extents without proving every case reachable or the original TU owner.
The direct `lui`/`addiu` base pairs at `0x80036f84/88`, `0x80037a04/08`,
and `0x80037cc8/cc` are reviewed; their indirect case-pointer
rows remain candidate relocations.
The retail data census retains separate 224-, nine-, and five-pointer rows,
while the current source claims one contiguous `0x3bc`-byte RODATA owner for
the tables and their intervening zero word. A focused current-source object
emits a 956-byte `.rodata` section and 238/238 relocation rows at the same
offsets, with identical types and `.text` referents. Pointer addends still
differ at 236/238 rows because the compiled case blocks have different text
offsets (205 of those deltas are -8); this is not a strict data match.
The nine secondary targets in index order are `0x80037a20`, `0x80037a88`,
`0x80037b10`, `0x80037b34`, four copies of `0x80038c78`, and
`0x80037b58`. The five tertiary targets are `0x80037ce4`,
`0x80037d00`, `0x80037d88`, `0x80037db8`, and `0x80037de4`.
All 180 curated direct `j/jal` words in the dispatcher, from `0x80036fc0`
through `0x80038c54`, have been decoded against the Japanese retail bytes:
all opcode classes and encoded targets agree with their curated rows. Their
internal jump targets are dispatcher block heads, so those direct rows are
reviewed; table-pointer rows and the two `jalr` targets remain unresolved.
Six additional HI16/LO16 pairs were checked against the retail words and
signed lows: the outer-loop object array at `0x80036f00/04`, camera position
at `0x80036f18/1c`, current-object store at `0x80036f38/3c`, template
array at `0x80036f54/58`, current-template store at `0x80036f60/64`, and
current-object reset at `0x80038c90/94`. The dispatcher indexes a 24-byte
template with the current object's halfword ID before storing that pointer
at `map_object_state+0x8734`. The existing 0x8744-byte state layout now
types that slot as `KfMapObjectTemplate *`; no new datum is claimed. The
object pointer lives in the adjacent `+0x8738` slot. Both pointers have
reviewed direct address pairs, while their dynamic uses remain separate
control-flow questions.
One dispatcher case supplies `KfMapObjectTemplate` +0x0c as an unsigned
halfword vertex index to `0x800369b8`, then supplies +0x0e and +0x10 as
unsigned halfword collision radius and height to `0x8002b9d4` in mode
`0x90` (`0x80038324`–`0x80038354`). Other proven consumers read the same
+0x0c/+0x0e bytes as signed pose offsets or individual marker bytes, so
the collision interpretation is a variant-specific view; the shared
template layout has not been globally changed.
In the same action branch, unsigned template byte +0x16 is passed to
`map_object_play_spatial_sound`, while byte +0x17 is passed to
`map_object_set_cell_marker`. The latter agrees with the existing
`marker_action_51` field; +0x16 remains an evidence-only sound selector.

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
The transition driver at `0x80016820` installs either the initialized
32-entry default table at `0x80063e00` or the CD-populated table at
`0x8019e138`. Default slots 3 and 9 both contain the no-op stub
`0x80015d50`; the alternate table's contents are not fixed by the retail
image, so neither indirect call can be promoted to that stub.

Two other dispatcher cases pass ordered start/end motion offsets to the exact
`func_80036b68` helper. Four direct address pairs cover one 0x20-byte load
datum at `0x8006d6e4`, now typed as SDK `SVECTOR[4]`. Japanese retail bytes
decode to `(0,-1424,0,0)`, `(0,-912,0,0)`, `(0,-100,300,0)`, and
`(0,0,64,0)`; the next curated row begins at `0x8006d704`. The defining TU and
original linkage remain unproved, so its identity stays address-derived and
the giant dispatcher's original TU owner remains WIP. The current
`map_object_action_update.c` now defines the four-vector initialized table.
After a GAME target-only refresh, direct strict objdiff gives the datum and
its 0x20-byte `.data` section 100%; `func_80036ed4` remains WIP at
7.0693793%. The source claim establishes the current link owner without
proving the historical TU boundary.

The provisional `bss_801c7540.map_cells[88][80]` spans `0x11300` bytes, while
`0x8002aaa4` directly reads a shape-table base at BSS offset `0x10000`.
Its cached selected-layer pointer is now typed as `KfMapOccupancyLayer *`:
retail byte loads at layer +1/+2/+3 correspond to elevation,
quarter-turns, and the shape selector. This field-spelling correction
produced an identical focused object for the WIP function (17.8% listing)
and changes no shared layout.
The source-owned 49-word shape-opcode switch has one reviewed direct base
pair at `0x8002ac54/58`: raw `lui at,0x8001; addiu at,at,0x134c` resolves
to `func_8002aaa4_rodata`. Retail `0x8002ac34` subtracts `0x10` from
the unsigned record opcode, sign-extends it, and rejects values above
`0x30`, bounding the table to opcodes `0x10`–`0x40`. All 49 retail words
target blocks inside `0x8002aaa4`; 13 targets are distinct and 37 rows
select the default block `0x8002b5c4`. Pointer rows remain candidate
indirect edges despite this raw block-head check.
The focused compiled and retail `.rodata` sections both contain exactly 49
`R_MIPS_32` pointer rows, and their complete row-by-row target-equivalence
grouping is identical (13 groups, no differing rows). Their raw pointer
values still differ because the 0xb60-byte retail body and current compiled
body place the case blocks at different offsets; the low `.rodata` fuzzy
score is not evidence of a missing case or changed table grouping.
The current focused C body is 0xaf0 bytes, 0x70 shorter than retail; its
visible CFG is 162 blocks and 97 branches against 174 and 99 in retail.
The pointer-group result narrows the remaining work to the case bodies,
control joins, and instruction schedule rather than the opcode-to-case map.
The opcode `0x11` row at `0x80011350` points to `0x8002acb0`. That entry
clears `$s7` at `0x8002acb4`, then branches on `$s7` at `0x8002acb8`;
the branch is false along this table entry. No row in this bounded switch
enters at `0x8002acb8`, and the decoded direct edges do not target it.
This apparent dead branch is an unattributed CFG residue, not evidence
for adding a fabricated source flag or changing the shape-record opcode.
Retail transition phase one at `0x80016820` copies `0x3e80` words
(`0xfa00` bytes, exactly 80×80×10 map-cell bytes) to the BSS base, then
copies `0x600` words (`0x1800` bytes) to its +0x10000 interior. The
second loaded region ends at +0x11800, just before the collision-cache tail.
This establishes two distinct copy destinations and a loaded extent, not
the internal extent of the variable-length shape records. The provisional
equipment-record view beginning at +0x115a8 also falls inside that second
copy, so its simultaneous storage ownership needs a separate audit.
This overlap does not yet prove that the map grid has only 80 rows:
`0x80035894` passes an unguarded byte-valued source row to `0x80035194`,
and `0x80036ed4` has additional rectangle-copy calls. Rows beyond 79 may
share or repurpose the region. Keep the grid extent and shape-bank owner WIP
until those coordinate ranges and all consumers are established.
The dispatcher has six proven direct calls to `0x80035194` at `0x800371c0`,
`0x80037230`, `0x800372e8`, `0x800374e0`, `0x800375d0`, and `0x80037c70`;
they pass byte-loaded coordinates and field mask `0x2d`. Together with its
two calls from `0x80035894`, these establish eight direct callers, but do
not establish a bound on every resource-provided source row.

The strict `game.collision_grid_sample` report relinked 142/142 GAME target
units. Overall edge-check remains open on three unrelated `.rodata` addends
and incomplete known-reference ownership. The raw-backed dispatcher tables
above do not resolve those unrelated edge gaps, and no cache boundary was
guessed from the overlapping views.

The later `0x8002c670` source now selects its center through the typed
`map_cells[row][column]` grid. It uses the enclosing cell's byte
representation only for the runtime-selected layer lighting byte; a layout
check ties the lighting offset to `KfMapOccupancyCell`. The focused listing
improved from 75.8% to 76.4% and remains WIP. Its remaining first lighting
referent differs in address formation: retail computes
`bss_801c7540+4 + map_z*800 + map_x*10 + selected_layer`, whereas the
probe forms a typed cell base before loading its +4 lighting field. The two
forms address the same byte. No relocation identity was changed to conceal
the different field-base provenance.

A focused current-source data-owner audit compared each initialized ELF
section, relocation row, and claimed placement against its GAME target.
`game.collision_height_wrappers` has matching `.data` of 3,520 bytes;
`game.map_mask_window_sweep` has matching `.data` of 28 bytes;
`game.map_object_init_records` has matching `.data` of 270 bytes and
`.rodata` of 1,016 bytes; and `game.map_object_spawn_scatter` has matching
`.rodata` of 84 bytes. All four have no placement issue in this focused
check. The 68-byte `game.map_object` table has all 17 relocation rows at
the correct offsets, with matching types and referents, but every case
pointer addend is four bytes later in the compiled text. The 196-byte
`game.collision_shape_dispatch` table also has all 49 relocation rows at
matching offsets, types, and referents; its case-pointer addends differ
with the WIP function layout, including the 37 default rows at -228 bytes.
Neither WIP table is a strict data match.

Fifteen direct `jal` sites in six collision wrappers with identical focused
listings were reviewed against their reachable retail CFG blocks, delay slots,
source calls, and decoded targets. `0x8002b7f8` calls `0x8002a988` and
`0x8002aaa4` at `0x8002b834/84c`; `0x8002bd3c` calls
`matrix_rotate_quarter_turns` at `0x8002bd70/80/90`. The `0x8002bdbc`
wrapper calls `0x800158b4` at `0x8002bdf4/be10` and `0x8001584c` at
`0x8002be2c/be40/be54/be74`. The `0x8002be9c` wrapper calls `0x8002bdbc`
at `0x8002bee8`; `0x8002bf38` calls `0x8002be9c` at `0x8002bf94`; and
`0x8002c1d4` calls `0x8002c170` at `0x8002c218/230`. Separate safe
one-function carves admitted all fifteen direct rows as `R_MIPS_26`, with
none withheld. A focused wrapper rebuild retains 10/17 identical
listings, including these six callers; the seven existing WIP siblings retain
their prior verdicts. This review does not establish an indirect or
switch-table target.
The identical focused `0x8002c290` mask helper has three separately reviewed
internal `j` sites at `0x8002c37c`, `0x8002c3c4`, and `0x8002c404`, targeting
its own blocks at `0x8002c3cc`, `0x8002c41c`, and `0x8002c3a8`. Retail CFG,
source labels, and delay slots agree. A safe one-function carve emitted three
local-section `R_MIPS_26` rows with no withholding; the wrapper unit remained
10/17 identical focused listings afterward.

The WIP `0x8002c424` line scanner has three raw-decoded internal `j` sites:
`0x8002c534→0x8002c59c` clears the first-layer mask, `0x8002c594→0x8002c5e4`
sets the second-layer mask, and `0x8002c5d8→0x8002c5e8` advances past the
ordinary secondary-cursor increment. Each target is a reachable block in
the same function; the source labels and delay slots agree. A safe one-VA
carve emitted all three local-section `R_MIPS_26` relocations with no
withholding. The focused wrapper unit remains 10/17 SAME listings, while
`0x8002c424` remains WIP at 73.5%; relocation promotion does not explain
its instruction-order residue.

The preceding WIP run scanner at `0x8002c170` also has three proven internal
`j` edges: `0x8002c1a0→0x8002c1c4` advances an unhandled state,
`0x8002c1b0→0x8002c1c0` advances after entering the matching run, and
`0x8002c1c4→0x8002c180` repeats the 24-cell scan. Retail CFG and delay
slots match the source's state transitions. A safe one-VA carve emitted
all three as local-section `R_MIPS_26` with no withholding. Focused
`0x8002c170` stays WIP at 67.9% and the wrapper unit stays 10/17 SAME;
its unresolved pointer/register lifetime is separate from relocation
identity.

The WIP `0x8002bfd4` line rasterizer has two direct internal `j` sites
that were separately reviewed: `0x8002c038→0x8002c044` joins the signed
step setup, and `0x8002c0ec→0x8002c168` exits the first major-axis loop.
Both words decode to reachable local blocks and their delay slots match
the source paths. A safe one-VA carve emitted local-section `R_MIPS_26`
for both without withholding. The rasterizer's low focused similarity is
unchanged; these rows establish control referents, not codegen identity.

The `0x8002c670` mask-window builder has two reviewed internal joins:
`0x8002c828→0x8002c83c` after choosing the layer masks and
`0x8002ca6c→0x8002ca88` after choosing the center mask byte. Retail words,
reachable blocks, source branches, and delay slots agree. Its safe one-VA
carve emitted both local-section `R_MIPS_26` rows with no withholding;
the focused window-sweep listing remains WIP at 76.4%.
