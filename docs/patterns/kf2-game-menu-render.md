# GAME menu renderer campaign

The GAME menu band at `0x8001f8b8..0x800221e8` contains 21 functions. This
pilot reconstructs ten of them alongside the three exact functions recorded
in [the primitive/menu-list note](kf2-game-primitive-menu-list.md). The
`probe-gcc257-o2-g0` output is a comparison tool, not evidence of the original
compiler version.

| GAME address | Function | Verdict |
| --- | --- | --- |
| `0x8001fb8c` | `menu_draw_window` | WIP, 99.78788% fuzzy |
| `0x80020748` | `menu_draw_two_option` | exact, 244/244 bytes |
| `0x80020b50` | `menu_blit_sprite_translucent` | exact, 464/464 bytes |
| `0x80020d20` | `menu_blit_sprite` | WIP, 99.44068% fuzzy |
| `0x80020ef8` | `menu_blit_sprite_fixed_clut` | WIP, 99.39449% fuzzy |
| `0x800210ac` | `menu_draw_string` | WIP, 99.66904% fuzzy |
| `0x80021510` | `menu_draw_number` | WIP, 98.61957% fuzzy |
| `0x80021a68` | `menu_frame_begin` | exact, 376/376 bytes |
| `0x80021be0` | `menu_present_frame` | exact, 172/172 bytes |
| `0x80022058` | `menu_format_number` | WIP, 90.94% fuzzy |

The two-option widget follows the KF1 control flow except that KF2 omits the
initial primitive-cursor load. It uses entries 1 through 4 of the 20-entry
`KfMenuSpriteDef` table at `0x80063e80`; each descriptor is 12 bytes. The
translucent blitter is a corresponding game wrapper around SDK quad operations,
with RGB `0xff`, offsets of six pixels, and ordering-table depth 20. It is not
an SDK body. `menu_frame_begin` flips the active display buffer, resets the
ordering table, advances the cursor animation frame, and updates the image
upload rectangle. `menu_present_frame` uploads that image between installing
the display environments and submitting the ordering table. The pointer at
`0x8006d9f0` and `RECT` at `0x8006d9f8` are beyond the CPE initialized end;
their source TU remains unknown, so both are declared externally.

The opaque blitters agree on calls, data referents, and straight-line control
flow. Their repeated X-coordinate computation remains differently scheduled:
retail subtracts the margin from the point first and then subtracts width;
the probe folds width and margin before subtraction. A signed-to-unsigned
`KfMenuPoint` trial improved those two listings but regressed the exact
translucent blitter, so the shared signed type is retained. The string renderer
has the retail eight-block CFG, both kana-mark paths, and 14-pixel advance;
the residual differences are a `v0`/`v1` choice around glyph UV calculation
and eight bytes of stack frame. The number renderer has the seven-block retail
CFG, with two repeated atlas-U load/order differences and entry scheduling.
`menu_format_number` implements the five-argument decimal and optional-glyph
behavior, but its padding loop and frame differ. These observations do not
identify a compiler mechanism.

The window renderer has the same ten-block CFG, six branches, direct calls,
ordered sprite referents, and text-row loop as retail. Its sole listing
difference is a 40-byte probe frame versus the retail 48-byte frame; this
is an unattributed residue, so the natural source is retained. The other
functions in the pilot band remain unclaimed: `0x8001f8b8`, `0x8001fc94`,
`0x8002083c`, `0x80020990`, `0x800217f0`,
`0x80021a60`, `0x80021c8c`, and `0x80021e00`. The eight-byte
`0x80021a60` stub is a `jr ra`/`nop` pair called once; its game versus library
provenance and semantic name are unresolved. No function in this campaign was
classified as vendored. Strict `kf match` generated the exact verdicts above;
the command still exits nonzero for repository-wide known-reference data
ownership, while the target relink completes.
