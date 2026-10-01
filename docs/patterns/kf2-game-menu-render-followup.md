# GAME menu renderer follow-up

This 28-function pass follows the item-list, card-label, window, sprite, and
display calls surrounding the exact menu-label units. Each address received
the matcher address, retail disassembly/CFG, incoming-xref, callee, string,
and match-state queries. Call edges and shared typed menu data define the
campaign; adjacency alone does not establish a translation-unit boundary.

| GAME address | Retail role or ownership issue | Verdict |
| --- | --- | --- |
| `0x8001ddd0` | first item-list controller, 14 direct calls | WIP: list record and table ownership |
| `0x8001e0a8` | paired item-list controller, 14 direct calls | WIP: list record and table ownership |
| `0x8001e378` | menu input poll | **exact**, existing source |
| `0x8001e484` | menu/model input controller | WIP: 66-block state flow |
| `0x8001e94c` | numeric text renderer | WIP: 30-call glyph/data owner |
| `0x8001f008` | second numeric text renderer | WIP: 55-call glyph/data owner |
| `0x8001f798` | paired, six-row menu labels | **exact, 288/288 bytes**, newly resolved |
| `0x8001f8b8` | menu model/preview controller | WIP: list and model record |
| `0x8001fb8c` | window title and highlighted rows | WIP: 99.78788%, frame extent |
| `0x8001fc94` | numeric/list renderer | WIP: record and data owner |
| `0x80020748` | two-option widget | **exact**, existing source |
| `0x8002083c` | menu model preview | WIP: 99.65882%, frame extent |
| `0x80020990` | numeric heading | **exact**, existing source |
| `0x80020b50` | translucent sprite packet | **exact**, existing source |
| `0x80020d20` | cursor sprite packet | **exact, 472/472 code bytes** in current direct objdiff |
| `0x80020ef8` | fixed-CLUT sprite packet | **exact, 436/436 code bytes** in current direct objdiff |
| `0x800210ac` | glyph-string packets | WIP: 99.66904%, frame/register choice |
| `0x80021510` | number-glyph packets | WIP: 98.61957%, atlas-U load/order |
| `0x800217f0` | nine-slice panel | **exact**, existing source |
| `0x80021a60` | eight-byte return stub | **exact**, source-owned with adjacent frame-begin function |
| `0x80021a68` | frame begin | **exact**, existing source |
| `0x80021be0` | frame present | **exact**, existing source |
| `0x80021c8c` | display state entry | WIP: 99.956985%, frame extent |
| `0x80021e00` | display exit | **exact**, existing source |
| `0x80021f10` | start quad primitive | **exact**, existing source |
| `0x80021f60` | commit quad primitive | **exact**, existing source |
| `0x80021fb0` | initialize menu list | **exact**, existing source |
| `0x80022058` | decimal number formatter | WIP: 90.94%, loop/frame structure |

The paired-row renderer uses two glyph strings and six selection bytes,
calling the fixed-CLUT, translucent, and string renderers on each row. Its
retail CFG, call order, and 26-pixel row advance were already represented in
`game.menu_pair_rows`. Advancing the selection pointer before incrementing the
independent row counter gives the retail instruction order without changing
the source behavior. Focused `kf try` reports SAME; strict objdiff reports
one exact function and 288/288 code bytes.

The existing window, preview, and display-state frame differences have no
proven source extent; no padding was added. The two cursor blitters retain the
shared signed point type used by the exact translucent sibling, and fresh
focused builds plus direct objdiff confirm both blitters exact. The eight-byte
`0x80021a60` stub and adjacent frame-begin body now form one source unit;
fresh focused comparison reports both listings identical. The larger
item-list and text renderers have separate later source claims and verdicts.

The strict run verified 152/152 GAME target relinks. It exited at incomplete
known-reference data ownership and two unrelated map-object `.rodata` addend
divergences; the `game.menu_pair_rows` objdiff result itself is 100%.
