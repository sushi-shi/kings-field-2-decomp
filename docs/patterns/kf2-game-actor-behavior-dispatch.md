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

A fresh image-qualified pass over `actor_fixup_group_targets` at `0x8003f7ec`
confirmed its 9/9 blocks, 4/4 branches, single return, and two validated
referents. Its focused listing differs only in the order of the independent
`0xff`/`-1` setup instructions and the commuted operands of the target-offset
`addu` in a branch delay slot. Both exact sibling listings remain `SAME`.
Neither difference establishes a different pointer owner or source operation,
so the WIP body is unchanged.

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

Action 26's direct state-1 path joins state 0 at `0x8003ecd4`; that jump
clears actor byte `+0x0d` in its delay slot before the shared vertical-motion
tail at `0x8003f10c`. The prior C cleared the byte only in state 0. Moving
the store into the shared state-1 fall-through fixes the behavior and raises
focused similarity to 86.4%. The known CFG successor lists still agree;
strict `.text` is 98.35699% (9368 candidate bytes), while `.rodata` remains
39.834026% from the displaced default-handler offset.

In the same action, reading group fields `+0x12` and `+0x14` before changing
vertical position gives the compiler the retail load/store interleave at
`0x8003ec94..0x8003ecb0`. The focused listing reaches 86.6%, and isolated
strict `.text` reaches 98.55366%; candidate size and table contents do not
change.

Retail action 26's state-1 jump at `0x8003ecd4` enters `0x8003f10c`, the
same vertical-motion and timer tail reached by action 29. Sharing that tail
in C removes the candidate's separate action-26 copy and places the
`+0x0d` zero store in the jump delay slot. Focused similarity reaches 89.3%;
isolated strict `.text` is 98.92133% (9360 candidate bytes versus 9356
retail), and strict `.rodata` recovers to 95.591286% because the default
handler offset is nearly aligned again. The known CFG successor lists still
agree by block order, with 410/410 blocks and 213/213 branches.

Action 25's `0x8002` stream form advances its persisted word index after
each of three operands: `index+2` at `0x8003ee04`, `index+3` at
`0x8003ee14`, and `index+4` at `0x8003ee20`. The C previously persisted
only the final index. Spelling all three stores matches that raw behavior.
The decoded 16-bit opcode is used as a promoted 32-bit value; keeping it
as `s32` in C removes a redundant candidate zero-extension. Focused
similarity is 87.4%, and isolated strict `.text` is 99.32193% (9376
candidate bytes versus 9356 retail). The action-25 changes shift the
default handler to `+16` versus retail, so strict `.rodata` is 39.834026%
despite the same 0x3c4-byte, 241-row table. The known CFG successor lists
remain aligned; the remaining count residue should be resolved from raw
instructions rather than by changing the table.

The stream base in action 25 is the existing target pointer in retail
(`$s4` at `0x8003ed80` and `0x8003eda4`). Removing a duplicate C `script`
pointer local lets the candidate use that pointer directly and removes an
extra `move s2,s4`. Isolated strict `.text` reaches 99.36896% (9372
candidate bytes); focused similarity is 87.5%, and the known CFG remains
aligned. The table offset effect remains while other count residues remain.

In the common linked-actor exit, retail loads actor slot byte `+0` once at
`0x8003f3e4` and reuses it when comparing the other actor's target type at
`0x8003f418`. Caching that byte in the source removes a later candidate
reload while preserving the snapshot semantics. Focused similarity is 87.7%;
strict `.text` is 99.4224% (9368 candidate bytes), with the same aligned
known CFG. The RODATA default-handler offset is still `+12`.

