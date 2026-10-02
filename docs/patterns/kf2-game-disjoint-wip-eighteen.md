# GAME disjoint WIP recheck: 18 functions

Fresh isolated pinned-probe objects and focused quick listings cover 13 GAME
units, **18 WIP functions**, and **14 strict-exact sibling controls**. The
table records strict `.text` scores; focused listing similarity is a different
measure. All functions retain their existing source claims and the raw call,
field, and referent controls established in their dedicated campaign dossiers.

| GAME VA | Strict verdict | First bounded difference |
| --- | ---: | --- |
| `0x80016260` | WIP 98.790085% | Eight byte-valued transition arguments retain their widths; stack-byte saved registers and sentinel branch scheduling differ. CFG 67/67. |
| `0x8001fb8c` | WIP 99.78788% | Window body agrees; retail reserves eight additional frame bytes. CFG 10/10. |
| `0x800210ac` | WIP 99.66904% | String glyphs and kana marks agree; retail frame is eight bytes larger and one UV calculation swaps registers. CFG 8/8. |
| `0x80021c8c` | WIP 99.956985% | Four raw words differ only in frame and saved-`ra` slot offsets; the exit sibling stays exact. CFG 7/7. |
| `0x80022058` | WIP 97.39% | The decimal formatter is a candidate leaf while retail reserves eight frame bytes. CFG 36/36. |
| `0x800226ec` | WIP 93.60504% | Card directory slot-seed byte loads and initialization around `memset` are scheduled differently. CFG 13/13. |
| `0x800228c8` | WIP 85.15625% | Retail uses `lb` for two title bytes and advances a byte offset through both digit loops; the source's typed header and calls agree. CFG 24/24. |
| `0x80022b74` | WIP 93.666664% | Card payload reader has an 80-byte retail frame versus 72-byte candidate frame and a different slot saved-register lifetime. CFG 9/9. |
| `0x80022ca0` | WIP 95.896774% | Card writer orders slot-seed loads, zero fill, and saved-register setup differently. CFG 24/24. |
| `0x800279cc` | WIP 98.23967% | Landing-state controller has a 72-byte retail frame versus 64-byte candidate frame; two player-state address pairs are folded into a retained base. CFG 70/70. |
| `0x8002ce68` | WIP 65.85185% | Retail reloads three O32 stack arguments after the free-slot call; the candidate saves them early and grows its frame from 40 to 56 bytes. CFG 5/5. |
| `0x80030c18` | WIP 96.521736% | Map-cell object renderer keeps the 4176-byte frame and calls; independent quarter-turn and flags moves swap order. CFG 11/11. |
| `0x800311b0` | WIP 92.14815% | Textured quad fields and five-way bounds control agree; a code-byte store moves out of a branch delay slot and saved-register choices differ. CFG 8/8. |
| `0x80031850` | WIP 99.29851% | World-model renderer keeps the call/referent set; saved-register and independent light-address scheduling differ. CFG 40/40. |
| `0x80031d8c` | WIP 94.65414% | Animated-object renderer reloads blend mode where retail preserves it in a saved register. CFG 7/7. |
| `0x800320b0` | WIP 73.95918% | Radius layer-mask bounds and row/column loops agree; coordinate setup and induction registers differ. CFG 10/10. |
| `0x80032174` | WIP 93.6% | View-cell comparisons and referents agree; the result register and final move differ. CFG 5/5. |
| `0x800321d8` | WIP 98.4359% | The provisional arena-base literal emits `lui/ori`; retail has a signed-low relocatable `lui/addiu` pair without a proved complete source owner. CFG 3/3. |

The exact controls are card format (one), resource runtime (five, including
the newly exact `0x80032274` VAB updater), menu exit (one), floor-item search
and update (two), map-cell draw siblings (three), and player collision/death
sound helpers (two). The VAB source correction and its five exact ordered
relocations are documented in [the resource campaign](game-resource-cd-eleven.md).

An off-tree signed-byte spelling for the card reader changed strict text only
from 85.15625% to 85.25%; the candidate still emitted `lbu` at all four
title-byte sites where retail has `lb`. A typed signed view lowered it to
80.2%. Neither establishes an original source change, so the tracked card
reader remains untouched. The floor-item profile and K&R negative controls
are recorded in [the collision campaign](game-collision-mask-ten.md). No
artificial frame storage, volatile carrier, or register-steering edit was kept.

A fresh five-function isolated card-unit control confirms that changing the
`encoded` byte destinations to `s8 *` leaves the reader at 85.15625% strict;
using a signed-byte union instead lowers it to 69.575%. Explicit
`-fsigned-char` and `-funsigned-char` compiler controls also leave all five
function scores unchanged, including the exact format wrapper. These controls
do not explain retail's `lb` pair or justify changing the shared card header.

## Fresh main-loop target control

A fresh safe delink of GAME `0x80013634` and `0x8001369c` admitted both
reviewed `DAT_80198630` HI16/LO16 pairs at `0x800138c4/0x800138c8` and
`0x8001394c/0x80013950`, with zero withheld rows. Its module object is
byte-identical to the current cached delink target. Recompiling the unchanged
`game.main` source and comparing directly against that fresh module gives
`main` **100%** and `game_main_loop` **99.67553% strict**. The only `.text`
word differences are the two arena-address instructions: retail uses
`lui a0,0x800a; addiu a0,a0,-20320` for `0x8009b0a0`, while the C literal
emits `lui a0,0x8009; ori a0,a0,0xb0a0`. The original arena-source mechanism
remains unresolved; no C or identity edit is justified.

The older **99.569145%** isolated score in the root near-exact dossier came
from a prior comparison. Its explanation that **99.67553% necessarily meant
a target missing the two BSS pairs is superseded**: the fresh target has both
pairs and still scores 99.67553%. The older object's precise configuration
was not preserved here, so the score difference is not attributable to one
source or delinker change. Use the fresh target and direct score above for
the current verdict.

## Fresh exact controls across ten claimed functions

Focused `kf try --unit` builds and fresh safe delinks give **100% strict text**
for all ten functions in four GAME units:

| Unit | Functions and retail text sizes | Safe delink |
| --- | --- | --- |
| `game.animation_sparse_vertices` | `0x80033bfc` 196 B, `0x80033cc0` 124 B, `0x80033d3c` 696 B | 7 ordered relocations, none withheld |
| `game.map_object_spawn_scatter` | `0x800365d8` 292 B, `0x800366fc` 440 B, `0x800368b4` 144 B, `0x80036944` 116 B | 75 relocations, none withheld |
| `game.render_frame` | `0x80033584` 28 B, `0x800335a0` 1012 B | 190 relocations, none withheld |
| `game.event_command_dispatch` | `0x8004678c` 3156 B | 471 relocations, none withheld |

These are current exact controls, not new closures: the old `0x800366fc`
37.3% and `0x80033d3c` 95.21839% WIP entries elsewhere are stale. The
event controller's five compared object symbols also report 100% strict.
No C source was changed for this recheck.
