# GAME player and actor trajectory campaign

This GAME.EXE batch follows confirmed calls from player damage and actor motion
into the trajectory solver and collision dispatcher. Exact means a strict
`kf sema --image game match` result of `100.000000000%`; focused `kf try`
listing identity is stated separately. The `0x801d8d40..68` collision-cache
bytes still overlap the provisional end of the equipment record view, so no
new overlapping global was defined.

The earlier tables and progress notes below are chronological snapshots.
The final twenty-function player call-graph table gives the current strict
verdicts for that subset.

| GAME VA | Verdict | Evidence or remaining question |
| --- | --- | --- |
| `800158b4` | WIP, existing source | Focused 72.0% local with matching 3/3 CFG blocks; only the loop counter and start-pointer register assignment differs. The source retains the signed 16-bit interpolation behavior. |
| `80015918` | WIP, source | 0x2b0-byte solver now has the fixed-point discriminant, square roots and output angle in C; strict report is 93.0814%, and focused probe has 41/41 CFG blocks. Root selection and instruction ordering remain unresolved. |
| `80015bc8` | exact, new | Twelve O32 arguments; solver `80015918`, fixed 2D length, `rcos` and `rsin`; full-word and halfword result views. |
| `80023384` | WIP, source | Collision-cache words set player vertical margins; focused code/CFG differ, cache extent remains provisional. |
| `8002360c` | exact, new | Six-argument resource transition called by `80029014` and `8002985c`; waits for CD, draw and video synchronization before restoring player and map state. The neighboring audio resource controller remains address-derived. |
| `80024498` | WIP, unclaimed | 0x34c-byte player damage reaction with a jump table and player-death path. |
| `800248a8` | WIP, unclaimed | 0x3fc-byte damage dispatcher with a jump table and eight damage-component calls. |
| `80026498` | WIP, unclaimed | Weapon action dispatcher called twice by `8002665c`; indirect jump through `DAT_80011260` and `DAT_800667e8` table referents need curation before source. |
| `80027928` | exact, new | Collision-depth check calls `player_death_begin` and sets a player status byte. |
| `80029014` | exact, new | Player/map-object reaction dispatcher called by `8002985c`; two 32-bit flag masks at player +0x140, callback byte at `state_8017d118+4`, and twelve reviewed direct control edges. |
| `80029168` | exact, existing | Clears player reaction, movement and rotation increments. |
| `800291d0` | exact, existing | Starts reaction state 1 with a byte mode. |
| `800291ec` | exact, new | Map-object reaction indexes 0x44-byte objects, accumulates camera angles and writes either rotation or position reaction. Strict report is 100%; pointer/byte/index views at map-object +0x40 are modeled as a union, with pointed-record extent still WIP. |
| `800293d4` | exact, existing | Starts camera reaction state 2 and copies the target rotation. |
| `80029428` | exact, existing | Starts damage reaction state 3 and copies rotation. |
| `80029464` | exact, existing | Player damage reaction state setter. |
| `800294f8` | exact, existing | Adjacent player damage reaction state setter. |
| `80029570` | exact, existing | Begins player death once, plays its sound and records rotation if provided. |
| `800295f8` | exact, existing | Advances idle player to reaction state 4. |
| `80029624` | WIP, existing source | Adjacent phase/secondary-counter step remains 62.44898% strict (57.4% local) with 20/20 blocks and a return-frontier/branch lifetime residue; a simpler nested zero test did not alter codegen. |
| `800296e8` | exact, existing | Applies signed HP delta, clamps and begins death at zero. |
| `8002975c` | exact, existing | Applies signed MP delta and clamps to the maximum. |
| `800297b4` | exact, existing | Applies equipment HP regeneration and drain intervals. |
| `80038d04` | exact, new | Actor home placement; signed local offsets and terrain height query. |
| `80038f20` | WIP, unclaimed | Actor scan has unresolved indirect callback. |
| `8003a614` | WIP, source | Actor-to-player distance and yaw tests call player damage; codegen residue after matched CFG/calls. |
| `8003a778` | WIP, unclaimed | 0x27c-byte actor distance and random-choice path. |
| `8003a9f4` | WIP, source | 89.04444% strict: type-3 filter, 200-actor scan and five-argument distance ABI recovered; CFG branch fold remains. KF1 `actor_pool_find_overlap` supports the scan shape. |
| `8003ab5c` | WIP, source | 88.53488% strict: sibling probe omits the type-3 filter; same branch/lifetime residue. |
| `8003acb4` | exact, existing | Actor bind-current helper; now contiguous in the animation unit. |
| `8003ad90` | exact, existing | Animation phase wrap. |
| `8003adc4` | exact, existing | Animation phase clamp. |
| `8003ae20` | exact, existing | Animation phase crossing test. |
| `8003ae50` | WIP, unclaimed | 0x4ec-byte actor motion application with collision, trigonometry and looped retries. |
| `8003b33c` | exact, new | Four ordered collision-dispatcher calls; actor-state mode at `+0x93a4`. |
| `8003b520` | exact, new | Trajectory setup passes twelve arguments to `80015bc8`; stack parameter remains 32-bit until halfword store. |
| `8003b5bc` | exact, existing | Adjacent actor status byte setter. |
| `8003b5d0` | WIP, unclaimed | 0x3d4-byte collision/damage path; output of `8002b9d4` and player-damage calls need a typed model. |
| `8003b9a4` | exact, new | Actor motion decay through `SquareRoot0` and signed halfword approach. |
| `8003bae4` | exact, new | Direction scaling and signed X/Z motion approach. |
| `8003bba0` | exact, existing | Actor yaw approach. |
| `8003bcd0` | exact, existing | Yaw/motion wrapper. |
| `8003bd40` | WIP, source | CFG, calls and referents agree; remaining register, load and delay-slot schedule differs. A direct Manhattan-distance accumulator experiment scored lower and was reverted. |
| `8003be38` | exact, new | Three-axis motion and collision query. |
| `8003bf74` | exact, existing | Three-axis yaw/motion wrapper. |
| `8003c000` | exact, existing | Actor group vertex world position. |
| `8003c10c` | exact, existing | Actor group position helper. |
| `8003c220` | exact, existing | Actor animation-state step. |

The contiguous `8003b9a4..8003c000` run now has one actor-motion unit: six
of seven listings are identical, with only `8003bd40` WIP. The contiguous
`8003b33c..8003b5d0` run is one three-function exact unit. Binding the current
actor and the three animation helpers also form the exact tail of one
six-function unit; the two preceding distance probes remain WIP. These
consolidations remove Psy-Q modules without changing the exact neighbors.
The `80015918..80015ce0` trajectory math run likewise owns the WIP solver
and two identical-listing helpers in one unit; the earlier fixed-interpolation
run remains separate because its `800158b4` source is still WIP.
The contiguous `80028ec0..8002985c` player reaction run is now one unit with
15 identical listings; only `80029624` remains WIP. It replaced nine
same-profile units, reducing the Psy-Q module count by eight.

## Connected camera, movement, equipment, and damage batch

The next pass follows the main player update's direct call graph through camera
turning, horizontal movement, input actions, weapon effects, and damage. The
`80028224..8002897c` camera/movement pair has one shared source; its second
function is byte-exact, and the first retains one instruction-selection
residue. Unclaimed rows below have no source object and therefore no strict
percentage; their call/data evidence is a reconstruction boundary, not a
match claim.

| GAME VA | Verdict | Evidence or remaining question |
| --- | --- | --- |
| `80024498` | WIP, unclaimed | Damage reaction calls death and reaction setters; the validated `DAT_80011128` indirect jump still needs switch-table ownership. |
| `800248a8` | WIP, unclaimed | Damage dispatcher makes eight `player_calculate_damage_component` calls and uses validated switch table `DAT_80011148`. |
| `80024ca4` | exact, new | Sixteen-argument radial damage wrapper chooses 3D distance or `80015698`, then scales before `800248a8`. Exact caller `8003ff18` constrains the ABI. |
| `80025a18` | WIP, unclaimed | Weapon/magic effect dispatcher repeatedly calls `80025878` and the effect constructor; validated `DAT_80011188` indirect jump remains. |
| `80026330` | exact, preserved | Weapon transform still strict 100% after switching to the shared `80034344` declaration. |
| `80026464` | exact, preserved | Power/magic threshold helper still strict 100%. |
| `80026498` | WIP, unclaimed | Weapon action switch through validated `DAT_80011260`; `DAT_800667e8` payload and caller return paths need typing. |
| `8002665c` | WIP, unclaimed | Weapon attack/effect update calls `80026498`, vertex query, rotation and collision probes. |
| `8002722c` | WIP, unclaimed | Action selector has two unresolved indirect jumps through `DAT_80011298` and `DAT_800112b0`. |
| `800274ec` | WIP, unclaimed | Horizontal movement called three times by `8002851c`; collision dispatcher and trigonometric turn adjustment remain the source boundary. KF1 `player_move_horizontal` corroborates its heading/distance ABI. |
| `80027928` | exact, preserved | Collision-depth death check remains strict 100%. |
| `80027988` | exact, preserved | Signed impact sound helper remains strict 100%. |
| `800279cc` | WIP, unclaimed | Main collision branch calls `8002b604`, `8002b9d4`, damage reaction and death checks; cache extent is provisional. |
| `80027f78` | WIP, unclaimed | Sibling collision branch calls death, fixed X/Z length and collision bounds; cache extent is provisional. |
| `80028224` | exact, source | Strict 100% after the shared view-angle array was typed signed; the 32/32 CFG blocks and all referents agree. The lower pitch clamp now emits retail `addiu v0,zero,-700`. |
| `8002851c` | exact, new | Strict 100% across 0x460 bytes. KF1 movement block establishes signed 16-bit forward/strafe/magnitude locals; GAME uses signed yaw at +0xfa and zero-forward movement branch. Forty reviewed player-state address pairs and 17 transfers. |
| `8002897c` | WIP, existing source | Inclusive 71..80 predicate remains 88.57143% strict. Semantically equivalent early-return trial changed CFG and was reverted. |
| `80028998` | WIP, unclaimed | Player input/weapon/magic update is a void no-argument caller of `8002722c`, `80025a18`, and attack helpers. The `DAT_800667e8+0xc` table endpoint is still candidate. |
| `80028ec0` | exact, preserved | Three-axis reaction angle step remains strict 100%. |
| `80028fa8` | exact, preserved | Transition pool release remains strict 100%. |
| `80029014` | exact, preserved | Player reaction dispatcher remains strict 100% after the header refinement. |
| `80029168` | exact, preserved | Clears motion and reaction state; strict 100%. |
| `800291d0` | exact, preserved | Starts reaction state 1; strict 100%. |
| `800291ec` | exact, preserved | Map-object position/rotation reaction remains strict 100%. |

`KfPlayerState+0x144` is now a signed word movement limit; +0x148 is the
signed turn limit. The retail movement code's halfword stores also refine
+0xe8..+0xef into four halfword fields without changing the state extent.
`kf inventory check` passes. The shared GAME target relink verifies 155/155
units, while global edge-check still reports two map-object RODATA addend
differences and incomplete known-reference data ownership.

## Contiguous player core and equipment units

Direct calls between equipment setters, stat recalculation, experience growth,
initialization, camera pose and attack setup support two contiguous player
source runs. All 25 claims below are `100.000000000%` in the fresh strict
objdiff report after consolidation. `game.player_core_run` owns
`80023570..80024498`; `game.player_state_equipment` owns
`80024ed4..80025a18`. The latter carries the original equipment switch
`RODATA(0x80011168, 0x1c)` and session weapon-buffer
`DATA(0x80075da0, 0xc000)` claims. Both data sections match retail.

| GAME VA | Verdict | Recovered role |
| --- | --- | --- |
| `80023570` | exact | Restore equipment, weapon, view and status effects. |
| `8002360c` | exact | Stage two CD/audio transitions and restore map state. |
| `80023814` | exact | Scale a player value by rank with a 5000 cap. |
| `80023868` | exact | Add nine equipment bonus components. |
| `80023984` | exact | Recalculate attack, defense and magic from base stats and equipment. |
| `80024034` | exact | Accumulate physical-power training and recalculate at threshold. |
| `800240cc` | exact | Accumulate magic training and recalculate at threshold. |
| `80024164` | exact | Add experience and apply level-growth rows. |
| `80024384` | exact | Calculate one damage component with the player damage scale. |
| `80024448` | exact | Apply signed HP delta and begin death at zero. |
| `80024ed4` | exact | Copy camera position and angles with player height offsets. |
| `80024f4c` | exact | Clear status fields and restore current HP/MP. |
| `80025004` | exact | Initialize player growth, equipment and camera state. |
| `80025184` | exact | Bind the session weapon asset buffer and enable audio settings. |
| `800251f0` | exact | Clear player movement state. |
| `80025234` | exact | Synchronize player position and height to the map. |
| `800252e4` | exact | Test distance and facing cone to a point. |
| `800253ac` | exact | Test distance to a point with height margin. |
| `800253fc` | exact | Set player-state byte +0x97. |
| `8002540c` | exact | Set byte +0x98 and clear the paired byte when active. |
| `80025434` | exact | Set byte +0x99 and clear the paired byte when active. |
| `8002545c` | exact | Bind one of seven equipment slots and recalculate stats. |
| `8002569c` | exact | Equip a weapon and load its asset. |
| `80025754` | exact | Begin a normal or alternate weapon attack. |
| `80025878` | exact | Find an actor target and optionally return rotated origin and direction. |

The intervening `80024498` player damage reaction and `800248a8` damage
dispatcher remain unclaimed WIP functions with validated switch-table
boundaries. The later `80024ca4` radial damage wrapper is now exact. The
`800247e4` status-cap function is exact in its own source. These gaps prevent
joining the two runs merely by address proximity. The next function,
`80025a18`, is a 0x918-byte unclaimed weapon/effect dispatcher with a
validated indirect jump through `DAT_80011188` and repeated calls to
`80025878`; its switch and data ownership remain WIP. Consolidation replaces
16 former units with two, reducing the Psy-Q module count by 14 while
preserving all strict listings. The global edge-check still has two
map-object RODATA addend divergences and incomplete known-reference closure;
these do not change the player unit verdicts.
After this reduction, a full pinned `kf build` no longer reports Psy-Q's
module-count error. PSX links; GAME, OPEN and END still fail on unresolved
symbols from incomplete program-wide linkage (`InitCARD`, `malloc` and
`display_buffers` are their respective first reported errors).

## Ten-function weapon action and collision batch

This batch follows the weapon update's direct calls through the action
selector, horizontal movement, and collision response. The four source-owned
functions are strict `100.000000000%` after merging each contiguous pair;
the six unclaimed functions have no objdiff percentage and remain WIP.

| GAME VA | Verdict | Evidence or boundary |
| --- | --- | --- |
| `80026330` | exact | Weapon animation offset transformed into world space; contiguous with the power test. |
| `80026464` | exact | Player physical power and magic must both be at least 60; the pair is one `game.player_weapon_transform_power` unit. |
| `80026498` | WIP, unclaimed | A 13-entry indirect jump at `DAT_80011260` selects weapon/magic effects; the separate 13-byte table at `800667e8` has candidate referent and unresolved owner. Two proven calls come from `8002665c`. |
| `8002665c` | WIP, unclaimed | 0xbd0-byte weapon update calls `80026498` twice and also calls the effect constructor, vertex query, rotation math and the player power test. Its caller in `8002985c` remains candidate. |
| `8002722c` | WIP, unclaimed | Input/action selector called twice by `80028998`, with two unresolved indirect jumps through `DAT_80011298` and `DAT_800112b0`. |
| `800274ec` | WIP, unclaimed | Horizontal movement has three proven calls from exact `8002851c`; it queries collision `8002b9d4` and adjusts heading with trigonometric functions. |
| `80027928` | exact | Collision depth can begin player death; cache +0x11810 remains a provisional interior view. |
| `80027988` | exact | Impact magnitude maps to sound 12 volume; contiguous with `80027928` in `game.player_collision_sound`. |
| `800279cc` | WIP, unclaimed | Main collision branch calls both exact helpers, damage reaction and collision probes; the +0x11800 cache/equipment extent remains unresolved. |
| `80027f78` | WIP, unclaimed | Sibling collision branch calls death, fixed X/Z length, vertical bounds and collision probes; it shares the provisional cache. |

The two pairwise consolidations save two more Psy-Q modules. No switch table,
cache tail or separate global was claimed from candidate-only evidence.

## Ten-function camera, input, and reaction batch

The camera/movement calls from the player update lead into the input
controller and then the reaction dispatcher. The ten verdicts below come from
the refreshed strict report. Seven are `100.000000000%` exact; the other three
retain explicit source or ownership residues.

| GAME VA | Verdict | Evidence or residue |
| --- | --- | --- |
| `80028224` | exact, source, 100% | Camera yaw/pitch update matches after the shared view-angle array was typed signed, including retail's `addiu -700` lower clamp. |
| `8002851c` | exact | Forward/strafe movement and normalization remains exact in the same camera unit. |
| `8002897c` | WIP, source, 88.57143% | Inclusive 71..80 predicate has the same 3-block CFG; remaining difference is `v0`/`v1` lifetime and return move. A direct early return changed CFG and was not kept. |
| `80028998` | WIP, unclaimed | Input controller calls the action selector twice, weapon/effect dispatcher, power test and attack starter; two reads of `DAT_800667e8+0xc` remain candidate data referents. |
| `80028ec0` | exact | Steps three signed reaction angular velocities and accumulates the offsets. |
| `80028fa8` | exact | Temporarily clears two player option bytes around map/pool cleanup. |
| `80029014` | exact | Dispatches motion-flag and resource reactions from two full-word masks. |
| `80029168` | exact | Clears reaction and movement state. |
| `800291d0` | exact | Enters reaction state 1 with a byte mode. |
| `800291ec` | exact | Applies a map-object position or rotation reaction. |

