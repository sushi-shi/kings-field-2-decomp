# GAME combat-to-effect chain: 30 current strict verdicts

Actor group effects constructs effects through `func_80040308`; its position
helpers and the effect pool, motion, collision, sound, and spawn functions form
the connected caller/callee controls below. Fourteen complete-profile GAME
units were freshly compiled and compared with isolated strict objdiff:
**26 exact functions and four WIPs**. This is a current-image check; the
earlier [constructor-family dossier](kf2-game-effect-constructor-family-28.md)
already records most exact controls and its bounded raw negative trials.

| GAME address | Source unit | Strict text | Verdict |
| --- | --- | ---: | --- |
| `0x8003c000` | `actor_group_position` | 100% | Exact position helper. |
| `0x8003c10c` | `actor_group_position` | 100% | Exact position helper. |
| `0x8003c220` | `actor_group_position` | 100% | Exact position helper. |
| `0x8003c3e0` | `actor_group_position` | 99.64539% | WIP: 23/23 CFG and 11/11 branches; yaw intermediate registers exchange roles. |
| `0x8003c614` | `actor_group_effects` | 86.041916% | WIP: 45/45 CFG, 15/15 branches; physical direction calls 11/7 and constructor calls 9/10; 492-byte table 37.90650% with 123 rows and 19 classes preserved. |
| `0x8003fa2c` | `effect_spatial_sound` | 100% | Exact spatial sound helper. |
| `0x8003fa68` | `effect_collision_probe` | 70.73333% | WIP: retail has four physical collision calls against one compiled call, although source spells all four. |
| `0x8003fb94` | `effect_update` | 100% | Exact effect helper. |
| `0x8003fdac` | `effect_update` | 100% | Exact power helper. |
| `0x8003fdd0` | `effect_update` | 100% | Exact effect helper. |
| `0x8003feb0` | `effect_update` | 100% | Exact effect helper. |
| `0x8003ff18` | `effect_update` | 100% | Exact effect helper. |
| `0x800400c0` | `effect_update` | 100% | Exact effect helper. |
| `0x800401b4` | `effect_update` | 100% | Exact effect helper. |
| `0x80040220` | `effect_update` | 100% | Exact pool scan. |
| `0x80040264` | `effect_update` | 100% | Exact scaled pool initializer. |
| `0x800402a4` | `effect_update` | 100% | Exact fixed pool initializer. |
| `0x80040308` | `effect_constructor` | 98.43441% | WIP: 116/116 CFG, 22/22 branches and all 157 ordered text relocs; 492-byte table 51.01626%. |
| `0x800416ec` | `effect_rotate_scale_offset_y` | 100% | Exact rotation/scale helper. |
| `0x8004177c` | `effect_move_probe` | 100% | Exact motion probe. |
| `0x8004195c` | `effect_aim_and_move` | 100% | Exact aim/motion helper. |
| `0x80041b14` | `effect_target_motion` | 100% | Exact target-motion helper. |
| `0x80041cd0` | `effect_scale_step` | 100% | Exact scaling step. |
| `0x80041d7c` | `effect_spawn_zero_direction` | 100% | Exact zero-direction spawn; eight-byte datum exact. |
| `0x80041e0c` | `effect_spawn_zero_direction` | 100% | Exact companion spawn. |
| `0x80041e94` | `effect_spawn_motion` | 100% | Exact motion spawn. |
| `0x8004212c` | `effect_spawn_motion` | 100% | Exact companion motion spawn. |
| `0x80042298` | `effect_scatter` | 100% | Exact scatter helper. |
| `0x80042424` | `effect_scatter` | 100% | Exact scatter helper. |
| `0x800424f0` | `effect_scatter` | 100% | Exact scatter helper. |

The constructor's four decoded differences begin with a repeated zero-byte
store in the retail kind-26 delay slot; adding a redundant store has no
independent source justification. Its 123 pointer rows preserve 62 target
classes and 65 exact addends despite the table percentage. The group caller
and collision probe each already spell the meaningful calls in source; the
pinned compiler folds some physical sites. Group-position's focused residue
is only swapped yaw registers, with three exact siblings. No new call,
referent, field width, or ownership fact supports a C edit. No new exact
closure is claimed.
