# GAME menu item and event caller pilot

This batch fixes ten previously non-exact or unclaimed GAME functions selected
from the menu item, player action, card-label, and event caller graph. The
addresses were checked against current worker ownership before source edits.
Each function received the retail address/CFG, caller/callee, string, data
reference, relocation, adjacent-function, source-history, and match-state
pass. The KF1 item menu in `../kings-field/src/game/menu.c` supplied a source
shape control, not a substitute for KF2 instruction evidence.

| GAME address | Final verdict | Evidence and remaining work |
| --- | --- | --- |
| `0x80018ac8` | WIP, unclaimed | The 0x240-byte item-menu controller calls the exact glyph selector and list initialization, model load/release, renderer, input, and archive-image viewer. Its 56-byte stack list record exceeds the currently proved 36-byte shared prefix; pointer fields at +36/+44 need an owned complete type. |
| `0x80018dec` | **100% strict** | Appended to the contiguous glyph-row unit. It copies 24-byte glyph rows and decrements equipped-item quantities through typed `player_state` fields. The retail reference at `0x8019856b` is player state +0x9b. |
| `0x80018f8c` | WIP, **96.66474% strict** | Appended to the same unit. Item IDs 71–80 adjust player HP/MP, base magic, two unknown signed fields, and event counters, then cap vitals and play cue 13. The source preserves retail's independent 79/80 branch stage. Focused comparison has a four-byte longer body and differing case-75/76 branch layout; no fabricated padding or assembly was added. |
| `0x80023178` | WIP, **93.52941% strict** | Existing card-label writer retains its two Shift-JIS decimal loops. Focused CFG is 19/19 blocks and 10/10 branches; initial register allocation and loop schedule differ. Three adjacent functions remain listing-identical. |
| `0x80025a18` | WIP, unclaimed | The 0x918-byte player/effect dispatcher has a broad switch, 99 CFG blocks, exact player/effect callees, and an indirect branch through candidate table `0x80011188`. Complete table and player-state ownership remain open. |
| `0x8002665c` | WIP, unclaimed | The 0xbd0-byte action controller reaches actor collision, animated-vertex, audio, and effect helpers. Its complete motion/action state structure remains unproved. |
| `0x8002722c` | WIP, unclaimed | Direct callee of the menu item controller; it reads magic cost and player MP, then reaches unresolved indirect control through candidate tables `0x80011298` and `0x800112b0`. |
| `0x800274ec` | WIP, unclaimed | The 0x43c-byte player motion loop uses `rsin`, `rcos`, collision queries, and vector angle; the surrounding collision-cache ownership remains provisional. |
| `0x800460a0` | WIP, **99.268295% strict** | Existing animation phase helper has matching six-block CFG, calls and referents. The remaining difference exchanges saved registers for step and half-step. Source-order and width experiments did not improve it; the existing source is preserved. |
| `0x800462bc` | WIP, unclaimed | The 0x444-byte event dispatcher calls the animation phase helper and menu/display functions. Its candidate 16-entry table at `0x80012890` and indirect record callback require ownership before a C body can be claimed. |

`game.menu_glyph_rows` retains strict 100% for both `0x80018d08` and
`0x80018dec`, and its initialized glyph data is 3360/3360 bytes exact. The
strict pass relinked 142/142 GAME units. Global closure remains blocked by
three unrelated `.rodata` addend differences in the TMD and map-object units.
No function in this batch was banked.