`80028ec0` and `80028fa8` were prepended to the contiguous
`game.player_reaction` source, removing two additional Psy-Q modules. The
expanded unit has 15 of 16 identical focused listings; its existing
`80029624` remains WIP with the same CFG residue. Fresh strict scores confirm
all seven exact claims in this batch after the merge.

## Ten-function reaction and damage batch

This batch audits the next ten contiguous functions in the same reaction
unit. Nine are strict `100.000000000%`; the phase counter remains WIP at
`62.448980000%`. The exact claims stayed unchanged after the preceding unit
merge.

| GAME VA | Verdict | Evidence or residue |
| --- | --- | --- |
| `800293d4` | exact | Enters reaction state 2 and copies camera rotation. |
| `80029428` | exact | Enters reaction state 3 with supplied rotation. |
| `80029464` | exact | Copies reaction rotation/motion and starts state 0x10. |
| `800294f8` | exact | Sibling rotation/motion setter starts state 0x12. |
| `80029570` | exact | Starts player death once, plays sound and accepts optional rotation. |
| `800295f8` | exact | Starts state 4 from idle and clears X rotation. |
| `80029624` | WIP, source, 62.44898% | Advances two halfword phase counters and scales the result; CFG branch lifetime and return frontier differ despite correct behavior, calls and referents. |
| `800296e8` | exact | Applies signed HP change with clamp and death transition. |
| `8002975c` | exact | Applies signed MP change with clamp. |
| `800297b4` | exact | Applies equipment HP recovery and drain intervals. |

Retail `80029624` keeps the zero-phase sentinel at the tail and a shared
return. A reversible positive-path source trial preserved behavior and all
neighboring exact listings but reduced its focused CFG match to 19 versus 20
blocks, so the repository source remains unchanged. The current source still
has 20/20 blocks and no unsupported control-flow or data claim was added.

## Ten-function equipment-to-weapon effect batch

This batch follows the equipped weapon through attack setup, target selection,
and the magic/effect path. The seven source-owned functions remain strict
`100.000000000%`; the three larger dispatchers remain unclaimed WIP, with no
objdiff score. Their switch/data ownership is not yet proved.

| GAME VA | Verdict | Evidence or boundary |
| --- | --- | --- |
| `80025234` | exact | Player position and map height synchronization in the 15-function equipment unit. |
| `8002545c` | exact | Seven equipment slots bind 32-byte records and recalculate combat stats. |
| `8002569c` | exact | Weapon equip loads the asset and resets attack state. |
| `80025754` | exact | Normal or alternate weapon attack startup. |
| `80025878` | exact | Actor target selection and optional rotated origin/direction outputs; repeatedly called by `80025a18`. |
| `80025a18` | WIP, unclaimed | 0x918-byte effect dispatcher uses an indirect jump through `DAT_80011188`, calls the target helper, and is called by both `80026498` and the input controller. Its effect table owner remains candidate. |
| `80026330` | exact | Weapon animation offset transformed into a world position. |
| `80026464` | exact | Physical power and magic both meet the 60 threshold. |
| `80026498` | WIP, unclaimed | 0x1c4-byte selector indexes 26-byte magic rows, checks `mp_cost` at +0x16, optionally spends MP, and dispatches through 13 entries at `DAT_80011260`. The separate 13-byte effect-ID table at `800667e8` is still candidate-only. |
| `8002665c` | WIP, unclaimed | 0xbd0-byte weapon update calls `80026498` twice; both sites pass a byte magic ID, a boolean charge/spend condition, and an integer effect parameter. Its broader effect/collision call set needs source reconstruction. |

`80026498` stores the selected `KfMagicRecord *` at player-state +0x7c after
the unsigned MP check; initialization clears the same word. The player header
now types it as `selected_magic_record`, as in the KF1 source, with a checked
offset. Both direct caller sites support `u8, s32, s32` arguments and neither
uses a return value; the function identity's old pointer-argument seed was
corrected without claiming a body. The existing 15-function equipment unit
remains listing-identical after this header refinement. No table or interior
global was claimed from candidate-only relocation evidence.

## Ten-function damage, movement, and reaction WIP batch

Only functions that were non-exact at selection count in this batch. Exact
neighbors serve as regression controls. The contiguous damage call graph
yielded one newly strict function, while the two upstream damage switches and
the movement/reaction functions retain their stated source or data boundaries.

| GAME VA | Final verdict | Evidence or residue |
| --- | --- | --- |
| `80024498` | WIP, unclaimed | 0x34c-byte HP/death and motion reaction; direct callers pass an optional VECTOR origin, signed damage, and halfword flags. The eight-entry `DAT_80011128` switch and its case ownership remain unclaimed. |
| `800248a8` | WIP, unclaimed | 0x3fc-byte damage/status dispatcher; eight proven component calls, a seven-entry `DAT_80011148` switch, and exact actor/effect callers prove twelve arguments. Its switch and status field model are incomplete. |
| `80024ca4` | **exact, 100%** | 0x230-byte radial damage wrapper with sixteen caller-proven arguments; selects 3D or player-relative distance, computes fixed12 attenuation, and forwards nine damage/status values to `800248a8`. |
| `8002722c` | WIP, unclaimed | 0x2c0-byte input action selector has two unresolved indirect jumps through `DAT_80011298` and `DAT_800112b0`. |
| `800274ec` | WIP, unclaimed | 0x43c-byte horizontal movement adjusts heading with sine/cosine and queries collision `8002b9d4` twice; its collision result fields remain provisional. |
| `80028224` | exact, source, 100% | Camera yaw/pitch CFG, calls, and referents match. Typing the shared view-angle array signed makes the lower-pitch constant emit retail `addiu -700`. |
| `8002897c` | WIP, source, 88.57143% | Inclusive 71..80 predicate has the retail 3/3 CFG; remaining `v0`/`v1` lifetime differs. An early-return trial made 4 blocks and was reverted. |
| `80028998` | WIP, unclaimed | 0x528-byte input/weapon update calls the action selector, effect dispatcher, power test, and attack starter. `DAT_800667e8+0xc` remains a candidate effect-ID referent. |
| `80029624` | WIP, source, 62.44898% | Phase counter helper retains correct signed halfword behavior, calls, 20/20 CFG blocks, and exact neighbors; the zero sentinel and shared-return frontier still differ. |
| `8002985c` | WIP, unclaimed | Large player update has two curated fragments in a 0x112c-byte range; unresolved fragment/control and data ownership prevent a complete source claim. |

The new `game.player_radial_damage` source exactly matches all 560 retail
bytes. Two player-state HI16/LO16 pairs, three direct calls, and two internal
jumps were decoded and curated before its target refresh. The focused result
was `1/1 SAME`; a fresh strict report gave `100.000000000%` and verified
138/138 GAME target unit relinks. The report still exits on the known three
other RODATA addend mismatches and incomplete global reference closure.

## Ten-function trajectory, magic, and collision boundary batch

These ten were non-exact or unclaimed at selection. No new function reached
strict `100%` in this batch. The fixed-point trajectory source gained a branch
shape supported by retail control flow, while the other functions retain their
source or ownership boundaries.

| GAME VA | Final verdict | Evidence or residue |
| --- | --- | --- |
| `800158b4` | WIP, source | Fixed interpolation has matching 3/3 CFG blocks; pointer/counter register assignment and load schedule differ. The adjacent `1584c` and `1586c` listings remain exact. |
| `80015918` | WIP, source, 95.75581% strict | Ballistic solver has two `SquareRoot0` calls and matching 41/41 CFG blocks and 24/24 branch counts. A retail-supported explicit time-selection branch improved the strict score from 93.0814%; arithmetic register lifetime and branch successors still diverge. Adjacent `15bc8` and `15ce0` remain exact. |
| `80023384` | WIP, source | Collision vertical-bounds helper has matching 5/5 CFG blocks. Probe CSE combines player-height loads that retail keeps separate; the cache words at BSS +0x11818/+0x1181c still overlap the provisional equipment tail. |
| `80025a18` | WIP, unclaimed | The 0x918-byte effect dispatcher repeatedly calls `25878` and uses validated indirect table `DAT_80011188`; switch-data ownership remains unresolved. |
| `80026498` | WIP, unclaimed | Magic selector uses 26-byte records, halfword `mp_cost`, and 13-entry indirect table `DAT_80011260`. Its separate effect-ID array at `800667e8` is now classified as data, but no source owner is established. |
| `8002665c` | WIP, unclaimed | The 0xbd0-byte weapon updater calls `26498` twice and mixes animation, rotation, sound, and collision paths; its full call/data ownership is not yet reconstructed. |
| `800279cc` | WIP, unclaimed | The 0x5ac-byte collision branch calls `2b604`, `27928`, `2b9d4`, `27988`, `24498`, and `23384`; collision-cache extent remains provisional. |
| `80027f78` | WIP, unclaimed | The 0x2ac-byte sibling collision branch calls `2b9d4` three times, then death/distance/bounds helpers; cache layout remains provisional. |
| `8002aaa4` | WIP, unclaimed | The 0xb60-byte cell lookup takes five caller-supported arguments and has an indirect jump through `DAT_8001134c`; switch owner and complete semantics are unresolved. |
| `8003a9f4` | WIP, source | Actor collision probe receives five O32 arguments and returns a hit index or `-1`. Focused comparison is 63.2% with 13 retail versus 12 compiled CFG blocks; fifth-argument lifetime and branch folding differ. |

Retail `26498` indexes the 13 bytes at `800667e8`, while `28998` directly
loads its final byte. Their contents are effect IDs, not an ASCII literal.
The address-derived `DAT_800667e8` identity has no claimed source owner;
three HI16/LO16 pairs were reviewed against the direct `lbu` sites. Retail
census and inventory validation pass. A full pinned `kf build` builds PSX.EXE and
clears the module cap; GAME/OPEN/END still stop at the existing unresolved
`InitCARD`, `malloc`, and `display_buffers` symbols respectively.

## Ten-function actor-to-player target and trajectory batch

These ten GAME functions were non-exact or unclaimed when selected. All remain
WIP. The actor damage wrapper has one new retail-supported type correction;
the remaining source residues and unclaimed data/control boundaries are listed
individually. Exact actor-animation and actor-motion neighbors were preserved.

| GAME VA | Final verdict | Evidence or residue |
| --- | --- | --- |
| `80039108` | WIP, unclaimed | Candidate scorer is called by exact `actor_select_best_target`; it uses the validated `DAT_80011cd8` indirect jump, one unresolved indirect call, angle tests, random selection, and six distance checks. The switch and callback targets remain unproved. |
| `8003983c` | WIP, strict `99.075380000%` fuzzy | Typed actor lifecycle/target update is called by the exact frame scan `8003f610`. It checks current actor and group, player camera X/Z, random activation, overlap `3a9f4`, then target selection or home placement. Focused CFG now aligns at 37/37 blocks, 24/24 branches, and 13/13 return frontiers; the player-state +0x10a byte meaning and residual saved-register choices remain open. |
| `80039c94` | WIP, unclaimed | Actor hit/damage dispatcher is called by weapon update, actor radial damage, and effect dispatch. It calls the fixed curve helper eight times, increments player magic/physical training and XP, selects targets, and has an unresolved indirect callback. The 0x684-byte body lacks a complete callback/data owner. |
| `8003a318` | WIP, source, 73.59162% strict | Radial actor damage takes sixteen caller-supported arguments, scans 200 actors, and forwards a scaled damage record to `39c94`. Retail uses logical right shifts on masked attenuation products; typing their arithmetic unsigned raised strict score from 72.98953%. CFG is 26 versus 25 blocks. Adjacent `3a778` remains exact. |
| `8003ab5c` | WIP, source, 88.53488% strict | The 200-actor collision scan matches the call set and width tests; focused CFG is 12 versus 11 blocks. A KF1-backed signed-16 loop-index trial added truncation absent from retail, so it was reverted. Four adjacent animation functions remain exact. |
| `8003ae50` | WIP, unclaimed | Three exact actor-motion helpers call this 0x4ec-byte motion/collision routine. It probes `2b9d4` and `2b7f8`, rotates motion with sine/cosine, and takes square roots; its mutation and retry paths still need a typed source model. |
| `8003b5d0` | WIP, unclaimed | The large actor collision/damage branch is called by `3d184` and calls `2b604`, `2b9d4`, and the twelve-argument player damage dispatcher. The shared collision-cache extent remains provisional. |
| `8003bd40` | WIP, source, 86.53226% strict | Actor X/Z steering has the retail 9/9 CFG blocks, 5/5 branches, calls, and referents. Register allocation, load ordering, and delay-slot schedule differ; six other functions in the actor-motion unit remain exact. |
| `8003c614` | WIP, unclaimed | The 0xa70-byte actor trajectory/effect dispatcher is called three times by `3d184`, calls group-position helpers (`3c000` exact, `3c3e0` WIP) and the WIP trajectory solver `15918`, and has an unresolved indirect jump. Its case table/source owner remains unknown. |
| `8003d184` | WIP, unclaimed | The 0x248c-byte actor update dispatcher is called by the exact frame scan and calls `3c614`, `3bd40`, `3b5d0`, player damage, collision, audio, and animation helpers. It jumps through validated `DAT_800120d8` and makes further indirect calls; its complete control/data ownership is unproved. |

The signed loop-index trial for `3ab5c` compiled to extra truncation and was
reverted, preserving the prior 88.53488% strict result. The only retained C
change is the unsigned fixed-point attenuation in `3a318`, matching the
retail `srl` instructions. Fresh GAME matching relinked 141/141 target units;
the global check still reports the three existing, unrelated RODATA addends
and incomplete known-reference closure.

## Ten-function actor and player-state event batch

These ten GAME functions were non-exact or unclaimed at selection. None reached
strict `100%`. Source edits that only reshaped associative arithmetic or
commutative pointer addition were reverted after focused comparison; the
underlying retail semantics remain represented by the prior C.

| GAME VA | Final verdict | Evidence or residue |
| --- | --- | --- |
| `8003a614` | WIP, source, 96.91011% strict | Actor facing/range test calls player damage with eight inputs; 6/6 CFG blocks agree. Retail materializes a second player camera address separately, while the probe reuses its earlier base; adjacent `3a778` remains exact. |
| `8003c3e0` | WIP, source, 96.36170% strict | Actor trajectory stepper has all seven direct geometry calls and 23/23 CFG blocks. Stack extent, pitch-argument lifetime, arithmetic operands, and register assignment differ; the three preceding group-position helpers remain exact. |
| `8003d084` | WIP, source, 91.60000% strict | Signed actor byte +0x4b clamps a sound-note offset before `rand()`. The 5/5 CFG and sole call match; the final `-2` and jitter association schedules differently. Adjacent spatial-sound caller remains exact. |
| `8003f7ec` | WIP, source, 85.86207% strict | Converts up to forty groups of sixteen relative target offsets to pointers, with `-1` becoming null. Its 9/9 CFG and owner referent match; immediate order and one commutative `addu` operand order differ. Neighboring frame scan and actor-load helper remain exact. |
| `800460a0` | WIP, source, 99.268295% strict | Advances actor animation in even phase steps and calls the frame helper twice. All six blocks, two branches, calls, constants, and referents match; the probe assigns step and half-step to opposite saved registers. |
| `800462bc` | WIP, unclaimed | Event-command dispatcher has a candidate 16-entry indirect table at `DAT_80012890` and an unresolved callback through a record field; calls `460a0` three times and several exact menu/player helpers. Table and callback ownership remain unproved. |
| `8004678c` | WIP, unclaimed | Scene controller has an indirect table at `DAT_800128d0`, a later indirect callback, and proven map-object, actor, audio, CD, TMD, and notification calls. Its mixed state families lack a complete typed source model. |
| `800475d8` | WIP, unclaimed | Paired controller calls `PadRead`, CD services, pose interpolation, map-object reset, and frame helpers; its 0x6c0-byte branch/state family still lacks a complete source model. |
| `8003fb94` | WIP, source, 87.791046% strict | Effect damage router forwards a player hit or an actor hit through optional facing gate; 11/11 CFG blocks, six branches, calls, and referents agree. Prologue and stacked-argument scheduling differ; nine adjacent effect functions remain exact. |
| `800492dc` | WIP, unclaimed | Card-save restore walker is called by `22b74` and has no direct callees. Its 0x5e0-byte body copies disjoint byte, halfword, and word payload fields into player and graphics state; packed payload schema and complete owners remain unresolved. |

The focused comparisons preserved every existing exact neighbor. A fresh
strict GAME report verified 143/143 target-unit relinks and retained the same
three unrelated RODATA addend divergences. No function or data claim was added
for the unresolved event tables or card payload.

## Ten-function map placement and actor-event continuation

This fresh GAME set connects map-cell placement to the map-object and frame
controllers, then follows the event stream into save restoration. The
source-backed functions remain non-exact at the latest strict report; the
three large controllers retain no source claim. No semantic trial was kept
solely for a higher fuzzy display score.

