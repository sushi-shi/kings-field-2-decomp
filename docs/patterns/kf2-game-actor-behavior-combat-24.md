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

## Connected actor initialization and motion-math controls

The dispatcher's proven call at `0x8003efcc` passes eight arguments to
`0x8003bd40`: world X/Z, speed, range, the actor's prior signed angle,
two group bytes, and target `5`. The retail O32 register and stack words
agree with the current caller and callee declarations. No signature or
field-width correction follows from this call site.

The following **29 distinct GAME claims** complete a fresh isolated strict
screen of actor initialization, trajectory, and vector helpers connected to
the motion family. They were not counted in the 24-row table above. The
vector angle and tolerance helpers called by `0x8003bd40` are exact. Only
the ballistic trajectory solver remains WIP; its focused comparison has
41/41 CFG blocks, 24/24 branches, two `SquareRoot0` calls and one angle
call, with the same ordered relocation targets. Its first difference is the
register holding the second discriminant subtraction, not a changed
operand, guard, or referent.

| GAME address | Source unit | Strict text | Verdict |
| --- | --- | ---: | --- |
| `0x80015034` | `vector_math` | 100% | Exact forward-vector helper. |
| `0x80015104` | `vector_math` | 100% | Exact rotation helper. |
| `0x80015148` | `vector_math` | 100% | Exact scale helper. |
| `0x80015188` | `vector_math` | 100% | Exact scale helper. |
| `0x800151e4` | `vector_math` | 100% | Exact scale helper. |
| `0x80015224` | `vector_math` | 100% | Exact scale helper. |
| `0x80015280` | `vector_math` | 100% | Exact X/Z sum helper. |
| `0x800152ac` | `vector_math` | 100% | Exact angle-tolerance callee. |
| `0x800152e8` | `vector_math` | 100% | Exact angle-half-turn helper. |
| `0x800152f8` | `vector_math` | 100% | Exact shortest-angle helper. |
| `0x80015318` | `vector_math` | 100% | Exact X/Z angle callee. |
| `0x80015468` | `vector_math` | 100% | Exact two-axis length helper. |
| `0x800154a8` | `vector_math` | 100% | Exact three-axis length helper. |
| `0x800154fc` | `vector_actor_helpers` | 100% | Exact actor-vector helper. |
| `0x80015574` | `vector_actor_helpers` | 100% | Exact actor-vector helper. |
| `0x800155a4` | `vector_distance` | 100% | Exact point-distance helper. |
| `0x80015698` | `vector_distance_between` | 100% | Exact pair-distance helper. |
| `0x800157ac` | `actor_random_scalar` | 100% | Exact random scalar helper. |
| `0x800157f8` | `actor_random_scalar` | 100% | Exact random scalar helper. |
| `0x8001584c` | `actor_fixed_interpolation` | 100% | Exact interpolation helper. |
| `0x8001586c` | `actor_fixed_interpolation` | 100% | Exact interpolation helper. |
| `0x800158b4` | `actor_fixed_interpolation` | 100% | Exact interpolation helper. |
| `0x80015918` | `actor_trajectory_math` | 95.49419% | WIP: arithmetic temporary/register assignment; calls and CFG agree. |
| `0x80015bc8` | `actor_trajectory_math` | 100% | Exact trajectory caller. |
| `0x80015ce0` | `actor_trajectory_math` | 100% | Exact trajectory helper. |
| `0x80038cc8` | `actor_pool_find` | 100% | Exact actor-pool lookup. |
| `0x80038dc4` | `actor_copy_group_defaults` | 100% | Exact group-default copy. |
| `0x80038e38` | `actor_initialize_from_group` | 100% | Exact actor initializer. |
| `0x80039080` | `actor_pool_clear` | 100% | Exact actor-pool reset. |

This control set is **28 exact and one WIP**. It provides no independently
supported C correction for the two dispatcher/motion residues or the
trajectory solver. No source, data, or relocation claim was changed.

Fresh focused and isolated strict controls of the adjoining actor-group and
animation units leave their previously recorded gaps unchanged:
`actor_group_position` has three exact siblings and `0x8003c3e0` at
99.64539% (23/23 CFG; yaw temporary registers exchanged); `actor_animation`
has six exact siblings and `0x8003ae50` at 99.31746% (58/58 CFG; the source
already masks the obstacle angle, but the probe schedules that independent
mask later). `actor_group_effects` remains 86.041916% text and 37.9065%
for its 492-byte switch table, with 45/45 CFG and 15/15 branches. Its
retail/compiled physical helper calls remain 11/7 and constructor calls
9/10; the source already spells the eleven helper expressions, while the
documented shared-tail alternatives either changed arguments or regressed.
The retail group dispatcher stores its third O32 argument at `sp+200` and
sets a cursor to `sp+200`, whereas the pinned probe sets that cursor to
`sp+204`; both then read the same first script word at `sp+204`. The
supplied Psy-Q 3.0 `STDARG.H` anchors `va_start` after the last named
argument, so this four-byte cursor-origin difference alone does not prove
a different historical macro or function signature. Its call-site and
absolute stack-slot behavior remain the controls.
No additional semantic edit follows from these rechecks.

The complete refreshed group/animation control set is listed below. These
**20 are rechecks**, including rows already present in this note or the
event/animation note; they do not add 20 new denominator claims.

| GAME address | Source unit | Strict text | Verdict |
| --- | --- | ---: | --- |
| `0x80033b34` | `animation_keyframe` | 100% | Exact. |
| `0x80033bfc` | `animation_sparse_vertices` | 100% | Exact. |
| `0x80033cc0` | `animation_sparse_vertices` | 100% | Exact. |
| `0x80033d3c` | `animation_sparse_vertices` | 100% | Exact. |
| `0x80033ff4` | `animation_sparse_find` | 100% | Exact. |
| `0x80039c14` | `actor_fixed_curve` | 100% | Exact sibling. |
| `0x80039c94` | `actor_fixed_curve` | 98.11751% | WIP, 72/72 CFG; curve-call set aligned. |
| `0x8003a9f4` | `actor_animation` | 100% | Exact sibling. |
| `0x8003ab5c` | `actor_animation` | 100% | Exact sibling. |
| `0x8003acb4` | `actor_animation` | 100% | Exact sibling. |
| `0x8003ad90` | `actor_animation` | 100% | Exact sibling. |
| `0x8003adc4` | `actor_animation` | 100% | Exact sibling. |
| `0x8003ae20` | `actor_animation` | 100% | Exact sibling. |
| `0x8003ae50` | `actor_animation` | 99.31746% | WIP, 58/58 CFG; angle-mask scheduling. |
| `0x8003c000` | `actor_group_position` | 100% | Exact sibling. |
| `0x8003c10c` | `actor_group_position` | 100% | Exact sibling. |
| `0x8003c220` | `actor_group_position` | 100% | Exact sibling. |
| `0x8003c3e0` | `actor_group_position` | 99.64539% | WIP, 23/23 CFG; yaw temporary registers. |
| `0x8003c614` | `actor_group_effects` | 86.041916% | WIP, 45/45 CFG; helper/constructor call-site layout. |
| `0x800460a0` | `actor_animation_seek_phase` | 99.268295% | WIP, 6/6 CFG; saved step-register roles. |

The rechecks yield **15 exact and five WIP** with no fresh exact closure or
supported source edit.
