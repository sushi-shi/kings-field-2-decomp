# GAME card, menu, and transition ten

These ten non-exact GAME functions form a card/menu caller graph, with the fade
transition used by its menu path. Retail disassembly, CFG, caller/callee edges,
string and data references, current source/history, match state, and the
vendored inventory were inspected for each. None has a supported Psy-Q archive
body attribution; the card and graphics library calls are external boundaries.

| GAME address | Retail evidence and current source | Final verdict |
| --- | --- | --- |
| `0x8001876c` | Main menu selection loop calls the card choice controllers and redraws two frames. Current source has 34/34 CFG blocks and the same known successors; the indirect jump remains unresolved. | **WIP, 96.36646% strict**; `-1` and `-99` occupy exchanged saved registers, and the final result checks use a different load schedule. Exact adjacent `0x189f0` is preserved. |
| `0x8001a898` | The menu controller builds a 3240-byte frame, calls glyph-row and list setup, loads a selected item model, then runs a two-frame input loop. The menu path calls it directly. | **WIP, unclaimed**; its large stack workspace and list/selection fields need a complete shared model before a truthful source claim. The 0x8009a5e8 counter block already has a shared owner. |
| `0x8001aa9c` | Card choice controller calls card menu children, polls selection, and on result `-2` stops CD and fades sequence volume. | **WIP, 97.305786% strict**; 12/12 branches agree, but retail reloads the result after the redraw loop, yielding 25 CFG blocks versus 24 compiled. An equivalent `else if` probe emitted the same bytes. |
| `0x8001d340` | Copies indexed bytes and looks up halfword codes in the 6-by-120 primary item-code table; three menu callers are confirmed. | **WIP, 93.10345% strict**; 4/4 CFG blocks agree, with only the table stride's last shift scheduled after the table-base HI16/LO16 in retail. |
| `0x8001d654` | Looks up halfword codes in the 5-by-120 secondary table; two menu callers are confirmed. | **WIP, 90.47619% strict**; the same one-instruction table-stride scheduling residue remains. Equivalent typed linear indexing did not change the listing. |
| `0x800226ec` | Enumerates `DIRENTRY` records, checks the `BISLPS-00069` prefix, sorts fifteen slots, and totals size. Card menu callers are proven. | **WIP, 93.60504% strict**; 13/13 CFG blocks and call set agree, while the opening `memset` argument/store order and source byte-load extension differ. The adjacent card-byte owner at 0x8006d6a4 remains unresolved. |
| `0x800228c8` | Reads a `bu00:` card header and decodes experience, level, and slot fields through its 32-bit output pointers. | **WIP, 85.15625% strict**; 24/24 CFG blocks agree, but card-byte initialization and digit-loop register/schedule differences remain. A signed local-byte probe did not change the first divergence. |
| `0x80022b74` | Retries card-file open/read, checks the payload byte sum, and calls the player-state restore walker on success. | **WIP, 93.666664% strict**; 9/9 CFG blocks agree with instruction-order and delay-slot residues. Exact `memory_card_format` in the same unit is preserved. |
| `0x80023178` | Formats the player experience and level into card-label glyph bytes; the card preparation function calls it. | **WIP, 93.52941% strict**; 19/19 CFG blocks agree, but the compiled version keeps the label in `a0` and the first quotient in `a2`, while retail moves the label to `a3` and uses `a0`. An equivalent pointer-alias probe emitted the same bytes. Three exact wait/checksum siblings are preserved. |
| `0x800349bc` | Fade transition emits four textured quads per frame, polls pad input, and returns at a level boundary; the exact upload function calls it. | **WIP, 92.34296% strict**; 14/14 CFG blocks agree, but the quad/present branch layout differs. The exact adjacent TIM upload and transition caller are preserved. |

Focused source-only probes were discarded when their listings stayed non-exact.
No shared source, identity, relocation, or unit claim changed in this batch.
A fresh strict `kf match --image game` relinked all 145/145 target units and
confirmed every score above. Global edge-check still stops on the three known,
unrelated TMD/map-object `.rodata` addends. No repository tests, bank, or commit
were run.
