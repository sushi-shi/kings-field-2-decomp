# GAME actor and collision ten

These ten related GAME functions connect the collision dispatcher
to actor proximity, motion, targeting, and spatial sound. Each was checked
against retail extent, disassembly, CFG, incoming and outgoing references,
strings, source history, vendored inventory, and current match state. None has
supported Sony/Psy-Q archive attribution.

| GAME address | Decisive retail evidence | Final verdict |
| --- | --- | --- |
| `0x8002aaa4` | The 0xb60-byte collision dispatcher is called by the height wrappers and branches indirectly through candidate table `DAT_8001134c`. | **WIP, unclaimed**; the table owner and indirect targets remain unproved. |
| `0x80039108` | The 0x4c0-byte actor scorer calls geometry, angle, random, and target helpers and branches through candidate table `DAT_80011cd8`. | **WIP, unclaimed**; its scorer cases and table owner remain unresolved. |
| `0x80039c94` | The 0x684-byte player/actor combat helper calls `func_80039c14` eight times, an unresolved slot-18 callback, then training, target, and angle helpers. | **WIP, source claimed, 91.85132% recorded strict**; the focused listing still differs first in stack-argument load/save order and saved-register assignment. |
| `0x8003a9f4` | Scans 200 actors, excludes inactive/target type 3/current/masked actors, and tests an alternate Y position for flagged actors. | **Exact, 360/360 bytes strict**; branch-local distance queries reproduce retail's shared call setup. |
| `0x8003ab5c` | Companion scan omits the target-type-3 exclusion but keeps the alternate-position collision query. | **Exact, 344/344 bytes strict**; the same branch-local source form matches. |
| `0x8003ae50` | The 0x4ec-byte actor collision response calls the five-channel dispatcher, height probe, collision snapshot, angle, sine/cosine, and square root helpers. | **WIP, source claimed, 99.31746% recorded strict**; 58/58 CFG blocks and all direct calls agree. The 99.2% focused listing differs at obstacle-angle register assignment and mask timing; six neighboring functions remain exact. |
| `0x8003b5d0` | Drives actor vertical motion through floor, rising, falling, and trajectory cases with collision and damage calls. | **WIP, 87.95102% strict**; all direct calls agree, while switch order, one CFG block, and return-frontier layout differ. |
| `0x8003c3e0` | Repeatedly steers actor pitch/yaw toward a target Y offset of 1600 and advances a position vector. | **WIP, 99.64539% strict**; 23/23 CFG blocks and direct calls agree; only yaw-error and shifted-numerator registers differ in the focused listing. |
| `0x8003c614` | The 0xa70-byte actor/effect dispatcher calls vector, animation, spatial sound, and effect helpers and contains an indirect jump. | **WIP, source claimed, 55.3% focused / 79.86976% strict**; the 123 switch rows group cases correctly, while its frame and shared constructor path remain unresolved. |
| `0x8003d084` | Clamps a signed actor byte at +`0x4b` to ±12 and adds a scaled `rand` result to its note offset. | **Exact, 100% strict**; the centered random pitch jitter matches the retail listing and ordered relocation, and a fresh focused build keeps both this leaf and its `0x3d0e8` caller `SAME`. |

The two exact actor scans retain typed `VECTOR` paths. Their C source places
the distance query in each branch; the pinned compiler shares the call site
in precisely the retail control flow. A temporary collision-layer predicate
probe aligned `0x8003b5d0`'s two branch directions but left its broader switch
WIP, so the source was not changed. A temporary sound-offset subtraction probe
moved work into the `rand` call delay slot and was also discarded.

Strict GAME matching relinked 145/145 target units. The four adjacent actor
animation functions, three actor-group helpers, three actor-motion helpers,
and the actor spatial-sound wrapper retained their exact listings. Global
edge-check still stops on the three existing unrelated TMD/map-object
`.rodata` addend mismatches. The full `kf build` built PSX; GAME, OPEN, and END
retain the pre-existing unresolved first symbols `InitCARD`, `malloc`, and
`display_buffers`. No repository tests, bank, or commit were run.

A later focused revisit of `0x8003b5d0` kept the source at 66.5% listing
similarity with 40/40 CFG blocks, 21/21 branches, and all three preceding
motion helpers identical. Retail tests motion state `0x20` first and shares
one vertical-step block across three collision outcomes. A temporary explicit
`if` dispatch lowered listing similarity to 62.3%. A temporary shared-step
label aligned the `bnez` after the collision call, but compiled only 39 CFG
blocks with an extra return frontier (67.1% listing). Both experiments were
initially discarded pending a direct strict comparison.

