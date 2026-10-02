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
| `0x8002f808` | Alternate prepared TMD renderer | **WIP, 91.176970% fresh direct strict objdiff; 58.9% focused listing**. FT3/FT4 clipping and packet fields are modeled. The retail-backed zero-count guard and postdecrement loop give 51/51 CFG blocks, 35/35 branches, 4/4 return frontiers, and matching known successor lists. Forming the projected-vertex base inside the packet loop removes two extra global address pairs. Placing each normal packet path before its clipping fallback follows the retail branch layout and raises strict similarity from 64.961624% to 91.176970%. The target and source each have 48 function relocation rows in the same type/referent order; all external call counts agree. The first differing control is #6, and retail's 168-byte frame versus the probe's 120-byte frame remains unattributed. |
| `0x8002ff5c` | Prepared TMD object copier | **Claimed WIP; 55.842945% isolated direct strict objdiff; 19.0% focused listing**. The sole caller passes asset, 16-bit object index, and a 4096-byte output object. The C claim models 14 copy calls, four-child FT4/FT3 subdivision, five or three midpoint vertices, packet counts, and final vertex/normal copies. All 14 external copy-call targets and counts agree. Retail and C have 11/11 CFG blocks, 6/6 branches, the same known successor lists, and 16 relocation rows each. Raw FT4 and FT3 paths copy respectively four and three packet words into a shared scratch record at `sp+48`; subsequent byte reads at offsets 48, 49, 52, 53, 56, and 57 prove the local texture-word view. Retail initializes its midpoint pointer at `sp+64` and later copies vertices from that address; the next stack record begins at `sp+1088`, supporting the 1024-byte, 128-`SVECTOR` workspace. The probe uses the same `sp+48` scratch and `sp+64` midpoint addresses, while the current candidate uses a 1232-byte frame against retail's 1248; the remaining upper temporary/spill extent is unresolved. The first subdivided FT4 child writes two full words at packet offsets 24 and 28; a shared typed view now expresses both index pairs without an aliasing cast, preserving the adjacent halfword in the second pair. The retail local halfword scratch that feeds those words is not yet modeled. A typed `SVECTOR` aggregate-copy probe for four corner records expanded text 2816→2892 bytes and reduced strict matching to 44.068710%; the retained union view spells the raw two-word vertex copies. A shared `u8` packet-mode local compiled to a byte-identical object and was discarded. The pinned no-scheduler profile was a temporary 12.2% focused regression and was discarded. |
| `0x80030c18` | `render_map_cell_object` | **WIP, 96.521736% direct strict objdiff; 98.7% focused listing**. Retail/source CFGs have 11/11 blocks and 6/6 branches; the call set and typed cell/lighting fields agree. Four prologue instructions are ordered differently before `SetRotMatrix`, after which the focused listing is SAME. Moving the object-index read earlier in a temporary source expanded the frame and was discarded. This is an unattributed codegen residue; all three sibling functions in the unit remain strict 100% controls. |
| `0x80030de4` | Two-layer map-cell emitter | **Direct strict objdiff 100%; focused SAME**. Retail and source have 10/10 CFG blocks, two calls to `render_map_cell_object`, and the same 80-column grid stride. Computing each layer's vertical position before its depth position aligns the lower-layer load and x/z instruction schedule. This result is not banked. |
| `0x80030f5c` | Map-cell row traversal | **Exact, 100%**; adjacent regression control. |
| `0x80031024` | Render-model row traversal | **Exact, 100%**; adjacent regression control. |
| `0x800311b0` | Textured screen quad builder | **WIP, 92.148150% fresh direct strict; 57.5% focused listing**. Four callers and O32 byte/halfword stack loads prove its 15-argument signature; CFGs have 8/8 blocks and 5/5 branches. Retail places the packet-code store in a branch delay slot and saves one more register than the probe. |
| `0x80031850` | World-model renderer | **WIP, 99.29851% isolated direct strict; 94.5% focused listing**. Retail/source have 40/40 CFG blocks, 16/16 branches, the same call set and known successors. Separate typed collision-row selection preserves both lighting branches. The null path indexes the complete 80-column map grid by Z row then X column, matching retail coordinate evaluation order and the exact player-weapon lighting consumer. Reading the selected `u8` into a scalar after the player layer-byte offset now reproduces the retail `player_state+0x128` pair followed by the `bss_801c7540+4` pair and `lbu 0` in the same instruction order; all 68 relocation type/name rows now agree in order. The earlier pointer-local form reversed those address pairs and used `bss+0` with `lbu 4`. The remaining listing differences begin with saved-register assignment and later independent instruction order, so the source remains WIP. A typed row/cell-pointer probe regressed to 87.3% focused and was discarded. Initializing the selected object index inside the low-clip branch now places its zeroing at retail's branch delay slot, improving the prior 98.92538% strict and 93.8% focused results. |
| `0x80031d8c` | Animated-object renderer | **WIP, 94.654140%**. Seven CFG blocks agree. The caller loads the sixth argument from a byte field and this function forwards it to `0x8002ebe0`, which shifts it into texture blend bits; the source and identity now type it as `s32 blend_mode` instead of a pointer. This type correction leaves the focused listing unchanged; retail retains the value in `s7` while the probe reloads the stack argument. |
| `0x800321d8` | `resource_tmd_queue_read` | **WIP, 98.435900%**. Calls, registry referents, and three CFG blocks agree. Retail forms the fixed arena `0x8009b0a0` with `lui/addiu`; the provisional C literal forms `lui/ori`. A temporary extern probe reproduces the opcode, but the arena symbol's binding and extent remain unproved, so the source literal stays unchanged. |
| `0x8003247c` | Per-frame actor/placed-object resource dispatcher | **Claimed WIP; 92.00273% current direct strict objdiff and 71.0% focused listing**. The C models the actor, animated map-object, sound-action, ordinary map-object, effect, and placed-object passes with typed records; retail/source CFGs have 96/96 blocks and 54/54 branches. Explicit common tails for the actor, map-object, effect, and placed-entry loops follow the raw retail increment and decrement paths. The actor visibility and radius checks share a typed position pointer distinct from the draw position returned by `func_8003c10c`, following raw value flow across that call. The actor radius check and ordinary map-object radius check sit at loop tails and branch back to shared render paths. The actor identity and nonidentity branches each prepare their draw arguments before the compiler merges them to one call, matching the retail branch-local setup; actor identity and placed-object rotation zero stores follow retail's z/y/x order. The ordinary visibility and radius paths each establish a typed template-row pointer before the shared draw block. The ordinary draw writes rotation before selecting render mode. Map-object action bodies follow the retail animated/sound/ordinary order, and the sound-outside frame reset follows its raw branch target. The sound-volume path clamps to its configured maximum when the computed distance volume reaches the radius, with the saturation branch before the scaling block. A typed camera-position pointer serves all three sound-distance coordinates. Retail `lbu` calls establish unsigned action and sound-radius arguments; signed `lh` reads establish actor/effect blend fields. Full-width map-layer-mask returns and local visibility masks remove caller-side truncation. A typed union view spells the low/high bytes of the packed spawn sequence without aliasing casts; focused map-object reset and 0x80036944 exact controls remain SAME. The 32-byte identity matrix is strict 100%. Retail/source frames are 768/760 bytes. Retail uses `sp+56..60` for the three rotation halfwords and passes `sp+56` as draw argument four; actor scale comes from `actor+72` in argument five at `sp+16`. No retail access establishes a missing object in the eight-byte gap at `sp+64..71`, so frame-size padding would be unsupported. The effect normal-draw path advances its record before jumping to the count tail, while mode 12 falls through its separate increment; the C now retains both effect draw calls, matching retail's five total `func_80031850` call sites. The first reported effect-loop CFG successor reaches the same next-record increment in both objects despite different block numbering; each has 82 relocation rows, including 13 local jumps. |

