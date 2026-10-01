# GAME menu list and glyph-selection callers

These ten GAME functions form a menu list and item-code caller graph. The
retail address, block disassembly, callers/callees, strings, data references,
current match state, adjacent claims, source history, and vendored census were
checked for each. The five early controllers call the same list/frame helpers;
the later item-list controllers use the two exact initialized item-code pages.
The KF1 menu remains a source-shape control only.

| GAME address | Final verdict | Retail evidence and open owner |
| --- | --- | --- |
| `0x80019834` | **strict exact, 412/412 bytes** | List/input loop uses exact row selector `0x199d0`, list initialization, two frame redraws, and 688 bytes of stack workspace. |
| `0x80019ac4` | **strict exact, 544/544 bytes** | Copies the 200-byte initialized equipment-label table, calls `0x19ce4`, then dispatches to three selection branches. |
| `0x80019ce4` | **100% strict** | New 496-byte C source builds ten 20-byte glyph labels from typed equipped-player IDs. `unknown_98` selects the tenth ID or substitutes `unknown_99`; missing IDs write `-1`. Alternate and base 24-byte source rows come from the existing exact tables. Thirteen player-state relocation pairs and three decoded internal jumps are reviewed. |
| `0x80019ed4` | **strict exact, 1,056/1,056 bytes** | Item/equipment controller loads and releases models, renders the menu, and calls player equip helpers. |
| `0x8001a2f4` | **strict exact, 508/508 bytes** | Second list/input controller uses the exact `0x199d0` selector and `player_set_unknown_97`. |
| `0x8001aa9c` | **strict exact, 484/484 bytes** | Card/menu choice source preserves the direct calls, 136-byte frame, and endless volume fade. |
| `0x8001d030` | **strict exact, 784/784 bytes** | First item-list controller calls the primary code translator, preview model helpers, and menu input/frame helpers. |
| `0x8001d340` | **strict exact, 116/116 bytes** | Primary six-page, 120-entry item-code translator; its initialized table is 1,440/1,440 bytes exact. |
| `0x8001d654` | **strict exact, 84/84 bytes** | Secondary five-page translator; its initialized table is 1,200/1,200 bytes exact. |
| `0x8001d6a8` | **strict exact, 552/552 bytes** | Item-list/model renderer calls the exact selector and menu preview/display helpers. |

Fresh focused builds and direct per-unit objdiff comparisons now establish
**10/10 strict exact** for this survey. The primary and secondary item-code
units retain their 1,440- and 1,200-byte exact initialized tables; the
equipment-list unit also retains its 200-byte exact label table. These current
object comparisons supersede the historical WIP and unclaimed verdicts. No
source edit, linked build, repository test, or banking was done for this
verification.
