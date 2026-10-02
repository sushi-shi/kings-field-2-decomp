# GAME actor seventeen-function WIP verdict

Six coordinated actor units were rebuilt with focused `kf try --unit` and
compared against fresh safe one-VA GAME delinks using isolated strict
objdiff. Twelve functions are **100%** exact; five remain WIP. No C or
compiler-profile edit was retained in this pass.

| GAME VA | Unit | Strict text | Verdict |
| --- | --- | ---: | --- |
| `0x8003d084` | `actor_spatial_sound` | 100% | Exact, 100 bytes. |
| `0x8003d0e8` | `actor_spatial_sound` | 100% | Exact, 156 bytes. |
| `0x80038e38` | `actor_initialize_from_group` | 100% | Exact, 196 bytes. |
| `0x8003c000` | `actor_group_position` | 100% | Exact, 268 bytes. |
| `0x8003c10c` | `actor_group_position` | 100% | Exact, 276 bytes. |
| `0x8003c220` | `actor_group_position` | 100% | Exact, 448 bytes. |
| `0x8003c3e0` | `actor_group_position` | 99.64539% | WIP, 23/23 CFG blocks and 11/11 branches; first difference is the yaw intermediate register. |
| `0x80039108` | `actor_candidate_score` | 93.93092% | WIP, 59/59 CFG blocks and 37/36 branches; 524-byte table is 96.183205%. The residual Boolean branch merge has prior negative source-shape controls. |
| `0x8003b9a4` | `actor_motion` | 100% | Exact, 320 bytes. |
| `0x8003bae4` | `actor_motion` | 100% | Exact, 188 bytes. |
| `0x8003bba0` | `actor_motion` | 100% | Exact, 304 bytes. |
| `0x8003bcd0` | `actor_motion` | 100% | Exact, 112 bytes. |
| `0x8003bd40` | `actor_motion` | 86.53226% | WIP, 9/9 CFG blocks and 5/5 branches; call set and values agree, but evaluation and register schedules differ. |
| `0x8003be38` | `actor_motion` | 100% | Exact, 316 bytes. |
| `0x8003bf74` | `actor_motion` | 100% | Exact, 140 bytes. |
| `0x8003983c` | `actor_lifecycle_target` | 99.19598% | WIP, 37/37 CFG blocks and 24/24 branches; actor pointer and lifecycle-constant register choices differ. |
| `0x80039b58` | `actor_lifecycle_target` | 90.95744% | WIP, 9/9 CFG blocks and 4/4 branches; retail keeps the free-slot byte in a saved register, while the candidate materializes it at each use. |

The five WIPs preserve their observed call and field meaning in this screen.
The scorer's extra retail branch remains the only structural branch-count
gap in this group. A percentage or register selection does not justify
source changes without a new raw field, call, referent, or CFG fact.
