# GAME actor-group effect call-tail audit

GAME `func_8003c614` is a 2,672-byte switch dispatcher. A fresh one-VA safe
delink materialized 305 relocations with none withheld. A focused quick build
has 45/45 CFG blocks and 15/15 branches. Direct isolated strict comparison
of the retained source is **86.041916%** text and **37.906506%** for its
492-byte switch table. The table's 123 rows retain the retail's 19 target
classes, so case identities are not the source of this call-layout gap.

Retail has eleven distinct `func_8003c3e0` calls at body offsets `+0x204`,
`+0x264`, `+0x30c`, `+0x3ec`, `+0x430`, `+0x4a4`, `+0x568`, `+0x5a4`,
`+0x718`, `+0x83c`, and `+0x9f8`; the current compiler emits seven.
Retail has nine `func_80040308` calls and the probe has ten. In retail,
kind `0x78` calls the constructor at `+0x6a8`, leaves zero in its fifth
outgoing argument slot, then falls through to the constructor at `+0x6c0`.
The latter site is also reached by seven directional kinds after they set
their own direction and optional argument slots. The source expresses two
five-argument kind-`0x78` calls and one shared eight-argument directional
call. This preserves the meaningful arguments; the configured compiler does
not merge the different-arity calls as retail did.

Two off-tree source-equivalent call-layout controls were negative. Giving
each directional case its own constructor expression left the physical
counts at seven direction helpers and ten constructors while lowering strict
text to **82.49701%** and table similarity to **37.29675%**. A unified
constructor expression for kind `0x78` and the directional cases yielded
nine constructor sites and **86.687126%** text / **38.922764%** table, but
only by passing extra `0x400, 1` variadic arguments on kind `0x78`. The
constructor's kind-120 arm does not read those words, and no retail source
semantics supports adding them. That variant was discarded despite its
higher score. Both trials still emitted only seven direction-helper sites.

Kind `0x79` has a separate raw layout residue. Retail's `bgez` at body
`+0x2cc` reaches the local jump at `+0x2d8`; the negative path first sets
the travel time to zero. The jump writes that value to outgoing argument
slot `sp+20` in its delay slot before reaching the shared constructor at
`+0x6b4`. The probe instead spills the value to a local slot and branches
directly to its common call. Spelling an explicit positive branch and local
label in off-tree C emitted the baseline object byte-for-byte. A branch-local
constructor lowered strict text to **84.562874%** while raising table
similarity to **40.955284%**. Neither variant established a different
condition, argument, or source operation, so the tracked C remains intact.

These results are bounded to focused compilation and isolated safe-target
objdiff. They do not identify the original compiler or justify a fake local,
volatile argument carrier, or altered constructor arity.
