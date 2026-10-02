# GAME LIBGPU PRIM format strings

The pinned Psy-Q 3.0 `LIBGPU.LIB/PRIM` member emits four successive
`.rdata` Code records. Each record occurs uniquely and byte-exactly in
`GAME.EXE` retail:

| GAME extent | Archive bytes | Content |
| --- | ---: | --- |
| `0x80013520..0x80013535` | 22 | `tpage` format string |
| `0x80013536..0x80013546` | 17 | Two leading zeros, then `clut` format string |
| `0x80013547..0x8001359d` | 87 | Leading zero; `clip`, `ofs`, `tw`, `dtd`, and `dfe` strings with internal zero padding |
| `0x8001359e..0x800135ef` | 82 | Two leading zeros; `disp`, `screen`, `isinter`, and `isrgb24` strings with internal zero padding |

The data census now covers all 208 bytes as eleven complete format
strings, including their newline and NUL bytes, plus the actual zero
prefixes and padding. Its previous byte-pattern string spans omitted
the newline and terminator, causing the rest of each archived record
to appear as an unclassified gap. Archive section layout establishes
emitted bytes and vendor member, but not the original C declaration or
literal linkage.

Fourteen existing direct pairs target eleven string starts in this
range. All fourteen source sites are currently `instruction-word`
candidate evidence, with no curated PRIM function boundary covering
their full source. No relocation tier or function identity was
promoted from the literal-byte match, and no game source claims this
vendored data.