The later `0x80039c94` focused comparison keeps its eight direct curve calls,
the unresolved slot-18 callback, and exact `0x80039c14` sibling. Retail loads
the eleventh stack argument as a halfword, but the existing explicit `u16`
uses already account for that instruction. Changing the formal `amount` type
from `s32` to `u16` in a temporary source enlarged the compiled frame from
168 to 176 bytes and lowered listing similarity from 62.1% to 55.2%; it was
discarded. The baseline has 72 retail versus 71 compiled CFG blocks, with
46/46 branches and seven return-frontier edges.
The one-block CFG difference is at the final linked-actor target-type guard:
retail branches over a redundant `li v0,0x10` and lands directly on the state
store, creating a separate one-instruction block. The probe puts
`move a0,linked` in the guard's delay slot and reaches the `li` before the
same store. Both retain the guarded target-type call and state write; no
source behavior is missing there.
An explicit source `goto` to that state-write join compiled identically to
the existing conditional and was discarded.

The related `0x8003a614` actor-to-player damage gate remains WIP at 81.2%
focused listing similarity. Retail forms camera X and Z through separate
absolute `player_state` pairs, while this source reuses a saved base for Z.
Exact `0x800396c4` uses the same typed `player_state.camera_position` fields
and emits separate pairs, so this difference does not justify splitting the
complete player-state owner into overlapping globals.

A later focused four-object rebuild and direct strict objdiff covered the
22-function actor animation/motion/position run after the shared actor
layout update. Eighteen functions are exact: `0x8003a9f4`, `0x8003ab5c`,
`0x8003acb4`, `0x8003ad90`, `0x8003adc4`, `0x8003ae20`,
`0x8003b33c`, `0x8003b520`, `0x8003b5bc`, `0x8003b9a4`,
`0x8003bae4`, `0x8003bba0`, `0x8003bcd0`, `0x8003be38`,
`0x8003bf74`, `0x8003c000`, `0x8003c10c`, and `0x8003c220`.

| GAME address | Current direct strict verdict | Residue |
| --- | ---: | --- |
| `0x8003ae50` | WIP, 99.31746% | Obstacle-angle mask and temporary register schedule. |
| `0x8003b5d0` | WIP, 87.95102% | Vertical-state dispatch and joined collision paths. |
| `0x8003bd40` | WIP, 86.53226% | Independent coordinate loads and angle/limit register assignment. |
| `0x8003c3e0` | WIP, 99.64539% | Yaw-error fraction registers. |

The selected retail disassembly confirms `0x8003b5d0` tests motion state
`0x20` before `0x30`, `0`, and `0x10`. The existing switch models those
states and the case bodies; prior `if`/shared-label probes lost CFG blocks.
KF1 actor movement uses a different state/record layout, so it supplies a
lead but no source transplant. The later direct comparison below supersedes
the earlier decision about the shared label.

The retail rising-state path at `0x8003b73c` is reached when the collision
result is zero, when the collision mask lacks bit 4 and actor flag `0x400` is
set, and when the flagged actor is below the collision height. All three
paths store the same next Y and increment the same signed speed halfword.
The source now expresses this proved common path once. At `0x8003b7b8`,
retail branches when collision bit 4 is clear, so the retained C lays out
the bit-set collision path before the clear-bit actor-flag check. Isolated
source probes moved strict `0x8003b5d0` `.text` from **76.17551%** (`980`
retail / `984` compiled bytes) to **84.25714%** for the common rise path,
then **87.95102%** (`980` retail / `988` compiled bytes) with the bit-4
branch layout. The retained focused quick build reports 68.2% WIP, 40/39 CFG blocks,
21/21 branches, and 10/11 return-frontier edges. Its first CFG successor
difference is in the earlier motion-state dispatch: retail tests `0x20`
first, while the probe tests `0x10` first. The three preceding functions
`0x8003b33c`, `0x8003b520`, and `0x8003b5bc` remain focused SAME and
isolated strict 100%. No exactness claim is made for the rising-state body.

A later raw-first source-only probe made the state dispatch explicit in
retail order (`0x20`, `<33`, `0`, `0x10`, then `0x30`), and another shared the
state-reset label used by the rising and falling paths. Both retained the
40/39 block gap and added an extra saved register; the focused listings were
68.9% and 68.4% versus the retained 68.2%. They were discarded. The gap is
currently attributable to dispatch layout, not evidence of a missing case.

