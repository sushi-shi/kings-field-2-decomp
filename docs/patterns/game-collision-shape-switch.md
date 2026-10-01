# GAME collision-shape opcode switch

GAME `0x8002aaa4` is a game collision-shape dispatcher with three proven
callers at `0x8002b604`, `0x8002b67c`, and `0x8002b7f8`. Its C claim is
`game.collision_shape_dispatch`; the current body remains WIP.

Retail `0x8002ac34` subtracts `0x10` from the unsigned record opcode and
rejects indices above `0x30`. The following table-base load at
`0x8002ac54/0x8002ac58` and indirect `jr` at `0x8002ac68` select one of
49 initialized pointer words at `0x8001134c..0x8001140c` (Japanese EXE file
offsets `0xb4c..0xc0c`). All 49 raw words equal the curated target addresses.
Their 13 distinct values are block heads within `0x8002aaa4..0x8002b603`.
The 37 default entries point to `0x8002b5c4`. These facts validate the
pointer data and its extent; the semantic CFG still treats the `jr` edge
as indirect, without promoting individual case successors to proven calls
or branches.

The phase-one loader in `0x80016820` copies `0x600` words (`0x1800` bytes)
to `bss_801c7540+0x10000`, the bank base used by this dispatcher. The copy
ends at +`0x11800`, before the collision-cache tail. It bounds the loaded
region but does not prove the variable-length record layout or a separate
source definition for that interior address. The range overlaps the
provisional map-cell and equipment views of this complete BSS object, so a
second global or a permanent field split is not yet justified.

The 49 `mips32_candidate` rows at `0x8001134c..0x8001140c` are now
`pointer-reviewed`/`reviewed`. A safe GAME one-VA carve for `0x8002aaa4`
reports zero withheld relocations and emits 49 `R_MIPS_32` entries in
the module's `.rel.rodata`; the module has 144 total relocations, versus
95 in the text-only carve. Both retail and source objects have a 196-byte
`.rodata` table with 49 `R_MIPS_32` rows and the same 37-entry default
grouping. The table's case
offsets still differ because the WIP text layouts differ: direct objdiff
reports 19.897959% `.rodata` and 31.652473% `.text` similarity, not
exactness. A fresh focused build reports 14.0% listing similarity, with
174/165 target/source CFG blocks and 99/97 branches. No C source or
signature change is retained from this audit.
