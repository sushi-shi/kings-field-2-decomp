# GAME player view and motion: 25 current strict verdicts

This cohort follows `player_state` from its camera-pose getter and reset through
magic/action selection, horizontal camera movement, and the camera-turn caller.
The later camera-turn function calls the horizontal mover three times, while
the action controller and its bounded magic-ID predicate share player input
state.
The contiguous equipment unit supplies the exact typed-state controls around
the variadic effect selector. These connections define the screen, not an
original translation-unit boundary.

After the `e075607` checkpoint, all six GAME units were compiled with focused
`kf try --context 0 --no-flow` and compared as isolated target/candidate
objects with strict objdiff. **Twenty functions are 100% exact; five remain
WIP.** The focused listings were 20/25 SAME. No source or retail inventory
change was retained.

| GAME VA | Function | Retail bytes | Strict text | Verdict |
| --- | --- | ---: | ---: | --- |
| `0x80023484` | `player_reset_view` | 236 | 100% | Exact |
| `0x80024ed4` | `player_get_camera_pose` | 120 | 100% | Exact |
| `0x80024f4c` | `player_reset_status` | 184 | 100% | Exact |
| `0x80025004` | `player_initialize_state` | 384 | 100% | Exact |
| `0x80025184` | `game_initialize_session` | 108 | 100% | Exact |
| `0x800251f0` | `player_clear_motion` | 68 | 100% | Exact |
| `0x80025234` | `player_sync_position_to_map` | 176 | 100% | Exact |
| `0x800252e4` | `player_distance_to_point_in_cone` | 200 | 100% | Exact |
| `0x800253ac` | `player_distance_to_point` | 80 | 100% | Exact |
| `0x800253fc` | `player_set_unknown_97` | 16 | 100% | Exact |
| `0x8002540c` | `player_set_unknown_98` | 40 | 100% | Exact |
| `0x80025434` | `player_set_unknown_99` | 40 | 100% | Exact |
| `0x8002545c` | `player_set_equipment_slot` | 576 | 100% | Exact |
| `0x8002569c` | `player_equip_weapon` | 184 | 100% | Exact |
| `0x80025754` | `player_begin_weapon_attack` | 292 | 100% | Exact control; source untouched |
| `0x80025878` | `func_80025878` | 416 | 100% | Exact |
| `0x80025a18` | `func_80025a18` | 2,328 | 98.02234% | WIP: O32 argument home and frame |
| `0x80026498` | `func_80026498` | 452 | 100% | Exact |
| `0x8002665c` | `func_8002665c` | 3,024 | 98.67857% | WIP: address/register lifetime |
| `0x8002722c` | `func_8002722c` | 704 | 99.09091% | WIP: retail eight-byte frame |
| `0x800274ec` | `player_move_horizontal` | 1,084 | 89.85240% | WIP: 35/34 CFG blocks |
| `0x80028224` | `func_80028224` | 760 | 100% | Exact |
| `0x8002851c` | `func_8002851c` | 1,120 | 100% | Exact |
| `0x8002897c` | `func_8002897c` | 28 | 88.57143% | WIP: result-register assignment |
| `0x80028998` | `func_80028998` | 1,320 | 100% | Exact |

The first WIP divergence in `0x800274ec` is a saved-register assignment before
the camera-position loop. Retail has 35 blocks, 20 branches, an eight-byte
`SVECTOR` local, and 11 ordered direct calls; the candidate has 34 blocks and
the same branch/call counts. The extra retail block is the split slide-flag
store at `+0x34c/+0x350`. Current C already has the shorter-step retry and
slide-attempt flag. Retail rematerializes eleven more `player_state` address
pairs, but they denote fields already present in C. This is not evidence for
another collision branch or a new global.

Retail `0x8002897c` is a seven-instruction signed interval predicate:
`slti` against 81, a branch with zero-result delay slot, then `slti` against
71 and `xori 1`. It has no calls or data referents, and its two proved callers
are `0x80028998` and `0x80029014`. The source computes the same bounded
predicate; the unequal words concern the result register, so no artificial
local or extra branch was introduced.

The remaining three WIPs have complete existing semantic models. At
`0x80025a18`, 99/99 CFG blocks, 31/31 branches, all 16 target probes, all 11
effect-constructor calls, and 53 switch rows in 31 destination classes agree;
retail homes and reloads the first variadic argument in a 112-byte frame,
while the defined `va_list` C uses a 104-byte frame. At `0x8002665c`, the
114/114 CFG and 68/68 branches agree; the first difference assigns the loaded
weapon ID to another register and later retains a `player_state` base where
retail reconstructs field addresses. At `0x8002722c`, 33/33 CFG blocks and
16/16 branches agree; retail allocates eight bytes, while the candidate is a
frameless leaf. None of these observations proves a missing value, call, or
source object. The table addend residue in the selector follows its text
layout difference and is not an independent table-owner claim.

The earlier [30-function camera/transform control](kf2-game-camera-transform-30.md)
covers vector, matrix, world-translation, and event-pose helpers. This screen
adds the player-state/action path and deliberately leaves the separate
collision and actor-attack source campaigns untouched. KF1 source/history can
suggest spellings, but every verdict here is from KF2 GAME retail and the
current focused objects. No repository tests, lint, linked build, or broad
match was run in this scoped pass.

## Fresh camera/movement caller control

A later 22-claim safe GAME carve admitted 1,757 relocations with zero
withheld. Ten complete source units were compiled in isolation against that
current target, and focused quick builds checked both nonexact units. The
result is **19 strict exact, three WIP**, with no new exact closure or source
edit. The graph is connected through the camera-position state, the three
horizontal-move calls in `0x8002851c`, and the collision response.

| Unit | Exact function VAs | WIP function VAs and strict text |
| --- | --- | --- |
| `player_core_run` | `0x80023570`, `0x8002360c`, `0x80023814`, `0x80023868`, `0x80023984`, `0x80024034`, `0x800240cc`, `0x80024164`, `0x80024384`, `0x80024448` | — |
| `player_camera_turn` | `0x80028224`, `0x8002851c` | — |
| `player_distance_margin` | `0x80023430` | — |
| `player_reset_view` | `0x80023484` | — |
| `player_select_magic_action` | — | `0x8002722c`: 99.09091%; `0x800274ec`: 89.85240% |
| `player_status_cap` | `0x800247e4` | — |
| `player_collision_response` | — | `0x80027f78`: 95.91228% |
| `player_collision_bounds` | `0x80023384` | — |
| `player_weapon_transform_power` | `0x80026330`, `0x80026464` | — |
| `player_weapon_render` | `0x800316c8` | — |

The three WIPs retain the previously established raw field widths and
ordered calls. The selector's eight-byte retail frame, mover's one extra
retry-flag block and separate player-state address pairs, and response's
four extra candidate player-state pairs are the first bounded divergences;
none proves an omitted source operation. The exact sibling controls and
the `player_collision_bounds` dedicated compiler profile remain intact.
`player_weapon_render` was read as an exact control under the render owner's
source lane. No repository tests, lint, broad build, or linked build was run.
