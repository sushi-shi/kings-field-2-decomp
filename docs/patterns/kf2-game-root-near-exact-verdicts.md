# GAME near-exact follow-up verdicts

These are strict GAME.EXE verdicts from the pinned probe. Each target was checked
against retail disassembly, incoming and outgoing references, strings, and a
focused object listing. A WIP score is not an exact claim. Source-only probes
that changed bytes without independent source evidence were reverted.

| VA | Function | Strict verdict | First remaining evidence |
| --- | --- | --- | --- |
| `0x800158b4` | `func_800158b4` | WIP, 98.0% | Nine signed halfword interpolations and 3/3 CFG blocks agree. The probe assigns loop pointer/count registers differently and schedules the two pointer increments in the opposite order; `func_8001584c` and `func_8001586c` remain exact. |
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

The thirteen WIP rows are observable residues, not compiler/backend
attributions. The new exact row is not banked while the matching campaign and
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

The shared `map_cell.h` prototypes now carry the retail-supported byte-width
layer input and void occupancy-update return. The previously inconsistent
actor initializer declaration was removed, and its exact listing stayed
identical. Exact actor-home and map-placement callers retained their strict
100% status after adopting the shared signature; actor-home also uses the
common collision-cache height view instead of a duplicate raw offset.
