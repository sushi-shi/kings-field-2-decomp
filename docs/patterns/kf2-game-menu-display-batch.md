# GAME menu and display follow-up batch

This 25-function GAME survey follows proven menu frame, sprite, text, input,
memory-card, and fade calls. For each address, the GAME semantic address,
disassembly/CFG, callers, callees, strings, and match state were inspected.
The menu controllers form a call-connected group; the three display functions
are connected through the transition and graphics runtime. This is a campaign
boundary, not proof of original translation-unit boundaries.

| GAME address | Retail role and decisive evidence | Verdict |
| --- | --- | --- |
| `0x8001930c` | archive-backed menu setup; archive, TIM upload, frame and primitive calls | unclaimed; resource lifetime and large stack record WIP |
| `0x80019834` | menu list/input loop; calls `0x800199d0`, list init, frame renderer | unclaimed; list output records WIP |
| `0x800199d0` | selects 26-byte entries and copies 24-byte glyph rows from `0x80065770` | unclaimed; source table extent WIP |
| `0x80019ac4` | menu controller; references `0x80064910` and calls `0x80019ce4` | unclaimed; record/table ownership WIP |
| `0x80019ce4` | menu row and label preparation around `0x80065770` | unclaimed; source table extent WIP |
| `0x80019ed4` | larger selection controller with internal dispatch | unclaimed; indirect target and state model WIP |
| `0x8001a2f4` | list/input controller using `0x800199d0` | unclaimed; copied-row ownership WIP |
| `0x8001a4f0` | list/input controller using `0x800199d0` and menu rendering | unclaimed; copied-row ownership WIP |
| `0x8001a7fc` | input poll, sound cue 18, two redraws, then release wait | **exact, 156/156 bytes** |
| `0x8001a898` | menu list and frame controller | unclaimed; stack list and state WIP |
| `0x8001aa9c` | menu controller calling the card panel and input poll | unclaimed; caller state WIP |
| `0x8001ac80` | memory-card menu flow; card startup, panel, and row builder | unclaimed; card record model WIP |
| `0x8001af30` | probes 15 card entries at 40-byte stride and writes glyph rows | **exact, 256/256 bytes**; card-entry type remains WIP |
| `0x8001b030` | two-frame card panel, font rows, six detail arguments | **exact, 284/284 bytes** |
| `0x8001b14c` | input and frame controller using menu cursor state | unclaimed; caller state WIP |
| `0x8001b2dc` | input and frame controller with button animation state | unclaimed; caller state WIP |
| `0x8001b554` | memory-card menu flow and conditional panel draws | unclaimed; card record model WIP |
| `0x8001b834` | card-entry scan, menu list, and panel draws | unclaimed; card-entry struct WIP |
| `0x8001ba80` | builds three menu glyph labels from constants and `0x80064b54` | unclaimed; 20-byte label suffix owner WIP |
| `0x8001bb94` | builds four menu glyph labels from `0x80064b54` and `0x80064b18` | unclaimed; label suffix owners WIP |
| `0x8001bcfc` | memory-card menu flow, card probe, and panel renderer | unclaimed; card record model WIP |
| `0x8001bf68` | memory-card temporary-file and pad/input controller | unclaimed; state transition model WIP |
| `0x80034644` | pool reset called by `display_reset` | **already exact**, `game.pool` |
| `0x800349bc` | four textured fade quads; frame advance and pad release/press | WIP, 91.981950% fuzzy |
| `0x80034e10` | archive/TIM image transition and VRAM snapshot | WIP, 99.114586% fuzzy |

The three new exact C sources are `menu_simple_loop.c`, `menu_card_rows.c`,
and `menu_card_panel.c`. The first matches a 32-byte stack frame, direct input
calls, the two-frame loop, and both internal jumps. Its nine control-flow
relocations were reviewed. The row builder advances 15 card entries at a
40-byte stride, appends 20-byte glyph rows for successful probes, and writes
parallel value, slot, and icon streams. Its direct call relocation was
reviewed, while the input card-entry type is still an unresolved record.
The card panel uses the typed glyph string and
sprite tables: its title is `menu_window_layouts[1].rows[panel]`, and the
font and translucent sprite are descriptors 1 and 5. It forwards six detail
arguments to `0x800217f0`. Its ten direct call/data relocations were
reviewed. `kf try` emitted identical instruction listings for all three; focused
`kf match` reported 100% and the target relink verified 137/137 GAME units.
The full command still exits at the repository-wide known-reference data
ownership closure (361 config-only data ranges), not at these source units.

The fade source reproduces the four quad dimensions, texture coordinates,
CLUT/tpage settings, shade calculations, primitive insertion, frame calls,
and pad state logic. Its first real code differences are stack/register
allocation and instruction ordering around the fade state and repeated quad
setup. No codegen mechanism is attributed. `0x80034e10` retains its earlier
two buffer-boundary register-choice residues. Neither WIP score is treated as
closure. No function in this batch was identified as vendored.

The next panel-detail renderer at `0x800217f0` proves that the sprite
descriptor array continues beyond the first seven records. It starts a
nine-slice draw at `0x80063f04`, descriptor 11 of the 12-byte table rooted at
`0x80063e80`. Nine descriptors end at `0x80063f70`, exactly where
`menu_window_layouts` begins. The initialized bytes between those boundaries
form 20 aligned sprite descriptors, so the curated `menu_sprite_defs` extent
is now `0xf0` bytes. Earlier Ghidra seed fragments inside the array are
interior fields, not separate data owners.
