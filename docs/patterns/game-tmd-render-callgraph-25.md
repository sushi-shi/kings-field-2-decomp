# GAME TMD and render call graph: 25-function pass

This pass follows the prepared TMD index converter through packet enqueue,
map-cell rendering, and the frame's model dispatcher. Each address below is
in GAME.EXE. Retail disassembly/CFG, xrefs, strings, current source and match
state, adjacent claims, vendored attribution, and the KF1 renderer where
applicable were reviewed before source probes. Percentages are the last strict
objdiff report, not a new broad match. Focused `kf try` confirmed the stated
listing residues and exact controls.

| VA | Function or role | Verdict and decisive evidence |
| --- | --- | --- |
| `0x8002d5dc` | `tmd_prepare_primitive_indices` | **Exact, fresh direct strict 100%; focused SAME**. A single typed `KfTmdPrimitive *` now carries the packet-body address both into the eight index-update cases and into the next-packet calculation. This removes the redundant byte-pointer view that made the compiler copy the input asset from `a0` to `t2`. The 724-byte `.text`, 116-byte switch `.rodata`, and all 39 ordered relocation rows match retail. |
| `0x8002d8b0` | `tmd_register` | **Exact, 100%**; regression control in the contiguous TMD unit. |
| `0x8002d8f0` | `tmd_set_slot` | **Exact, 100%**; regression control. |
| `0x8002d910` | `tmd_release_slot` | **Exact, 100%**; regression control. |
| `0x8002d918` | TMD projection with fog depth | **Exact, 100%**; regression control. |
| `0x8002da94` | TMD projection and invalid-vertex marking | **Exact, 100%**; regression control and callee of `0x8002f808`. |
| `0x8002dbd8` | `tmd_transform_vertices` | **Exact, 100%**; regression control. |
| `0x8002dc80` | `tmd_transform_vertices_depth` | **Exact, 100%**; regression control. |
| `0x8002dd28` | `tmd_project_vertices` | **Exact, 100%**; regression control. |
| `0x8002ddb4` | Textured TMD packet walker | **WIP, 97.347160% fresh direct strict; 93.6% focused listing**. Four packet modes and call/referent set agree; retail has 44 CFG blocks and 28 branches versus 42/26 in the probe. Its FT3/GT3 division paths each retain a separate positive-depth branch before the shared ordering-table tail; C already expresses the check, but the probe folds those two branch blocks. Moving `depth` into each packet-mode case produced the same focused listing and CFG, so that scope-only probe was discarded. The source mechanism for preserving the branches remains unproved. |
| `0x8002e4dc` | Paired textured packet walker | **WIP, 97.278400% fresh direct strict; 92.0% focused listing**. The same 44/42 block and 28/26 branch gap persists; external calls and address-referent counts agree. A source-only typed header-view probe gave the same listing; a `u8` mode-local probe lowered the focused listing to 89.9% without changing CFG. Both were discarded because neither establishes a missing source fact. |
| `0x8002ebe0` | Blended/lit packet walker | **WIP, 95.279450% fresh direct strict; 88.7% focused listing**. Four colored/textured modes and the full-width blend-bit contribution are modeled; external calls and address-referent counts agree. Retail/source CFGs have 26/25 blocks and 17/16 branches. Retail checks signed fixed depth and the `0x2000` ordering-table limit at the packet tail, while the probe computes the equivalent in-range flag at entry and reuses it. Splitting the two C conditions into nested guards moved the check back to the late tail and produced 26/26 blocks, 17/17 branches, and matching successor lists, but left the 104-byte probe frame versus retail's 96 bytes and lowered focused similarity to 80.1%. That syntax change added no independent source fact, so it was discarded; the original source mechanism remains unproved. |
| `0x8002f194` | `render_enqueue_map` | **Exact, 100%**; regression control beside the map clipper. |
| `0x8002f5b0` | Clipped GT3 fan builder | **WIP, 95.833336%**. The clipped-vertex and SDK call set agree; retail keeps a different saved-register assignment and orders one `lhu`/`sh` pair before the depth divide. |
| `0x8002f808` | Alternate prepared TMD renderer | **WIP, 91.176970% fresh direct strict objdiff; 58.9% focused listing**. FT3/FT4 clipping and packet fields are modeled. The retail-backed zero-count guard and postdecrement loop give 51/51 CFG blocks, 35/35 branches, 4/4 return frontiers, and matching known successor lists. Forming the projected-vertex base inside the packet loop removes two extra global address pairs. Placing each normal packet path before its clipping fallback follows the retail branch layout and raises strict similarity from 64.961624% to 91.176970%. The target and source each have 48 function relocation rows in the same type/referent order; all external call counts agree. The first differing control is #6, and retail's 168-byte frame versus the probe's 120-byte frame still needs a proven local-object model. |
| `0x8002ff5c` | Prepared TMD object copier | **Claimed WIP; 51.506750% direct strict objdiff; 12.0% focused listing**. The sole caller passes asset, 16-bit object index, and a 4096-byte output object. The C claim models 14 copy calls, four-child FT4/FT3 subdivision, five or three midpoint vertices, packet counts, and final vertex/normal copies. All 14 external copy-call targets and counts agree. Retail and C have 11/11 CFG blocks, 6/6 branches, the same known successor lists, and 16 relocation rows each. Raw FT4 and FT3 paths copy respectively four and three packet words into a shared scratch record at `sp+48`; subsequent byte reads at offsets 48, 49, 52, 53, 56, and 57 prove the local texture-word view. Retail initializes its midpoint pointer at `sp+64` and later copies vertices from that address; the next stack record begins at `sp+1088`, supporting the 1024-byte, 128-`SVECTOR` workspace. The probe uses the same `sp+48` scratch and `sp+64` midpoint addresses, so retail's 1248-byte frame versus the probe's 1360-byte frame comes from the upper temporary/spill region, whose ownership remains unresolved. The first subdivided FT4 child writes two full words at packet offsets 24 and 28; a shared typed view now expresses both index pairs without an aliasing cast, preserving the adjacent halfword in the second pair. The retail local halfword scratch that feeds those words is not yet modeled. A typed `SVECTOR` aggregate-copy probe for four corner records expanded text 2816→2892 bytes and reduced strict matching to 44.068710%; the retained union view spells the raw two-word vertex copies. A shared `u8` packet-mode local compiled to a byte-identical object and was discarded. The pinned no-scheduler profile was a temporary 12.2% focused regression and was discarded. |
| `0x80030c18` | `render_map_cell_object` | **WIP, 96.521736% direct strict objdiff; 98.7% focused listing**. Retail/source CFGs have 11/11 blocks and 6/6 branches; the call set and typed cell/lighting fields agree. Four prologue instructions are ordered differently before `SetRotMatrix`, after which the focused listing is SAME. Moving the object-index read earlier in a temporary source expanded the frame and was discarded. This is an unattributed codegen residue; all three sibling functions in the unit remain strict 100% controls. |
| `0x80030de4` | Two-layer map-cell emitter | **Direct strict objdiff 100%; focused SAME**. Retail and source have 10/10 CFG blocks, two calls to `render_map_cell_object`, and the same 80-column grid stride. Computing each layer's vertical position before its depth position aligns the lower-layer load and x/z instruction schedule. This result is not banked. |
| `0x80030f5c` | Map-cell row traversal | **Exact, 100%**; adjacent regression control. |
| `0x80031024` | Render-model row traversal | **Exact, 100%**; adjacent regression control. |
| `0x800311b0` | Textured screen quad builder | **WIP, 92.148150% fresh direct strict; 57.5% focused listing**. Four callers and O32 byte/halfword stack loads prove its 15-argument signature; CFGs have 8/8 blocks and 5/5 branches. Retail places the packet-code store in a branch delay slot and saves one more register than the probe. |
| `0x80031850` | World-model renderer | **WIP, 94.0% isolated direct strict; 80.5% focused listing**. Retail/source still have 40/40 CFG blocks, 16/16 branches, the same call set and known successor lists. Raw retail computes the typed collision-row pointer separately in both lighting branches before they join; the retained C now does the same instead of joining on the six-bit index. A typed byte-offset local in the null path now forms the map base after both graphics coordinates and player offset, as retail does; all 68 ordered relocation type/referent pairs agree. Direct strict rises from 86.4% and focused listing from 70.8%, with target/source `.text` sizes 1340/1336 bytes. The intermediate branch-only source scored 94.41791% strict, but ordered referents 18–23 differed; retaining the source-backed null-path order gives 94.0% strict. The first remaining divergence is saved-register allocation for the scale pointer and blend argument, followed by map-cell coordinate evaluation order. A whole-function view-position pointer lowered the pre-offset focused listing to 70.6%; a branch-local pointer left it at 70.8%, and typed coordinate locals compiled identically to the branch-only source. These were discarded. |
| `0x80031d8c` | Animated-object renderer | **WIP, 94.654140%**. Seven CFG blocks agree. The caller loads the sixth argument from a byte field and this function forwards it to `0x8002ebe0`, which shifts it into texture blend bits; the source and identity now type it as `s32 blend_mode` instead of a pointer. This type correction leaves the focused listing unchanged; retail retains the value in `s7` while the probe reloads the stack argument. |
| `0x800321d8` | `resource_tmd_queue_read` | **WIP, 98.435900%**. Calls, registry referents, and three CFG blocks agree. Retail forms the fixed arena `0x8009b0a0` with `lui/addiu`; the provisional C literal forms `lui/ori`. A temporary extern probe reproduces the opcode, but the arena symbol's binding and extent remain unproved, so the source literal stays unchanged. |
| `0x8003247c` | Per-frame actor/placed-object resource dispatcher | **Claimed WIP; 90.637980% prior direct strict objdiff and 71.0% current focused listing**. The C models the actor, animated map-object, sound-action, ordinary map-object, effect, and placed-object passes with typed records; retail/source CFGs have 96/96 blocks and 54/54 branches. Explicit common tails for the actor, map-object, effect, and placed-entry loops follow the raw retail increment and decrement paths. The actor visibility and radius checks share a typed position pointer distinct from the draw position returned by `func_8003c10c`, following raw value flow across that call. The actor radius check and ordinary map-object radius check sit at loop tails and branch back to shared render paths. The actor identity and nonidentity branches each prepare their draw arguments before the compiler merges them to one call, matching the retail branch-local setup; actor identity and placed-object rotation zero stores follow retail's z/y/x order. The ordinary visibility and radius paths each establish a typed template-row pointer before the shared draw block. The ordinary draw writes rotation before selecting render mode. Map-object action bodies follow the retail animated/sound/ordinary order, and the sound-outside frame reset follows its raw branch target. The sound-volume path clamps to its configured maximum when the computed distance volume reaches the radius, with the saturation branch before the scaling block. A typed camera-position pointer serves all three sound-distance coordinates. Retail `lbu` calls establish unsigned action and sound-radius arguments; signed `lh` reads establish actor/effect blend fields. Full-width map-layer-mask returns and local visibility masks remove caller-side truncation. A typed union view spells the low/high bytes of the packed spawn sequence without aliasing casts; focused map-object reset and 0x80036944 exact controls remain SAME. The 32-byte identity matrix is strict 100%. Retail/source frames are 768/760 bytes. Retail uses `sp+56..60` for the three rotation halfwords and passes `sp+56` as draw argument four; actor scale comes from `actor+72` in argument five at `sp+16`. No retail access establishes a missing object in the eight-byte gap at `sp+64..71`, so frame-size padding would be unsupported. The effect normal-draw path advances its record before jumping to the count tail, while mode 12 falls through its separate increment; the C now retains both effect draw calls, matching retail's five total `func_80031850` call sites. The first differing CFG successor is still in effect handling at B62, while the source and retail each have 82 relocation rows, including 13 local jumps. |

