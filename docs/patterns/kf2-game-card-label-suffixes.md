# GAME card-menu labels and glyph suffixes

This 25-function GAME pass follows proven calls through memory-card dialogs,
glyph label builders, and item-list screens from `0x8001b2dc` to
`0x8001dc64`. Each address received the six matcher evidence queries:
address, block disassembly, incoming xrefs, callees, strings, and current
match. The linked call group defines the survey; address adjacency proves only
the contiguous source runs stated below. No surveyed function was identified
as vendored code.

| Address | Retail role and current owner question | Verdict |
| --- | --- | --- |
| `0x8001b2dc` | card choice/input loop with frame and sound calls | **exact** in the later card-panel reconstruction |
| `0x8001b554` | card startup, temporary-file probe, panel, and input calls | WIP: card record and lifecycle |
| `0x8001b834` | card-directory scan, row builder, and list controller | **exact** with the 40-byte SDK `DIRENTRY` input |
| `0x8001ba80` | three positioned card labels, one shared suffix | **exact, 276/276 bytes** |
| `0x8001bb94` | four positioned card labels, two shared suffixes | **exact, 360/360 bytes** |
| `0x8001bcfc` | card startup, directory enumeration, list, and panel calls | **exact** with the card record model |
| `0x8001bf68` | temporary-file/card-format and dialog controller | WIP: state transition flow |
| `0x8001c12c` | dialog renderer and input controller | **exact** with the dialog stack record |
| `0x8001c550` | two glyph rows from suffix entries 0 and 1 | **exact, 220/220 bytes** |
| `0x8001c62c` | three glyph rows from suffix entries 2–4 | **exact, 324/324 bytes** |
| `0x8001c770` | three glyph rows with alternate horizontal positions | **exact, 320/320 bytes** |
| `0x8001c8b0` | three glyph rows for another menu state | **exact, 324/324 bytes** |
| `0x8001c9f4` | two glyph rows with one changed glyph code | **exact, 224/224 bytes** |
| `0x8001cad4` | one positioned glyph row from suffix entry 13 | **exact, 112/112 bytes** |
| `0x8001cb44` | conditional first label and two fixed labels | **exact, 400/400 bytes** |
| `0x8001ccd4` | two glyph rows from suffix entries 14 and 15 | **exact, 220/220 bytes** |
| `0x8001cdb0` | two-frame title and variable-row renderer | **exact, 264/264 bytes** |
| `0x8001ceb8` | display entry, window, input, and item-list controller | **exact, 376/376 bytes** |
| `0x8001d030` | item list, model load, input, and code translator | WIP: record and table owner |
| `0x8001d340` | indexed code/name translator | WIP: `0x80065c20` 240-byte row table extent |
| `0x8001d3b4` | second item-list controller | WIP: record and table owner |
| `0x8001d654` | second indexed code/name translator | WIP: `0x800661c0` 240-byte row table extent |
| `0x8001d6a8` | menu model/list renderer with item preview | WIP: list/model state |
| `0x8001d8d0` | menu model/list renderer and translator | WIP: list/model state |
| `0x8001dc64` | display entry, window, input, and item-list controller | WIP: caller state and menu lifecycle |

The source-owned `menu_label_suffixes` object at `0x80064af0` is sixteen
contiguous 20-byte initialized glyph suffixes, ending at `0x80064c30`.
Retail bytes, ordered pointer references, and the `0x8001ccd4` use of the
sixteenth entry establish its extent. `0x80064c30` begins a distinct indexed
glyph-row table referenced by earlier list selectors. The new
`KfMenuLabelSuffix` type preserves the 20-byte copy; the existing
`KfMenuGlyphString` supplies the positioned 28-byte destination. Eighty
four-byte Ghidra seed fragments were replaced by one supported data owner,
and each used interior HI16/LO16 pair was reviewed.

The eight contiguous label builders and adjacent `0x8001cdb0` renderer and
`0x8001ceb8` controller are one `game.menu_label_templates` unit. A fresh
focused `kf try` reported 10/10 SAME; direct objdiff reports 2784/2784
code bytes and 320/320 data bytes. The two
contiguous card-label builders form `game.menu_card_labels`, with 2/2 SAME
and strict 636/636 code bytes. The most recent strict run verified 152/152
GAME target relinks. Its global exit remains at incomplete known-reference
data ownership and two unrelated map-object `.rodata` relocation-addend
divergences; neither affects these menu units.

The later card-row reconstruction also makes `0x8001af30` and its load
browser caller `0x8001ac80` exact. The row helper now accepts the pinned
40-byte SDK `DIRENTRY` type and passes `entry->name` to the card-header
reader; its three browser callers pass their typed entry arrays directly.
The checked 20-byte card glyph row remains an output-storage view because
changing the exact helper's s16 cursor to a row pointer changed its frame and
instruction schedule. Eight card label-suffix copies now use typed 20-byte
`memcpy` calls, retaining the exact label and dialog listings.

The adjacent GAME SDK boundary is excluded from game-source progress:
`0x80049ee0` is the complete pinned `CARD.OBJ` `_card_clear` body (46/52
fixed bytes, two archive XREFs), while `0x80049f74` and `0x80049f94` are
`CdReset` and `CdSync` inside the exact `LIBCD.LIB/SYS` text section. The
single zero word at `0x80049f70` separates those archive objects.
