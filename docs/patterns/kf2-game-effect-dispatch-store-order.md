# GAME effect dispatcher: independent scale-store order

The GAME `effect_update_dispatch` switch at `0x80042650` remains WIP.
Four case-local groups write independent scale halfwords in a different
order from the raw retail program. Each source change preserves the final
record state and restores the observed store order:

| Path | Retail stores | Prior C stores | Retail body offsets |
| --- | --- | --- | --- |
| Kind 12 reset | Z, Y, X | X, Y, Z | `+0x1774..+0x177c` |
| Kind 10 reset | Z, Y, X | X, Y, Z | `+0x2700..+0x2708` |
| Kind 6 phase two | Z, X | X, Z | `+0x2c34..+0x2c38` |
| Kind 101 update | X, Z, Y | X, Y, Z | `+0x2f6c`, `+0x2f78..+0x2f7c` |

In kind 6, both Z and X receive the same `func_8001584c` result before a
separate Y interpolation. In kind 101, the three halfwords receive one
sum before the position-Y update. The raw opcodes and field offsets, not
the percentage, support the reorderings. The neighboring loads, position
stores, and shared tails remain scheduled differently.

A fresh safe one-VA target materialized 1,068 relocations with none
withheld. Focused builds retain 446/448 CFG blocks and 217/217 branches.
Isolated strict text rises from 95.181404% to **95.1837%** (13,936 retail
versus 13,804 candidate bytes). Exactly eight candidate `.text` bytes
changed: the store offsets at `+0x1744`, `+0x174c`, `+0x2698`, `+0x26a0`,
`+0x2bb8`, `+0x2bbc`, `+0x2ef8`, and `+0x2efc`. Candidate RODATA is
byte-identical to its baseline and remains 31.614786% strict. All 470
ordered text relocation type/target pairs and 128 switch-pointer rows
are unchanged; retail and candidate each retain 206 direct `jal` sites.

The earlier growth-tail scheduling divergence around retail `+0x2d4`
remains the first raw text gap. The source still writes the same scale and
phase fields on that path. An off-tree kind-8 parent-coordinate preload
trial inserted a word into the candidate body without proving a different
field or source operation; it was discarded.

Two later differences are bounded by the existing source. Around retail
`+0x81c..+0x838`, the collision-cache result is loaded twice, while the
probe reuses its first load; the C already reads the named cache result
twice. At kind 6's 32-iteration loop, retail tests the decremented index
against `-1` with `bne`, while the probe uses `bgez` even though the C
spells `index != -1`. An off-tree `do`/`while (--index != -1)` spelling
still emitted `bgez` and was discarded. No new source operation is supported
by either residue.

The kind-6 radius calculation has a separate instruction-selection gap:
retail shifts the `actor+0x1e` unsigned extent times 25 left by eight,
then arithmetically right by twelve; the candidate reduces the known
nonnegative value to a logical shift by four. An off-tree signed-result
cast compiled byte-identically, so the tracked arithmetic remains as
spelled. The unchanged related `effect_update.c` (ten functions),
`effect_spawn_zero_direction.c` (two), `effect_spawn_motion.c` (two),
`effect_scatter.c` (three), and the spatial-sound helper (one) all retained
fresh strict 100% text certificates from the preceding checkpoint audit.
The adjacent `effect_scale_step` at `0x80041cd0` also remains strict 100%
(172/172 bytes) in a fresh focused, safe one-VA comparison.
