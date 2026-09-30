# GAME menu list and numeric pages

This batch covers exactly ten previously non-exact GAME functions connected by
the item-list controllers, their two indexed code tables, and numeric menu
rendering. Each address received the matcher address, block disassembly/CFG,
incoming-xref, callee, string, and match-state queries. The retail evidence
supports the two table extents but does not establish one common source file
for the surrounding controllers.

| GAME address | Retail evidence | Final verdict |
| --- | --- | --- |
| `0x8001d340` | first indexed halfword translator, 240-byte page stride | **WIP, 93.10345% strict**; page-offset shift order differs, 1,440/1,440 table bytes exact |
| `0x8001d654` | second indexed halfword translator, same stride and unsigned-byte index | **WIP, 90.47619% strict**; page-offset shift order differs, 1,200/1,200 table bytes exact |
| `0x8001ddd0` | first item-list controller calls the primary translator, item preview, and shared list renderer | WIP, unclaimed; complete list-state record is unresolved |
| `0x8001e0a8` | paired item-list controller calls the secondary translator and the same preview/render helpers | WIP, unclaimed; list-state record is unresolved |
| `0x8001e484` | input/model controller with many incoming calls and preview rotation/translation state | WIP, unclaimed; model/input state is unresolved |
| `0x8001e94c` | numeric text renderer references ten 20-byte suffix rows at `0x80064a00` | WIP, unclaimed; renderer record and table's source-file owner are unresolved |
| `0x8001f008` | paired numeric text renderer references the remaining two suffix rows | WIP, unclaimed; renderer record and table's source-file owner are unresolved |
| `0x8001f8b8` | preview controller calls the menu renderer, two-option widget, and input helpers | WIP, unclaimed; preview/list record is unresolved |
| `0x8001fb8c` | window title and highlighted row renderer | **WIP, 99.78788% strict**; retail uses a 48-byte frame, current C emits 40 bytes |
| `0x8001fc94` | shared numeric/list renderer calls the exact primitive-buffer pair and menu draw helpers | WIP, unclaimed; large list/graphics record is unresolved |

`0x8001d654` now owns `menu_item_code_secondary[5][120]` at
`0x800661c0–0x80066670`. The first four pages contain numeric codes; the
fifth is mostly zero but has values at indices 99–109. The following byte
starts the separately owned card-file prefix. The former Ghidra fragments
inside this range included false string classifications. A complete retail
byte audit, the 240-byte addressing stride, and the boundary support the
single initialized table. The source loop has the retail guard, unsigned
byte index, halfword load, increment, and return shape. Its first instruction
order difference is the final page-offset shift emitted before the table
base load, while retail puts the shift after the base load. The natural
two-dimensional array expression is retained without codegen steering.

The 240 bytes at `0x80064a00–0x80064af0` decode as twelve consecutive
`KfMenuLabelSuffix` records. The renderer at `0x8001e94c` references rows
0, 1, and 4–11; `0x8001f008` references rows 2–3. The next address is the
existing exact 16-record `menu_label_suffixes` object. This proves the
record extent and explains the interleaved Ghidra data fragments, but does
not by itself prove that either renderer or the adjacent label builders
defined the 12-record object. Its source owner and structural identity are
left pending consumer reconstruction.

Focused comparison of `menu_draw_window` confirms matching control flow,
calls, and referents. Only the frame allocation and saved-register offsets
differ. No local with a supported lifetime explains the extra eight bytes,
so its source remains unchanged. None of the ten has a supported Psy-Q or
other vendored attribution.

The strict GAME report confirmed the two code percentages and both table
data sections at 100%, with 141/141 target units relinked. Retail census
validation and `git diff --check` passed. The global strict command still
stops at known-reference closure and unrelated TMD/map `.rodata` addends.
No repository tests, banking, or commit were performed.
