# GAME render-frame and notification continuation

This 25-function GAME campaign follows confirmed calls from the frame renderer
through map-cell geometry, sliding panels, resource updates, and notification
effects. Every row was checked against retail disassembly and CFG, incoming
and outgoing calls, string and data references, relocation evidence, adjacent
functions, and the current object report. KF1's
`src/game/geometry_render.c` provides the notification-phase counterpart;
`src/game/render_enqueuers.c` helps identify the packet-renderer family.
These structural similarities do not establish original KF2 TU boundaries.

| GAME address | Retail behavior / evidence | Verdict |
| --- | --- | --- |
| `0x80031024` | model matrix and TMD render setup; called by `0x800335a0` | unclaimed; model-array ownership WIP |
| `0x800312f4` | sliding textured panel from player lower collision margin | **new exact, 144/144 bytes** |
| `0x80031384` | paired panel from player upper collision margin | **new exact, 144/144 bytes** |
| `0x800314d4` | sets shared graphics color bytes | existing exact |
| `0x80031634` | draws scaled collision-channel primitives | existing exact |
| `0x800316c8` | selects model, GTE lighting/fog and TMD projection | unclaimed; model and render-grid fields WIP |
| `0x80031850` | matrix setup and object model draw; called by map traversal | unclaimed; 0x53c-byte renderer |
| `0x80031d8c` | lit model setup and TMD packet draw | unclaimed; 0x214-byte renderer |
| `0x80031fa0` | resource registry lookup | existing exact |
| `0x80032008` | TMD read completion | existing exact |
| `0x80032040` | map-cell layer-mask lookup | existing exact |
| `0x800320b0` | radius-aware layer mask | WIP, 73.95918%; map-grid owner |
| `0x80032174` | tests map-cell visibility | WIP, 93.6%; map-grid owner |
| `0x800321d8` | queues TMD read | WIP, 98.4359%; resource arena boundary |
| `0x80032274` | updates VAB resource range | WIP, 90.933334%; audio/resource owner |
| `0x80032364` | updates TMD resource range | existing exact |
| `0x8003247c` | traverses map objects and calls the geometry helpers | unclaimed; 0xb70-byte frame renderer |
| `0x80032fec` | draws one notification quad | existing exact |
| `0x80033140` | draws active notification quads | existing exact |
| `0x800331d0` | enqueues a notification and optional payload | existing exact |
| `0x80033274` | sets a notification digit's texture V | existing exact |
| `0x80033284` | advances notification phase and dequeues groups | **new exact, 768/768 bytes** |
| `0x80033584` | flips the display-buffer index after the notification updater | exact, 28/28 bytes; no known direct caller |
| `0x800335a0` | orchestrates view, map, panel, and notification rendering | exact, 1,012/1,012 bytes after target refresh |
| `0x80033994` | renders menu model preview | existing exact |
| `0x800339fc` | loads TMD archive through resource registry | existing exact |

The two panel wrappers in `graphics_sliding_panels.c` share the textured-quad
callee at `0x800311b0`. Their loads from player-state offsets `+0x120` and
`+0x124` are signed vertical collision margins: retail `0x80023384` computes
them from collision-cache bounds and camera height. The two new HI16/LO16
relocation pairs target these player-state fields, and the strict report is
288/288 bytes. The cache's complete owner is still being recovered, so the
source retains the existing field spellings.

`notify_enqueue.c` now owns the contiguous `0x800331d0`–`0x80033584` run.
The new phase function reuses the initialized seven-quad table and the
`KfNotificationQuad` digit view. KF1's notification update has the same
queue-group dequeue rule: consume equal IDs together, except the numeric
payload ID, then reset the phase. Retail branches and byte stores support
the four phases: prepare, brighten, hold, and darken. The strict report
matches all three functions (948/948 code bytes) and the owned 126-byte quad
table. No separate interior global was introduced.

The five unclaimed functions above are larger render paths with direct calls
and shared graphics data but incomplete object/model ownership. In particular,
the `0x8003247c` traversal feeds `0x80031850` and `0x80031d8c`; the
`0x800335a0` frame driver reaches the exact panel and notification helpers.
Their relationships support this campaign, not a claimed shared source unit.
The four WIP functions retain their existing owners and percentages; none was
changed to protect a score.

Focused `kf try` found the two panel wrappers and the three-function
notification unit identical. The subsequent strict `kf match` report marks
them all 100%, as well as the consolidated eight-function `tmd_pipeline.c`
unit (1,284/1,284 bytes). The command still exits on GAME-wide
known-reference closure; 183/183 target relinks and all nine source-owned
data units validate. Repository tests and final full build belong to the
campaign coordinator.

## Ten-function map and frame-render continuation

A later exactly-ten pass followed the current frame driver through collision
mask construction, TMD packet drawing, world and animated model rendering,
and map placement. Each retail body, CFG, incoming and outgoing references,
strings, adjacent claims, and current match was inspected. None has a
vendored-library attribution. The seven unclaimed functions were left
unclaimed because their packet, table, or callback owner is incomplete.
The three existing C bodies were left unchanged because the focused
differences did not establish a different source-level behavior.

