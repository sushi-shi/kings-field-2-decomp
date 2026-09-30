# GAME menu, graphics, and card linkage batch

This 25-function GAME survey follows the menu numeric/model draws through
their TMD, asset, animation, and primitive-pool helpers, and the card
write/read calls to their payload transforms. Each address was inspected with
the retail extent, CFG/disassembly, callers, direct calls, strings, and match
state. These links justify the survey; they do not prove shared original TU
ownership. Existing asset, animation, pool, and TMD C units belong to their
respective campaigns and were not edited here.

| GAME address | Retail role and decisive evidence | Verdict |
| --- | --- | --- |
| `0x8002083c` | menu item-model preview; RotMatrix, light/color/rotation/translation matrices, TMD draw | unclaimed; exact stack matrix layout and owner WIP |
| `0x80020990` | draws one or two numeric menu headings and counters with sprite, text and number helpers | **exact, 448/448 bytes** |
| `0x80022300` | routes selection sounds 13 and 16–18 through audio/sequence and VSync calls | WIP, **89.972980%**; 7 retail versus 6 compiled CFG blocks |
| `0x80022394` | reads pad and marks active input | already exact |
| `0x800223cc` | waits through an idle-input release period | already exact |
| `0x80022438` | waits until pad input is released | already exact |
| `0x8002d8b0` | registers item-model TMD in graphics runtime slot | already exact |
| `0x800339fc` | walks and registers a TMD archive | already exact |
| `0x80033ab4` | sets selected asset | already exact |
| `0x80033afc` | selects asset record | already exact |
| `0x80033b34` | selects a clip keyframe and blend fraction | already exact |
| `0x80033bfc` | expands sparse vertex stream against a base | already exact |
| `0x80033cc0` | decodes sparse vertex positions in place | already exact |
| `0x80033d3c` | adjusts model vertex coordinates and calls ScaleMatrix | unclaimed; animation object/vertex ownership WIP |
| `0x80033ff4` | finds a vertex in a sparse stream | already exact |
| `0x80034070` | binds an animated TMD object, pool allocation and decoded vertices | unclaimed; object lifetime and animation state WIP |
| `0x80034344` | resolves resources and sparse animation keyframes for a vertex | unclaimed; asset/animation record ownership WIP |
| `0x800345e4` | returns asset vertex count | already exact |
| `0x80034674` | marks primitive pool allocation | already exact |
| `0x800346b0` | releases one primitive pool record | already exact |
| `0x800346f8` | releases all primitive pool records; menu display entry calls it | already exact |
| `0x80034764` | releases stale pool records | already exact |
| `0x800347d0` | allocates a primitive pool record | already exact |
| `0x80048d24` | copies graphics/player state into card-save payload, called by `0x80022ca0` | unclaimed; 0x5b8-byte copy schema and aggregate owners WIP |
| `0x800492dc` | restores graphics/player state from card-save payload, called by `0x80022b74` | unclaimed; 0x5e0-byte copy schema and aggregate owners WIP |

The numeric overlay, now in the contiguous `menu_two_option.c` unit, reproduces the retail 104-byte stack frame,
two 28-byte glyph strings, both sprite descriptors, all three CFG branches,
and the ordered calls. Its first counter selects a byte for kind 3 or the
player's 32-bit gold field otherwise; the second counter uses the menu item
count. The byte at `0x8009a648` is `game_counter_bytes[0x60]` inside the
reviewed 0x78-byte BSS array. The pair `lui 0x800a; lbu -22968` resolves to
that interior address through signed LO16 carry semantics. A previous `0x800aa648`
guess was rejected by the delinker's `decoded-target-mismatch` check and was
corrected before the 100% result. Two reviewed relocation pairs at
`0x80020a5c` and `0x80020a84` complete the exact source comparison.

`0x80022300` still has a structural CFG difference: retail branches away
from cue 16 and uses a separate internal jump to the common sound block,
while the current source compiles to one direct taken branch. An explicit
label was tested and compiled identically to the current humane C, so the
source retains the simpler shared behavior. The asset/animation helpers and
the card payload transforms are classified from confirmed calls and raw
field copies; their unsourced record layouts are not inferred from proximity.

Focused `kf match` reports `0x80020990` strict 100% and GAME target relink
171/171. `kf-retail-validate` passes. The match command still exits at the
repository-wide known-reference data-ownership closure. The parent campaign
owner will run the full build and repository tests.
