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
