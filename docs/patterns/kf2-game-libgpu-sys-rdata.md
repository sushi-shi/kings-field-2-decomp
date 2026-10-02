# GAME LIBGPU SYS format strings

Three separate `.rdata` Code records in pinned Psy-Q 3.0
`LIBGPU.LIB/SYS` match `GAME.EXE` retail uniquely and byte-exactly:

| GAME extent | Archive bytes | Content |
| --- | ---: | --- |
| `0x800133f0..0x8001341a` | 43 | `SetGraphDebug` format string, including newline and NUL |
| `0x8001341b..0x80013424` | 10 | Leading zero and `DrawPrim` string with NUL |
| `0x80013425..0x80013454` | 48 | Three leading zeros and GPU-timeout format string with newline and NUL |

The data census now covers the full 101 archived bytes as three
complete strings and their exact zero prefixes. The following
`0x80013455..0x80013457` bytes are outside these Code records and
remain unclassified. The archive proves vendor member and emitted
literal bytes, but not the original C declarations or linkage.

Three existing candidate direct pairs target these strings: two are
currently `instruction-word`, while the pair to `DrawPrim` is
`reachable-code` in the exact vendored function at `0x8005fa30`.
A focused safe one-VA carve of `DrawPrim` withheld no function or
relocation and resolves the pair at `0x8005fa58/5c` to the full
`DrawPrim` literal at `0x8001341c` with addend zero. The three curated
relocation tiers remain candidate pending separate source-site review.
