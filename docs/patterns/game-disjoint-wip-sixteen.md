# GAME disjoint WIP strict screen

Ten GAME units were compiled individually with the manifest's pinned GCC
profile and compared against their current delinked target objects by isolated
strict objdiff. All **33 functions** received a final verdict in this pass:
**17 existing exact controls and 16 WIPs**, with no exact-count movement.

| Unit | Function verdicts | Target/probe `.rel.text` rows |
| --- | --- | ---: |
| `game.menu_draw_window` | `menu_draw_window` WIP 99.78788%. | 19/19 |
| `game.menu_draw_string` | `menu_draw_string` WIP 99.66904%. | 54/54 |
| `game.menu_format_number` | `menu_format_number` WIP 97.39%. | 7/7 |
| `game.collision_height_wrappers` | Exact: `func_8002b604`, `func_8002b7f8`, `func_8002bc18`, `func_8002bd3c`, `func_8002bdbc`, `func_8002be9c`, `func_8002bf38`, `func_8002bfac`, `func_8002c170`, `func_8002c1d4`, `func_8002c290`. WIP: `func_8002b67c` 94.895836%, `func_8002b73c` 98.40426%, `func_8002b874` 91.5%, `func_8002b9d4` 95.37931%, `func_8002bfd4` 73.91262%, `func_8002c424` 98.29932%. | 199/195 |
| `game.graphics_textured_quad` | `func_800311b0` WIP 92.14815%. | 7/7 |
| `game.render_map_cell` | `render_map_cell_object` WIP 96.521736%; `func_80030de4`, `func_80030f5c`, `func_80031024` exact. | 77/77 |
| `game.actor_lifecycle_target` | `func_8003983c` WIP 99.19598%; `func_80039b58` WIP 90.95744%. | 52/52 |
| `game.actor_group_position` | `func_8003c000`, `func_8003c10c`, `func_8003c220` exact; `func_8003c3e0` WIP 99.64539%. | 37/37 |
| `game.event_target_stream` | `func_800462bc` WIP 98.68132%. | 57/57 |
| `game.event_map_object_controller` | `func_800475d8` WIP 99.166664%. | 62/62 |

Retail `func_80039b58` uses a 124-byte actor stride, byte fields at `+0`,
`+2`, and `+9`, and the same two calls as the typed C. Its separate actor and
field cursors do not justify a duplicate C pointer invented for codegen.
Retail `func_800462bc` saves actor byte `+12` at `0x80046528`, then
deliberately reloads it at `0x80046538` as the first phase-change call's
second argument. The source likewise saves and reloads it; replacing that
argument with the saved byte would be wrong. The 64-byte table's one shifted
target remains downstream of independent load scheduling. All other WIP
referent, call, field, and CFG limits are documented in their dedicated
collision, graphics, actor, menu, and event notes. The collision-wrapper
relocation count difference is an address-materialization residue, not a new
data identity: ordered relocation sequences differ only by two additional
target `bss_801c7540` HI16/LO16 pairs, in the height and snapshot helpers.
All nine other units have identical ordered text-relocation type and symbol
sequences, not merely equal counts. No C, inventory, or compiler-profile edit
was retained.
