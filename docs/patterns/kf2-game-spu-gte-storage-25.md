# GAME SDK storage boundary: 25-target audit

This read-only `GAME.EXE` audit covers 25 address-only load-image data
identities at `0x8006d810..0x8006d87c`. The words below come from the
hash-pinned retail image. Reference counts are curated **candidate**
HI16/LO16 rows, not proved original relocations. An SDK count means that
the referring instruction lies within an exact Psy-Q 3.0 function in
`functions_vendored.tsv`; an unclassified count has no such proof.

| GAME target | Retail word | Candidate refs | Exact SDK refs | Unclassified refs |
| --- | --- | ---: | --- | ---: |
| `0x8006d810` | `0x1f801070` | 1 | — | 1 |
| `0x8006d814` | `0x1f801074` | 1 | — | 1 |
| `0x8006d820` | `0x00000000` | 5 | — | 5 |
| `0x8006d824` | `0x00000000` | 2 | — | 2 |
| `0x8006d828` | `0x00000000` | 2 | — | 2 |
| `0x8006d82c` | `0x00000000` | 10 | — | 10 |
| `0x8006d830` | `0x00000000` | 7 | — | 7 |
| `0x8006d838` | `0x1f801c00` | 1 | LIBSPU 1 | 0 |
| `0x8006d83c` | `0x00000000` | 4 | LIBSPU 2 | 2 |
| `0x8006d840` | `0x1f801c00` | 84 | LIBSPU 80 | 4 |
| `0x8006d844` | `0x1f8010c0` | 3 | LIBSPU 2 | 1 |
| `0x8006d848` | `0x1f8010c4` | 3 | LIBSPU 2 | 1 |
| `0x8006d84c` | `0x1f8010c8` | 3 | LIBSPU 2 | 1 |
| `0x8006d850` | `0x1f8010f0` | 4 | LIBSPU 3 | 1 |
| `0x8006d854` | `0x1f8010f4` | 3 | LIBSPU 2 | 1 |
| `0x8006d858` | `0x1f801014` | 3 | LIBSPU 2 | 1 |
| `0x8006d85c` | `0x1f801c00` | 3 | LIBSPU 3 | 0 |
| `0x8006d860` | `0x1f801c00` | 1 | LIBSPU 1 | 0 |
| `0x8006d864` | `0x00000000` | 14 | LIBSPU 5 | 9 |
| `0x8006d868` | `0x00000000` | 32 | LIBSPU 16 | 16 |
| `0x8006d86c` | `0x1f801c00` | 1 | LIBSPU 1 | 0 |
| `0x8006d870` | `0x00000000` | 6 | LIBSND 6 | 0 |
| `0x8006d874` | `0xffffffff` | 3 | LIBSND 3 | 0 |
| `0x8006d878` | `0x1f801c00` | 16 | LIBSND 11 | 5 |
| `0x8006d87c` | `0x801b5aa0` | 32 | LIBGTE 32 | 0 |

Of 244 candidate references, 122 fall in exact LIBSPU, 20 in exact
LIBSND, 32 in exact LIBGTE, and 70 in unclassified text. The first seven
targets have 28 unclassified references in `0x8004c744..0x8004d250`,
adjacent to known LIBCD functions but outside their exact spans. The
zero-valued words do not establish separate source objects, and neither
adjacency nor register-like values prove the original defining module.
The last word points to `0x801b5aa0`, 16 bytes beyond the claimed
`render_mask_scan_state` end at `0x801b5a90`, and is referenced only by
exact LIBGTE clipping functions. It does not extend the game-owned mask
state or prove a game-owned BSS object at that address. All 25 identities
remain address-only. No source,
identity, or relocation row changed, and no build was needed for this
read-only classification.