| GAME VA | Final verdict | Evidence or boundary |
| --- | --- | --- |
| `80034f90` | WIP, source, 97.86822% strict | Rotates placement offsets with `rcos`/`rsin`, then updates both typed occupancy layers. Focused 14/14 CFG and 7/7 branches agree; cell-origin/flag saved-register assignment and one address-formation schedule differ. |
| `80035194` | WIP, source, 89.59545% strict | Copies selected byte fields between rotated occupancy rectangles; focused 51/51 CFG blocks and 26/26 branches agree, with broader index/bit-field scheduling differences. |
| `800356ac` | WIP, source, 75.57377% strict | Writes the selected map-cell marker and object scale, gated by player-state +0x6a. Focused 9/9 CFG and 4/4 branches agree, but row/column address formation differs in both arms; five neighboring map-object reset listings remain exact. |
| `80035894` | WIP, unclaimed | Placement controller calls `34f90` and `35194` twice each and `356ac`; two indirect transfers and the loaded `8006787c` pattern records lack complete ownership. |
| `80036190` | WIP, source, 89.78417% strict | Actor-facing map-object query calls rotation, distance, and angle helpers. Focused 15/15 CFG blocks and 8/8 branches agree but initial bound-check successor, signed index lifetime, and return frontier differ. Removing the redundant C check worsened CFG and was reverted. |
| `80036e24` | WIP, source, 98.86364% strict | Per-frame brightness loop calls both CD services, player camera pose, and frame renderer. Focused 5/5 CFG and 2/2 branches agree; only mode/endpoint/step saved-register assignment differs. |
| `800461a0` | WIP, source, 99.12676% strict | Explicit `0xf1` and `0xfe` branches with a shared fallback join now give matching 14/14 CFG blocks and 5/5 branches. The two record-cursor registers remain exchanged; adjacent `46144` remains exact. |
| `80047c98` | WIP, unclaimed | Event controller calls terrain probe, actor distance, scene interpolation, notification, and resource restore; an indirect callback and mixed event-object state remain unproved. |
| `800482f8` | Exact, source, 100% strict | Clears typed event control and arena, initializes checked sentinel fields and saved offsets; all six event-state listings now match exactly. |
| `800489ac` | WIP, unclaimed | Save decoder calls pointer-offset restoration and map-object reset; the indirect table at `80012bf8` and packed payload schema have only candidate ownership. |

The three already exact candidates `8002d4f4`, `80034818`, and `80045e5c`
were excluded before locking this set. Existing exact controls in the shared
units remained listing-identical after focused probes.

## Ten-function map visibility and resource continuation

This GAME set follows map-cell masks and TMD resources into map rendering and
map-object effects. All ten were below strict `100%` at selection. The
source-backed bodies have the retail call sets and the stated CFG limits, but
none gained a justified exact source correction in the focused pass.

| GAME VA | Final verdict | Evidence or residue |
| --- | --- | --- |
| `8002f5b0` | WIP, source, 95.833336% strict | Fan-triangulates clipped map vertices into GT3 packets. Focused 13/13 CFG, 7/7 branches; vertex register assignment, UV load order, depth division, and loop schedule differ. Adjacent map enqueue remains exact. |
| `8002f808` | WIP, source, 63.362473% strict | Draws prepared or normal TMD map primitives through clipping and lighting. Focused 51/51 CFG blocks but 35 retail versus 34 probe branches; primitive early-return and packet paths need more source reconstruction. |
| `80030c18` | WIP, source, 96.521736% strict | Builds a cell matrix, lighting, and fog before selecting normal or prepared map rendering. Focused 11/11 CFG, 6/6 branches; first divergence is entry address/load/register scheduling before `SetRotMatrix`. |
| `80030de4` | WIP, source, 89.52128% strict | Draws either or both occupancy layers for one map cell. Focused 10/10 CFG, 6/6 branches; view-relative position and layer byte-load schedule differ. The following `30f5c` and `31024` listings remain exact. |
| `800320b0` | WIP, source, 73.95918% strict | ORs layer-mask bytes in a bounded square around a world position. Focused 10/10 CFG, 6/6 branches; row/column arithmetic and pointer lifetime differ. |
| `80032174` | WIP, source, 93.60000% strict | Checks whether a cell rectangle contains the current view cell. Focused 5/5 CFG, 3/3 branches; return-value and coordinate-register scheduling differ. |
| `800321d8` | WIP, source, 98.43590% strict | Allocates and queues a TMD archive read with matching 3/3 CFG and call set. Retail forms the fixed arena pointer `0x8009b0a0` with `lui/addiu`; the probe's literal forms `lui/ori`. Main-loop initialization also passes this address, but its source owner or linker mechanism remains unproved. |
| `80032274` | WIP, source, 90.933334% strict | Updates a range of VAB stream slots and queues missing entries. Focused 13/13 CFG, 7/7 branches; the probe retains an extra indexed-slot register, changing frame size and branch schedule. The neighboring `32364` TMD range updater stays exact. |
| `80036464` | WIP, source, 95.32258% strict | Acquires and initializes a map effect object, then selects an action from template kind. Focused 12/12 CFG, 3/3 branches with incomplete indirect-table reachability; source register schedule differs, and the unit's RODATA text addend remains affected by earlier WIP `36190`. |
| `800366fc` | WIP, source, 37.336365% strict | Updates marker/action state across 396 map objects. Focused CFG is 37 retail versus 38 probe blocks with 25/25 branches; switch-arm ordering and flag mutation remain non-exact. The preceding scatter source and following marker-query listing remain exact. |

The arena address is a provisional fixed boundary, not a proved source-level
symbol; no overlapping global or linker placement was added to manufacture
`321d8`'s target instruction form. The exact `32364` resource updater and all
other exact neighboring claims remained unchanged.

## Ten-function scene transition and effect-motion continuation

This GAME set follows the main loop's resource transition into animation,
actor collision, and effect spawning. All ten were non-exact or unclaimed at
selection. Retail CFG, incoming and outgoing references, strings, adjacent
claims, and current source were inspected for each address. One source-backed
function reached strict `100%`.

| GAME VA | Final verdict | Evidence or boundary |
| --- | --- | --- |
| `8001369c` | WIP, unclaimed | The 0x2f0-byte main loop clears runtime arrays, initializes CD/display, and drives resource transition and frame services. Forty-six outgoing proven references cross several BSS owners, so the complete source object model remains open. |
| `80015d58` | WIP, unclaimed | Opens seven archive names and queues VAB/map resources, then makes length-prefixed copies into fixed destinations. The destination extents lack a proved common source owner. |
| `80015fd4` | WIP, unclaimed | Saves five callback-state bytes, sets transition values, yields CD work while `16820` runs, then installs TMD slot zero and invokes an indirect callback. The TMD pointer `0x8012da68` is outside the linked load image and has no proved source-level owner. |
| `80016260` | WIP, unclaimed | Scene transition controller has 67 CFG blocks, audio sequence changes, CD yielding, and repeated `16820` calls. Its caller passes eight O32 inputs; the transition payload and persistent callback state are not fully modeled. |
| `80016820` | WIP, unclaimed | The 62-block phase dispatcher queues archive reads and resource copies and uses five adjacent exact phase setters. Its indirect phase jump and candidate table remain unresolved. |
| `80033d3c` | WIP, source, 95.21839% strict | Scales sparse morph deltas as MATRIX rows and accumulates them into eight-byte vertex records. Focused 17/17 CFG blocks and 10/10 branches agree, but the skip-group flush and loop-exit layout diverge; the two preceding sparse helpers remain exact. |
| `8003b5d0` | WIP, source, 75.66939% strict | Actor vertical collision mode reads the provisional `bss_801c7540+0x1180a..10` cache and calls the collision probe. Focused 40/40 CFG blocks and 21/21 branches agree, but early phase-arm order and register lifetimes differ; three preceding motion helpers remain exact. |
| `8003fa68` | WIP, source, 70.73333% strict | Cooldown gate dispatches four mode values (`0xa1`, `0x31`, `0xb1`, `1`) to `2b9d4`. Retail keeps four distinct calls; the current compiler merges the source calls. A direct-return trial also merged them and was discarded. |
| `80040308` | WIP, unclaimed | The 0x13e4-byte effect constructor calls exact pool helpers but branches indirectly through candidate table `0x8001249c`. Table and branch targets are not promoted from candidate evidence. |
| `80041e94` | **Exact, 664/664 bytes strict** | Effect motion and random spread call `400c0`, `rand`, vector helpers, then `40308`. Retail's eighth fixed O32 argument precedes further mode operands; modeling that ABI in C gives a focused identical listing and strict `100%`. Adjacent `4212c` remains 364/364 exact. |

The strict GAME report relinked 144/144 target units. Its global check still
stops at the three pre-existing unrelated RODATA text-addend divergences and
incomplete reference closure; these do not affect the `41e94` verdict.

## Ten-function actor targeting and trajectory continuation

This GAME set follows target scoring through actor magic, radial player
damage, collision probes, movement, and the behavior dispatcher. Each
function's retail body, CFG, direct references, strings, source, and current
strict score was inspected. None reached a justified new exact match. The
source-backed units were probed against their existing exact neighbors.

| GAME VA | Final verdict | Evidence or residue |
| --- | --- | --- |
| `80039108` | WIP, unclaimed | The 0x4c0-byte, 59-block target scorer returns a signed candidate value. An indirect jump at `3916c` and indirect call at `39564` still need dispatch and callback ownership. |
| `80039c94` | WIP, unclaimed | The 0x684-byte, 72-block actor magic recipient is called by actor damage and effects; its indirect call at `39f38` and record contract remain unresolved. |
| `8003a318` | WIP, source, 73.59162% strict | Typed 200-actor magic scan has the proven distance and `39c94` calls. Focused CFG is 26 retail versus 25 compiled blocks, with the sentinel arm folded; adjacent `3a778` remains exact. |
| `8003a614` | WIP, source, 96.91011% strict | Actor-to-player radial damage gate has matching six-block CFG and three branches. The compiler reuses a player-state base where retail forms a second address and assigns the scaled inputs to different registers; adjacent `3a778` remains exact. |
| `8003a9f4` | WIP, source, 89.04444% strict | First 200-actor collision candidate scan checks target type and an alternate-height flag. Focused CFG is 13 retail versus 12 compiled blocks; stack-argument lifetime and branch-arm layout differ, while four adjacent animation helpers remain exact. |
| `8003ab5c` | WIP, source, 88.53488% strict | Sibling candidate scan omits the target-type check. Focused CFG is 12 retail versus 11 compiled blocks with the same alternate-position layout residue; four adjacent animation helpers remain exact. |
| `8003ae50` | WIP, unclaimed | The 0x4ec-byte, 58-block motion helper calls three collision probes, angle helpers, `SquareRoot0`, `rsin`, and `rcos`. Its actor terrain/result-field contract is incomplete. |
| `8003bd40` | WIP, source, 86.53226% strict | Computes X/Z angle, Manhattan range, and tolerance before turning the actor. Focused 9/9 CFG and 5/5 branches agree, but actor-base and saved-register scheduling differ. Reusing `dx` for the Manhattan sum reduced focused similarity and was discarded; six neighboring motion helpers remain exact. |
| `8003c3e0` | WIP, source, 96.524826% strict | Iterative direction solver uses actor rotation, bounded yaw/pitch, and player-state displacement. Focused 23/23 CFG and 11/11 branches agree; the probe uses an 88-byte frame where retail uses 80, with stack-argument and saved-register lifetime differences. Three preceding vector helpers remain exact. |
| `8003c614` | WIP, unclaimed | The 0xa70-byte, 45-block behavior dispatcher calls `3c3e0` eleven times and branches indirectly at `3c7c0`; the table and complete mode contract remain candidate evidence. |

No C or configuration change from this ten-function batch was retained. The
source-backed scores are from the latest strict GAME report; focused probes
confirmed the listed CFG and exact-neighbor controls.

## Ten-function player damage and action dispatch continuation

This GAME set follows player damage through magic, movement, and collision
dispatch. All ten were non-exact or unclaimed when locked. Retail bodies,
CFG, calls, data references, strings, adjacent functions, and existing source
were inspected. No source rewrite survived focused comparison; the retained
change is a narrower, directly checked model of three switch-table spans.

| GAME VA | Final verdict | Evidence or boundary |
| --- | --- | --- |
| `80024498` | WIP, unclaimed | The 53-block damage path has an eight-entry bounded indirect jump through `80011128`. All eight retail words target this body; the jump's complete control-flow semantics remain unresolved. |
| `800248a8` | WIP, unclaimed | The 36-block damage calculation calls `player_calculate_damage_component` eight times and dispatches through seven bounded words at `80011148`. Every target lies in this function; no C owner is yet established. |
| `80025a18` | WIP, unclaimed | The 99-block weapon/effect path calls exact `25878` and the effect constructor. Its effect-state contract and candidate table at `80011188` are incomplete. |
| `8002665c` | WIP, unclaimed | The 114-block weapon/magic updater calls exact `26498`, animation vertex helpers, and vector rotation. The coupled equipment, magic, and animation state still lacks a complete source model. |
| `8002722c` | WIP, unclaimed | The 33-block action selector has two bounded indirect jumps. The six words at `80011298` and 19 at `800112b0` all target this function, but semantic indirect reachability and magic-state ownership remain open. |
| `800274ec` | WIP, unclaimed | The 35-block movement path combines horizontal trig, collision probes, and the provisional collision-cache view at `bss_801c7540+0x11800`. The equipment/cache extent remains unresolved. |
| `800279cc` | WIP, unclaimed | The 70-block collision path calls `2b604`, `2b9d4`, and `24498`; it reads the same provisional cache. No separate overlapping global was introduced. |
| `80027f78` | WIP, unclaimed | The 27-block collision response uses three probe calls, vector length, and player death handling. The probe result and cache contracts are not yet complete. |
| `8002897c` | WIP, source, 88.57143% strict | All three CFG blocks match. Retail retains the upper-bound flag in `v1` and result in `v0`, while the probe swaps them and adds a final move. Direct-return and alternate-local trials regressed or remained non-exact and were discarded. |
| `80029624` | WIP, source, 62.44898% strict | The 20/20 CFG blocks and 9/9 branches agree, but the zero-phase sentinel return and join schedule differ. Signedness and single-return trials did not improve the listing; the neighboring exact reaction functions were preserved. |

The four data spans are now `pointer[8]`, `pointer[7]`, `pointer[6]`, and
`pointer[19]` across the two damage functions and the action selector,
respectively. Their 40 pointer words and four HI16/LO16 base pairs were
reviewed against retail, retaining address-derived identities and blank
ownership. `kf-retail-validate`, `kf inventory check`, GAME target delinking,
and `git diff --check` passed. The full build produced PSX; GAME, OPEN, and
END still stop at unresolved `InitCARD`, `malloc`, and `display_buffers`,
respectively, without a module-cap error. No repository tests were run.

## Ten-function trajectory, collision, and actor-damage continuation

This GAME set follows interpolation and trajectory helpers into player input,
actor damage, target fixup, and effect routing. Retail bodies, CFG, callers,
callees, strings, source history, and current match state were inspected for
all ten. No function reached a new strict `100%` match. One layout-backed
source correction was retained without changing its strict score.

| GAME VA | Final verdict | Evidence or residue |
| --- | --- | --- |
| `800158b4` | WIP, source, 98.00000% strict | The nine-halfword interpolation loop has matching 3/3 CFG blocks and no calls. The probe assigns the pointer and countdown to opposite registers and schedules their increments differently. A natural increment-order trial shortened the listing and was discarded; the two preceding fixed-point leaves remain exact. |
| `80015918` | WIP, source, 95.49419% strict | The trajectory solver has matching 41/41 CFG blocks, 24/24 branches, and three direct calls. Its discriminant and time-choice values occupy different registers after the first check. The two following trajectory/vector helpers remain exact. |
| `80023384` | WIP, source, 50.744186% strict | The five-block vertical-bounds helper reads two provisional collision-cache words and calls player death on either failed bound. The probe combines and hoists three player-height loads; retail reloads them separately in both arms. Cache extent still overlaps the provisional equipment tail. |
| `80028998` | WIP, unclaimed | The 59-block player input/weapon controller calls the action selector twice, the effect dispatcher, power test, and attack starter. `DAT_800667e8+0xc` remains a candidate effect-ID referent, so its complete source/data owner is open. |
| `8002985c` | WIP, unclaimed | The large player update has two curated fragments in a 0x9dc-byte body. The semantic CFG is unavailable until fragment ranges and internal control are resolved; no complete C claim was made. |
| `8003a318` | WIP, source, 73.59162% strict | The typed radial actor-damage loop preserves distance calls and sixteen-argument forwarding, but retail has 26 CFG blocks versus 25 compiled; the sentinel arm is folded. Adjacent actor target finder `3a778` remains exact. |
| `8003a614` | WIP, source, 96.91011% strict | The six-block actor-to-player radial gate has matching calls and branches. Retail forms a second player camera address independently, while the probe reuses the earlier base; adjacent `3a778` remains exact. |
| `8003bd40` | WIP, source, 86.53226% strict | The actor X/Z steering function has matching 9/9 CFG blocks and 5/5 branches. Its angle, Manhattan range, and tolerance logic are retained; register and load scheduling differ while the six exact neighboring motion helpers remain exact. |
| `8003f7ec` | WIP, source, 85.86207% strict | Retail forms the target-candidate pool base as `target_groups + 40` (`40 × 0x78 = 4800`), exactly the next field in the checked actor-state layout. The source now expresses that derivation. A focused listing shows 9/9 CFG and two instruction-order residues; its separate listing-similarity metric is 93.8%. Both adjacent functions remain strict exact. |
| `8003fb94` | WIP, source, 87.791046% strict | The effect damage router has matching 11/11 CFG blocks, 6/6 branches, calls, and referents. Prologue saves and stacked-argument setup are scheduled differently; nine adjacent effect-update functions remain exact. |

