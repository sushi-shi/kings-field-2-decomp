# GAME OTAG primitive-name pointer record

The 24 retail words at `GAME.EXE` `0x8006d5fc..0x8006d65b` match one
96-byte `.data` code record from the pinned Psy-Q 3.0 `LIBGPU.LIB` `OTAG`
member (`E:\PS-X~~00\SRC\GPU\OTAG.C`). They are not game data.
The archive supplies 24 32-bit section-relative patches into its
71-byte `.sdata` string block. The patch offsets, in order, are
`0x44, 0x40, 0x3c, 0x38, 0x34, 0x30, 0x2c, 0x28`, four `0x24`, four
`0x20`, then `0x1c, 0x18, 0x14, 0x10, 0x0c, 0x08, 0x04, 0x00`.
Each of the 24 linked retail words equals the `.sdata` retail base
`0x8006d97c` plus its archive patch offset. The strings identify GPU
primitive forms from `S16`/`T16` through `FG3`.

The exact vendored `get_p_name` function at `0x80063a80` addresses the
table base at retail sites `0x80063ac4/0x80063ac8`, then reads an indexed
word. That direct HI16/LO16 pair is still a curated candidate; the raw
base and indexed load establish this table's use, but do not by themselves
promote the relocation. No reviewed direct pair targets a table interior.

The immediately following retail `0x8006d65c..0x8006d673` bytes match a
*different* 24-byte `.data` record from the same OTAG member. The exact
`get_p_size` function references that second record. The remainder of
the current `0x8006d65c..0x8006d67f` coverage gap and the original
C declarations/linkage are not established by this table audit.

The 24 Ghidra four-byte identities for only the proven pointer record
were consolidated into one address-derived `DAT_8006d5fc` inventory
identity of extent `0x60`. It remains address-only because the archive
proves a contiguous emitted record, not the original C spelling or
allocation class. The 24 raw `mips32_candidate` pointer rows remain
candidate. A one-VA safe carve of `get_p_name` admitted five relocation
records (eight machine relocations), including the
`0x80063ac4/0x80063ac8` pair to `DAT_8006d5fc`, with no withheld
function or relocation. This validates
the current bounded delink referent; it does not promote the raw-word
pointer candidates or establish the original declaration. No source or
vendored function claim changed.
