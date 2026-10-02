# GAME event focused nine

Nine GAME event/phase units were rebuilt individually off-tree with their
pinned manifest profiles and compared by isolated strict objdiff against the
existing safe-delink target objects. Their `.rel.text` target/candidate row
counts agree in every unit. This screen retained no source, data, relocation,
or profile change.

Fourteen functions are already strict **100% exact**: the two pose
interpolators at `0x80045f20` and `0x80045fd4`; map-object spawn
`0x80046700`; command dispatcher `0x8004678c`; the three counters at
`0x800473e0`, `0x80047434`, and `0x800474c4`; all six event-state functions
at `0x800482f8..0x800484e4`; and save stream `0x80048554`. Their unit text
relocations agree respectively 12/12, 3/3, 218/218, 26/26, 30/30, and
30/30.

| Function | Fresh strict text | `.rel.text`, target/candidate | Verdict |
| --- | ---: | ---: | --- |
| `0x800460a0` animation phase seek | 99.268295% | 4/4 | WIP. Prior raw audit confirms the phase/half-step fields and six-block control; its two saved-register roles remain exchanged. |
| `0x80047c98` world event dispatcher | 99.81618% | 72/72 | WIP. Prior raw audit confirms 85/85 blocks, 55/55 branches, direct calls, and a still-indirect callback; the rotation argument and constant-one saved registers differ. |
| `0x800489ac` restore decoder | 98.82883% | 24/24 | WIP. Prior raw audit confirms the 23/23 block and 7/7 branch structure with the reviewed 64-byte jump table; actor-base and sentinel registers differ. |

The current exact command dispatcher supersedes older partial-match notes.
The three WIPs have no newly evidenced wrong field, call, referent, or branch
that supports altering their C sources. No exact result was newly closed.
The dispatcher's five eight-byte initialized DATA claims at
`0x800679a0..0x800679c7` are individually 100% byte-exact in this object
comparison; event-state and counter BSS symbols have no initialized-byte
score.
