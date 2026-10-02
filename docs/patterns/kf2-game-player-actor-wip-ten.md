# GAME player and actor WIP recheck: ten functions

Fresh focused quick builds and isolated pinned-probe objdiff cover nine disjoint
GAME units: **ten WIPs** and **25 strict-exact sibling controls**. Each score
below is direct strict `.text`, not focused listing similarity. Raw call and
field evidence remains as recorded in the dedicated player, actor, and target
candidate dossiers; no register lifetime was forced in source.

| GAME VA | Strict verdict | First bounded difference |
| --- | ---: | --- |
| `0x80015918` | WIP 95.49419% | Trajectory discriminant and selected-time registers differ after the same three calls; CFG 41/41, with two exact siblings. |
| `0x80025a18` | WIP 95.85052% | Retail homes the first argument before its frame and reads the optional position at the next word; the probe uses a different variadic home and has 99/97 CFG blocks. All 15 siblings stay exact. |
| `0x8002665c` | WIP 98.67857% | Retail rematerializes two more `player_state` HI16/LO16 pairs; no referent or call is missing. CFG 114/114 and 52-byte RODATA exact. |
| `0x8002722c` | WIP 99.09091% | Retail reserves eight stack bytes; the candidate is a leaf with the same 33/33 CFG and 16/16 branches. |
| `0x800274ec` | WIP 89.85240% | Horizontal movement keeps its 11 calls and 128-byte frame; the probe has 34 versus 35 retail CFG blocks and reuses a player-state base. |
| `0x80027f78` | WIP 95.91228% | Collision response has 27/27 CFG blocks and the same calls; the candidate forms four additional player-state address pairs. |
| `0x80039108` | WIP 93.93092% `.text`, 96.183205% RODATA | Candidate scoring retains the table target classes and 59/59 CFG blocks; a return-path merge leaves 36 versus 37 retail branches and shifts table addends. |
| `0x80039c94` | WIP 98.11751% | Typed `u16` power and eight exact curve-helper calls recover 72/72 CFG blocks; saved argument and accumulator registers still differ. The curve leaf stays exact. |
| `0x8003bd40` | WIP 86.53226% | Actor horizontal steering keeps its geometry calls and 9/9 CFG blocks; independent coordinate loads and angle registers differ. Six adjacent motion helpers stay exact. |
| `0x8003d184` | WIP 99.931595% `.text`, 100% RODATA | Retail separately loads a target halfword for interval and trigger bits; the compiler reuses one load. CFG 410/410 and branch count 213/213, with saved-register and stack-offset residue. |

An off-tree equipment build using the authentic Psy-Q `<STDARG.H>` instead of
`<stdarg.h>` emitted the same bytes for all 16 unit functions, so it did not
recover the retail variadic argument home. An off-tree behavior-dispatch source
that spelled its interval and trigger tests as two field reads likewise emitted
the same single-load candidate. GCC 2.5.7 `-fno-cse-skip-blocks` lowered that
body to 99.86746% and RODATA to 43.205395%; disabling instruction scheduling
lowered it to 92.39419% and RODATA to 37.5%. These trials were discarded.
No source or profile change was retained from this second batch.

The equipment unit's 60 ordered RODATA pointer rows have the same 38
target-equivalence classes in retail and candidate, including the same
physical order of distinct case bodies. Non-exact addends move by four or
28 bytes as nearby code shifts. These rows support neither a missing switch
case identity nor a physical case-order correction.

For collision response `0x80027f78`, a full-unit off-tree
`-fno-cse-skip-blocks` control moved strict text only from 95.91228% to
95.93567%; disabling instruction scheduling fell to 73.83041%. Neither
eliminated the extra player-state address pairs, so no profile change follows.
For magic dispatch `0x8002665c`, the same off-tree CSE control regressed strict
text from 98.67857% to 96.968254%; its exact `0x80026498` sibling and RODATA
remained exact. The target-only address rematerializations do not justify a
unit-wide profile change.
