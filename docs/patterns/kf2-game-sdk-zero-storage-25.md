# GAME SDK-adjacent zero storage: 25-claim audit

This second, disjoint 25-claim GAME cohort covers curated address-only
identities at `0x8006da60..0x8006db2b`. All 25 retail words begin with zero
bytes at their claimed address in the hash-pinned Japanese `GAME.EXE`.
That byte value does not distinguish a real four-byte object from an interior
view, padding, or a larger zero-initialized allocation. The counts below are
candidate HI16/LO16 references in `config/retail/relocs.tsv`, not admitted
original relocations. The source-site class comes from
`functions_vendored.tsv`: `MALLOC` is a 1,548-byte Sony `MALLOC.OBJ`
wildcard-object signature with 1,336 fixed bytes, `LIBCD` means an exact
Psy-Q 3.0 archive function, and `unclassified` means no admitted exact
function contains that site.

| GAME address | Candidate refs | Source-site evidence | Owner verdict |
| --- | ---: | --- | --- |
| `0x8006da60` | 12 | Sony `MALLOC.OBJ`: `_ExpAllocArea`, `_expand`, `InitHeap`, `malloc`. | No game owner; original datum boundary WIP. |
| `0x8006da68` | 2 | Sony `MALLOC.OBJ`: `_ExpAllocArea`, `InitHeap`. | No game owner; boundary WIP. |
| `0x8006da70` | 5 | Sony `MALLOC.OBJ`: `_ExpAllocArea`, `_expand`, `InitHeap`, `malloc`. | No game owner; boundary WIP. |
| `0x8006da78` | 3 | Sony `MALLOC.OBJ`: `InitHeap`, `malloc`. | No game owner; boundary WIP. |
| `0x8006da80` | 9 | Exact `LIBCD` `getintr`. | No game owner. |
| `0x8006da81` | 0 | No current curated direct reference; adjacent to `0x8006da80`. | Remains address-only; no separate datum inferred. |
| `0x8006da88` | 1 | Exact `LIBCD` `getintr`. | No game owner. |
| `0x8006da90` | 3 | Exact `LIBCD` `getintr`. | No game owner. |
| `0x8006da98` | 7 | Exact `LIBCD` `getintr`. | No game owner. |
| `0x8006daa0` | 5 | Exact `LIBCD` `getintr`. | No game owner. |
| `0x8006daa8` | 7 | Six exact `LIBCD` refs; one unclassified `0x8004a824`. | No game owner; unknown site unresolved. |
| `0x8006dab0` | 6 | Five exact `LIBCD` refs; one unclassified `0x8004a7ec`. | No game owner; unknown site unresolved. |
| `0x8006dab8` | 2 | Exact `LIBCD` `get_alarm`, `set_alarm`. | No game owner. |
| `0x8006dac0` | 3 | Exact `LIBCD` `CD_cachefile`, `CD_newmedia`. | No game owner. |
| `0x8006dac8` | 14 | Only unclassified `0x8004c8ac..0x8004d020` sites. | Archive/extent candidate; no owner promotion. |
| `0x8006dad0` | 4 | Only unclassified `0x8004c458..0x8004ce58` sites. | Archive/extent candidate. |
| `0x8006dad8` | 15 | Only unclassified `0x8004c444..0x8004d038` sites. | Archive/extent candidate. |
| `0x8006dae0` | 25 | Only unclassified `0x8004c4b8..0x8004cfec` sites. | Archive/extent candidate. |
| `0x8006daf0` | 6 | Only unclassified `0x8004c2e4..0x8004ce2c` sites. | Archive/extent candidate. |
| `0x8006daf8` | 14 | One exact `data_ready_callback`; 13 unclassified `0x8004c2fc..0x8004d044` sites. | No game owner; full archive boundary WIP. |
| `0x8006db00` | 9 | Two exact `data_ready_callback`; seven unclassified `0x8004c2f4..0x8004ce08` sites. | No game owner; boundary WIP. |
| `0x8006db10` | 5 | Only unclassified `0x8004c6ac..0x8004ccf0` sites. | Archive/extent candidate. |
| `0x8006db18` | 2 | Only unclassified `0x8004c6b4/0x8004c9f4`. | Archive/extent candidate. |
| `0x8006db20` | 3 | Only unclassified `0x8004c6bc..0x8004cccc` sites. | Archive/extent candidate. |
| `0x8006db28` | 3 | One exact `data_ready_callback`; two unclassified `0x8004c44c/0x8004c4c8`. | No game owner; boundary WIP. |

There are 165 candidate references: 22 from the Sony `MALLOC.OBJ` signature
run, 45 inside exact `LIBCD.LIB` functions, and 98 from unclassified sites.
The latter cluster lies beside exact `LIBCD` functions but its own archive
member and function boundaries remain unproved. These zero words must not
be turned into 25 separate game globals or promoted merely because a
Ghidra data census split them. No source, data-identity, relocation, or
function-vendored row changed in this audit.
