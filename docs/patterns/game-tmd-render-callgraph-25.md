# GAME TMD and render call graph: 25-function pass

This pass follows the prepared TMD index converter through packet enqueue,
map-cell rendering, and the frame's model dispatcher. Each address below is
in GAME.EXE. Retail disassembly/CFG, xrefs, strings, current source and match
state, adjacent claims, vendored attribution, and the KF1 renderer where
applicable were reviewed before source probes. Percentages are the last strict
objdiff report, not a new broad match. Focused `kf try` confirmed the stated
listing residues and exact controls.

| VA | Function or role | Verdict and decisive evidence |
| --- | --- | --- |
| `0x8002d5dc` | `tmd_prepare_primitive_indices` | **WIP, 96.132600%**. The eight typed packet-mode cases match the retail field offsets. The probe retains the input asset in `t2` and adds one prologue instruction; retail keeps it in `a0`, shifting the switch table and branches by four bytes. |
| `0x8002d8b0` | `tmd_register` | **Exact, 100%**; regression control in the contiguous TMD unit. |
| `0x8002d8f0` | `tmd_set_slot` | **Exact, 100%**; regression control. |
| `0x8002d910` | `tmd_release_slot` | **Exact, 100%**; regression control. |
| `0x8002d918` | TMD projection with fog depth | **Exact, 100%**; regression control. |
| `0x8002da94` | TMD projection and invalid-vertex marking | **Exact, 100%**; regression control and callee of `0x8002f808`. |
| `0x8002dbd8` | `tmd_transform_vertices` | **Exact, 100%**; regression control. |
| `0x8002dc80` | `tmd_transform_vertices_depth` | **Exact, 100%**; regression control. |
| `0x8002dd28` | `tmd_project_vertices` | **Exact, 100%**; regression control. |
| `0x8002ddb4` | Textured TMD packet walker | **WIP, 97.347160%**. Four packet modes and call/referent set agree; retail has 44 CFG blocks and 28 branches versus 42/26 in the probe. Its FT3/GT3 division paths each retain a separate positive-depth branch before the shared ordering-table tail; C already expresses the check, but the probe folds those two branch blocks. The source mechanism for preserving them remains unproved. |
| `0x8002e4dc` | Paired textured packet walker | **WIP, 96.587975%**. The same 44/42 block and 28/26 branch gap persists. A source-only typed header-view probe gave the same listing and was discarded. |
| `0x8002ebe0` | Blended/lit packet walker | **WIP, 95.279450%**. Four colored/textured modes and the full-width blend-bit contribution are modeled; one depth-tail block and branch remain absent. |
| `0x8002f194` | `render_enqueue_map` | **Exact, 100%**; regression control beside the map clipper. |
| `0x8002f5b0` | Clipped GT3 fan builder | **WIP, 95.833336%**. The clipped-vertex and SDK call set agree; retail keeps a different saved-register assignment and orders one `lhu`/`sh` pair before the depth divide. |
| `0x8002f808` | Alternate prepared TMD renderer | **WIP, 63.362473% prior strict**. FT3/FT4 clipping and packet fields are modeled. A retail-backed zero-count guard and postdecrement loop now give 51/51 CFG blocks, 35/35 branches, and 4/4 return frontiers in the focused probe, versus 51/51, 34/35, and 3/4 before. The focused listing rises only 50.3% to 50.5%; the first differing successor is B10, and retail's 168-byte frame versus the probe's 112-byte frame still needs a proven local-object model. |
| `0x8002ff5c` | Prepared TMD object copier | **Unclaimed**. The sole caller passes asset, 16-bit object index, and a 4096-byte output object. Retail's 1248-byte frame subdivides FT4 packets into four and FT3 packets into three using midpoint vertices, copies remaining packets, then appends vertex and normal data. Its roughly 1 KB index workspace and all packet extents are not yet a complete C owner. |
| `0x80030c18` | `render_map_cell_object` | **WIP, 96.521736%**. Call set and typed cell/lighting fields agree; the focused listing differs only in two prologue load/move positions. Moving the object-index read earlier in a temporary source expanded the frame and was discarded. |
| `0x80030de4` | Two-layer map-cell emitter | **Focused SAME; prior strict 89.521280%**. Retail and source have 10/10 CFG blocks, two calls to `render_map_cell_object`, and the same 80-column grid stride. Computing each layer's vertical position before its depth position aligns the lower-layer load and x/z instruction schedule. `kf try game.render_map_cell` now reports the whole listing SAME; a new strict project match has not been run under the focused-only constraint. |
| `0x80030f5c` | Map-cell row traversal | **Exact, 100%**; adjacent regression control. |
| `0x80031024` | Render-model row traversal | **Exact, 100%**; adjacent regression control. |
| `0x800311b0` | Textured screen quad builder | **WIP, 92.148150%**. Four callers and O32 byte/halfword stack loads prove its 15-argument signature. Retail places the packet-code store in a branch delay slot and saves one more register than the probe. |
| `0x80031850` | World-model renderer | **WIP, 84.710450% prior strict**. Its three-argument packet-walker call is corrected. Retail branches to depth projection when the world matrix is null and falls through to ordinary projection; spelling this branch order in C makes the focused 40/40-block, 16/16-branch CFG successor lists agree, improving the focused listing from 69.8% to 70.5%. Graphics-base and coordinate evaluation order still differ. |
| `0x80031d8c` | Animated-object renderer | **WIP, 94.654140%**. Seven CFG blocks agree; retail retains its final render-data argument in `s7` while the probe reloads the stack argument. |
| `0x800321d8` | `resource_tmd_queue_read` | **WIP, 98.435900%**. Calls, registry referents, and three CFG blocks agree. Retail forms the fixed arena `0x8009b0a0` with `lui/addiu`; the provisional C literal forms `lui/ori`. A temporary extern probe reproduces the opcode, but the arena symbol's binding and extent remain unproved, so the source literal stays unchanged. |
| `0x8003247c` | Per-frame placed-object/resource dispatcher | **Unclaimed**. The 0xb70-byte, 96-block body calls both range updaters, five world-model draws, an animated-object draw, and mask/sound helpers. Its mixed placed-map, actor, and render record ownership needs a complete typed model before a C claim. |

The three narrow GAME function-identity corrections at `0x8002ff5c`,
`0x80030de4`, and `0x800311b0` now record the retail/caller-supported
signatures. `kf sema --image game addr` parses all three; focused comparisons
of their existing caller/owner units preserve the prior WIP and exact sibling
listings. The `0x8002f808` loop change preserves exact `render_enqueue_map`.
No repository tests, lint, full build, broad match, bank, or commit were run.
