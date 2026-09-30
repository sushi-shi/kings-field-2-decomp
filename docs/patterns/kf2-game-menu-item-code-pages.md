# GAME menu item-code lookup pages

This 25-function GAME pass follows the item-list controllers through their
indexed code translator and onward to the menu text and sprite renderers.
Each address received the matcher address, block disassembly/CFG, incoming
xrefs, callees, strings, and match-state queries. Shared calls and table
references define the campaign; the address range is not a TU claim.

| GAME address | Role or decisive remaining question | Verdict |
| --- | --- | --- |
| `0x8001d030` | item-list controller calling the primary translator | WIP: list state and `0x80065950` data |
| `0x8001d340` | primary byte/code translator | WIP: 93.10345% strict code; 1440/1440 data exact |
| `0x8001d3b4` | paired item-list controller | WIP: list state and second lookup owner |
| `0x8001d654` | secondary indexed halfword lookup | WIP: see the later [menu list and numeric pages](kf2-game-menu-list-pages-ten.md) audit |
| `0x8001d6a8` | item-list/model renderer | WIP: model and row state |
| `0x8001d8d0` | related list/model renderer and primary translator caller | WIP: record/state model |
| `0x8001dc64` | display, window, input, and item-list controller | WIP: menu lifecycle |
| `0x8001ddd0` | item-list controller and primary translator caller | WIP: list state |
| `0x8001e0a8` | paired item-list controller | WIP: list state |
| `0x8001e378` | menu input poll | **exact**, existing source |
| `0x8001e484` | menu/model input controller | WIP: 66-block state flow |
| `0x8001e94c` | numeric text renderer | WIP: glyph/data owner |
| `0x8001f008` | second numeric text renderer | WIP: glyph/data owner |
| `0x8001f798` | six-row paired label renderer | **exact**, existing source |
| `0x8001f8b8` | menu model/preview controller | WIP: list/model record |
| `0x8001fb8c` | window title and highlighted rows | WIP: 99.78788%, frame extent |
| `0x8001fc94` | numeric/list renderer | WIP: record/data owner |
| `0x80020748` | two-option widget | **exact**, existing source |
| `0x8002083c` | item-model preview | WIP: 99.65882%, frame extent |
| `0x80020990` | numeric heading | **exact**, existing source |
| `0x80020b50` | translucent sprite packet | **exact**, existing source |
| `0x80020d20` | cursor sprite packet | WIP: 99.44068%, X computation order |
| `0x80020ef8` | fixed-CLUT sprite packet | WIP: 99.39449%, X computation order |
| `0x800210ac` | glyph-string packets | WIP: 99.66904%, frame/register choice |
| `0x80021510` | number-glyph packets | WIP: 98.61957%, atlas-U load/order |

`0x8001d340` computes a group page stride of 240 bytes and uses a byte index
to fetch a halfword code. The next distinct table base at `0x800661c0`
bounds six 240-byte pages beginning at `0x80065c20`. The source owns this
entire `u16[6][120]` initialized object; strict objdiff confirms all 1,440
data bytes. The function's loop, widths, CFG, and referent agree with retail.
Its first code divergence is the final page-offset shift emitted immediately
before the table-base load rather than immediately after it. A base-pointer
expression experiment moved the load before the function's entry guard and
lost more correspondence, so the natural indexed source is retained WIP.

The secondary translator at `0x8001d654` uses the next table base and the
same 240-byte page stride. A later [byte and reference audit](kf2-game-menu-list-pages-ten.md)
established its five-page, 1,200-byte extent, replaced the false Ghidra string
fragments with one initialized table, and reconstructed the translator as
90.47619% strict WIP. No surveyed function was attributed to vendored code.

Retail census validation, GAME target regeneration, and focused compilation
passed. Strict objdiff scored the primary function at 93.10345% and its data
at 100%; the GAME target relink verified 156/156 units. The global strict
command still exits at known-reference closure and two unrelated map-object
`.rodata` addend divergences.
