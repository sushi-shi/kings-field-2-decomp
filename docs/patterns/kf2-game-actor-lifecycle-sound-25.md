# GAME actor lifecycle and target sound: 25 current verdicts

This connected actor-state/target graph was rebuilt with focused `kf try`
under each unit's manifest profile. Twenty claims remain strict exact and five
remain WIP; these are current controls, not new byte-match closures. The
lifecycle update calls target selection and actor preparation, the behavior
dispatcher calls the spatial-sound helper, and the other helpers share the
typed actor/target state.

| GAME VA | Function | Strict text | Verdict |
| --- | --- | ---: | --- |
| `80038cc8` | `actor_pool_find_free` | 100% | Exact pool control. |
| `80038d04` | `actor_set_home_position` | 100% | Exact placement control. |
| `80038dc4` | `actor_copy_group_defaults` | 100% | Exact group copy. |
| `80038e38` | `actor_initialize_from_group` | 100% | Exact lifecycle initialization. |
| `80038efc` | `actor_set_lifecycle_and_home_position` | 100% | Exact wrapper. |
| `80038ff0` | `actor_prepare_and_initialize` | 100% | Exact preparation. |
| `80039048` | `func_80039048` | 100% | Exact indexed preparation. |
| `80039080` | `actor_pool_clear` | 100% | Exact actor-state reset. |
| `800390d0` | `actor_set_target` | 100% | Exact target setter. |
| `80039108` | `func_80039108` | 93.93092% | WIP scorer; the case-9 branch merge and table addends remain bounded. |
| `800395c8` | `actor_select_best_target` | 100% | Exact scorer caller. |
| `800396c4` | `actor_select_target_for_player_distance` | 100% | Exact distance wrapper. |
| `80039710` | `actor_find_target_of_type` | 100% | Exact typed-target scan. |
| `80039758` | `actor_select_target_type_in_own_group` | 100% | Exact group target selection. |
| `800397a8` | `actor_reset_target_and_reselect` | 100% | Exact target reset. |
| `8003983c` | `func_8003983c` | 99.19598% | WIP lifecycle update; 37/37 CFG and 24/24 branches, saved-register residue. |
| `80039b58` | `func_80039b58` | 90.95744% | WIP 200-actor group scan; 9/9 CFG and 4/4 branches, free-slot byte lifetime. |
| `8003ae20` | `actor_animation_crossed_phase` | 100% | Exact behavior sound-trigger control. |
| `8003c000` | `func_8003c000` | 100% | Exact group-position sibling. |
| `8003c10c` | `func_8003c10c` | 100% | Exact group-position sibling. |
| `8003c220` | `func_8003c220` | 100% | Exact group-position sibling. |
| `8003c3e0` | `func_8003c3e0` | 99.64539% | WIP yaw-intermediate register assignment; 23/23 CFG. |
| `8003d084` | `func_8003d084` | 100% | Exact sound-pitch helper. |
| `8003d0e8` | `func_8003d0e8` | 100% | Exact spatial-sound dispatch. |
| `8003d184` | `func_8003d184` | 99.931595% | WIP behavior dispatch, with exact 241-row RODATA. |

## Lifecycle state names

KF1's recovered actor lifecycle has states 0 dormant, 1 active, 2 waiting
for range exit, and 3 disabled. KF2 raw `0x80038e38` stores 1 into actor
byte `+0x09` during initialization; raw `0x80038efc` stores 3 there before
setting the home position. In KF2 `0x8003983c`, state 0 may activate or enter
state 2; states 1 and 2 return to 0 when the actor exits the group range.
These matching state transitions support the same four semantic constants in
the KF2 actor header. The lifecycle update, group scan, initialization, and
home wrapper now use them without changing the represented values.

Fresh focused rebuilds kept initialization `SAME` and both home-wrapper
functions `SAME`. Safe GAME delinking of the two lifecycle WIPs accepted 52
relocations with none withheld; isolated strict text stayed 99.19598% for
`0x8003983c` and 90.95744% for `0x80039b58`. The enum names do not claim a
new match or settle the group scan's caller.

The same active-state name replaces four literal comparisons in the two
actor collision searches and two player-damage helpers. Post-integration
safe one-VA carves withheld no relocations. Isolated strict text remains
100% for `0x8003a9f4`, `0x8003ab5c`, and `0x8003a778`; the existing
`0x8003a318` WIP remains 99.86911% over 764 bytes.

## Target sound fields

Retail `0x8003d0e8` loads the target-candidate byte at `+0x04`, tests bit
`0x80`, and passes either its low seven bits plus 96 to
`audio_play_spatial_range` or the unmasked value plus 96 to
`audio_play_spatial_default_range`. Retail `0x8003d184` reads that same byte
and skips the sound trigger when it is `0xff`. The two independently decoded
uses support naming `KfTargetCandidate.sound_code`; the shared struct now has
a compile-time `+0x04` offset assertion. The same behavior path loads the
unsigned halfword at target `+0x0a` twice, masks its high two bits to select
phase, random, or periodic trigger mode, and uses the low fourteen bits as
the trigger interval. No other target-candidate C path reads that halfword,
so its typed field is now `sound_trigger` with a `+0x0a` offset assertion.
These are semantic type-model corrections, with no generated instruction
change.

