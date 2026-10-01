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
| `0x8001d030` | A C claim reconstructs its 40 glyph rows, 40 byte values, 40 word codes, 40 indices, primary translator, preview/input calls, and item purchase path. The indexed page and byte at `0x80065aeb` belong to one initialized six-page, 120-byte-per-page table; the mutable multiplier at `0x8006d694` still lacks whole-object ownership. Twenty-four direct control-flow and eight BSS relocation rows were checked against retail, leaving zero withheld rows in a focused carve. Counter and code pointers, selected-item lifetime, word-width quantity arithmetic, and the combined purchase rejection branch now explain the frame, referents, and CFG. The purchase expression now matches retail after inlining the one-use cost and spelling price first in the multiplication. Isolated direct objdiff confirms 784/784 text bytes and ordered relocations. | **Exact, 100% direct objdiff** |
| `0x8001d3b4` | Source claims the 3,720-byte frame: 120 glyph rows, 120 byte values, 120 word codes, and 120 indices after a 52-byte list. It calls the secondary translator, checks stock against the shared `0x8006d694` quantity, and credits player gold on sale. Retail reloads the list's stored entry count before preview setup; expressing that field access yielded an identical focused 0x2a0-byte listing. Direct objdiff now confirms all 672 code bytes. Twenty direct rows and three signed-low BSS pairs were decoded against raw retail; the one-VA safe carve withheld none. The multiplier's owning datum remains unresolved. | **Exact, 100% direct objdiff** |
| `0x8001d6a8` | Appended to the adjacent secondary-code unit. The 3,232-byte frame, 120 rows/byte values/indices, 14 direct calls, selected-item byte width, and the `game_counter_bytes` HI/LO referent are confirmed by retail. A source-only `u8` probe made its listing identical; strict comparison retained it. | **Exact, 100%** |
| `0x8001d8d0` | Source claims its 1,328-byte frame, 40-row list, two-frame opening, item trade checks, special item `0x75` tenfold quantity, and menu sequence transition. Its indexed page at `0x80065b30` belongs to `menu_item_mask_pages[4]`; the mutable `0x8006d694` multiplier still lacks a complete owner. Thirty-two direct rows plus the signed-low counter pair were decoded against retail, and its one-VA safe carve withheld none. Focused compilation and direct objdiff confirm all 916 code bytes. | **Exact, 100% direct objdiff** |
| `0x8001ddd0` | Source claims the 1,328-byte item-list controller with the primary translator, preview/input path, purchase checks, and page `menu_item_mask_pages[5]` at `0x80065ba8`. Twenty-one direct rows and three signed-low BSS pairs were checked against raw retail; its one-VA safe carve withheld none. The 0x2d8-byte focused listing has 22/22 CFG blocks and 12/12 branches. The same purchase-expression refinement closes its 728/728 text bytes with matching ordered relocations; the mutable multiplier still lacks a complete owner. | **Exact, 100% direct objdiff** |
| `0x8001e0a8` | Source claims its 1,320-byte frame, 40-row list, secondary code translator, item-model/input calls, and two-frame redraw. Twenty direct rows and three signed-low BSS pairs were checked against raw retail; its one-VA safe carve withheld none. The focused listing has 22/22 CFG blocks and 12/12 branches. The same purchase-expression refinement closes its 720/720 text bytes with matching ordered relocations; the mutable multiplier still lacks a complete owner. | **Exact, 100% direct objdiff** |
| `0x8001e484` | Common 48-byte-frame input service has at least 16 direct callers. It receives a list, a separate byte-index array, and two word outputs; it updates cursor fields `+0x1e..+0x22`, model preview state, and sound cues. A focused build and direct objdiff agree on all 1,224 code bytes; a one-VA safe delink withheld zero relocations. The shared multiplier at `0x8006d694` still lacks complete data ownership. | **Exact, 100% direct objdiff** |
| `0x8001e94c` | Repeated number/glyph renderer uses `menu_sprite_defs`, 20-byte label suffixes at `0x80064a00..0x80064adc`, player values, and `menu_format_number`. Its source now emits an identical focused listing; label ownership remains a separate data-model question. | **Exact, 100% focused** |
| `0x8001fc94` | The C claim covers the 55-block list renderer, its row/value/code pointers, sprite descriptors, primitive-buffer calls, and mode-specific number and glyph paths. Removing a redundant outer positive-row guard leaves the loop's own bound check to handle zero rows, as retail does. Retail advances the detail glyph pointer after each halfword and the number pointer after drawing. The 128-byte frame, 33/33 branches, 55/55 blocks, and ordered referents agree. Retail computes the card-column mode check inside the row path at `0x8001fd5c..0x8001fd68` and again after the loop at `0x80020700..0x80020704`; spelling the condition at its C use sites removes an early cached calculation. Only a lower-panel constant/index instruction order remains different. The KF1 renderer confirms only a related loop shape. | **WIP, 99.70803% direct strict objdiff** |

