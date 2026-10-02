# GAME menu sprite and number pointer: 27 direct pairs

The shared `POLY_FT4 *current_poly_ft4` at GAME BSS `0x8006d9e0`
is defined by the current frame-writer source and read by two separate
packet renderers. Its original defining translation unit and BSS allocation
mechanism remain candidate. This pass reviews only direct source-site
relocations; it changes neither source nor ownership.

| Enclosing GAME function | Raw `lui` site addresses | Verdict |
| --- | --- | --- |
| `menu_blit_sprite_translucent` `0x80020b50` | `0x80020b6c`, `0x80020b7c`, `0x80020b8c`, `0x80020b9c`, `0x80020c58`, `0x80020c74`, `0x80020c84`, `0x80020c98`, `0x80020cb4`, `0x80020ccc`, `0x80020ce4`, `0x80020cf4`. | 12 reviewed pairs; strict text remains 100%. |
| `menu_draw_number` `0x80021510` | `0x8002154c`, `0x80021618`, `0x80021634`, `0x80021648`, `0x8002165c`, `0x80021684`, `0x8002169c`, `0x800216c0`, `0x800216e8`, `0x80021708`, `0x80021720`, `0x8002173c`, `0x80021764`, `0x8002177c`, `0x800217a8`. | 15 reviewed pairs; strict text remains 100%. |

All 27 retail sites decode as `lui 0x8007` immediately followed by `lw`
using the same register as the base and signed low immediate `0xd9e0`.
The address is therefore exactly `0x8006d9e0`; both current C functions
explicitly dereference the typed pointer while constructing textured
primitives. The rows were promoted from candidate/`paired-reachable` to
reviewed/`paired-reviewed` without changing their image, site, partner,
target, opcode, register, or addend. Two safe one-VA carves, one per
function, withheld no function or relocation and admitted all 12 and 15
pairs against `current_poly_ft4` with zero addend. Isolated strict objdiff
kept both texts at 100%. No candidate outside these two bounded functions
was promoted, and no repository tests, lint, broad match, full build, or
full-image link were run.
