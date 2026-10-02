# GAME LIBGPU TMD primitive labels

The pinned Psy-Q 3.0 `LIBGPU.LIB` `TMD` member emits a 109-byte
`.sdata` code record at `GAME.EXE` `0x8006d90c..0x8006d978`.
All 109 archive bytes equal retail. The record holds 16 NUL-terminated
primitive labels, with zero padding between the aligned starts:
`F3L`, `G3L`, `FT3L`, `GT3L`, `F3`, `G3`, `FT3`, `GT3`,
`F4L`, `G4L`, `FT4L`, `GT4L`, `F4`, `G4`, `FT4`, and `GT4`.
Each label includes a trailing space before NUL. The next byte at
`0x8006d979` is outside the archived record and remains unclassified.

The data inventory now includes each complete string, its NUL, and
the intervening zero padding. Four labels (`F3`, `G3`, `F4`, `G4`)
were previously hidden inside coverage gaps; several recognized
strings were one byte short. The archive establishes the emitted
literal bytes and member, but not the original C literal declarations
or linkage. No game source claims this vendor pool.

There is one current candidate direct pair to each of the 16 string
starts, at GAME `0x8006d90c`, `0x8006d914`, `0x8006d91c`,
`0x8006d924`, `0x8006d92c`, `0x8006d930`, `0x8006d934`,
`0x8006d93c`, `0x8006d944`, `0x8006d94c`, `0x8006d954`,
`0x8006d95c`, `0x8006d964`, `0x8006d968`, `0x8006d96c`, and
`0x8006d974`. All 16 source sites are currently in the
`instruction-word` channel. The literal-byte comparison does not
promote their relocation status.
