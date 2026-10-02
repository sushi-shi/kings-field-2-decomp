# GAME TMD pipeline depth tails

The two textured packet walkers, GAME `0x8002ddb4` and `0x8002e4dc`, use
the same FT3 depth sequence: compute the average projected depth, reject a
nonpositive result, add the caller's depth bias, reject an ordering-table
index outside 8192 entries, then call `AddPrim`. Retail branches out of the
FT3 arm at the table bound. Spelling that bound as an early `break` in both
FT3 arms preserves the source behavior and exposes the missing branch under
the pinned GCC probe. It is not evidence that the source now reproduces the
original control-flow layout: retail still joins a shared enqueue tail at a
different point.

| GAME walker | Before: focused CFG / branches; strict text | Retained FT3 guard: focused CFG / branches; strict text |
| --- | --- | --- |
| `0x8002ddb4` | 44/42, 28/26; 97.34716% | 44/44, 28/28; 97.434494% |
| `0x8002e4dc` | 44/42, 28/26; 97.29398% | 44/44, 28/28; 97.38307% |

The retail FT3 tail has `mflo a0`, a delayed `blez a0`, a depth-bias load,
and a jump whose delay slot adds that bias before reaching the shared
ordering-table check. The candidate now emits the positive-depth branch and
an explicit `sltiu`/`beqz` table-bound branch in the FT3 arm, but moves the
addition and enqueue jump. Thus the equal block and branch **counts** do not
establish matching block order or instructions. The first reported CFG
successor still differs at the earlier packet-mode dispatch.

Changing the GT3 arms to the same early-bound form instead produced 44/45
blocks and 28/30 branches in each walker, so that trial was reverted. A
similar early guard in the lit walker `0x8002ebe0` produced 26/26 blocks and
17/17 branches, yet dropped its strict text from 95.27945% to 92.265755%; it
was also reverted. The retained lit walker is still 26/25 blocks and 17/16
branches, with its 96-byte retail versus 104-byte candidate frame and depth
argument schedule unresolved.

Retail has one enqueue tail for the lit walker's four packet modes. A
source-only trial routed those modes through an explicit common `AddPrim`
call, preserving their branch and call behavior. The focused listing rose
from 88.7% to 89.4%, but isolated strict text fell from 95.27945% to
92.630135%; it still had 26/25 blocks and 17/16 branches. The compiler also
spilled the depth argument as a halfword, whereas retail preserves a shifted
word. The trial was reverted; the common tail alone does not recover the
retail argument lifetime or scheduling.

The focused rebuild kept the unit's eight sibling functions exact. An
isolated strict object comparison confirmed all eight at 100%, the two FT3
figures above, and an unchanged 95.27945% for `0x8002ebe0`. The retail and
candidate objects each have 169 `.rel.text` entries with identical ordered
relocation types and target symbols. Separate off-tree compiler probes did
not justify changing the unit profile: GCC 2.5.7 with
`-fno-cse-skip-blocks` was byte-identical to the prior candidate, and GCC
2.6.0 regressed six of the eight exact siblings. No function in this note is
newly exact or bankable.