A fresh `0x8002f808` stack audit found the same ten saved registers (`ra` and
`s0`–`s8`) in retail and the probe. Retail passes the extra `Clip4FTP`
arguments at `sp+16..32`, forms the shade pointer at `sp+80`, and uses word
spills at `sp+88`, `+96`, `+104`, `+112`, and `+120`. No decoded instruction
loads, stores, or forms an address into `sp+36..79`. The 48-byte frame delta
therefore does not establish a missing local object's type or extent; no
padding or artificial local was added. The first non-frame listing difference
is a retail primitive-count spill/reload before its zero guard versus the
probe's direct register branch.

The first FT4 child in retail confirms the retained packed-index values:
`0x800303ec` and `0x800303f8` write `ab`/`ac` halfwords to one stack word,
which `0x80030408` copies to packet +24. `0x8003041c` replaces only its low
halfword with `ad`, and `0x80030430` copies the word to packet +28 with `ac`
still in the high halfword. The source's `vertex1_vertex2` and
`vertex3_pad2` assignments express precisely those bytes. A fresh focused
build was 12.0% listing DIFF; the then-current 1248/1360-byte frame and
packet-local schedule were unexplained. Both retail and probe form the
midpoint workspace at `sp+64`; retail stores FT4 corner words at `sp+16..47`
and computes signed halfword midpoint averages with eight-byte output strides.
The 112-byte frame gap therefore did not support shrinking the 128-entry
midpoint array. That pass retained no source edit.

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
10.2%. Both probes were discarded; at that checkpoint the retained builder was
12.0% focused and 51.506750% direct strict.
Retail also reuses one stack word at `sp+1088` for each packed FT4/FT3
vertex-index word, then reads the selected signed halfword. An off-tree
source probe expressed that real value flow with one `u32`/`s16[2]` union
across both packet modes. The pinned compiler folded it back to the
existing direct packet loads: 12.0% focused and 51.506750% direct strict,
byte-identical to the retained source. The retail local's lifetime and
allocation remain unresolved; no scratch variable was kept for score alone.
A narrower off-tree `u32`/`u16[2]` pair used only for the first FT4 child's
two packed-index words also failed: the candidate grew from 2,816 to 2,952
text bytes and direct strict alignment fell to 2.873620%. The compiler
allocated separate halfword spills rather than retail's reused four-byte
scratch word, so this view was likewise discarded.
A second off-tree union probe also used the same typed word for child index
halfword/byte emission, following its apparent retail reuse at `sp+1088`.
The compiler still did not reproduce that stack lifetime: 12.5% focused and
45.663803% direct strict. It was discarded, leaving the existing packet
semantics and WIP allocation claim unchanged.
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

