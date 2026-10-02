# GAME actor group call topology

The complete retail GAME `func_8003c614` body at `0x8003c614..0x8003d083`
has eleven direct calls to `func_8003c3e0` and nine to the variadic
`func_80040308`. A fresh one-unit safe delink has no withheld relocations.
The current C object has seven and ten call sites respectively; direct strict
text similarity is 84.98203%. All 123 switch rows preserve the same partition
into 19 destination groups in retail and C. Their pointer addends differ
because the handler code has a different layout. This is a call-topology
gap, not evidence for editing table rows.

Retail keeps distinct position-helper calls at function offsets `+0x3ec`,
`+0x4a4`, `+0x568`, and `+0x5a4`; the current compiler merges several
source cases into one helper call. The simple-direction paths jump to a
constructor at `+0x6c0`, physically after the first kind-`0x78` constructor
at `+0x6a8`. Kind `0x78` also reaches that later constructor, while the
current object emits separate calls for the direction tail and second random
position. The call targets and argument constants at these divergent paths
were checked against the disassembly before changing source shape.

Isolated C controls did not recover those counts. Giving each of five
direction cases a branch-local constructor lowered strict similarity to
81.65–82.51%; changing all five lowered it to 81.992516%, with eight
position-helper and eleven constructor calls. Moving the shared direction
tail after kind `0x78` raised strict similarity to 85.196106% but left the
seven/ten call counts unchanged. None of these controls was retained. The
source still expresses the observed case actions, and the original source
or compiler mechanism behind the duplicated retail helper calls remains
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
