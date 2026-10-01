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
map bounds, cursor update, and final state writes. The original TU owner
of the shared `0x801b5a70` state remains unresolved; a tentative source
definition is described below. `0x8002c670` has a separate WIP source
claim. The new rasterizer was initially WIP at
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
| `0x80036ed4` | WIP, 90.94679% direct strict code; 78.6% focused listing | The 0x1df4-byte no-argument map-object dispatcher has a source claim and three bounded indirect jumps. Its 956-byte table section has all 238 reviewed pointer rows; the case-block order now gives 79/79 ordered direct calls, while case-pointer addends and text layout still differ and indirect callback targets remain unresolved. |

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
listings. A refreshed focused build puts `0x80036190` at 99.3% listing
and direct strict objdiff at 98.56115% (556/556 bytes): its only remaining
instruction difference is the order of the stack-argument load and result
move before `angle_within_tolerance`. The earlier guard-removal probe had
regressed its then-current 82.9% listing and remains reverted.
`0x80036464` is 92.8% focused and 95.32258% direct strict (372/372
bytes), with saved-register and reset-call delay-slot scheduling residue.
The `0x800363bc`/`0x800363dc` siblings and 68-byte switch table are direct
strict 100%. KF1's analogous map-object source and GCC257 pattern ledger
support the sequence-pointer/source shape but leave the KF2 residues
unattributed; no source edit was retained.

