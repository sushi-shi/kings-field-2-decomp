# GAME TMD packet consumers: 23-function focused pass

This pass follows the prepared-object edge `0x80030c18 -> 0x8002ff5c`,
the four-packet TMD walkers, and the frame/render callers that consume their
shared graphics state. An address is a GAME.EXE identity throughout. `SAME`
below is a focused object listing verdict; it is not a new strict certification.
The existing strict exacts are retained as controls.

| Function | Retained verdict | Evidence and remaining question |
| --- | --- | --- |
| `0x8002d5dc` | WIP; focused 38.6% | Eight packet-mode cases are typed; its entry packet-base move still differs. |
| `0x8002dd28` | Exact control; focused SAME | Projects the typed TMD vertex array used by the walkers. |
| `0x8002ddb4` | WIP; focused 93.6% | Textured walker has two retail depth-tail branches absent from the probe. |
| `0x8002e4dc` | WIP; focused 92.0% | Signed promoted packet mode recovers retail `srl`/`slti`; depth-tail branches and stack-slot choice remain. |
| `0x8002ebe0` | WIP; focused 88.7% | G3/G4 color is the SDK `CVECTOR`; retail shares one `AddPrim` tail, but the source-only shared-tail trial fell to 70.4% and was discarded. |
| `0x8002f194` | Exact control; focused SAME | Map packet emitter consumes the shared four-byte `CVECTOR`. |
| `0x8002f5b0` | WIP; focused 82.6% | Clipped fan uses the complete SDK `EVECTOR`; remaining saved-register and UV/color store order differs. |
| `0x8002f808` | WIP; focused 50.5% | Prepared-asset FT3/FT4 renderer has 51/51 focused CFG blocks; the retail 168-byte frame versus 112-byte probe lacks a proved extra object. |
| `0x8002ff5c` | Unclaimed WIP | Sole caller supplies a 4096-byte `KfTmdPreparedAsset`; 1248-byte retail frame, 128-entry `SVECTOR` midpoint workspace, and 14 `resource_copy_words` calls are proved. Complete child-packet writes remain unresolved. |
| `0x80030c18` | WIP; focused 98.7% | Caller limits source primitive count below 16 before subdivision. Its only listing difference is the order of the `shape+2` byte load and the third-argument move. |
| `0x80030de4` | Exact control; focused SAME | Map-cell layer renderer calls `0x80030c18`. |
| `0x80030f5c` | Exact control; focused SAME | Scans the map-cell render mask after TMD selection. |
| `0x80031024` | Exact control; focused SAME | Render-grid walker calls the cell object path. |
| `0x800311b0` | WIP; focused 57.5% | FT4 GPU packet writer retains the proven `AddPrim` call and field stores; local probe chooses a smaller register-save set and moves the SDK code byte store before the semitrans branch. Earlier strict report was 92.14815%. |
| `0x800312f4` | Exact control; focused SAME | One sliding panel calls the shared FT4 writer. |
| `0x80031384` | Exact control; focused SAME | Paired sliding panel calls the same writer. |
| `0x80031414` | Exact control; focused SAME | Color-byte overlay calls the FT4 writer. |
| `0x800314d4` | Exact control; focused SAME | Stores the control and three color bytes consumed by `0x80031414`. |
| `0x80031850` | WIP; focused 68.9% | World-model renderer calls the textured TMD walkers through the typed packet state. |
| `0x80031d8c` | WIP; focused 79.5% | Animated-object renderer calls `0x8002ebe0` with its depth sign-extended at the call site. |
| `0x800321d8` | WIP; focused 95.8% | Resource TMD queue read has an unresolved `0x8009b0a0` address-form origin; leave the literal unchanged pending a proved owner. |
| `0x8003247c` | Unclaimed WIP | Frame child reaches the TMD/resource update and world-model paths; the complete workspace/record owner is not established. |
| `0x800335a0` | Exact control; focused SAME | Frame driver calls the render/resource children and preserves the packet-consumer chain. |

The prepared-object builder begins at output object `+0x0c`, sets its
primitive offset to 28, and reads the input object with a 28-byte stride.
It recognizes FT4 modes `0x2c/0x2e` and FT3 modes `0x24/0x26`, expands each
selected face into four packets, and appends five or three midpoint vertices.
The output object vertex count is the input count plus generated midpoints;
the builder copies original vertices, then midpoints, then normals. Retail
does not store the output object's `scale` word. This evidence supports the
typed `KfTmdPreparedAsset *` contract now declared in `tmd_packets.h`, but
not yet a complete C claim for the child-packet UV/index writes.
The three packet-walker declarations also live there with one caller-visible
signature each; focused TMD, world-model, animated-object, and cell-renderer
listings stayed at their prior verdicts after the declaration consolidation.

The `0x8002ebe0` trial made the shared `AddPrim` tail explicit and routed
clip failures directly to the packet advance. Retail has that shared block,
but the probe kept `blend_mode` live in a saved register, changed packet
pointer allocation, and shortened the function. Changing the third formal
from `s16` to `s32` alone gave 85.7% focused; combining it with the shared
tail gave 70.4%. The sole caller already sign-extends its depth argument,
so neither signature trial established a source correction.
