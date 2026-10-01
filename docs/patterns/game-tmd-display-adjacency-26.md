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
| `0x8002d5dc` | WIP, 96.13260% prior strict | Eight typed packet-mode cases and offsets agree; an extra entry move shifts jump-table targets. Source-only packet-base and `&object->primitive_offset` interior-pointer variants did not remove the first divergence; both were discarded. |
| `0x8002d8b0` | Exact, 100% | TMD registration. |
| `0x8002d8f0` | Exact, 100% | TMD slot setter. |
| `0x8002d910` | Exact, 100% | TMD slot release. |
| `0x8002d918` | Exact, 100% | Fog-depth projection. |
| `0x8002da94` | Exact, 100% | Alternate vertex projection. |
| `0x8002dbd8` | Exact, 100% | Vertex transform. |
| `0x8002dc80` | Exact, 100% | Depth-biased vertex transform. |
| `0x8002dd28` | Exact, 100% | Projected-vertex conversion. |
| `0x8002ddb4` | WIP, 97.34716% prior strict | Textured packet walker retains two fewer CFG branches than retail. A source-only explicit shared-tail trial reduced the focused listing from 93.6% to 88.1%; it was discarded, leaving all eight exact siblings unchanged. |
| `0x8002e4dc` | WIP, 96.587975% prior strict | Paired textured packet walker has the same two-branch gap. The raw mode byte is kept in a promoted signed `s32`: retail uses `srl s8` and signed `slti` dispatch; focused listing rose from 89.9% to 92.0% while eight exact siblings stayed SAME. |
| `0x8002ebe0` | WIP, 95.27945% prior strict | Blend/lighting packet walker lacks one retail depth-tail block. |
| `0x8002f194` | Exact, 100%; focused SAME | Map packet emitter and its four-byte initialized color datum. |
| `0x8002f5b0` | WIP, 95.833336% prior strict | Clipped GT3 fan uses the SDK's complete 44-byte `EVECTOR`. Focused listing stayed DIFF 82.6% after this type correction; saved registers and one UV/color store schedule differ. |
| `0x8002f808` | WIP, 63.362473% prior strict | Alternate prepared TMD renderer has typed FT3/FT4 fields and 51/51 focused CFG blocks, but retail's 168-byte frame versus the probe's 112-byte frame has no proved extra object. The decoded body addresses only `sp+80` as a local color and accesses no `sp+36..79` slot directly; the gap alone cannot justify padding. Focused listing stayed DIFF 50.5%. |
| `0x8002ff5c` | Unclaimed WIP | Retail has a 1248-byte frame, four 8-byte source-vertex records at `sp+16..47`, a 128-entry `SVECTOR` midpoint workspace at `sp+64..1087`, and 14 proven `resource_copy_words` calls. Entry multiplies the object index by 28, then writes the output object's primitive offset as 28. Its sole caller limits source primitives to fewer than 16, bounding new midpoint vertices to 75. Packet-control and complete local ownership remain unresolved. |

The pinned Psy-Q `Clip3FTP`/`Clip4FTP` signatures accept `EVECTOR **`, and
their 44-byte result type places depth at `+16`, perspective at `+20`, packed
screen coordinates at `+24`, `CVECTOR` color at `+28`, and UV at `+32`.

Both selected FT3/FT4 branches increment the output primitive count by three,
so each source face becomes four packets; they append three or five midpoint
vertices respectively. The builder's final copy sequence sets its output
object vertex count to the source count plus generated midpoint count, copies
original vertices followed by the midpoint workspace, then sets the normal
offset to the end of those 8-byte vertices and copies the source normals.
Those object fields are proven
even though the per-primitive expansion records remain incomplete.
Retail does not store the output object's `scale` word at `+24`; a whole-object
copy would therefore misrepresent this builder.

The G3/G4 on-disk packet's four-byte color at `+0` is passed directly to
`NormalColorCol3` and `NormalColorCol` by `0x8002ebe0` (`move a3,s1` before
the three-normal call and `move a1,s1` before the fourth-normal call). Its
shared field is now typed as the SDK `CVECTOR`. Focused rebuilds preserved eight SAME
`game.tmd_pipeline` siblings and the SAME `0x8002f194`, `0x80030de4`,
`0x80030f5c`, and `0x80031024` controls. The G3/G4 WIP listing and other
compiled consumers were unchanged.

`game.render_map` focused comparison preserves the exact `0x8002f194`
listing after replacing the former partial game-side view; the two WIP
siblings retain their previous focused listings. The 1248-byte prepared
builder has no source claim because its full packet and stack-record model
is not yet established. No frame padding or codegen-only edit was retained.