The fresh isolated `game.tmd_pipeline` object is 97.388570% strict for its
6,372-byte `.text` section and keeps all eight adjacent functions at direct
strict 100%. Its three WIP walkers have ordered relocation
type/referent agreement when each object's *own* symbol offsets are used:
`0x8002ddb4` 44/44, `0x8002e4dc` 44/44, and `0x8002ebe0` 41/41. The two
textured walkers' retail bodies are 32 bytes longer than the probe; each has
two local positive-depth exits after the FT3 and GT3 divides that the probe
folds into a common check. In the lit walker, retail stores the blend word at
`sp+32` before `tmd_get_object`, shifts it after the call, then tests signed
fixed depth at the shared packet tail (`0x8002f118`–`0x8002f130`). The probe
keeps the blend word in a saved register and hoists the equivalent depth-range
flag. The sole source caller passes a signed depth; the raw callee truncation
at the tail does not establish whether the original formal was `s16` or `s32`.
No source change is justified by those codegen differences alone.
An isolated positive-depth spelling probe put the FT3 and GT3 insertion bodies
inside `if (depth > 0)` in both textured walkers, preserving their packet
semantics. The compiled direct strict scores remained exactly 97.347160% and
97.278400%, respectively; GCC still folded the same two branches. The
retained source therefore remains unchanged.

