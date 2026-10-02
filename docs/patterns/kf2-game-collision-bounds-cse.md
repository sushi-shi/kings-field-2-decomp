# GAME collision-bounds CSE control

GAME `func_80023384` at `0x80023384` computes the lower and upper vertical
collision margins from two distinct cache words and the same three player
fields. It calls `player_death_begin` when either bound is fatal. The two
proven calls into this helper from the player collision branches and the
position-sync call, its two outgoing death calls, ten address pairs, and the
5/5-block, 2/2-branch CFG agree with the unchanged C source. It has no string
referent.

The configured GCC 2.5.7 `-O2 -G0 -mcpu=r2000` probe retained one
`player_state+0x134` base in `$s0` across both calculations. Retail instead
forms independent HI16/LO16 pairs for all three player fields in each half.
The ordinary probe emits 160 text bytes at 76.23256% strict match; disabling
`cse-follow-jumps` alone leaves those bytes unchanged, and disabling the
instruction scheduler yields 63.20930% with the same size. An isolated
`-fno-cse-skip-blocks` compilation emits exactly 172/172 retail text bytes.
All 22 ordered relocations have the same offsets, types, symbols, and addends,
and a direct raw `.text` comparison is byte-identical. A fresh focused rebuild
under the dedicated single-unit profile reports `SAME`.

The source remains the same typed expression in both halves. The special
profile applies only to `game.player_collision_bounds`; it is an observed
toolchain control, not proof of the original compiler flags or source file
boundary. No padding, artificial local, or assembly was added.

The same flag is not a general fix for player-state address reuse. An
isolated whole-unit control of `game.player_collision_sound` preserves its two
exact siblings, but lowers its WIP `func_800279cc` from 97.35812% to
90.62259% strict text. That unit keeps its existing profile.
An isolated control of `game.actor_player_damage` also leaves its
`func_8003a614` at 96.91011% strict text despite its own player-state-base
reuse residue. No profile change is applied to that unit.
The five-function `game.memory_card_directory` control likewise retains all
five strict scores, including its exact format helper; the flag does not
alter that reader's signed-byte or offset-walk residues.