The two branches of `map_object_set_cell_marker` each load the object's Z
and X coordinates before forming the map-cell address in retail. A temporary
source probe with explicit coordinate locals preserved all five SAME
listings in `game.map_object_reset`, but left this function at 55.9% and
changed only its arithmetic/address schedule. No source change was retained;
the duplicated marker writes and scale updates still agree with retail.
A later typed row-pointer correction in both branches matches the retail
row-first calculation: the fresh focused unit is 6/6 SAME, and isolated
strict objdiff confirms `map_object_set_cell_marker` plus its five siblings
at 100%. The earlier coordinate-local probe remains a rejected intermediate.

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
rows retain the `mips32_candidate` kind, with their raw pointer values now
individually reviewed.
The 224 + 9 + 5 table rows at `0x8001191c`–`0x80011cd4` match all 238 raw
Japanese retail words at their file offsets. Every decoded pointer is aligned,
lands within the dispatcher, and starts a retail CFG block; all three table
bounds are independently enforced by the indexed `jr` paths above. These
rows now have `pointer-reviewed/reviewed` evidence status, without claiming
historical linker relocation records or resolved indirect callees. A focused
one-VA safe carve withheld zero relocations and zero functions.
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
its 0x20-byte `.data` section 100%.
Against the same target object, isolated direct comparisons put the prior
numeric-case source at 7.0693793% strict code and 26.572327% strict
`.rodata`; the reordered current source reaches 89.75535% strict code and
36.740044% strict `.rodata`. The retail code section is 7,668 bytes (probe
7,628 bytes), and pointer addends in the 956-byte jump-table section still
follow the different case-block offsets.
The source claim establishes the current link owner without
proving the historical TU boundary.
The dispatcher source now reads and updates `rotation.vy` at object +0x26
in ten action 2/3/16/88 call and sine paths. Retail loads `lh 28(s1)` and
`lhu/sh 28(s1)` with `s1 = object + 0x0a`; the prior `rotation.pad` accessed
+0x2a. Two action-98 target-angle uses still read +0x2a and remain `pad`.
The source now orders its 23 explicit top-level action cases by their unique
retail first-table target addresses. An isolated pinned compile of the
previous numeric-order source matched only 22/79 direct-call positions in
the retail sequence; the reordered source matches all 79/79 in order. This
independently supports the physical source block order as well as the bounded
table pointers, so the correction is retained. The focused listing improves
from 27.7% to 75.4% but remains DIFF with register/text-layout differences;
the current target/probe CFG has 329/319 blocks and 180/176 branches, so no
strict exact claim follows.
At action 8, timer 2, retail loads the previous angle with `lhu`, stores the
step velocity at object +0x36 before the addition, masks the updated angle to
12 bits, stores it, and compares that same masked value with signed `slti`.
The retained unsigned source read and local `s32 angle` produce those
instructions in the focused probe without a redundant halfword reload.
Actions 81 and 225 both read and write a one-byte state at object +0x40:
retail uses `lbu`/`sb` at `54(s1)` with `s1 = object + 0x0a`. The prior
`extra_40.bytes[2]` source accessed +0x42; `bytes[0]` is now used for these
state paths, while the independent +0x42 parameter uses remain unchanged.
The field correction raises isolated strict code from 86.52322% to
86.52582%; the earlier focused listing was 47.4%.
Actions 96 and 98 instead use a signed/unsigned halfword at object +0x3e:
retail `lh`, `lhu`, and `sh` use `52(s1)`, while the prior source's
`extra_40.halfwords[1]` addressed +0x42. Their velocity reads and writes now
use the existing `tail.fields.unknown_3e` signed/value union views. The
separate earlier +0x42 rotation uses remain as they were. Isolated strict code
rises from 86.52582% to 86.53104%; focused listing was unchanged at this step.
For action 98, retail loads one signed angle, branches on the timer with the
`-0xa0` alternative in the delay slot, applies `+0xa0` on the fallthrough,
then masks and stores the result once. The retained shared `angle` assignment
gives that branch shape and a single store. It raises direct strict code from
86.53104% to 86.61241%, although the local focused listing falls from 47.4%
to 47.2% amid remaining register/offset differences.
In action 81, retail reloads the object +0x3b boundary byte at the entry and
after intervening calls (`lbu` at `49(s1)`); the earlier source cached the byte
in a saved local across all timer branches. Reading the shared object field at
each use restores the byte-load referent and raises direct strict code from
86.61241% to 87.818985% and focused listing from 47.2% to 49.0%.
Jump-table `.rodata` is 36.740044% because case-block offsets remain WIP.
Retail action 81 calls `func_800369b8` after loading the pose vertex index,
then reads the template's reach and height halfwords. Moving the latter two
source reads after that call releases their saved temporary lifetimes and
raises direct strict code from 87.818985% to 89.70005%, with focused listing
49.0% to 75.5%. The height at template +0x10 is a single `lhu 16(s3)`, so the
shared 24-byte pose view now types that field as `u16`, with an offset check;
the other pose-view consumer retains its 89.4% focused WIP result. This final
width correction raises direct strict code to 89.75535%, while focused listing
settles at 75.4%.
Retail action 4 branches on the linked index sentinel to a local null-pointer
assignment at `+0x568`; its linked-object stores occupy the two preceding
jump delay slots. Moving `linked = 0` from its declaration into an explicit
`else` reproduces that CFG segment through the timer reload at `+0x56c`.
Focused listing rises to 75.9%, and direct strict code to 89.953575% with
7,644 candidate bytes against 7,668 retail bytes. The 32-byte data section
remains exact. Jump-table pointer addends move with the case bodies, so the
956-byte rodata section is only 17.295597% strict at this point. Retail
stores negative values to object +0x0e using signed `addiu` immediates. The
field is now `s16`; an isolated signed-header probe changed only the three
negative literal instructions in this dispatcher, to match retail, raising
strict code to 90.35316% and focused listing to 76.1%. Isolated probes of
all four other field consumers (`map_object_reset`, `map_object_init_records`,
`event_map_object_controller`, and `render_resource_dispatch`) changed only
debug type annotations, preserving their emitted code. Focused checks after
the header edit preserved five exact reset siblings; the other three units
remain WIP at their preceding focused results. Retail's `lhu` reads in
unsigned contexts therefore coexist with this signed field declaration.
At action 225, raw `lui v0,0x8017` and `addiu v0,v0,0x67d0` at
`0x80038b58/5c` form `0x801767d0`, the supported start of
`map_object_state.objects` (`map_object_state +0x1e00`). The source already
references that array. The previously absent reviewed HI/LO row is now
accepted by focused safe delinking with addend `0x1e00` and no withholding;
the direct strict code score rises from 90.35316% to 90.3542%, and the
focused listing rises from 76.1% to 76.2%. The remaining source/text and
jump-table addend differences keep the function WIP.
An explicit action-225 failure label did not reproduce retail's local
`+0x1c24` failure block: the probe merged the zero store with an earlier
action's zero store at `+0x1480`, lowering direct strict code to 90.2097%.
That source-shape trial was reverted.
The four retail `SVECTOR` start/end offsets at `0x8006d6e4`–`0x8006d703`
each have their own `lui`/`addiu` pointer construction: source sites
`0x800384ec/f4` and `0x80038584/8c` independently form all four addresses.
The earlier one-array C definition let GCC derive each second pointer as
`a3 = a2 + 8`, leaving two of those retail HI/LO pairs absent. Four adjacent
initialized globals preserve all 32 retail data bytes exactly and emit the
four separate pointer pairs. Their identities remain address-derived because
original linkage is unproved; the following `0x8006d704` datum is unchanged.
The focused safe carve accepts all four renamed referents with zero addends
and no withheld rows. Direct strict code is now 90.466354%, the focused
listing is 76.5%, and the 32-byte `.data` remains 100%; the 956-byte
`.rodata` pointer-addend result is still WIP at 17.033543%.
Action 83's retail case 2 falls into the `func_800366fc` completion block,
while case 3 appears later and jumps back into that completion block. Moving
the common label/body immediately after case 2 gives the source the same
physical CFG. Its state-byte dispatch also uses a signed `slti` range test
and `bltz` after an `lbu`, a pattern emitted by a nested switch over states
0–3 but not by the earlier if/else chain, which emitted `sltiu` and omitted
the negative-range branch. The retained switch preserves all four cases and
the default behavior. The final quick focused listing is 78.6%; direct
strict code is 90.94679% (7,664 probe bytes versus 7,668 retail), 32-byte
`.data` remains 100%, and the 956-byte `.rodata` rises to 39.937107% as
case pointers move toward their retail offsets. The label-only intermediate
scored higher on code alone (91.22796%) but did not reproduce the retail
state-dispatch CFG or table addends, so it was not retained.

