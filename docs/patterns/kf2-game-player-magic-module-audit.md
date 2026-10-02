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

## Fresh player caller and sibling controls

A manifest-profile isolated strict pass on six GAME units covered 25 functions:
20 existing exact controls and five WIPs. The 15 functions before
`func_80025a18` in `game.player_state_equipment`, both weapon transform/power
functions, `func_80026498`, `func_80023384`, and `func_80028998` are all
100%. None is a newly bankable exact result.

| WIP function | Strict text | Raw-backed verdict |
| --- | ---: | --- |
| `0x80025a18` equipment dispatcher | 98.02234% | 99/99 CFG blocks, 31/31 branches, 16/16 probe and 11/11 constructor call sites; 108 ordered text relocation type/target pairs and all 53 switch rows in 31 destination classes agree. The candidate table addends are each four bytes below retail. Retail homes and reloads `effect_id` in a 112-byte frame; the candidate keeps `a0` in a 104-byte frame. The first differing successor is after the early shared probe call. No extra live source object is established. |
| `0x8002665c` magic updater | 98.67857% | 114/114 CFG blocks, 68/68 branches, 20 ordered calls, and exact 52-byte switch table. The full module has 256 retail versus 252 candidate text relocation rows; two missing `player_state` HI16/LO16 pairs are repeated base materializations, with no missing datum or call. |
| `0x8002722c` magic selector | 99.09091% | 33/33 CFG blocks and 16/16 branches; its remaining difference is already bounded by the player trajectory dossier. |
| `0x800274ec` horizontal move | 89.85240% | 35/34 CFG blocks and 20/20 branches. Eleven calls retain their order; the remaining player-state address pairs and one failure-path block lack a supported source correction. |
| `0x8002897c` interval predicate | 88.57143% | 3/3 CFG blocks and 1/1 branch; the seven differing words are a Boolean-result register choice, with its sibling exact. |

As a control on the equipment dispatcher's first switch arms, an off-tree
source spelling let case 52 fall through directly into case 4's shared body
instead of using `goto simple_effect`. The focused compiler emitted a
SHA256-identical object, leaving strict text at 98.02234% and all 15
equipment siblings exact. That spelling does not recover the entry home,
frame, or table addends and was not retained. Indexing from `&effect_id`
past the named formal was also left off-tree: its score gain depends on an
undefined C traversal with no evidence that retail used it.

Four further caller/control units were focused and compared as complete
manifest-profile objects. Across their 31 functions, 28 are existing exact
controls: ten in `game.player_core_run`, sixteen in `game.player_reaction`,
and two landing helpers in `game.player_collision_sound`. The other three
remain WIP: `0x800279cc` at **98.23967%** with 70/70 CFG blocks and 37/37
branches, `0x80027f78` at **95.91228%** with 27/27 blocks and 14/14 branches,
and `0x8002985c` at **99.26569%** with sixteen exact siblings. The collision
sound difference begins with a 72- versus 64-byte frame and saved-register
allocation; the response has redundant candidate `player_state` address
materializations; the reaction has clamp and loop-register scheduling.
Their source-backed field and store-order corrections are already retained
in the player trajectory dossier. This extension brings the current screen
to 56 functions, 48 existing exact controls, eight WIPs, and no new C edit or
exact closure.
