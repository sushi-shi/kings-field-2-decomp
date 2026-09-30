# GAME target-candidate scorer pilot

`func_80039108` is an unclaimed 0x4c0-byte GAME function at `0x80039108`.
Its only proven direct caller is `actor_select_best_target`. The candidate
signature is `s32 func_80039108(KfTargetCandidate *target, s32 player_distance)`;
the first two arguments and signed result use are supported by that caller and
the body, while the full candidate-record field family remains incomplete.

Retail reads `target->type`, returns zero for `0xff`, then bounds-checks
`type - 2` against `0x82` and indexes words at `0x80011cd8`. The 131 contiguous
raw-word relocation candidates at `0x80011cd8..0x80011ee0` point to ten unique
instruction addresses, all inside this function. The default block at
`0x8003953c` accounts for 114 entries. This strongly suggests one switch table
with 131 rows. It does not promote the individual candidate relocations to
validated or proven ownership. The current `data.tsv` and identity inventories
still split those words into address-derived pointer rows; source reconstruction
should replace them with one unit RODATA claim only after delinker validation.

The body loads `actor_state+0x93ac` as the current actor context and reads
`player_state+0xd8` position fields in several case paths. Direct calls include
`func_800157ac`, `rand`, `vector_xz_to_angle`, `angle_within_tolerance`, and
`func_80015574`. A separate `jalr` at `0x80039564` obtains a function pointer
through `game_graphics_runtime+0xffe4` plus 0x40; its callee remains unresolved.
No strings are referenced. The case paths perform distance checks, angle tests,
random gating, and score selection, with a shared return at `0x800395ac`.

Final verdict: **WIP, unclaimed**. There is no source object or strict score.
The next concrete ownership step is conservative validation of the contiguous
switch table and the callback field, followed by a typed candidate-record
model. No candidate jump or indirect call is treated as a proven target.
