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
| `game.render_world_model` | `0x80031850` WIP, 99.29851% |
| `game.render_animated_object` | `0x80031d8c` WIP, 94.65414% |
| `game.render_frame` | 2/2 exact |
| `game.animation_sparse_vertices` | 3/3 exact |

The retained world-model source has the raw-backed branch-local collision
row and a typed null-path map-cell lookup. A fresh focused listing first
diverges at the scale-pointer/blend saved-register assignment; its branch
and call structure remains as previously documented. The animated renderer first diverges in the
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
world-model register and instruction-order differences are unresolved. That
earlier source had all **68/68 ordered** relocation type/symbol rows aligned.

The null-world-matrix path now indexes the complete typed occupancy grid by
the camera's Z row and X column, forms its lighting-index address, then applies
the player's cached layer offset. Retail computes the Z row before the X
column; the former byte-offset expression led the candidate to compute X
first. The typed source raises focused listing similarity from 87.1% to
92.3% and direct strict objdiff from **95.32836% to 97.11642%** after a
targeted one-unit rebuild. The **68 relocation types and symbol names**
remain present, but the candidate now forms the BSS map base before loading
the player-state layer offset, reversing two HI16/LO16 pairs relative to
retail. It also forms the base of the cell and reads its `+4` field, whereas
retail materializes the lighting-byte address with the `+4` addend before
the load. Both address forms read the same byte; the source remains WIP, and
the original expression spelling is unproved. An off-tree row-pointer
variant preserved the `+4` base addend but moved its address formation even
earlier and scored 95.77612% strict, so it was discarded. Focused flow still
has 40/40 blocks, 16/16 branches, and matching known successors; the first
control difference is a register assignment at branch 10. The exact weapon
renderer reads this same lighting byte through the cached player offset;
collision-shape dispatch updates that offset between zero and five.

The null-path byte is now read as one scalar after the player layer-byte
offset. This retains the typed 80-column grid lookup and matches retail's
ordered `player_state+0x128` and `bss_801c7540+4` address pairs, `lbu 0`,
and surrounding instruction schedule. The prior pointer-local form created
the BSS address too early and selected the same byte with `lbu 4`. A targeted
one-unit rebuild and isolated direct objdiff improve the source from
**97.11642% to 98.92538%**; focused listing rises from 92.3% to **93.8%**.
Retail/source still have 40/40 CFG blocks and 16/16 branches. The remaining
first difference is saved-register assignment. A separate typed row/cell
pointer probe scored 87.3% focused and was discarded.

The selected TMD object index now receives zero inside the `clip < 0x80`
branch, while the high-clip branch assigns its masked index. Retail clears the
selected index in the branch delay slot before either path runs; the previous
source initialized it before the condition and moved that instruction much
earlier. The branch-local source preserves the value in both paths and, after
a targeted one-unit rebuild, improves focused similarity from **93.8% to
94.5%** and isolated direct strict objdiff from **98.92538% to 99.29851%**.
The 40 CFG blocks, 16 branches, and 68 ordered relocation rows remain aligned.
Focused exact controls `game.player_weapon_render` (1/1) and
`game.render_frame` (2/2) remain SAME. Saved-register and independent
instruction-order differences still prevent exact matching.

A focused dispatcher relocation census appears to differ at the player camera
position (`player_state +0xdc` in retail versus `+0xe0` in the probe), but the
actual loads are equivalent. `camera_position` starts at `+0xd8`: retail
loads `vx`/`vz` at `0`/`8` from one saved base and materializes `+0xdc`
separately for `vy`; the probe loads `vx`/`vy` at `0`/`4` from one base and
materializes `+0xe0` separately for `vz`. All three component referents are
correct. Apart from this address-reuse choice and local branch targets, the
external relocation counts, types, symbols, and addends agree. This is not a
missing field or identity claim, so no source or relocation edit is warranted.

