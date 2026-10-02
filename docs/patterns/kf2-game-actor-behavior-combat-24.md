# GAME actor behavior and combat: 24 current strict verdicts

The actor behavior dispatcher calls the motion, animation, vertical-collision,
and spatial-sound helpers below. Its callback/phase path also reaches the
actor animation seeker through the event target stream. This campaign follows
those confirmed call and actor-state relationships; it does not infer one
source unit from address proximity.

Seven complete-profile GAME source units were freshly compiled and compared
with isolated strict objdiff. **Eighteen functions are exact and six remain
WIP.** The dispatcher's 964-byte, 241-row switch table is strict exact.
The table below reports current strict text scores, including exact sibling
controls.

| GAME address | Source unit | Strict text | Verdict |
| --- | --- | ---: | --- |
| `0x80039c14` | `actor_fixed_curve` | 100% | Exact curve helper. |
| `0x80039c94` | `actor_fixed_curve` | 98.11751% | WIP: 72/72 CFG, 46/46 branches, eight curve calls; accumulator and stack layout residue. |
| `0x8003a9f4` | `actor_animation` | 100% | Exact animation/collision helper. |
| `0x8003ab5c` | `actor_animation` | 100% | Exact animation/collision helper. |
| `0x8003acb4` | `actor_animation` | 100% | Exact actor-current binder. |
| `0x8003ad90` | `actor_animation` | 100% | Exact wrapped phase advance. |
| `0x8003adc4` | `actor_animation` | 100% | Exact clamped phase advance. |
| `0x8003ae20` | `actor_animation` | 100% | Exact phase-crossing test. |
| `0x8003ae50` | `actor_animation` | 99.31746% | WIP: 58/58 CFG and direct calls agree; retail masks the obstacle angle before motion-length multiplication, while the probe schedules the independent mask afterward. |
| `0x8003b33c` | `actor_motion_collision` | 100% | Exact motion/collision helper. |
| `0x8003b520` | `actor_motion_collision` | 100% | Exact motion/collision helper. |
| `0x8003b5bc` | `actor_motion_collision` | 100% | Exact short motion helper. |
| `0x8003b5d0` | `actor_motion_collision` | 96.42041% | WIP: 40/39 CFG, 21/21 branches; state-`0x10` reset join differs. |
| `0x8003b9a4` | `actor_motion` | 100% | Exact motion helper. |
| `0x8003bae4` | `actor_motion` | 100% | Exact motion helper. |
| `0x8003bba0` | `actor_motion` | 100% | Exact motion helper. |
| `0x8003bcd0` | `actor_motion` | 100% | Exact motion helper. |
| `0x8003bd40` | `actor_motion` | 86.53226% | WIP: 9/9 CFG, 5/5 branches and calls; independent coordinate-load/register schedule differs. |
| `0x8003be38` | `actor_motion` | 100% | Exact motion helper. |
| `0x8003bf74` | `actor_motion` | 100% | Exact motion helper. |
| `0x8003d084` | `actor_spatial_sound` | 100% | Exact sound-gate helper. |
| `0x8003d0e8` | `actor_spatial_sound` | 100% | Exact shared sound caller. |
| `0x8003d184` | `actor_behavior_dispatch` | 99.931595% | WIP: 410/410 CFG, 213/213 branches and exact table; repeated target halfword load becomes a probe `nop`. |
| `0x800460a0` | `actor_animation_seek_phase` | 99.268295% | WIP: 6/6 CFG and two render-frame calls; saved `step`/`half_step` registers are exchanged. |

Fresh focused checks of the two structural candidates preserve the three
exact motion-collision siblings and the exact curve sibling. The vertical
collision path retains a retail 40/39 block and 10/11 known return-frontier
gap; a natural shared-reset label previously regressed strict 96.42041% to
95.216324% and was reverted. The fixed-curve path now has **72/72** CFG
blocks and **46/46** branches; older 72/71-block notes describe a prior
source state. Its eight curve calls and callback remain represented.

The animation source already masks the obstacle angle before computing motion
length; the compiler schedules those independent operations in the opposite
order. The dispatcher's single repeated `lhu`, the phase seeker's register
roles, and the other WIPs do not reveal a missing referent, call, or
independently supported type correction. The detailed raw case and negative
controls are
in [the actor collision dossier](game-actor-collision-ten.md) and
[the behavior-dispatch dossier](kf2-game-actor-behavior-dispatch.md). No C,
header, relocation, or data-owner edit is justified by this screen, and no
new exact closure is claimed.

## Deep motion and dispatcher control

Fresh focused `game.actor_motion` keeps six of seven listings exact and
`0x8003bd40` at 9/9 CFG and 5/5 branches. Its five ordered text relocations
match retail by type and target: the actor-state HI/LO pair, then calls to
`vector_xz_to_angle`, `angle_within_tolerance`, and `func_8003bcd0`.
Retail independently loads actor X and Z around the two subtractions and
leaves two conditional-negation delay slots empty; the probe groups the
coordinate loads and fills those slots with independent work. Signed
halfword reference-angle extension and the distance/tolerance gate already
agree. There is no new field, call, or control-flow fact to change in C.

Fresh focused `game.actor_behavior_dispatch` remains 410/410 CFG and
213/213 branches, with incomplete indirect-jump reachability. All **341
ordered text relocation type/target pairs** match retail. Its first
instruction-selection gap is a second retail `lhu` of target `+0x0a`, used
for the trigger bits; the probe reuses the earlier interval load. An
off-tree natural spelling with two explicit field reads compiled
byte-identically to retained C: strict text stayed **99.931595%**, its
964-byte table stayed exact, and the second load remained a `nop`.
No duplicate volatile read or artificial carrier was retained.
