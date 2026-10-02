# GAME renderer graph: 25 strict verdicts

This cohort follows map-cell and world-model rendering into the frame
dispatcher, its animated-object path, and the textured-quad helper. The 25
rows are fresh isolated strict `objdiff-cli diff` results after focused
`kf try` builds. Call relationships establish the graph; adjacency does not
imply a common translation unit. The five WIPs have matching known call sets
and CFG topology, so this pass retains their current C and identity claims.

| GAME address | Function | Strict text | Verdict |
| --- | --- | ---: | --- |
| `0x8002d32c` | `display_begin_frame` | 100% | Exact frame-setup control. |
| `0x8002d3c4` | `display_present_frame` | 100% | Exact frame-present control. |
| `0x8002d458` | `tmd_select` | 100% | Exact TMD selector. |
| `0x8002d484` | `tmd_get_object` | 100% | Exact current-object getter. |
| `0x8002d4a8` | `tmd_set_current_vertices` | 100% | Exact vertex-pointer setter. |
| `0x8002d5dc` | `tmd_prepare_primitive_indices` | 100% | Exact prepared-index control. |
| `0x8002f194` | `render_enqueue_map` | 100% | Exact map-primitive enqueue control. |
| `0x80030c18` | `render_map_cell_object` | 96.521736% | WIP: first divergence is quarter-turn byte-load and flag-move order before `SetRotMatrix`; 11/11 CFG blocks and 6/6 branches. |
| `0x80030de4` | `func_80030de4` | 100% | Exact map-cell sibling. |
| `0x80030f5c` | `func_80030f5c` | 100% | Exact map-cell sibling. |
| `0x80031024` | `func_80031024` | 100% | Exact map-cell sibling. |
| `0x800311b0` | `func_800311b0` | 92.148150% | WIP textured quad: saved-register and packet-code delay-slot schedule; 8/8 blocks and 5/5 branches. |
| `0x800312f4` | `func_800312f4` | 100% | Exact quad caller. |
| `0x80031384` | `func_80031384` | 100% | Exact quad caller. |
| `0x80031414` | `func_80031414` | 100% | Exact color-byte quad caller. |
| `0x800314d4` | `func_800314d4` | 100% | Exact color-byte setter. |
| `0x800316c8` | `func_800316c8` | 100% | Exact player-weapon render control. |
| `0x80031850` | `func_80031850` | 99.298510% | WIP world model: ordered referents and calls agree; first divergence is saved-register assignment, with 40/40 blocks and 16/16 branches. |
| `0x80031d8c` | `func_80031d8c` | 94.654140% | WIP animated object: retail saves the sixth blend argument where the probe reloads its stack home; 7/7 blocks and 2/2 branches. |
| `0x8003247c` | `func_8003247c` | 92.185790% | WIP resource dispatcher: 768-byte retail versus 760-byte probe frame; 96/96 blocks and 54/54 branches. |
| `0x80033584` | `display_toggle_buffer_index` | 100% | Exact frame-buffer toggle. |
| `0x800335a0` | `func_800335a0` | 100% | Exact frame controller and direct dispatcher caller. |
| `0x800339fc` | `asset_registry_load_tmd_archive` | 100% | Exact TMD archive loader. |
| `0x80033ab4` | `asset_registry_set` | 100% | Exact asset registry writer. |
| `0x80033afc` | `asset_registry_select` | 100% | Exact asset registry selector. |

**Twenty controls are strict exact; five functions remain WIP.** Raw retail
calls establish the textured-quad helper's four local callers and the
dispatcher call to the animated-object renderer. The dispatcher retains its
actor, map-object, effect, and placed-object passes; its five world-renderer
calls and one animated-renderer call are accounted for. The map-cell unit's
540-byte initialized data and the dispatcher's 32-byte identity matrix also
compare exactly. The dispatcher's two excess candidate text relocations come
from camera-base rematerialization, not a proved extra referent.

The first differences above do not establish a missing branch, argument,
field, or data owner. In particular, no raw access proves a missing object
inside the dispatcher's eight-byte frame gap. The blend argument's width and
forwarded value are supported by its caller and callee. KF1 render source is
useful for the general packet path but cannot establish KF2's prepared TMD
builder or these instruction schedules. No source/config edit was retained;
the fuller function-by-function evidence is in
[game-tmd-render-callgraph-25.md](game-tmd-render-callgraph-25.md).
