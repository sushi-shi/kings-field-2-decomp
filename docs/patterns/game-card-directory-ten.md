# GAME card-directory and dialog ten

This batch follows confirmed calls from item/card menus into directory enumeration,
card-header decoding, and save writing. Each GAME function was checked with the
matcher address, block disassembly, incoming references, callees, strings, and
match-state queries. None has supported vendored attribution.

| Address | Retail role | Final verdict |
| --- | --- | --- |
| `0x8001a4f0` | item-list controller using row selection and model loading | WIP, unclaimed; list record and model lifetime unresolved |
| `0x8001a898` | paired list/model loop and exclusion selector | WIP, unclaimed; selector state and list record unresolved |
| `0x8001ac80` | card startup, directory scan, list, and preview panel | WIP, unclaimed; caller's entry-array lifecycle unresolved |
| `0x8001b2dc` | card choice/input loop with sound and frame calls | WIP, unclaimed; menu selection state unresolved |
| `0x8001b554` | card startup, temporary-file probe, panels, and input | WIP, unclaimed; directory record and lifecycle unresolved |
| `0x8001b834` | card-entry row selector, panel, and save reader | WIP, unclaimed; complete caller stack record unresolved |
| `0x8001bcfc` | card startup, probe, enumeration, format flow, and cleanup | WIP, unclaimed; list-record lifetime unresolved |
| `0x800226ec` | enumerates and sorts fifteen SDK `DIRENTRY` records | WIP, **93.60504% strict**; instruction scheduling and first-loop prelude differ |
| `0x800228c8` | reads a 0x280-byte card header, decodes EXP/LV and slot ID | WIP, **85.15625% strict**; byte loads and loop scheduling differ |
| `0x80022ca0` | creates the card save, title, icon, and payload | WIP, unclaimed; full card-header and payload ownership unresolved |

The two reconstructed helpers are contiguous in `game.memory_card_directory`.
The first uses the pinned SDK's 40-byte `DIRENTRY`, counts matching names, and
sorts by the filename's slot digit. The second passes three 32-bit output
pointers: six Shift-JIS title digits at +0x2c encode EXP, two at +0x3e encode
LV, and filename byte +12 encodes the save-slot ID. The exact caller at
`0x8001af30` now uses a word scratch for LV and copies its low byte into the
menu output, preserving its listing.

The helpers were merged with contiguous `0x80022b48` and `0x80022b74` so the
single `bu00:` literal at `0x80011120` belongs to one unit. Strict comparison
preserves `0x80022b48` at **100%**, `0x80022b74` at its prior **93.666664%**
WIP score, the six-byte read-only literal, and the 16-byte file-prefix datum
at **100%**. The final GAME target relink verified **142/142** units. The
global strict command still exits on the three pre-existing TMD/map-object
read-only addends and incomplete known-reference ownership.

The linked load-image bytes at `0x8006d6a4/5` are `0x20,0x00`; `0x8006d6a8`
holds `bu00:*`. They remain external address-derived identities. A provisional
definition beside `memory_card_loaded_slot` at `0x8006d6a0` compiled with
offsets +1/+2/+4, while retail requires +4/+5/+8. The three-byte boundary gap
is evidence of a separate contribution, not a reason to insert source padding
or claim a data-only owner. That provisional definition was reverted, and the
event unit's original one-byte data object was rebuilt. The full integration
build still reports these unresolved card symbols along with its other known
unresolved links. Repository tests, banking, and commits were not run.
