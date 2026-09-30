# GAME menu screen pilot

The 25 GAME functions at `0x8001c12c..0x8001f798` are connected by proven
calls to the menu frame, window, sprite, text, list, and input helpers. Each
address was checked with the GAME semantic address, block disassembly, incoming
and outgoing xrefs, strings, and current match query. Address proximity is
only a pilot boundary; translation-unit ownership remains WIP.

| GAME address | Retail shape | Verdict |
| --- | --- | --- |
| `0x8001c12c` | dialog renderer/input controller | unclaimed |
| `0x8001c550` | label-template builder | unclaimed |
| `0x8001c62c` | label-template builder | unclaimed |
| `0x8001c770` | label-template builder | unclaimed |
| `0x8001c8b0` | label-template builder | unclaimed |
| `0x8001c9f4` | label-template builder | unclaimed |
| `0x8001cad4` | label-template builder | unclaimed |
| `0x8001cb44` | conditional label-template builder | unclaimed |
| `0x8001ccd4` | label-template builder | unclaimed |
| `0x8001cdb0` | menu frame renderer | unclaimed |
| `0x8001ceb8` | window/input controller | unclaimed |
| `0x8001d030` | item-list controller | unclaimed |
| `0x8001d340` | code/name-table translator | unclaimed |
| `0x8001d3b4` | item-list controller | unclaimed |
| `0x8001d654` | code/name-table translator | unclaimed |
| `0x8001d6a8` | menu model/list renderer | unclaimed |
| `0x8001d8d0` | menu model/list renderer | unclaimed |
| `0x8001dc64` | window/input controller | unclaimed |
| `0x8001ddd0` | item-list controller | unclaimed |
| `0x8001e0a8` | item-list controller | unclaimed |
| `0x8001e378` | five-argument menu input poll | **exact, 268/268 bytes** |
| `0x8001e484` | input/menu-model controller | unclaimed |
| `0x8001e94c` | numeric text renderer | unclaimed |
| `0x8001f008` | numeric text renderer | unclaimed |
| `0x8001f798` | six paired-row renderer | WIP, 96.236115% fuzzy |

`0x8001e378` reads pad input, wraps a selected row at the ends, emits sound
cues 16–18, and writes selected/confirmed/cancelled results through three
pointers. Its direct calls, three internal jumps, and cursor-direction store
have reviewed relocation rows. Strict `kf match` reports 100% for its unit.

The six-row renderer at `0x8001f798` has the retail six-block CFG, all sprite
and font referents, and ordered calls. Its sole instruction-list difference is
the placement of the loop-index increment: the probe fills the first Y-load
delay slot, while retail leaves that slot empty and increments after the
second Y-load. The current source preserves the natural loop and remains WIP.

The eight short label builders load 20-byte glyph suffixes from the
`0x80064af0..0x80064c08` initialized-data region. Current inventory splits
those suffixes into 4-byte Ghidra seed entries and unclassified gaps; source
claims await a supported object/table extent. The translators at `0x8001d340`
and `0x8001d654` use 240-byte indexed rows around `0x80065c20` and
`0x800661c0`; their owning table and bounds remain unresolved. The larger
screen controllers call these helpers and menu renderers, but their stack
records and resource ownership need a shared typed model before source claims.
No function in this pilot was identified as vendored code.

The related display transition at GAME `0x80034e10` is a separate WIP source
claim (99.114586% fuzzy). It reads a TIM from the archive into the graphics
runtime, uploads it, saves and restores a 320×240 VRAM rectangle with the
Psy-Q image APIs, waits for a pad release/press when the fade helper reports
a negative status, and restores the primitive-buffer split. Its eight-block
CFG, calls, data referents, and constants agree with retail. The remaining
instruction differences are register choices in the two buffer-boundary
calculations; no compiler mechanism is attributed. The `RECT` at
`0x8006d6dc` is initialized to `(320, 0, 320, 240)`, and its defining
translation unit remains unresolved.
