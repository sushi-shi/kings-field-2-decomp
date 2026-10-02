# GAME LIBGPU SYS data section boundary

The pinned Psy-Q 3.0 `LIBGPU.LIB` `SYS` member comes from
`E:\PS-X~~00\SRC\GPU\SYS.C`. Its exact `.text` section is already
identified at `GAME.EXE` `0x8005f4a0..0x80061a57`. The archive emits
exactly two `.data` code records, 49 and 67 bytes, for a `0x74`-byte
section. Retail `0x8006d588..0x8006d5fb` agrees throughout:

- `0x8006d588..0x8006d5b8` is the 49-byte `$Id: sys.c...$` revision
  string, including its NUL byte. The former string-census row stopped
  one byte early.
- `0x8006d5b9..0x8006d5bb` is three zero alignment bytes at the start
  of the second emitted record. Those bytes alone are not an object.
- `0x8006d5bc..0x8006d5fb` holds 16 aligned pointer words. The archive
  has 16 32-bit section patches at second-record offsets `+3,+7,...,+0x3f`.
  The first retail word is the revision-string base `0x8006d588`; the
  other 15 words equal exact SYS text base `0x8005f4a0` plus their
  respective archive patch offsets. All 16 linked words agree.

The 16 Ghidra four-byte data/identity rows for this pointer fragment
were consolidated into one address-only `DAT_8006d5bc` row of extent
`0x40`. The revision string row now includes its NUL, and the three
alignment bytes remain a separate unclassified gap. The archive proves
emitted bytes and patches; it does not prove the original C declarations
or linkage of the pointer fragment.

SYS also emits a separate 24-byte `.sdata` record. Its first archive
patch points to `.data+0x34`, which is retail `0x8006d5bc`; the linked
word at `0x8006d8c4` has that value. Its second archive patch is an
external `printf` reference, matching retail `0x8006d8c8`, followed
by four zero words. That cross-section relationship is strong vendor
provenance. The six Ghidra four-byte rows for this exact `.sdata`
record were consolidated into one address-only `DAT_8006d8c4` identity
of extent `0x18`. Its direct interior views retain offsets from that
base; the existing raw-word `mips32_candidate` rows were not promoted.
No game source or vendored function claim changed.

A one-VA safe carve of exact SYS function `0x8005f4c8` loaded the
corrected metadata with zero withheld functions or relocations. Its
direct pairs to the three former `.sdata` identities now resolve as
`DAT_8006d8c4` at addends `0`, `0x0c`, and `0x10`, preserving the
retail target addresses. It has no direct reference to the revised
`DAT_8006d5bc` identity, whose attribution rests on the archive bytes
and patches.