Action 17's retail call at dispatcher offset `+0x16c0` separates two
independent reads of the linked object index and marker mask. Retail reloads
the bytes at object offsets `+0x31` and `+0x30` after `func_80036b68` and
rebuilds the linked-object address before setting its marker. Keeping the
pointer and mask live across the call omitted that sequence; scoping the
reads to the two branches reproduces the local instructions. Action 15's
retail cases `0x72`, `0x73`, and `0x74` select control offsets `0x28`,
`0x2c`, and `0x30`, then share one halfword write through
`event_state.control`. Selecting offsets from the typed control fields
reproduces the raw branch schedule, HI/LO referent, and shared store.
Action 19's three scale halfword stores run at object offsets `+0x30`,
`+0x2e`, then `+0x2c` in retail; spelling the assignments in that order
reproduces the local sequence. Its `game_counter_bytes[0x4c]` is zero
extended on load but then compared by signed `slti`, so an `s32` local
captures the promoted comparison type. Moving the shared action-start block
between timer cases 1 and 2 reproduces retail's fallthrough, removing an
extra compiled jump. Action 96 has an explicit timer-0 branch, timer-1
branch, and default exit in retail; a `switch` yields that raw dispatch
schedule and preserves both cases. The retained object now has a 80.6%
focused listing, 94.6771% strict code (7,672 probe bytes versus 7,668
retail), exact 32-byte `.data`, and 39.989517% strict `.rodata`; the
dispatcher remains WIP. The focused fuzzy score dipped from 81.5% after
the action-96 switch, while direct strict code and the local raw CFG improved.
Action 89's second and fifth timer phases each select one of two five-byte
occupancy layers with a retail branch and an optional `+5` before storing
markers `0x74` and `0x75`. The previous conditional array indexes compiled
branchless mask arithmetic; typed `KfMapOccupancyLayer *` selections restore
the retail branch shapes. The retail `lui 0x801c; addiu 0x7540` pairs at
`0x80037d48/4c` and `0x80037e3c/40` each directly form the already owned
`bss_801c7540` base. Two reviewed HI/LO rows make those referents explicit;
the safe one-VA carve admits both with no withheld rows.
Action 83's completion block reads its state byte once and branches through
state 1, signed `<2`, state 0, then signed `<4` before the state-1 sound
block. A four-state switch reproduces this branch sequence; a literal
1-first `if` ladder instead placed the sound block too early. The current
direct strict result after the signed velocity and predecrement corrections
is 96.54460% code (7,676 probe bytes versus 7,668 retail), 32/32 exact
`.data`, and 39.989517% `.rodata`; the focused listing is 82.6% DIFF.
Later code placement and switch pointer addends remain WIP.

