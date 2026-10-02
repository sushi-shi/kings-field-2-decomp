# GAME LIBCD ISO9660 small data records

The pinned Psy-Q 3.0 `LIBCD.LIB` `ISO9660` member identifies the
`GAME.EXE` load-image bytes `0x8006d7b8..0x8006d7cb` as vendored
`.sdata`. Its source path in the archive is
`E:\PS-X~~00\SRC\CD\ISO9660.C`. The corresponding retail bytes match
each archive record: a four-byte scalar `01 00 00 00` at `0x8006d7b8`,
the six-byte `CD001\0` string at `0x8006d7bc`, then the nine-byte
record beginning with two zeros at `0x8006d7c2` and containing `.\0`
at `0x8006d7c4` and `..\0` at `0x8006d7c8`. One final zero byte at
`0x8006d7cb` separates it from the next record at `0x8006d7cc`.

The data inventory now includes both string terminators and records the
zero separators explicitly. The identity rows retain address-derived
names `DAT_8006d7b8`, `DAT_8006d7c4`, and `DAT_8006d7c8` with extents
`0x4`, `0x2`, and `0x3`. This establishes the emitted bytes and vendor
member, but not the original C declarations or linkage. No game source
claims these objects.

One-VA safe carves of exact vendored `CdSearchFile` `0x8004b8a0` and
`CD_cachefile` `0x8004bf64` admitted all relocations with no withheld
function. `CdSearchFile` resolves two direct pairs to the scalar base.
`CD_cachefile` resolves pairs to the dot-string base and to the
dot-dot-string base plus offsets `0` and `2`. Those referents preserve
the retail addresses; no candidate relocation status was promoted
merely from matching the archive bytes.