A fresh targeted `game.render_resource_dispatch` object comparison measures
**92.00273% direct strict** and 71.0% focused listing. Both objects still have
96 CFG blocks, 54 branches, and 82 ordered relocation rows. The first reported
effect-loop successor difference sends both versions to the same next-record
step (`s0 += 72`); the block numbers differ because their compiled block order
differs. Two off-tree camera-pointer lifetime probes each raised the focused
listing to 72.6%, but neither reproduced retail's early saved `+0xd8` base and
separate `+0xdc` Y address. They were discarded: the three component accesses
already have the right meaning, and a register-lifetime preference alone does
not justify a source change.
Reordering the independent actor-loop cursor increments regressed focused
listing from 71.0% to 70.9% and reassigned their saved registers; moving the
actor count setup ahead of the position cursor kept the 71.0% verdict without
recovering retail's full setup order. Neither source-only probe was retained.

## Fresh isolated render control

Ten graphics/render units were compiled individually from the current sources
with their complete manifest profiles and compared against the existing
delinked target objects. This covers 17 function claims: 12 are strict exact,
and five remain WIP. No C or inventory change was retained.

| GAME function | Current direct strict verdict | Text relocations, target/candidate |
| --- | --- | --- |
| `0x80031850` world model | WIP, 99.29851% | 68/68 |
| `0x80031d8c` animated object | WIP, 94.65414% | 22/22 |
| `0x8003247c` render/resource dispatch | WIP, 92.18579% | 82/84 |
| `0x80030c18` map cell object | WIP, 96.521736% | 77/77 for its unit |
| `0x800311b0` textured quad | WIP, 92.14815% | 7/7 |
| `0x80030de4`, `0x80030f5c`, `0x80031024` map-cell siblings | All exact | 77/77 for their unit |
| `0x800312f4`, `0x80031384`, `0x80031414`, `0x800314d4` panel/color helpers | All exact | Equal per unit |
| `notification_draw_quad`, `notification_draw`, `notify_enqueue`, `notification_digit_set_v`, `func_80033284` | All exact | Equal per unit |

The dispatcher's two extra candidate relocations rematerialize the player
camera base. Raw component addresses remain correct, so the count difference
does not establish a missing owner or field. The other four WIPs retain the
previously documented register, frame, or instruction-order residues. This
control used focused off-tree compilation and isolated strict objdiff only.

## Fresh 26-claim packet and frame control

A separate safe delink and isolated strict compilation from the current sources
covered 26 claims connected by the frame driver's primitive buffer, packet,
map, world-model, and resource-dispatch calls. All 26 compiled with their
manifest profiles. Twenty are strict exact: all three `primitive_buffer`
claims, all ten `display` claims, both sliding-panel claims, both color-byte
claims, both `render_frame` claims, and `render_enqueue_map`. The display BSS
has no byte-comparison score; the 4-byte `render_frame` DATA and 4-byte
`render_map` DATA are exact.

| Non-exact GAME claim | Fresh strict text | Focused CFG and branches | Raw/source verdict |
| --- | ---: | --- | --- |
| `0x8002f5b0` clipped-map packet | 95.833336% | 13/13, 7/7 | The call set and unsigned depth-wrap behavior agree; the first differences are saved-register assignment and independent packet-store scheduling. |
| `0x8002f808` prepared-map renderer | 92.038376% | 51/51, 35/35 | FT4 edge order and known successors agree; the 168/120-byte frame and instruction schedule have no proved missing source object. |
| `0x800311b0` textured quad | 92.14815% | 8/8, 5/5 | The 15 O32 arguments, typed `POLY_FT4` stores, and `AddPrim` call agree; retail saves one extra argument register and places the code-byte store in a different delay slot. |
| `0x80031850` world model | 99.29851% | 40/40, 16/16 | All 68 ordered text referents agree; saved-register and independent instruction order remain. |
| `0x80031d8c` animated object | 94.65414% | 7/7, 2/2 | The caller sign-extends its depth halfword, and the draw calls agree; retail saves the blend argument earlier than the probe. |
| `0x8003247c` resource dispatch | 92.18579% | 96/96, 54/54 | The three camera coordinates agree; two extra candidate address pairs rematerialize their player-state base. The B62 edge reaches the same effect-record advance under different block numbering. |

