# GAME actor attack and target selection: 25 current strict verdicts

This cohort follows confirmed source and retail call edges: the group-target
fixup and distance helpers call best-target selection; selection scores each
candidate and sets the chosen target; actor lifecycle calls those helpers;
actor damage reaches the player damage and reaction chain. Home/preparation
functions are exact controls for the lifecycle caller. These are GAME image
identities, not address-only cross-overlay associations.

All 18 source units were freshly compiled with their complete unit profiles
and compared by isolated strict objdiff. The **25 functions comprise 19 exact
controls and six WIPs**. Scores below are strict text scores; exact table
controls are stated separately. Focused or loose similarity is not counted
as closure.

| GAME address | Source unit | Strict text | Verdict |
| --- | --- | ---: | --- |
| `0x80024498` | `player_damage_reaction` | 100% | Exact reaction callee; 32-byte table exact. |
| `0x800248a8` | `player_apply_damage` | 100% | Exact damage callee; 28-byte table exact. |
| `0x80024ca4` | `player_radial_damage` | 100% | Exact radial caller of damage application. |
| `0x80038d04` | `actor_set_home_position` | 100% | Exact home-position control. |
| `0x80038efc` | `actor_home_wrapper` | 100% | Exact lifecycle/home wrapper. |
| `0x80038f20` | `actor_home_wrapper` | 100% | Exact preparation wrapper. |
| `0x80038ff0` | `actor_prepare_initialize` | 100% | Exact preparation caller. |
| `0x80039048` | `actor_prepare_initialize` | 100% | Exact adjacent initializer. |
| `0x800390d0` | `actor_set_target` | 100% | Exact selected-target store. |
| `0x80039108` | `actor_candidate_score` | 93.93092% | WIP: 59/59 CFG, 37/36 branches; case-9 tolerance/callback join differs. Its 524-byte table is 96.183205%, with preserved target classes. |
| `0x800395c8` | `actor_select_best_target` | 100% | Exact scorer caller and target selector. |
| `0x800396c4` | `actor_target_distance` | 100% | Exact distance-driven selector. |
| `0x80039710` | `actor_find_target` | 100% | Exact group target lookup. |
| `0x80039758` | `actor_select_group_target` | 100% | Exact group target caller. |
| `0x800397a8` | `actor_target_reset` | 100% | Exact reselect caller. |
| `0x800397d8` | `actor_byte_0c_set` | 100% | Exact target-state byte setter. |
| `0x80039804` | `actor_byte_0c_set_changed` | 100% | Exact conditional target-state byte setter. |
| `0x8003983c` | `actor_lifecycle_target` | 99.19598% | WIP: 37/37 CFG and 24/24 branches; actor/constant saved-register roles differ. |
| `0x80039b58` | `actor_lifecycle_target` | 90.95744% | WIP: 9/9 CFG, 4/4 branches; free-slot byte lifetime differs. |
| `0x8003a318` | `actor_player_damage` | 99.86911% | WIP: incoming `amount_and_flags` and falloff stack loads choose opposite temporaries. |
| `0x8003a614` | `actor_player_damage` | 96.91011% | WIP: four ordered calls and 6/6 CFG agree; retail separately materializes player camera Z. |
| `0x8003a778` | `actor_player_damage` | 100% | Exact adjacent damage helper. |
| `0x8003f610` | `actor_fixup_group_targets` | 100% | Exact target-fixup caller. |
| `0x8003f7ec` | `actor_fixup_group_targets` | 85.86207% | WIP: 9/9 CFG and 4/4 branches; sentinel immediate and commutative pointer-add order differ. |
| `0x8003f860` | `actor_fixup_group_targets` | 100% | Exact adjacent group helper. |

The six WIPs retain their known calls and data identities. The scorer's
case-9 source already expresses its tolerance exit; a natural `break`
compiled byte-identically, while an inverted positive guard regressed both
text and table. Splitting the lifecycle slot guard also compiled identically.
Focused CFG still warns that the scorer's table jump is indirect, so its
59/59 block count does not prove equivalent reachability or return frontiers.
The actor damage source uses the shared typed player-damage declaration; its
object remained byte-identical after that interface cleanup. These controls,
and the raw argument/field details, are in [the actor collision dossier](game-actor-collision-ten.md).
This fresh comparison supplied no independent source fact for changing a C
body, header, relocation, or data owner, and created no new exact closure.
