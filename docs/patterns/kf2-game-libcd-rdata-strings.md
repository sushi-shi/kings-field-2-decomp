# GAME LIBCD diagnostic strings

Three `.rdata` Code records from the pinned Psy-Q 3.0 `LIBCD.LIB`
archive match `GAME.EXE` retail uniquely and byte-exactly:

| Member | GAME extent | Bytes | Contents |
| --- | --- | ---: | --- |
| `EVENT` | `0x80012dfc..0x80012e10` | 21 | `CdInit: Init failed` with newline and NUL |
| `CDROM` | `0x80012ff0..0x8001301c` | 45 | Final-sector format string with newline and NUL |
| `CDROM` | `0x8001301d..0x80013034` | 24 | Three leading zeros and DMA-status format string with newline and NUL |

The data census now covers each full archive record. The following
`0x80012e11..0x80012e13` and `0x80013035..0x80013037` bytes remain
outside these records and unclassified; no ownership is inferred across
the intervening region. Literal bytes and vendor member are exact, but
original C declarations and linkage remain unknown.

Three existing candidate direct pairs target the strings. The EVENT
reference at `0x8004b60c` is `reachable-code` in exact vendored
`CdInit`; the two CDROM references are `instruction-word`. No
relocation was promoted solely because the data bytes match.

A focused safe one-VA carve of `CdInit` `0x8004b5dc` withheld no
function or relocation. Its direct pair at `0x8004b60c/10` resolves
to the complete EVENT literal at `0x80012dfc` with addend zero.