A fresh one-unit strict report for `0x80036ed4` confirms this verdict:
7,668 retail code bytes score 96.54460%; the 32-byte `.data` is exact,
and the 956-byte `.rodata` is 39.989517%. The focused object has 180/180
branches and one return, but 329 retail versus 327 candidate CFG blocks.
The primary dispatch `jr` at `0x80036f98` still limits reachability analysis.
Raw action-98 code stores the incremented signed velocity halfword in the
delay slot of its negative-velocity branch; combining the source update and
temporary into one expression emitted the same candidate listing. Reversing
the action-225 or action-34 collision conditions did not close the two-block
gap; the former lowered the focused score and the latter changed only local
layout. Widening action 4's previous-timer temporary inserted a redundant
zero-extension before its signed comparisons. These isolated trials were
reverted. No new source fact was established, and the dispatcher remains WIP.

Action 8 stores `-16` into the extra word's first halfword at retail
`0x80037694..98`, then reloads it with signed `lh` at `0x800376c4`
for `angle_velocity_step`. A layout-identical signed-halfword union view
models that velocity without a cast and changes the probe's `ori 0xfff0`
to retail's `addiu -16`. The map-object unit's two exact helpers and all
four exact spawn-scatter listings remain SAME on focused rebuild.

Action 22 decrements the extra halfword and immediately tests its truncated
result. Retail `0x80037858..78` keeps the decremented value in `v0`, masks
it to 16 bits, and branches with the Z-position store in the delay slot.
Spelling this as `if (--object->extra_40.halfwords[0] == 0)` rather than a
separate decrement and reload reproduces that entire local schedule.

