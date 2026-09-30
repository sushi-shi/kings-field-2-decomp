# GAME menu glyph-row selectors

This 25-function GAME pass follows proven calls from the early item/list
controllers into two glyph-row selectors and their menu rendering siblings.
For each address, the matcher address, block disassembly, incoming xrefs,
callees, strings, and match state were inspected. A call relationship is the
campaign boundary; it does not prove one original source file.

| GAME address | Evidence and remaining owner question | Verdict |
| --- | --- | --- |
| `0x8001876c` | top menu dispatch calls frame/window and item/card flows | WIP: state and indirect dispatch |
| `0x800189f0` | positioned player value glyphs | **exact**, existing source |
| `0x80018ac8` | item-list input and model lifecycle | WIP: controller state |
| `0x80018d08` | filters 24-byte glyph rows by nonzero byte mask | **exact, 228/228 code bytes**, new |
| `0x80018dec` | related mask selector checks equipped item IDs and one unresolved player byte | **exact** in a later contiguous unit pass |
| `0x80018f8c` | player equipment/stat menu action | WIP: player transition state |
| `0x80019240` | player value clamp | **exact**, existing source |
| `0x800192ac` | companion clamp leaf | **exact**, existing source |
| `0x800192dc` | companion clamp leaf | **exact**, existing source |
| `0x8001930c` | archive/TIM menu setup | WIP: resource lifetime and stack record |
| `0x800199d0` | selects 26-byte records into the second glyph table | **exact, 244/244 code bytes**, new |
| `0x80019ce4` | ten-row glyph builder using both tables | **exact in later follow-up**; typed player-state selection bytes |
| `0x80019ed4` | selection controller with menu calls and indirect dispatch | WIP: dispatch/state model |
| `0x8001a4f0` | list/input controller calling both selectors | WIP: list/model lifetime |
| `0x8001a7fc` | menu loop helper | **exact**, existing source |
| `0x8001a898` | list/frame controller calling the exclusion selector | WIP: list/model state |
| `0x8001af30` | card glyph-row collector | **exact**, existing source |
| `0x8001b030` | card panel entry | **exact**, existing source |
| `0x8001b14c` | adjacent card panel renderer | **exact**, existing source |
| `0x8001d030` | item-list controller with another indexed data base | WIP: `0x80065950` data owner |
| `0x8001d3b4` | paired item-list controller | WIP: list record and data owner |
| `0x8001d6a8` | list and item-model renderer | WIP: menu/model state |
| `0x8001d8d0` | related list/model renderer | WIP: menu/model state |
| `0x8001ddd0` | item-list controller | WIP: list record and table ownership |
| `0x8001e0a8` | paired item-list controller | WIP: list record and table ownership |

The initialized range at `0x80064c30..0x80065950` contains 140 consecutive
24-byte `KfMenuGlyphRow` entries. Retail instructions address its first 120
rows from `0x80064c30` and the final 20 from a separate base at `0x80065770`.
The source defines those as two adjacent arrays, and strict objdiff verifies
their entire 3,360-byte contents. The next byte at `0x80065950` begins a
different unresolved data range. Retail table-base HI16/LO16 pairs in the
two selector functions and `0x80019ce4` now name their reviewed owners.

`0x80018d08` copies a glyph row for each nonzero input byte, writes the byte
and original index to parallel outputs, and returns the count. Its five-block
CFG, 24-byte copy, call set, and ordered table referent match. The related
`0x800199d0` accepts 26-byte records: byte zero is selected only when equal
to one, and the unsigned halfword at offset `0x16` is widened into a 32-bit
output slot. The `KfMenuSelectionRecord` layout keeps the 21 interior and two
trailing bytes opaque. Its six-block CFG and output order are exact.

Later retail and type review resolved `0x80018dec`'s exclusion address:
`0x8019856b` is `player_state + 0x9b`, the equipped weapon ID. Its eight
neighbor checks are the seven equipped armor/accessory IDs and
`player_state.unknown_99`. A typed source body now matches strict 100% beside
exact `0x80018d08`; the two functions and 3,360-byte table remain exact.
The later [menu list selection pass](kf2-game-menu-list-selection-ten.md)
established that `0x80019ce4` reads these typed player-state bytes and matches
strict 100%. No surveyed function was classified as vendored.
