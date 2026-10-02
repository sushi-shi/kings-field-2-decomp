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

## Fresh packet-walker and caller controls

A new narrow safe delink of the complete `game.tmd_pipeline` unit has 338
relocations and none withheld. Direct strict comparison confirms all eight
small siblings exact. The textured walkers remain `0x8002ddb4` **97.434494%**
and `0x8002e4dc` **97.38307%**, each with 44/44 CFG blocks and 28/28
branches; the lit walker `0x8002ebe0` remains **95.27945%**, 26/25 blocks and
17/16 branches. Target and candidate each have 169 ordered text relocations
and exactly one physical `AddPrim` call relocation in each walker.

The lit mismatch is bounded to its shared enqueue tail. Retail reloads the
packed signed depth from the stack, shifts it to the index, and branches on
nonpositive depth and the 8192-entry table bound separately. The current
probe computes the invariant range Boolean before the primitive loop and
uses one branch at the tail. The caller `0x80031d8c` sign-extends its depth
halfword before the call, supporting the existing `s16` boundary. The prior
early-guard and common-tail source trials above did not close the gap; no
new source spelling is retained from this recheck.

The probe's pre-loop test is `((u16)(fixed_depth - 1) < 8191)`, which accepts
exactly signed depths 1 through 8191. It is equivalent to the retail pair of
signed-positive and unsigned-upper-bound branches for this halfword input;
the discrepancy is when the checks execute, not which depth values enqueue.

Twelve directly related caller claims were also rebuilt against fresh safe
targets, all with zero withheld relocations:

| Caller unit | Strict result and bounded verdict |
| --- | --- |
| `render_world_model` | `0x80031850` 99.29851%, 40/40 CFG and 16/16 branches. The residual swaps saved-register roles for call inputs and schedules two `-1` comparisons differently; the TMD calls agree. |
| `render_animated_object` | `0x80031d8c` 94.65414%, 7/7 CFG and 2/2 branches. Retail saves the blend argument early in `$s7`; the probe reloads it from its stack home at the final lit-walker call. Its depth argument remains sign-extended in both. |
| `player_weapon_render`, `menu_model_render`, `menu_item_model` | `0x800316c8`, `0x80033994`, `0x800221e8`, and `0x800222bc` are strict exact. The item unit's 28-byte DATA is exact. |
| `render_map_cell` | `0x80030c18` 96.521736% with 11/11 CFG and 6/6 branches; its `0x80030de4`, `0x80030f5c`, and `0x80031024` siblings and 540-byte DATA are exact. The remaining cell-renderer difference is independent entry load/address order before `SetRotMatrix`. |
| `resource_startup` | `0x80015d58` 89.03145% and `0x80015fd4` 89.710144%, with 83-byte RODATA exact. The unresolved workspace literals still lack proved defining owners for the retail signed-low relocations. |

Seven callers are exact and five remain WIP. This pass found no new
source-backed field, call, width, or referent correction in those callers.

An additional off-tree control gave each textured walker one explicit
source-level enqueue label after its four packet arms, carrying the same
primitive pointer and depth to a shared `AddPrim` expression. The current
compiler already reduces the four case-local calls to the one retail call
site, so that physical call count does not establish a shared original C
tail. Strict text fell from 97.434494% to **96.8428%** for `0x8002ddb4` and
from 97.38307% to **96.77951%** for `0x8002e4dc`; all eight exact siblings
remained exact. Both off-tree trials were discarded.

A separate off-tree signature control widened only the lit walker's third
parameter from `s16` to `s32`. Without explicit halfword normalization, the
compiled function changed size and lost its strict function correspondence.
Normalizing once to `s16` before the four depth checks reproduced the current
**95.27945%** candidate in strict comparison. The sole known caller
sign-extends its halfword argument, and the retail callee shifts it left then
arithmetically right before using the ordering-table index. These controls
give no reason to change the current fixed-width boundary.

Splitting each lit-arm depth condition into two nested signed `if` statements
was another off-tree source-equivalent check. It preserved the single
physical `AddPrim` call but compiled a 1,468-byte body against 1,460 retail
bytes; isolated objdiff did not assign a function similarity score to that
layout. It did not establish the retail per-packet guard shape and was
discarded.
