# GAME near-exact follow-up verdicts

These are strict GAME.EXE verdicts from the pinned probe. Each target was checked
against retail disassembly, incoming and outgoing references, strings, and a
focused object listing. A WIP score is not an exact claim. Source-only probes
that changed bytes without independent source evidence were reverted.

| VA | Function | Strict verdict | First remaining evidence |
| --- | --- | --- | --- |
| `0x800158b4` | `func_800158b4` | **Exact, 100%** | A fresh focused rebuild and direct objdiff match all 100 function bytes. The nine signed halfword interpolations retain the retail load-delay and pointer-increment schedule; the complete three-function unit matches 204/204 `.text` bytes. The earlier 98.0% report was stale. |
| `0x80017608` | `memory_arena_allocate_block` | WIP, 99.78261% | Arena calls, owner write, 8/8 CFG blocks, and 4/4 branches agree. Retail computes `available - 12` in `$v0` before subtracting size into `$a0`; the probe keeps both operations in `$a0`. The 56 exact sibling functions remain unchanged. |
| `0x8001876c` | `func_8001876c` | WIP, 96.36646% | Seven-entry switch, call set, and 34/34 CFG blocks agree. The remaining tail has a different reload/constant-register schedule; `func_800189f0` remains exact. |
| `0x8002083c` | `func_8002083c` | WIP, 99.65882% | Four MATRIX locals account for the used stack slots, but retail reserves 64 more bytes. There is no evidence for another live object; adjacent `menu_draw_two_option` and `func_80020990` remain exact. |
| `0x80021c8c` | `func_80021c8c` | WIP, 99.956985% | All 93 instruction positions, four direct calls, and 24 data/control referents align. The only four raw differences are the prologue/epilogue frame allocation and saved `$ra` slot: retail uses 32/24 bytes, the probe 24/16. The exact `func_80021e00` sibling is unchanged. No source-visible local or outgoing argument explains the extra eight bytes, so no padding was added. |
| `0x80028224` | `func_80028224` | exact, 100% | The signed `s16` view-angle field preserves retail's `addiu -700` pitch assignment. Both functions in `game.player_camera_turn` are exact; all exact consumers in player state, reaction, and weapon-transform units remain exact after the shared type correction. |
| `0x8002d5dc` | `tmd_prepare_primitive_indices` | WIP, 96.1326% | 17/17 CFG blocks and packet-field accesses agree; the probe adds an entry `move`, moving the switch table target by four bytes. The source keeps the true packet reference. |
| `0x80032174` | `map_cell_visible` | WIP, 93.6% | View-cell referents and bounds checks agree. Replacing the shared return with early returns worsened the CFG and was reverted; register/return scheduling remains. |
| `0x80032274` | `resource_vab_update_range` | WIP, 90.933334% | Correct `audio_state+0x30` referent, stream-queue call, state halfword writes, and 13/13 CFG blocks. The probe advances a slot pointer by eight where retail recomputes the slot offset each iteration. |
| `0x80034f90` | `func_80034f90` | WIP, 97.86822% | `rcos`, `rsin`, `bss_801c7540`, field offsets, and 14/14 CFG blocks agree; register allocation and one independent address calculation schedule differ. |
| `0x80035194` | `func_80035194` | WIP, 89.59545% | Nine-argument cell-copy model and field masks are supported. Focused listing has 51/51 CFG blocks but different mask-hoisting and row/column register lifetimes. |
| `0x80036e24` | `func_80036e24` | WIP, 98.86364% | Five direct calls, loop controls, and 5/5 CFG blocks agree; three saved argument registers are cyclically assigned differently. |
| `0x8003983c` | `func_8003983c` | WIP, 99.07538% | New typed lifecycle/target source matches the direct call set, actor/player referents, 37/37 CFG blocks, 24/24 branches, and 13/13 return frontiers. Retail keeps current actor in `$s0` and constant/chance in `$s1`; the probe uses the reverse registers. Player byte +0x10a remains unnamed. |
| `0x8003d084` | `func_8003d084` | WIP, 91.6% | Signed actor byte, ±12 clamp, and `rand()` call agree. Two natural reassociations of the return expression moved the `-2` to the wrong instruction position; both were reverted. `func_8003d0e8` remains exact. |