| GAME address | Final verdict | Evidence or first unresolved boundary |
| --- | --- | --- |
| `0x8002c670` | WIP, unclaimed | Eleven-block mask driver calls the exact grid sampler, four segment rasterizations, the row fill, and eight pairs of neighbor-mask helpers. Its interpolated seven-pair table at `0x80067874` still lacks a source owner. |
| `0x8002ddb4` | WIP, unclaimed | Forty-four-block TMD packet builder has four `NormalClip` paths and a final `AddPrim`; packet fields and ownership are incomplete. |
| `0x8002e4dc` | WIP, unclaimed | Forty-four-block sibling uses the same four `NormalClip` and color-depth paths; its packet model shares the open `2ddb4` boundary. |
| `0x8002ebe0` | WIP, unclaimed | Twenty-six-block animated packet builder uses four `NormalClip`, six color calls, and `AddPrim`; the input record and packet fields are not fully typed. |
| `0x8002ff5c` | WIP, unclaimed | Eleven-block prepared-TMD builder makes fourteen `resource_copy_words` calls; the compact packet layout is not recovered. |
| `0x800311b0` | WIP, 92.148150% | Fifteen-argument `POLY_FT4` helper has 8/8 blocks, 5/5 branches, and correct `AddPrim` call; retail saves one more argument register and schedules mode/texture stores differently. |
| `0x80031850` | WIP, 84.650750% | World-model renderer has 40/40 blocks and 16/16 branches, with the correct GTE/TMD call set; address and saved-argument scheduling differ. |
| `0x80031d8c` | WIP, 94.654140% | Lit animated-model renderer has 7/7 blocks and 2/2 branches; retail saves the late render-data argument in `s7`, while the probe reloads it from the caller stack. |
| `0x8003247c` | WIP, unclaimed | Ninety-six-block placed-object traversal calls `31850` five times and `31d8c` once; resource, map-mask, and model-record views remain incomplete. |
| `0x80035894` | WIP, unclaimed | Sixty-four-block map placement controller calls occupancy and pattern helpers and has unresolved indirect call/jump control; the pattern band at `0x8006787c` remains candidate-owned. |

All ten have no string references. The existing exact panel, map-mask,
notification, and frame-driver listings were preserved. No source, data,
identity, or relocation claim was added from a candidate table or an
instruction-order residue.

## Display-buffer leaf at 0x80033584

The 0x1c-byte gap between the exact notification updater and the frame driver
is a return-delimited leaf, not padding. Retail loads the graphics runtime's
display-buffer byte at `0x8017d140`, compares it with zero, and stores the
result in the return delay slot. The HI16/LO16 pair uses the signed low half
`0xd140`; no direct caller or string reference is known. Its source claim in
`render_frame.c` preserves the existing typed field and is contiguous with the
frame-driver claim. This grouping establishes a valid unit run, not an
original TU boundary.

After an image-specific target refresh, a focused build reported both
functions identical. Isolated objdiff gave the two-function unit 1,040/1,040
`.text` bytes and 4/4 `.data` bytes, with the leaf 28/28 and frame driver
1,012/1,012. Raw section bytes and ordered `readelf -r` relocations match the
carved target exactly. The earlier frame-driver WIP score was stale under this
target; no frame-driver C was changed to obtain this verdict.

## Floor-item constructor ABI check

The adjacent floor-item unit has two identical focused controls,
`0x8002ce2c` and `0x8002cf40`, while constructor `0x8002ce68` remains
**55.7% focused WIP** (65.85185% in the older strict report's different
metric). Its five proven call sites are all in `game_main_loop` and pass seven
arguments. Retail uses a
40-byte frame, keeps the first four arguments in `s3`, `s4`, `s1`, and `s2`,
then reads the fifth stack slot both as `lbu` for `item->kind` and as `lw`
for the `kind == 1` test. It reads width as a word and height as an unsigned
halfword only on that branch, before `memory_allocate`; its `StoreImage` and
`DrawSync` calls agree with C. The current probe saves all three stack
arguments into extra saved registers at entry and allocates 56 bytes. A
controlled GCC 2.6.0 compile of the same source also chose a 56-byte frame,
so merely switching that compiler does not explain the retail ABI schedule.
No source change was retained without evidence for a different signature or
evaluation order.

The resource unit's nearby `map_cell_visible` at `0x80032174` is also WIP:
retail and source agree on the single caller at `0x800327b4`, both render-grid
referents, and the four return paths. Its current focused listing is 37.9%
similar because the probe uses `v0` for the position arithmetic and moves a
temporary result into `v0` in the return slot, whereas retail calculates the
fallback predicate directly in `v0` and returns with a `nop` slot. The older
strict report gives 93.6%. A source-only early-return spelling lowered the
focused comparison to 26.7% and introduced an extra jump; it was discarded.
