# GAME actor and collision: ten current WIP verdicts

Focused quick builds and isolated strict objdiff checked four disjoint GAME
units. Ten functions remain WIP and **15 adjacent functions remain exact**.
The strict objects were freshly compiled from the tracked sources; no source or
inventory edit was justified by this pass.

| Unit | WIP address: strict text (focused listing) | Exact controls |
| --- | --- | --- |
| `actor_player_damage` | `3a318`: 99.86911% (97.5%); `3a614`: 96.91011% (81.2%) | `3a778` |
| `actor_fixup_group_targets` | `3f7ec`: 85.86207% (93.8%) | `3f610`, `3f860` |
| `collision_height_wrappers` | `2b67c`: 94.895836% (87.1%); `2b73c`: 98.40426% (75.5%); `2b874`: 91.5% (85.8%); `2b9d4`: 95.37931% (62.2%); `2bfd4`: 73.91262% (35.5%); `2c424`: 98.29932% (94.3%) | Eleven other claims |
| `player_interval_71_80` | `2897c`: 88.57143% (28.6%) | `28998` |

The raw and focused comparisons preserve each WIP's direct call set and
known referents. `3a318` differs first in two incoming argument registers;
`3a614` reuses one player-camera base where retail rematerializes it.
`3f7ec` differs in constant setup and a commutative pointer addition.
`2b67c` retains a computed elevation where retail reloads the cached height;
`2b73c` differs in row-pointer and index registers. `2b874` alone has a
different CFG block count (8 retail, 9 probe): retail stores each selected
radius before loading interaction height, while the probe schedules both
through a common tail. The source already spells the branch-specific radius
stores and shared height store. `2b9d4` differs in frame/register lifetime;
`2bfd4` algebraically cancels an origin that the retail computes on each axis;
`2c424` assigns its indexed cursor to another saved register. `2897c` has
the signed interval bounds but different result-register lifetime.

No register carrier, fake store, or overlapping global was introduced to
imitate these remaining instruction schedules. Verification used only focused
compiles, isolated strict comparisons, and raw instruction review.