A later off-tree control moved the full-width blend-bit calculation before
`tmd_get_object`. Isolated strict text rose from 95.279450% to 96.08767%,
but the probe shifted `$a1` before the call and stored it in the call delay
slot. Retail saves the unshifted argument before the call, then reloads,
shifts, and stores it afterward. The probe also retained the premature depth
range calculation and 104-byte frame against retail's 96 bytes. It was
discarded; the higher score did not establish a source correction.

The `0x8002ddb4` GT3 ordering-table guard was also tested with the explicit
break form retained for FT3. Direct strict text rose from 97.434494% to
97.521835%, but the compiled CFG changed from 44/44 blocks and 28/28
branches to 44/45 and 28/30. Its new local bound test and jump duplicate
retail's shared-tail work, so this single-arm control was discarded.

A fresh raw review confirms that all four colored/textured cases converge on
`0x8002f114`, which stores the packet code byte at primitive offset `+7` before
the signed depth and `0x2000` bound checks. A second shared-tail C probe used
that same byte offset but still yielded 25/26 blocks and 16/17 branches:
91.934250% isolated strict, 89.2% focused, and 43 function relocations versus
retail's 41 (two extra local jumps). All eight exact functions in the unit
remained 100%. The probe was reverted; the packet-field and shared-tail facts
are retained as retail evidence rather than a source-structure assertion.

The three narrow GAME function-identity corrections at `0x8002ff5c`,
`0x80030de4`, and `0x800311b0` now record the retail/caller-supported
signatures. `kf sema --image game addr` parses all three; focused comparisons
of their existing caller/owner units preserve the prior WIP and exact sibling
listings. The `0x8002f808` loop change preserves exact `render_enqueue_map`.
No repository tests, lint, full build, broad match, bank, or commit were run.

## Prepared packet UV component view

