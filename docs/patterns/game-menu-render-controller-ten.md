# GAME menu rendering and preview-controller ten

These ten previously non-exact GAME functions share menu glyph, number,
sprite, and preview calls. For each, retail address and extent, disassembly,
incoming references, outgoing references, strings, and current match state were
inspected. None has a supported vendored attribution. The three large
unclaimed renderers still need a complete source and data-owner model.

| GAME address | Retail evidence | Final verdict |
| --- | --- | --- |
| `0x8001e94c` | Repeated calls to `menu_draw_string`, `menu_format_number`, and `menu_draw_number` use 20-byte initialized suffix rows at `0x80064a00` onward. | **WIP, unclaimed**; the suffix-table owner and full numeric-rendering state are unresolved. |
| `0x8001f008` | The paired numeric renderer repeats the same call family and suffix rows through `0x80064adc`. | **WIP, unclaimed**; its full source/data contract is unresolved. |
| `0x8001f8b8` | Two 28-byte glyph rows, eleven label-kind cases, input polling, two frame draws per iteration, and a confirmed choice return. | **WIP, 94.36464% strict**; 42/42 CFG blocks and 18/18 branches match, but block ordering and saved-register allocation differ. |
| `0x8001fb8c` | Menu window renderer with ten blocks and shared sprite state. | **WIP, 99.78788% strict**; retail has a 48-byte frame, current C a 40-byte frame, with no evidenced extra local. |
| `0x8001fc94` | Large preview renderer calls glyph/number helpers and draws several `POLY_FT4` primitives with the shared sprite definitions. | **WIP, unclaimed**; its complete record and drawing-state contract are unresolved. |
| `0x80020d20` | Sprite blit reads sprite width and subtracts an eight-pixel margin from X. | **WIP, 99.44068% strict**; equivalent X arithmetic produces a different load/order schedule. |
| `0x80020ef8` | Paired fixed-CLUT sprite blit uses a seven-pixel X margin. | **WIP, 99.39449% strict**; the same arithmetic-order residue remains. |
| `0x800210ac` | String drawing loops over glyphs with eight matching CFG blocks. | **WIP, 99.66904% strict**; retail uses a 56-byte frame versus 48 bytes and differs in one glyph-UV register choice. |
| `0x80021510` | Number drawing has seven matching CFG blocks and uses the shared glyph atlas. | **WIP, 98.61957% strict**; entry delay-slot and repeated atlas-U load order differ. |
| `0x80022058` | Number formatter selects style/padding and writes digit glyphs. | **WIP, 95.74% strict**; 36/37 CFG blocks match, but retail has an eight-byte frame while current C compiles as a leaf. |

`0x8001f8b8` now has a distinct source claim in `game.menu_preview_choice`.
Its 31 call, data-pair, and internal-control-flow relocation rows were checked
against retail. A retail-supported split of the two input bits improved its
focused CFG to 42/42 blocks. A source-equivalent alternate loop exit made the
block layout worse and was discarded. The function's forwarded list/render
pointers remain opaque until `0x8001fc94` establishes their record type.

The strict GAME pass relinked 143/143 target units, preserved the six existing
non-exact sources at the scores above, and reported `0x8001f8b8` at 94.36464%.
Retail census validation passed. Global edge-check still stops at three
pre-existing unrelated TMD/map-object `.rodata` addends. No repository tests,
banking, or commit were performed.
