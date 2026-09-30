# GAME menu and card controller records

This ten-function GAME campaign follows card and item-list callers into the
shared menu glyph, number, preview, and primitive helpers. Each function was
checked through the retail address, disassembly/CFG, incoming and outgoing
references, strings, and current match queries. No function in this set has
Sony/Psy-Q library-body evidence.

| Address | Retail role and ownership boundary | Final verdict |
| --- | --- | --- |
| `0x8001a4f0` | Item/model list controller calls the row selectors and preview helpers; its `0x800649ec` glyph prefix and large list record need a shared type. | WIP, unclaimed |
| `0x8001ac80` | Card startup and directory/preview flow calls 18 distinct helpers; the card-entry array and lifetime remain unresolved. | WIP, unclaimed |
| `0x8001b2dc` | Seven-row option controller copies six player-state flags, toggles a selected flag, and draws paired glyph labels. | **WIP, 95.601265% strict**; 22/22 CFG blocks and 12/12 branches agree, but the option-count register and resulting schedule differ. |
| `0x8001b554` | Card startup, temporary-file probe, directory scan, input loop, and frame rendering; the card-list record is not typed completely. | WIP, unclaimed |
| `0x8001d030` | Primary item-list branch uses the `0x80065950` page and `0x80065aeb` interior lookup plus preview/menu helpers. | WIP, unclaimed; table and list workspace owner unresolved |
| `0x8001d3b4` | Secondary item-list branch uses the paired translator and a large stack workspace. | WIP, unclaimed; list/model record unresolved |
| `0x8001d6a8` | Item-model selector has a 3,232-byte frame with typed glyph rows, two 120-byte arrays, and an incomplete menu-list record. | WIP, unclaimed; complete list workspace type unresolved |
| `0x8001e94c` | Status/numeric renderer draws glyph suffix rows and player values with menu string/number helpers. | WIP, unclaimed; full label workspace and ordered calls unresolved |
| `0x8001f008` | Straight-line paired component renderer reads player attack/combat halfwords and glyph suffix rows 2 and 3. | WIP, unclaimed; label-prefix copy and full repeated call schedule unresolved |
| `0x8001fc94` | Shared numeric/list renderer emits several FT4 packets and calls the exact primitive-buffer pair. | WIP, unclaimed; complete render record and packet paths unresolved |

The new `0x8001b2dc` source is appended to the contiguous
`game.menu_card_panel` unit. Retail loads `0x80198597..0x8019859c` from
`player_state+0xc7..+0xcc`, then writes those bytes back on exit. The source
uses the established audio option fields and four `unknown_c9` bytes; it does
not define an overlapping BSS object. Its two 28-byte glyph strings and six
local flags account for the observed stack accesses. Twelve decoded HI/LO
pairs were curated to `player_state`, and the direct calls, internal jumps,
and cursor-data pairs in its range were promoted from candidate rows after
instruction review. The source's 22-block CFG and calls match retail, while
the probe reloads the literal option count where retail keeps six in `s4`.
No artificial register carrier was added.

The two preceding functions in `game.menu_card_panel` remain focused SAME.
`kf-retail-validate` passed, strict GAME matching relinked 145/145 units, and
the new function scored 95.601265%. The global edge check retains the same
three unrelated TMD/map-object `.rodata` addends. A full `kf build` after the
source and relocation edits built PSX; GAME, OPEN, and END retained the known
first unresolved `InitCARD`, `malloc`, and `display_buffers` symbols.
