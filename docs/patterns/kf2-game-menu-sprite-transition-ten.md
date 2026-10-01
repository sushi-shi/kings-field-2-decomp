# GAME menu controller and display transitions

This batch covers exactly ten previously non-exact or unclaimed GAME functions
connected by the root menu, its sprite and number renderers, and the display
transition. Each received matcher address, block disassembly/CFG, incoming
xref, callee, string, and match-state queries. None has supported vendored
attribution. Current percentages below are from direct focused objdiff.

| GAME address | Retail evidence and final verdict |
| --- | --- |
| `0x8001876c` | Root controller dispatches seven menu choices through a pointer table and returns a result to `0x80029014`. **WIP, 96.36646%**; 34/34 known CFG blocks and 14/14 branches, but saved-register choice and result reload schedule differ. Its 28/28-byte switch table matches exactly. |
| `0x80020d20` | Menu sprite quad, paired with `0x80020ef8`. **Strict exact, 472/472 bytes**. |
| `0x80020ef8` | Sprite quad with fixed CLUT. **Strict exact, 436/436 bytes**. |
| `0x800210ac` | Draws a positioned glyph string. **WIP, 99.66904%**; 8/8 CFG blocks, but the retail frame is 56 bytes versus 48 in C, with a glyph UV register difference. |
| `0x80021510` | Draws fixed-width numeric glyphs from two atlas columns. **Strict exact, 736/736 bytes**. |
| `0x80021c8c` | Enters the menu display mode. **WIP, 99.956985%**; retail allocates 32 stack bytes and C allocates 24. The adjacent display-exit function remains exact. |
| `0x80022058` | Formats decimal glyphs and blank-fill output. **WIP, 97.39% strict; 78.5% focused listing**. The remaining eight-byte frame and return delay-slot difference have no proved source owner. |
| `0x80022300` | Menu sound-cue dispatch. **Strict exact, 148/148 bytes**; cues 13 and 16–18 follow the retail branch and call paths. |
| `0x800349bc` | Draws four textured fade quads, reporting pad input or fade completion. **WIP, 92.34296% strict**; the boundary branch now has retail's successor map, while state placement and quad setup remain different. |
| `0x80034e10` | Archive TIM/VRAM transition. **Strict exact, 384/384 bytes**. |

The new `0x8001876c` source is contiguous with exact `0x800189f0` in
`game.menu_location_number`. The seven linked case targets at
`0x80011098–0x800110b4` form one source-owned `RODATA` claim. Each table
word, the table-address HI16/LO16 pair, and the controller's direct `j`/`jal`
sites now have reviewed relocation evidence. An explicit result join for
cases 0, 1, and 5 reproduces retail's branch with the result store in its
delay slot. Separate display-mode calls for result `-3` and other results
reproduce the retail call set. The indirect `jr` is supported by the table,
although the CFG tool still marks indirect reachability incomplete.

The later focused `0x80022058` probe compared two equivalent fill-index
placements. Moving its initialization after the style-dependent count updates
raised listing similarity from 71.4% to 78.5% and restored the retail zeroing
instructions on the alternate style paths. The original C leaf still lacks
retail's eight-byte frame, which has no supported source object; this remains
WIP, not a strict match claim.

The card-row caller at `0x8001af30` was also corrected to pass full 32-bit
outputs to `0x800228c8`. Retail writes words for experience, level, and
one-based save-slot ID; the original byte-sized level scratch understated
the callee's write. The exact card-row listing remained 100% after the type
and name correction. Its contiguous card-format neighbor has been handed
to the card-directory owner for a shared literal/source merge.

Focused comparison and the strict report preserve exact `0x800189f0`,
`0x8001af30`, and the 28-byte switch table. Retail census validation passed.
The report's remaining menu functions are WIP; no repository tests, banking,
or commit were performed in this batch.

The adjacent two-option preview controller `0x8001f8b8` was also rechecked
after the later GAME target refresh. Its focused listing remains **84.8% WIP**.
The first differences assign the four incoming values to different saved
registers and place the `-99` result test and return block differently; the
retail and compiled call sets still agree. A temporary source-only `while`
form preserved behavior but lowered listing similarity to 81.2%, so the
tracked C remains unchanged. The decimal formatter `0x80022058` remains
**78.5% focused WIP** with the same unexplained eight-byte frame and return
delay-slot difference; neither residue supports a fake local or padding.
KF1 uses a plain `for` fill loop for its four-argument formatter. A temporary
KF2 spelling with that loop fell to 62.3% focused because it lost retail's
zero-index instructions on the style branches and chose a different fill
cursor. The retained KF2 `do` loop remains the better-evidenced source.
A controlled GCC 2.6.0 compile of the retained C also omitted the eight-byte
frame, so that compiler switch alone does not explain the remaining residue.

Fresh focused rebuilds and direct per-unit objdiff comparisons establish
**5/10 strict exact** in this ten-function set. The five WIPs are `0x1876c`
(96.36646%), `0x210ac` (99.66904%), `0x21c8c` (99.956985%), `0x22058`
(97.39%), and `0x349bc` (92.34296%). The numeric glyph drawer, both sprite
quad helpers, the sound-cue dispatcher, and the TIM/VRAM transition are exact.
Adjacent `0x189f0`, `0x21e00`, and `tim_upload_images` remain strict exact
controls. The earlier percentage and CFG statements for these exact functions
are historical; no tests, linked build, or banking were run in this recheck.