The twelve WIP rows are observable residues, not compiler/backend
attributions. The new exact rows are not banked while the matching campaign and
shared worktree changes continue. The shared source remains typed and the exact
neighbors above were preserved by focused comparison.

The `0x80021c8c` frame residue is visible in a direct objdiff comparison, not
just the percentage: only offsets `+0x000`, `+0x004`, `+0x164`, and `+0x170`
differ. The same eight-byte frame delta appears in the source-backed menu
window and string renderers, but that recurrence alone does not identify a
compiler mechanism or justify artificial stack use.

## KF1-backed horizontal movement identity

GAME `0x800274ec` remains unclaimed. Its three proven calls from the exact
camera-movement function pass a full-word heading and signed distance, and the
retail body calls `rsin`, `rcos`, collision queries, and
`vector_xz_to_angle`. The KF1 `player_move_horizontal` body has the same
movement/collision role; this supports the `player_move_horizontal(s32 heading,
s32 distance)` identity without assuming an identical implementation. The
retail return loads a local result at stack +0x50, so the declared result is
`s32` rather than KF1's apparent fixed-success contract.

The caller, its identity row, and its three reviewed call referents now use
that spelling. Both functions in `game.player_camera_turn` remain strict exact;
GAME relink verified 146/146 units. The required full build produced PSX and
retained the established unresolved overlay symbols. The body of `0x800274ec`
still needs reconstruction and a strict 100% comparison.

The retail body first multiplies `-rsin(heading)` and `rcos(heading)` by the
distance, then queries collision at the proposed X/Z with radius 800, height
1700, and mode `0x31`. An unobstructed result stores the proposed position and
the cached layer into player state; a collision can take the angle-deflection
path, retry a shorter move in 22-unit steps, or fall back to one axis. The
return is initialized to zero and set to one on the unobstructed path. These
KF2-specific branches and cache fields are why the larger KF1 body remains a
shape reference, not source to copy wholesale.

## Later GAME source-polish verdicts

The 2026-10-01 strict report is a checkpoint, not a closure criterion for the
WIP rows below. Each row was checked against the selected GAME retail CFG,
calls, referents, strings, and a rebuilt focused listing. Source-only changes
that moved instructions away from retail were reverted.

