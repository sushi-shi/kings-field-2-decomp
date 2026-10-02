# GAME LIBSPU diagnostic strings

Four pinned Psy-Q 3.0 `LIBSPU.LIB` `.rdata` Code records match
`GAME.EXE` retail uniquely and byte-exactly:

| Member | GAME extent | Bytes | Content |
| --- | --- | ---: | --- |
| `SPU` | `0x80013038..0x80013054` | 29 | SPU timeout and reset-wait strings with two zero padding bytes |
| `SPU` | `0x80013055..0x8001307f` | 43 | Three leading zeros, wait-ready string, one zero pad, and DMA-wait string |
| `SPU` | `0x80013080..0x80013090` | 17 | IOCTL/IRQ wait string |
| `S_M_INT` | `0x80013094..0x800130ac` | 25 | Allocator diagnostic format string |

The census now spells all six strings through their NUL bytes and
includes each archive-backed zero prefix or padding span. The SPU
timeout and allocator diagnostic strings also include their newline.
The three zero bytes at `0x80013091..0x80013093` separate members and
remain unclassified; the three following `S_M_INT` bytes at
`0x800130ad..0x800130af` are likewise outside its Code record.
Original C literal declarations and linkage are not established by
the bytes alone.

Six existing direct pairs target these strings: five are
`reachable-code` inside exact vendored SPU functions, and one S_M_INT
pair is currently `instruction-word`. They remain candidate pending
separate source-site relocation review.

A focused safe one-VA carve of exact `_spu_ioctl` `0x8004def0`
withheld no function or relocation. Its two direct pairs at
`0x8004e6d8/dc` and `0x8004e6e0/e4` resolve to the full SPU timeout
and IOCTL/IRQ wait strings at `0x80013038` and `0x80013080`, both with
addend zero. No relocation status changed.