The adjacent actor-group effect dispatcher `0x8003c614` remains WIP. In its
position-mode `-2` arm, retail loads the first script word at `0x8003c6a8`
before the first vertex call, the second at `0x8003c6b8` before the second
vertex call, and the third at `0x8003c6d8` after both calls. The C now
consumes each variadic word at that use site, preserving the caller's order
and removing the probe's eager three-word register retention. A focused
one-unit build improves the listing from 51.1% to 55.3%; direct strict
objdiff is 79.86976% (2,672 retail bytes versus 2,700 compiled). The
source remains WIP: its 192-byte retail frame versus the probe's 184 bytes,
kind-`0x79` constructor join, and switch-table target offsets still differ.
Off-tree probes that merely moved the common constructor label or duplicated
its calls did not reproduce retail's shared call at `0x8003ccc8` and were
discarded. A separate branch-local direction pointer meant to merge kind
`0x78` into that call fell to 51.0% focused and added outgoing arguments
unconsumed by constructor kind `0x78`, so it was discarded too. Raw kind
`0x16` writes `-1` to outgoing `20(sp)` at
`0x8003ca98`; it is not a five-argument-only exception to that shared call.
An isolated compile with the normalized Psy-Q 3.0 `STDARG.H` produced the
same instruction stream and 79.86976% strict score as the retained header;
the macro spelling does not explain this function's cursor or frame residue
under the pinned compiler.
The retail and compiled 123-word switch tables each partition their case
indices into the same 19 destinations; the table's low strict data score
comes from differing code-target offsets, not a missing or merged case arm.

The position-mode `-2` interpolation scales each signed coordinate delta by
256 before adding the actor position. Raw retail uses `subu`, `sll 8`, and
`addu` for each axis. The source now casts those intermediate operands to
`u32`, preserving the retail 32-bit wrap when a delta is negative or the sum
overflows, without changing the focused 55.3% listing. The remaining first
divergence is still the 192-byte retail frame versus the 184-byte probe frame;
the shared constructor join and switch-label offsets remain WIP.

## Ten-function actor motion and collision recheck (2026-10-01)

Focused rebuilds and GAME raw queries rechecked the ten WIPs below. The
stored strict report is stale; these percentages describe focused listings,
not exact closure. All exact siblings in the touched units remain `SAME`.

| GAME address | Focused | Current evidence boundary |
| --- | ---: | --- |
| `0x80039108` | 35.4% | The 131 jump-table row classes match retail, but the probe shares the case-9 angle calls with another case. Retail has a separate call pair. |
| `0x80039c94` | 62.1% | Eight fixed-curve calls and the indirect callback are present; the final linked-actor guard contributes one CFG-block layout difference. |
| `0x8003a318` | 97.5% | Radial attenuation, calls, and branches agree; two incoming stack-argument registers are exchanged. |
| `0x8003a614` | 81.2% | Damage gate and four direct calls agree; retail uses separate camera address loads where the probe reuses a base. |
| `0x8003ae50` | 99.2% | Collision paths and calls agree; only the obstacle-angle temporary and mask schedule differ. Six neighboring animation helpers are `SAME`. |
| `0x8003b5d0` | 68.2% | Retail dispatch tests state `0x20` first and shares a rise step. Moving all C switch cases to retail physical order fell to 39.1% focused and was reverted; three preceding motion helpers are `SAME`. |
| `0x8003bd40` | 68.2% | Signed X/Z magnitude, tolerance gate, and three calls agree; independent coordinate loads and saved-register assignment differ. Six motion siblings are `SAME`. |
| `0x8003c3e0` | 95.4% | Movement calls and typed yaw error agree; the yaw fraction and shifted numerator occupy different registers. Three adjacent group-position helpers are `SAME`. |
| `0x8003f7ec` | 93.8% | The 40-by-16 target walk agrees; sentinel setup and pointer-add operand order differ, with both neighboring helpers `SAME`. |
| `0x8003fa68` | 57.5% | Retail has four separate collision calls for effect types 1–4; the compiler merges the same source call paths. No proven caller reaches the uninitialized default-result cases. |

The `0x8003b5d0` case-order trial changed physical layout without recovering
the one-block CFG gap; preserving the supported state semantics and exact
siblings is preferable. None of the ten newly reached exact, and no source
change from this recheck was retained. Only focused builds and retail queries
were run.

A fresh isolated profile control for GAME `0x8003fa68` left the C and retail
call arguments unchanged. GCC 2.6.0 lowered strict text from the configured
GCC 2.5.7 result of **70.73333%** to **57.02667%** over 300 bytes;
GCC 2.5.7 with `-fno-cse-skip-blocks` emitted the same **70.73333%** result.
Retail's four separate `func_8002b9d4` sites and the source's four cases are
still folded by the configured compiler, so neither profile change is kept.

## Vertical-state dispatch follow-up (2026-10-02)

Fresh retail disassembly of `0x8003b5d0` checks state `0x20` first, then a
signed `< 33` range, state `0`, state `0x10`, and finally state `0x30`.
Expressing this sparse dispatch explicitly in C preserves the four state
bodies, their calls and constants, and the three exact preceding siblings.
The first seven ordered control transfers now agree with retail; focused
listing similarity moves from 68.2% to 69.3%. Isolated strict text for this
function moves from 87.95102% to 89.616325%. The remaining 40/39 CFG-block
gap and saved-register/return-join differences keep it WIP. A simple physical
case reorder and the opposite high-range branch were discarded.