| Function | Strict checkpoint | Verdict |
| --- | ---: | --- |
| `8001930c` map preview | 98.26363% | WIP. The archive-entry calculation has the correct inputs and call set; retail uses a 64-byte frame and keeps the image in `$s6`, while the probe uses 56 bytes and `$s0`. Reordering the addition worsened the first divergence. |
| `8001b554` card browser | 98.478264% | WIP. The first difference follows the temporary-file probe: retail moves its result to `$a0`, while the probe keeps `$v0` and schedules the row pointer in the branch slot. |
| `8001bf68` card format flow | 97.12389% | WIP. Retail and probe branch on the same probe statuses; rewriting the guards in retail order moved the default branch and frame, so the existing source remains. |
| `8002b73c` occupancy update | 98.40426% | WIP. The cell rectangle, unsigned bounds, and byte updates agree; row/column register allocation differs. Callers pass both `1` and `-1`, so the scaled update now shifts an unsigned value without signed-shift undefined behavior. The exact `8002b7f8` sibling retains its raw `sll`/`srl` pair with the cast before the shift. |
| `8003a318` actor damage | 99.86911% | WIP. Its first focused difference swaps the registers chosen for two incoming stack arguments; no signature, call, CFG, or referent correction was supported. |
| `8003c3e0` actor group position | 99.64539% | WIP. Retail and probe compute the same masked yaw error and signed division; only the yaw-error and shifted-numerator registers exchange roles. |
| `800461a0` marker stream | 99.12676% | WIP. Retail keeps the scan and marker pointers in the opposite argument registers, with marker-relative byte loads. Pointer-base rewrites introduced extra instructions and were reverted. Retail's non-marker retry does not advance the cursor; source records that stream constraint. |
| `8001369c` main loop | 99.67553% | WIP. The only focused text difference is the fixed arena base `8009b0a0`: retail forms it with carry-adjusted `lui/addiu`, while the literal pointer uses `lui/ori`. No arena object owner or original address mechanism is proved; adjacent `main` remains exact. |
| `8001a4f0` item/magic controller | 99.74359% | WIP. Focused listing differs only in the initializer loop's count and constant registers; the loop stores, calls, branches, and data references agree. No source fact supports changing the live locals. |
| `8001fb8c` menu window | 99.78788% | WIP. All body instructions, calls, and referents align; the only focused differences are a 48-byte retail frame versus a 40-byte probe frame and their saved-register offsets. No live source object accounts for the extra eight bytes. |
| `8001f8b8` preview choice | 94.36464% | WIP. The label glyph writes and call set agree, but the probe assigns the three retained arguments to different saved registers and places the input-release exit after the input loop. A source-equivalent `while` form worsened the CFG and was reverted. |
| `80022058` decimal formatter | 97.39% | WIP. Retail reserves an eight-byte leaf frame and loads its fifth argument at stack `+24`; the probe eliminates that frame and loads at `+16`. The remaining branch displacements follow this one-word offset, and no live local explains a frame. |
| `800226ec` card-directory scan | 93.60504% | WIP. Retail reads the two slot defaults with `lb`; the probe uses `lbu` because their signed values are immediately narrowed into a byte array. The rest of the focused residue is register choice and one downstream instruction shift. The source keeps the supported `s8` global declarations without adding artificial sign-dependent work. |
| `8001fc94` menu list renderer | 98.44964% | WIP. The call and referent sets agree. The first focused difference is the row-width register and early scheduling of the two-value card-mode test; moving its C declaration inside the loop, including a guarded `do` form, changed the frame and worsened the listing, so both were reverted. Casting before each range subtraction makes the retail wrap defined in C without changing the focused object. The last difference moves a zero initialization two instructions. |
| `80022ca0` card write | 95.896774% | WIP. The focused body is 96.7% similar with the correct card calls, file path, image stores, and data referents. The remaining entry differences include the two signed default-byte loads narrowed into `slot_digit`, early initialization scheduling, and saved-register selection; no source edit is justified by them. |
| `800349bc` menu fade | 92.34296% | WIP, 43.2% focused. The four packet shapes and direct call/referent set agree, but retail stores the pad-release state on the stack, while the probe retains it in a saved register; this changes the frame and loop/exit layout. The range check now casts before subtracting one so the retail `addiu` wrap is defined in C. Both sibling functions retain exact focused listings. |
| `80035894` map-object placement loader | 95.312874% | WIP, 92.7% focused. The 350-record walk, placement fields, occupancy-layer selection, direct calls, and switch table are sourced. The first difference schedules two independent byte/halfword stores around a template load; the next assigns the region-index arithmetic to different registers. The layer elevation is an unsigned byte, so its seven-bit shift is already defined. No source fact supports an ordering change. |
| `800475d8` event map-object controller | 98.56481% | WIP. The focused body is 89.4% similar with matching calls and referents. The first residual assigns the active object to `$s0` instead of retail `$s1`, cascading through the state loops; one later branch-delay initialization is scheduled differently. No identity or control-flow change is supported. |
| `8003bd40` actor motion target | 86.53226% | WIP. Retail's world-coordinate subtraction, negative magnitude, and signed sum comparison use wrapping `subu`/`negu`/`addu`. The source now spells those operations with unsigned intermediates, preserving the focused 68.2% listing and all six exact siblings. Remaining differences begin at actor-pointer register allocation and stack-argument scheduling. |
| `80047c98` event world dispatcher | 97.37745% | WIP. Its focused listing is 85.7% with the direct call/referent set intact; the first difference swaps saved registers for rotation and the constant one. The linked-object notification branch in retail enters a shared call block; an equivalent early-exit C form emitted the same bytes and was reverted. The source retains the unresolved indirect callback instead of inventing a target. |
| `800489ac` event save decoder | 98.82883% | WIP, 95.1% focused. Retail passes stack `+16` to the exact ten-pointer offset decoder, indexes the selected pointer by a four-byte stride, then walks actor and target sentinels. The first residual swaps the register holding `0xff` with the actor-base register; the source already declares all ten pointers, so the curated identity note now records the full array rather than four entries. |
| `8002b874` collision snapshot | 91.5% | WIP, 85.8% focused. Retail selects player, actor, or map-object state and writes the same position, radius, and interaction-height fields as the typed source. Its actor and map-object arms store radius before the final halfword height, while the probe selects a different load/store schedule and register for the shared height. An explicit shared-tail `goto` emitted the same listing and was reverted; no field or branch correction is supported. Ten exact collision siblings remain SAME. |
| `8002c170` mask-row run scan | 89.8% | WIP, 67.9% focused. Both direct call sites are in the exact row-fill sibling, and retail/source agree on the 24-cell unsigned bound, byte comparison, state transition, step, and return paths. The first difference assigns the row cursor and two state values to different registers; no source-level correction follows from it. |
| `8002b9d4` collision channel query | 92.74483% | WIP, 59.4% focused. Retail and source gate the same grid, actor, map-object, and player calls by mode, store the same cache indices/flags, and agree on 23/23 CFG blocks and 12/12 branches. The probe uses a 56-byte frame and a different saved-register assignment where retail uses 64 bytes and keeps a second copy of mode in `$s7`; no additional live source object is evidenced. |
| `8002bfd4` mask-line rasterizer | 60.46602% | WIP, 35.5% focused after a real width correction. Retail masks both coordinates to 16 bits for its bounds checks **and** grid address; the old C checked `(u16)` coordinates but indexed with full-width values. Both raster loops now index with `(u16)` coordinates, aligning the row/column address arithmetic and preserving the ten exact collision siblings. CFG is 22/22 blocks and 11/11 branches; the remaining entry differs in frame and origin/delta register scheduling. |
| `8002c424` mask-line scan | 89.65306% | WIP, 73.5% focused. The eight proven calls from the map-mask sweep supply the six-argument step/stride pattern, and retail/source read the same scan-state, mask, and occupancy objects. CFG is 23/22 blocks with 14/14 branches; the probe assigns the second input to a saved register where retail first copies it to `$t5`, changing cursor-pointer lifetimes. Later mask-case joins differ by one block. No data-owner or control change is yet supported. |
| `8002ce68` floor-item capture | 65.85185% | WIP, 55.7% focused. All five proven call sites are in `game_main_loop`, whose source passes seven arguments. Retail and source store the same item fields, call free-slot search, allocate pixels only for kind 1, then call `StoreImage` and `DrawSync`. CFG is 5/5 blocks and 2/2 branches, with exact free-slot and update siblings. Retail reloads the three stack arguments after free-slot search, while the probe saves them in `$s` registers before the call and grows the frame from 40 to 56 bytes. No signature or field correction supports forcing those lifetimes. |
| `8002c670` map-mask sweep | 90.048485% | WIP, 76.4% focused. Retail doubles view X/Z with `sll`; the source now casts to `u32` before shifting so the same 32-bit wrap is defined. The four line draws, eight line scans, mask-cell referents, and shape table remain source-owned; CFG is 11/11 blocks and 4/4 branches. The first residual is an independent addition destination register; later state-byte stores and map-cell base scheduling differ, so no exact claim follows from this source correction. |

