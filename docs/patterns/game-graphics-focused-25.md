# GAME graphics and render focused 25

This pass follows the frame driver's primitive-buffer, screen-quad, map-cell,
TMD, and asset-registry calls. A fresh, isolated `kf try --no-flow` build was
run for each listed unit. `SAME` below means an identical focused listing; it
does not by itself certify strict objdiff 100%. No source or target-inventory
change was retained from this pass.

| Unit | GAME function verdicts from the focused builds |
| --- | --- |
| `game.primitive_buffer` | `0x80021f10` SAME; `0x80021f60` SAME; `0x80021fb0` SAME. |
| `game.tmd_prepare_primitive_indices` | `0x8002d5dc` DIFF, 38.6% listing; prior direct strict 96.13260%. |
| `game.tmd_prepared_subdivide` | `0x8002ff5c` DIFF, 12.0% listing; prior direct strict 51.50675%. |
| `game.render_map_cell` | `0x80030c18` DIFF, 98.7% listing; prior direct strict 96.521736%. `0x80030de4` SAME, prior direct strict 100%; `0x80030f5c` SAME; `0x80031024` SAME. |
| `game.graphics_textured_quad` | `0x800311b0` DIFF, 57.5% listing; prior direct strict 92.14815%. |
| `game.graphics_sliding_panels` | `0x800312f4` SAME; `0x80031384` SAME. |
| `game.graphics_color_bytes_draw` | `0x80031414` SAME. |
| `game.graphics_color_bytes_set` | `0x800314d4` SAME. |
| `game.player_weapon_render` | `0x800316c8` SAME. |
| `game.render_animated_object` | `0x80031d8c` DIFF, 79.5% listing; prior direct strict 94.65414%. |
| `game.render_resource_dispatch` | `0x8003247c` DIFF, 67.7% listing; prior direct strict 90.63798%. |
| `game.render_frame` | `0x80033584` SAME; `0x800335a0` SAME. |
| `game.asset_registry` | `0x800339fc` SAME; `0x80033ab4` SAME; `0x80033afc` SAME. |
| `game.asset_vertex_count` | `0x80034070` SAME; `0x80034344` SAME; `0x800345e4` SAME. |

The result is 19 identical listings and six WIPs across 25 source claims.
The prior strict scores above come from the individual direct objdiff dossiers;
this focused pass did not refresh a whole-image report or bank any function.
The `0x8002ff5c` and `0x80030c18` source files are released to the separate
prepared-object campaign for follow-up.

The full retail evidence pass for `0x800311b0` confirms a 0x144-byte body,
four proven screen-quad callers, no string references, one proven `AddPrim`
call, and three validated graphics-runtime address pairs. O32 stack loads
prove byte texture/color arguments and halfword tpage/clut arguments. Its
packet stores and depth guard are represented in the current typed `POLY_FT4`
source. The first focused mismatch is saved-register assignment; later the
retail packet-code store fills the semitransparency branch delay slot, while
the probe stores it earlier. There is no missing referent, call, or supported
source fact to justify changing the C merely to move these instructions.

The large `0x8003247c` dispatcher still has 96/96 CFG blocks, 54/54 branches,
one return, and the documented 82 ordered relocation rows. Its first differing
known successor is in effect handling at B62: retail's taken edge is B79 and
the probe's is B78; both fall through to B63. The existing typed effect draw
paths have not yet established the original source construct that yields that
edge. The 768/760-byte retail/probe frame difference and effect-path register
lifetimes remain open, so no speculative local or forced branch was added.

Only isolated `kf try` builds and retail semantic reads were performed. No
repository tests, lint, full linked build, or banking was run.