The connected `0x8002d5dc` index converter is now direct strict 100% for its
724-byte `.text` and 116-byte switch `.rodata`; focused `kf try` reports
`SAME`. Its ten `.text` and 29 `.rodata` relocation rows agree in site, type,
and referent order. The original two-pointer source made the compiler move
the input asset from `a0` to `t2`, adding four bytes and shifting all 29
switch-table pointer addends. Reusing the typed primitive pointer for both
packet mutation and packet advance removes that unnecessary live byte
pointer while preserving the eight retail packet-mode field writes. Earlier
count-width and zero-guard probes did not solve this residue; the indirect
switch targets retain their curated candidate status despite the exact
object.

The connected clipped fan `0x8002f5b0` still has a supported packet model and
95.833336% fresh isolated strict score. Its `render_enqueue_map` sibling and
the owned four-byte color datum both remain direct strict 100%. All 23
ordered clipped-fan relocation type/referent pairs agree. Retail stores
all three UV halfwords at packet
offsets 12, 24, and 36 before the three color words; the current C spells
that same order, but the probe schedules the final UV load/store later. A
source-only local holding the third UV before the packet writes moved its load
ahead of the buffer-bound check, reduced the focused listing from 82.6% to
80.7%, and changed the two earlier UV schedules. It was discarded; the
adjacent exact `render_enqueue_map` listing remained `SAME`.

