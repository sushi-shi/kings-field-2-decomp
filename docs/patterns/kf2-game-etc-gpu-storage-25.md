# GAME ETC/GPU storage boundary: 25-target audit

This read-only `GAME.EXE` cohort covers 25 address-only load-image
identities at `0x8006d884..0x8006d8e4`, immediately after the separately
audited SPU/GTE band. The words are from the hash-pinned retail image.
Reference counts are curated **candidate** HI16/LO16 rows; the exact
Psy-Q counts classify referring instructions within archive-matched
functions, not the data's original allocation.

| GAME target | Retail word | Candidate refs | Exact SDK refs | Unclassified refs |
| --- | --- | ---: | --- | ---: |
| `0x8006d884` | `0x1f801114` | 1 | LIBETC 1 | 0 |
| `0x8006d888` | `0x1f801118` | 1 | LIBETC 1 | 0 |
| `0x8006d88c` | `0x00000000` | 3 | LIBETC 2 | 1 |
| `0x8006d890` | `0x00000000` | 14 | LIBCD 4, LIBETC 5, LIBGPU 2 | 3 |
| `0x8006d894` | `0x8006d4d0` | 7 | LIBETC 6 | 1 |
| `0x8006d898` | `0x00000000` | 4 | LIBETC 3 | 1 |
| `0x8006d89c` | `0x00000000` | 3 | LIBETC 3 | 0 |
| `0x8006d8a0` | `0x1f801070` | 5 | LIBETC 4 | 1 |
| `0x8006d8a4` | `0x1f801074` | 6 | LIBETC 5 | 1 |
| `0x8006d8a8` | `0x00000000` | 8 | LIBETC 7 | 1 |
| `0x8006d8ac` | `0x1f8010f4` | 9 | LIBETC 2 | 7 |
| `0x8006d8b0` | `0x00000000` | 3 | LIBETC 2 | 1 |
| `0x8006d8b4` | `0x1f801814` | 3 | LIBETC 3 | 0 |
| `0x8006d8b8` | `0x1f801110` | 2 | LIBETC 2 | 0 |
| `0x8006d8bc` | `0x00000000` | 2 | LIBETC 2 | 0 |
| `0x8006d8c0` | `0x00000000` | 2 | LIBETC 2 | 0 |
| `0x8006d8c4` | `0x8006d5bc` | 27 | LIBGPU 23 | 4 |
| `0x8006d8c8` | `0x8005f430` | 18 | LIBGPU 1 | 17 |
| `0x8006d8cc` | `0x00000000` | 5 | LIBGPU 3 | 2 |
| `0x8006d8d0` | `0x00000000` | 25 | LIBGPU 17 | 8 |
| `0x8006d8d4` | `0x00000000` | 10 | LIBGPU 5 | 5 |
| `0x8006d8d8` | `0x00000000` | 3 | LIBGPU 1 | 2 |
| `0x8006d8dc` | `0x1f801810` | 13 | LIBGPU 3 | 10 |
| `0x8006d8e0` | `0x1f801814` | 26 | LIBGPU 15 | 11 |
| `0x8006d8e4` | `0x1f8010a0` | 4 | LIBGPU 2 | 2 |

Of 204 candidate references, 4 lie in exact LIBCD, 50 in exact
LIBETC, and 72 in exact LIBGPU functions. The remaining 78 come from
unclassified source sites, mostly in the nearby SDK-shaped text band;
address proximity cannot assign those sites to a vendor archive. The
hardware-register values and pointer-shaped retail words likewise do
not prove a standalone source object. In particular, `0x8006d8c8`
contains `0x8005f430`, which the vendored inventory identifies as
Sony `LIBC.LIB` `printf` by a unique 16/16-byte signature; its 17
unclassified readers still cannot be assigned a defining archive.
All 25 identities remain
address-only; no data, relocation, source, or BSS claim was changed.
