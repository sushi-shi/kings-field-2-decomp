# GAME LIBCD BIOS command-pointer records

The 29 pointer words at `GAME.EXE` `0x80067a48..0x80067abb` are one
116-byte `.data` code record in the pinned Psy-Q 3.0 `LIBCD.LIB` `BIOS`
member (`E:\PS-X~~00\SRC\CD\BIOS.C`). The member's `.text` section
is independently exact in the vendored census. These words are SDK
storage, not game data.

The archive supplies 29 consecutive 32-bit section patches. Its
`.rdata` base resolves to retail `0x80012c38`, and its `.sdata` base
to `0x8006d718`; the patches alternate between command-name strings
in those two sections. Every linked word equals its archived section
base plus addend, including first word `0x8006d758` and last word
`0x8006d720`. Five candidate direct HI16/LO16 pairs in the current
inventory target only the table base; no curated pair targets an
interior address. They remain candidate rather than being promoted by
the 29-word data comparison.

The 29 Ghidra four-byte data and identity rows were consolidated into
one address-only `DAT_80067a48` inventory row of extent `0x74`. The
archive proves the emitted record and patch destinations. The original
C declaration and linkage of the table are not established.

The next 32-byte BIOS `.data` record is separate at
`0x80067abc..0x80067adb`. Its eight archive patches resolve to the
same `.rdata` and `.sdata` blocks; all eight linked retail words match,
including a duplicated `0x8006d760` at the final two positions. Its
eight Ghidra rows were separately consolidated into one address-only
`DAT_80067abc` identity of extent `0x20`. The two emitted records do
not prove two specific original C declarations. All raw-word
relocation candidates remain untouched. No game source or vendored
function claim changed.

A one-VA safe carve of exact BIOS `get_alarm` `0x8004a624` loaded the
updated manifest with no withheld function or relocation. It admitted
the `0x8004a654/0x8004a658` HI16/LO16 pair to `DAT_80067a48` and a
separate pair to adjacent `DAT_80067abc`. The carve validates the
current bounded referents; it does not promote all table-word pointer
candidates or prove the original C declaration.

The same archive's `.sdata` supplies a separate command-register
record. Its seven-byte `NoIntr` string occupies retail
`0x8006d770..0x8006d776` including NUL. The next 33-byte emitted
record is one zero alignment byte at `0x8006d777`, followed by eight
aligned words at `0x8006d778..0x8006d797`. All 33 bytes match retail:
the first five words are `0x1f801800`, `0x1f801801`, `0x1f801802`,
`0x1f801803`, and `0x1f801c00`; the final three words are zero. The
inventory now includes the string terminator and keeps the alignment
byte separate, while the eight Ghidra word/byte fragments form one
address-only `DAT_8006d778` row of extent `0x20`. Candidate direct
references to its interior words remain candidates until reviewed per
site; this byte match does not prove the original C declaration.

The first focused `getintr` carve could not verify this range because
that function has two curated fragments without recorded fragment
ranges; the safe delinker withheld it and its 69 candidate relocations.
The nonfragmented exact BIOS `CD_cw` `0x8004ad60` provides a direct
control: its one-VA safe carve withheld nothing, and 14 direct pairs to
the former separate words now resolve to `DAT_8006d778` with the
retail-preserving offsets `0`, `4`, `8`, `0x14`, `0x18`, or `0x1c`.

A second BIOS `.sdata` command-register record follows. Its seven-byte
format string at `0x8006d798..0x8006d79e` includes the NUL byte. The
archive's next 25-byte emitted record is one zero alignment byte at
`0x8006d79f`, followed by six pointer words at
`0x8006d7a0..0x8006d7b7`: `0x1f801018`, `0x1f801020`,
`0x1f8010f0`, `0x1f8010b0`, `0x1f8010b4`, and `0x1f8010b8`.
The format string and all 25 record bytes match retail. The inventory
now keeps the string terminator and alignment byte separate and groups
the six former Ghidra pointer rows into address-only `DAT_8006d7a0`
of extent `0x18`. Nine candidate direct HI16/LO16 pairs in exact BIOS
`CD_read` `0x8004b498` refer to this record; their status remains
candidate. Its one-VA safe carve loaded the manifest with no withheld
function, but withheld all 13 of that function's candidate relocations
under `non-reachable-code-channel`, including the nine pairs to this
record. The raw retail disassembly and archive patch list support the
referents, but the current `instruction-word` channel is insufficient
for safe admission without separate per-site review. These bytes
establish the emitted vendor record but leave its original C
declaration unresolved.
