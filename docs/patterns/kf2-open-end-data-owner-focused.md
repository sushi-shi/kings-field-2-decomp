# OPEN and END focused data-owner audit

The OPEN and END programs share a load window; every address below is scoped
to its image. A fresh manifest-profile compile of all 18 reconstructed units
into separate off-tree image directories, followed by isolated strict objdiff,
found 40 of 41 functions already exact. All text relocation section sizes
agree. `OPEN` `audio_play_voice` is the sole WIP at 99.82353%: retail and
candidate have the same 136-byte body and call, but use 64-byte and 40-byte
frames respectively. Its source has no evidenced extra local to explain the
24-byte frame gap, so no stack padding was added.

| Image and unit | Function verdict | Initialized-data verdict | Target BSS |
| --- | --- | --- | ---: |
| OPEN `main` | 3/3 exact | 9-byte DATA exact | 9 B |
| OPEN `display_init` | 1/1 exact | none | 24,816 B |
| OPEN `audio_init` | 1/1 exact | none | 344 B |
| OPEN `audio` | 1/1 exact | none | 42 B |
| OPEN `title` | 6/6 exact | 28-byte DATA exact | none |
| OPEN `display_frame` | 2/2 exact | none | 4 B |
| OPEN `cd_file` | 1/1 exact | 11-byte DATA exact | none |
| OPEN `resources` | 1/2 exact; `audio_play_voice` 99.82353% | none | none |
| OPEN `movie` | 1/1 exact | 31-byte RODATA exact | none |
| OPEN `movie_stream` | 7/7 exact | 8-byte DATA exact | 408,624 B |
| END `main` | 2/2 exact | 9-byte DATA exact | 4 B |
| END `display_init` | 1/1 exact | none | 24,816 B |
| END `audio_init` | 1/1 exact | none | 344 B |
| END `audio` | 1/1 exact | none | 26 B |
| END `display_frame` | 2/2 exact | none | 4 B |
| END `cd_file` | 1/1 exact | 11-byte DATA exact | none |
| END `movie` | 1/1 exact | 31-byte RODATA exact | none |
| END `movie_stream` | 7/7 exact | 8-byte DATA exact | 408,624 B |

Every listed initialized section is byte exact, including its named object
symbols. The residual data mismatch is storage class and placement: all 12
units with target `.bss` emit candidate COMMON symbols instead of a `.bss`
section. The candidate's tentative definitions retain the intended names
and references but are not placed in their target order. Several COMMON
requests are also rounded to eight bytes: OPEN/END `display_current` is
target 4 B versus candidate 8 B; OPEN `audio` has target pointer/halfword
objects of 4/2 B versus 8-byte requests; END `audio` has the same pattern;
`current_poly_ft4` is 4/8 B; and the OPEN/END main words are 4/8 B.
The complete display target BSS in each image is 24,816 B, holding
`display_current` 4 B, `display_buffers` 8,424 B, `display_primitives`
16,384 B, and alignment. The sequence table is 344 B in both images and
the candidate COMMON request is also 344 B.

The strongest complete-extent control is shared `src/lib/movie_stream.c`.
Each image's 408,624-byte target `.bss` is exactly five sequential source
objects: `vlcbuf0` and `vlcbuf1` of 163,840 B each, `imgbuf` 15,360 B,
`Ring_Buff` 65,536 B, and `dec` 48 B. In OPEN they begin at `0x8003e058`,
`0x80066058`, `0x8008e058`, `0x80091c58`, and `0x800a1c58`; END has
different bases (`0x8003ae50`, `0x80062e50`, `0x8008ae50`, `0x8008ea50`,
`0x8009ea50`) with the same contiguous extents. The candidate emits five
same-sized COMMON requests. The target delink object gives its `.bss`
alignment 8; that and the global symbol binding are curated model choices,
not original-source proof. Seven exact movie-stream functions and their
relocations support the source object's use and layout, but neither the
original compiler's tentative-definition behavior nor its exact defining
translation-unit boundary is established.

An off-tree `-fno-common` control on the complete OPEN movie-stream unit
rules out a simple compiler-profile switch. All five tentative buffers move
from COMMON into `.data`, not `.bss`: candidate `.data` grows from 8 to
408,632 B, while target `.data` remains 8 B and target `.bss` is 408,624 B.
The control's five symbol sizes remain correct and their offsets become
`+8`, `+0x28008`, `+0x50008`, `+0x53c08`, and `+0x63c08` inside `.data`,
corresponding to the target `.bss` offsets `+0`, `+0x28000`, `+0x50000`,
`+0x53c00`, and `+0x63c00`. Target `.bss` alignment is 8; the control's
`.data` alignment is 4. Four of seven formerly exact movie-stream functions
regress (to 95.53191%, 99.655174%, 98.35821%, and 99.189186%); the
other three remain exact. The `-fno-common` mechanism therefore does not
explain the retail model's section placement under this pinned profile.
Making symbols `static` solely to alter section class would discard
currently shared declarations without evidence of original linkage.
No source, inventory, or profile change was retained here. This audit ran
focused compiles and isolated strict comparisons only.
