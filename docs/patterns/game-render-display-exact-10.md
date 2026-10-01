# GAME display and graphics-control exact pass

This related pass follows frame setup, the two sliding textured panels, and
graphics control/color updates. Each address is in GAME.EXE. The current
claimed source and retail function identity were checked with focused unit
builds and fresh direct objdiff against the corresponding delinked unit. All
ten are strict 100%; no source correction was indicated.

| VA | Function | Unit | Verdict |
| --- | --- | --- | --- |
| `0x8002d0a4` | `fog_set_near` | `game.display` | Exact, direct strict 100% |
| `0x8002d0d4` | `display_initialize` | `game.display` | Exact, direct strict 100% |
| `0x8002d248` | `display_reset` | `game.display` | Exact, direct strict 100% |
| `0x8002d32c` | `display_begin_frame` | `game.display` | Exact, direct strict 100% |
| `0x8002d3c4` | `display_present_frame` | `game.display` | Exact, direct strict 100% |
| `0x800312f4` | `func_800312f4` | `game.graphics_sliding_panels` | Exact, direct strict 100% |
| `0x80031384` | `func_80031384` | `game.graphics_sliding_panels` | Exact, direct strict 100% |
| `0x80031414` | `func_80031414` | `game.graphics_color_bytes_draw` | Exact, direct strict 100% |
| `0x800314d4` | `func_800314d4` | `game.graphics_color_bytes_set` | Exact, direct strict 100% |
| `0x800314fc` | `func_800314fc` | `game.collision_channel_draw` | Exact, direct strict 100% |

The five adjacent `game.display` TMD selector/object helpers also remain
direct strict 100%, as do the related focused listings for
`game.collision_channel_add`, `game.notification_quad`, `game.render_frame`,
and `game.asset_registry`. The related `func_800316c8` player-weapon renderer
also remains direct strict 100% after a fresh focused rebuild. This pass used
only focused builds and direct unit comparisons; no tests, full build, broad
match, or banking were run.
