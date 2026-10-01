# GAME TMD/display continuation

This 24-function GAME survey follows the menu model preview through display
state, TMD registration, projection, and packet rendering. Each address was
checked against retail disassembly and CFG, incoming and outgoing calls,
strings, data references, relocations, adjacent functions, and current match
state. KF1's `src/lib/tmd.inc`, `src/lib/tmd_transform.inc`, and
`src/game/render_enqueuers.c` provide structural counterparts, but differences
in KF2 packet modes, interfaces, and object ownership were kept explicit.

| GAME address | Retail behavior / evidence | Verdict |
| --- | --- | --- |
| `0x8002d0a4` | sets fog near distance in graphics runtime | existing exact |
| `0x8002d0d4` | initializes display environments | existing exact |
| `0x8002d248` | resets display state | existing exact |
| `0x8002d32c` | begins frame and selects draw environment | existing exact |
| `0x8002d3c4` | presents frame | existing exact |
| `0x8002d458` | selects active TMD | existing exact |
| `0x8002d484` | returns one TMD object record | existing exact |
| `0x8002d4a8` | installs current TMD vertex pointer | existing exact |
| `0x8002d4b8` | selects an object's vertex array | existing exact |
| `0x8002d4f4` | updates view position/rotation, map cells, and two view matrices | **new exact, 232/232 bytes** |
| `0x8002d5dc` | scales primitive vertex/normal indices through packet-mode switch | unclaimed; 29-entry jump-table/RODATA ownership WIP |
| `0x8002d8f0` | replaces a TMD slot pointer | existing exact |
| `0x8002d910` | empty retail slot-release body | existing exact |
| `0x8002d918` | projects vertices with fog-dependent depth output | **new exact, 380/380 bytes** |
| `0x8002da94` | projects vertices and marks invalid GTE results | **new exact, 324/324 bytes** |
| `0x8002dbd8` | transforms vertices without projection | existing exact |
| `0x8002dc80` | transforms vertices with fixed depth | existing exact |
| `0x8002dd28` | projects vertices with zero secondary depth | existing exact |
| `0x8002ddb4` | emits depth-cued TMD primitive packets | unclaimed; 0x728-byte packet renderer |
| `0x8002e4dc` | emits depth-cued packets with a related primitive mode path | unclaimed; 0x704-byte packet renderer |
| `0x8002ebe0` | emits lit TMD primitive packets via NormalColorCol/Col3 | unclaimed; 0x5b4-byte packet renderer |
| `0x8002f5b0` | clips, lights, depth-cues, and enqueues a polygon | unclaimed; 0x258-byte packet builder |
| `0x8002f808` | projects a selected TMD and renders clipped packets | unclaimed; 0x754-byte renderer, calls `0x8002da94` |
| `0x8002ff5c` | subdivides/copies TMD primitive data into generated packets | unclaimed; 0xcbc-byte routine with 1248-byte stack frame |

The new `display.c` function at `0x8002d4f4` uses the typed graphics render
state and existing matrix helpers. Its full ten-function unit has identical
listings under `kf try`; focused `kf sema match` reports 100% for the new
function and preserves the nine existing exact functions.

`tmd_pipeline.c` now owns the contiguous `0x8002d8b0`–`0x8002ddb4` run,
combining registration, projection, and transform functions. The two new
projection routines share the projected-vertex array and current TMD vertex pointer.
The first has three modes selected by fog distance; the second checks the GTE
flag word against `0x1000`. Initial code with separately initialized branch
loops compiled to equivalent zero checks and separate stack output slots.
Initializing one `remaining` count before the fog branch, then decrementing
inside the selected loop, reproduces the retail decrement/compare, common
stack slots, and instruction schedule without artificial locals. Focused
`kf try` reports identical listings for both, and `kf sema match` reports
100.000000000% for each.

The primitive-index walker is closely related to KF1's typed switch over
F3/G3/FT3/GT3/F4/G4/FT4/GT4 packet bodies. KF2 instead accepts a TMD pointer
and its 29-word dispatch table begins at `0x80011410`; current seed data
classifies each table word independently. Its indirect `jr` successors and
RODATA extent need review before claiming a switch-bearing source unit. The
six renderers retain address-derived names pending complete packet and
runtime field ownership. Their direct calls support the roles above, but do
not prove original translation-unit boundaries.

Focused `kf match` marks every function in the consolidated `tmd_pipeline.c`
unit strict exact: 8/8 functions, 1,284/1,284 code bytes. The global command
still exits on repository-wide known-reference closure; its target relink
check passed 183/183 units. The parent campaign owner handles repository tests.

## Cross-game packet-renderer evidence (2026-10-01)

An aligned 32-byte-window search between the current KF1 GAME linked object
and KF2 retail GAME finds 181 matching windows for KF2 `8002ddb4`, 193 for
`8002e4dc`, and 20 for `8002ebe0` inside KF1 `8001c7f8`
`render_enqueue_tmd`. These are shared instruction fragments, not proof that
the whole renderers, source files, or toolchains are identical. KF1's
[`695d1ae6`](https://github.com/sushi-shi/kings-field-decomp/commit/695d1ae6)
history and `game-enqueue-stack-order.md` demonstrate that declaration order
of real packet-header/count scalars controls their spill slots in the pinned
GCC probe. Its `6a56ebd7` history also records how a typed projected-vertex
base recovered a renderer address chain; KF2 needs its own referent check
before applying that source shape.

KF2 `8002e4dc` initially stored the first loop scalar at `sp+40` where
retail uses `sp+48`, then the second at `sp+48` where retail uses `sp+40`.
Declaring the existing `header` before `remaining`, and assigning the header
inside the loop, restores those retail spill slots without changing packet
semantics or the eight exact sibling functions. Direct strict objdiff remains
97.27840% for this 1796-byte WIP: branch extents and the positive-depth
FT3/GT3 paths still differ. A source-equivalent positive-depth guard trial
compiled identically and was reverted. KF1 therefore supplies a verified
source-order clue here, not a closure claim.
