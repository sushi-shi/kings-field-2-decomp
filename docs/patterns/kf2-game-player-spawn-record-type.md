# GAME player spawn-record view at 0x800667a0

The initialized 40-byte GAME datum at `0x800667a0` has five eight-byte
records. Retail `func_80025a18` materializes its base at
`0x8002600c/10`, copies records with an eight-byte stride, and reads
the fourth signed halfword at `0x800260fc` with `lh a2,6(s1)` for an
effect parameter. The next separately claimed four-vector datum begins
at `0x800667c8`, exactly after those five records.

Current `player_state_equipment.c` defines `DAT_800667a0` as
`SVECTOR[5]` under `DATA(0x800667a0, 0x28)` and uses each record's
`pad` halfword as that parameter. The curated data and identity type
columns previously said `s16[5][4]`; they now reflect the established
current C declaration as `SVECTOR[5]`. Name, address, extent, bytes,
source, and address-only tier are unchanged. The original C type and
defining translation unit remain unproved.

A focused quick compile kept 15 of 16 function listings identical;
`func_80025a18` remains a WIP. Isolated strict objdiff reports both
`DAT_800667a0` (40 bytes) and adjacent `DAT_800667c8` (32 bytes) at
100%; `func_80025a18` remains 98.02234% strict. No source claim or
initializer changed.
