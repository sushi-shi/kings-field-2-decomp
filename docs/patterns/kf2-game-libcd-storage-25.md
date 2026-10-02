# GAME CD-adjacent storage: 25-claim exclusion audit

These 25 `GAME.EXE` data-identity candidates occupy `0x8006d718..0x8006d7df`,
after the game-owned zero direction vector at `0x8006d708`. This address band
alone does not identify their defining translation unit. The retail words
below are decoded from the hash-pinned `retail/jp/GAME.EXE` at the exact listed
addresses. Reference counts are **candidate** GAME rows in the curated
relocation inventory, not promoted linker truth. `LIBCD` means that at least
one referring instruction lies within a function classified as exact Psy-Q
3.0 `LIBCD.LIB` text in `functions_vendored.tsv`; `unclassified` sites are
listed separately rather than assigned to the archive by proximity.

| GAME address | Retail bytes/word | Candidate refs | Reference evidence and verdict |
| --- | --- | ---: | --- |
| `0x8006d718` | `0x00000000` | 22 | 20 in exact `LIBCD` (`CD_cachefile`, `CD_newmedia`, `CdSearchFile`, `CD_cw`, `pollintr`); two at `0x80049ff4/ffc` unclassified. No game owner. |
| `0x8006d71c` | `0x00000000` | 3 | All in exact `CdRead`, `cd_cw`, `getintr`; no game owner. |
| `0x8006d778` | `0x1f801800` | 10 | Eight in exact `LIBCD` (`CD_cw`, `CD_read`, `CD_vol`, `getintr`); two unclassified at `0x8004a790/83c`. No game owner. |
| `0x8006d77c` | `0x1f801801` | 3 | All in exact `CD_cw`, `CD_vol`, `getintr`; no game owner. |
| `0x8006d780` | `0x1f801802` | 4 | All in exact `CD_cw`, `CD_vol`, `getintr`; no game owner. |
| `0x8006d784` | `0x1f801803` | 5 | All in exact `CD_read`, `CD_vol`, `getintr`; no game owner. |
| `0x8006d788` | `0x1f801c00` | 2 | Both in exact `CD_init`; no game owner. |
| `0x8006d78c` | `0x00000000` | 7 | All in exact `CD_cw`, `get_alarm`, `getintr`, `pollintr`; no game owner. |
| `0x8006d790` | `0x00000000` | 21 | 20 in exact `LIBCD` functions; one at `0x8004a81c` unclassified. No game owner. |
| `0x8006d794` | `0x00000000` | 12 | 11 in exact `LIBCD` functions; one at `0x8004a7e4` unclassified. No game owner. |
| `0x8006d7a0` | `0x1f801018` | 1 | Exact `CD_read`; no game owner. |
| `0x8006d7a4` | `0x1f801020` | 1 | Exact `CD_read`; no game owner. |
| `0x8006d7a8` | `0x1f8010f0` | 1 | Exact `CD_read`; no game owner. |
| `0x8006d7ac` | `0x1f8010b0` | 1 | Exact `CD_read`; no game owner. |
| `0x8006d7b0` | `0x1f8010b4` | 1 | Exact `CD_read`; no game owner. |
| `0x8006d7b4` | `0x1f8010b8` | 4 | Exact `CD_read`; no game owner. |
| `0x8006d7b8` | `0x00000001` | 2 | Exact `CdSearchFile`; no game owner. |
| `0x8006d7c4` | `2e 00` (`"."`) | 1 | Exact `CD_cachefile`; no game owner. |
| `0x8006d7c8` | `2e 2e 00` (`".."`) | 1 | Exact `CD_cachefile`; no game owner. |
| `0x8006d7ca` | `00` (terminator view) | 1 | Exact `CD_cachefile`; this interior byte is not a separate source object. |
| `0x8006d7cc` | `0x1f801800` | 2 | Only `0x8004c754/788`, outside current exact function spans; CD-adjacent candidate, no owner promotion. |
| `0x8006d7d0` | `0x1f801801` | 1 | Only `0x8004c7bc`, outside current exact function spans; CD-adjacent candidate. |
| `0x8006d7d4` | `0x1f801802` | 0 | No current curated relocation; raw word alone does not prove an object or owner. |
| `0x8006d7d8` | `0x1f801803` | 2 | Only `0x8004c764/908`, outside current exact function spans; CD-adjacent candidate. |
| `0x8006d7dc` | `0x1f801018` | 4 | Only `0x8004c918/9c0/ce78/ceac`, outside current exact function spans; CD-adjacent candidate. |

The first 20 claims have 103 candidate references, of which 97 fall in
exact `LIBCD.LIB` function spans. The five later claims have nine candidate
references, all outside currently classified exact function spans. This
supports excluding the first group from game-data source matching and
preserving the latter group's address-only identity; it does not prove a
historical data-object boundary. The repeated `0x1f8018xx` words, the
`"."`/`".."` bytes, and adjacent exact archive functions are additional
context, not a license to assign an unclassified referring function or
promote a candidate relocation. No source, identity, or relocation row was
changed in this audit.
