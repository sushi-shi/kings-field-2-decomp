# GAME player reaction and actor collision: 24-function control

This campaign uses three related GAME units: player reaction (17 functions),
player collision sound (three), and actor vertical collision (four). A fresh
focused compile of each unit and isolated direct objdiff against its retail
module gives **21 strict-exact functions and three WIPs**. These are current
object comparisons; older percentages in the broader campaign notes may
describe earlier source or target states.

| Unit | Exact GAME functions | WIP GAME function, strict text |
| --- | --- | --- |
| `game.player_reaction` | `28ec0`, `28fa8`, `29014`, `29168`, `291d0`, `291ec`, `293d4`, `29428`, `29464`, `294f8`, `29570` (`player_death_begin`), `295f8`, `29624`, `296e8` (`player_adjust_hp`), `2975c` (`player_adjust_mp`), `297b4` | `2985c`: 99.26569% |
| `game.player_collision_sound` | `27928`, `27988` | `279cc`: 98.23967% |
| `game.actor_motion_collision` | `3b33c`, `3b520`, `3b5bc` | `3b5d0`: 96.42041% |

The reaction controller `2985c` retains 142/142 CFG blocks and 79/79
branches. Its first focused differences are the fade-clamp register and
instruction schedule; the later map-object address and loop registers also
differ. The signed phase helper `29624` is already strict exact, despite an
older WIP entry elsewhere. An off-tree split of the fade value into a
separate `s16` local caused an extra unsigned reload and lowered focused
similarity from 96.4% to 96.3%. Widening the function's shared value to
`s32` changed its frame and lowered focused similarity to 87.8%. Both
trials were discarded.

The player collision controller `279cc` has 70/70 CFG blocks, 37/37
branches, and the same known successor lists by block order. Its first
focused difference is the 72-byte retail versus 64-byte candidate frame
and allocation of the player-position base. Later retail forms two
additional player-state address pairs while the candidate retains a base
through landing-state zero stores. No missing call, width, or field
referent was established by this audit.

The actor vertical-motion controller `3b5d0` has 40 retail versus 39
candidate CFG blocks and 21/21 branches. Retail's bit-4 collision settle
path clears the vertical speed in a jump delay slot and enters the reset
shared with state `0x20`; the candidate places the state-byte reset in a
jump delay slot to the epilogue. A shared-reset-label probe recovered the
local jump shape but lowered focused similarity from 94.3% to 88.6% and
changed the return frontier from 10/11 to 10/9. Explicit signed and
unsigned speed locals expanded the frame from 72 to 80 bytes and lowered
focused similarity to 82.4%. Neither probe improved the full control
structure, so neither was retained.

All three WIPs keep their current source models. This campaign changed no
source, retail inventory, or build configuration. Validation used only
focused unit compiles and isolated strict objdiff; no repository tests,
lint, or full build were run.