The adjacent map-cell pattern pair remains WIP on a fresh focused check:
`0x80034f90` is 92.5% listing and `0x80035194` is 58.8% listing.
Retail `0x80034fe8..0x80035004` computes complementary byte offsets
`0` and `5` for the two five-byte occupancy layers. Replacing those
source offsets with typed array indexes kept the same fields but lowered
the first listing to 76.7%; the trial was reverted. The rectangle copier
still has all 51 retail CFG blocks, with frame and mask scheduling residue.

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
At `0x8002ab5c`–`0x8002ab80`, the selected occupancy layer's byte at
`+0x03` indexes unsigned 16-bit offsets at the loaded shape-bank base.
Spelling that read as an indexed `u16` table access preserves the complete
17.8% focused dispatcher listing. This establishes the access width and
selector relationship, not the bank's internal record extent or a separate
global owner.
The subsequent signed halfword at record `+0` scales the input radius by
`>> 12`, the signed halfword at `+2` gives the command count, and commands
begin at `+4`. These raw reads now use one four-byte typed record header
with layout checks; the variable-length command body remains a separate
pointer. That focused listing was 17.2% WIP, lower because the compiler
retained the header pointer differently. The visible CFG then had
162 blocks and 97 branches against retail's 174 and 99; the typed layout
is retained because its offsets and signed loads are independently proved.
The table's opcode `0x18` and `0x19` rows enter `0x8002b534` and
`0x8002b5a4`. The first stores a signed record operand plus cached height
as the lower bound. With the sign flag it also stores that lower bound as
the result, then records a floor hit only if it is below Y. With bit
`0x40000000` it stores the lower bound as the height limit before testing
`bottom_y <= lower_bound` for bit 8. Opcode `0x19` stores the analogous
upper bound. Both paths advance the command stream directly; neither uses
the generic old-height-limit restoration. Source now expresses those direct
stores and joins without comma expressions. Focused comparison is 17.3%
WIP, with 161 compiled CFG blocks and 97 branches against retail's 174 and
99; the bounded indirect switch still limits CFG certainty.
Opcode `0x10` enters `0x8002ac70`. Retail updates the result cache only
when the signed operand plus cached height is lower than its old value,
then reloads the cached result for the Y comparison. The `0x30`/`0x32`
height path at `0x8002b38c` reaches the same flag-setting join only after
its candidate wins; it stores that candidate and compares the local value
with Y in the branch delay slot. The source now preserves the conditional
store and the distinct comparison values at this shared join. Focused
similarity falls to 15.6% while compiled CFG returns to 162 blocks and 97
branches. The lower score is retained because the cache writes and reads
are separately proved by raw instructions, and the indirect-switch layout
still dominates the difference.
The focused compiled and retail `.rodata` sections both contain exactly 49
`R_MIPS_32` pointer rows, and their complete row-by-row target-equivalence
grouping is identical (13 groups, no differing rows). Their raw pointer
values still differ because the 0xb60-byte retail body and current compiled
body place the case blocks at different offsets; the low `.rodata` fuzzy
score is not evidence of a missing case or changed table grouping.
Before the typed header and case refinements, the focused C body was 0xaf0
bytes, 0x70 shorter than retail, with 162 blocks and 97 branches against
174 and 99 in retail.
The pointer-group result narrows the remaining work to the case bodies,
control joins, and instruction schedule rather than the opcode-to-case map.
The opcode `0x11` row at `0x80011350` points to `0x8002acb0`. That entry
clears `$s7` at `0x8002acb4`, then branches on `$s7` at `0x8002acb8`;
the branch is false along this table entry. No row in this bounded switch
enters at `0x8002acb8`, and the decoded direct edges do not target it.
This apparent dead branch is an unattributed CFG residue, not evidence
for adding a fabricated source flag or changing the shape-record opcode.
The live opcode `0x11` path keeps its first signed height candidate in a
register through both comparisons. If `bottom_y` is below that candidate,
the second signed height candidate is tested; a second candidate below
`bottom_y` sets bit 8, whereas the other arm conditionally updates the
result cache and stores `-100000` as the height limit. The first candidate
is written to the height-limit cache only at the `0x8002ad24` join, reached
when the second candidate sets bit 8 or the first comparison fails. The
source now follows those store paths instead of writing the first candidate
before the comparisons. The focused listing improves from 15.6% to 16.2%
but remains WIP; this change is retained for the raw-proven store timing.
Opcode `0x32` clamps its diagonal coordinate between signed record operands
at `+4` and `+6`: retail `0x8002b31c` moves the lower endpoint into the
coordinate when it is too small, and `0x8002b330` moves the upper endpoint
when it is too large. The prior C condition erroneously used the upper
endpoint for both cases. The corrected two-arm clamp is source-backed;
focused listing remains 16.2% WIP.
The shared `0x8002ada4` path for the `0x20`–`0x23` geometry cases also
has two exclusive cache effects. Retail writes the first signed height
candidate to the height limit when `bottom_y` is at or above it; when
`bottom_y` is below it, retail may instead lower the result cache with
the second candidate and set the case flags if that result is below Y.
The source now spells these paths directly and advances the record without
a temporary old-limit restoration expression. Focused similarity is 16.1%
WIP, with the raw store targets and conditional order retained.
The old loop also loaded and rewrote the height limit around every command,
including the default path. Retail has only six height-limit stores in
this function: initialization at `0x8002ab38`, opcode `0x11` at
`0x8002ad20/28`, the shared geometry join at `0x8002adec`, opcode `0x31`
at `0x8002b4ac`, and opcode `0x18` at `0x8002b590`. The loop-wide restore
has therefore been removed, with opcode `0x31` writing its own limit after
its result update. Focused similarity falls to 14.0% because this changes
the compiler's whole-function lifetime and schedule; the source keeps the
raw-proven cache write sites. The large body remains WIP.
Opcode `0x22` has an explicit case flag value of 5: its retail entry at
`0x8002af1c` loads `a2 = 5` in the branch delay slot before any path can
reach the shared geometry flag update. The prior C case omitted this
assignment and could reuse a prior command's value. The source now sets 5
at entry; focused listing remains 14.0% WIP. The default focused CFG view
has 165 compiled blocks and 97 branches against retail's 174 and 99; both
views warn that the bounded switch remains an unresolved indirect jump.

The related opcode `0x21` path at `0x8002ae28` preserves all four signed
quarter-turn boundary tests before the shared geometry update. Opcode `0x30`
at `0x8002b0b0` uses the corresponding rotated coordinate and divides it by
the signed record halfword at `+0x0a` only after both inclusive bounds pass.
Opcode `0x31` at `0x8002b3e8` advances only past the command word when result
bit 0 is clear; when set, it also consumes the ten-byte record body before
testing the rotated coordinate. Opcode `0x40` at `0x8002b4d8` toggles the
unsigned cached layer between offsets 0 and 5, recalculates height from the
new layer's elevation, and visits that layer once. All four paths agree with
the retained source. A fresh focused build remains 14.0% listing similarity;
direct strict objdiff is 33.42445% code and 19.642857% switch `.rodata`.
The source, 49-word table owner, and unresolved indirect switch edges were
left unchanged.

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