The retained `3f7ec` correction did not change its strict objdiff score, so
it is reported as WIP. GAME target relink verified 145/145 units; the global
check still stops at three known unrelated RODATA addends and incomplete
reference closure. A full `kf build` produced PSX; GAME, OPEN, and END retain
unresolved `InitCARD`, `malloc`, and `display_buffers`, with no module-cap
error. No repository tests, banking, or commit were run.

## Ten-function player damage and collision-height continuation

This GAME set connects the two player-damage dispatchers with eight functions
in the contiguous collision-height and render-mask unit. Each retail body,
CFG, caller/callee set, data reference, string result, adjacent claim, and
current strict score was checked. The only retained C correction models a
directly observed collision-cache store; it improves one WIP score without
altering exact neighbors or the unit's load-image data.

| GAME VA | Final verdict | Evidence or residue |
| --- | --- | --- |
| `80024498` | WIP, unclaimed | The 53-block player damage path dispatches through eight reviewed words at `80011128`, all targeting this body. Its complete branch and source-data ownership remain open. |
| `800248a8` | WIP, unclaimed | The 36-block damage dispatcher calls the exact component helper eight times and branches through seven reviewed words at `80011148`. The caller-proven twelve-argument ABI is retained; no complete C body is claimed. |
| `8002b67c` | WIP, source, 94.895836% strict | The four-block cell-height wrapper sets the cache layer, height, and cell before calling the map collision probe. Retail reloads the just-stored cache height for the call; the probe retains its register value. |
| `8002b73c` | WIP, source, 98.40426% strict | The eight-block occupancy rectangle update has matching four-branch flow and byte stores. Row/column register assignment and pointer-increment scheduling remain different. |
| `8002b874` | WIP, source, 93.52273% strict | The eight-block cache-position selector copies player, actor, or map-object position and dimensions. The probe has nine blocks and schedules the actor radius store after an additional halfword load. |
| `8002b9d4` | WIP, source, **92.055176% strict** | The 23-block collision dispatcher preserves all twelve branches and direct calls. Retail stores the `3ab5c` result at `bss_801c7540+0x11824`, tests it, then clears that actor-index slot to `-1` in the mode-`0x40` arm. Modeling that observed store raised strict similarity from 89.40000%; broader result-register and branch-arm ordering still differ. |
| `8002bfd4` | WIP, source, 54.15534% strict | The 22-block line rasterizer writes the 24×24 render mask. The probe has 21 blocks; line-step branch shape and byte-mask scheduling differ. |
| `8002c170` | WIP, source, 89.80000% strict | The eleven-block row scanner finds a contiguous run of matching mask bytes. Its five branches agree, while pointer/state register assignment and operand order differ. |
| `8002c290` | WIP, source, 71.32674% strict | The fifteen-block map-cell mask evaluator uses typed occupancy layers and the render-mask scan state. Its ten branches agree, but the first-layer lighting arm reaches a different successor in the probe. |
| `8002c424` | WIP, source, 55.789116% strict | The 23-block scan-line propagator uses typed map cells and mask state. The probe has 22 blocks with fourteen matching branch counts; loop-entry and skip-arm joins remain non-exact. |

`bss_801c7540+0x11824` is a temporary interior view of the complete
startup-cleared BSS object. Its boundary with the provisional equipment
record array is not proved, so no overlapping global was added. The reviewed
damage switch tables remain address-derived, and both indirect dispatches
remain unresolved. The collision unit's nine other functions retain strict
`100%` listings, and its initialized DATA remains 3520/3520 bytes exact.
The strict run relinked 145/145 GAME target units; global closure still stops
at three unrelated RODATA addends. A full `kf build` produced PSX; GAME,
OPEN, and END retain unresolved `InitCARD`, `malloc`, and `display_buffers`
without a module-cap error. No repository tests, banking, or commit were run.

## Ten-function player action and actor-motion continuation

This GAME set follows the player action selector and horizontal movement into
collision response and actor motion. All ten were non-exact when locked.
Retail CFG, callers, callees, data references, strings, adjacent claims, and
KF1 analogues were inspected. The existing source and its strict exact
neighbors remain unchanged after a source-only actor-motion probe.

| GAME VA | Final verdict | Evidence or boundary |
| --- | --- | --- |
| `80025a18` | WIP, unclaimed | The 99-block weapon/effect dispatcher calls exact `25878`; the indirect table at `80011188` is still candidate, and effect-state mutation is not fully typed. |
| `8002665c` | WIP, unclaimed | The 114-block weapon/magic updater calls exact `26498`, animation vertex helpers, and rotation math; its coupled animation/equipment/effect state lacks a complete source model. |
| `8002722c` | WIP, unclaimed | The 33-block action selector uses two reviewed, bounded internal switch tables at `80011298` (six words) and `800112b0` (19 words). Pointer rows and base relocations are curated; indirect reachability and owning C control flow remain unresolved. |
| `800274ec` | WIP, unclaimed | The 35-block horizontal mover is called three times by exact `2851c`; its `rsin`/`rcos` displacement, collision retry, and wall-deflection logic resemble KF1 `player_move_horizontal`, but KF2's collision-cache fields and retry arms differ. |
| `800279cc` | WIP, unclaimed | The 70-block player collision path calls `2b604`, `2b9d4`, `24498`, and reaction helpers; it reads the provisional cache at `bss_801c7540+0x11800`. |
| `80027f78` | WIP, unclaimed | The 27-block sibling collision path makes three `2b9d4` probes and calls death and distance helpers. The cache/equipment boundary remains unproved. |
| `80028998` | WIP, unclaimed | The 59-block input/weapon controller calls `2722c` twice and `25a18`; the `DAT_800667e8+0xc` effect-ID referent is candidate and has no proved source owner. |
| `8002985c` | WIP, unclaimed | The 0x9dc-byte player update has two curated fragments; its complete internal control flow and data ownership remain unresolved. |
| `8003ae50` | WIP, unclaimed | Three exact actor-motion helpers call this 0x4ec-byte X/Z mover. The KF1 `actor_move_xz_with_collision` analogue supports its blocked/sliding intent, but KF2 has additional collision and trigonometric retry paths that are not yet typed. |
| `8003b5d0` | WIP, source, **75.66939% strict** | The vertical-mode source retains 40/40 blocks and 21/21 branches. Retail tests phase `0x20` before the `0/0x10/0x30` arms; the current compiler chooses another dispatch order. An equivalent condition-spelling probe improved listing similarity from 65.1% to 66.5% but added a saved register and left CFG successors different, so it was discarded. The three preceding actor-motion functions remain strict exact. |

The provisional collision cache remains an interior view of its complete BSS
owner, without a competing global. No source, manifest, header, or retail
inventory edit was retained in this batch. The actor-motion source-only probe
preserved its three exact neighboring listings. Focused listing similarity is
not the strict objdiff percentage. No repository tests, banking, or commit
were run.

## Ten-function interpolation and actor-damage refinement

The revised GAME set is `158b4`, `15918`, `23384`, `2897c`, `29624`,
`3a318`, `3a614`, `3bd40`, `3fb94`, and `3c3e0`. `26498` was initially
proposed but a fresh report showed it already strict exact, so it was kept
only as a control and replaced with the non-exact `3c3e0` before counted
work. Retail CFG, calls, data references, callers, adjacent claims, current
source, and relevant KF1 source were reviewed. Two source changes survived
focused and strict comparison.

| GAME VA | Final verdict | Evidence or residue |
| --- | --- | --- |
| `800158b4` | **exact, 100% strict (100/100)** | Separate source, target, and destination matrix-element pointers and source-before-target element evaluation emit retail's load/increment schedule. The two preceding interpolation leaves remain exact. |
| `80015918` | WIP, source, 95.49419% strict | The trajectory solver retains 41/41 CFG blocks, 24/24 branches, SquareRoot0 calls, and caller-backed widths. Discriminant and time-choice register lifetimes still differ; its two following vector helpers remain exact. |
| `80023384` | WIP, source, 50.744186% strict | Five-block player bounds test reads two provisional collision-cache words and calls death for either failed bound. The probe combines/hoists player-height loads that retail repeats; the cache/equipment boundary is still provisional. |
| `8002897c` | WIP, source, 88.57143% strict | Three-block inclusive 71..80 predicate matches the branch and constants; retail keeps upper-bound condition in `v1` and returns directly in `v0`. Explicit upper-local and else-arm source probes did not preserve that schedule and were discarded. |
| `80029624` | WIP, source, 62.44898% strict | The signed-halfword phase/ramp helper retains 20/20 CFG blocks and 9/9 branches. The zero sentinel and shared return scheduling differ; exact adjacent reaction functions remain intact. |
| `8003a318` | WIP, source, 73.59162% strict | The 200-actor radial-damage scan keeps caller-backed sixteen-argument forwarding and distance calls. Retail has 26 CFG blocks versus the probe's 25; the sentinel arm is folded in the probe. Exact `3a778` remains intact. |
| `8003a614` | WIP, source, 96.91011% strict | Six-block actor-to-player damage gate matches calls/branches; retail forms an independent second player-camera address while the probe reuses the first base. Exact `3a778` remains intact. |
| `8003bd40` | WIP, source, 86.53226% strict | Nine-block X/Z steering logic matches calls and referents. Register/load scheduling differs; six exact actor-motion siblings remain intact. |
| `8003fb94` | WIP, source, 87.791046% strict | Eleven-block effect damage router matches its calls and referents. Prologue and stacked-argument scheduling differ; nine effect-update siblings remain exact. |
| `8003c3e0` | WIP, source, **99.64539% strict (562/564)** | Retail loads pitch override once as a word, sign-tests the low halfword for `-1`, and otherwise writes the original word. Modeling that view raised strict score from 96.524826%. The remaining yaw-normalization `a2`/`v1` lifetime differs; 23/23 CFG blocks, 11/11 branches, and three adjacent exact group-position helpers remain intact. |

The two retained changes are in `actor_fixed_interpolation.c` and
`actor_group_position.c`. A source-only unsigned yaw trial emitted wrong
unsigned loads/compares, and a normalized-yaw rewrite moved the branch
schedule away from retail; both were discarded. GAME target relink verified
146/146 units. Global closure still stops at the same three unrelated RODATA
addends. The full build produced PSX; GAME, OPEN, and END still stop at first
unresolved `InitCARD`, `malloc`, and `display_buffers` without module-cap
failure. `git diff --check` passed. No repository tests, banking, or commit
were run.

## Ten-function map-object and event-control continuation

This GAME set is `356ac`, `36190`, `36464`, `366fc`, `36e24`, `460a0`,
`461a0`, `462bc`, `4678c`, and `475d8`. The ten were non-exact when locked.
The four larger retail bodies were checked for extent, CFG, incoming calls,
outgoing calls, data references, strings, and adjacent ownership. Their
indirect targets remain candidate or unresolved; a direct `jal` is distinguished
from a proposed table pointer. Source-only probes did not support a retained
change.

| GAME VA | Final verdict | Evidence or residue |
| --- | --- | --- |
| `800356ac` | WIP, source, 75.57377% strict | The typed 10-byte occupancy-cell marker update has 9/9 CFG blocks and 4/4 branches. Retail loads Z then X and materializes the BSS base later; a direct `map_cells[z][x]` source probe kept the behavior but did not improve the instruction schedule. Five exact reset-unit siblings remain intact. |
| `80036190` | WIP, source, 89.78417% strict | The typed 396-object proximity/facing scan has 15/15 CFG blocks and 8/8 branches. The initial range exit and two per-object retry joins differ; removing the redundant explicit initial guard reduced the probe to 14 blocks, so the existing source was kept. Its two exact map-object siblings remain intact. |
| `80036464` | WIP, source, 95.32258% strict | The effect-object spawn has 12/12 known blocks and 3/3 branches, but its `0x36554` indirect switch and the unit's RODATA text addend are not closed. Saved object-id/height registers and the reset-call store delay slot differ; exact neighboring helpers remain intact. |
| `800366fc` | WIP, source, 37.336365% strict | The 396-object action/marker scan has 37 retail versus 38 probe CFG blocks and 25/25 branches. Retail carries a pointer at marker offset `+0x38`, whereas the probe carries the timer at `+0x08`; a typed member-pointer probe compiled identically and was discarded. The preceding scatter function and following marker query remain exact. |
| `80036e24` | WIP, source, 98.86364% strict | The brightness/CD/camera frame loop has 5/5 CFG blocks and 2/2 branches. Only saved argument-register assignment for mode, final phase, and step remains different; no independently supported source correction was found. |
| `800460a0` | WIP, source, 99.268295% strict | The actor phase seek has 6/6 CFG blocks and 2/2 branches. Retail keeps even step and half-step in different saved registers. Declaration-order and half-step expression probes were neutral or worse and were discarded. |
| `800461a0` | WIP, source, 99.12676% strict | The event marker scan has 14/14 CFG blocks and 5/5 branches. Retail's record cursor and marker pointers occupy the opposite registers; source-order and alternate marker-view probes did not preserve the listing. Adjacent `46144` remains exact. |
| `800462bc` | WIP, unclaimed | The `0x444`-byte event-command interpreter dispatches bytes `0xf0..0xff` through candidate 16-word table `DAT_80012890`, calls exact `46144` and the WIP phase/marker helpers, and later calls through an unresolved pointer. Table targets and callback owner are not proved. |
| `8004678c` | WIP, unclaimed | The `0xc54`-byte scene controller dispatches `0x52..0x74` through candidate table `DAT_800128d0`, later calls an unresolved callback, and directly calls map-object, pose, audio, CD, notification, and player-stat helpers. The event-record layout and indirect targets are incomplete. |
| `800475d8` | WIP, unclaimed | The `0x6c0`-byte paired controller reads four O32 input slots, optionally allocates/resets a map object, and calls PadRead, pose interpolation, CD service, counter, and frame helpers. The complete record and branch-state model remains open. |

The three large controllers have no new source or manifest claims. All source
trials were temporary; the shared source, header, config, and retail inventories
remain unchanged from the prior batch. No target-wide regeneration was needed
for this read-only/source-probe set. No repository tests, banking, or commit
were run.

## Ten-function actor movement and player-damage continuation

The locked GAME set is `2897c`, `29624`, `3a318`, `3a614`, `3ae50`,
`3b5d0`, `3bd40`, `3c3e0`, `3f7ec`, and `3d084`. All ten were non-exact when
locked. Retail CFG, direct calls, referenced data, incoming uses, adjacent
claims, and relevant KF1 analogues were reviewed. One evidence-backed source
change survived focused and strict comparison.

| GAME VA | Final verdict | Evidence or residue |
| --- | --- | --- |
| `8002897c` | WIP, source, 88.57143% strict | The three-block inclusive 71..80 check retains retail's signed upper and lower comparisons. A natural logical-AND expression instead compiled to one unsigned range check and was discarded; exact callers remain untouched. |
| `80029624` | WIP, source, 62.44898% strict | The signed-halfword phase/ramp helper has 20/20 CFG blocks and 9/9 branches. Its zero sentinel and shared-return schedule remain different; exact reaction neighbors remain intact. |
| `8003a318` | WIP, source, **77.02094% strict** | Retail has separate unscaled (`falloff == 0x1000`) and scaled damage arms. Explicit `else` preserves that source distinction, improving strict 73.59162% to 77.02094% and probe CFG 25 to 26 blocks, equal to retail's 26; branches remain 13/13. The 16-argument forwarding and call set are retained; frame and stack-argument schedules still differ. Exact `3a778` remains intact. |
| `8003a614` | WIP, source, 96.91011% strict | Six-block actor-to-player damage gate has the retail call and branch set. Retail forms a separate player-camera address for a later Z load; the probe reuses the X base. Exact `3a778` remains intact. |
| `8003ae50` | WIP, unclaimed | The 58-block actor X/Z mover calls collision probes, angle math, SquareRoot0, `rsin`, and `rcos`. KF1 `actor_move_xz_with_collision` supports blocked/sliding behavior, but KF2's additional retry paths and typed collision result contract are not yet reconstructed. |
| `8003b5d0` | WIP, source, 75.66939% strict | The 40-block vertical motion/collision selector has 21/21 branches and the retail collision calls. State-arm order and return joins remain different; three adjacent actor-motion functions stay exact. |
| `8003bd40` | WIP, source, 86.53226% strict | Nine-block actor X/Z steering helper retains `vector_xz_to_angle`, tolerance test, and motion call. Saved-register and load scheduling remain non-exact; the adjacent exact motion siblings are preserved. |
| `8003c3e0` | WIP, source, 99.64539% strict (562/564) | Retail and probe share 23/23 CFG blocks, 11/11 branches, calls, and referents. Only yaw-normalization intermediate registers `a2`/`v1` differ. The three preceding group-position helpers remain exact. |
| `8003f7ec` | WIP, source, 85.86207% strict | The 40-group × 16-slot pointer fixup has 9/9 blocks and 4/4 branches. Retail uses offset-plus-base operand order and initializes the two sentinel constants in the opposite order; a natural pointer-expression reversal compiled identically and was discarded. Its two neighboring unit functions remain exact. |
| `8003d084` | WIP, source, 91.60000% strict | The five-block signed actor sound-note clamp and `rand` scale have matching calls and branches. GCC reassociates the final `-2` after the random shift; exact spatial-sound caller `3d0e8` remains intact. |