The shared action-26/29 motion tail first loads position `+0x30` and timer
`+0x72`, then subtracts 256 and 1 respectively at `0x8003f10c..0x8003f128`.
Keeping both next values live before the stores and treating the arithmetic
timer as a promoted `s32` reproduces the retail interleave, `addiu -1`, and
left-shift zero test. The candidate now has one extra instruction overall:
strict `.text` is 99.567764% (9360 versus 9356 retail bytes), focused
similarity is 87.9%, and the known CFG remains aligned. The 241-row table
still has a displaced default handler, so strict `.rodata` remains 39.834026%.

Action 21's state-2 timer is likewise read as `lhu` and decremented as a
promoted `s32` before storing the halfword at retail `0x8003ebb4..0x8003ebcc`.
The prior unsigned postfix decrement made GCC materialize `0xffff`, add a
register, then materialize `-1` again for the signed `-1` comparison. A
promoted next-value local emits the retail `addiu -1` and eliminates that
extra instruction. Focused similarity rises to 92.6%; candidate and retail
`.text` are both 9356 bytes, with isolated strict `.text` at 99.66182% and
strict `.rodata` at 96.62863%. The default-handler and action 25-30 table
addends now match exactly; the remaining differing handler addends are
`-4` on rows 0, 1, 4, 5, 9-13, 16, 17, 23, and 24. The full known CFG
successor lists remain aligned.

The first remaining raw instruction-selection difference is before the main
table: retail loads target halfword `+0x0a` twice at `0x8003d1f8` and
`0x8003d1fc`, while the pinned probe common-subexpresses the two source
uses into one load and schedules a `nop`. Repeating the field access in C
produced the same candidate listing, so this is an unattributed codegen
residue. Action 3's zero-state branch has a similar residue: retail places
`move a0,v1` in its `0x8003d424` delay slot and loads comparison constant
99 only at `0x8003d558`; the candidate hoists that constant into the delay
slot and uses `$v1` directly for the state increment, leaving subsequent
handler offsets four bytes early. Hoisting the current-state C local did
not change the candidate listing. The retail frame is 144 bytes versus the
candidate's 128, with corresponding local stack-slot differences. None of
these differences justifies fake locals, volatile accesses, or assembly.
This dispatcher remains WIP at strict 99.66182% `.text` and 96.62863%
`.rodata`, not an exact or bankable match.

Action 3's nonzero-state path has a more precise C form. Retail loads the
signed halfword at `0x8003d41c`, copies it to `$a0` in the branch delay slot
at `0x8003d428`, compares it with 99 at `0x8003d558`, and stores its increment
at `0x8003d564`. A postfix increment of the stored halfword emits that
instruction sequence; the prior assignment from `current_state + 1` hoisted
the 99 constant into the delay slot. This change alone adds one instruction
to the full object, so it is paired with an independently evidenced action-11
correction: retail calls `func_8003b5bc` at `0x8003e374` and clears actor
state in that call's delay slot at `0x8003e378`. The state must therefore be
cleared before the call in C. The prior source placed the clear afterward,
leaving a `nop` delay slot and one extra instruction.

With both corrections, the focused listing rises to 97.3% with 410/410
known CFG blocks, 213/213 branches, and matching known successor lists.
Isolated strict comparison against the refreshed one-VA GAME target reports
99.77725% `.text` over exactly 9356 bytes on each side and **100% `.rodata`**
over 964 bytes, including all 241 switch-table rows. All 341 ordered text
relocations and 241 ordered RODATA relocations have the same offsets, types,
and referents; all text-relocation masked immediates agree as well. Raw
`.text` differs at 67 of 2339 aligned instruction words. Objdiff classifies
61 argument mismatches, three replacements, and one insert/delete pair per
side. The earliest differences are the 144-byte retail versus 136-byte
candidate frame and saved-register offsets, then `li 3`/`li 0x93` in `$v0`
versus `$v1` at offsets `0x40`/`0x44`. The first missing instruction is the
entry's repeated target-halfword load versus probe CSE. Later residues are
operand/branch orientation, saved-register choices, and local-slot schedule.
No exact or banked claim is made.

