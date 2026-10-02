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
