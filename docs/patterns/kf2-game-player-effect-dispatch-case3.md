# GAME player effect dispatcher case-3 Z join

GAME `0x80025a18` is the variadic, 53-case player effect dispatcher. Its
case 3 probes for an actor and chooses an effect position. On the no-actor
path, retail adds the forward X component, copies the camera Y, computes
the forward Z sum in a jump delay slot, then jumps **into** the Z store
shared with the collision-hit actor-position override. The raw jump at
`0x80025b14` targets `0x80025c34` (`nop`), immediately before the common
`sw v0,56(sp)` at `0x80025c38`. A collision miss bypasses that store after
the earlier computed position has already been written.

The prior C wrote the no-actor Z field before jumping past the shared
store. Giving the two store-taking paths one `case3_z` value and a labelled
Z store preserves the three behaviors while reproducing the retail join:
the candidate now computes the no-actor sum in its jump delay slot and
lands at a `nop` followed by the shared word store. The collision-hit path
also reaches that store after loading the actor's Z. This is a raw-backed
source correction, not an exact closure.

| Control | Before | Retained case-3 join |
| --- | --- | --- |
| Focused CFG blocks | 99 retail / 96 candidate | 99 / 97 |
| Focused branches | 31 / 31 | 31 / 31 |
| Isolated strict text, `0x80025a18` | 95.556700% | 95.850520% |
| Isolated strict `.rodata` | 87.603300% | 39.876034% |
| Other unit functions | 15/15 strict exact | 15/15 strict exact |
| Switch destination groups | 31 across 53 rows, equal | 31 across 53 rows, equal |

The candidate body shrinks from `0x918` to `0x914` bytes. Fifty table
addends move four bytes earlier relative to the prior candidate, leaving
all 53 currently different from retail (the prior candidate had 42 equal
offsets). The 53-row table still has the same 31 destination equivalence
groups in the same order. The unit's 492 `.rel.text` entries preserve their
ordered type/target-symbol sequence relative to the pre-edit candidate;
that sequence was already differently ordered from retail. The extra retail
entry reload and eight-byte frame gap remain, as does the placement of the
shared rotation probe. The improved case-3 control fact should not be
reversed merely to regain transient table offsets; final exactness still
requires their raw addends, code, and data classes to agree.

An isolated switch-layout control moved cases 10, 6, and 40 beside case 4,
immediately before case 11, while leaving their shared rotation-probe call
and argument values intact. The current-source baseline is **95.85052%**
strict text and **39.87603%** table; this grouping fell to **94.14605%**
text and **39.46281%** table. All fifteen sibling functions and the three
initialized data symbols remained exact. Retail places the short case-10 and
case-6 jump arms later than the shared probe, so this grouping also lacks a
retail physical-order basis. It was discarded; the remaining 99/97 CFG and
112/104-byte frame differences still need independent source evidence.

A fresh 16-claim safe GAME delink materialized 1,044 relocations with none
withheld. Direct strict comparison confirms **95.85052%** dispatcher text,
**39.876034%** switch RODATA, and all fifteen siblings exact. Reading all 53
retail switch pointers in physical target order gives cases `7, 2, 3, 0, 13,
51, 52, 4, 11, 5, 9, 8, 10, 6, 12, 1, 43, 42, 44, 45, 40, 39, 49, 50,
34/35/38, 15, 17, 14, 16, 19`, then the default group. This is already
the C switch-arm order. The table retains 31 destination classes, so further
case permutation has no raw basis. Retail and candidate use the same local
stack offsets through `sp+68`; their saved-register areas begin at `sp+84`
and `sp+76` respectively. No retail stack access in the eight-byte frame
gap establishes a missing source object. The retail entry reloads `effect_id`
from its argument home while the candidate keeps `$a0`, which remains an
unattributed codegen difference. No C, identity, or table edit follows.

Case 1 has a separate raw-supported join. Its retail handler at `0x80025ff4`
jumps to `0x80025a7c` with `li $a0,500` in the delay slot. That target is
the case-7 probe argument setup immediately before the shared
`func_80025878` call; case 7 enters with `$a0=1000`. The prior C repeated
both the probe and constructor expressions in case 1. The retained C gives
the two cases one labelled probe call with a case-specific scale and gives
case 1 an explicit jump to that label. The candidate jumps from body
`+0x5c0` to the corresponding probe setup at `+0x60`, also loading 500 in
the delay slot. An off-tree form with a repeated probe call and a jump to
the shared constructor emitted a byte-identical object; the retained form
states the raw shared-probe control directly. Fresh focused CFG moves from
99/97 to **99/98 blocks** with 31/31 branches; isolated strict dispatcher text improves from
**95.85052%** to **96.993126%**. All fifteen sibling functions remain exact,
the three initialized data claims remain 100%, and the 53 switch rows still
have the identical 31-class relation. The shorter case body moves table
addends: all 53 were checked against the fresh target, with eight early
candidate addends four bytes lower and 45 later addends 28 bytes lower;
none is numerically exact. RODATA strict similarity temporarily falls from
**39.876034%** to **22.314049%**; this does not disprove the decoded
control-flow join.
Both objects still have eleven `func_80040308` call relocations, while the
retail dispatcher has one additional local control relocation at this
intermediate stage. The case-1 source correction is retained; the remaining
one-block CFG gap is addressed below. The entry argument-home difference
stays open.

The one remaining block gap came from a probe-call merge. Retail has separate
`func_80025878` calls at `0x80025dd8` for the case-4/10/6/40 rotation path
and `0x80026214` for cases 34/35/38, then shares the later rotation-effect
constructor setup. The first retained case-1 form let GCC fold those two
source-expressed probes into one late call. Spelling the early path's
constructor locally, while leaving the later `emit_rotation_effect` label,
preserves their identical arguments and permits GCC to share only the
constructor. The resulting candidate retains both distinct probes and one
constructor tail, matching the retail **16/16** probe and **11/11**
constructor call-site counts. Focused CFG is now **99/99** blocks and
**31/31** branches; strict dispatcher text rises to **98.02234%**. All
fifteen siblings stay exact. Both objects have 108 dispatcher text
relocations with identical ordered type/symbol pairs. All 53 switch rows
retain their 31-class relation, and every candidate pointer addend is now
exactly four bytes below retail, consistent with the single missing entry
argument-home reload. RODATA similarity returns to **39.876034%**. The
first known successor still differs at the early rotation probe's join,
and the 112/104-byte frame gap remains, so this is a retained source
correction rather than an exact closure.
