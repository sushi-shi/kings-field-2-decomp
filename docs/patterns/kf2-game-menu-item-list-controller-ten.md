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
| `0x8001d030` | A first-pass C claim reconstructs its 40 glyph rows, 40 byte values, 40 word codes, 40 indices, primary translator, preview/input calls, and item purchase path. The indexed page and byte at `0x80065aeb` now belong to one initialized six-page, 120-byte-per-page table; the mutable multiplier at `0x8006d694` still lacks whole-object ownership. Twenty-four direct control-flow and eight BSS relocation rows were checked against retail, leaving zero withheld rows in a focused carve. A persistent counter pointer and list-owned code pointer now explain the retail frame and referents; purchase control flow and register scheduling remain divergent. | **WIP, 80.8% focused listing** |
| `0x8001d3b4` | Second controller has a 3,720-byte frame: 120 glyph rows, 120 byte values, 120 word codes, and 120 indices after a 52-byte list. It calls the secondary translator, uses the word codes for experience arithmetic, and reads the shared `0x8006d694` multiplier as both a word and a byte. The multiplier's owning datum remains unresolved. | **WIP, unclaimed** |
| `0x8001d6a8` | Appended to the adjacent secondary-code unit. The 3,232-byte frame, 120 rows/byte values/indices, 14 direct calls, selected-item byte width, and the `game_counter_bytes` HI/LO referent are confirmed by retail. A source-only `u8` probe made its listing identical; strict comparison retained it. | **Exact, 100%** |
| `0x8001d8d0` | Related 1,328-byte controller calls the primary translator and same input/preview chain, adds a menu sequence transition, and indexes a candidate page interior at `0x80065b30`. That page and the `0x8006d694` multiplier still lack a complete owner. | **WIP, unclaimed** |
| `0x8001ddd0` | Another 1,328-byte item-list controller shares the primary translator and preview path, reading an interior of the candidate `0x80065aec` page and the shared multiplier. Neither is yet a supported source-owned table. | **WIP, unclaimed** |
| `0x8001e0a8` | 1,320-byte counterpart uses the secondary translator, item-model calls, menu input, and two-frame redraw. Its multiplier and complete list-storage contract are not source-owned. | **WIP, unclaimed** |
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
