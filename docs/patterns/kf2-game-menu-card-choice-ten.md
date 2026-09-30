# GAME menu list and card-choice controllers

These ten GAME functions connect the item-list path to memory-card choices,
card rows, and the two-frame menu panel. Each address received the matcher
address, block disassembly, incoming xref, callee, string, and match queries.
Call and shared-record evidence define this pass; contiguity alone does not
establish an original source-file boundary.

| Address | Retail role and decisive evidence | Final verdict |
| --- | --- | --- |
| `0x8001a4f0` | item/list controller calls the two row selectors and model helpers | WIP, unclaimed; record and model lifetime |
| `0x8001a7fc` | two-frame input/sound redraw helper | strict exact, 100% |
| `0x8001a898` | list/model loop calls the `0x80018dec` selector, model load, and preview renderer | WIP, unclaimed; list record and model lifecycle remain unresolved; selector is exact in a later pass |
| `0x8001aa9c` | card-choice controller calls both choice branches, input poll, window draw, CD stop, and sequence fade | **WIP, 97.305786% strict**; 25 retail versus 24 compiled CFG blocks |
| `0x8001ac80` | card startup and directory/list/panel flow | WIP, unclaimed; card-entry record extent |
| `0x8001af30` | selects fifteen 40-byte card entries into glyph and value streams | strict exact, 100% |
| `0x8001b030` | draws a two-frame card detail panel with six geometry arguments | strict exact, 100% |
| `0x8001b14c` | two-choice card prompt and paired glyph labels | strict exact, 100% |
| `0x8001b2dc` | seven-row choice controller with pad/sound handling and two-frame redraw | WIP, unclaimed; outside-load selection bytes `0x801a8597..0x801a859c` |
| `0x8001b554` | card startup, temporary-file probe, panel, and input flow | WIP, unclaimed; card record and lifecycle state |

The `0x8001aa9c` source reconstructs both choice results, the input-poll
loop, two-frame redraw, and the terminal CD-stop/sequence-volume fade. It
uses two typed `KfMenuGlyphString` labels and the supported
`audio_state.sequence_id` field. The retail 136-byte frame and ordered direct
call set are reproduced. Fifteen decoded `jal`/internal `j` targets and the
HI16/LO16 pair for `audio_state+4` were reviewed in the relocation inventory.
The first remaining code divergence is at the result-reset join: retail
reloads the result through a separate block, while the current compiler
retains it in a register. A source-equivalent label form did not change that
shape and was discarded. The function remains WIP; no volatile carrier,
padding local, or fabricated call was used to force the join.

The four previously exact functions remain strict 100%. The focused and
strict checks for the new controller verified 136/136 GAME target relinks.
The global strict command still exits at known-reference ownership closure
and three unrelated TMD/map-object `.rodata` addends. No repository tests,
banking, or commit were performed for this pass.