Only `src/game/actor_player_damage.c` changed. The focused probe kept exact
`3a778`, and strict objdiff reports the same exact control. GAME target relink
verified 149/149 units. Global edge check still stops at three unrelated
RODATA addends. The full `kf build` built PSX; GAME, OPEN, and END retain the
known first unresolved `InitCARD`, `malloc`, and `display_buffers` links with
no module-cap failure. No repository tests, banking, or commit were run.

## Merged-master player damage and movement pass

This ten-function GAME pass rechecked retail blocks, incoming and outgoing
references, strings, current source ownership, and the available KF1 source.
The percentages below are the existing strict objdiff baselines; this pass
used focused `kf try` only, as requested. No source or inventory edit was
retained.

| GAME VA | Verdict | Evidence or remaining constraint |
| --- | --- | --- |
| `80023384` | WIP, source, 50.744186% baseline | Both collision-cache margins and two death calls are modeled; focused 5/5 CFG differs in address formation and arithmetic scheduling. Cache extent at BSS `+0x11818/+0x1181c` remains provisional. |
| `80024498` | WIP, unclaimed | The 0x34c-byte damage path has an eight-entry internal switch at `80011128`, but complete case control flow and source ownership remain unresolved. |
| `800248a8` | WIP, unclaimed | The 0x3fc-byte damage dispatcher calls the exact component helper eight times and has a seven-entry internal switch at `80011148`; its twelve-argument body remains incomplete. |
| `80025a18` | WIP, unclaimed | The 0x918-byte weapon/effect dispatcher calls the exact target selector and effect constructor; the indirect table and state contract remain provisional. |
| `8002665c` | WIP, unclaimed | The 0xbd0-byte weapon/magic updater mixes two weapon-effect calls with animation, rotation, and collision paths; no complete C body is established. |
| `8002722c` | WIP, unclaimed | Two bounded jump tables at `80011298/800112b0` are reviewed, but indirect control flow and the full action-state model remain unresolved. |
| `800279cc` | WIP, unclaimed | The 0x5ac-byte main collision branch calls the provisional cache probes, damage reaction, and death helpers; cache ownership remains open. |
| `80027f78` | WIP, unclaimed | The 0x2ac-byte sibling collision branch makes three collision probes and invokes distance and death handling; its complete probe result contract remains open. |
| `8002897c` | WIP, source, 88.571430% baseline | Retail checks signed `<81`, then `<71`, and returns the inverse lower check. Five source-only semantic forms were probed; none matched the 0x1c-byte listing, so the original 3/3 CFG source was kept. |
| `80029624` | WIP, source, 62.448980% baseline | The phase/ramp helper still has 20/20 CFG blocks and 9/9 branches. A positive-guard return trial lost a CFG block; widening its signed phase local changed retail's halfword load and reduced listing similarity. Both were discarded, preserving fifteen exact neighbors. |

The next player-owned gaps examined read-only are `800274ec`, `80028998`,
and `8002985c`. The first has a KF1 movement analogue but a different KF2
collision call graph; the second depends on the unresolved action selector and
effect-ID table; the third is fragmented and has no curated CFG yet. These
are not a second completed ten-function batch.

### Focused damage-source continuation

`src/game/player_apply_damage.c` now claims GAME `800248a8` and its seven-word
switch table at `80011148`. The twelve-argument source models the eight
component calls, status application, and final signed damage forwarded to
`80024498`. Twenty-nine direct player-state HI16/LO16 relocations and ten
direct internal branch/call rows were checked against raw retail opcodes and
targets. Focused `kf try` reports **1/1 SAME** for the 0x3fc-byte listing;
no strict objdiff run was performed under the current focused-only constraint.

`src/game/player_damage_reaction.c` now claims GAME `80024498` and its
eight-word switch table at `80011128`. The source preserves the caller-backed
three-argument ABI, HP subtraction, origin-relative knockback, death path,
and 8-way reaction. Ten direct player-state HI16/LO16 pairs and seven internal
jumps were raw-checked and curated. A full-word reaction-flags parameter,
explicit origin-height intermediate, reverse-order null vector stores,
signed-halfword duration arguments, and explicit case 7 improved the focused
listing from 79.6% to **88.5% WIP**. The remaining difference is localized to
duration arithmetic register allocation and a four-byte body-size gap; the
direct calls, referents, switch bound, and 53/53 CFG blocks with 22/22 branches
now agree. No strict exact claim is
made.

Retail `lh` at `8001e94c` supports `KfPlayerState.unknown_54` as `s16`.
The layout-identical header and structure row refinement preserves focused
`game.player_state_equipment` 15/15 SAME,
`game.player_status_cap` 1/1 SAME, and
`game.player_apply_damage` 1/1 SAME. The menu owner's focused
`game.menu_status_render` probe confirms the signed `lh` now matches retail;
that unit remains 97.8% WIP for unrelated row-step register scheduling.

`src/game/player_move_horizontal.c` now claims GAME `800274ec` as a first-pass
movement/collision source. Retail and KF1 both support heading-derived X/Z
motion, collision sliding, an alternate diagonal retry, and final motion-state
halfwords; KF2's two collision probes and cache layout remain distinct. The
focused target initially withheld 18 candidate direct calls/jumps. All 18
were validated against encoded retail instruction targets, and 24 player/cache
HI16/LO16 pairs were separately validated against raw opcodes, base registers,
and signed low addends before curation. A second focused carve reports zero
withheld relocations. A subsequent shared accept-position join follows the
retail backward edge and reduces the compiled CFG from 40 to 39 blocks, though
focused similarity moves from 36.8% to 35.7%. The retail `0x27624` branch
sets its retry flag in the delay slot on both outcomes, so the source now does
the same. This reduces the compiled CFG again, to 38 blocks, while focused
similarity moves to **34.6% WIP**. Retail has 35 blocks and 20 branches versus
38 and 23 in the current probe. GCC reuses a player-state
base register across accesses that retail materializes separately; collision
cache ownership at BSS `+0x11800` is still provisional. No strict exact claim
is made.

GAME `80028998` is now sourced beside the existing `8002897c` interval leaf
in `player_interval_71_80.c`. Retail and the KF1 analogue support queued item
actions, timed magic shots, charge recovery, and a halfword mask sequence for
weapon attacks. Its 18 direct J/JAL sites and 62 adjacent HI16/LO16 address
pairs were checked against raw retail words; the focused carve now withholds
zero relocations. The source needed separate charge additions on the null and
nonnull magic-record paths, and a shared cancel path for failed charged
attacks. A checked high-halfword flag view reproduces retail's independent
`lhu player_state+0x142`. Focused `kf try` reports **SAME** for all 0x528
bytes of `80028998`; no strict objdiff pass was run under the focused-only
constraint. Adjacent `8002897c` remains WIP (28.6% focused) with its prior
source unchanged.

Retail `80028998` writes `DAT_800667e8+0xc` to `player_state+0x78`, then reads
and advances through halfwords. Bytes at `800667e8..800667fb` resolve the
formerly split datum: twelve indexed effect IDs (`27 28 3c 42 54 56`, twice)
followed by four aligned masks `0x20, 0x10, 0x80, 0xffff`. The complete
0x14-byte `KfPlayerMagicIdSequence` layout and checked player pointer at
+0x78 are curated, while the data's original TU owner remains unresolved.
Focused `game.player_magic_dispatch` stays 1/1 SAME after the typed table read.
`8002985c` still has two retail fragments and no complete CFG; its 113
outgoing xrefs remain candidate until the ranges are curated.

### Collision response and reaction follow-up

`src/game/player_collision_response.c` claims GAME `80027f78` as a
first-pass collision response. Eleven direct control relocations and twenty
player/cache HI16/LO16 pairs were checked against raw retail words; its
focused carve withholds none. Retail's three world probes, accepted camera
position, death check, horizontal motion scaling, and signed cache-height
fallback are represented. Moving the accepted-position and scaling joins to
the retail backward-edge order raised the focused listing from 46.2% to
**80.0% WIP**. The probe has 26 versus retail's 27 CFG blocks, with 14/14
branches and 1/1 returns. The first substantive residue starts at the
horizontal scale: retail keeps one pointer to the reaction vector in `s0`
and loads both components through it, while the probe materializes the Z
component as a separate global address. A pointer-lifetime source trial
produced a larger frame and 78.3%; a shared blocked-return trial produced
79.6%. A containing-reaction pointer trial fell to 75.3%. None was retained.
The collision-cache field is still an interior
view of a provisionally bounded BSS object, and this listing is not exact.

At this stage, the adjacent `800279cc` remained unclaimed after a full retail CFG, call, and
data pass: its 70-block mode dispatch uses modes 0, 16, 32, 64, and 80, with
collision probes, sound, damage, and death effects sharing the provisional
cache. A complete source model would need the mode-specific state contract;
the adjacency alone does not establish a common translation unit.

Two small player WIPs were probed without retained source changes.
`8002897c` still has its exact `80028998` sibling; a direct early-return
form changed its focused listing to 50.0% but inverted the retail branch
direction and added an internal jump. `80029624` still has fifteen exact
reaction siblings; preserving the raw unsigned phase halfword in a local
gave 55.4% and moving the invalid-phase return to a trailing branch gave
54.0%, both below the current focused 57.4%. Retail explicitly loads raw
halfwords before signed comparisons, so the signed interpretation remains
part of its supported semantics despite the codegen residue.

The existing `player_move_horizontal` WIP at `800274ec` now keeps its trial
destination as a `VECTOR`. Retail repeatedly stores X and Z at stack offsets
24 and 32, consistent with that field stride; the original local declaration
is not proved. The view models one trial world position rather than two
independent coordinates. Focused similarity rises from 34.6% to **46.4% WIP**;
retail/probe CFG still differs at 35/38 blocks and 20/23 branches. The
cache boundary and retry semantics remain provisional, so the listing is not
exact and no score-driven locals were introduced.

GAME `80024498` now compiles to a **focused SAME** 0x34c-byte listing in
`game.player_damage_reaction`. Retail carries its derived reaction duration
through signed halfword stores and arguments. Changing the source-local
`duration` from `s32` to `s16` removes the sole four-byte instruction gap and
its downstream branch-address shifts without changing the damage algorithm,
eight-way switch, or source/data referents. No strict objdiff pass was run
under the focused-only constraint, so it is not yet banked exact.

The same width discipline resolves GAME `80029624`: its computed phase-ramp
result is a signed halfword, and retail's invalid-phase `-1` sits after the
main calculation path. Typing the local `result` as `s16` and using a positive
guard for the main path yields **focused SAME** for all 0xc4 bytes. The
complete `game.player_reaction` unit now reports 16/16 SAME focused listings;
strict objdiff certification remains pending under the current constraint.

### Current player combat and collision graph verdicts

This 27-function related graph follows equipment-derived combat stats through
damage, action selection, movement, and collision response. “Strict exact”
identifies earlier verified objdiff results; “focused SAME” is only a listing
result from the present quick-build pass. The remaining members retain their
WIP or unclaimed status.

| GAME VA | Current verdict | Distinguishing evidence |
| --- | --- | --- |
| `80023384` | WIP source | Two collision-cache vertical margins; 63.2% focused after arithmetic-order correction, provisional cache extent. |
| `80023814` | strict exact | Rank-scaled player value. |
| `80023868` | strict exact | Nine equipment bonus components. |
| `80023984` | strict exact | Derived combat stats and equipment calls. |
| `80024034` | strict exact | Physical training accumulator. |
| `800240cc` | strict exact | Magic training accumulator. |
| `80024164` | strict exact | Experience and level growth. |
| `80024384` | strict exact | Scaled damage component. |
| `80024448` | strict exact | Signed HP delta and death threshold. |
| `80024498` | focused SAME | Signed halfword reaction duration; eight-way switch. |
| `800247e4` | strict exact | Status cap helper. |
| `800248a8` | focused SAME | Eight damage-component calls and seven-way switch. |
| `80024ca4` | strict exact | Sixteen-argument radial damage wrapper. |
| `80025a18` | unclaimed WIP | Large weapon/effect dispatcher and candidate table owner. |
| `8002665c` | unclaimed WIP | Weapon/magic updater with 114 retail CFG blocks. |
| `8002722c` | unclaimed WIP | Two bounded internal tables; indirect control unresolved. |
| `800274ec` | WIP source | VECTOR trial position and signed SVECTOR deflection, 61.9% focused; retail branch meaning restored. |
| `80027928` | strict exact | Collision-depth death check. |
| `80027988` | strict exact | Collision impact sound magnitude. |
| `800279cc` | WIP source, 77.8% focused | Five-mode collision dispatcher with matched CFG; cache extent and codegen residue remain provisional. |
| `80027f78` | WIP source | Three collision probes, 80.0% focused. |
| `80028224` | strict exact | Signed camera rotation update. |
| `8002851c` | strict exact | Forward/strafe player movement. |
| `8002897c` | WIP source | Inclusive 71..80 predicate; register-lifetime residue. |
| `80028998` | focused SAME | Timed attacks and halfword mask sequence. |
| `80029624` | focused SAME | Signed phase-ramp return; reaction unit 16/16 SAME. |
| `8002985c` | unclaimed WIP | One retail 0x112c-byte body, 142 CFG blocks; indirect jump reachability remains unresolved. |

The `80025a18` ABI needs more than one caller to describe. Retail homes
`a0` through `a3` before allocating its frame, then reads the incoming `a0`
as an unsigned effect selector bounded by 52. The exact `80026498` caller
passes the selector alone on some paths and passes a second position pointer
on others; `80028998` passes only the selector. No observed callee path reads
the extra incoming argument, so the curated identity records a first `s32`
argument and an unresolved variable tail. Its `DAT_80011188` jump table and
case reachability remain candidate evidence; no source body is claimed.

The current `80027f78` focused diff starts after the two successful world
probes. Both retail and C request the same fixed X/Z length and use the same
collision-cache referent. Retail routes the exhausted-length path through a
short return block after the scaling loop; an explicit C label now gives the
same 27/27 CFG blocks, 14/14 branches, and 4/4 return-frontier edges, raising
similarity to **80.5% WIP**. Retail retains one reaction-vector base in `s0`
for both component loads and stores, while this compiler rematerializes the
Z field's global address and swaps the threshold/vector saved registers. A
typed containing-reaction pointer probe lowered similarity to 75.3% and
enlarged the frame, so it was discarded. The remaining mismatch is not
evidence for an overlapping collision-cache global or a different call target.

In `800274ec`, retail stores two signed halfword deltas at stack `+40/+44`
before `vector_xz_to_angle`, then overwrites the same slots with the
`rsin`/`rcos` slide offsets. KF1 `player_move_horizontal` uses an `SVECTOR`
for this deflection. Replacing separate scalar deltas with a single `SVECTOR`
for both stages preserves that width and lifetime and raises focused
similarity from 46.4% to **60.2% WIP**. The compiled frame now matches the
retail 128 bytes. Retail still has 35 versus 38 compiled CFG blocks and 20
versus 23 branches; collision-cache ownership remains provisional.
The raw branch at `80027604` skips the special height test when
`flags & -6` is nonzero. The prior C condition had this arm reversed;
correcting it leaves the focused percentage unchanged but restores the
retail branch meaning. At `80027840`, retail enters the axis retry if that
height-test flag is set, or otherwise if collision flag bit 0 is set. The
prior negated flag test was also reversed; the corrected `flag || bit` guard
raises the focused listing slightly to 60.4%. Retail's diagonal-retry branch
at `8002787c` jumps back to the same axis-retry block used by that guard.
Sharing that branch in C removes the duplicate axis checks: focused similarity
reached 64.5% with 35/35 CFG blocks and 20/21 branches. KF1 uses a ternary
for the same angle deflection and retail selects between `+2016` and `+2080`
without an intervening jump. The corresponding C ternary raises the focused
listing to **65.7% WIP** and removes that extra jump, though the compiled CFG
now has 34 versus retail's 35 blocks and still 21 versus 20 branches. The
remaining first mismatch is in saved-register lifetime and the early
player-state base; the cache extent is unresolved. Retail at `80027638`
checks `death_state` and calculates `flags & 0x30` only in that branch's delay
slot; it does not gate the subsequent height comparison on those flags.
Removing the extra C condition restores that behavior and brings branch
counts to 20/20. Focused similarity falls to **61.9% WIP** and CFG blocks
are now 35/33; the semantic correction is retained despite the lower score.
The wall-deflection bearing loads both cached X/Z and player X/Z with `lhu`
before subtracting into signed 16-bit `SVECTOR` components. Explicit `u16`
source views now reproduce those four load widths; the focused score remains
**61.9% WIP**. This narrows an instruction-selection difference without
resolving the separate saved-register and CFG residue.

