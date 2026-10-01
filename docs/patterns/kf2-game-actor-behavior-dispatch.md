# GAME actor behavior dispatcher: control flow and data table

GAME `func_8003d184` is a source-owned 0x248c-byte dispatcher with one
`RODATA(0x800120d8, 0x3c4)` claim. Retail bounds the action selector to
`0..240`; all 241 reviewed table words point inside this function. The
source remains WIP, and its table identity is a curated ownership model.

The physical order of the retail case entry points is `2, 3, 0, 1, 12/16,
5, 13/17, 9, 10, 4, 23, 24, 11, 18, 14, 15, 19, 20, 21, 22, 26, 25,
27, 29, 28, 30, 240, 6/7/8/default`. Reordering the existing C case arms
without changing their bodies raised focused listing similarity from 44.8%
to 66.3%. This gives the compiler the retail fall-through layout without
claiming that case-order proximity proves a translation-unit boundary.

At retail `0x8003d1f8..0x8003d33c`, the sound gate uses signed `div` for
the second counter remainder, and its three branches share one call to
`func_8003d0e8` at `0x8003d334`. The C now uses a signed interval and a
shared call path. With these and the physically ordered sound arms, the
focused listing reached 68.2%. At that point its compiled CFG had 358 blocks and
180 branches versus retail's 410 and 213; this is still far from an exact
match. No previously exact function is owned by this source unit.

An isolated fresh GCC 2.5.7/ASPSX 1.07 compilation, compared strictly with
the safe-delinked retail module at that point, gave 82.40958% `.text` (retail
9356 bytes, candidate 8552 bytes) and 25.045723% `.rodata`. The source's
main switch contributes all 241 pointer relocations in the first 0x3c4
candidate RODATA bytes, matching the extent and row count of the retail
table. Candidate RODATA has 24 extra bytes: a zero word followed by five
more code-pointer relocations from a nested switch. That extra table is a
source CFG issue; the low RODATA percentage also reflects changed handler
offsets and does not by itself dispute the main table ownership.

The first major source-model gap began at retail case 3, `0x8003d3e4`.
The raw entry immediately reads actor byte `+0x0f` and signed halfword
`+0x70`; the previous C case 3 did not read the latter. Retail clears actor
state `+0x70` on initialization, clears the `0x10000` flag with mask
`0xfffeffff`, and then follows a state-dependent path through `0x8003d634`
before the shared motion tail at
`0x8003dd7c`. Its zero-state path calls
`actor_advance_animation_clamped`,
`actor_animation_crossed_phase` with phase `0x800`, and potentially
`func_800157f8`, `func_800365d8`, and `map_object_spawn_effect`. A later
state path calls the actor callback at `state_8017d118 + 0x0c` indirectly,
then may call `actor_set_lifecycle_and_home_position` or
`actor_set_home_position`. The previous C instead modeled this action as one
phase-crossing damage call. The current C reconstructs initialization,
zero-state effects and spawn conditions, later state increments and callback,
lifecycle handling, and the shared motion tail. The actor-group field at
`+0x30` is now `u16` because retail reads that full halfword twice to derive
the effect ID. The callback is spelled
`state_8017d118.active_table[19](actor)`: retail loads the table pointer at
state `+0x0c` and calls its `+0x4c` entry. The neighboring
`actor_set_lifecycle_and_home_position` family independently uses that form.
Its runtime target remains indirect.