The KF1 GAME and OPEN map enqueue sources independently support the ordinary
FT3/FT4 packet and normal-shading sequence used as a control here: typed TMD
field reads, `NormalClip`, `NormalColorCol`, four or three `DpqColor` calls,
then an ordering-table insertion. The searched KF1 sources do not contain a
4096-byte prepared-object builder, midpoint subdivision, or a
`Clip3FTP`/`Clip4FTP` fallback corresponding to KF2 `0x8002ff5c` and
`0x8002f808`. Their loop syntax is therefore not evidence for those KF2
bodies. Fresh KF2 focused checks leave the builder at 12.0% and the alternate
renderer at 50.5%, with their previously reviewed call and referent sets.
The exact `0x8002f194` enqueue sibling remains `SAME`. Moving the alternate
renderer’s projected-vertex base assignment into its packet loop follows the
retail address setup and KF1 control shape, removing two extra address pairs.
The first source-only probe reached 64.961624% strict and 52.1% focused.
Retail lays out the ordinary FT4 and FT3 draw bodies before their clipping
fallbacks; reversing both C conditions to express those normal bodies first
raised the retained source to 91.176970% strict and 58.9% focused. All 48
relocation type/referent rows now agree in order. The exact sibling and
unchanged clipped-fan listing remain controls, and the owned four-byte color
datum remains direct strict 100%. In the caller,
retail places the runtime address pair before `lbu s0,2(s2)` and
`move s3,a2`, then masks orientation in the `SetRotMatrix` delay slot.
Source-only variants placing the mask or the whole orientation read after
that call each lowered the focused caller listing from 98.7% to 92.3% and
removed that delay-slot mask. Neither was retained; the three exact caller
siblings remain `SAME`.
Moving the packet-header declaration into the same loop compiled to the
identical focused listing, so it did not explain retail's stack spills and
was discarded.
For `0x8002ff5c`, merely widening the UV scratch union to four bytes raised a
temporary probe to 20.4% focused and 54.321472% direct strict, but the added
word member had no source use and would only steer allocation. Computing the
two UV components through that word instead dropped focused similarity to
10.2%. Both probes were discarded; the retained builder remains 12.0%
focused and 51.506750% direct strict.
Retail also reuses one stack word at `sp+1088` for each packed FT4/FT3
vertex-index word, then reads the selected signed halfword. An off-tree
source probe expressed that real value flow with one `u32`/`s16[2]` union
across both packet modes. The pinned compiler folded it back to the
existing direct packet loads: 12.0% focused and 51.506750% direct strict,
byte-identical to the retained source. The retail local's lifetime and
allocation remain unresolved; no scratch variable was kept for score alone.
For the alternate renderer, a loop-local 32-bit mode value derived from the
header's top byte raised focused similarity to 62.2%, but lowered isolated
strict matching from 91.176970% to 90.720680%. Retail's separate mode spill
supports the value's lifetime, not a particular C declaration, so the probe
was discarded. Reusing the clipped fan's `vertex_count` formal as its loop
counter compiled to the identical 82.6% focused listing and was also
discarded.
Moving the builder's output-packet pointer setup after source-object reads
produced only 12.1% focused and 51.455215% strict; delaying just its
28-byte increment likewise gave 12.1% focused and 50.895706% strict. The
retail entry forms the output object pointer early, but neither source order
reproduced its caller-saved register and scratch layout, so both remained
temporary.

