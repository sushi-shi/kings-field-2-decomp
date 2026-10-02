# GAME occupancy BSS boundary: 25 direct targets

This read-only `GAME.EXE` cohort covers 25 distinct target addresses in
`0x801c7068..0x801d8d82`. The curated relocation inventory has 169
direct HI16/LO16 pairs to them, all `paired-reviewed`/reviewed. The
counts below describe direct source sites, not independently allocated
globals. No claim is split at an interior target.

| GAME target | Existing object/interior offset | Reviewed pairs |
| --- | --- | ---: |
| `0x801c7068` | `DAT_801c7068` base | 2 |
| `0x801c706a` | `DAT_801c7068+0x2` | 3 |
| `0x801c706c` | `DAT_801c7068+0x4` | 3 |
| `0x801c7078` | `player_weapon_records` base | 3 |
| `0x801c7540` | `bss_801c7540+0x0` | 18 |
| `0x801c7544` | `bss_801c7540+0x4` | 3 |
| `0x801c7547` | `bss_801c7540+0x7` | 1 |
| `0x801d7540` | `bss_801c7540+0x10000` | 3 |
| `0x801d8ae8` | `bss_801c7540+0x115a8` | 8 |
| `0x801d8d40` | `bss_801c7540+0x11800` | 6 |
| `0x801d8d44` | `bss_801c7540+0x11804` | 5 |
| `0x801d8d4a` | `bss_801c7540+0x1180a` | 19 |
| `0x801d8d4c` | `bss_801c7540+0x1180c` | 13 |
| `0x801d8d50` | `bss_801c7540+0x11810` | 28 |
| `0x801d8d54` | `bss_801c7540+0x11814` | 6 |
| `0x801d8d58` | `bss_801c7540+0x11818` | 8 |
| `0x801d8d5c` | `bss_801c7540+0x1181c` | 3 |
| `0x801d8d60` | `bss_801c7540+0x11820` | 9 |
| `0x801d8d64` | `bss_801c7540+0x11824` | 8 |
| `0x801d8d68` | `bss_801c7540+0x11828` | 4 |
| `0x801d8d70` | `bss_801c7540+0x11830` | 6 |
| `0x801d8d74` | `bss_801c7540+0x11834` | 1 |
| `0x801d8d78` | `bss_801c7540+0x11838` | 3 |
| `0x801d8d80` | `bss_801c7540+0x11840` | 4 |
| `0x801d8d82` | `bss_801c7540+0x11842` | 2 |

The existing eight-byte `DAT_801c7068` motion vector ends at
`0x801c7070`; the eight bytes before `player_weapon_records` at
`0x801c7078` remain unowned. Eighteen 68-byte weapon records occupy
exactly `0x4c8` bytes through `0x801c7540`, where the separate
startup-cleared BSS claim begins. These are current source/carving
boundaries, not proof of the original allocation classes.

The phase-one resource copy writes `0x600` words (`0x1800` bytes) to
`bss_801c7540+0x10000` and ends at `+0x11800`. Retail collision-shape
code uses that base; reviewed collision/player sites then access cache
fields from `+0x11800` onward. The highest reviewed direct field access
is the halfword at `+0x11842`: `sh` at `0x8002b9c4` and `lhu` at
`0x80027c90` touch through `+0x11844`, the current claim's end.

The source's provisional `[88][80]` map grid extends into the loaded
shape-bank range, while its 20-record equipment view beginning at
`+0x115a8` crosses the end of that bank and the start of the cache.
These overlapping views do not establish simultaneous independent
allocations. The complete original BSS extent, internal unions, and
defining translation units remain unresolved. This pass made no source,
identity, or relocation change; it required no build.
