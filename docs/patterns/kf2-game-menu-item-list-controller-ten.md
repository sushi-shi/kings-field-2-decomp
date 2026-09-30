# GAME item-list controller and renderer ten

This batch examined ten non-exact GAME functions linked by the menu-list
initializer, item-code translators, model preview, input service, and glyph
renderer. Each received the six `kf sema --image game` evidence queries:
address, block disassembly, callers, callees, strings, and match state. None
has a retail string reference. The King's Field I `menu_list_render` source is
a shape comparison for `0x8001fc94`, not proof that its full body or TU was
shared with GAME II.

| GAME address | Retail evidence and unresolved ownership | Final strict verdict |
| --- | --- | --- |
| `0x8001aa9c` | Existing card-choice controller has the right calls and 12/12 branches, but retail reloads the result after the redraw join; compiled C keeps it in a register and has 24 versus 25 blocks. A source-equivalent `else if` probe did not change the listing. | **WIP, 97.305786%** |
| `0x8001d030` | A C claim reconstructs its 40 glyph rows, 40 byte values, 40 word codes, 40 indices, primary translator, preview/input calls, and item purchase path. The indexed page and byte at `0x80065aeb` belong to one initialized six-page, 120-byte-per-page table; the mutable multiplier at `0x8006d694` still lacks whole-object ownership. Twenty-four direct control-flow and eight BSS relocation rows were checked against retail, leaving zero withheld rows in a focused carve. Counter and code pointers, selected-item lifetime, word-width quantity arithmetic, and the combined purchase rejection branch now explain the frame, referents, and CFG. The remaining listing difference is price/quantity load order and temporary register assignment in one purchase expression. | **WIP, 96.6% focused listing** |
| `0x8001d3b4` | Source claims the 3,720-byte frame: 120 glyph rows, 120 byte values, 120 word codes, and 120 indices after a 52-byte list. It calls the secondary translator, checks stock against the shared `0x8006d694` quantity, and credits player gold on sale. Retail reloads the list's stored entry count before preview setup; expressing that field access yielded an identical focused 0x2a0-byte listing. Twenty direct rows and three signed-low BSS pairs were decoded against raw retail; the one-VA safe carve withheld none. The multiplier's owning datum remains unresolved. | **Focused SAME, 1/1** |
| `0x8001d6a8` | Appended to the adjacent secondary-code unit. The 3,232-byte frame, 120 rows/byte values/indices, 14 direct calls, selected-item byte width, and the `game_counter_bytes` HI/LO referent are confirmed by retail. A source-only `u8` probe made its listing identical; strict comparison retained it. | **Exact, 100%** |
| `0x8001d8d0` | Source claims its 1,328-byte frame, 40-row list, two-frame opening, item trade checks, special item `0x75` tenfold quantity, and menu sequence transition. Its indexed page at `0x80065b30` belongs to `menu_item_mask_pages[4]`; the mutable `0x8006d694` multiplier still lacks a complete owner. Thirty-two direct rows plus the signed-low counter pair were decoded against retail, and its one-VA safe carve withheld none. Focused compilation emitted an identical 0x394-byte listing. | **Focused SAME, 1/1** |
| `0x8001ddd0` | Source claims the 1,328-byte item-list controller with the primary translator, preview/input path, purchase checks, and page `menu_item_mask_pages[5]` at `0x80065ba8`. Twenty-one direct rows and three signed-low BSS pairs were checked against raw retail; its one-VA safe carve withheld none. The 0x2d8-byte focused listing has 22/22 CFG blocks and 12/12 branches, with only the price/quantity load and temporary-register order divergent. The mutable multiplier still lacks a complete owner. | **WIP, 96.2% focused listing** |
| `0x8001e0a8` | Source claims its 1,320-byte frame, 40-row list, secondary code translator, item-model/input calls, and two-frame redraw. Twenty direct rows and three signed-low BSS pairs were checked against raw retail; its one-VA safe carve withheld none. The focused listing has 22/22 CFG blocks and 12/12 branches, with the same price/quantity load and temporary-register residue as `0x8001ddd0`. The mutable multiplier still lacks a complete owner. | **WIP, 96.2% focused listing** |
| `0x8001e484` | Common 48-byte-frame input service has 66 CFG blocks and at least 16 direct callers. It receives a list, a separate byte-index array, and two word outputs; it updates cursor fields `+0x1e..+0x22`, model preview state, and sound cues. Input-state transitions and the shared multiplier owner remain unresolved. | **WIP, unclaimed** |
| `0x8001e94c` | Repeated number/glyph renderer uses `menu_sprite_defs`, 20-byte label suffixes at `0x80064a00..0x80064adc`, player values, and `menu_format_number`. Its source now emits an identical focused listing; label ownership remains a separate data-model question. | **Exact, 100% focused** |
| `0x8001fc94` | 55-block list renderer uses the list's row/value/code pointers, sprite descriptors, exact primitive-buffer helpers, and mode-specific number and glyph paths. The KF1 renderer confirms a related loop shape, but the complete GAME II record and packet branches remain unresolved. | **WIP, unclaimed** |

`KfItemMenuList` is a distinct 52-byte list view: the common `KfMenuList`
prefix, a glyph-row pointer at `+0x24`, a byte-value pointer at `+0x2c`, and
a word-code pointer at `+0x30`. This is supported by direct stack stores in
`0x8001d030` and `0x8001d3b4`; `0x8001d6a8` uses only the first two extra
pointers. The unrelated `KfMagicMenuList` retains its signed-word values at
`+0x30`. `0x8001e484` receives the index array in `a1`, distinct from the
byte-value pointer stored in the list at `+0x2c`.

The focused 0x8001d6a8 listing and subsequent strict GAME report are exact;
the adjacent 0x8001d654 translator remains WIP at 95.7% focused similarity
because its final stride shift sits on the other side of table-base setup.
The final GAME report relinked 149/149 units and had 391 exact of 461 scored.
Global edge closure still stops at three established unrelated `.rodata`
addends in TMD/map-object units. The required full `kf build` built PSX;
GAME, OPEN, and END retained their established first unresolved link symbols
`InitCARD`, `malloc`, and `display_buffers`. No repository tests, banking, or
commit were performed.