## State-local motion values

The state-0, state-0x10, and state-0x30 paths calculate independent vertical
positions. Scoping each path's `next_y` to its own arm, with collision and
phase values confined to the arms that use them, restores the retail 72-byte
frame and four saved registers without changing the operations or calls.
The focused listing rises from 69.3% to 94.0%; isolated strict text rises
from 89.616325% to 94.37551%. The other three functions in the unit remain
strict 100% and focused `SAME`. Retail and probe still have 40/39 CFG blocks
and 21/21 branches. The remaining code differs around the state-0x10 join
and state-0x20 height update, so the function remains WIP.
A local signed velocity variable compiled identically to the retained source.
Routing the state-0x10 settle path through the state-0x20 exit reduced the
focused listing to 88.3% and was discarded; the target's physical join is
not reproduced by that source spelling.

Caching the collision floor once in the state-0x10 ceiling branch reflects
retail's single load at `0x8003b7d4` and subsequent use for its comparison
and position store. That natural local raises focused similarity from 94.0%
to 94.3% and isolated strict text from 94.37551% to 96.42041%; the three
siblings remain strict exact. The remaining differences are the state-0x10
return join and the state-0x20 independent load schedule, with no missing
call or field referent established.

A fresh isolated control confirms this retained source at **96.42041%**
strict text and 94.3% focused listing. Retail jumps from the state-`0x10`
settle arm at body `+0x238` to the reset at `+0x2a0`, whereas the probe
stores the state byte in its direct return jump's delay slot. An off-tree C
label joining that settle arm with the state-`0x20` reset is semantically
faithful but GCC instead merges the separate clear-bit exit with that reset:
strict text falls to **95.216324%**, focused listing to 88.6%, and known
return frontiers move from 10/11 to 10/9 against retail's 10. The trial was
reverted. Casting the state-`0x20` speed read to `u16` before its modular
halfword increment emits the retained object byte-for-byte and was also not
kept. All 59 ordered relocation type/symbol pairs and three exact sibling
functions remain unchanged in these controls.

An isolated actor-damage follow-up (2026-10-02) reconfirmed
`func_8003a318` at **99.86911%** strict text, `func_8003a614` at
**96.91011%**, and the adjacent `func_8003a778` at **100%**. The first
function's early stack loads assign `amount_and_flags` and `falloff` to
opposite temporary registers while retaining all 26 CFG blocks, 13
branches, calls, and referents. Moving the independent amount-mask
assignment before the damage-position flag test in an off-tree C copy was
semantically equivalent but lowered strict text to **91.272255%**; the exact
sibling and second WIP stayed unchanged. This source-order trial was
discarded. The second function's six blocks, three branches, four calls, and
field widths still agree; retail rematerializes the player camera Z address
where the probe reuses the existing camera base. No supported C change was
retained, and the tracked source stayed unchanged. Verification used focused
`kf try` and isolated strict objdiff only.

The sole proven external caller of `0x8003a318` is GAME `0x8003ff18` at
`0x8004008c`; its own source body is strict exact. It passes the fifth input
as the callee's `u16` falloff and the fifteenth as amount/flags. Retail and
candidate both load amount/flags before falloff and apply the same
`0x8000`/`0x7fff` masks. Their first difference is the choice of `$v1` and
`$a0` for those two independent loaded values, not argument order or width.
The exact caller control therefore does not support changing the callee
signature to chase the near-exact score.

## Actor target/damage call family: 27 current strict verdicts

A post-checkpoint isolated rebuild of nine GAME units with their complete
manifest profile, followed by direct strict objdiff and focused `kf try`,
rechecked the scorer, its actor target/collision callers, damage handlers,
animation and group helpers. The table lists every function in those units:
**16 exact controls and 11 WIPs**. The 241-row behavior table remains byte
exact, while the scorer's 131-row table remains WIP with all ten target
classes preserved. This is a linked call/data campaign, not a claim that
these nine source modules were one original TU. Earlier percentages in this
note are historical focused listings; this table is the current strict result.

