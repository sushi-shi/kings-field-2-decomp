# GAME actor group call topology

The complete retail GAME `func_8003c614` body at `0x8003c614..0x8003d083`
has eleven direct calls to `func_8003c3e0` and nine to the variadic
`func_80040308`. A fresh one-unit safe delink has no withheld relocations.
The current C object has seven and ten call sites respectively; direct strict
text similarity is 85.93563%. All 123 switch rows preserve the same partition
into 19 destination groups in retail and C. Their pointer addends differ
because the handler code has a different layout. This is a call-topology
gap, not evidence for editing table rows.

Retail's 192-byte frame separates the blend-mode target at `sp+80` from the
trajectory-loop target at `sp+136`. It reuses `sp+120` for angle and motion
payloads in mutually exclusive switch arms. The earlier C reused one target
vector for both purposes and kept angle and motion payloads separate, yielding
a 184-byte frame. Distinct target variables and an angle/motion union now
model those actual uses. Ordering the local objects to their retail stack
slots also aligns position `+48`, offset `+64`, predicted position `+96`,
direction `+112`, rotated input `+128`, travel time `+152`, and trajectory
angle `+156`. A focused quick build improved listing similarity from 55.3%
to 67.4%; isolated strict text improved from 84.98203% to 85.93563%.
Target and candidate still have 123 ordered `.rodata` relocations and the
same 19-class table partition. The remaining helper/constructor call-site
counts are 11/7 and 9/10. These stack facts support the current C object
layout, but do not prove the original source's exact local declarations.

Retail keeps distinct position-helper calls at function offsets `+0x3ec`,
`+0x4a4`, `+0x568`, and `+0x5a4`; the current compiler merges several
source cases into one helper call. The simple-direction paths jump to a
constructor at `+0x6c0`, physically after the first kind-`0x78` constructor
at `+0x6a8`. Kind `0x78` also reaches that later constructor, while the
current object emits separate calls for the direction tail and second random
position. The call targets and argument constants at these divergent paths
were checked against the disassembly before changing source shape.

Earlier isolated C controls did not recover those counts. Giving each of five
direction cases a branch-local constructor lowered strict similarity to
81.65–82.51%; changing all five lowered it to 81.992516%, with eight
position-helper and eleven constructor calls. An earlier move of the shared
direction tail after kind `0x78`, before the stack correction, reached
85.196106% but was discarded. The same tail placement after the corrected
locals reaches 85.93563% and is retained: it matches the physical retail
neighborhood without changing any case's arguments. The compiler still emits
the second kind-`0x78` constructor separately, so the original source or
compiler mechanism behind the duplicated retail helper calls remains
unattributed.

An isolated GCC 2.5.7 `-fno-cse-skip-blocks` object produced the same
84.98203% strict comparison. This compiler rejected `-fno-crossjumping` as
an invalid option, so that flag provides no controlled profile comparison.
The pinned GCC 2.6.0 probe lowered strict text similarity to 60.167664%; it
does not explain the duplicated retail call sites.

The related `func_80039108` target scorer has the same kind of evidence
limit. Retail keeps separate type-9 and type-11 angle/tolerance calls,
whereas the current compiler merges their identical argument path. Its
131-row switch table preserves all ten target classes at all indices;
offset differences track code layout. No table addend or field was changed
to compensate for this source/codegen residue.
