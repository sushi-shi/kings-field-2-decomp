# GAME actor motion: 21-function current verdict

Focused compiles and isolated strict objdiff checked five disjoint GAME
units: `actor_motion` (seven functions), `actor_lifecycle_target` (two),
`actor_animation` (seven), `actor_group_position` (four), and
`actor_animation_seek_phase` (one). **Fifteen functions are strict exact;
six remain WIP.** No source edit was justified by this pass.

| Unit | Strict-exact GAME addresses | WIP address and strict text |
| --- | --- | --- |
| `actor_motion` | `3b9a4`, `3bae4`, `3bba0`, `3bcd0`, `3be38`, `3bf74` | `3bd40`: 86.53226% |
| `actor_lifecycle_target` | none | `3983c`: 99.19598%; `39b58`: 90.95744% |
| `actor_animation` | `3a9f4`, `3ab5c`, `3acb4`, `3ad90`, `3adc4`, `3ae20` | `3ae50`: 99.31746% |
| `actor_group_position` | `3c000`, `3c10c`, `3c220` | `3c3e0`: 99.64539% |
| `actor_animation_seek_phase` | none | `460a0`: 99.268295% |

All six WIPs preserve their retail direct call sets and known field
referents. Focused CFG block/branch counts agree at `9/9, 5/5` for
`3bd40`; `37/37, 24/24` for `3983c`; `9/9, 4/4` for `39b58`;
`58/58, 34/34` for `3ae50`; `23/23, 11/11` for `3c3e0`; and
`6/6, 2/2` for `460a0`. The remaining first differences are register
allocation, address reuse, and instruction scheduling already examined in
the actor campaign notes. Source and inventory stayed unchanged. Only
focused compiles and isolated strict comparisons were run.

A fresh `0x800460a0` raw/focused check confirms identical 6/6 blocks, 2/2
branches, and ordered calls; only the `step`/`half_step` saved-register choice
differs. Off-tree GCC 2.5.7 `-fno-cse-skip-blocks` leaves strict text at
99.268295%, while GCC 2.6.0 O2 regresses it to 80.39024%. Neither profile
explains the retail bytes, so the source and profile remain unchanged.
The same off-tree controls on `actor_motion` lower `0x8003bd40` from
86.53226% to 84.38710% under GCC 2.5.7 no-CSE, preserving its six exact
siblings; GCC 2.6.0 O2 lowers the WIP to 27.709677% and regresses all six.
For `actor_group_position`, no-CSE lowers `0x8003c3e0` from 99.64539% to
96.028366% while preserving three exact siblings; GCC 2.6.0 O2 lowers the
WIP to 81.17731% and regresses those siblings. No alternate profile is
retained.
