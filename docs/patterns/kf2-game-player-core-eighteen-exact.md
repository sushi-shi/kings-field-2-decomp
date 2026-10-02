# GAME player core eighteen-function exact controls

Focused rebuilds of seven player units showed identical listings. Fresh
safe one-VA GAME delinks and isolated strict objdiff checks confirm **100%**
`.text` for every function below. No C or profile changed in this pass.

| GAME VA | Unit | Retail text bytes |
| --- | --- | ---: |
| `0x80028224` | `game.player_camera_turn` | 760 |
| `0x8002851c` | `game.player_camera_turn` | 1,120 |
| `0x80023430` | `game.player_distance_margin` | 84 |
| `0x80023484` | `game.player_reset_view` | 236 |
| `0x800247e4` | `game.player_status_cap` | 196 |
| `0x800316c8` | `game.player_weapon_render` | 392 |
| `0x80026330` | `game.player_weapon_transform_power` | 308 |
| `0x80026464` | `game.player_weapon_transform_power` | 52 |
| `0x80023570` | `game.player_core_run` | 156 |
| `0x8002360c` | `game.player_core_run` | 520 |
| `0x80023814` | `game.player_core_run` | 84 |
| `0x80023868` | `game.player_core_run` | 284 |
| `0x80023984` | `game.player_core_run` | 1,712 |
| `0x80024034` | `game.player_core_run` | 152 |
| `0x800240cc` | `game.player_core_run` | 152 |
| `0x80024164` | `game.player_core_run` | 544 |
| `0x80024384` | `game.player_core_run` | 196 |
| `0x80024448` | `game.player_core_run` | 80 |

These exact controls cover camera turn, distance, view reset, status cap,
weapon transform/render, and core progression/damage helpers. They protect
the adjacent player reaction and collision WIPs during later edits. Exact
current objects do not establish the original compiler or TU boundary.
