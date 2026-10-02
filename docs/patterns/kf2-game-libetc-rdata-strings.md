# GAME LIBETC interrupt diagnostics

Four `.rdata` Code records from the pinned Psy-Q 3.0 `LIBETC.LIB`
archive match `GAME.EXE` retail uniquely and byte-exactly:

| Member | GAME extent | Bytes | Contents |
| --- | --- | ---: | --- |
| `INTR` | `0x80013378..0x800133ab` | 52 | `intr.c` revision string with NUL |
| `INTR` | `0x800133ac..0x800133c0` | 21 | Interrupt-overflow diagnostic with newline and NUL |
| `INTR_DMA` | `0x800133c4..0x800133dd` | 26 | DMA-bus-error diagnostic with newline and NUL |
| `VSYNC` | `0x800133e0..0x800133ef` | 16 | VSync-timeout diagnostic with newline and NUL |

The three zeros at `0x800133c1..0x800133c3` and the two at
`0x800133de..0x800133df` fall between, not inside, the respective
Code records. The data census now includes each full string and keeps
those alignment bytes unclassified. Archive bytes prove the vendor
members and literal extents; original C declarations and linkage
remain unknown.

Four existing candidate direct references target the records: one
raw-word pointer to the revision string, two instruction-word pairs
to the interrupt and DMA diagnostics, and one reachable-code pair in
exact vendored `v_wait` to its timeout string. None was promoted from
literal-byte identity. A focused safe one-VA carve of `v_wait`
`0x8005f068` withheld no function or relocation and resolves its
direct pair at `0x8005f0b0/b4` to `DAT_800133e0` with addend zero.
