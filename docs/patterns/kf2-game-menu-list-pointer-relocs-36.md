# GAME menu-list primitive pointer: 36 direct pairs

GAME `func_8001fc94` renders three lower-panel packet bands through the
shared `POLY_FT4 *current_poly_ft4` pointer at BSS `0x8006d9e0`. The
current source defines that pointer in `menu_frame_begin.c`; its original
translation-unit owner and BSS allocation mechanism remain candidate.
This pass reviews only the direct relocation sites in `menu_list_render.c`.

The 36 retail `lui` sites are:

| Band | GAME site addresses |
| --- | --- |
| First | `0x800200b4`, `0x800200c4`, `0x800200d4`, `0x800200e8`, `0x800201ac`, `0x800201d4`, `0x800201e4`, `0x800201fc`, `0x80020224`, `0x80020244`, `0x80020264`, `0x80020274` |
| Second | `0x800202d0`, `0x800202e0`, `0x800202f0`, `0x80020300`, `0x800203c4`, `0x800203e0`, `0x800203f0`, `0x80020404`, `0x80020420`, `0x80020438`, `0x80020450`, `0x80020460` |
| Third | `0x80020494`, `0x800204a4`, `0x800204b4`, `0x800204c8`, `0x800205e4`, `0x8002060c`, `0x8002061c`, `0x80020634`, `0x8002065c`, `0x8002067c`, `0x8002069c`, `0x800206ac` |

Each site has an adjacent retail `lw` using the same base register. Its
`lui 0x8007` and signed low immediate `0xd9e0` resolve exactly to
`0x8006d9e0`, and the current source dereferences the typed pointer at
the corresponding packet operations. All 36 curated rows changed only
from candidate/`paired-reachable` to reviewed/`paired-reviewed`; their
sites, partners, opcodes, targets, and addends are unchanged. The safe
one-VA carve of `0x8001fc94` admitted 36/36 `current_poly_ft4` pairs
with zero addends and withheld no relocation or function. Isolated strict
objdiff leaves `func_8001fc94` at its prior 99.70803% WIP. The remaining
text residue does not justify a source or storage-owner change.
