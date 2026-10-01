# GAME menu list and numeric pages

This batch covers exactly ten previously non-exact GAME functions connected by
the item-list controllers, their two indexed code tables, and numeric menu
rendering. Each address received the matcher address, block disassembly/CFG,
incoming-xref, callee, string, and match-state queries. The retail evidence
supports the two table extents but does not establish one common source file
for the surrounding controllers.

| GAME address | Retail evidence | Final verdict |
| --- | --- | --- |
| `0x8001d340` | first indexed halfword translator, 240-byte page stride | **exact: 116/116 code and 1,440/1,440 data bytes** in current direct objdiff |
| `0x8001d654` | second indexed halfword translator, same stride and unsigned-byte index | **exact: 636/636 unit code and 1,200/1,200 data bytes** in current direct objdiff; the unit also owns `0x8001d6a8` |
| `0x8001ddd0` | first item-list controller calls the primary translator, item preview, and shared list renderer | **exact, 728/728 code bytes** in current direct objdiff |
| `0x8001e0a8` | paired item-list controller calls the secondary translator and the same preview/render helpers | **exact, 720/720 code bytes** in current direct objdiff |
| `0x8001e484` | input/model controller with many incoming calls and preview rotation/translation state | **exact, 1,224/1,224 code bytes** in current direct objdiff |
| `0x8001e94c` | numeric text renderer references ten 20-byte suffix rows at `0x80064a00` | **exact, 1,724/1,724 code and 240/240 owned data bytes** in current direct objdiff |
| `0x8001f008` | paired numeric text renderer references the remaining two suffix rows | **exact, 1,936/1,936 code bytes** in current direct objdiff |
| `0x8001f8b8` | preview controller calls the menu renderer, two-option widget, and input helpers | **WIP, 94.36464% strict**; call/referent set agrees, while saved-register and block order differ |
| `0x8001fb8c` | window title and highlighted row renderer | **WIP, 99.78788% strict**; retail uses a 48-byte frame, current C emits 40 bytes |
| `0x8001fc94` | shared numeric/list renderer calls the exact primitive-buffer pair and menu draw helpers | **WIP, 98.44964% strict**; row-count and card-column scheduling differ |

`0x8001d654` now owns `menu_item_code_secondary[5][120]` at
`0x800661c0–0x80066670`. The first four pages contain numeric codes; the
fifth is mostly zero but has values at indices 99–109. The following byte
starts the separately owned card-file prefix. The former Ghidra fragments
inside this range included false string classifications. A complete retail
byte audit, the 240-byte addressing stride, and the boundary support the
single initialized table. The source loop has the retail guard, unsigned
byte index, halfword load, increment, and return shape. A previous source
revision differed in page-offset shift order. The current natural
two-dimensional array expression emits the complete retail listing in a
focused build, and direct objdiff confirms the code and data sections exact.

The 240 bytes at `0x80064a00–0x80064af0` decode as twelve consecutive
`KfMenuLabelSuffix` records. The renderer at `0x8001e94c` references rows
0, 1, and 4–11; `0x8001f008` references rows 2–3. The next address is the
existing exact 16-record `menu_label_suffixes` object. This proves the
record extent and explains the interleaved Ghidra data fragments. The current
`game.menu_status_render` C unit claims it as `menu_header_labels[12]`, and
direct objdiff confirms all 240 bytes; the original TU boundary remains
uncertain.

Focused comparison of `menu_draw_window` confirms matching control flow,
calls, and referents. Only the frame allocation and saved-register offsets
differ. No local with a supported lifetime explains the extra eight bytes,
so its source remains unchanged. None of the ten has a supported Psy-Q or
other vendored attribution.

The initial strict GAME report found the two earlier code differences, both
table data sections at 100%, and 141/141 target units relinked. Its global
command stopped at known-reference closure and unrelated TMD/map `.rodata`
addends. The later translator checks above used focused builds and direct
objdiff only; no repository tests or banking were performed.

Fresh focused builds and direct per-unit objdiff establish **7/10 strict
exact** in this survey. The current WIPs are `0x8001f8b8` (94.36464%),
`0x8001fb8c` (99.78788%), and `0x8001fc94` (98.44964%); all three retain
their correct call/referent sets and have no proved source correction for
their register, block-order, or frame residues. The seven exact function
verdicts and both initialized table claims above reflect current objects;
no source edit or linked build was done in this recheck.
