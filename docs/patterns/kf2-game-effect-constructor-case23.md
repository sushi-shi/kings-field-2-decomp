# GAME effect constructor: kind 23 argument order

`func_80040308` (`game.effect_constructor`, GAME `0x80040308`) remains WIP.
The retail kind-23 switch entry at body `+0x12a8` calls
`effect_pool_initialize_scaled(record, 8, 0x400)` at `+0x12b0`. Only after
that call does it load the three variadic halfwords at `+0x12cc`, `+0x12d8`,
and `+0x12e4`, storing them at record offsets `+0x40`, `+0x42`, and `+0x44`.
The previous C evaluated those arguments before the initializer. The retained
source moves their reads after the initializer, preserving the typed `u16`
values and all three record stores.

A focused `kf try --unit game.effect_constructor` quick build now reports
40.3% listing similarity (previously 39.8%), with 116/116 CFG blocks,
22/22 branches, and 26/26 known return frontiers. A one-unit isolated strict
objdiff on byte-identical tracked source reports `.text` 89.12569% (previously
88.88609%) and `.rodata` 32.92683% (unchanged). The candidate retains all
157 ordered text and 123 switch-table relocation rows, 21 direct
`effect_play_spatial_sound` sites, and the retail table's 62 target-equivalence
classes across all 123 entries; seven pointer addends are exact. This is a
source-order correction, not an exact match.

Two off-tree controls were rejected. Reading and storing each argument
immediately after the initializer raised strict `.text` to 90.07070%, but
the compiler merged a sound-call site, giving 20 against retail's 21 and
116/117 CFG blocks; `.rodata` fell to 14.939024%. Explicitly sharing a
case-2/kind-23 sound tail also moved the switch table farther from retail.
The retained local form keeps the proven read order and the call/CFG counts.