`KfItemMenuList` is a distinct 52-byte list view: the common `KfMenuList`
prefix, a glyph-row pointer at `+0x24`, a byte-value pointer at `+0x2c`, and
a word-code pointer at `+0x30`. This is supported by direct stack stores in
`0x8001d030` and `0x8001d3b4`; `0x8001d6a8` uses only the first two extra
pointers. The unrelated `KfMagicMenuList` retains its signed-word values at
`+0x30`. `0x8001e484` receives the index array in `a1`, distinct from the
byte-value pointer stored in the list at `+0x2c`.
The menu services now have one declaration in `menu.h`: the input service
receives the embedded `KfMenuList` prefix, and the renderer receives the
complete mode-dependent 52-byte record through a generic pointer before
applying its typed render view. All 16 source calls to the input service pass
`&menu.list`; focused builds preserve the exact menu controls. This removes
incompatible per-file declarations without claiming that the differently
named payload fields were one original C struct.

A follow-up on the shared choice/input helper `0x8001f8b8` found a focused
84.8% WIP listing with 42/42 CFG blocks and 18/18 branches. Its 15 proven
callers and outgoing call/data references agree with source. The first control
difference is the loop exit and `input_wait_release` block placement; two
source-equivalent exit rewrites lowered the focused score and changed the CFG,
and an explicit top-of-loop `break` followed by a common release/return
similarly lowered it to 81.2% with 40/42 blocks and 19/18 branches. None was
retained. Register allocation and local initialization order
remain unattributed residues. Its four pad-bit checks now spell the pinned
`PADLup`, `PADLdown`, `PADRright`, and `PADRdown` macros; the focused listing
and CFG are unchanged at 84.8%.

The focused 0x8001d6a8 listing and subsequent strict GAME report are exact.
A fresh focused comparison also emits identical listings for the adjacent
0x8001d654 translator and 0x8001d6a8 controller (2/2); the older 95.7%
translator result was stale. Strict status for that translator has not been
rechecked.
In the three purchase controllers, reversing the commutative price/multiplier
expression changed the emitted `mult` operand order but moved the earlier
loads farther from retail, lowering focused similarity from 96.6% to 95.7%
at `0x8001d030` and from 96.2% to 95.3%/95.2% at `0x8001ddd0`/`0x8001e0a8`.
No source or referent evidence distinguishes those two C spellings, so that
probe was discarded; the shared residue remains unattributed.

The shared shop quantity at `0x8006d694` is an initialized load-image word
with retail value `1`. Its 24 validated address references are confined to
the item purchase/sale controllers, menu input service, item-model loader, and
amount renderer. Adjacent words are independently identified as item-model
allocation pending (`0x8006d68c`), input idle counter (`0x8006d690`), and
cursor animation frame/direction (`0x8006d698..0x8006d69c`). This supports
the quantity's menu role and a nearby menu-state cluster, but does not prove
which original translation unit defined the word; no DATA claim was added.
The earlier campaign report relinked 149/149 units and had 391 exact of 461
scored. Global edge closure stopped at three unrelated `.rodata` addends in
TMD/map-object units. An earlier full `kf build` built PSX; GAME, OPEN, and END
retained their first unresolved link symbols `InitCARD`, `malloc`, and
`display_buffers`. The current continuation used focused builds and one-VA
safe delinks only; no repository tests or banking were performed.

The later source-only inline-cost probe removed the shared price/quantity
load-order residue without changing the purchase rule: the funds condition
computes `(s32)menu.codes[selected] * DAT_8006d694` directly, while the
completed-purchase update still assigns `cost` separately. Reversing the
commutative multiplication operands in that inline form removed the final
single `mult` difference. Normal focused quick builds return SAME for all
three controllers. Isolated native objdiff reports 100.0% for their whole
text sections (784, 728, and 720 bytes), and raw ordered relocation
comparisons are identical. No repository tests or full build were run.

A later lower-panel loop correction expresses the GAME retail row increment
before `primitive_buffer_begin_poly_ft4`, matching its call delay slot and
the analogous KF1 sibling source shape. The normal focused listing improves
from 97.7% to 98.6% with its 128-byte frame intact. Isolated objdiff now
reports 98.44964% fuzzy similarity, slightly below its earlier 98.581024%
metric, while all 201 raw ordered relocation sites match the retail target.
The loop is retained for its direct instruction and referent evidence; the
early scroll/card-mode register and calculation placement remained WIP at that
checkpoint.

A later direct retail recheck showed the card-column predicate is computed
inside the row path and independently after the loop. The C source had cached
that predicate before entering the loop. Removing the cache preserved the
condition and all calls/referents while moving its calculation to the retail
position; a focused rebuild and direct strict objdiff improved `0x8001fc94`
from 98.44964% to 99.70803%. The only remaining difference is the order of
`s6 = 0xff` and `s3 = 0` before the lower-panel row loop. No source-backed
reason for that order has been established, so the function remains WIP.
