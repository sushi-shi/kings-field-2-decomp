# GAME LIBCD CDROM register records

The pinned Psy-Q 3.0 `LIBCD.LIB` `CDROM` member (`E:\PS-X~~00\SRC\CD\CDROM.C`)
emits two adjacent `.sdata` code records at `GAME.EXE`
`0x8006d7cc..0x8006d837`. The first is 96 bytes, comprising CD and
device register pointers, two small constants, and trailing zero words.
The second is a separate 12-byte zero record. All 108 bytes match
retail exactly; the next inventory row begins at `0x8006d838` and is
outside these archive records.

The former Ghidra word, halfword, and coverage-gap fragments now form
two bounded address-only identities: `DAT_8006d7cc` (`0x60`,
`u32[24]`) and `DAT_8006d82c` (`0x0c`, `u32[3]`). They describe the
archived load-image records, not two proved original C declarations.
No game source claims them.

The current relocation inventory has 50 candidate direct pairs whose
targets fall inside the two records. Their source sites span GAME RVAs
`0x3bef4..0x3ca50` and remain in the `instruction-word` channel.
The archive and linked words prove the data bytes, but neither that
comparison nor address proximity promotes these candidates. Some
source sites also lack a curated contiguous function boundary, so a
single focused carve cannot certify the entire reference set.

A one-VA safe carve of curated `0x8004d080` loaded the new identities
with no withheld function. It withheld all eight of that function's
candidate relocations as `non-reachable-code-channel`, including four
to `DAT_8006d7cc` at addends `0x18` and `0x1c`. No relocation status
or code-channel classification was changed to force admission.
