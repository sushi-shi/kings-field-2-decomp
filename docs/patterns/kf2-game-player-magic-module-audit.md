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

## Fresh 26-claim movement and equipment control

Narrow safe targets and isolated strict objects from current source cover
seven related GAME units, with focused quick builds for each. Twenty-two
claims are strict exact. The fifteen exact `player_state_equipment` siblings
are `player_get_camera_pose`, `player_reset_status`,
`player_initialize_state`, `game_initialize_session`,
`player_clear_motion`, `player_sync_position_to_map`,
`player_distance_to_point_in_cone`, `player_distance_to_point`,
`player_set_unknown_97`, `player_set_unknown_98`,
`player_set_unknown_99`, `player_set_equipment_slot`,
`player_equip_weapon`, `player_begin_weapon_attack`, and `func_80025878`.
`func_80026498`, both `player_weapon_transform_power` claims, both
`player_camera_turn` claims, `player_distance_to_point_with_margin`, and
`player_reset_view` are also exact. The equipment unit's 92-byte DATA and
the magic updater's 52-byte RODATA are exact.

| Remaining claim | Fresh strict text | Focused control and bounded verdict |
| --- | ---: | --- |
| `0x80025a18` equipment dispatcher | 98.02234% | 99/99 CFG, 31/31 branches; 16/16 target probes and 11/11 effect constructors. All 53 switch rows retain 31 destination classes; candidate addends are four bytes below retail. Retail homes/reloads the first variadic argument in a 112-byte frame; current `va_list` C keeps the ABI-correct second-argument cursor in a 104-byte frame. No defined source replacement for that home is proved. |
| `0x8002665c` magic updater | 98.67857% | 114/114 CFG, 68/68 branches, 20 ordered calls. Two missing candidate `player_state` HI16/LO16 pairs are repeat address materializations of the same phase field. |
| `0x8002722c` magic selector | 99.09091% | 33/33 CFG, 16/16 branches; both switch-table class relations and 82 ordered function referents agree. The retail eight-byte leaf frame has no proved live source object. |
| `0x800274ec` horizontal mover | 89.85240% | 35/34 CFG, 20/20 branches, 11 ordered calls. The one retail-only block stores the already modeled slide-attempt flag at the retry join; extra retail player-state pairs rematerialize known fields. |

The 244-byte equipment RODATA remains 39.876034% and the 100-byte selector
RODATA 37.5% while their ordered table destination classes remain supported.
Fresh unit text-relocation totals are 492/492 for equipment, 256/252 for
magic update, and 148/126 for selector plus movement (retail/candidate);
their differences are the already identified repeated `player_state`
HI16/LO16 address pairs. The exact camera-turn caller invokes horizontal
movement at three proven retail sites; the menu-location helper and exact
player action controller reach the selector at three sites in total.
No new exact claim or C/config change resulted from this pass.

## Current player session and movement continuation (2026-10-02)

The older `player_warp_to_floor_entry` `0x80017cf8` lead is not a current KF2
GAME claim: neither that identity nor VA occurs in the current source,
function identities or unit manifest. A fresh isolated manifest-profile
comparison instead covers 29 connected current claims in the player bounds,
distance, reset-view, core-run and state/equipment units. Twenty-eight are
already strict exact:

| Unit | Strict-exact GAME addresses | WIP |
| --- | --- | --- |
| `player_collision_bounds` | `0x80023384` | None |
| `player_distance_margin` | `0x80023430` | None |
| `player_reset_view` | `0x80023484` | None |
| `player_core_run` | `0x80023570`, `0x8002360c`, `0x80023814`, `0x80023868`, `0x80023984`, `0x80024034`, `0x800240cc`, `0x80024164`, `0x80024384`, `0x80024448` | None |
| `player_state_equipment` | `0x80024ed4`, `0x80024f4c`, `0x80025004`, `0x80025184`, `0x800251f0`, `0x80025234`, `0x800252e4`, `0x800253ac`, `0x800253fc`, `0x8002540c`, `0x80025434`, `0x8002545c`, `0x8002569c`, `0x80025754`, `0x80025878` | `0x80025a18` **98.02234%** |

Focused `game.player_state_equipment` confirms all 15 siblings SAME and
`0x80025a18` at 99/99 CFG blocks and 31/31 branches. The already-reviewed
variadic argument home and first successor after a shared probe remain WIP;
the 53 switch rows preserve all 31 destination classes. A second disjoint
player selection/view control adds twelve current claims: strict exact
`0x800247e4`, `0x80026330`, `0x80026464`, `0x80026498`, `0x80028224`,
`0x8002851c`, and `0x80028998`; WIP `0x8002665c` **98.67857%**,
`0x8002722c` **99.09091%**, `0x800274ec` **89.85240%**,
`0x80027f78` **95.91228%**, and `0x8002897c` **88.57143%**. The focused
magic updater retains 114/114 CFG blocks and 68/68 branches with two
`player_state` base rematerialization pairs missing from the probe. The
magic selector has 33/33 blocks and 16/16 branches with an eight-byte retail
leaf frame; horizontal movement has 35/34 blocks and 20/20 branches with a
previously documented redundant zero path. The collision response and
interval helper retain their documented address-lifetime/register residues.
No new source fact or exact closure was found; all C and identity files remain
unchanged.

The fresh `0x800274ec` raw retry-join check locates the extra retail block:
`bltz` at body `+0x2bc` sets the slide-attempt value to one in its delay
slot and targets `+0x350`; loop exhaustion falls through `+0x34c`, which
sets the same value before `+0x350` stores it at stack `+0x40`. The current
candidate has corresponding value assignments at `+0x28c` and `+0x318`,
then joins at `+0x31c` while retaining the flag in a register. The existing
`slide_attempted = 1` source expresses both paths; the extra retail block
does not establish a missing retry, call, or field. A fresh safe two-VA
delink accepted all 321 relocations with none withheld, and the focused
comparison still has 35/34 blocks, 20/20 branches, and eleven ordered calls.