| GAME address | Strict text | Verdict |
| --- | ---: | --- |
| `0x80039108` | 93.93092% | WIP; case-9 tolerance Boolean joins the callback branch, 59/59 CFG and 37/36 branches. |
| `0x8003a318` | 99.86911% | WIP; early `amount_and_flags`/`falloff` stack loads use opposite temporaries. |
| `0x8003a614` | 96.91011% | WIP; four calls and 6/6 CFG agree; player camera Z base is rematerialized only in retail. |
| `0x8003a778` | 100% | Exact actor recipient control. |
| `0x8003983c` | 99.19598% | WIP; 37/37 CFG and 24/24 branches; actor and constant saved-register roles differ. |
| `0x80039b58` | 90.95744% | WIP; 9/9 CFG and 4/4 branches; free-slot byte lifetime differs. |
| `0x80039c14` | 100% | Exact fixed-curve sibling. |
| `0x80039c94` | 98.11751% | WIP; eight curve calls and 72/72 CFG agree; accumulator/store schedule remains. |
| `0x8003a9f4` | 100% | Exact animation control. |
| `0x8003ab5c` | 100% | Exact animation control. |
| `0x8003acb4` | 100% | Exact actor-bind control. |
| `0x8003ad90` | 100% | Exact animation control. |
| `0x8003adc4` | 100% | Exact animation control. |
| `0x8003ae20` | 100% | Exact phase-crossing control. |
| `0x8003ae50` | 99.31746% | WIP; 58/58 CFG and 34/34 branches; independent store/delay schedule. |
| `0x8003b33c` | 100% | Exact actor collision control. |
| `0x8003b520` | 100% | Exact actor trajectory caller. |
| `0x8003b5bc` | 100% | Exact state setter. |
| `0x8003b5d0` | 96.42041% | WIP; 40/39 CFG and 21/21 branches; state-`0x20` reset join differs. |
| `0x8003c000` | 100% | Exact group position control. |
| `0x8003c10c` | 100% | Exact group position control. |
| `0x8003c220` | 100% | Exact group position control. |
| `0x8003c3e0` | 99.64539% | WIP; 23/23 CFG and 11/11 branches; yaw intermediate register differs. |
| `0x8003f610` | 100% | Exact target-fixup caller. |
| `0x8003f7ec` | 85.86207% | WIP; 9/9 CFG and 4/4 branches; immediate and commutative operand order differs. |
| `0x8003f860` | 100% | Exact target-fixup sibling. |
| `0x8003d184` | 99.931595% | WIP; 410/410 CFG, 213/213 branches, exact table; repeated retail `lhu` is probe `nop`. |

The scorer's raw case-9 tail branches from the tolerance result at
`0x80039298` directly into scoring and jumps from `0x800392a0` to the
return path on failure. The source already makes the separate angle and
tolerance calls. Positive-guard and return-shape probes previously moved
switch addends or regressed CFG, so those forms were not repeated. In the
damage sender, a KF1 radial-damage analogue suggested checking the local
falloff width. Removing redundant `u16` casts and, separately, declaring
`scaled_amount` as `u16` both emitted the retained object byte-for-byte in
off-tree complete-profile builds; neither establishes a missing width fact.
The independent amount/flag source-order trial above regressed strict text.
No actor C, header, compiler profile, or curated identity edit was retained;
the exact sibling controls remained exact. Only focused unit builds and
isolated strict comparisons were used.

Two later off-tree type/control checks constrain the open source model.
Spelling the scorer default as explicit `type < 128` success and callback
failure branches, exactly the two retail decisions at `0x80039544` and
`0x8003956c`, emits the retained scorer object byte-for-byte; it does not
separate the case-9 Boolean branch. KF1's radial-damage handler suggested
four narrow leading arguments for `0x8003a614`, but trying `u16, u16, s16,
s16` made the probe emit `andi` and shift/sign-extension instructions at
entry that retail does not have. Strict text fell from **96.91011%** to
**85.33708%**, while exact `0x8003a778` remained exact. The current wider
arguments are therefore better supported by KF2 raw entry instructions;
the KF1 signature is not transferred. Both trials were discarded.

For `0x8003b5d0` state `0x20`, retail separately reads the signed and
unsigned views of actor halfword `+0x52` before the floor comparison. An
off-tree `u16 next_speed` local that stores the increment and tests its
signed view is a faithful expression of that value chain, but the pinned
probe emitted a SHA256-identical object to the retained source. Its three
exact siblings stayed exact; this trial does not explain the 40/39 block
join and was discarded.

An ordered relocation check of all nine refreshed actor units found matching
type/symbol sequences in the scorer (64 text and 131 table rows), lifecycle
(52), fixed curve (37), animation (80), motion collision (59), group
position (37), fixup (37), and behavior dispatcher (341 text and 241 table
rows). The damage unit is the exception: retail has **34** text rows and
the probe **30**. Two retail-only `player_state` HI16/LO16 pairs occur before
the distance call and before `vector_xz_to_angle` in `0x8003a614`; the source
already reads the corresponding camera fields, while GCC reuses an earlier
base. Splitting the X/Z expressions into natural typed locals compiled
SHA256-identically to the retained object, so the missing address
materializations are not grounds for a false global owner or duplicate
source read. This trial was discarded with the exact sibling intact.

