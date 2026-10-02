# GAME map, animation, and frame focused controls

The current GAME sources were compiled individually with each unit's pinned
manifest profile, including its MIPS CPU flag, and compared by isolated strict
objdiff against the existing safe-delink target objects. These were focused
object comparisons only; no source, identity, relocation, or profile change
was made.

Nine map and animation units are already **100% exact** in all 18 function
claims. Their target/candidate `.rel.text` row counts also agree:

| Unit | Exact functions | Relocation rows, target/candidate |
| --- | ---: | ---: |
| `game.collision_grid_sample` | 1 | 21/21 |
| `game.animation_keyframe` | 1 | 1/1 |
| `game.animation_sparse_find` | 1 | 1/1 |
| `game.map_placed_expand` | 1 | 5/5 |
| `game.map_object_reset` | 6 | 28/28 |
| `game.map_object_collision_query` | 1 | 7/7 |
| `game.map_object_spawn_scatter` | 4 | 27/27 |
| `game.map_object_vertex_world` | 2 | 9/9 |
| `game.map_object_motion` | 1 | 14/14 |

An independent 11-unit frame/graphics screen gives 13 exact functions and
three unchanged WIPs. Exact units are `graphics_sliding_panels` (2),
`graphics_color_bytes_draw`, `graphics_color_bytes_set`,
`collision_channel_draw`, `collision_channel_add`, `player_weapon_render`,
`render_frame` (2), and `menu_model_render`; the first function of
`message_stream_find_marker` and two siblings of `menu_transition` are also
exact. All 11 units have equal target/candidate `.rel.text` row counts.

| WIP address | Fresh strict text | Target/candidate unit relocation rows | Current raw/source verdict |
| --- | ---: | ---: | --- |
| `0x80036e24` frame/CD service | 98.86364% | 5/5 | Existing raw audit found the five direct calls, 5/5 CFG blocks, and 2/2 branches present. The remaining mode/end/step saved-register assignment has no supported source correction. |
| `0x800461a0` marker search | 99.12676% | 13/13 | Existing raw audit found 14/14 CFG blocks and 5/5 branches. The stream cursor and marker pointers occupy exchanged registers; preserving the non-advancing retry path matters more than this allocation. |
| `0x800349bc` menu fade | 96.31408% | 96/96 | Four textured quads and their call/branch topology remain present. Retail spills the pad state in a 72-byte frame; the candidate uses a saved register and 64-byte frame. No extra live object is proved, so no padding or fake spill was added. |

The 18 exact map/animation claims and 13 exact frame/graphics claims are
existing matches, not newly closed functions. The three WIPs retain their
prior source-backed semantics and remain unbanked.

The initialized object claims visible in these strict reports are also
byte-exact: `collision_default_cell`,
`render_model_yaw_smoothing_accumulator`, and `menu_transition_rect` each
score 100%. The separate BSS symbol `map_object_state` has no initialized-byte
score; this report makes no placement claim for it.
