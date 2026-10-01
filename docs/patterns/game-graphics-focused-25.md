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

## Connected world-model follow-up

`0x80031850` has one 0x53c-byte source claim in `game.render_world_model`.
Five proven calls from `0x8003247c` reach it; the retail body has no string
references. Its current focused listing is DIFF 80.5%. A direct one-unit
objdiff initially read an old base object and reported 86.4%; after explicitly
rebuilding only `build/objdiff/game/base/80031850_render_world_model.o`, the
fresh strict result is **94.0%**. The whole-image `kf sema match` view still
shows the stale 86.4% report and must not be used as this source revision's
verdict.

The target and source retain 40 CFG blocks, 16 branches, the same known
successor lists, and the same proven call set. The 68 ordered relocation rows
also agree. The first focused differences are the saved-register choice for
the scale pointer and blend value, followed by the order of X/Z map-cell
coordinate calculations. The current typed source computes the supported
collision-row and null-world lighting addresses without a missing data
reference; no source-backed correction was found in this pass. WIP retained.

KF1 master is a structural comparison, not an instruction template here.
Its `render_screen_sprite` is strict 100% and also advances a 40-byte
`POLY_FT4` cursor, checks the buffer bound, fills XY/UV fields, and calls
`AddPrim`. KF1 retail also has a proven `jal SetPolyFT4` at `0x8001e4cc`
using the Release 2.5 SDK.
KF2 retail `0x800311b0` has no packet-setup call and directly stores the
length/code bytes, as its Psy-Q 3.0 lowercase macro spelling does. KF1's
`render_actor` and `render_entities` likewise share animation-cache and
pool-loop ideas with KF2 but use different lighting and render call sets.
Those analogs support the current object roles while offering no justified
KF2 source edit for the remaining register/CFG differences.

KF1 commit `4a01223d` reached the graphics owner by replacing loose display
globals with `game_graphics_runtime` fields while retaining the quad packet
logic; it supplies no alternative KF2 packet-store ordering.

In the KF2 dispatcher's raw effect pass (`0x80032c38`–`0x80032e6c`), retail
keeps the effect base and five field-address streams in saved registers. The
normal draw passes position, rotation, scale, and cache from those streams,
then advances them at the common loop tail; mode 12 makes a separate draw
with a null world matrix. The current compiler derives fewer persistent
streams from the typed record pointer and advances some before the normal
call. The retail addresses prove the field offsets and call arguments, but
they do not prove separate source-level pointer variables. Adding redundant
locals solely to hold those registers would be codegen steering, so the
dispatcher remains WIP.

KF1's `render_entities` is a useful counterexample: its source has one
`sprite++` record pointer, yet the compiled effect loop advances two saved
address streams by its 60-byte record stride. Multiple compiled streams alone
therefore cannot establish multiple source pointers for KF2.

KF1 also has a strict-exact eight-mode `tmd_prepare_primitive_indices` at
GAME `0x8001c2b0`. Its field shifts agree with KF2's typed switch, but KF1
reads the current TMD asset from a global and guards zero counts before its
do-loops. KF2 retail takes the asset pointer as an argument and its entry
decrements the count before the zero test. Prior KF2 trials of the KF1 loop
spelling changed register/frame order without removing the extra entry move;
the counterpart confirms packet fields, not a transferable loop syntax.

Two exact-control units were rebuilt by targeting only their base objects and
compared in separate one-unit objdiff
projects. `game.render_frame` is **2/2 strict 100%** (`0x80033584`,
`0x800335a0`); `game.asset_registry` is **3/3 strict 100%** (`0x800339fc`,
`0x80033ab4`, `0x80033afc`). These controls stayed exact across the current
graphics-runtime and data-owner changes.

Three disjoint WIP renderer base objects were also explicitly rebuilt and
compared in separate one-unit strict reports. Their current scores are
`0x800311b0` **92.14815%**, `0x80031d8c` **94.65414%**, and `0x8003247c`
**90.63798%**. These reproduce the earlier direct baselines; the intervening
data-owner work has not silently changed the code verdicts. The dispatcher's
32-byte `render_world_identity_matrix` remains strict 100% data within its
one-unit report.

Fresh isolated focused builds also preserved seven adjacent exact listings:
`notification_draw_quad` and `notification_draw` (2/2), `notify_enqueue`,
`notification_digit_set_v`, and `func_80033284` (3/3), plus
`func_800314fc` and `func_80031634` (1/1 each). The quad renderer's authentic
Psy-Q packet macros and chained coordinate writes remain source-backed
controls for `0x800311b0`; they do not establish the missing saved-register
or packet-code scheduling choice in that WIP. These seven checks are focused
listing controls, with earlier strict-exact verdicts, rather than a fresh
whole-image objdiff report.

The released `0x8002ebe0` blended TMD walker received another full retail
semantic pass. Its only external direct caller is `0x80031d8c`; there are no
strings. The four packet modes converge at retail `0x8002f114`, which stores
the packet code, reloads and sign-extends the fixed-depth argument, checks
positive depth and the `0x2000` ordering-table bound separately, then calls
`AddPrim` once. The current C duplicates the equivalent guard in each mode,
and the compiler hoists its validity check to entry, giving 25 rather than 26
CFG blocks. Earlier source-only shared-tail and nested-guard probes recovered
parts of the late CFG but changed the frame and worsened the direct strict
result; the raw tail does not prove which original C spelling avoided that
hoist. The source remains WIP at its prior **95.27945%** direct strict result.
A fresh focused `game.tmd_pipeline` build has eight identical listings and
three packet-walker WIPs, with no new source edit retained.

Rebuilding the four retained renderer WIPs after the concurrent shared-header
edits preserved their focused listing results: `0x800311b0` 57.5%,
`0x80031850` 80.5%, `0x80031d8c` 79.5%, and `0x8003247c` 67.7%. The
dispatcher still reports 96/96 CFG blocks, 54/54 branches, and the same first
known successor discrepancy at B62 in effect handling.
