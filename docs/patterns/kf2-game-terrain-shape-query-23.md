# GAME terrain, shape, and line-query cohort

This 23-function cohort connects the collision-grid sampler and shape
dispatcher to the height and mask helpers, then checks collision-channel and
map-object line-query controls. Each row below is a fresh isolated **strict**
`objdiff-cli diff` result after a focused `kf try` build of its unit. The
comparison used GAME target and candidate objects; `--loose` was not used.

| GAME address | Strict text | Verdict |
| --- | ---: | --- |
| `0x8002a988` | 100% | Exact collision-grid sample. |
| `0x8002aaa4` | 57.526100% | WIP shape dispatcher: 174/172 CFG blocks, 99/98 branches, and separate retail multiplication tails in cases `0x30`/`0x32`. |
| `0x8002b604` | 100% | Exact height-probe caller. |
| `0x8002b67c` | 94.895836% | WIP: retail reloads the stored cached height before the shape call; the probe retains the computed value. |
| `0x8002b73c` | 98.404260% | WIP row-pointer and loop-index register choice; the row update and byte store agree. |
| `0x8002b7f8` | 100% | Exact second shape caller. |
| `0x8002b874` | 91.500000% | WIP load and register schedule around the player, actor, and object height sources. |
| `0x8002b9d4` | 95.379310% | WIP frame and saved-register allocation; cache stores and call/result semantics agree. |
| `0x8002bc18` | 100% | Exact collision-row helper. |
| `0x8002bd3c` | 100% | Exact collision-row helper. |
| `0x8002bdbc` | 100% | Exact collision-row helper. |
| `0x8002be9c` | 100% | Exact collision-row helper. |
| `0x8002bf38` | 100% | Exact collision-row helper. |
| `0x8002bfac` | 100% | Exact collision-row helper. |
| `0x8002bfd4` | 73.912620% | WIP two-axis mask rasterizer; induction order and frame allocation differ. |
| `0x8002c170` | 100% | Exact mask-run scanner. |
| `0x8002c1d4` | 100% | Exact mask-row fill. |
| `0x8002c290` | 100% | Exact mask-cell update. |
| `0x8002c424` | 98.299320% | WIP saved-register assignment; typed cell lookup and cursor control agree. |
| `0x8002c670` | 91.624245% | WIP mask sweep: retail retains a mask-state pointer where the probe rematerializes two adjacent byte addresses. |
| `0x800314fc` | 100% | Exact collision-channel draw control. |
| `0x80031634` | 100% | Exact collision-channel accumulator. |
| `0x80036078` | 100% | Exact map-object range query. |

The result is **15 strict-exact functions and eight WIPs**. The grid sampler's
10-byte initialized datum, height wrappers' 3,520-byte datum, and mask
sweep's 28-byte shape table are also byte-exact. The shape dispatcher's
196-byte `.rodata` is 27.551018% strict by bytes and addends, while all 49
reviewed pointer rows preserve the same thirteen target classes and order.
Its indirect `jr` remains an indirect edge, not a proved callee. The safe
one-function shape carve materialized 239 relocations with zero withheld;
retail and candidate both contain 23 shape-bank HI16/LO16 pairs and one
switch-base pair. Retail has 47 local `R_MIPS_26` sites versus 42 in the
candidate, consistent with the remaining physical control-flow differences.

The mask sweep calls `0x8002bfd4` four times at `0x8002c9ac`,
`0x8002c9c4`, `0x8002c9dc`, and `0x8002c9f0`. The rasterizer has no outgoing
function call or string reference. Its two axis loops, unsigned cell origins,
signed 16-bit direction/error/count tests, and byte-grid stores are already
expressed in source. The sweep has eleven blocks and four branches on both
sides; its typed lighting-field and mask-state owners agree with retail. No
new global, missing call, or source-level branch follows from the residual.

The shape dispatcher's case `0x11` contains a retail `bnez` immediately after
`move s7,zero`; no reviewed table row or direct edge enters that branch, so
its taken successor is unreachable from the decoded table entry. It cannot
justify a fabricated C condition. KF1's collision-grid code is only an
algorithmic lead: its shape dispatch does not share this switch contract.
The detailed case, table, and negative source-probe evidence remains in
[kf2-game-collision-height.md](kf2-game-collision-height.md). This cohort
retains the current source and identities unchanged.