For the two-bound player death helper at `80023384`, retail forms each cached
bound plus 1600 before subtracting the three player-height terms. Splitting
those arithmetic steps in C raises focused similarity from 42.7% to
**63.2% WIP**, while preserving its 5/5 CFG blocks and 2/2 branches. The
remaining divergence is the probe's saved `player_state+0x134` base pointer
versus retail's separate address loads; the cache's complete extent remains
provisional.

GAME `8002722c` has three proven direct calls: the menu result path at
`800189bc` and two action-controller calls at `800289f0/80028a30`. Each
supplies one ID in `a0` and ignores the result. Retail immediately copies
`a0` to `a1`, rejects `0xff`, indexes 26-byte magic records, and compares
the record's unsigned MP cost; no incoming second argument is read. Its
address-derived identity now records `void func_8002722c(s32 magic_id)` as a
supported ABI. The six- and nineteen-entry internal switch tables remain
reviewed pointer data with candidate indirect reachability, so the body is
still unclaimed WIP. The focused `game.player_interval_71_80` caller remains
SAME at `80028998`; the `8002897c` predicate remains WIP.

The weapon/effect dispatcher at `80025a18` checks `effect_id <= 0x34`
unsigned before indexing the word table at `80011188`. All 53 consecutive
retail words from file offset `0x988` point to aligned addresses inside its
`0x918`-byte body; the following word is zero padding. They name 31 distinct
targets: 21 entries share `80026314`, and three share `80026204`. The former 53
single-word Ghidra candidates are now one address-derived `pointer[53]`
datum of size `0xd4`, with raw-reviewed pointer rows and the directly decoded
HI/LO base pair. A focused one-VA delink accepted that base pair, and `kf sema`
reports the complete table extent. Indirect case reachability and the C body
remain candidate/unclaimed; this data curation does not establish a TU owner.

The next player-controller call audit separates decoded direct calls from
fragmented caller reachability. In `8002665c`, 19 previously candidate `jal`
rows were checked against the retail instruction words and their exact target
addresses; its `8002714c` combat-stat call was already reviewed. The same
check reviewed twelve direct `jal` rows inside `800279cc`. Focused one-VA
delinks materialized these named `R_MIPS_26` calls. All fifteen internal `j`
rows inside the single-fragment `800279cc` body also have proven CFG edges;
raw-word review admitted them as local-section `R_MIPS_26` relocations, and a
second one-VA delink withheld none. The two calls from `8002985c` to
`800279cc` and its one call to `8002665c` were later reviewed after the
caller extent and bounded case table were established below.
Separately, the main-loop word at `800138e0`
decodes a direct `jal 8002985c` with a `nop` delay slot; its reviewed row
passed a one-VA delink of `8001369c`. None of these edges proves the three
large controllers' complete C bodies or callee signatures. Focused listings
for the adjacent `26498`, `27928`, `27988`, and `24498` sources remain SAME.
The contiguous `80028ec0..8002985c` reaction source retains 16/16 focused
SAME listings, including the damage and death helpers called by this graph.
The `800279cc` entry overwrites `a0` through `a3` with player-position words
and a fixed collision radius before its first call, never reads caller stack
arguments, and returns after calling the void bounds helper without setting
`v0`; its identity therefore records an address-derived `void (void)` ABI.
Its player-state byte at `+0xd0` dispatches 0, `0x10`, `0x20`, `0x40`, and
`0x50`. The first three values match KF1's grounded/falling/step-up vertical
state numbering, but KF2's extra two states and collision-cache coupling
remain to be reconstructed before renaming the shared field.
In `8002665c`, retail reaches the `0x10`/`0x11` weapon-mode fork only after
checking `16 <= player_state+0x9b < 18`. Both alternatives assign the effect
ID and count-byte index before use. Ghidra's apparent incoming third argument
and `t0` live-in on the fallback edge are artifacts of its unpruned fork;
the complete function ABI remains unclaimed pending the other paths.
The full 0xbd0-byte body has 114 retail CFG blocks and twenty proven calls,
including two to the exact magic selector, three vector rotations, and the
combat-stat helper. Its only surveyed external caller is a proven direct call
in the large `8002985c`, so neither function has a complete source-backed
control-flow contract yet.
At that caller, `8002a728` is `jal 8002665c` with a `nop` delay slot, and the
callee immediately overwrites `a0` from player state and reads no incoming
stack arguments. The caller ignores `v0`; this supports a no-argument void
candidate but does not establish all register live-ins across 114 blocks, so
the address-derived identity keeps its ABI unresolved.
The safe one-VA carve for `8002665c` also withholds zero relocations and
functions. That closes its current target-object references, not the
114-block source semantics or call-site ABI.
The separate `80025a18` effect dispatcher has 53 bounded pointer words
to 31 in-body case targets. A CFG walk from those entries reaches all 64
previously withheld J/JAL sites; every raw word decodes to its recorded
destination. The conservative one-VA delinker accepted their reviewed rows.
The remaining HI/LO pairs at `80025cf4` and `8002600c` point at adjacent
initialized templates: four `SVECTOR` records at `800667c8` and five
four-halfword records at `800667a0`. The first array's fourth halfword is
also read separately as a signed effect parameter, so its original C type
is unresolved. The retail loops copy each eight-byte record with `lwl/lwr`
and advance by eight; the arrays exactly fill the two spans between the
card assets and the magic-ID sequence. Their bounded data identities and
raw-checked HI/LO pairs also passed the conservative carve, reducing the
dispatcher from 66 to **zero** withheld relocations. The indirect jump's
original C form and defining TU remain candidate.
Focused controls in the same player/equipment graph remain 15/15 SAME for
`game.player_state_equipment`, 2/2 SAME for weapon transform/power, and
1/1 SAME for the magic selector. The sourced horizontal mover `800274ec`
remains 61.9% WIP; its wider register/stack schedule residue supplies no
new source-backed correction.
The separate `8002722c` action selector initially withheld eight direct-J
rows. Its two bounded tables contain 25 raw pointers to twelve unique
in-body targets, and a CFG walk from those targets reaches all eight sites.
Every raw instruction decodes to its recorded in-body destination; the
conservative one-VA delinker accepted the eight reviewed rows and now
withholds zero relocations. The indirect case edges, C control flow, and
source owner remain unresolved.
KF1 `player_select_magic` in `../kings-field/src/game/player_death.c`
only clears charge and stores the selected magic record. KF2 `8002722c`
adds an unsigned MP-cost gate, status restrictions, two bounded switches,
and a charged-action state transition. The KF1 function supports the family
relationship but is not a source-equivalent body.

The GAME `8002985c` seed understated its body as `0x9dc` bytes and two
fragments. Raw code continues directly at `8002a238`: nine encoded branches
from the first part enter the tail, including `8002a204 -> 8002a238`, and
the tail restores the entry frame's saved `ra`/`s0`–`s2` before returning at
`8002a980`. The next function starts at `8002a988`. The curated body is now
one contiguous `0x112c`-byte extent, giving `kf sema cfg` 142 blocks and 315
edges. The first safe one-VA carve withheld 63 direct-control candidates
because the indirect jump at `80029c08` leaves automatic reachability
unresolved; no C source follows from the corrected extent.
That jump indexes `DAT_80011300` only for unsigned values 0 through 18. The
nineteen raw pointer words at `80011300..80011348` are aligned, all point into
this body, and end before the separate `8001134c` collision-dispatch table.
They are now one `0x4c`-byte address-derived table with reviewed pointer rows;
the raw `lui`/`addiu` base pair at `80029bf4/80029bf8` is also reviewed.
A CFG walk from the ten unique table targets reaches every one of the 63
withheld J/JAL sites. Each retail word decodes to its recorded target, and
the conservative delinker accepted the rows when reviewed: the focused
`0x112c` carve now withholds **zero** relocations. The indirect jump remains
an unresolved control-flow edge in `kf sema`; this review does not establish
the original C switch form or source TU owner.

GAME `800279cc` now has a first-pass source claim in the contiguous
`game.player_collision_sound` unit. Its 52 direct `lui`/signed-low references
to `player_state` and provisional collision-cache interiors were checked
against the retail GAME.EXE words and added in site order. The three-function
focused carve withholds no relocations. The source models the five observed
vertical modes, collision probes, landing damage, height adjustment, and view
bob; modes `0x10`, `0x20`, and early `0x40` paths go straight to the shared
finish, whereas grounded mode and the `0x50` landing response enter height
adjustment. Retail uses signed `lh` for player +`0x110` landing motion and
signed +`0x12e` movement-speed threshold. The `+0x110` field family is now
`s16[3]` without changing layout; its other source consumers only clear it.
The same `+0x12e` halfword is also read unsigned for view-bob phase, so the
shared field has a single layout-checked union with signed and unsigned views.
KF1 `player_update_vertical_motion` independently casts its movement speed to
`s16` at the step threshold; this is source-shape support, while the KF2 `lh`
and `lhu` instructions establish the two KF2 views.
The step-up test compares the new Y against Y minus vertical speed. Signed
speed capture in a promoted `s32` local before the falling-mode store emits
retail's direct `lh` at `80027df4`, rather than `lhu` with explicit sign
extension. The signed zero-height guard and retail short-rise branch
orientation bring the focused listing to **78.1% WIP** with 70/70 CFG blocks, 37/37 branches,
and 1/1 returns. The 0x48-byte retail frame, early camera-base register
assignment, and later schedule still differ; no exact claim is made.
Neighboring `80027928`/`80027988` remain SAME, as do all 16 reaction listings
and `player_reset_view` after the signed-field refinement. The two camera-turn
and fifteen player-state/equipment listings also remain SAME after the speed
union. Collision-cache ownership at `bss_801c7540+0x11800` remains
provisional. The separate map campaign observed a `0x1800`-byte shape-bank
copy ending there, which supports a boundary but does not establish the cache
or equipment extents on either side.

### Current player and weapon call-graph verdicts

This 30-function pass used focused listing comparisons; `SAME` below does
not itself certify strict objdiff 100%. Unclaimed bodies have no C score.

| GAME VA | Verdict | Current evidence |
| --- | --- | --- |
| `80024ed4` | SAME | Player pose control in the 16-function equipment unit. |
| `80024f4c` | SAME | Player reset control in the same unit. |
| `80025004` | SAME | Player initialization control. |
| `80025184` | SAME | Session initialization control. |
| `800251f0` | SAME | Motion clear control. |
| `80025234` | SAME | Map-position sync control. |
| `800252e4` | SAME | Point-cone distance control. |
| `800253ac` | SAME | Point distance control. |
| `800253fc` | SAME | Player byte setter. |
| `8002540c` | SAME | Player byte setter. |
| `80025434` | SAME | Player byte setter. |
| `8002545c` | SAME | Equipment slot control. |
| `8002569c` | SAME | Weapon equip control. |
| `80025754` | SAME | Attack begin control. |
| `80025878` | SAME | Actor-target selector control. |
| `80025a18` | DIFF, 82.9% | Variadic effect dispatcher covers the 53-entry switch; pointer/countdown cases, actor-index and rotation varargs, three-component actor position copy, and constant status-effect kinds follow reviewed retail call sites. Cases 4/51/52, 6/10, and 40 share one probe with distinct retail scales. Cases 39/49/50 now follow retail's distinct constructor signatures: case 39 passes the camera rotation as argument six, while cases 49/50 join case 7's five-argument tail. Cases 14/16/19 spell their literal status kinds in separate constructor calls; the candidate now reproduces the retail argument-preparation tail at matching object offsets. Retail and candidate each have 16 selector sites, 11 constructor sites, 108 relocations with identical kind/referent counts, 31 local jumps, 31 branch instructions, and a 0x918-byte body. All 53 switch rows have the same 31 destination groups, with 42 target offsets already byte-identical. Relocation order and earlier code still differ: the candidate places the shared rotation probe after case 11, while retail places it before. CFG remains 99/95 blocks with one unresolved switch jump on each side; stack frames are 112/104 bytes. |
| `80026330` | SAME | Weapon transform control. |
| `80026464` | SAME | Power/magic threshold control. |
| `80026498` | SAME | Magic selector control. |
| `8002665c` | DIFF, 91.1% | Weapon/magic update follows retail's field widths, phase-store order, configured-shot call argument, damage branch, countdown, and vector reuse; its 208-byte probe frame matches retail. The ordered 20-call sequence and 114/114 CFG blocks with 68/68 branches agree in the focused object. The candidate retains a `player_state+0x94` base across the sound call where retail rematerializes the attack-phase load and store, leaving two fewer address pairs but no distinct missing referent. The equipped ID is widened to signed `s32` for retail `slti`; keeping it as `u8` instead emits `sltiu`. |
| `8002722c` | DIFF, 89.3% | Both bounded switches and player/magic state are modeled; action case 10 falls through to retail's shared action-byte stores. CFG has 33/33 blocks and 16/16 branches. Both pointer tables have the same destination equivalence groups as retail (four in the six-entry table and eight in the nineteen-entry table), and all 82 function relocations agree in order. Its unexplained retail eight-byte leaf frame and original TU remain WIP. |
| `800274ec` | DIFF, 61.9% | Horizontal movement source; collision and retry schedule residue. CFG has 35/33 blocks and 20/20 branches; all 11 direct calls agree in order. The candidate hoists player-state bases across the routine, giving 42 versus retail's 66 relocations: exactly twelve fewer `player_state` HI/LO pairs, with no distinct referent omitted. |
| `80027928` | SAME | Collision-depth death helper control. |
| `80027988` | SAME | Landing-sound helper control. |
| `800279cc` | DIFF, 78.1% | 70/70 CFG blocks and 37/37 branches; all 12 direct calls agree in order. Promoted signed movement speed emits retail's direct `lh`. The candidate uses an earlier player-state field base for the landing response's `+0x110` load/store, leaving 127 versus retail's 131 relocations: two fewer HI/LO pairs, with no distinct referent missing. Frame and load-schedule residue remain. |
| `80027f78` | DIFF, 80.5% | Exhausted-length return follows retail's extra join block; 27/27 CFG blocks and 14/14 branches, with all six direct calls in retail order. The candidate rematerializes four extra `player_state` HI/LO pairs for reaction-motion fields, giving 59 versus retail's 51 relocations with no distinct referent missing. |
| `80028224` | SAME | Camera-turn control. |
| `8002851c` | SAME | Camera update control and horizontal-mover caller. |
| `8002897c` | DIFF, 28.6% | Seven-instruction signed interval predicate has v0/v1 assignment residue. Equivalent early-return, ternary, and conjunction spellings either leave that residue or change the retail CFG; the conjunction folds to a two-instruction unsigned range test. Exact `80028998` remains unchanged. |
| `80028998` | SAME | Attack/action controller and magic selector caller. |

For `80025a18`, the override-pointer load uses retail `lw 4(s0)` after
`s0 = sp + 112` and candidate `lw 0(s0)` after `s0 = sp + 108`. With the
respective 112- and 104-byte frames, both address the saved `a1` argument
slot at the entry stack pointer `+4`; the offset difference does not change ABI.

The adjacent `8002985c` update body has one proven external call from
`game_main_loop` at `800138e0`; the other incoming pointers are its bounded
internal switch targets. That caller does not prepare arguments or consume
`v0`, and the callee initializes `a0`–`a3` before using them. Its first-pass
`void func_8002985c(void)` source now covers the bounded 19-entry switch,
status timers, texture animation, and collision update, and focused `kf try`
reports **91.3% WIP**. The four VRAM source rows at `8006d6b0` are one
initialized `RECT[4]` owner; four raw-checked HI16/LO16 pairs identify its
interior row addresses. Another 182 raw-audited HI16/LO16 pairs resolve into
owned player, actor, map-object, callback, event, and effect state; two
`lui`/`ori` low-memory constants remain literal values. The movement-limit
source now follows the observed base-value stores, branch order, and the
bounded switch table's physical case-body order. The
case-1 reaction rotation copies the complete eight-byte `SVECTOR`, including
its last two bytes, as proved by paired unaligned word loads and stores; the
shared player-state union preserves all earlier component accesses and the
separate `player_reset_view` listing stays SAME. Case 0 and case 16 converge
on the observed pose tail, while the live reaction states converge on the
single view-update call. Case 4 now keeps the post-`rsin` angle phase in a
local for its store and zero check, following the retail register value chain.
The equipment tick path re-reads the equipped weapon record after the
possible HP-adjust call before testing its MP interval, as retail does.
The ordered 94 direct calls and all 482 function relocations agree with
retail. The 19-entry switch table has the same ten distinct destination
groups on each side. Focused CFG comparison has 142 blocks and 79 branches
on each side; both views retain one unresolved switch jump. The first remaining listing
differences are register choices and immediate-subtraction schedules; an
equivalent assignment spelling for the `-100` branch emitted the same
candidate instructions. The 112-byte frame and saved-register prologue
match retail, while several instruction schedules still diverge. This is a
started claim, not an exact match.
The `unknown_0e` reaction timer is correctly modeled as `u16`: retail uses
unsigned halfword loads for both adjustment paths at player `+0x0e`, then
signed comparisons after conversion. Its retail `-3300` store materializes
the negative full-width immediate, whereas the candidate materializes the
same low halfword as `0xf31c`; changing the field to signed would contradict
the retail loads.
The death state reads a signed `unknown_106` counter and uses the reaction
overlay's motion halfword at `+0x154`; the case-16 flag is its rotation
halfword at `+0x14e`. The height update compares the old `+0x134` value
with 1500 before adding 500, as shown by the branch delay slot, rather than
clamping the incremented value.
The case-16/18 stop checks cover the three signed rotation-delta halfwords,
without including the adjacent death counter. The effect-clear loop walks
128 records by pointer while counting down, and the death continuation
executes the teleport branch before the reset branch, matching the raw CFG.

