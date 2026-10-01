# GAME menu preview and sprite follow-up

This ten-function GAME batch follows the card-format preview controller into the
two-option menu, sprite packets, glyph rendering, decimal formatting, and TMD
primitive preparation. Each address was checked against retail disassembly,
CFG, callers, callees, strings, references, and the current strict match state.
The source models remain GAME code; none has vendored-library evidence.

| Address | Final verdict | Retail-supported boundary |
| --- | --- | --- |
| `0x8001bf68` | WIP, 97.123890% | Card probe, error dialogs, and slot write agree; probe-status register and later write-path schedule differ. |
| `0x8001f8b8` | WIP, 94.364640% | Two-label preview selector has 42/42 CFG blocks and 18/18 branches; block order and saved-register allocation differ. |
| `0x8001fb8c` | WIP, 99.787880% | Window rows, calls, and CFG agree; retail reserves 48 stack bytes versus 40 in the probe, with no evidenced extra live object. |
| `0x8002083c` | WIP, 99.658820% | Item TMD preview uses the correct four matrix locals and referents; retail reserves 64 further stack bytes without a proved source owner. |
| `0x80020d20` | **Strict exact, 100%** | Animated cursor quad, 472/472 code bytes. |
| `0x80020ef8` | **Strict exact, 100%** | Fixed-CLUT cursor quad, 436/436 code bytes. |
| `0x800210ac` | WIP, 99.669040% | Glyph packets, kana marks, and eight CFG blocks agree; retail has a 56-byte frame versus 48, plus a UV register choice. |
| `0x80021510` | **Strict exact, 100%** | Two-column number glyph atlas, 736/736 code bytes in a fresh direct per-unit comparison. The historical 98.619570% probe has been superseded. |
| `0x80022058` | WIP, 97.390000% strict | Decimal digits, blank fill, and style glyphs agree; CFG is 36/36 blocks and 19/19 branches with matching successor lists. Retail reserves eight stack bytes, while the probe is a leaf. |
| `0x8002d5dc` | WIP, 96.132600% | The typed TMD packet parser has 17/17 CFG blocks, but one extra entry instruction shifts its 29-row switch-table addend by four bytes. |

For both exact sprite functions, the quad's left edge is naturally expressed as
`position x - (sprite width + left margin)`. The margin is eight pixels for the
animated cursor and seven for the fixed-CLUT cursor. This is the same placement
as the earlier source, and the grouped source emits the retail load and
subtraction sequence. Temporary probes of an explicit unsigned cast and an
alternative grouping did not produce identical listings. No artificial local,
padding, volatile carrier, or assembly was retained for the other residues.
For the decimal formatter, two temporary pointer-loop spellings still emitted
a leaf return without retail's eight-byte frame, so its existing source was
kept. KF1's four-argument decimal formatter supports the digit/padding loop;
KF2's fifth style argument and glyph prefixes are separately visible in its
retail branches.

A later fresh focused build and direct per-unit objdiff give `0x80022058`
97.39% strict across its 400-byte body. The first difference is the retail
`addiu sp,sp,-8`, followed by the fifth-argument load at `24(sp)` instead of
the probe's `16(sp)`; the return delay slot restores that frame where the probe
emits `nop`. The numeric and style instructions, call set, and referents remain
aligned. No additional live source object has been established, so the source
retains the supported formatter behavior rather than introducing artificial
frame storage.

Focused comparisons reported both sprite listings SAME. Strict `kf match`
confirmed 100% for each and relinked 145/145 GAME units. The global edge check
still stops on the three previously known unrelated `.rodata` addends in TMD
preparation and map-object units. A full `kf build` after these source edits
built PSX; GAME, OPEN, and END retained their existing first unresolved link
symbols (`InitCARD`, `malloc`, and `display_buffers`).
