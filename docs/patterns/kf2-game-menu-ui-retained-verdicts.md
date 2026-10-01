# GAME menu UI focused verdicts

This related batch follows the menu selection, glyph, input, status, and sprite
families through their shared calls and data. Each row was rebuilt with a
focused `kf try --unit ... --context 0 --no-flow` probe. `SAME` means an
identical focused listing; strict `kf match` closure was not run in this pass.
The source models and curated retail evidence, rather than a fuzzy score,
remain the criteria for retaining changes.

| GAME address | Function | Focused verdict |
| --- | --- | --- |
| `0x8001876c` | `func_8001876c` | DIFF 90.8%; result-stack and return-path instruction residue |
| `0x800189f0` | `func_800189f0` | SAME |
| `0x80018ac8` | `func_80018ac8` | SAME |
| `0x80018d08` | `func_80018d08` | SAME |
| `0x80018dec` | `func_80018dec` | SAME |
| `0x80018f8c` | `func_80018f8c` | SAME |
| `0x80019240` | `func_80019240` | SAME |
| `0x800192ac` | `func_800192ac` | SAME |
| `0x800192dc` | `func_800192dc` | SAME |
| `0x80019834` | `func_80019834` | SAME |
| `0x800199d0` | `func_800199d0` | SAME |
| `0x80019ac4` | `func_80019ac4` | SAME |
| `0x80019ce4` | `func_80019ce4` | SAME |
| `0x80019ed4` | `func_80019ed4` | SAME |
| `0x8001e378` | `func_8001e378` | SAME |
| `0x8001e484` | `func_8001e484` | SAME |
| `0x8001e94c` | `func_8001e94c` | SAME |
| `0x8001f008` | `func_8001f008` | SAME |
| `0x8001f798` | `func_8001f798` | SAME |
| `0x8001f8b8` | `func_8001f8b8` | DIFF 84.8%; loop exit and input-wait block placement |
| `0x8001fb8c` | `menu_draw_window` | DIFF 83.5%; target 48-byte versus probe 40-byte frame; other instructions align |
| `0x80020b50` | `menu_blit_sprite_translucent` | SAME |
| `0x80020d20` | `menu_blit_sprite` | SAME |
| `0x80020ef8` | `menu_blit_sprite_fixed_clut` | SAME |
| `0x800210ac` | `menu_draw_string` | DIFF 90.4%; target 56-byte versus probe 48-byte frame and glyph UV register choice |
| `0x80021510` | `menu_draw_number` | DIFF 93.1%; font width/height load order in two UV paths |

The separate archive-image preview at `0x8001930c` remains DIFF 80.2% in a
normal focused `kf try`. SDK `setXYWH` and `setUVWH` macros express the two
rectangles without changing the listing. Its calls, referents, 47 CFG blocks,
and 27 branches align; the remaining observable differences start with a
64-byte versus 56-byte frame and the archive-entry arithmetic schedule.
The arithmetic expression has been restored after a left-associative probe
failed to improve the instruction order. No compiler mechanism is inferred
from these residues.

The related fade transition at `0x800349bc` draws four `POLY_FT4` rectangles.
The pinned Psy-Q 3.0 `setXYWH` and `setUVWH` macros replace eight equivalent
field-write groups in its source. A normal focused probe retains DIFF 43.2%,
with 14/14 CFG blocks and 8/8 branches; adjacent `tim_upload_images`
(`0x8003494c`) and `func_80034e10` remain SAME. Retail spills the fade state
on a 72-byte frame, while the current 64-byte probe keeps it in a saved
register. The macro substitution changes source expression without claiming
an exact-match gain.
