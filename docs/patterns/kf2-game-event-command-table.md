# GAME scene-command switch table

`func_8004678c` at `0x8004678c` dispatches command bytes `0x52..0x74` through
the 35-word table at `0x800128d0..0x8001295b`. Retail subtracts `0x52`,
checks the unsigned index against `0x22`, scales by four, loads a word from
the table, and jumps through it. Every curated pointer equals the corresponding
word in the hash-validated Japanese `GAME.EXE`; all 35 targets are aligned
basic-block starts within this function, with 21 distinct destinations.
The table's 35 `mips32_candidate` rows are now `table-reviewed`/`reviewed`.
A one-VA safe carve emitted all 35 `R_MIPS_32` rows in `.rel.rodata` and
withheld zero relocations. These are reviewed table targets, not direct-call
edges or proof of the original source spelling.

The jump table shows shared command handlers. `0x63..0x66`, `0x68`, and
`0x6a..0x6d` enter the same map-object marker scan at `0x800467f8`;
`0x6f..0x71` set offsets `0x28`, `0x2c`, and `0x30` before a shared event-state
path. `0x5a..0x5e` select five eight-byte ID lists at `0x800679a0..0x800679c7`
and join one magic-record scan. `0x53`, `0x5f..0x62`, `0x69`, and `0x6e`
enter the common no-op/exit block. These destinations explain some of the
retail CFG but do not make the existing partial C body complete.

The source currently covers several commands through `0x74` but leaves the
five ID-list arms and other large shared paths WIP. The fresh focused probe
compiles and reports 16.4% listing similarity; the last strict report was
32.816223% and is stale after concurrent source/config edits. No exact claim
or source-body change follows from the table review. KF1 `master` has typed
`map_event.c` and `map_events.c` behavior but no analogous 35-way command
table or body to transplant; KF2 retail instructions and references govern
the remaining reconstruction.