The scorer's direct caller and adjacent target helpers were also rebuilt in
their own complete-profile units and compared with strict objdiff. All eight
functions are **100%**: `actor_select_best_target` (`0x800395c8`),
`actor_set_target` (`0x800390d0`),
`actor_select_target_for_player_distance` (`0x800396c4`),
`actor_find_target_of_type` (`0x80039710`),
`actor_select_target_type_in_own_group` (`0x80039758`),
`actor_reset_target_and_reselect` (`0x800397a8`), and the two
`actor_home_wrapper` claims (`0x80038efc`, `0x80038f20`). They are exact
caller/field controls, not new closures; no source changed in this screen.

## Connected target and damage callees: 27 strict controls

The actor scorer and damage paths directly call the corresponding angle,
distance, random-scalar, and player-damage functions below. The actor behavior and
fixed-curve callers also use the vector and interpolation siblings in their
respective source units. Each listed claim was freshly rebuilt with its
complete pinned unit profile and compared against its own GAME retail object
with direct strict objdiff. Every verdict is **100%**; the player damage
switch tables are also exact. These are reconfirmed controls, not newly
matched functions, and no source or ownership metadata was changed.

| GAME address | Function | Verdict |
| --- | --- | --- |
| `0x80015034` | `pitch_yaw_to_forward_vector` | Exact, 208 text bytes. |
| `0x80015104` | `vector_rotate_yxz` | Exact, 68 text bytes. |
| `0x80015148` | `vector2i_scale_shift11` | Exact, 64 text bytes. |
| `0x80015188` | `vector3s_scale_shift12` | Exact, 92 text bytes. |
| `0x800151e4` | `vector2i_scale_shift12` | Exact, 64 text bytes. |
| `0x80015224` | `vector3s_scale_shift12_alt` | Exact, 92 text bytes. |
| `0x80015280` | `vector3i_add_xz` | Exact, 44 text bytes. |
| `0x800152ac` | `angle_within_tolerance` | Exact, 60 text bytes. |
| `0x800152e8` | `angle_mod_delta_le_half_turn` | Exact, 16 text bytes. |
| `0x800152f8` | `angle_shortest_delta` | Exact, 32 text bytes. |
| `0x80015318` | `vector_xz_to_angle` | Exact, 336 text bytes. |
| `0x80015468` | `fixed_vector2_length` | Exact, 64 text bytes. |
| `0x800154a8` | `fixed_vector3_length` | Exact, 84 text bytes. |
| `0x800154fc` | `func_800154fc` | Exact, 120 text bytes. |
| `0x80015574` | `func_80015574` | Exact, 48 text bytes. |
| `0x800155a4` | `vector_distance_to_point` | Exact, 244 text bytes. |
| `0x80015698` | `func_80015698` | Exact, 276 text bytes. |
| `0x800157ac` | `func_800157ac` | Exact, 76 text bytes. |
| `0x800157f8` | `func_800157f8` | Exact, 84 text bytes. |
| `0x8001584c` | `func_8001584c` | Exact, 32 text bytes. |
| `0x8001586c` | `func_8001586c` | Exact, 72 text bytes. |
| `0x800158b4` | `func_800158b4` | Exact, 100 text bytes. |
| `0x80024498` | `func_80024498` | Exact, 844 text and 32 table bytes. |
| `0x800248a8` | `func_800248a8` | Exact, 1020 text and 28 table bytes. |
| `0x80024ca4` | `func_80024ca4` | Exact, 560 text bytes. |
| `0x800397d8` | `func_800397d8` | Exact, 44 text bytes. |
| `0x80039804` | `func_80039804` | Exact, 56 text bytes. |

This exact callee chain bounds the remaining scorer/damage WIPs to their
callers' own instruction ordering, case joins, and address materialization.
It gives no basis for changing a callee signature, table owner, or global
placement to raise a caller score.

One off-tree source-structure check targeted the remaining lifecycle scan
`0x80039b58`. Retail saves `0xff` in `$s4` and uses two separate branches
for slot state and group index; the retained C uses one short-circuit test.
Spelling those branches as two early `goto next` guards is a faithful raw
control-flow form, but the complete-profile probe produced a SHA256-identical
object and the same **90.95744%** strict score. The `0x8003983c` sibling
remained at **99.19598%**. No C edit was retained.

The fixed-curve WIP `0x80039c94` does not justify an extra 8-byte motion
local to explain its 168-versus-160-byte frame. Retail passes stack `sp+40`
as the angle result to `func_800154fc`, then passes `actor+80` directly as
the output of `pitch_yaw_to_forward_vector` and the input of
`vector3s_scale_shift12`; the source has those same object destinations.
The frame difference remains unattributed after the exact vector callees
were confirmed, so no padding or surrogate `SVECTOR` was introduced.

