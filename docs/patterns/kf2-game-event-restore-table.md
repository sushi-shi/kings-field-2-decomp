# GAME event-restore switch table

`func_800489ac` (`GAME.EXE` `0x800489ac`, 0x378 bytes) restores actor and
map-object state from a saved byte stream. Retail first scans two 0xff-ended
actor/target lists, then processes 396 map-object records. The map opcode
subtracts `0xf0`, bounds the result to 0..15, shifts it by two, loads a word
from `event_restore_opcode_jump_table` at `0x80012bf8`, and jumps through that
word. The table has exactly 16 retail words; each points to a decoded label
inside this function. The table-base HI16/LO16 pair is already validated.

Those 16 `mips32_candidate` rows are now `pointer-reviewed`/`reviewed` while
retaining their source kind. A focused safe delink of this VA emits 16
`R_MIPS_32` entries in `.rel.rodata`, with zero withheld relocations or
functions. `kf sema --confirmed-only` reports all 16 incoming table pointers
as validated. This proves the table's local switch role, not any target for an
unrelated indirect callback.

The function remains WIP (current strict report 98.82883%). The focused
source rebuild preserves the call set, table referent, switch structure and
saved-stream behavior. Its first difference assigns the actor base and
0xff sentinel to `$a2`/`$a1` instead of retail's `$a1`/`$a2`, then changes
the following group-loop instruction schedule. There is no independently
supported C correction for that residue; no register steering was retained.
