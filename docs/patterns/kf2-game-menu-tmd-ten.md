# GAME item-list and TMD renderer pilot

This ten-function pilot follows confirmed item-list calls and the connected
TMD render pipeline. For each function, retail block disassembly, CFG,
callers, callees, strings, data references, relocation evidence, adjacent
claims, current match state, source history, and the KF1 counterpart were
checked. Addresses are GAME.EXE identities only.

| Address | Final verdict | Evidence and remaining work |
| --- | --- | --- |
| `0x8001d8d0` | WIP, unclaimed | The 0x394-byte item-list controller calls the primary item-code translator, `menu_list_init`, model preview, frame/present helpers, and `0x1e484`. Its selected-record view at `0x80065aec+0x44` and shared state at `0x8006d694` are still candidate owners. |
| `0x8001ddd0` | WIP, unclaimed | A 0x2d8-byte sibling calls the same item-list/model/input helpers and addresses `0x80065aec+0xbc`. The complete list record and selected-state semantics remain unproved. |
| `0x8001e0a8` | WIP, unclaimed | A 0x2d0-byte sibling uses the secondary item-code translator and the same preview/input/frame loop. Its indirect list state is not yet a complete typed object. |
| `0x8001e484` | WIP, unclaimed | The 0x4c8-byte input/model-preview helper has 16 direct callers and updates the identified rotation, rotation-step, translation, and idle-counter globals. Its input decision record and full state contract remain unresolved. |
| `0x8002ddb4` | WIP, **97.34716% strict** | New C renders FT3/FT4/GT3/GT4 packets with the caller-proved third blend-mode argument, face normals, depth cue, clip, and ordering-table bounds. Retail keeps two additional CFG blocks around the per-face depth exit and shared `AddPrim` tail; an explicit-label probe worsened the match and was discarded. |
| `0x8002e4dc` | WIP, **96.587975% strict** | Adjacent four-mode textured renderer retains each TMD packet's tpage and raw GPU mode byte. The call set, texture fields, packet extents, and KF1 render-enqueue shape agree; the same shared depth-tail difference remains. |
| `0x8002ebe0` | WIP, **95.8% strict** | New C handles textured GT3/GT4 and untextured G3/G4 through `NormalColorCol3/Col`. The sole caller passes a sign-extended 16-bit fixed OT depth; a separate argument supplies tpage blend bits. Its remaining packet ordering and tail schedule are not attributed. |
| `0x8002f5b0` | WIP, **95.833336% strict** | Existing source has 13/13 CFG blocks and matching calls/referents. First divergence is s3/s4 assignment, followed by UV-store and depth-division scheduling; no independently supported source correction was found. |
| `0x8002f808` | WIP, **63.362473% strict** | Existing prepared-map renderer has 51/51 CFG blocks but one branch and return-frontier difference. Preserve its typed packet and clipped-vertex owners pending a source-backed control-flow correction. |
| `0x8002ff5c` | WIP, unclaimed | The 0xcbc-byte prepared-TMD builder has a 1248-byte frame, copies packet words, and writes the header/object/payload into the caller's 4096-byte prepared asset. Multiple packet-mode conversions and stack views need full ownership before a C claim. |

`game.tmd_pipeline` now owns the contiguous 0x8002d8b0–0x8002f194
run in ascending claim order. Its eight earlier listings remain strict 100%;
the three added renderer listings remain WIP. The merged unit reduced the
GAME module count by one, and all 145 target units relinked. Global edge
closure remains blocked by three unrelated, established TMD/map-object
`.rodata` addends. A subsequent full `kf build` built PSX.EXE;
GAME/OPEN/END retain their known first unresolved link symbols
(`InitCARD`, `malloc`, `display_buffers`). No repository tests or banking
were run.