The exact player damage reaction `0x80024498` calls
`func_80029464` and `func_800294f8`; a fresh isolated strict rebuild of
their `game.player_reaction` unit confirms both callees at **100%** (148 and
120 text bytes). Its other fifteen claims are unchanged: fourteen exact
controls and the previously known `0x8002985c` at **99.26569%**. That large
dispatcher is outside this direct damage edge; no edit was inferred from its
shared unit placement.

The lifecycle WIP `0x8003983c` directly calls the actor preparation and
home-position chain. Fresh isolated strict comparisons confirm all five
claims in that chain at **100%**: `actor_set_home_position` (`0x80038d04`,
192 bytes), `actor_copy_group_defaults` (`0x80038dc4`, 116),
`actor_initialize_from_group` (`0x80038e38`, 196),
`actor_prepare_and_initialize` (`0x80038ff0`, 88), and its adjacent
`func_80039048` wrapper (56). These are exact caller/callee controls and no
new closures. The lifecycle WIP's divergent saved-register roles do not
justify changing their interfaces.

One final off-tree scorer control replaced case 9's successful
`goto score_target` with a natural `break` from the switch. Retail's case-9
success does branch to the same post-switch scoring tail, so this form
preserves the raw behavior. The complete-profile probe nevertheless emitted
the retained object byte-for-byte: **93.93092%** text, **96.183205%**
table, and the same 37/36 branch gap. No C edit was retained.
The corresponding positive guard with `break` reproduced the prior
positive-guard regression (**89.68750%** text, **37.02290%** table) and was
also discarded.

The direct player-damage chain exposed one real interface correction.
`render_frame.c` defines `func_800335a0(const VECTOR *, const SVECTOR *)`,
and its curated GAME identity and nonnull callers agree; `player_reaction.c`
still declared two `s32` parameters for its two `(0, 0)` calls. After the
shared pointer declaration was added to `graphics.h`, the stale local
declaration was removed. The focused unit has **16/17 SAME** listings; fresh
isolated strict leaves all sixteen exact text claims, its 32-byte data and
76-byte rodata exact, and `0x8002985c` at **99.26569%**. The complete object
is SHA256-identical to the pre-correction one, as expected for null O32
arguments. This fixes source type consistency without claiming a new match.
The actor damage caller also now uses `player.h`'s existing semantic
`func_800248a8` declaration instead of a duplicate twelve-argument local
prototype. Focused output and fresh isolated strict remain **99.86911%**,
**96.91011%**, and **100%** for its three claims; the whole object is
SHA256-identical to baseline. In particular, `0x8003a614`'s fourth damage
word is the exact callee's `status_flags` slot, a semantic mapping that the
shared declaration now exposes without changing the O32 call.

## Actor lifecycle and AI network: 22 strict verdicts

This read-only follow-up follows actor preparation, target behavior, group
effects, spatial sound, steering, and trajectory calls. Each row is a current
GAME complete-profile isolated strict comparison. The pool, spatial, motion,
trajectory, and group-effect units were freshly rebuilt; the seven scalar,
interpolation, and target-byte controls were rebuilt earlier in the same
checkpoint. The result is **19 exact controls and three WIPs**.

| GAME address | Function | Strict verdict |
| --- | --- | --- |
| `0x800157ac` | `func_800157ac` | 100%, 76 text bytes. |
| `0x800157f8` | `func_800157f8` | 100%, 84 text bytes. |
| `0x8001584c` | `func_8001584c` | 100%, 32 text bytes. |
| `0x8001586c` | `func_8001586c` | 100%, 72 text bytes. |
| `0x800158b4` | `func_800158b4` | 100%, 100 text bytes. |
| `0x80015918` | `func_80015918` | WIP 95.49419%, 688 text bytes. |
| `0x80015bc8` | `func_80015bc8` | 100%, 280 text bytes. |
| `0x80015ce0` | `func_80015ce0` | 100%, 112 text bytes. |
| `0x80038cc8` | `actor_pool_find_free` | 100%, 60 text bytes. |
| `0x80039080` | `actor_pool_clear` | 100%, 80 text bytes. |
| `0x800397d8` | `func_800397d8` | 100%, 44 text bytes. |
| `0x80039804` | `func_80039804` | 100%, 56 text bytes. |
| `0x8003b9a4` | `func_8003b9a4` | 100%, 320 text bytes. |
| `0x8003bae4` | `func_8003bae4` | 100%, 188 text bytes. |
| `0x8003bba0` | `func_8003bba0` | 100%, 304 text bytes. |
| `0x8003bcd0` | `func_8003bcd0` | 100%, 112 text bytes. |
| `0x8003bd40` | `func_8003bd40` | WIP 86.53226%, 248 text bytes. |
| `0x8003be38` | `func_8003be38` | 100%, 316 text bytes. |
| `0x8003bf74` | `func_8003bf74` | 100%, 140 text bytes. |
| `0x8003c614` | `func_8003c614` | WIP 86.041916% text, 37.90650% 492-byte table. |
| `0x8003d084` | `func_8003d084` | 100%, 100 text bytes. |
| `0x8003d0e8` | `func_8003d0e8` | 100%, 156 text bytes. |

