# GAME display and TMD packet adjacency

This 26-function pass covers the contiguous GAME display/TMD run from
`0x8002d0a4` through the prepared-object builder at `0x8002ff5c`. Calls and
shared TMD/primitive-buffer state connect the setup functions to packet
projection, clipping, and map emission. Exact percentages are from the last
strict report; fresh `kf try` findings are marked as focused listing checks.
Contiguity is a working ownership boundary, not historical TU proof.

| GAME VA | Verdict | Evidence or remaining boundary |
| --- | --- | --- |
| `0x8002d0a4` | Exact, 100% | Fog-near setter and GTE state. |
| `0x8002d0d4` | Exact, 100% | Display/GPU initialization. |
| `0x8002d248` | Exact, 100% | Primitive-buffer and display reset. |
| `0x8002d32c` | Exact, 100% | Begin-frame OT and primitive-buffer selection. |
| `0x8002d3c4` | Exact, 100% | GPU sync, buffer swap, and OT draw. |
| `0x8002d458` | Exact, 100% | TMD slot selection. |
| `0x8002d484` | Exact, 100% | Current TMD object lookup. |
| `0x8002d4a8` | Exact, 100% | Projected-vertex pointer setter. |
| `0x8002d4b8` | Exact, 100% | Object vertex-array selection. |
| `0x8002d4f4` | Exact, 100% | View matrix/position update. |
| `0x8002d5dc` | WIP, 96.13260% prior strict | Eight typed packet-mode cases and offsets agree; an extra entry move shifts jump-table targets. A source-only packet-base variant did not remove the first divergence. |
| `0x8002d8b0` | Exact, 100% | TMD registration. |
| `0x8002d8f0` | Exact, 100% | TMD slot setter. |
| `0x8002d910` | Exact, 100% | TMD slot release. |
| `0x8002d918` | Exact, 100% | Fog-depth projection. |
| `0x8002da94` | Exact, 100% | Alternate vertex projection. |
| `0x8002dbd8` | Exact, 100% | Vertex transform. |
| `0x8002dc80` | Exact, 100% | Depth-biased vertex transform. |
| `0x8002dd28` | Exact, 100% | Projected-vertex conversion. |
| `0x8002ddb4` | WIP, 97.34716% prior strict | Textured packet walker retains two fewer CFG branches than retail. |
| `0x8002e4dc` | WIP, 96.587975% prior strict | Paired textured packet walker has the same two-branch gap. |
| `0x8002ebe0` | WIP, 95.27945% prior strict | Blend/lighting packet walker lacks one retail depth-tail block. |
| `0x8002f194` | Exact, 100%; focused SAME | Map packet emitter and its four-byte initialized color datum. |
| `0x8002f5b0` | WIP, 95.833336% prior strict | Clipped GT3 fan uses the SDK's complete 44-byte `EVECTOR`. Focused listing stayed DIFF 82.6% after this type correction; saved registers and one UV/color store schedule differ. |
| `0x8002f808` | WIP, 63.362473% prior strict | Alternate prepared TMD renderer has typed FT3/FT4 fields and 51/51 focused CFG blocks, but retail's 168-byte frame versus the probe's 112-byte frame has no proved extra object. Focused listing stayed DIFF 50.5%. |
| `0x8002ff5c` | Unclaimed WIP | Retail has a 1248-byte frame, a 128-entry `SVECTOR` midpoint workspace, and 14 `resource_copy_words` call sites. Its sole caller limits source primitives to fewer than 16, bounding new midpoint vertices to 75. Packet-control and complete local ownership remain unresolved. |

The pinned Psy-Q `Clip3FTP`/`Clip4FTP` signatures accept `EVECTOR **`, and
their 44-byte result type places depth at `+16`, perspective at `+20`, packed
screen coordinates at `+24`, `CVECTOR` color at `+28`, and UV at `+32`.
`game.render_map` focused comparison preserves the exact `0x8002f194`
listing after replacing the former partial game-side view; the two WIP
siblings retain their previous focused listings. The 1248-byte prepared
builder has no source claim because its full packet and stack-record model
is not yet established. No frame padding or codegen-only edit was retained.
