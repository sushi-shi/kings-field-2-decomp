# GAME menu renderer follow-up

This 28-function pass follows the item-list, card-label, window, sprite, and
display calls surrounding the exact menu-label units. Each address received
the matcher address, retail disassembly/CFG, incoming-xref, callee, string,
and match-state queries. Call edges and shared typed menu data define the
campaign; adjacency alone does not establish a translation-unit boundary.

| GAME address | Retail role or ownership issue | Verdict |
| --- | --- | --- |
| `0x8001ddd0` | first item-list controller, 14 direct calls | **exact, 728/728 code bytes** in later isolated objdiff; table TU owner remains open |
| `0x8001e0a8` | paired item-list controller, 14 direct calls | **exact, 720/720 code bytes** in later isolated objdiff; table TU owner remains open |
| `0x8001e378` | menu input poll | **exact**, existing source |
| `0x8001e484` | menu/model input controller | **exact, 1,224/1,224 code bytes** in later isolated objdiff |
| `0x8001e94c` | numeric text renderer | **exact, 1,724/1,724 code and 240/240 data bytes** in current direct objdiff |
| `0x8001f008` | second numeric text renderer | **exact, 1,936/1,936 code bytes** in current direct objdiff |
| `0x8001f798` | paired, six-row menu labels | **exact, 288/288 bytes**, newly resolved |
| `0x8001f8b8` | menu model/preview controller | WIP: 94.36464% direct objdiff, 84.8% focused; 42/42 blocks and 18/18 branches |
| `0x8001fb8c` | window title and highlighted rows | WIP: 99.78788% direct objdiff; 48-byte retail versus 40-byte source frame |
| `0x8001fc94` | numeric/list renderer | WIP: 98.581024% direct objdiff, 97.7% focused; 55/55 blocks and 33/33 branches |
| `0x80020748` | two-option widget | **exact**, existing source |
| `0x8002083c` | menu model preview | WIP: 99.65882% direct objdiff; 224-byte retail versus 160-byte source frame |
| `0x80020990` | numeric heading | **exact**, existing source |
| `0x80020b50` | translucent sprite packet | **exact**, existing source |
| `0x80020d20` | cursor sprite packet | **exact, 472/472 code bytes** in current direct objdiff |
| `0x80020ef8` | fixed-CLUT sprite packet | **exact, 436/436 code bytes** in current direct objdiff |
| `0x800210ac` | glyph-string packets | WIP: 99.66904% direct objdiff, 90.4% focused; frame/register choice |
| `0x80021510` | number-glyph packets | **exact, 736/736 code bytes** in later isolated objdiff |
| `0x800217f0` | nine-slice panel | **exact**, existing source |
| `0x80021a60` | eight-byte return stub | **exact**, source-owned with adjacent frame-begin function |
| `0x80021a68` | frame begin | **exact**, existing source |
| `0x80021be0` | frame present | **exact**, existing source |
| `0x80021c8c` | display state entry | WIP: 99.956985% direct objdiff; 32-byte retail versus 24-byte source frame |
| `0x80021e00` | display exit | **exact**, existing source |
| `0x80021f10` | start quad primitive | **exact**, existing source |
| `0x80021f60` | commit quad primitive | **exact**, existing source |
| `0x80021fb0` | initialize menu list | **exact**, existing source |
| `0x80022058` | decimal number formatter | WIP: 95.74% direct objdiff, 71.4% focused; retail eight-byte frame and loop scheduling |

The paired-row renderer uses two glyph strings and six selection bytes,
calling the fixed-CLUT, translucent, and string renderers on each row. Its
retail CFG, call order, and 26-pixel row advance were already represented in
`game.menu_pair_rows`. Advancing the selection pointer before incrementing the
independent row counter gives the retail instruction order without changing
the source behavior. Focused `kf try` reports SAME; strict objdiff reports
one exact function and 288/288 code bytes.

The existing window, preview, and display-state frame differences have no
proven source extent; no padding was added. The two cursor blitters retain the
shared signed point type used by the exact translucent sibling, and fresh
focused builds plus direct objdiff confirm both blitters exact. The eight-byte
`0x80021a60` stub and adjacent frame-begin body now form one source unit;
fresh focused comparison reports both listings identical. The larger
item-list and text renderers have separate later source claims and verdicts.

The strict run verified 152/152 GAME target relinks. It exited at incomplete
known-reference data ownership and two unrelated map-object `.rodata` addend
divergences; the `game.menu_pair_rows` objdiff result itself is 100%.

Fresh direct objdiff confirms both `menu_item_model` functions at 100%
(212/212 and 68/68 code bytes) and all three `menu_sound_cue` functions at
100% (148/148, 56/56, and 108/108 code bytes). The current focused builds
also preserve the `menu_two_option` exact sibling. The remaining
`menu_draw_window` listing differs only by its frame allocation and saved
register offsets. The `menu_two_option` preview body similarly differs only
by a 64-byte frame offset; `menu_display_state` differs only by an eight-byte
frame offset. A typed temporary for the numeric atlas's second
column worsened `menu_draw_number` from 93.1% to 86.0%; moving the list
renderer's card-column computation into its row loop reduced its focused
listing from 97.7% to 92.5% or 89.9%, depending on scope. All probes were
discarded. Neither a larger artificial stack object nor a volatile carrier
is supported by retail evidence.

The preview-choice controller's first listing divergence is saved-register
assignment for its list pointer, render mode, and item ID. Its label
initialization and return block are also scheduled in a different order even
though its 42-block/18-branch CFG matches. An explicit common-return probe
lowered the focused listing to 81.2% and was reverted; the remaining residue
does not justify changing its input, result, or label semantics.

The shared `func_8001fc94` declaration and function identity now take a
`const void *` payload: the 19 callers pass item, card, and magic menu records
with the same 36-byte list prefix but mode-dependent pointer slots afterward.
The renderer uses a checked `KfMenuRenderList` view internally. This keeps the
interface faithful to its callers without giving them incompatible struct
types; its focused 97.7% listing is unchanged.

A later follow-on closed both stock-list controllers by expressing their
one-use purchase cost directly in the funds condition. It also closed
`menu_draw_number`: a byte cast at the Psy-Q U-coordinate boundary restores
the retail addition order, and checking through the initialized code pointer
restores the first argument-save placement. Normal focused listings and
isolated native objdiff are exact for these three; their raw ordered
relocation listings match the carved retail objects. The earlier failed
typed-temporary probe above remains a historical source-shape control.