Retail repeatedly reads target `+0x08` with `lhu` immediately before calls
to the exact `actor_advance_animation_clamped` and
`actor_advance_animation_wrapped` helpers. The same halfword controls phase
thresholds and the group-position phase step. The behavior dispatcher and
event target stream both consume this field, now named `animation_step`,
with a compile-time `+0x08` offset assertion. Focused rebuilds kept the
behavior listing and both exact sound siblings unchanged; the event stream
also rebuilt successfully with the renamed field.
The isolated behavior candidate object is SHA256-identical to the object
before this rename.

Fresh focused builds keep both spatial-sound functions `SAME` and the behavior
function at its prior `97.6%` focused listing. Safe one-VA GAME delinking
accepted all five spatial-sound and 923 behavior-dispatch relocation rows,
with zero withheld. Isolated strict objdiff confirms both sound functions at
100%; behavior stays 99.931595% text and 100% RODATA. The behavior callback
and the lifecycle scan's external caller remain unresolved where retail
contains no proved direct target. No address or relocation identity was
changed.

Raw `0x80039b58` masks its input to a low halfword, reads the actor group as
an unsigned byte at `+0x02`, walks 200 records at 124-byte stride, and calls
the same two typed lifecycle helpers as C. A literal scan of the retail GAME
load image found no full-word `0x80039b58` pointer, while semantic xrefs find
no external direct caller. A separate raw word scan found no encoded `j` or
`jal` to that address in GAME.EXE. Neither absence proves the function
unreachable or establishes an indirect caller, so its address-derived
identity remains WIP.

## Motion-vector caller graph

The three proven direct callers of `func_8003ae50` pass the contiguous actor
motion fields beginning at `+0x50` (retail call sites `0x8003bac0`,
`0x8003bb7c`, and `0x8003bef0`). The callee loads signed halfwords at input
offsets 0 and 4 at `0x8003ae88`, `0x8003ae98`, `0x8003aeb8`, and
`0x8003aebc`, then stores those same offsets at `0x8003b300` and
`0x8003b304` when the output flag is set. The middle signed halfword is the
Y component used independently by the 3-D caller. This establishes an
`SVECTOR *` interface more directly than an unstructured `s16 *`; the mover
now uses `vx` and `vz` while its callers retain their actor-field pointer
cast until the complete actor motion field is modeled. No field width,
argument location, or generated instruction changed.

Five focused quick builds cover the 21 connected claims below. The two
changed units retain 12/14 identical listings. Safe one-VA delinking of
their WIPs accepted 42 animation and 5 steering relocations with none
withheld; isolated strict text remains 99.31746% for the mover and
86.53226% for steering. The three adjacent WIPs also retain their prior
strict values: vertical motion 96.42041%, fixed curve 98.11751%, and phase
seek 99.268295%, each from fresh safe one-VA targets with no withheld
relocations. Existing exact siblings remain focused `SAME` and were already
strict certified; this interface correction banks no new function.

| GAME VA | Function or role | Verdict |
| --- | --- | --- |
| `80039c14` | fixed-curve scalar helper | Exact control. |
| `80039c94` | actor fixed curve | WIP 98.11751%; prior raw call/CFG gap remains bounded. |
| `8003a9f4` | actor collision search | Exact control. |
| `8003ab5c` | alternate collision search | Exact control. |
| `8003acb4` | current-actor binding | Exact control. |
| `8003ad90` | wrapped phase update | Exact control. |
| `8003adc4` | clamped phase update | Exact control. |
| `8003ae20` | phase crossing | Exact control. |
| `8003ae50` | X/Z mover | WIP 99.31746%; two masked-angle register/schedule differences. |
| `8003b33c` | 3-D collision move | Exact control. |
| `8003b520` | trajectory start | Exact control. |
| `8003b5bc` | vertical mode setter | Exact control. |
| `8003b5d0` | vertical motion | WIP 96.42041%; state-`0x20` reset join. |
| `8003b9a4` | decayed movement caller | Exact control. |
| `8003bae4` | scaled movement caller | Exact control. |
| `8003bba0` | yaw steering | Exact control. |
| `8003bcd0` | yaw/movement wrapper | Exact control. |
| `8003bd40` | horizontal steering | WIP 86.53226%; saved-register/load schedule. |
| `8003be38` | 3-D movement caller | Exact control. |
| `8003bf74` | pitch/yaw movement wrapper | Exact control. |
| `800460a0` | animation phase seek | WIP 99.268295%; step/half-step register assignment. |
