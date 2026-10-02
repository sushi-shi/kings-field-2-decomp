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

For lifecycle scan `0x80039b58`, a fresh two-function safe delink confirms
90.95744% strict text over 188 bytes; the preceding `0x8003983c` remains
99.19598%. Retail advances a base actor cursor and a second cursor anchored
at actor `+9`, but both refer to the same typed record. A natural index-based
`for` loop was compiled as a control for those two induction views. It
lowered the scan to 56.744682% while leaving the preceding function
unchanged. The index trial was reverted; no duplicate cursor or raw-offset
view was added merely to reproduce register allocation.

### Target and math control campaign, 29 functions

A later pinned-profile isolated compile of 19 related actor target, group,
trajectory, and animation units yields 23 strict-exact functions. The exact
GAME entry points are `80015bc8`, `80015ce0`, `80038dc4`, `80038e38`,
`80038efc`, `80038f20`, `80038ff0`, `80039048`, `80039080`, `800390d0`,
`800395c8`, `800396c4`, `80039710`, `80039758`, `800397a8`, `800397d8`,
`80039804`, `80039c14`, `8003c000`, `8003c10c`, `8003c220`, `8003d084`,
and `8003d0e8`. Their six non-exact companions have these current strict
verdicts:

| GAME address | Strict text | Final bounded verdict |
| --- | ---: | --- |
| `80015918` | 95.49419% | The ballistic solver retains its 41/41 CFG blocks, 24/24 branches, direct calls, and two exact following math helpers; discriminant/time-value register lifetimes differ. |
| `8003983c` | 99.19598% | Lifecycle update retains 37/37 blocks and 24/24 branches with matching actor/group/player referents; its actor pointer and lifecycle constant use different registers. |
| `80039b58` | 90.95744% | Group scan retains 9/9 blocks and 4/4 branches; the retail free-slot byte remains in a saved register, while the candidate reloads/materializes it. The index-based loop control above remains negative. |
| `80039c94` | 98.11751% | Fixed-curve caller retains its raw-backed width and shared-store model and 72/72 CFG blocks; no new call or field discrepancy emerged. |
| `8003c3e0` | 99.64539% | Trajectory stepper retains 23/23 blocks, 11/11 branches, and correct pitch-override width; only the yaw-normalization intermediate assignment remains. |
| `800460a0` | 99.268295% | Seek-phase helper retains 6/6 blocks, 2/2 branches, calls, and referents; even-step and half-step saved-register assignment differs. |

The source and earlier raw/caller dossiers give no additional semantic,
relocation, or field correction for these six. No C, header, identity, or
profile change followed this screen, and all 23 exact controls remain intact.
For the five modules containing those six WIPs, target and candidate have
identical ordered relocation type/symbol sequences: respectively 8, 52,
37, 37, and 4 rows for trajectory math, lifecycle, fixed curve, group
position, and seek phase. This sequence check does not assert equal code
placement or every relocation addend.
