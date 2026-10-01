# Current GAME menu visual controls

This is a current `SLPS-00069` GAME.EXE comparison of thirteen focused
source units and fourteen functions. Each base object was compiled with its
declared unit profile, then compared with only its corresponding delinked
retail object. The six sprite/window calls, glyph and number helpers, frame
setup, status display, and model display form one visual call family. Earlier
inherited menu notes use a different address map; these verdicts use the
current `ADDRESS` claims and `kf sema --image game` identities.

| GAME VA | Function | Direct strict verdict |
| --- | --- | --- |
| `0x8001e94c` | `func_8001e94c` status renderer | 100% |
| `0x8001f008` | `func_8001f008` attribute renderer | 100% |
| `0x8001fb8c` | `menu_draw_window` | 99.78788% WIP |
| `0x80020b50` | `menu_blit_sprite_translucent` | 100% |
| `0x80020d20` | `menu_blit_sprite` | 100% |
| `0x80020ef8` | `menu_blit_sprite_fixed_clut` | 100% |
| `0x800210ac` | `menu_draw_string` | 99.66904% WIP |
| `0x80021510` | `menu_draw_number` | 100% |
| `0x800217f0` | `func_800217f0` panel compositor | 100% |
| `0x80021a60` | `func_80021a60` frame helper | 100% |
| `0x80021a68` | `menu_frame_begin` | 100% |
| `0x80021be0` | `menu_present_frame` | 100% |
| `0x80022058` | `menu_format_number` | 97.39000% WIP |
| `0x80033994` | `func_80033994` model renderer | 100% |

The retained `menu_draw_window` source and retail have identical non-frame
instructions. Retail allocates 48 stack bytes; the probe allocates 40, moving
the same six saved registers and `$ra` by eight bytes. Its initialized
`menu_sprite_defs` and `menu_window_layouts` compile to **2,704/2,704**
matching bytes. `menu_draw_string` likewise differs in a 56-byte retail
versus 48-byte probe frame plus equivalent temporary-register choices in the
glyph UV calculation. `menu_format_number` is a leaf with the same number
formatting branches and stores, but retail reserves an eight-byte frame while
the probe uses none; the terminal negative-index branch also places `li a2,10`
in a different delay slot. No source fact supports a dummy local, forced
padding, or another codegen-only change, so all three remain WIP.

The exact model renderer now uses the shared `tmd_packets.h` declaration of
`func_8002e4dc(u16 object_index, s32 depth_bias)` instead of a private
`s32` object-index prototype. Its call passes the literal zero, and a focused
rebuild plus direct one-function objdiff preserve all 104/104 exact code
bytes. The duplicate local declaration of `func_8002d918` was also removed;
`tmd.h` already declares that function with the same signature.

The status renderer's 240-byte `.data` section and the frame-begin unit's
8-byte `.data` section are exact. The other eleven functions are strict
100%, not merely focused-listing `SAME`. Only targeted per-unit compilation
and direct objdiff were used; no repository tests, lint, full linked build,
broad match, or banking was run.