The copied FT4/FT3 texture words at stack `+48..+63` contain packed, separate
8-bit U and V components. After the FT4 `resource_copy_words` at body `+0x35c`,
retail reads U and V with `lbu` from `sp+48/49`, `+52/53`, `+56/57`, and
`+60/61` before averaging the components. The old local `u16` view produced
`lhu` plus shifts. A two-byte U/V struct now names those fields inside the
same 16-byte local packet view, preserving the four-child FT4 and FT3 paths,
all 14 direct copy calls, and the object extent.

The FT3 path repeats the byte pattern after its copy call at body `+0x8b4`:
retail loads `sp+48/49`, `+52/53`, and `+56/57` at `+0x8c8..+0x91c`.

The first byte-field view moved strict text from **51.50675%** to
**50.339878%** and candidate `lbu`/`lhu` counts from 10/19 to 18/13. The
unused halfword member of the midpoint UV scratch union was then removed:
the scratch is the same two-byte U/V struct as the copied packet fields.
That intermediate focused object reached **52.31411%** strict. Its candidate
text is 2,724 bytes against retail's 3,260; the objdiff helper reports the retail
function size, not the candidate extent. The frame narrows from 1,360 to
1,240 bytes, eight bytes below retail's 1,248. Candidate `lbu`/`lhu` counts move
to 55/2, versus retail's 53/0. All 14 direct `resource_copy_words` calls and
16 ordered relocation rows remain present. This is a type/lifetime
correction supported by the raw byte accesses; register and scratch-word
scheduling still leave the function WIP.

An off-tree index-word union for packed vertex indices compiled
byte-identically to the original halfword-view source and was discarded.
An isolated GCC 2.6.0 profile control of the first byte-field view fell to
46.65767% strict, so the unit profile remains unchanged. Verification used
only focused object compiles, isolated strict comparisons, raw disassembly,
and `git diff --check`.

The remaining candidate `lhu` instructions read the low half of the source
object's 32-bit vertex count at the FT4/FT3 midpoint-index setup, whereas
retail loads the word. An off-tree `u32` index-local trial removed both
`lhu` instructions but let high bits contaminate the packed first-child
index words; an explicit 16-bit mask restored the required packet semantics.
That masked trial was **51.305523%** strict versus the then-current
**52.31411%**, and narrowed the frame another eight bytes away from retail.
The raw word load alone does not prove the original index-local width, so the
retained 16-bit packet indices remain unchanged.

The UV component inputs are unsigned bytes, and retail averages them with
logical `srl` rather than signed `sra`. Casting both inputs to `u32` before
the two component sums makes that width explicit. A fresh focused comparison
improves strict text from **52.31411%** to **53.768097%** with the same
2,724-byte candidate text, 1,240-byte frame, 14 copy calls, and 16 relocation
rows. Candidate `srl`/`sra` counts move from 10/40 to 26/24; retail is 24/24.
The two extra logical shifts and remaining scratch spills keep the function
WIP. This source change preserves packet values and was retained.

Retail reloads the source object's vertex offset for each of the four FT4
corners and three FT3 corners, then adds that offset to the asset base and the
packet's signed vertex index. The earlier C cached the vertex base before the
packet loop, which removed those seven raw `lw` operations. Reading the typed
`source->vertex_offset` at each corner and again for the final original-vertex
copy preserves the same addresses without a long-lived alias. An off-tree
probe rose from **53.768097%** to **55.263805%** strict; the retained focused
object is byte-identical to that probe. Its text is 2,796 bytes versus the
3,260-byte retail body, with the same 1,240-byte candidate frame, 14 copy
calls, and 16 relocation rows. The remaining code and frame differences are
unattributed; no artificial local or profile change was added.
An off-tree full-width midpoint-index probe kept the packet halfword truncation
explicit and used the same direct vertex addresses, but fell to **54.446625%**
strict. Retail's word load of `vertex_count` still does not establish the
original index-local width, so the 16-bit packet-index locals remain.

### Reused packet-index scratch control

A fresh isolated strict comparison of GAME `0x8002ff5c` confirms the retained
**55.263805%** text result; a focused quick build now reports **19.0%**
listing. Retail and candidate still have 11/11 known CFG blocks, 6/6
branches, all 14 ordered `resource_copy_words` calls, and 16 ordered
relocations. The retail frame is 1,248 bytes against the candidate's 1,240.

