# GAME LIBSPU register-pointer record

The pinned Psy-Q 3.0 `LIBSPU.LIB` `SPU` member emits a 28-byte
`.sdata` code record that matches GAME retail bytes
`0x8006d840..0x8006d85b`. Its seven little-endian words point to
`0x1f801c00`, `0x1f8010c0`, `0x1f8010c4`, `0x1f8010c8`,
`0x1f8010f0`, `0x1f8010f4`, and `0x1f801014`. The adjacent word at
`0x8006d85c` is outside the 28-byte archive record.

The seven Ghidra pointer fragments are now one bounded address-only
`DAT_8006d840` identity of extent `0x1c`. The archive and retail
bytes identify an emitted SDK record, while its original C
declaration and linkage remain unresolved. No game source claims it.

The current relocation inventory contains 103 candidate direct
pairs into these seven words: 93 `reachable-code` and ten
`instruction-word`. The target addresses and exact vendored SPU
text support the record's referents, but the data comparison alone
does not promote those pairs.

A one-VA safe carve of exact vendored `_spu_init` `0x8004d5c0`
withheld no function or relocation. Its eight direct pairs into this
record resolve to `DAT_8006d840` at addends `0` or `0x10`. This is a
focused identity/referent control; it does not review every candidate
site in the SPU text section.