The two frame/packet groups and their callers have complete verdicts here;
fresh comparisons establish no new exact claim. The remaining differences
provide no independent field, width, call, relocation-owner, or control-flow
fact for a humane C edit. No source or curated inventory was changed.

## Extended 24-claim render call chain

A second narrow safe-target pass followed the map packet emitters and the
frame/resource dispatcher into 18 other confirmed graphics, animation, and
asset callers. All 18 are strict exact: `notification_quad` (2 claims),
`notify_enqueue` (3), `menu_model_render` (1), `asset_registry` (3),
`animation_keyframe` (1), `animation_sparse_vertices` (3),
`animation_sparse_find` (1), `asset_vertex_count` (3), and
`player_weapon_render` (1). The notification unit's 126-byte DATA is exact.
These controls are connected by the scene's draw, asset selection, and
prepared-vertex paths; exactness is established by isolated objdiff against
fresh safe targets, not by the old aggregate report.

The six non-exact claims in this 24-claim chain are the two `render_map`
emitters and `0x800311b0`, `0x80031850`, `0x80031d8c`, and `0x8003247c`
listed above. Focused quick builds reproduce their listed CFG and branch
counts. The first `0x8003247c` control difference at `+0x498` computes the
sound-volume coordinate using different registers; raw source operands still
denote the same camera and map-object fields. Its B62 taken edge names a
different block ordinal but reaches the same effect-record advance. The
`0x8002f808` first divergence remains the 168/120-byte frame and FT4 packet
instruction schedule; the raw body provides no access proving an omitted
48-byte source object. No new source, owner, or relocation edit follows from
this extended control.

The fresh `render_map` safe target and compiled unit each have 100 text
relocations with identical ordered type/symbol pairs. Retail `0x8002f808`
has one physical `AddPrim` call after its FT4/FT3 branches; the current
compiler also merges the two humane source calls to one site. Its retail
call inventory still includes two `NormalClip`, two `NormalColorCol`, seven
`DpqColor`, one `Clip4FTP`, one `Clip3FTP`, and one clipped-fan call.
`render_resource_dispatch` has 82 target versus 84 candidate text
relocations; the only extra candidate pair is HI16/LO16 `player_state` for
the already correct camera-coordinate loads. Its five world-model calls and
one animated-object call match the raw retail sites. The counts and referents
do not support deleting any source reference.
The complete external-call multisets also agree: 36 calls in each
`render_map` object and 27 in each dispatcher object, including all repeated
`DpqColor`, map-mask, range-update, and world-model calls.

An off-tree `-fno-cse-skip-blocks` GCC 2.5.7 control on the unchanged source
kept the exact `render_enqueue_map` sibling but regressed `0x8002f808` from
92.038376% to 86.76119%, `0x80031850` from 99.29851% to 94.96418%, and
`0x8003247c` from 92.18579% to 91.22131%. It left `0x8002f5b0`,
`0x800311b0`, and `0x80031d8c` unchanged. This compiler flag therefore
does not close the render family's instruction-order residue and was not
retained.

An independent off-tree GCC 2.6.0 `-O2` control also regressed every member
of the five render units: `render_enqueue_map` lost exactness (90.1673%),
the clipped/prepared map renderers fell to 73.86%/84.460556%, and the quad,
world, animated, and resource dispatchers fell to 45.493828%, 78.83582%,
75.07519%, and 86.008194%. This compiler substitution is contradicted by
the exact sibling and the wider connected corpus, so it was discarded.

Retail `0x8002f808` stores the decoded packet-mode word at `sp+120` and
reloads it in both textured arms. An off-tree, source-equivalent trial named
that value as `u32 packet_mode = header.word >> 24` and used it for the switch,
packet-code bits, and clipped-fan mode argument. The compiler still kept the
value in a register rather than producing the retail stack lifetime; its
strict text fell from 92.038376% to 91.614075%. The exact enqueue sibling
and clipped-fan score stayed unchanged. The trial was discarded; a forced
stack carrier would not be a supported source correction.