A follow-up focused pass tested three bounded control/dataflow spellings and kept
the established source. Enclosing the `80036190` object scan in the initial
range guard reduced its CFG from 15 to 14 blocks and its focused similarity
from 82.9% to 82.6%. Precomputing the first-layer selector in `80035194`
reduced focused similarity from 58.8% to 55.5% and added a saved-register
dependency at entry. Separating the rasterizer's start-coordinate loads in
`8002bfd4` and expressing `8002c424`'s second-layer sentinel as an explicit
early branch both emitted the prior focused listings. All four probes were
reverted; no new exact result or source-supported correction followed.

The shared `map_cell.h` prototypes now carry the retail-supported byte-width
layer input and void occupancy-update return. The previously inconsistent
actor initializer declaration was removed, and its exact listing stayed
identical. Exact actor-home and map-placement callers retained their strict
100% status after adopting the shared signature; actor-home also uses the
common collision-cache height view instead of a duplicate raw offset.

A later focused pass compared the related `80036464` map-object effect
spawner and `8002c670` mask sweep with retail before editing. The spawner has
12/12 visible CFG blocks, three branches, the same direct call set, and the
same map-object field effects; its 92.8% focused listing first assigns object
ID and height offset to opposite saved registers. The mask sweep has 11/11
blocks and four branches; spelling the lighting access through its typed
field instead of the checked byte offset emitted an identical 76.4% listing,
so the original byte-base expression was retained. Removing the redundant
entry guard from `80036190` reduced that function from 15 to 14 blocks and
82.9% to 82.6% focused similarity; the guard and baseline object were
restored. None of these probes establishes an exact match or a source
correction.

