# GAME effect constructor: case-local initializer calls

The GAME constructor at `0x80040308` remains WIP. Its switch rows for kinds
121/103 enter body offsets `+0x854`/`+0x860`; kinds 122/104 enter
`+0x894`/`+0x8a0`. Retail prepares `record` as `$a0` in each individual
case, selects render IDs `0x2e`/`0x0f` or `0x2f`/`0x10`, and joins at one
`effect_pool_initialize_scaled` call per pair (`+0x868` and `+0x8a8`).
The delay slots select scale `0x1000`. The previous C selected an ID in a
local and called the initializer after the join. It preserved the result but
let the probe omit the case-local `$a0` setups.

The retained C spells one initializer call in each case and joins afterward
for the shared record writes. The probe tail-merges each call pair into the
same retail call topology. A fresh one-unit safe target has **437 relocations
and none withheld**. Tracked `kf try --unit game.effect_constructor` gives
116/116 CFG blocks, 22/22 branches, and 26/26 known return frontiers.
Isolated strict objdiff moves `.text` from **98.05656%** to **98.43441%**
and `.rodata` from **18.394308%** to **51.016262%**; both text objects are
5,092 bytes. The retained object has all 157 text relocations in the same
ordered type/symbol sequence as retail. Its 123 pointer rows preserve all
62 target-equivalence classes, and exact pointer addends increase from 7
to 65. Both objects keep 69 ordered external calls, including 21 spatial
sound calls. The function is not exact.
The unit has one function, so there is no sibling exact to protect. Its
four-byte initialized `.data` remains strict exact; retail's 2,312-byte
NOBITS `.bss` still appears as tentative COMMON in the candidate object.

The first remaining raw word gap is kind 26 at `+0x304`: retail writes the
final scale halfword, jumps, and repeats the earlier zero-byte store in the
jump delay slot. The probe schedules the final scale store there and does
not repeat the zero store. The repeated store does not change the record,
so it is not a justified source addition. Kind 6 has an equivalent pointer
register path with one fewer probe instruction; kind 102 has a signed
halfword reload versus probe value propagation. These remain unattributed
instruction-selection/scheduling differences, not source corrections.
An isolated unchanged-source GCC 2.6.0 `-O2 -g0` control dropped strict
`.text` to 77.02750% and `.rodata` to 16.056911%; the retained GCC 2.5.7
profile remains the productive probe. This does not establish the historical
compiler version.

## Related pool and spawn call-site control

A fresh safe GAME carve of the constructor and 17 related functions in
`effect_update.c`, `effect_spawn_zero_direction.c`,
`effect_spawn_motion.c`, and `effect_scatter.c` validated 637 relocations
with none withheld. Focused builds reported all 17 related functions SAME,
and isolated strict objdiff confirmed each at 100% `.text`. This includes
the pool scan, both pool record initializers, and all seven spawn/scatter
functions. These are existing exact controls, not new closures. The
constructor stayed at 98.43441% `.text` (5,092 bytes on both sides) and
51.016262% `.rodata`; its four-byte `.data` remained exact.
Another eight neighboring motion/reset helpers in six effect units stayed
strict exact under a fresh 92-relocation safe carve with none withheld.
The constructor's spatial-sound callee at `0x8003fa2c` also stayed strict
exact (60 bytes, two safe relocations).

The next raw text gaps after kind 26 are limited to code placement and
register lifetime: kind 6 retains a retail `addu` copy after its store;
kind 102 reloads a signed halfword before division whereas the candidate
sign-extends its held value; a later call loads two independent words in
the opposite order. Internal jump addends shift with those instruction
counts. The jump-table rows retain their 62 target-equivalence classes,
and all 157 text relocation type/target identities remain ordered. No
new field, call, branch destination, or referent is justified by this
control, so the source remains unchanged.

The three player-side constructor callers were also rebuilt and compared
against a fresh 28-function safe carve (2,257 relocations, none withheld).
`player_core_run.c` kept all ten functions strict exact;
`player_magic_dispatch.c` kept its exact `0x80026498` sibling and
98.67857% `0x8002665c` WIP; `player_state_equipment.c` kept fifteen exact
siblings and its 98.02234% `0x80025a18` WIP. Retail and candidate objects
have the same physical constructor-call multiplicities in these units:
2/2, 1/1, and 11/11 respectively. The call-site evidence supports the
current constructor interface and kind arguments; it does not close the
independent player dispatcher residues.

The remaining nonexact caller `actor_group_effects.c` was separately checked
with a fresh one-function safe target (305 relocations, none withheld):
86.041916% text, 37.906506% switch table, 45/45 CFG blocks, and 15/15
branches. Retail has nine physical constructor calls against ten in the
candidate; its direction helper has eleven against seven. The existing
group-call dossiers establish that the candidate crossjumps several
source-expressed calls and that proposed nine-call spellings changed the
kind-`0x78` variadic arguments. The constructor interface and table rows
remain unchanged on this evidence.

The adjacent `effect_collision_probe.c` supplies a negative control for this
case-local technique. A fresh safe 0x8003fa68 comparison has 24 relocations,
none withheld, and remains 70.73333% strict text (300 retail versus 228
candidate bytes). Retail makes four `func_8002b9d4` calls with modes
`0x31`, `0xa1`, `0xb1`, and `1`; the candidate combines the four correct
source calls into one. Replacing each case's `break` with an explicit jump
to a common function exit in an off-tree source produced a byte-identical
focused listing and the same 14/15-block CFG. This does not justify
changing the case expressions or the callee arguments.