Case 25's stream opcodes `0x8000`, `0x8001`, `0x8003`, `0x8002`, and `0x8004`
are compared explicitly in that retail order at `0x8003ed98..0x8003edf4`.
The old C `switch` emitted a five-pointer nested jump table absent from
retail. Equivalent explicit C conditions remove it. After both source
corrections, focused listing similarity reached 75.9%; fresh isolated strict
objdiff showed 87.99786% `.text` (9356 retail bytes, 8980 candidate bytes)
and 38.58921% `.rodata`. Reordering the case-3 state branches to follow
retail's early `<20` shared-tail jump lowers the focused listing to 75.3%
but raises strict `.text` to 88.27362% (8976 candidate bytes); strict
`.rodata` is 38.070538% because its handler offsets move. Both RODATA
sections are exactly 0x3c4 bytes and
have exactly 241 `R_MIPS_32` table rows. The table's handler offsets still
differ because the body is WIP, but there is no additional candidate table.
Focused neighbor controls remain unchanged after the shared actor header
refinement: the adjacent `game.actor_spatial_sound` unit reports both
`func_8003d084` and `func_8003d0e8` as SAME, and the following
`game.actor_fixup_group_targets` unit reports direct caller `func_8003f610`
as SAME. Its other function remains a separate 93.8% WIP.

The retail common exit path at `0x8003f3c8..0x8003f5ec` handles actors
linked to `actor_state.other_actor`, then updates the lifecycle footprint.
Its linked-actor paths copy state and motion, test collision flags, or copy
the eight-byte orientation block and derive position from a rotated group
offset and `func_8003c000` vertex result. The C now spells those operations,
including the full orientation block and its separate three-halfword angle
snapshot. Retail's default action path calls callback slot 17 directly;
the former C guard on the selector had no retail counterpart and was removed.
At that stage the focused listing was 77.5%, and isolated strict `.text` was
93.98846% (retail 9356 bytes, candidate 9500 bytes). RODATA remains exactly
0x3c4 bytes with 241 pointer relocations; handler offsets remain WIP. The
focused CFG is now 410/411 blocks, 213/215 branches, and 1/1 returns
(retail/candidate), with both return frontiers having two known incoming
edges. Both objects retain an unresolved indirect jump at `0x8003d364`,
so the CFG census is not a proof of full reachability.

Retail selector 22 at `0x8003e9cc` initializes its action and jumps back to
the clamped-animation block at `0x8003e918`, which selector 19's state 32
also enters. The C now shares that actual path instead of emitting a second
copy. Focused listing reaches 77.7%, strict `.text` 94.459595% (9456
candidate bytes versus 9356 retail), and strict `.rodata` 38.381744%; the
single 0x3c4-byte table still has exactly 241 pointer relocations. This is
still a WIP comparison, not a banked exact result. The focused CFG after
sharing is 410/408 blocks and 213/214 branches (retail/candidate), with
1/1 returns and two known incoming return edges on each side.

The focused and strict percentages are comparison aids only. The dispatcher
remains WIP; exactness requires the remaining control flow and ordered
referents to match. The 241-row RODATA table and its reviewed referents should
remain intact during further work.

The previous-target-type selection in action 5 is now spelled as a switch over
4, 18, 23, and 24. Retail `0x8003d8e8..0x8003d934` branches through those
four values and writes a byte at actor `+0x70`; the former OR expression
lowered to a different range check. In action 12/16, retail loads `17` in the
delay slot at `0x8003d72c` and passes it in the sixth argument slot to
`func_8003bf74` at `0x8003d7bc`. The C had mistakenly passed the unrelated
sound interval's low bit. Retail also zeroes the three motion angle halfwords
in descending order at `0x8003d754..0x8003d75c` and adds shifted cell
coordinates before the stored positions; the source now follows that order.
The same retail path at `0x8003d810..0x8003d838` decreases vertical motion
only above `baseline + range` and increases it only below
`baseline - range`; the previous C inequalities reversed both conditions.
Focused similarity is 78.4%; isolated strict `.text` is 95.26421% (9356
retail bytes, 9472 candidate bytes). The 0x3c4-byte RODATA extent still has
241 pointer rows, and strict `.rodata` is 37.603733% while handler offsets
remain WIP. The neighboring `actor_spatial_sound` functions are both SAME;
in `actor_fixup_group_targets`, `func_8003f610` and `func_8003f860` remain
SAME and the separate 93.8% WIP remains WIP.

