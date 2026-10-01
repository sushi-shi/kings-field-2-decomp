# GAME actor behavior dispatcher: physical case order and phase-3 gap

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
focused listing reaches 68.2%. Its current compiled CFG has 358 blocks and
180 branches versus retail's 410 and 213; this is still far from an exact
match. No previously exact function is owned by this source unit.

An isolated fresh GCC 2.5.7/ASPSX 1.07 compilation, compared strictly with
the current safe-delinked retail module, gives 82.40958% `.text` (retail
9356 bytes, candidate 8552 bytes) and 25.045723% `.rodata`. The source's
main switch contributes all 241 pointer relocations in the first 0x3c4
candidate RODATA bytes, matching the extent and row count of the retail
table. Candidate RODATA has 24 extra bytes: a zero word followed by five
more code-pointer relocations from a nested switch. That extra table is a
source CFG issue; the low RODATA percentage also reflects changed handler
offsets and does not by itself dispute the main table ownership.

The first major source-model gap begins at retail case 3, `0x8003d3e4`.
The raw entry immediately reads actor byte `+0x0f` and signed halfword
`+0x70`; the current C case 3 does not read the latter. Retail clears actor
state `+0x70` on initialization, clears the `0x10000` flag with mask
`0xfffeffff`, and then follows
a state-dependent path through `0x8003d634` before the shared motion tail at
`0x8003dd7c`. Its zero-state path calls
`actor_advance_animation_clamped`,
`actor_animation_crossed_phase` with phase `0x800`, and potentially
`func_800157f8`, `func_800365d8`, and `map_object_spawn_effect`. A later
state path calls the actor callback at `state_8017d118 + 0x0c` indirectly,
then may call `actor_set_lifecycle_and_home_position` or
`actor_set_home_position`. The present C instead models this action as one
phase-crossing damage call. That source-model conflict, not a compiler wall,
explains the large text and CFG divergence here. The callback target remains
indirect; no concrete callee is inferred from the nearby code.

The focused 68.2% is a comparison aid only. Case 3 must be rebuilt from its
raw control flow and checked with isolated strict objdiff before a new match
claim; the 241-row RODATA table and its reviewed referents should remain
intact while doing so.
