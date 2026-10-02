# GAME glyph renderer: 24 current-primitive pointer pairs

`GAME.EXE` `menu_draw_string` at `0x800210ac..0x8002150f` draws the base
glyph and optional kana marks through the shared `POLY_FT4 *current_poly_ft4`
view. Its current C definition is in `menu_frame_begin.c` at BSS
`0x8006d9e0`, and the glyph renderer declares the pointer through the
shared graphics header. The present source owner is supported by the frame
writer; the original translation unit and BSS allocation mechanism remain
unproved. This audit changes no source or type.

The 24 formerly candidate GAME relocation pairs in the glyph function have
these `lui` site addresses:

`0x80021100`, `0x800211cc`, `0x800211e0`, `0x800211f0`,
`0x80021200`, `0x80021214`, `0x80021228`, `0x8002123c`,
`0x8002126c`, `0x8002130c`, `0x80021324`, `0x80021334`,
`0x80021344`, `0x80021358`, `0x8002136c`, `0x80021380`,
`0x800213b0`, `0x80021450`, `0x80021468`, `0x80021478`,
`0x80021488`, `0x8002149c`, `0x800214b0`, `0x800214c4`.

Each raw retail site is `lui 0x8007` immediately followed by `lw` with the
same base register and signed low immediate `0xd9e0`, resolving exactly to
`0x8006d9e0`. The sites lie in the bounded reachable glyph function, whose
source repeatedly dereferences `current_poly_ft4` for the textured packet
and kana marks. All 24 rows were promoted from candidate/`paired-reachable`
to reviewed/`paired-reviewed` without changing their sites, targets,
registers, opcode forms, or addends. A safe one-VA carve at `0x800210ac`
withheld no function or relocation and materialized all 24 as
`R_MIPS_HI16+R_MIPS_LO16` pairs against `current_poly_ft4` with zero addend.

Isolated strict objdiff after the carve leaves `menu_draw_string` at
99.66904% WIP, unchanged from the earlier direct result. The previously
recorded frame/register allocation residue is not evidence to alter the
correct pointer loads. No repository tests, lint, broad match, full build,
or full-image link were run.
