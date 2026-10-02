# GAME LIBETC small-data records

The pinned Psy-Q 3.0 `LIBETC.LIB` archive emits several separate
`.sdata` records whose bytes match `GAME.EXE` retail storage:

| Member | GAME extent | Archive record | Retail contents |
| --- | --- | --- | --- |
| `INTR_VB` | `0x8006d880..0x8006d88f` | 16 bytes | three hardware pointers `0x1f801110`, `0x1f801114`, `0x1f801118`, then zero |
| `INTR_VB` | `0x8006d890..0x8006d893` | 4 bytes | zero word exported as `Vcount` at `.sdata+0x10` |
| `INTR` | `0x8006d894..0x8006d89f` | 12 bytes | patched pointer to `0x8006d4d0`, then two zero words |
| `INTR` | `0x8006d8a0..0x8006d8a7` | 8 bytes | `0x1f801070`, `0x1f801074` |
| `INTR` | `0x8006d8a8..0x8006d8ab` | 4 bytes | zero |
| `INTR_DMA` | `0x8006d8ac..0x8006d8af` | 4 bytes | `0x1f8010f4` |
| `INTR_DMA` | `0x8006d8b0..0x8006d8b3` | 4 bytes | zero |
| `VSYNC` | `0x8006d8b4..0x8006d8c3` | 16 bytes | `0x1f801814`, `0x1f801110`, then two zero words |

The archive patch in the 12-byte `INTR` record points to its separate
`.data` base, which is the already bounded GAME `0x8006d4d0`
record. The `INTR_VB` member directly exports `Vcount` at `.sdata+0x10`,
which places its second four-byte record at `0x8006d890`; the `VSYNC`
member explicitly imports that symbol. Its curated identity is now
global `Vcount` with 32-bit storage. Original C signedness and the
current defining C module remain unresolved. All other corrected
identities remain address-only: their archive records do not prove
separate original C declarations or linkage.

Seventy-three current candidate direct pairs target the corrected
records, 23 in `reachable-code` and 50 in `instruction-word`; their
status was not promoted from byte matching. Focused one-VA safe
carves of exact vendored `ResetCallback` `0x8005e848` and `VSync`
`0x8005ef64` withheld no function or relocation. `ResetCallback`
resolves its direct pair to `DAT_8006d894`. Nine VSync pairs into
`DAT_8006d8b4` resolve at offsets `0`, `4`, `8`, or `0x0c`;
three VSync pairs to `Vcount` remain separate from the VSYNC record.
A repeat one-VA VSync safe carve after naming `Vcount` withheld nothing
and resolved all three pairs to that exported symbol at addend zero.
