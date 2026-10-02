# GAME event-to-actor animation: 26 current strict verdicts

The event target stream calls actor phase seeking and frame presentation;
event pose and controller paths share that frame entry, while keyframe and
sparse-vertex helpers supply animated objects. Fifteen related GAME source
units were freshly compiled with their full profiles and compared by isolated
strict objdiff. **Twenty-one functions are exact and five remain WIP.**
This is a current-image confirmation; the earlier
[event dispatch audit](kf2-game-event-dispatch-audit.md) and
[render callgraph](game-tmd-render-callgraph-25.md) hold the longer raw
investigations and negative source trials.

| GAME address | Source unit | Strict text | Verdict |
| --- | --- | ---: | --- |
| `0x80031d8c` | `render_animated_object` | 94.65414% | WIP: 7/7 CFG, 2/2 branches; retail retains the typed sixth blend argument where the probe reloads it. |
| `0x80033584` | `render_frame` | 100% | Exact display-buffer toggle. |
| `0x800335a0` | `render_frame` | 100% | Exact shared frame presenter; four-byte datum exact. |
| `0x80033b34` | `animation_keyframe` | 100% | Exact keyframe selection. |
| `0x80033bfc` | `animation_sparse_vertices` | 100% | Exact sparse-vertex expansion. |
| `0x80033cc0` | `animation_sparse_vertices` | 100% | Exact sparse-vertex decode. |
| `0x80033d3c` | `animation_sparse_vertices` | 100% | Exact sparse-vertex helper. |
| `0x80033ff4` | `animation_sparse_find` | 100% | Exact sparse-vertex lookup. |
| `0x80045f20` | `event_pose_interpolate` | 100% | Exact pose interpolation. |
| `0x80045fd4` | `event_pose_interpolate` | 100% | Exact companion pose interpolation. |
| `0x800462bc` | `event_target_stream` | 98.68132% | WIP: 46/46 CFG and 21/21 branches; independent phase/state load order, with 64-byte table 92.1875% by addend. |
| `0x80046700` | `event_map_object_spawn` | 100% | Exact map-object spawn callback. |
| `0x8004678c` | `event_command_dispatch` | 100% | Exact 3,156-byte command dispatcher, 40-byte data and 140-byte table exact. |
| `0x800473e0` | `event_counter` | 100% | Exact event counter helper. |
| `0x80047434` | `event_counter` | 100% | Exact event counter helper. |
| `0x800474c4` | `event_counter` | 100% | Exact event counter helper. |
| `0x800475d8` | `event_map_object_controller` | 99.166664% | WIP: 55/54 CFG, 29/29 branches; unused local zero falls through in retail but moves into the probe delay slot. |
| `0x80047c98` | `event_world_dispatch` | 99.81618% | WIP: 85/85 CFG, 55/55 branches; rotation and constant-one saved-register roles exchange. |
| `0x800482f8` | `event_state` | 100% | Exact event state helper. |
| `0x800483a8` | `event_state` | 100% | Exact callback-slot invoker. |
| `0x800483d8` | `event_state` | 100% | Exact event state helper. |
| `0x80048428` | `event_state` | 100% | Exact event state helper. |
| `0x80048498` | `event_state` | 100% | Exact event state helper. |
| `0x800484e4` | `event_state` | 100% | Exact event state helper. |
| `0x80048554` | `event_save_stream` | 100% | Exact save encoder and 660-byte table. |
| `0x800489ac` | `event_restore_stream` | 98.82883% | WIP: 23/23 CFG, 7/7 branches; actor base and sentinel registers exchange, 64-byte table exact. |

The target-stream and restore decoder each contain an unresolved indirect
jump, so matching block counts are not complete reachability proofs. The
controller's one-block difference is a local assignment that the taken path
never observes; its 62 ordered referent identities remain aligned. The
animated renderer's sixth argument was already corrected to the retail
integer blend mode. None of these residues supplies a new call, field width,
table identity, or source-control correction. No C or data-owner edit was
retained, and no new 100% closure is claimed.
