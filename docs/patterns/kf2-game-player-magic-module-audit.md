# GAME player magic two-function module audit

The current `game.player_magic_dispatch` source claims the adjacent GAME
functions `0x80026498` and `0x8002665c` plus their 52-byte switch table.
Both claims must be selected in a safe delink to compare the module: use
`--va 0x80026498 --va 0x8002665c`. A one-function retail carve paired with
the full candidate module gives misleading call and relocation counts.

Fresh focused `kf try --unit game.player_magic_dispatch` keeps
`func_80026498` identical and `func_8002665c` WIP at 114/114 CFG blocks,
68/68 branches, and matching known successors. A direct strict comparison
of the two-function module is **98.850400%** text (3,476 retail versus
3,472 candidate bytes) and **100%** for the 52-byte RODATA table.

Both module objects have 48 `R_MIPS_26` sites and the same external callee
multiplicity, including both calls from the updater into the selector.
Retail has 256 `.rel.text` rows versus 252 candidate rows. The entire
count difference is two extra `player_state` HI16/LO16 address pairs:
retail separately forms the global address for three phase-field accesses
at owner offset `+0x94`, while the candidate forms a saved base once and
uses it for the later loads and store. The source already spells the same
typed field and intervening sound call. No volatile qualifier, artificial
address carrier, or duplicate source access was added to force the extra
relocations. The updater remains WIP; its missing rows are address
materialization residue, not a missing call target or different datum.
