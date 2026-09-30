# GAME event, pose, and notification stream

This 25-function batch follows the scene controllers' confirmed calls to the
pose projection and interpolation helpers, their notification counter and
collision-channel calls, and the shared event-state arena. Verdicts are per
function. The exact rows below are strict 100% source matches; the unclaimed
controllers remain WIP even where their callees are exact. Earlier campaign
notes that list `0x80045f20` as WIP predate its typed reconstruction.

| GAME address | Verdict | Evidence or remaining issue |
| --- | --- | --- |
| `0x80045cc0` | Exact | Resets the 128-slot effect pool. |
| `0x80045cf0` | Exact | Copies the 64 magic records into effect state. |
| `0x80045d1c` | Exact | Sweeps live effect slots and dispatches updates. |
| `0x80045e18` | Exact | Plays fixed sound `0x40`. |
| `0x80045e3c` | Exact | Plays a requested sound at volume 100. |
| `0x80045e5c` | WIP | Trigonometric terrain probe reads a collision-result pointer at `0x801d8d44`; its enclosing BSS owner overlaps an unresolved equipment view. |
| `0x80045f20` | **New exact** | Eight-argument camera-relative pose projection, 180/180 code bytes and four reviewed referents. |
| `0x80045fd4` | Exact | Optional position and angle interpolation; preserved after the contiguous pose merge. |
| `0x800460a0` | WIP | Animation phase helper has matching call graph but interchanged saved registers for step and half-step. |
| `0x80046144` | Exact | Finds an `f2` marker in a candidate byte stream. |
| `0x800461a0` | WIP, 99.12676% strict | Typed actor-group and event-state stream scan has 14/14 CFG blocks and 5/5 branches; only two record-cursor registers remain exchanged. |
| `0x800462bc` | WIP | Script-type dispatcher has a jump table and incomplete event-record ownership. |
| `0x80046700` | Exact | Spawns a map object for an event slot. |
| `0x8004678c` | WIP | Scene controller has two indirect transfers and incomplete event-object extent. |
| `0x800473e0` | Exact | Decrements a nonzero event-counter byte. |
| `0x80047434` | Exact | Increments a counter through 99 and enqueues a notification. |
| `0x800474c4` | Exact | Seven-argument collision-channel transition. |
| `0x800475d8` | WIP | Scene interaction calls the pose projector, channel transition, and notification path; record owners remain open. |
| `0x80047c98` | WIP | Event dispatcher calls the terrain probe, notification queue, and `0x80034e10` image transition; indirect/state ownership remains open. |
| `0x800482f8` | Exact | Clears and initializes typed event-state control, sentinels, and arena regions; 100% strict. |
| `0x800483a8` | Exact | Invokes the active callback with a zero argument. |
| `0x800483d8` | Exact | Saves event-arena offsets through the typed pointer table; 100% strict. |
| `0x80048428` | Exact | Rebases event-arena links by a signed delta. |
| `0x80048498` | Exact | Restores the typed event-arena pointer table; 100% strict. |
| `0x800484e4` | Exact | Rebases event-arena links for the reverse traversal. |

The five callers of `0x80045f20` establish its eight O32 arguments: three
local coordinates, pitch, yaw, vertical and depth offsets, and a `VECTOR *`
result. The retail body makes an `SVECTOR`, negates pitch in a six-byte angle
record, calls `vector_rotate_yxz`, then adds the player's camera position with
the 1600-unit vertical adjustment. A second source expression matched the
retail register schedule exactly. Its three camera-position HI16/LO16 pairs
and the direct call were reviewed against decoded instructions. The new
function and adjacent `0x80045fd4` are now one contiguous unit, with both
listings identical and `0x80045f20` reported at strict 100%.

`0x800461a0` is a WIP reconstruction in the contiguous marker-scanner unit.
Retail selects the first target-candidate pointer in the actor's group, scans
four-byte `f1`/`fe` records, compares one script byte against the event-state
control region, and returns a pointer into the candidate byte stream. The
actor-group, event-state, helper-call, and four internal jump referents were
decoded and curated. The current C expresses those observed fields and returns
with the retail 14-block, five-branch control shape; only two cursor-register
assignments remain exchanged. The preceding `0x80046144` remains identical.

The menu preview at `0x8002083c` still has a 64-byte stack-frame extent gap.
JP and US retail both use a 224-byte frame; EU retail also has a 224-byte
frame. This regional agreement constrains the missing source aggregate but
does not identify it, so no padding was inserted. The `0x80034e10` image
transition remains WIP at 99.114586%: JP and US use the same first
buffer-boundary register, while EU uses the other register seen in the current
probe. That regional codegen difference does not prove an original source
expression or compiler attribution.

The shared TMD packet header was kept layout-identical while making its
prepared payload and packed GT3/GT4 views parseable by the checked-layout
inventory. The header parser reads a 4096-byte prepared object, 40-byte GT3,
52-byte GT4, and 18-byte notification row. The exact `render_enqueue_map`
listing remains identical. No repository tests, banking, or commit were run
for this batch. A later full `kf match` is required after concurrent source
merges settle; the last global exit was blocked by a transient stale Ninja
source dependency and the existing known-reference data closure.