Moving the dispatcher's mode-12 effect call below its normal draw call raised
focused listing similarity from 63.1% to 64.5%, but the compiler still merged
the two source calls into one instruction. Direct strict fell from 87.46175%
to 87.18852% and the source lost one relocation row, so that syntax-only probe
was reverted.

Separating the effect loop's normal draw advance from its mode-12 fallthrough
restores retail's fifth `func_80031850` call and raises direct strict matching
to 87.892075%. Branch-local actor draw setup raises it further to 90.815575%.
Placing the special draw after the normal draw matches retail's call order and
removes two extra local jumps: both objects have 82 relocation rows, including
13 local jumps, and five draw calls. The final direct strict score is
90.637980% with the same 96-block, 54-branch CFG. A separate mode-zero
draw-call body produced 97 rather than 96 CFG blocks, so that probe was
reverted; the retained source uses one normal draw body.

An additional `0x8002ebe0` probe gathered the four packet-mode enqueue calls
into one late tail, as the retail jumps suggest. The source remained semantic,
but the combined guard still compiled to 25 rather than 26 CFG blocks and
fell from 95.279450% to 92.630135% direct strict. Splitting the signed
depth and ordering-table bound guards produced the retail's 26/26 blocks and
17/17 branches, but expanded the source text to 1468 bytes against retail's
1460 and yielded 0% direct strict. An unsigned bound variant further expanded
the frame. None proved the original source form, so all were discarded and the
focused TMD object was rebuilt from the retained source.

The three narrow GAME function-identity corrections at `0x8002ff5c`,
`0x80030de4`, and `0x800311b0` now record the retail/caller-supported
signatures. `kf sema --image game addr` parses all three; focused comparisons
of their existing caller/owner units preserve the prior WIP and exact sibling
listings. The `0x8002f808` loop change preserves exact `render_enqueue_map`.
No repository tests, lint, full build, broad match, bank, or commit were run.