The three WIPs retain current raw bounds: `0x80015918` has 41/41 CFG and
24/24 branches with two `SquareRoot0` and one angle call; `0x8003bd40`
has 9/9 CFG, 5/5 branches, and aligned angle/steering calls; `0x8003c614`
has 45/45 CFG, 15/15 branches, 123 table rows and 19 preserved target
classes. Its retail versus probe has 11/7 physical direction-helper and
9/10 constructor call sites, although the source already spells the
meaningful calls and arguments. Prior natural controls in the trajectory,
motion, and group-effect dossiers were negative. No source, profile, or
table-owner correction is supported by this screen; no new 100% closure was
created.

## Actor-linked collision helpers: 29 current strict verdicts

The actor home, motion, and vertical-state paths reach the GAME collision
grid, shape dispatcher, height wrappers, channel helpers, and player collision
response. A fresh complete-profile isolated strict comparison of eleven source
units gives **19 exact claims and ten WIPs**. The collision-height dossier contains
the detailed raw field and CFG audits; this table records the current
per-function result, including the exact data controls, without repeating
its negative source probes.

| GAME address | Strict text | Verdict |
| --- | ---: | --- |
| `0x80023384` | 100% | Exact player collision bounds control. |
| `0x80023430` | 100% | Exact player distance-margin helper. |
| `0x80023484` | 100% | Exact player view-reset helper. |
| `0x80027928` | 100% | Exact player collision-sound helper. |
| `0x80027988` | 100% | Exact player collision-sound helper. |
| `0x800279cc` | 98.239670% | WIP player collision sound; 70/70 CFG, 37/37 branches and 12 ordered calls agree, with two extra retail player-state address rematerializations. |
| `0x80027f78` | 95.912280% | WIP player collision response; 27/27 CFG and 14/14 branches agree, with motion-vector base reuse residue. |
| `0x8002a988` | 100% | Exact grid sampler; its 10-byte default cell is exact. |
| `0x8002aaa4` | 57.52610% | WIP shape dispatcher, 174/172 CFG and 99/98 branches; 196-byte table 27.55102%, but all 49 rows retain 13 target classes. |
| `0x8002b604` | 100% | Exact height-probe caller. |
| `0x8002b67c` | 94.895836% | WIP cached-height reload after store; fields and shape call agree. |
| `0x8002b73c` | 98.404260% | WIP row/column pointer and index schedule; byte store agrees. |
| `0x8002b7f8` | 100% | Exact second shape caller. |
| `0x8002b874` | 91.500000% | WIP source-selection load order; interaction-height store agrees. |
| `0x8002b9d4` | 95.379310% | WIP cache-store path, 64/56-byte frame; call and result agree. |
| `0x8002bc18` | 100% | Exact collision-row helper. |
| `0x8002bd3c` | 100% | Exact collision-row helper. |
| `0x8002bdbc` | 100% | Exact collision-row helper. |
| `0x8002be9c` | 100% | Exact collision-row helper. |
| `0x8002bf38` | 100% | Exact collision-row helper. |
| `0x8002bfac` | 100% | Exact collision-row helper. |
| `0x8002bfd4` | 73.912620% | WIP mask rasterizer; signed halfword tests and four incoming calls agree. |
| `0x8002c170` | 100% | Exact mask-run scanner. |
| `0x8002c1d4` | 100% | Exact mask-row fill. |
| `0x8002c290` | 100% | Exact mask-cell update. |
| `0x8002c424` | 98.299320% | WIP typed cell cursor; saved-register roles differ. |
| `0x8002c670` | 91.624245% | WIP mask sweep; four rasterizer calls and 28-byte shape table agree, with two extra retail address pairs in the nonzero-layer arm. |
| `0x800314fc` | 100% | Exact collision-channel draw helper. |
| `0x80031634` | 100% | Exact collision-channel add helper. |

The height-wrapper unit's 3,520-byte initialized row datum is strict
exact. The shape unit has no missing external data referent; its known
unreachable case-`0x11` branch and separate case-`0x30`/`0x32` arithmetic
tails still require source/compiler attribution. The other nine WIPs have
retail calls, field families, and CFG paths already represented in source.
For `0x800279cc`, retail's landing tail separately materializes
`player_state+0x110` for the halfword load and store; the candidate addresses
that same field from its live `player_state+0x138` base at offset `-0x28`.
This explains the two retail-only address pairs without a missing field.
No C, header, or curated owner edit was justified by this screen.