The raw four-byte stack slot at `sp+1088..1091` is reused across FT4 and FT3
child packets. For the first FT4 child, retail stores generated halfword
indices to `sp+1088` and `sp+1090`, loads the combined word, and writes it
to packet offset +24. It then replaces only the low halfword, loads the
combined word again, and writes offset +28, preserving the prior high
halfword. Later children store halfword indices to the same slots and read
their two bytes separately for packet fields. This proves the first child's
packed-word value flow; stack-slot reuse does not establish whether the
original C declared one shared object.

An off-tree typed four-byte union reused for both the word stores and every
later byte-index write preserved the packet values, but GCC folded the view
into register operations. Its frame shrank to 1,224 bytes and strict text
fell to **46.90061%**. The trial was discarded. No padding, volatile carrier,
or forced alias was introduced into tracked source. The current typed packet
fields and midpoint workspace remain unchanged; no new exact claim follows.

### Textured packet depth-guard recheck

The current `game.tmd_pipeline` source supersedes the older 44/42-block
snapshot above. A fresh isolated strict comparison puts GAME `0x8002ddb4`
at **97.434494%** and `0x8002e4dc` at **97.38307%**; both now have 44/44
known CFG blocks and 28/28 branches. The adjacent eight exact functions
remain 100%, and `0x8002ebe0` remains 95.27945% with 26/25 blocks and
17/16 branches. The first textured walker still maps its FT3 and GT3
positive-depth paths differently from retail: retail's two `blez` exits
precede jumps to one shared ordering-table bound check, while the probe
folds one exit and emits a local bound check in the other arm.

Two off-tree source-equivalent controls on `0x8002ddb4` were rejected.
Putting all four successful packet modes through one typed primitive-pointer
tail lowered strict text to **96.8428%**. Swapping the FT3 and GT3 bound-guard
spellings raised strict text to **97.69651%** and kept 44/44 blocks,
28/28 branches, and all eight exact siblings, but it merely moved the
missing `blez` from GT3 to FT3 and kept a duplicate local bound check.
The more uniform FT3 conditional form alone scored **97.34716%**. None
reproduced the retail pair of local exits or proved an original source
asymmetry, so `tmd_pipeline.c` is unchanged.

An independent current-source audit of GAME `0x8002f808` in `render_map.c`
reported **92.038376%** isolated strict, 51/51 CFG blocks, and 35/35
branches. Retail FT4 at `0x8002f930` maps `s4/s3/s2/s5` to the four
vertices and its `dy01/dy13/dy32/dy20/dy12` and
`dx01/dx13/dx32/dx20/dx12` difference registers follow the present
short-circuit check order. That check has no supported source correction;
`render_map.c` was not edited in this lane.

### Current graphics and render WIP screen

Fresh isolated strict comparisons and focused quick builds cover ten remaining
GAME render functions. The prior raw call, field, and referent audits above
remain the evidence for their source models; this screen found no new source
fact that warrants changing a function. The CFG counts below are retail/probe.

| Function | Strict text | Focused CFG / branches | Bounded verdict |
| --- | ---: | --- | --- |
| `0x8002ddb4` | 97.434494% | 44/44; 28/28 | FT3/GT3 depth-guard layout still differs; source-equivalent guard swaps do not reproduce both retail exits. |
| `0x8002e4dc` | 97.38307% | 44/44; 28/28 | Packet modes, calls, and owned fields remain aligned; no new control fact. |
| `0x8002ebe0` | 95.27945% | 26/25; 17/16 | The late depth/bound-check split is still the one live structural gap; the prior nested-guard trial was not source-backed. |
| `0x8002f5b0` | 95.833336% | 13/13; 7/7 | Clipped GT3 fan has the same calls and controls; saved-register assignment and depth-divide scheduling differ. |
| `0x8002f808` | 92.038376% | 51/51; 35/35 | Raw FT4 vertex and edge order agrees; 168/120-byte frame and instruction scheduling remain. |
| `0x80030c18` | 96.521736% | 11/11; 6/6 | Independent prologue loads around `SetRotMatrix` differ; three siblings and 540-byte initialized table are exact. |
| `0x800311b0` | 92.14815% | 8/8; 5/5 | The authentic `setPolyFT4` path has the same control and arguments; saved-register and packet-store scheduling remain. |
| `0x80031850` | 99.29851% | 40/40; 16/16 | Typed cell/lighting paths and ordered referents agree; remaining register/instruction order is unattributed. |
| `0x80031d8c` | 94.65414% | 7/7; 2/2 | Sixth-argument blend width and calls agree; retail saves it where the probe reloads it. |
| `0x8003247c` | 92.18579% | 96/96; 54/54 | Actor/effect/map passes retain the reviewed calls and fields; 768/760-byte frame and register lifetimes remain. |