The first-pass `8002722c` source now owns the contiguous run through
`800274ec` in `game.player_select_magic_action`, reducing one module. Its
two tables occupy one `RODATA(80011298, 0x64)` claim. Thirty-five direct
`lui`/signed-low pairs were checked against raw GAME words: 34 target
interior fields of the complete `player_state` object and one addresses
`effect_state.magic_records`. A two-VA safe carve with `8002722c` and
`800274ec` withholds zero relocations. Focused `kf try` reports **89.3%
WIP** for the action selector and preserves the horizontal mover's **61.9%
WIP** listing. The selector's 26-byte magic-row stride, unsigned MP cost,
signed equipment restrictions, six preliminary cases, nineteen action
cases, and final selected-record pointer are modeled in C. The retail
`+0x1e` charge gate reads and clears `magic_charge`; an initial source pass
incorrectly used `+0x1c` `attack_charge_committed` until the focused listing
exposed the field difference. Loading the unsigned MP cost before the
action-state stores reproduces retail's `a1` magic ID and `a2` record
lifetime. Ordering the second switch's case bodies by their retail block
sequence and clearing its three vector halfwords in retail store order
improves the listing while keeping the same case results. A shared
`repeat_count` local lowered similarity to 72.8% and was discarded. Retail
case 12 jumps from `80027460` to the common byte store at `800274c8`,
but its `li v0,5` delay slot at `80027464` sets the first action byte;
the resulting action pair is `(5,2)`, distinct from case 3's `(6,2)`.
That delay slot must be included when interpreting the apparent shared tail.
Retail case 10 also falls through from `80027420` to the action-byte stores at
`80027440`, shared with cases 1, 4–8, 11, and 18. Expressing that fallthrough
in C removes a duplicate compiled block and raises focused similarity from
88.0% to 89.3%, while the neighboring horizontal-movement listing stays at
61.9%. Retail and C still differ at the unexplained eight-byte leaf frame and
several branch offsets. The raw table pointers prove
bounded destinations, but semantic indirect edges and the original C/TU
form remain candidate.

### Player reaction helper and update slice

The next related ten-function slice is `80029428`, `80029464`, `800294f8`,
`80029570`, `800295f8`, `80029624`, `800296e8`, `8002975c`, `800297b4`,
and `8002985c`. These adjacent reaction-state setters, phase ramp, vital
adjusters, equipment tick helper, and main update share proven player-state
references; the main update directly calls the phase ramp, vital helpers, and
equipment helper. A fresh focused `kf try --unit game.player_reaction`
reports `SAME` for the first nine and **91.3% listing similarity** for
`8002985c`. Existing strict match records mark the nine controls
`100.000000000% exact`; explicit retail-versus-current-scratch objdiff
confirms the update at **97.582344% strict WIP**. The project-configured
base object and semantic match report were stale; the focused listing and
explicit object comparison are the current source evidence.

The retail `80029624` phase ramp reads both counters as unsigned halfwords,
sign-extends their working values, uses a shared scaled result, and returns
`-1` when both phases are zero. Its source and focused listing remain exact;
retail `8002985c` calls it three times at `800298a8`, `800298e4`, and
`8002a5b4`. The large update has the same 94 ordered direct calls, 482
ordered function relocations, 19-entry switch destination grouping, and
112-byte frame as retail. Its fresh CFG comparison gives 142/142 blocks,
79/79 branches, and 2/2 known return-frontier edges. Both sides have one
unresolved indirect switch jump, so equal known successor lists do not prove
complete semantic equivalence. The first differing control is branch 56:
retail uses `sltiu v1,v1,16; bne v1,zero`, whereas the candidate uses
`sltiu s0,s0,16; bne s0,zero`, with the same destination. Earlier listing
residue includes the clamped `4096` value held in `v0` by retail versus
`a3` in the candidate, and immediate-subtraction scheduling. The C source
already expresses the observed clamp, widths, calls, and branch conditions;
no source-backed correction was retained in this slice.

### Player collision bounds and core-state slice

The next ten-function slice is `80023384`, `80023430`, `80023484`,
`80023570`, `8002360c`, `80023814`, `80023868`, `80023984`, `80024034`,
and `800240cc`. Fresh focused comparisons show the nine functions after
`80023384` as `SAME`; existing strict records mark each at 100% exact.
The bounds helper remains **63.2% listing similarity** and direct one-function
objdiff confirms **76.23256% strict WIP**, with 5/5 CFG blocks, 2/2 branches,
2/2 known return-frontier edges, and two ordered `player_death_begin(NULL)`
calls. Retail separately loads player halfword fields `+0x134` and `+0x138`
and the camera Y word `+0xdc` in both bounds arms. The candidate keeps one
`player_state+0x134` base in `s0` across both arms, changing the frame and
removing repeated HI16/LO16 pairs; its conditions and observed referents
otherwise agree. The cache words at BSS `+0x11818`/`+0x1181c` now use shared
lower/upper-bound views in source. A focused rebuild kept the 63.2% listing,
and the complete cache/equipment boundary remains provisional.

### Player damage and early equipment control slice

A disjoint ten-function slice covering `80024164`, `80024384`, `80024448`,
`80024498`, `800247e4`, `800248a8`, `80024ca4`, `80024ed4`, `80024f4c`,
and `80025004` remains **10/10 exact** in strict per-function records.
Fresh focused comparisons of the owning core, damage-reaction, status-cap,
apply-damage, radial-damage, and equipment units report `SAME` for every
selected function. Direct one-function objdiff independently confirms
`80024498` and `800248a8` at 100%; these own the eight- and seven-entry
bounded damage switch tables respectively. The equipment unit's separate
`80025a18` dispatcher remains WIP, but its neighboring selected controls
are unchanged. No C correction was needed in this slice.

### Player equipment selector and dispatcher slice

The next disjoint ten-function slice comprises `80025184`, `800251f0`,
`80025234`, `800252e4`, `800253ac`, `800253fc`, `8002540c`, `80025434`,
`8002545c`, and `80025a18`. The first nine remain `SAME` in a fresh
focused equipment-unit comparison and have exact strict records. A direct
target-versus-fresh-scratch objdiff gives the dispatcher **95.54467% strict
WIP** while its normalized focused listing remains **82.9% similar**;
the semantic navigator's older 70.391754% row is stale. Direct objdiff
also confirms the adjacent target-selector control `80025878` at 100%.

Of the dispatcher's 53 switch rows, 42 have byte-identical destination
offsets. The other eleven are cases 1, 2, 3, 5, 6, 7, 8, 9, 10, 11, and
12; all rows from case 13 onward are byte-identical, as is case 0. Retail
places a shared `func_80025878` selector immediately after the cases 51/52
effect-ID store at body `+0x3b0`; case 4 enters at `+0x3b4`, and cases 6/10
jump into the selector's argument setup at `+0x3b8`. The candidate moves
that selector to a later shared tail; its switch grouping and ordered
call/referent sets remain correct. A bounded
scratch probe spelling literal selector calls for cases 4/6/10/40 produced
the same compiled object as the current shared-call source. The earlier
case-order probe moved this selector but lowered focused similarity and was
discarded. No source-backed C correction was retained.

### Player weapon, magic, and horizontal-movement slice

The next disjoint ten-function slice is `8002569c`, `80025754`, `80025878`,
`80026330`, `80026464`, `80026498`, `8002665c`, `8002722c`, `800274ec`,
and `80027928`. The seven controls remain `SAME` in fresh focused unit
comparisons and have 100% strict records. Fresh direct target-versus-scratch
objdiff gives `8002665c` **98.67857% strict WIP** (`91.1%` normalized
listing), `8002722c` **99.09091% strict WIP** (`89.3%` listing), and
`800274ec` **88.631% strict WIP** (`61.9%` listing). Older semantic match
rows for the first two WIPs are stale and must not replace these direct
comparisons.

The magic/weapon updater's first mismatches choose different registers for
the same equipped-ID and attack-phase loads; later the candidate keeps a
player-state base across the sound call where retail reloads it. Its 20
ordered calls and 114/114 CFG blocks still agree. The action selector's
four-byte extent gap reflects retail's otherwise-unused eight-byte leaf frame;
its 82 ordered relocations, bounded switch destination groups, 33/33 CFG blocks,
and 16/16 branches agree. In the horizontal mover, retail individually
forms camera-position addresses when accepting a position, whereas the
candidate reuses a player-state base and fills a jump delay slot with the
cache-layer store. It retains 11 ordered calls and 20/20 branches, but its
CFG still has 33 blocks against retail's 35. These observed source-equivalent
differences do not justify artificial locals or redundant loads, so no C
correction was retained.

### Player reaction-state caller and helper slice

Another related ten are `80029014`, `80029168`, `800291d0`, `800291ec`,
`800293d4`, `80029428`, `80029464`, `800294f8`, `80029570`, and the
update at `8002985c`. Nine helper listings remain `SAME` in a fresh focused
reaction-unit build, and their existing strict records are 100%. An
explicit retail-versus-current-scratch comparison confirms `800291ec`
at 100% and `8002985c` at **97.582344% strict WIP**. The update retains
91.3% normalized listing similarity, 142/142 known CFG blocks and 79/79
branches, with one unresolved switch jump on both sides. Its first
differences remain the `4096` clamp value held in different registers and
the `-100`/`-800` immediate-subtraction schedule; no source-backed change
was retained.

### Player collision, camera, and reaction control slice

The next disjoint ten-function slice is `80027988`, `800279cc`, `80027f78`,
`80028224`, `8002851c`, `8002897c`, `80028998`, `80028ec0`, `80028fa8`,
and `80029014`. Seven controls remain `SAME` in fresh focused comparisons
and have 100% strict records. Direct target-versus-fresh-scratch objdiff
gives the three WIPs `800279cc` **97.09642% strict** (`78.1%` normalized
listing), `80027f78` **95.91228% strict** (`80.5%` listing), and `8002897c`
**88.57143% strict** (`28.6%` listing). The first two semantic match rows
are stale; these direct object scores are current.

The landing and collision responses retain their retail ordered calls and
known CFG block/branch counts. Their remaining referent differences are
repeated player-state HI16/LO16 pairs: the landing candidate emits two fewer,
while the response candidate emits four extra pairs. The seven-instruction
signed `[71,80]` predicate differs only in `v0`/`v1` assignment and its
return delay slot; a natural nested-if scratch spelling emitted the same
candidate instructions as the current source and left exact neighbor
`80028998` unchanged. No tracked source correction was retained.

### Transfer review of merged KF1 source cleanup

The KF1 `master` history, rather than its dirty working tree, is the source
for this review. Commit `40cf67ca` replaced fallbacks that did no work before
return with direct returns; `ee562ab6` kept required joins while structuring
player fallback paths. Commit `a977131b` replaced fabricated object-owner
casts and manual variadic handling with supported typed views and APIs,
respectively. The broad goto-removal commit `d2c9b27c` is not an ancestor of
KF1 `master`, so it is not a merged precedent.

KF2's player gotos here enter shared landing, movement, effect, reaction,
or weapon-attack tails. In particular, `80027f78` needs the `exhausted`
return join, `800279cc` needs the common `finish` tail, and `80025a18`
shares switch-case selector and emission paths. These are not the simple
return-only fallbacks changed in KF1; deleting the joins would make the
retail CFG less faithful.

Two duplicate collision-cache views were removed instead. `80027928` now
uses the shared result word at BSS `+0x11810`; direct strict comparison keeps
`80027928` and `80027988` at 100%, and leaves `800279cc` at 97.09642%.
The height-limit word at BSS `+0x11814` has a reviewed retail load in
`800274ec` and stores in the collision dispatcher, so the player mover now
uses a shared view. Its direct strict score remains 88.631%, and adjacent
`8002722c` remains 99.09091%. The complete cache/equipment object boundary
is still provisional.

`80025a18` already has the supported `s32 effect_id, ...` definition: callers
pass one or two arguments, and the retail identity records the variadic ABI.
Three player callers now include its shared prototype instead of private
old-style declarations. Focused builds and direct strict comparisons kept
the exact caller controls `80026498`, `80028998`, and `800291ec` at 100%; the
dispatcher and neighboring WIPs retain their previous scores.

The exact damage core `800248a8` also has a complete 12-argument signature
backed by its definition, callers, and identity row. Its radial-damage caller
now uses that shared prototype instead of a generic local declaration.
Fresh direct strict comparisons keep both `800248a8` and caller `80024ca4`
at 100%.
The neighboring exact damage reaction `80024498` likewise has a shared
three-argument prototype now used by its damage and landing callers. Direct
strict comparisons keep `80024498`, `800248a8`, `80027928`, and `80027988`
at 100%; the landing WIP remains 97.09642%.
The collision-bounds helper `80023384` is now declared once in `player.h`
for its equipment and collision-response callers. Focused listings and
direct strict comparisons are unchanged: the helper is 76.23256% WIP,
the response `80027f78` is 95.91228% WIP, and the adjacent equipment and
landing controls remain exact.
The equipment and reaction units also use the existing shared map-cell
declaration of `func_8002b73c` rather than private duplicate prototypes;
direct strict comparisons keep `80025878` and `800291ec` exact and leave
`80025a18` and `8002985c` at their recorded WIP scores.

### Fresh reaction phase-helper correction

The older `80029624` WIP rows above are stale. A direct
retail-versus-current-scratch objdiff of `player_reaction.c` now reports
**100% strict**, with all 49 instructions identical. The focused reaction
unit lists it `SAME`; only `8002985c` remains non-exact in that unit.
This correction records the current source state rather than attributing
the match to the declaration cleanup above.

### Twenty-function player call-graph comparison

Fresh direct retail-versus-current-scratch comparisons cover nine non-exact
player bodies and eleven related exact caller/callee controls. The control
set anchors the collision, damage, equipment, magic, and reaction paths;
all addresses below belong to GAME.EXE. No row was closed by loose display
similarity.

| VA | Strict verdict | First remaining difference or control role |
| --- | --- | --- |
| `80023384` | 76.23256% WIP | Separate retail player-height address pairs versus a candidate base pointer; 5/5 CFG blocks. |
| `80023430` | 100% exact | Distance-margin caller control. |
| `80024498` | 100% exact | Damage reaction callee control. |
| `800248a8` | 100% exact | Twelve-argument damage dispatcher control. |
| `80024ca4` | 100% exact | Radial-damage caller control. |
| `80025878` | 100% exact | Effect target-selector callee control. |
| `80025a18` | 95.54811% WIP | The reviewed case 10/6 order is restored; eleven switch rows still differ in destination offset because retail's shared selector is earlier. Call and referent sets agree. |
| `80026498` | 100% exact | Magic dispatcher caller control. |
| `8002665c` | 98.67857% WIP | First difference is equipped-ID register assignment; later candidate reuses an attack-phase base across a call. |
| `8002722c` | 99.09091% WIP | Retail has an otherwise-unused eight-byte leaf frame; switch groups and 82 ordered relocations agree. |
| `800274ec` | 88.631% WIP | Candidate hoists player-position base; 33 CFG blocks versus retail's 35, with 20/20 branches and 11 ordered calls. |
| `80027928` | 100% exact | Collision/death threshold callee control. |
| `80027988` | 100% exact | Landing sound callee control. |
| `800279cc` | 97.09642% WIP | Two fewer player-state address pairs; 70/70 CFG blocks and 12 ordered calls agree. |
| `80027f78` | 95.91228% WIP | Four extra player-state address pairs; 27/27 CFG blocks and six ordered calls agree. |
| `8002897c` | 88.57143% WIP | Seven-instruction interval predicate keeps retail signed conditions but assigns `v0`/`v1` differently. |
| `80028998` | 100% exact | Charge and attack caller control. |
| `800291ec` | 100% exact | Reaction object helper control. |
| `80029624` | 100% exact | Phase/ramp helper; older WIP records are stale. |
| `8002985c` | 97.582344% WIP | First difference is clamp register assignment, then immediate-subtraction schedule; 142/142 known CFG blocks agree. |

The only current referent-count differences in this set repeat the same
reviewed player-state target at different instruction sites. The call sets
are complete where compared. Existing C types, branch meanings, and data
owners are supported by retail and caller evidence; no source change is
justified solely by these register, frame, and address-materialization
residues.

The `8002897c` interval predicate remains WIP: retail tests signed `<81`,
returns zero on failure, then inverts signed `<71`; two proven callers pass
item identifiers. A direct-return spelling of those same bounds introduced
an extra branch and jump absent from retail, despite a higher display-only
listing similarity; exact caller `80028998` stayed identical. The experiment
was discarded, and the original seven-instruction candidate was restored.
A scratch GCC 2.6.0 compile leaves the predicate at the same 88.57143%
strict score while regressing exact `80028998` to 95.36364%; it is not a
closure for this contiguous unit.
The same alternate profile leaves `8002722c` frameless rather than emitting
retail's eight-byte frame; its strict score falls from 99.09091% to
92.58523%, and adjacent horizontal movement falls to 74.38745%. This
does not explain the selector's remaining frame residue.

