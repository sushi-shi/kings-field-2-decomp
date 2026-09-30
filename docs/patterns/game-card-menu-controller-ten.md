# GAME card dialog and item-list controller ten

These ten previously non-exact GAME functions follow card-format and menu-list
calls. Each was checked against retail address/extent, block disassembly,
incoming references, callees, strings, and current match state. None has a
supported vendored attribution. Adjacency identifies a possible source run,
not a proven original translation unit.

| GAME address | Decisive retail evidence | Final verdict |
| --- | --- | --- |
| `0x8001bf68` | Temporary card-file probe chooses the format or error dialog, then writes the selected slot. | **WIP, 97.123890% strict**; the compiled probe status occupies `s0` where retail uses `v1`, with a later write-path schedule difference. |
| `0x8001c12c` | Format confirmation builds seven 28-byte glyph rows, uses suffixes 5/6/7/6/8, polls the two-option selector, and returns the result. | **Exact, 100% strict**, 1,060/1,060 bytes. |
| `0x8001d030` | Indexed 120-byte menu table, primary item-code translation, list setup, preview model, and input loop. | **WIP, unclaimed**; table extent and complete list workspace are unresolved. |
| `0x8001d3b4` | Paired list controller uses the secondary translator and a 3,720-byte stack frame. | **WIP, unclaimed**; list record and scratch-array contract are unresolved. |
| `0x8001d6a8` | List/model renderer filters entries, loads a preview model, draws frames, and reads selection input. | **WIP, unclaimed**; its 3,232-byte workspace and list/model state need a complete type. |
| `0x8001d8d0` | Related renderer uses the primary translator and a 1,328-byte stack frame. | **WIP, unclaimed**; indexed source table and record boundaries remain unresolved. |
| `0x8001ddd0` | Item-list controller calls the primary translator, preview loader, input handler, and frame renderer. | **WIP, unclaimed**; complete list-state record remains unresolved. |
| `0x8001e0a8` | Paired item-list controller calls the secondary translator and the same preview/input family. | **WIP, unclaimed**; complete list-state record remains unresolved. |
| `0x8001e484` | Shared model/input controller is called by the list handlers and updates selection and preview state. | **WIP, unclaimed**; its 66-block state flow and model/input record need reconstruction. |
| `0x80021c8c` | Menu display entry snapshots primitive buffers and music state before pausing a sequence. | **WIP, 99.956985% strict**; retail allocates a 32-byte frame, current C 24 bytes, with no evidenced source local to explain the difference. |

`0x8001c12c` is now contiguous with the existing `0x8001bf68` card-format
flow source. Its 264-byte frame contains seven typed `KfMenuGlyphString` rows
and three stack-held selector outputs. The retail order of initialization,
two-option switch, calls, branch targets, and delay slots is reproduced by
ordinary C. All 34 decoded call, internal-jump, and data-pair relocations in
its function range were reviewed; the glyph suffix, window layout, and sprite
referents use their existing owners. The adjacent `0x8001bf68` residue was
preserved. A source-equivalent switch probe changed block placement away from
retail, so it was discarded; no artificial register carrier was retained.

Focused comparison found the new listing identical. The strict GAME report
confirmed 100% for `0x8001c12c`, preserved `0x8001bf68` at 97.123890%, and
relinked 142/142 target units. Retail census validation passed. Global
edge-check still stops at the three pre-existing unrelated TMD/map-object
`.rodata` addends. No repository tests, banking, or commit were performed.
