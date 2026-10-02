# GAME collision shape and confirmed callers: 25-function verdict

Fresh focused builds and safe isolated GAME delinks cover the shape dispatcher,
its grid/height callers, the shared collision-row helpers, the collision
channels, and two player collision consumers. The seven target carves
materialize **499 relocations with none withheld**. Direct strict objdiff
reports **16 exact functions and nine WIPs**. The grid fallback's 10-byte
DATA and the height unit's 3,520-byte default-row DATA remain 100%.
Retail has three proven direct calls into the shape dispatcher, at
`0x8002b64c`, `0x8002b71c`, and `0x8002b84c`; their five-argument call
forms agree with the current height-wrapper source.

| GAME VA | Function or role | Retail bytes | Strict text | Verdict |
| --- | --- | ---: | ---: | --- |
| `0x80027928` | landing sound helper | 96 | 100% | Exact. |
| `0x80027988` | impact volume helper | 68 | 100% | Exact. |
| `0x800279cc` | player collision sound | 1,452 | 98.23967% | WIP; two extra retail `player_state` address pairs rematerialize an already referenced field. |
| `0x80027f78` | player collision response | 684 | 95.91228% | WIP; collision calls and branches agree, with a retained player-state base in the probe. |
| `0x8002a988` | grid/layer sampler | 284 | 100% | Exact. |
| `0x8002aaa4` | shape-command dispatcher | 2,912 | 57.52610% | WIP; case layout and two retail-only CFG blocks remain. |
| `0x8002b604` | shape height caller | 120 | 100% | Exact. |
| `0x8002b67c` | selected-layer height caller | 192 | 94.895836% | WIP; retail reloads cached height while the probe carries its computed value. |
| `0x8002b73c` | grid footprint update | 188 | 98.40426% | WIP; row pointer and index registers differ. |
| `0x8002b7f8` | adjusted-height shape caller | 124 | 100% | Exact. |
| `0x8002b874` | collision argument snapshot | 352 | 91.5% | WIP; actor/object radius and height load schedule differs. |
| `0x8002b9d4` | collision-channel dispatcher | 580 | 95.37931% | WIP; frame and saved-register schedule differs. |
| `0x8002bc18` | collision-row helper | 292 | 100% | Exact. |
| `0x8002bd3c` | collision-row helper | 128 | 100% | Exact. |
| `0x8002bdbc` | collision-row helper | 224 | 100% | Exact. |
| `0x8002be9c` | collision-row helper | 156 | 100% | Exact. |
| `0x8002bf38` | collision-row helper | 116 | 100% | Exact. |
| `0x8002bfac` | collision-row helper | 40 | 100% | Exact. |
| `0x8002bfd4` | mask-segment rasterizer | 412 | 73.91262% | WIP; two-axis induction and frame allocation differ. |
| `0x8002c170` | mask-run scanner | 100 | 100% | Exact. |
| `0x8002c1d4` | row-fill caller | 188 | 100% | Exact. |
| `0x8002c290` | layer mask update | 404 | 100% | Exact. |
| `0x8002c424` | mask-line scan | 588 | 98.29932% | WIP; cursor/register schedule differs. |
| `0x800314fc` | collision-channel draw | 312 | 100% | Exact. |
| `0x80031634` | collision-channel add | 148 | 100% | Exact. |

The shape dispatcher remains 2,912 retail versus 2,852 candidate text bytes,
with 174/172 CFG blocks and 99/98 branches. Its 196-byte switch table is
27.551018% strict, yet all 49 pointer rows retain the same thirteen target
classes and class order. The raw table addends differ on all 49 rows: the
candidate shifts are between 12 and 100 bytes earlier, with the shared
default target 60 bytes earlier. These are case-layout displacements, not
new destinations. Both objects reference the same collision BSS
fields; the retail object has five additional local `R_MIPS_26` jumps.
The shape-bank allocation and defining TU are still unproved; a separate
data-owner audit found no new boundary that would justify replacing its
current provisional view.
The first raw difference is a 128-byte versus 112-byte frame, followed by
the retail store of the persistent floor flag at `sp+16` and independent
cache-field load scheduling. No additional source object is proved by those
frame bytes.

The retail opcode `0x11` path enters body `+0x20c`, clears `$s7` at
`+0x210`, then branches on `$s7` at `+0x214`. No table row or decoded direct
edge enters the branch independently, so its taken edge is unreachable from
that case. The actual persistent floor flag is a separate stack value;
adding a source-level reset or forced branch would misstate it. In opcode
families `0x30` and `0x32`, retail uses distinct slope multiplication tails
before a shared cache-height continuation, whereas this GCC probe folds the
products. A natural typed `s32 slope = (s16)operand[4]` local in the `0x30`
tail compiled SHA256-identically to the retained source and was reverted.
Neither this result nor the table percentage justifies changing the case
values, fields, calls, or target addends. The shape dispatcher remains WIP.

All exact controls above were already exact in the current source; this
campaign verifies their preservation and banks no new function. The other
eight WIPs retain their existing calls, field identities, and documented
source meanings. No tracked C or inventory change was retained.
