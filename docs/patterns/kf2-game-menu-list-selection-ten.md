# GAME menu list and glyph-selection callers

These ten GAME functions form a menu list and item-code caller graph. The
retail address, block disassembly, callers/callees, strings, data references,
current match state, adjacent claims, source history, and vendored census were
checked for each. The five early controllers call the same list/frame helpers;
the later item-list controllers use the two exact initialized item-code pages.
The KF1 menu remains a source-shape control only.

| GAME address | Final verdict | Retail evidence and open owner |
| --- | --- | --- |
| `0x80019834` | WIP, unclaimed | A 0x19c-byte list/input loop uses the exact row selector `0x199d0`, list initialization, two frame redraws, and 688 bytes of stack workspace. The complete list output record, including pointer fields beyond the proved 36-byte prefix, remains unresolved. |
| `0x80019ac4` | WIP, unclaimed | Calls the now-exact `0x19ce4` row builder twice, then dispatches to three selection branches. Its loaded record at `0x80064910` and complete list workspace need an owner. |
| `0x80019ce4` | **100% strict** | New 496-byte C source builds ten 20-byte glyph labels from typed equipped-player IDs. `unknown_98` selects the tenth ID or substitutes `unknown_99`; missing IDs write `-1`. Alternate and base 24-byte source rows come from the existing exact tables. Thirteen player-state relocation pairs and three decoded internal jumps are reviewed. |
| `0x80019ed4` | WIP, unclaimed | Item/equipment controller loads and releases models, renders the menu, and calls player equip helpers. Indirect dispatch and the complete list/model state remain unresolved. |
| `0x8001a2f4` | WIP, unclaimed | Second list/input controller uses the exact `0x199d0` selector and `player_set_unknown_97`; its copied-row and output-record ownership remain open. |
| `0x8001aa9c` | WIP, **97.305786% strict** | Card/menu choice source preserves the direct calls, 136-byte frame, and endless volume fade. Focused CFG is 24 versus retail 25 blocks: the result-join branch reloads a stack word along a different path. A nested-condition source probe compiled identically and was reverted. |
| `0x8001d030` | WIP, unclaimed | First item-list controller calls the primary code translator, preview model helpers, and menu input/frame helpers. Its 0x310-byte body and list table at `0x80065950` need complete record ownership. |
| `0x8001d340` | WIP, **93.10345% strict** | Primary six-page, 120-entry item-code translator; 1440/1440 initialized bytes remain exact. Focused CFG is 4/4 blocks and the residual moves one shift across the table-base `lui/addiu`. No source fact supports a different indexing expression. |
| `0x8001d654` | WIP, **90.47619% strict** | Secondary five-page translator; 1200/1200 initialized bytes remain exact. It has the same one-shift/table-base scheduling residue and matching four-block CFG. |
| `0x8001d6a8` | WIP, unclaimed | Item-list/model renderer calls the exact selector and menu preview/display helpers. Its 0x228-byte body uses a large list/model workspace without a proved complete type. |

The new `game.menu_glyph_selection_rows` unit has a strict 100% function and
144/144 GAME target units relinked. Global known-reference closure still stops
at three unrelated TMD/map-object `.rodata` addends. Full `kf build` after the
new source built PSX.EXE; GAME/OPEN/END retain their known unresolved link
symbols. No repository tests or banking were run for this batch.
