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

The source now models the five ID-list arms and claims their five contiguous
eight-byte load-data records in the event-command unit. Direct retail bytes
are `07 08 09 0a ff 00 00 00`, `0e 0f 00 0d ff 00 00 00`,
`10 01 02 03 ff 00 00 00`, `04 11 05 06 ff 00 00 00`, and
`12 13 0b 0c ff ff ff ff`. Each list scans 26-byte magic records until
the first unavailable ID, sets its byte flag, allocates map-object slot
`0x15e..0x167`, and animates the object between the two camera-relative
positions with the retail `0..4096` fraction and three per-frame service
loops. The source's identity names remain address-derived, and original
data TU ownership is unresolved; this is a provisional unit ownership claim.

The focused probe reported **33.3%** listing similarity after the magic-ID
arm, up from **16.4%** before it. An isolated one-VA safe carve and direct section
comparison confirm the new `.data` contribution matches **40/40 bytes**;
the 140-byte `.rodata` jump table still differs in code-target addends because
the switch body is incomplete.

The `0x6f..0x71` transition path is also now modeled from the distinct
`0x469fc/0x46a04/0x46a0c` table entries: it spends ten attack-charge units,
waits for two resource transitions, moves the player to a map object's pose,
and queues asset-registry slot `0x181` if absent. That slot is the typed
`game_graphics_runtime.asset_registry_entries[0x181]`, whose offset is
`0x10720`, matching the retail load. After this arm the C emits 3140 bytes
against the 3156-byte retail body. Reordering the case groups into the raw
retail body sequence raises focused listing similarity from **28.5% to 61.9%**
without changing behavior. Spelling the shared object-action status as a
`switch` instead of an if-chain then raises it to **66.0%**; isolated strict
objdiff now gives **82.368820%** `.text` and **100%** `.data`. The first
remaining difference is the saved-register frame/register setup, followed
by CFG and referent residues. The 35 table
entries now have explicit C actions or verified no-op behavior, but the strict
whole-function verdict remains WIP. KF1 `master`'s
`src/game/map_scripts.c` (including history commit `bf051cfa`) gives useful
typed examples of magic-record updates, map-object acquisition, and blocking
animation loops; KF1's matched switch-source ordering example also predicted
the block-order improvement. Its command topology and parameters differ, so
all kept fields, constants, calls, and loop bounds here come from KF2 retail.

Fourteen previously absent adjacent HI16/LO16 pairs inside this dispatcher
are now reviewed: map-object, player, and event-state bases at raw sites
`0x4683c..0x472a0`. Each target/addend is inside an established typed owner;
the one-VA safe carve emits all fourteen pairs with zero withheld rows. These
curations restore real source referents in the target object rather than
removing the C references to accommodate an incomplete delink.
