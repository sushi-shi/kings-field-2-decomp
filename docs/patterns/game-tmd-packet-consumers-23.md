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
| `0x8002ddb4` | WIP; focused 93.6% | Textured walker has 44 retail CFG blocks and 28 branches versus probe 42/26; two depth checks are folded into a compiled shared tail. |
| `0x8002e4dc` | WIP; focused 92.0% | Signed promoted packet mode recovers retail `srl`/`slti`; retail has 44 CFG blocks and 28 branches versus probe 42/26, with two FT3/GT3 depth checks moved into the compiled shared tail. |
| `0x8002ebe0` | WIP; focused 88.7% | G3/G4 color is the SDK `CVECTOR`; retail has 26 CFG blocks/17 branches versus probe 25/16. A source-only shared-tail trial fell to 70.4% and was discarded. |
| `0x8002f194` | Exact control; focused SAME | Map packet emitter consumes the shared four-byte `CVECTOR`. |
| `0x8002f5b0` | WIP; focused 82.6% | Clipped fan uses the complete SDK `EVECTOR`; CFG agrees at 13 blocks/7 branches, while saved-register and UV/color store order differs. |
| `0x8002f808` | WIP; focused 50.5% | Prepared-asset FT3/FT4 renderer has 51/51 CFG blocks and 35/35 branches; the retail 168-byte frame versus 112-byte probe lacks a proved extra object. |
| `0x8002ff5c` | Unclaimed WIP | Sole caller supplies a 4096-byte `KfTmdPreparedAsset`; 1248-byte retail frame, 128-entry `SVECTOR` midpoint workspace, and 14 `resource_copy_words` calls are proved. Complete child-packet writes remain unresolved. |
| `0x80030c18` | WIP; focused 98.7% | Caller limits source primitive count below 16 before subdivision. CFG agrees at 11 blocks/6 branches; its only listing difference is the order of the `shape+2` byte load and the third-argument move. |
| `0x80030de4` | Exact control; focused SAME | Map-cell layer renderer calls `0x80030c18`. |
| `0x80030f5c` | Exact control; focused SAME | Scans the map-cell render mask after TMD selection. |
| `0x80031024` | Exact control; focused SAME | Render-grid walker calls the cell object path. |
| `0x800311b0` | WIP; focused 57.5% | FT4 GPU packet writer retains the proven `AddPrim` call and field stores; local probe chooses a smaller register-save set and moves the SDK code byte store before the semitrans branch. Earlier strict report was 92.14815%. |
| `0x800312f4` | Exact control; focused SAME | One sliding panel calls the shared FT4 writer. |
| `0x80031384` | Exact control; focused SAME | Paired sliding panel calls the same writer. |
| `0x80031414` | Exact control; focused SAME | Color-byte overlay calls the FT4 writer. |
| `0x800314d4` | Exact control; focused SAME | Stores the control and three color bytes consumed by `0x80031414`. |
| `0x80031850` | WIP; focused 68.9% | World-model renderer calls the textured TMD walkers through the typed packet state; target and probe both have 40 CFG blocks and 16 branches, but their early successor order differs. |
| `0x80031d8c` | WIP; focused 79.5% | Animated-object renderer calls `0x8002ebe0` with its depth sign-extended at the call site; CFG agrees at seven blocks and two branches, while frame/register allocation differs. |
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

The 14 static `resource_copy_words` sites split into five FT4 calls, five FT3
calls, one generic-packet call, and three final array copies. Each textured
arm copies its first output packet, copies three or four source texture words
into a stack record, then copies the other three output packets explicitly.
The FT4 sites are at function offsets `+0xf0`, `+0x35c`, `+0x4f4`, `+0x590`,
and `+0x62c`; FT3 uses `+0x718`, `+0x8b4`, `+0x9c8`, `+0xa34`, and `+0xa94`.
A temporary C probe with those 14 call sites still had 12 CFG blocks against
retail's 11, despite six branches on each side; its 1232-byte frame also
missed the retail 1248-byte extent. Its packet field writes remain different,
so it was not admitted as source. The signed integer object index, rather
than a callee-masked halfword, is supported by the retail entry instructions.
Retail writes generated FT4 UV components as individual bytes at packet
offsets `+8..9`, `+12..13`, and `+16..17`, then stores paired vertex indices
as words at `+24` and `+28`. A halfword-only face view is valid for reading
the source packet, but it does not yet express these output writes naturally.
The source vertex offsets in this builder are read with signed `lh`; the
other TMD walkers read their prepared indices with `lhu`. Keep that
consumer-specific signedness distinction until the original packet type is
resolved.
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