The `game.map_object` switch table is not an independent case-map defect.
Both object files carry 17 `R_MIPS_32` four-byte rows with the same four
destination groups in the same order. The target function offsets are
`0x000`, `0x22c`, `0x24c`, and `0x2d4`; the probe offsets are `0x000`,
`0x230`, `0x250`, and `0x2d8`. Only the first function, `80036190`, is four
bytes longer in the probe. All 17 table addends
therefore shift by exactly four bytes while the spawner itself remains 372
bytes. Resolve the first function's control-flow residue before treating those
table addends as a separate relocation-owner problem.

An explicit shared-result return path for `80036190` moved the initial guard
to a direct exit, but reduced the focused listing to 77.7% and CFG to 14
blocks while retaining three incoming return edges. It introduced an extra
saved result register, so the original returns were restored and rebuilt.
Redirecting only the entry guard to the existing final `return -1` produced
the same 82.6%/14-block listing as guard removal, and was also reverted.
Moving the three `continue` paths to one explicit labelled index/object
increment tail produced a byte-identical 82.9% object and was reverted in
favor of the simpler `for` loop.

A raw-byte follow-up of GAME `data.tsv` gaps found additional missed code.
The initial prologue-and-return filter accidentally excluded a `jr $ra`
whose delay slot ended exactly at the row boundary. Correcting that boundary
exposed the complete `80039048` actor wrapper, now a strict-exact claim in
the actor dossier. Scanning return words independently found three eight-byte
`jr $ra; nop` bodies: `80018764`, `80045f10`, and `80045f18`. All three
have provisional exact C claims; their no-xref limitations are recorded in
`game-return-stub-18764.md` and `game-return-stubs-45f10.md`. Six other
prologue-shaped unclassified hits from `8004a208` through `8004db38` fall
within Sony/Psy-Q CD or SPU archive spans already supported by the vendored
inventory. The scan is still a bounded heuristic, not a proof that every
unreferenced function has been found.
No curated GAME `mips26` target before the `80049f74` library region lands in
any remaining `data.tsv` row; this is a negative xref check, not proof that
all unreferenced code has been identified.
A separate decode of every direct `j`/`jal` word in admitted GAME function
bodies before that library boundary likewise found zero targets in the
remaining data rows. Indirect transfers and unadmitted code are outside that
negative control.
A corrected scan including a return whose delay slot ends at the data-row
boundary now finds no `jr $ra` word in the remaining pre-library GAME data
rows.