KF1's exact `player_update_vertical_motion` and `magic_cast` sources
confirm broad falling-state and effect-dispatch roles, but their map-grid
collision path and smaller magic set differ from KF2 retail. KF1
`player_move_horizontal` is itself a 96.56896% WIP with a heading-register
and repeated-address-materialization residue, so it cannot establish an
exact source form for the analogous KF2 residue. The KF1 matcher dossier
reports that direct global accesses and several typed pointer placements
all left that player-state anchor unresolved after
the SDK `SVECTOR` and signed bearing facts were restored; KF2 already uses
the corresponding supported vector and heading types.
KF1's exact 0x264-byte weapon-attack updater has the same phase and charge
role as KF2 `8002665c`, but KF2's 0xbd0-byte body adds special-weapon
modes and effect-record motion; its opening equipped-ID `lbu` already has
the right width and only a register-assignment residue.
KF1's exact `player_select_magic` at `800167e4` is only a 0x64-byte
selection setter: it clears charge, stores the chosen ID, and chooses or
clears the record pointer. KF2 `8002722c` is a 0x2c0-byte magic-action gate
with two switches, MP checks, and action-vector stores. The shared subject
does not make the KF1 source an expression or stack-frame template for the
KF2 leaf-frame residue. KF2's tail stores the already computed record pointer
from `a2` directly to `player_state+0x7c` at `800274dc`; it does not reload
the newly stored magic ID as the KF1 setter does.
KF2 raw `80027684`–`800276b0` loads unsigned X/Z halfwords from the
collision cache and player state, subtracts cache minus player on both
axes, stores signed halves, and passes them to `vector_xz_to_angle`.
KF1's analogous wall bearing subtracts Z in the opposite direction, so
that exact-source expression must not be copied into KF2.
The KF2 vertical path also uses both signed `lh` at `80027df4` and unsigned
`lhu` at `80027f00` for the same movement-speed halfword at
`player_state+0x12e`. Its current signed/unsigned union views preserve that
retail distinction; KF1's exact vertical-motion source independently casts
the corresponding speed to `s16` at its step threshold.

Fresh focused KF2 controls keep `80027928`/`80027988` at 100%, all sixteen
exact reaction siblings unchanged, and the WIP functions `80023384`,
`8002722c`, `800274ec`, `800279cc`, `80027f78`, and `8002985c` at their
existing verdicts.

The retail effect dispatcher homes `a0`-`a3` before allocating its frame.
It reloads `effect_id` from that home slot before the switch and uses a
`0x70` frame. The GCC 2.5.7 candidate also homes those four registers but
keeps `a0` live for the switch and uses `0x68`. Retail's
selector distance addresses are `sp+0x50/+0x54`, eight bytes above the
candidate's `sp+0x48/+0x4c` slots. No observed stack access identifies a
real omitted local in the gap, so no artificial storage was added.
The optional position argument is correct on both sides despite different
cursor displacements: retail anchors `s0` at its `sp+112` first-argument
home and loads `4(s0)`; the candidate anchors `s0` at its `sp+108`
second-argument home and loads `0(s0)`. Both read caller register `a1`.
A source-only probe replacing the repository's `stdarg.h` with the pinned
Psy-Q 3.0 `STDARG.H` kept the same `0x68` frame, `a1`-home cursor, and
83.2% focused listing, with all 15 exact equipment siblings unchanged.
The different macro expansions are canonicalized by this compiler here;
they do not explain the retail frame or switch placement.
An isolated GCC 2.6.0 `-O2 -mcpu=r2000` compile of this unit likewise
homes `a0`-`a3`, but keeps a `0x68` frame, omits the
retail reload, and scores `80025a18` only 56.620274% strict. It also
retains only six of the fifteen exact equipment siblings. The established
GCC 2.5.7 source/profile remains the better candidate; the mixed entry
evidence does not establish a historical compiler or license a fake local.

An isolated source probe gave the reaction update's `4096` clamp a distinct
`s16` local instead of reusing the prior phase result. That natural spelling
kept exact `800291ec` and `80029624` at 100% but lowered `8002985c` from
97.582344% to 97.482254%; it was not retained.

A second scratch probe moved only effect-dispatch cases 6 and 10 ahead of
cases 51/52/4, where retail places their shared selector. It preserved
exact `80025878` and `player_begin_weapon_attack` but lowered strict
`80025a18` from 95.54467% to 94.40206%. Its switch layout is therefore
not explained by that case order alone; the tracked source was unchanged.

## Player BSS source owners

The native GAME link had unresolved player-family globals even though their
retail addresses and consumers were curated. `player_core_run.c` now defines
`player_level_growth_table` at `800758f0` (100 twelve-byte rows, `0x4b0`)
and `player_state` at `801984d0` (`0x160`). The resource startup copy and
experience/initialization functions address the growth rows. Retail
`game_main_loop` clears `player_state` with exactly `0x58` word stores,
ending at `80198630`, whose next word still has no proved owner.

`player_state_equipment.c` now defines `player_weapon_records` at
`801c7078` (18 records of `0x44` bytes) and `bss_801c7540` at `801c7540`
(`0x11844` bytes). The first range ends exactly at the second; retail
magic dispatch bounds the weapon ID below 18, and resource startup copies
records into its base. Retail `game_main_loop` clears the second range with
`0x4611` word stores. Their original translation-unit boundary and some
interior field meanings remain WIP; the definitions do not assert those.
The session weapon asset buffer was already defined in the equipment unit;
its `0xc000` extent remains a candidate inferred from the KF1 counterpart.

Focused `kf try` retained all ten `player_core_run` listings and all 15
previously exact `player_state_equipment` listings. Direct strict comparison
of the core unit retained 10/10 function scores at 100%; the equipment
effect dispatcher `80025a18` remains WIP. The pinned compiler emits the
new tentative globals as COMMON, while the current retail target carves
fixed `.bss` ranges. This data-class residue is unresolved and does not
change the exact function verdicts.

The remaining player loaded-data referent `DAT_800667e8` is now one
initialized `KfPlayerMagicIdSequence` in `player_state_equipment.c`, directly
after its two effect-vector arrays. Its twelve effect bytes are
`39,40,60,66,84,86` repeated twice; its four little-endian attack masks
are `0x20,0x10,0x80,0xffff`. The retail payload and three validated
references in the magic dispatcher/input controller support the global
identity. The source is non-const because the pinned compiler otherwise
puts the table into `.rodata` and shifts the exact equipment switch-table
referent; the retail data is contiguous with the two initialized player
effect arrays. A target-only GAME refresh and focused direct objdiff show
the new datum at 100%, the three-array `.data` extent at 100% (`0x5c` bytes),
all 15 previously exact equipment functions still at 100%, and the effect
dispatcher `80025a18` still WIP. Exact magic/input consumer
listings remain unchanged.

The `80025a18` switch table also proves the physical order of its short
rotation cases: retail case 10 targets `.text+0x1070`, immediately before
case 6 at `+0x1078`. The prior C case order emitted case 6 first. Swapping
these adjacent cases restores that order without changing semantics; both
candidate targets now sit 24 bytes before retail, the same upstream offset
as nearby cases. Direct strict `80025a18` rises from 95.54467% to
95.54811%; the 15 exact equipment siblings remain at 100%. The upstream
24-byte difference remains unexplained. Retail's shared `func_80025878`
call is at `80025dd8`; case 10 at `80025f44`, case 6 at `80025f4c`, and
case 40 at `80026174` all jump back to its `80025dd0` argument setup.
The current C already expresses those cases through one labelled call, so
moving or duplicating it without an independently supported source shape
would only steer the compiler's placement.

The reviewed 53-word switch table identifies case 44 at `80011188+0xb0`
and case 45 at `+0xb4`. Retail case 44 sets effect ID `0x75` at
`80026148` and jumps through `80025de0` to `80026220`, which writes the
camera-rotation pointer as the constructor's sixth argument. Case 45 sets
`0x74` at `80026168` and jumps through `80025a8c` directly to `8002622c`,
skipping that sixth-argument store. The C previously sent both cases to
the rotation path; case 45 now joins the five-argument simple-effect path.
The constructor corroborates the distinction: retail kind 116 at
`80040f98` returns without reading a variadic argument, whereas kind 117
at `80040fa8` reaches the caller-supplied rotation-vector copy.
Focused comparison preserves all 15 exact equipment siblings. The dispatcher
remains WIP at 95.54467% strict, slightly below the prior 95.54811% score;
the corrected argument count is independently established by the raw call
path and overrides that transient similarity change.

The consolidated native GAME link no longer lists `DAT_800667e8` among its
unresolved names. A scratch placement of `va_start` only in cases 39/49/50
removed the early named-argument home store and lowered focused similarity;
the tracked dispatcher retains the original top-level `va_start`.

## Actor runtime BSS link owner

`actor_pool_clear.c` now defines the current `actor_state` BSS owner at
`8016b600`. Retail `game_main_loop` passes exactly `0x24f3` words to the
startup clear, bounding `0x93cc` bytes. The actor pool has 200 records at
`0x7c` bytes each, the target groups begin at `+0x60e0`, and the reviewed
runtime trailer ends at `+0x93cc`. The original translation-unit boundary
remains WIP. Focused comparison keeps `actor_pool_clear` at strict 100%.
The pinned compile/assemble chain emits this tentative definition as a
37,840-byte COMMON symbol (`st_size=0x93d0`); the retail target currently
carves 37,836 fixed `.bss` bytes (`0x93cc`). The four-byte difference equals
the unclaimed gap before `map_object_state` at `801749d0`, but no retail
access proves that gap belonged to the actor allocation. The source can
resolve the native symbol but is not an exact data-allocation match.
The saved compiler assembly itself describes the struct as 37,836 bytes in
its debug `.def` and then emits `.comm actor_state,37840`; this allocation
rounding occurs before ASPSX and does not justify changing the C type.

## Effect motion vector beside player weapon records

The remaining BSS name `DAT_801c7068` has eight validated incoming
references, all from `func_80042298` and `func_80042424`. Retail reads and
writes signed halfwords at `+0`, `+2`, and `+4`; it also passes the base to
`copyVector`. A provisional eight-byte `SVECTOR` definition now lives with
those consumers in `effect_scatter.c`. The next curated allocation,
`player_weapon_records`, begins at `801c7078`, leaving eight bytes after
the vector with no identified consumer. The current source does not claim
that intervening space or an original TU boundary.

Focused `kf try` kept all three effect-scatter listings identical; direct
strict objdiff reports 100% for `80042298`, `80042424`, and `800424f0`.
The candidate symbol is an eight-byte COMMON allocation, so its final
placement and the retail BSS class remain open even if the native link
resolves the name.

## Player collision and damage strict controls

A direct isolated objdiff of 14 related GAME units covers 29 functions:
23 are strict exact and six remain WIP. The exact set includes all ten
`player_core_run` functions, both camera-turn functions, the distance-margin
and reset-view helpers, the collision helpers at `80027928`, `80027988`, and
`80028998`, and six damage, status, and weapon-power functions at
`80024498`, `800247e4`, `800248a8`, `80024ca4`, `80026330`, and `80026464`.
The WIPs are `80023384` (76.23256%), `8002722c` (99.09091%),
`800274ec` (88.631%), `800279cc` (97.09642%), `80027f78` (95.91228%),
and `8002897c` (88.57143%). These are strict text comparisons, separate
from the focused listing percentages above.

Retail `80023384` reads the lower and upper collision-cache bounds and
repeats signed player `+0x134`, word camera Y `+0xdc`, and signed `+0x138`
loads in each arm before the two death calls. The current C preserves those
fields, arithmetic, branches, and call sites, but the probe compiler reuses
one saved player-state address and emits fewer address pairs. The other
large collision WIPs retain their ordered calls and known referents; no
source-backed change was accepted in this strict pass. Three isolated
natural spellings of the `8002897c` interval predicate either kept its
v0/v1 scheduling residue or changed the retail CFG, so the source remains
unchanged. The adjacent `game.actor_player_damage` focused control is
`8003a778` SAME; `8003a318` differs in argument-register assignment, while
`8003a614` retains the correct call/branch set but reuses the camera-position
base where retail rematerializes the Z-field address.

A fresh focused revisit of `800274ec` still reports 61.9% listing similarity,
35 retail versus 33 compiled CFG blocks, 20/20 branches, three return-frontier
edges, and the same eleven ordered calls. Retail forms separate absolute
player camera-field addresses in the main loop and builds a camera-position
pointer inside the slide loop; the probe keeps one camera base live across
both. A source-only typed pointer scoped to the slide loop changed the
register and retry-join layout without closing the block gap, so it was
discarded. The existing collision, retry, and signed-halfword output model
remains WIP at the prior 88.631% direct strict verdict.

The two adjacent GAME player-damage switch tables now have function-specific
curated identities. At `0x80011128`, retail `func_80024498` masks its selector
with 7 and bounds it to eight entries before the indirect jump; all eight raw
words are distinct aligned pointers inside that function. Its source claims
the complete 0x20-byte RODATA range. At `0x80011148`, retail `func_800248a8`
subtracts one, bounds the result to `0..6`, and jumps through seven distinct
aligned words, all inside its body; its source claims the complete 0x1c-byte
range. The identities are `func_80024498_rodata` and
`func_800248a8_rodata`, respectively. These owners do not establish the
original TU boundary.

Both safe one-VA delinks withheld zero relocations or functions (98 and 169
total relocations). Focused listings remain SAME. Isolated strict objdiff
rechecks 100% text and RODATA for both: 844/32 bytes for `24498` and 1020/28
bytes for `248a8`. Target and candidate relocation listings agree in order:
45 text plus eight RODATA entries for the first unit, and 81 text plus seven
RODATA entries for the second. This cleanup preserves the existing exact
verdicts and does not change the match count.

The `player_state_equipment` unit's `0x80011168..0x8001125b` RODATA is one
0xf4-byte claim, so its inventory now has one `u32[61]` owner rather than
separate identities for two interior tables. Raw words `0..6` are seven
distinct aligned pointers into exact `player_set_equipment_slot`; word 7 is
zero; words `8..60` are 53 aligned pointers to 31 labels inside
`func_80025a18`. Retail bounds the latter selector to `0..0x34` at
`0x80025a48..0x80025a70`. The second table begins at owner `+0x20`, and
the source already claims the entire span. No function body or pointer
target was changed.

A focused full-unit safe carve reports 17 objects, 1044 relocations, and
zero withholding. All 15 exact sibling listings remain SAME; `25a18`
remains 83.2% focused DIFF with 99/96 retail/compiled CFG blocks and 31/31
branches. The isolated complete-unit object has 100% `.data` (92 bytes),
98.01535% `.text` (5212 retail bytes), and 87.6033% `.rodata` (244 bytes).
The first 32 RODATA bytes, including the exact first table and zero
separator, are identical. All 60 table-pointer relocations occur at the
same ordered offsets in target and candidate; the remaining RODATA byte
difference follows the WIP second switch's in-body label offsets. This
ownership cleanup makes no new exact claim.

Two further single-function RODATA spans now have supported owner identities.
`func_8002722c_rodata` at GAME `0x80011298..0x800112fb` contains two
adjacent jump tables: retail bounds a six-entry index to `0..5` at
`0x800272e0..0x80027304`, then bounds a second 19-entry index to `0..18`
at `0x800273f4..0x80027418` and uses base `+0x18`. All 25 raw words are
aligned targets inside `func_8002722c`, with four and eight distinct case
labels respectively. `func_8002985c_rodata` at `0x80011300..0x8001134b`
has 19 raw aligned pointers to ten in-body labels; retail bounds its index
to `0..18` before the indirect jump at `0x80029c08`. Both sources already
claim the complete RODATA ranges, and these function owners do not prove
their original TU boundaries.

Safe one-VA delinks report 82 and 482 relocations, with zero withheld for
either unit. Focused listings are unchanged after the identity corrections:
`2722c` remains 89.3% DIFF, its `274ec` sibling 61.9% DIFF; sixteen exact
siblings in `game.player_reaction` remain SAME while `2985c` remains 91.3%
DIFF. No function body, retail pointer target, or match count changed.

The 13-word `player_magic_dispatch_rodata` at GAME `0x80011260..0x80011293`
is a bounded switch table for exact `func_80026498`: retail subtracts 38 from
the magic ID, rejects indices beyond 12 at `0x8002652c..0x80026530`, then
loads this table and jumps indirectly at `0x80026550`. Every raw word is an
aligned address inside the 0x1c4-byte function, so the 13 candidate pointer
rows are now reviewed without altering targets or source. A focused safe
carve of the two-function unit emits three objects and 525 relocations with
none withheld; the unit's 52-byte RODATA is strict 100%, and all 13 ordered
pointer relocation types, offsets, and addends match. `func_80026498` remains
SAME, while the `func_8002665c` caller remains 91.1% focused DIFF. The
complete unit text is 98.8504%, so no new exact claim is made.

The first seven words of `player_state_equipment_rodata` are also reviewed
pointer rows now. Retail bounds the equipment slot to `0..6` at
`0x80025464..0x8002546c` and dispatches through `0x80011168` at
`0x8002548c`; the seven raw words target seven aligned case blocks inside
`player_set_equipment_slot`. The same complete-unit safe carve still emits
17 objects and 1044 relocations with none withheld, because it already
accepted these pointer-shaped rows. All 15 exact siblings remain SAME,
`func_80025a18` remains WIP, and isolated objdiff confirms the seven ordered
RODATA relocation entries match. This review changes evidence status only.
