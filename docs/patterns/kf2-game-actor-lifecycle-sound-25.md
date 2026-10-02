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