At that pass, the `0x8002c670` source selected its center through the typed
`map_cells[row][column]` grid. It uses the enclosing cell's byte
representation only for the runtime-selected layer lighting byte; a layout
check ties the lighting offset to `KfMapOccupancyCell`. The focused listing
improved from 75.8% to 76.4% and remains WIP. Its remaining first lighting
referent differs in address formation: retail computes
`bss_801c7540+4 + map_z*800 + map_x*10 + selected_layer`, whereas the
probe formed a typed cell base before loading its +4 lighting field. The two
forms address the same byte. No relocation identity was changed to conceal
the different field-base provenance. A later field-base correction is recorded
below.

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

The same builder has 26 proven direct `jal` sites, all decoded in reachable
retail blocks with their delay slots and mirrored by source calls: two
`rcos`, one `rsin`, one mask clear, one grid sample, four segment raster
calls, one row fill, and eight calls each to the mask-cell and line-scan
helpers. All 26 were promoted to reviewed after the one-VA safe carve
emitted them as `R_MIPS_26` with zero withheld. Its focused listing remains
76.4% WIP; the already materialized calls do not resolve the typed
lighting-byte address-formation difference.

The mask-window TU now tentatively defines the complete 0x20-byte
`render_mask_scan_state` BSS object at `0x801b5a70`. Its shared type covers
the offsets used by the builder, both scan helpers, and the map renderer;
the GAME xref inventory has 40 validated references to the base. The
native GCC object emits `.comm render_mask_scan_state,32`, matching the
typed extent. A direct native GAME link no longer lists the name as
undefined, although the link still fails on other unresolved symbols.
The defining TU is a source ownership model, not proof of the original
file boundary or an independent startup clear. Focused mask and wrapper
listings stay 76.4% WIP and 10/17 SAME, respectively.

KF1's exact `collision_adjust_cell_occupancy` uses a row-local pointer and
advances the next-row pointer from that local at the loop head. This is a
useful shape analogue for KF2 `0x8002b73c`, but the games use different
cell records, dimensions, and flag arithmetic. An isolated KF2 probe of
`row = current_row + 80` compiled identically to the retained source;
the KF2 function remains 75.5% focused WIP, so no source transplant was
kept from the KF1 comparison.

The `0x8002b874` retail snapshot stores the actor's unsigned halfword at
`+0x1c` to the cached radius before loading its unsigned halfword at
`+0x1e` for the common height store. Its map-object path likewise stores
template `+0x04` before loading `+0x08`. The current C expresses these
widths and fields, but the focused compiler schedules the height loads
first and creates one additional CFG block; the listing stays 85.8% WIP.
Isolated probes widening only the local height to `s32` and comparing the
object index with the preceding actor-index sentinel both compiled
identically to the retained source. The raw comparison supports the current
shared field types but not a source-level change to force load order.

At `0x8002ca14`–`0x8002ca50`, the mask builder calculates
`map_z * 800 + map_x * 10 + first_layer_byte_offset` and adds the
`bss_801c7540+0x4` lighting-field base before the byte load. This is the
same address as the current typed 80-column `KfMapOccupancyCell` lookup.
Isolated flat-cell and direct typed-field spellings both compiled to the
same 76.4% focused listing as the retained source; neither resolved the
different base-add schedule. The known whole-object owner and lighting
field therefore remain unchanged.

The GAME `0x8002c170` mask-row scanner is now **100% strict** (100/100 code
bytes). Retail forms a pointer `v1 = row + index` while retaining the row
parameter in `a0`, then advances `v1` and the index by the signed step.
Expressing the read as `row[index]` lets the pinned compiler derive that
induction pointer. The prior explicit `cell = row + index; *cell; cell +=
step` source reused `a0` for the pointer and scored 89.8% strict. The
indexed source preserves the unsigned 24-cell guard, matching-run state,
return value, and two exact calls from `0x8002c1d4`. A focused rebuild
reports 11/17 identical listings; direct native objdiff confirms the new
scanner and all ten earlier exact siblings at 100%, with the 3,520-byte
`collision_default_rows` datum unchanged at 100%.