Action 26's group halfword at `+0x32` is loaded once at retail
`0x8003ecb4` and written to actor `+0x4c`, `+0x4a`, then `+0x48`; its
initial zero stores use that same descending order. The source now reads the
group value once and follows both store orders. Actions 28 and 30 likewise
clear motion halfwords `+0x54`, `+0x52`, then `+0x50`. Action 30 checks a
zero collision result before testing bit `0x80`, then shares the position
update after an optional impact call. That structure explains the retail
branch order at `0x8003f2c0..0x8003f368` better than the previous pair of
independent conditions. Focused similarity is now 78.9%; the compiled CFG
has 413 blocks and 216 branches against retail's 410 and 213. Isolated
strict `.text` is 95.584435% (9356 retail bytes, 9440 candidate) against a
fresh safe-delinked target that includes 31 newly curated GAME BSS
HI16/LO16 pairs. All 31 pairs passed the safe-delink check with zero withheld
rows; the `.rodata` extent and 241 table pointers remain unchanged.

The target-group byte view starts with `unknown_00` at offset 0, so
`unknown_01[2]` is byte `+3`. Retail action 19 reads that byte at
`0x8003ef00`; its former `[3]` index read byte `+4`. Actions 0 and 25
already use `[2]` for the same byte. Retail's 241-row table places action
22's entry at `+0x1848`, before action 21 at `+0x1870`; moving the C arms
into that order preserves the shared action-19 clamp path and raises
strict `.text` to 96.79991% (9408 candidate bytes). The focused listing
is 79.4%, with CFG 410/411 blocks and 213/215 branches.

Action 14 loads `actor_state.other_actor` once at `0x8003e6cc..0x8003e6d0`,
then repeatedly reads its signed halfword `+0x58` through the same pointer.
Caching that pointer in the C action matches the load pattern across its four
state comparisons. The focused listing is 79.9%, and fresh isolated strict
`.text` reaches 97.208206% (9356 retail bytes, 9376 candidate bytes) against
the target with the 31 curated BSS pairs. The table remains 0x3c4 bytes with
241 rows; strict `.rodata` is 38.018673% because handler offsets remain WIP.

In action 11, retail state 0, 2, and 3 paths enter the common collision call
at `0x8003e590`; state 1 and unknown states go directly to the turn block at
`0x8003e598`. The prior C retested the original stage after the switch and
emitted another five-instruction branch chain. Moving the choice into the
switch exits follows the retail paths. Focused listing similarity rises to
82.1%; the compiled CFG now has 409 blocks and 213 branches against retail's
410 and 213. Isolated strict `.text` is 97.507484%, and candidate and retail
are both 9356 bytes; `.rodata` is 38.537346% with the same 241-row table.

Action 5 had a source behavior error after motion testing. Retail
`0x8003da40..0x8003da98` clears the high state byte when bit `0x100` is
absent, sets it and exits when the random check succeeds, and otherwise
falls through to toggle the low byte for any nonzero motion result. The old C
skipped that last toggle when bit `0x100` was present and the random check
failed. The corrected flow raises focused similarity to 84.6% and aligns the
CFG census at 410/410 blocks and 213/213 branches. Isolated strict `.text`
reaches 97.59085% (9356 retail, 9360 candidate bytes), and strict `.rodata`
rises to 95.07262%; the remaining table-row differences track handler
offsets. The first differing CFG successor is now at block 380, in the
common exit path.

Retail action 30 branches on the zero collision result at `0x8003f2c0`,
falls through into one position-copy block at `0x8003f2c8`, and jumps back
to that same block after the impact call at `0x8003f354`. Explicitly sharing
that source block yields matching known CFG successor lists by block order,
though the unresolved indirect jump still makes the CFG incomplete. Strict
`.text` improves to 98.327065% (9368 candidate bytes versus 9356 retail).
Strict `.rodata` falls to 39.834026% because the default handler address is
now displaced by eight bytes; this is an offset-sensitive table effect, not
a change to the 241 pointer rows or their identities. The focused display
is 82.5%. Keep the raw-backed shared position path while locating the
remaining upstream instruction count and footer differences.