Current-source controls `game.animation_sparse_vertices` (three functions,
including `0x80033d3c`) and `game.render_frame` (two functions, including
`0x800335a0`) are strict 100% with focused `SAME` listings. They were already
exact in the generated report; their older WIP prose is stale. The exact
`render_enqueue_map`, eight TMD-pipeline siblings, and three map-cell siblings
also remain 100%. No C change or new exact claim resulted.

### Prepared subdivider header and object-base control

A fresh GAME `0x8002ff5c` raw/focused pass keeps the 3,260-byte prepared
subdivider at **55.842945% strict** after the primitive-count correction. Its sole caller, all 14 ordered
`resource_copy_words` calls, and the two internal target branches remain
identified. Retail's first packet-mode test loads a word, stores it to a
stack header at `sp+1088`, then reads the mode byte at `sp+1091`; the probe
retains the word in a register and extracts its high byte. The source already
uses the typed `KfTmdPacketHeader` union. An off-tree whole-union copy in
place of its word assignment left the focused listing unchanged.

Retail computes an object index from the header-adjusted asset base. An
off-tree spelling that indexed the same typed `KfTmdObject` array from the
existing `base` pointer moved that `addiu asset,+12` before the indexed
addition, but isolated strict text fell to **53.845398%** and the first
packet-mode difference remained. Both trials were discarded. The current
probe reserves 1,232 stack bytes against retail's 1,248, with the same
1,024-byte midpoint workspace at `sp+64`; no additional complete local
object is proved by the frame gap.

The first FT4 corner path provides a narrower width clue. Retail loads the
packed normal/vertex word at packet `+20` into the same stack scratch and
reads its high signed halfword for vertex zero; packet `+24` supplies the
next two signed indices, and packet `+28` supplies vertex three. An off-tree
typed four-byte union reused one packed word across these four corner reads.
It produced the retail 1,248-byte frame, but the pinned compiler still
selected direct `lh` and `sra` reads from the packet instead of retail's
`lw`/scratch-store/`lh` sequence. Isolated strict text fell to **51.109203%**;
the tracked source retains the proven halfword values without a synthetic
memory carrier.

An isolated GCC 2.5.7 `-O1` control on unchanged source expanded the body
from 2,788 to 2,980 bytes but lowered strict text from **55.263805%** to
**44.766872%**. The larger body alone does not explain retail's scratch
accesses or justify a unit profile change.

The output packet starts immediately after the 28-byte prepared-object record.
An off-tree typed `target + 1` expression for that same pointer produced the
identical focused listing; it did not move the pointer calculation toward the
retail entry schedule. The existing source keeps the record's own
`primitive_offset` as the packet-base expression because that field also
describes the serialized object layout.

Retail separately loads `source->primitive_count` to initialize the prepared
object and to set up the packet loop. The former C assignment reused one
cached value, leaving only one source load. Keeping the two reads in their
retail order raises isolated strict text from **55.263805%** to
**55.842945%**. The tracked source and off-tree trial produce identical
focused listings; all 14 direct calls and 16 ordered relocation kinds and
targets remain unchanged. The compiler still spills and reloads the object
pointer between the reads, so this correction does not explain the remaining
entry schedule or 16-byte frame gap.

An off-tree typed packet-pointer mode read changed the candidate from a
register shift to `lbu 3(source_packet)`, while retail reads the mode from
its copied stack header at `sp+1091`. Strict text fell to **55.575460%**;
the retained source keeps the local header value.