The neighboring WIP `0x8002c424` line propagator now uses the indexed
cursor form for both neighbor reads, then addresses the map cell directly
through `bss_801c7540.map_cells[map_z][map_x]` in both control arms. Raw
retail uses those indexed cursor and typed cell-address paths; the earlier
maintained pointer and local map-cell base changed their instruction order.
The focused listing is 94.3% and direct strict objdiff is 98.29932%; its
remaining observed difference is saved-register assignment. All eleven
exact wrapper siblings and the 3,520-byte datum remain 100%. A separate
`0x8002b874` branch-local score probe was reverted after the full retail
CFG showed a shared interaction-height store.

The next focused collision-utility audit rebuilt six GAME objects and ran
direct strict objdiff on their 24 function claims. Sixteen are exact:
`0x8002a988`, `0x8002b604`, `0x8002b7f8`, `0x8002bc18`,
`0x8002bd3c`, `0x8002bdbc`, `0x8002be9c`, `0x8002bf38`,
`0x8002bfac`, `0x8002c170`, `0x8002c1d4`, `0x8002c290`,
`0x8002ce2c`, `0x8002cf40`, `0x800314fc`, and `0x80031634`.
The eight remaining strict verdicts are:

| GAME address | Direct strict result | Current evidence limit |
| --- | ---: | --- |
| `0x8002b67c` | WIP, 94.895836% | Cached-height reload versus retained elevation value. |
| `0x8002b73c` | WIP, 98.404260% | Row-pointer and loop-index registers; KF1's different grid is only a shape lead. |
| `0x8002b874` | WIP, 91.5% | Radius/height load order and common-tail schedule. |
| `0x8002b9d4` | WIP, 92.744830% | Collision-channel result lifetime after the supported calls and branches. |
| `0x8002bfd4` | WIP, 73.912620% | Two-axis mask-rasterizer induction and frame layout. |
| `0x8002c424` | WIP, 98.299320% | Indexed cursor and direct cell lookup are retained; saved-register assignment remains. |
| `0x8002c670` | WIP, 91.624245% | A whole-grid byte view with typed row and cell sizes emits the retail `bss_801c7540+4` lighting-field referent; later mask-state schedules remain different. Its 28-byte shape table matches exactly. |
| `0x8002ce68` | WIP, 65.851850% | Retail reloads late O32 arguments after free-slot acquisition; the probe retains them in saved registers. |

Fresh focused `game.floor_item_find_free` reconstruction confirms the two
adjacent functions `0x8002ce2c` and `0x8002cf40` remain direct strict 100%,
while `0x8002ce68` remains 65.85185% WIP. Its five proven retail callers
are all in `game_main_loop` and pass kind 1. Retail reads the fifth O32
argument as `lbu` for the stored item kind, then as `lw` for the kind-1
branch; it reads the seventh as `lhu` for height. The current full-width
kind and `u16` height source preserve those width facts and the three
calls, but GCC hoists arguments 5–7 into saved registers before the
free-slot call, expanding the frame from 40 to 56 bytes. KF1's floor-item
loader uses a different placement record system and has no matching GPU
upload body. No width, call, or control-flow correction is supported, so
the source remains unchanged.

All calls, decoded referents, and exact sibling controls used for these
verdicts remain intact. No source or inventory change was justified by this
pass; the earlier focused-listing percentages in this document describe
their own probes, while this table records the current direct strict results.

The later isolated `game.map_mask_window_sweep` control confirmed this
field-base correction. Retail computes its center lighting address from
`bss_801c7540+4`, an 800-byte row stride, a 10-byte cell stride, and a
runtime layer offset of zero or five. The source now uses the complete
`map_cells` array as its byte view and derives both strides and the lighting
field offset from the typed cell layout. The collision dispatcher at
`0x8002aaa4` also toggles the shared cache layer through `& 5` and applies
it as a byte offset into a cell, supporting that representation. The focused
listing rose from 76.4% to 79.7% with all 11 blocks and four branches
preserved. The strict single-unit text result rose from 90.127270% to
91.624245%; the 28-byte initialized shape table remains exact. The first
residual is the interpolation `addu` destination, then scan-state flag
stores and traversal scheduling. No exact function was banked.
