# GAME menu render and card-entry branch

This ten-function GAME pass follows the window renderer, two-option panel,
sprite packet helpers, and the frame entry used by the card/menu controller.
Each function received the matcher address, block disassembly, incoming xref,
callee, string, and match queries. The linked call group is the survey
boundary; proximity alone does not establish a historical source file.

| Address | Evidence and role | Final verdict |
| --- | --- | --- |
| `0x8001fb8c` | draws a title and highlighted rows using the shared layout and sprite table | WIP, 99.787880%; 48-byte retail versus 40-byte compiled frame |
| `0x8001fc94` | menu/card controller calls number, glyph, and primitive helpers, then the empty callback at `0x80021a60` | WIP, unclaimed; 55 CFG blocks and 31 direct calls require menu state ownership |
| `0x80020748` | two-option menu panel through four sprite descriptors | strict exact, 100% |
| `0x8002083c` | item-model preview and two-option graphics | WIP, 99.658820%; unresolved 64-byte frame extent |
| `0x80020990` | numeric overlay using the event counter byte | strict exact, 100% |
| `0x80020b50` | translucent menu sprite packet | strict exact, 100% |
| `0x80020d20` | animated cursor sprite packet | WIP, 99.440680%; horizontal coordinate instruction order |
| `0x80020ef8` | fixed-CLUT cursor sprite packet | WIP, 99.394490%; same horizontal coordinate order |
| `0x80021a60` | empty callback reached by the menu/card controller | **new strict exact, 8/8 code bytes** |
| `0x80021a68` | resets the frame buffers and updates cursor animation | strict exact, 100% |

The new callback is exactly `jr ra; nop` in retail. Its only proven caller is
`0x8001fc94`; no SDK/archive attribution is supported. Its eight-byte claim
immediately precedes `menu_frame_begin` in the same contiguous source. Focused
`kf try` produced 2/2 identical listings; strict objdiff reports 384/384
code bytes and 2/2 functions for the combined unit.

The two sprite helpers agree with retail in CFG, call set, packet fields,
constants, and referents. Their first residue is the horizontal coordinate:
retail loads `position.x`, then `sprite.width`, subtracts the left margin
from the position, and subtracts the width. The current compiler forms
`width + margin` first. Reordering the C subtraction operands was tested in
both helpers; it added a load-delay `nop` and moved the immediate subtraction
after `subu`, worsening both focused listings. The original source expressions
were restored. The window and preview frame extents remain unattributed
codegen residues; no padding local was introduced.

The strict run that admitted `0x80021a60` verified 137/137 GAME target relinks.
It stopped at the repository-wide known-reference ownership closure and two
unrelated map-object `.rodata` addends. No repository tests were run.
