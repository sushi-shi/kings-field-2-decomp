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

For the two textured packet walkers, a temporary source-only probe changed
the FT3 and GT3 post-divide `if (depth <= 0) break` into an equivalent
positive-depth block. The complete focused `game.tmd_pipeline` comparison was
byte-for-byte identical to the retained source's listing (eight SAME, three
unchanged WIPs). The retail's two separate positive-depth exits therefore
remain unexplained by this ordinary branch spelling; no source change was
retained.

## Current 27-function graphics/TMD control refresh

A targeted twelve-unit rebuild and direct strict objdiff, after the current
GAME delink and shared-header changes, gives **24/27 exact functions**.
`game.tmd_prepare_primitive_indices` is now strictly exact: its 724-byte
body and 116-byte switch data both compare at 100%. Its prior 96.13260%
aggregate score was stale relative to the current source and target; a focused
`kf try` independently gives one identical listing. No new source edit was
needed or retained.

| Unit | Current direct strict verdict |
| --- | --- |
| `game.world_translate` | 1/1 exact |
| `game.primitive_buffer` | 3/3 exact |
| `game.display` | 10/10 exact |
| `game.tmd_prepare_primitive_indices` | 1/1 exact; 116/116 `.rodata` bytes exact |
| `game.graphics_textured_quad` | `0x800311b0` WIP, 92.14815% |
| `game.graphics_sliding_panels` | 2/2 exact |
| `game.graphics_color_bytes_draw` | 1/1 exact |
| `game.graphics_color_bytes_set` | 1/1 exact |
| `game.render_world_model` | `0x80031850` WIP, 95.32836% |
| `game.render_animated_object` | `0x80031d8c` WIP, 94.65414% |
| `game.render_frame` | 2/2 exact |
| `game.animation_sparse_vertices` | 3/3 exact |

The retained world-model source has the raw-backed branch-local collision
row and typed byte-offset null-path expression. A fresh focused listing
first diverges at the scale-pointer/blend saved-register assignment, then
in the null-path coordinate schedule; its branch and call structure remains
as previously documented. The animated renderer first diverges in the
saved-argument allocation and one extra retail `s7` save, while its calls
and packet path agree. The textured quad retains its packet-code delay-slot
and saved-register residue. No compiler-register steering was added to any
of the three WIPs. This refresh used only targeted builds, direct per-unit
objdiff, and focused `kf try`; it did not run repository tests, lint, a full
linked build, broad matching, or banking.

The adjacent large dispatcher `game.render_resource_dispatch` also received
one raw-backed actor cursor correction. Retail keeps the actor's position
pointer separate from the actor record pointer and advances both by the
`sizeof(KfActor)` stride (124 bytes) at their common loop tail. Keeping a
typed `VECTOR` member pointer live across the loop in C now emits that
separate increment; the visibility checks continue to use this pointer,
while the world-render call still uses its distinct draw-position result.
Its direct strict score rises from 90.63798% to **90.69262%**. The body
still has 96/96 CFG blocks and 54/54 branches, with the first successor
discrepancy at effect-pass B62; the 32-byte identity matrix remains exact.
No effect-pass source shape was inferred from the small score change.

The effect renderer's sixth argument has a separate raw-proven field owner.
Retail initializes its effect cache cursor at each record's `+0x3c`, passes
that address to both normal and special `func_80031850` calls, and advances
the cursor by the 72-byte record stride. The earlier C passed the direction
vector at `+0x34`. Both calls now use the first word of the record's existing
12-byte tail; no other source reads or writes that first word. The rest of
the tail still lacks a complete shared field model, so its name remains WIP.
The C now keeps a typed cache pointer separate from the record pointer
through the loop and advances it by `sizeof(KfEffectRecord)` at the shared
count tail, matching the retail `s3 += 72` instruction. A parallel typed
scale cursor begins at record `+0x2c`, feeds both render calls, and advances
by the same record stride, matching retail `s2 += 72`. The rotation cursor
begins at record `+0x24`, supplies modes 4, 8, and 12, and advances 72 bytes
in parallel with retail `s5`. Focused compilation confirms all three typed
member addends; direct strict objdiff rises from **90.69262%** through
**91.10109%** and **91.64891%** to **92.00273%**, with the 32-byte identity
matrix exact. The first differing CFG successor remains at effect-pass B62,
so these cursor findings do not close the dispatcher. Retail proves the
distinct pointer values and strides; whether the original C spelled each as
a separate local remains WIP.

A fresh two-unit map-cell/asset-registry control after the effect cursor edits
retains **6/7 strict exact** functions and 540/540 initialized map-cell data
bytes. `render_map_cell_object` is unchanged at **96.521736%** direct strict:
its focused 98.7% listing differs only in the order of the independent
orientation-byte load, third-argument move, and first view-matrix address
setup before `SetRotMatrix`. The other three map-cell functions and all
three asset-registry functions remain exact. No source edit was justified by
that entry schedule.

The world-model object's selected index is now a `u16` local. Retail narrows
the `clip & 0x7f` result with `andi` before the object-selection calls, and
the shared `tmd_select_object_vertices`, `tmd_get_object`, and packet-render
APIs take a `u16` object index. The previous `s32` local omitted that
conversion. A focused off-tree source probe improved the listing from 84.2%
to 87.1%; the retained source improves fresh direct strict objdiff from
**95.0% to 95.32836%**. The same 12-unit graphics/TMD control remains
**24/27 exact**, with its other two WIPs unchanged. The remaining
world-model register and instruction-order differences are unresolved.