The entry's `actor_state.unknown_93a4` assignment is now spelled as the two
source branches for the flag-4 choice. This preserves the same single store
and 410/410 known CFG blocks, but the probe uses retail's `$v0` for constants
3 and `0x93` at offsets `0x40`/`0x44` instead of `$v1`. Focused similarity
is 97.4%; strict `.text` is 99.78367%, with 64 of 2339 raw words unequal.
Strict `.rodata` remains 100%, and the 341 text plus 241 RODATA ordered
relocations still match offsets, types, referents, and masked addends. The
first non-frame instruction difference is now the repeated retail
target-halfword load at offset `0x78` that the probe coalesces.

Two remaining branch-orientation differences exposed reversed C conditions.
Action 5 at `0x8003d98c..0x8003d9a0` computes `word_14 < distance` and
skips writing state byte 1 when true; the write therefore occurs for
`distance <= word_14` after the earlier upper-distance test. Actions 13/17
at `0x8003db44..0x8003db4c` compute `angle < 513` and skip writing pitch
512 when true; the write occurs for `angle > 512` under the preceding 3585
bound. The source inequalities now express those raw paths. Focused
similarity reaches 97.5% with the same known CFG and exact 241-row RODATA;
isolated strict `.text` rises to 99.83497% over equal 9356-byte bodies.

Action 27's direct call to `func_8003bd40` derives world X/Z by adding the
actor's signed `+0x24`/`+0x22` offsets to its `+0x08`/`+0x07` cell bytes
shifted by 11. Retail loads each cell byte before its signed offset at
`0x8003ef8c..0x8003ef9c` and adds shift-first at
`0x8003efac`/`0x8003efb8`. Spelling the equivalent C additions in that
order restores both load interleaving and operand order. Focused listing
reaches 97.6%; strict `.text` is 99.931595% over equal 9356-byte bodies,
with 56 unequal raw instruction words. Strict `.rodata` stays 100% byte
identical. All 341 ordered text and 241 ordered RODATA relocation sites,
referents, and masked addends remain equal. The only objdiff replacement is
still the second retail `lhu` of target `+0x0a` versus a probe `nop`; other
residues are argument differences in frame/stack slots and register choices.

The apparent `vector3s_scale_shift12` call move in the focused diff for action
1 is an alignment artifact of the local-stack offsets. Retail passes target
`+0x14` with its first copied direction and then target `+0x18` with the
second; the candidate makes the same calls in the same order. Its local
copies occupy `sp+56`/`sp+64` instead of retail's `sp+48`/`sp+56`, so the
listing aligns the first candidate call with the second retail call. This
does not establish a call-order or referent error and warrants no source edit.

A fresh isolated safe-delink and focused rebuild reconfirm the 99.931595%
strict text and 100% RODATA result. The focused comparison has 410/410 known
blocks, 213/213 branches, and 97.6% listing similarity. Both indirect actor
callbacks have identical raw sites, pointer loads, and delay slots: slot 19
at body `+0x45c` reads `state_8017d118.active_table+0x4c` and passes the
actor in `$a0`; slot 17 at `+0x223c` reads table `+0x44` and passes no
argument. The first real content residue remains retail's second `lhu` at
body `+0x78` versus the probe's `nop`, with subsequent register and local
stack-slot choices. No callback identity, control-flow, or field correction
is supported by this comparison; the source remains unchanged and WIP.

The distinct retail local stack slots do not follow merely from hoisting
the existing case-11 direction vectors and linked-actor position vectors to
function scope. Hoisting both families makes the probe reserve 176 bytes,
with 99.85720% strict text and 41.39004% RODATA; hoisting only the
linked-actor vectors reserves 160 bytes, with 99.85549% text and the same
RODATA score. Retail reserves 144 bytes and the retained probe 136. Both
scope-only controls disturb case addresses and were reverted; the stack
layout remains an unattributed source/compiler residue.
