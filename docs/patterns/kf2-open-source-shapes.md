# OPEN.EXE: source shapes and one open residue (SLPS-00069)

OPEN.EXE's 25 source-claimed functions were reconstructed under
`probe-gcc257-o2-plain`. The existing strict baseline has 24 exact functions;
a fresh focused build of all ten units preserved 24 SAME listings and the
one WIP below. These source facts decided the last differences.

| OPEN address | Function | Focused verdict |
| --- | --- | --- |
| `0x80011ac0` | `main` | SAME |
| `0x80011f9c` | `opening_fade_out` | SAME |
| `0x80012060` | `opening_load_data` | SAME |
| `0x800120c8` | `display_initialize` | SAME |
| `0x80012204` | `audio_initialize` | SAME |
| `0x80012270` | `opening_open_audio` | SAME |
| `0x80012560` | `opening_draw_title` | SAME |
| `0x80012b7c` | `opening_draw_banner` | SAME |
| `0x80012d7c` | `opening_draw_prompt` | SAME |
| `0x80012f3c` | `opening_poll_pad` | SAME |
| `0x800130ac` | `primitive_buffer_begin_poly_ft4` | SAME |
| `0x800130fc` | `primitive_buffer_commit_poly_ft4` | SAME |
| `0x800131c8` | `display_begin_frame` | SAME |
| `0x8001322c` | `display_present_frame` | SAME |
| `0x80013284` | `cd_file_load_into` | SAME |
| `0x800133e0` | `tim_upload_images` | SAME |
| `0x80013450` | `audio_play_voice` | DIFF 82.9% focused; 99.82353% prior strict, frame only |
| `0x800134f0` | `opening_play_movie` | SAME |
| `0x8001383c` | `strSetDefDecEnv` | SAME |
| `0x800138f8` | `strInit` | SAME |
| `0x8001396c` | `strCallback` | SAME |
| `0x80013a78` | `strNextVlc` | SAME |
| `0x80013b0c` | `strNext` | SAME |
| `0x80013bbc` | `strSync` | SAME |
| `0x80013c30` | `strKickCD` | SAME |

The main loop calls the title, banner, prompt, input, display-frame, and movie
functions directly. The title builders call both primitive-buffer helpers;
the movie path calls both frame helpers and the streaming routines. These
decoded calls, together with the shared display and title-state globals,
define this image-qualified batch.

The shared `src/lib/cd_file.c` loader now gives `cd_file_load_into` and its
two initialized path fragments explicit `ADDRESS_AT`/`DATA_AT` claims for
OPEN and END. This removes the last name-only END unit binding while keeping
the independently curated addresses (`0x80013284`/`0x80011e64`) and the
5/3-byte data extents. Focused rebuilds of `open.cd_file` and `end.cd_file`
both report the loader listing SAME; no function or initializer bytes changed.

The seven `str*` streaming routines follow Sony's pinned Psy-Q 3.0
`SAMPLE/MOVIE/ANIM/MAIN.C` tutorial, but their bodies are adapted here.
The decoder environment is global and fixed to 320 by 240 pixels; `strInit`
binds the callback and stream flags internally; `strNext` checks sector
header words and frame count instead of the tutorial's resolution-change
path; `strSync` drops the tutorial print and volatile counter; and
`strKickCD` uses `CdSeekL` and another stream-mode bit. A raw name search of
the pinned library archives found none of these distinctive `str*` names;
that absence alone does not prove body ownership. Their provenance remains
tutorial-derived, game-adapted WIP rather than an archive-vendored attribution.

## Shapes that decided the match

| Function | Retail evidence | Source |
| --- | --- | --- |
| `main` | The constant 1 is materialised in each inner loop's preheader: `li s1,1` inside the title restart and `li s0,1` before the menu. The idle branch falls through into the demo code. | The title restart is `goto restart;`, not an enclosing `for (;;)`. Loop optimisation then treats only the intro and menu as loops, so it hoists no invariant above them. |
| `main` | The `RECT` for `ClearImage` stores `y` before `x`. | `rect.x = rect.y = 0;` then `w`, `h`. A chained assignment stores its right-hand target first. |
| `opening_open_audio` | The movie-sequence copy loop ends at `src + 0x1c60`, measured from the advanced cursor. | The cursor advances past the title sequence before the second `memcpy`. END's loader walks every section the same way. |
| `primitive_buffer_commit_poly_ft4` | `r0` stays in one register through the compare and the in-place subtraction; the fade byte is reloaded in the else arm. | `level = p->r0; if (level < fade) level = 0; else level -= fade;` with `u8 level`. An `s32 level` gives `slt`, and a ternary or a reversed test gives a different block order. |

The `memcpy` calls from `u8 *` buffers produce the MIPS `expand_block_move`
runtime alignment split: `or`/`andi 3`, an unaligned `lwl`/`lwr` loop, and an
aligned `lw` loop. The tail for a size that isn't a multiple of 16 is always
unaligned.

## Open residue: `audio_play_voice` frame

Retail `audio_play_voice` (0x80013450) allocates a 64-byte frame, reading
arguments 5 and 6 at `80(sp)`/`84(sp)`. The plain wrapper
`if (!program && !tone && !note) return; SsUtKeyOn(..., 0, left, right);`
matches every instruction but builds a 40-byte frame. The 24 extra bytes are
locals (`vars= 24`) or an equivalent outgoing-argument excess. No load or
store touches them.

Controls:

- GCC 2.4.1 and 2.6.0 from the Psy-Q 3.0 kit (DOSBox), and the 2.5.7 probe,
  all give 40 bytes. This is a source difference, not a compiler difference.
- US (SLUS-00158) and EU (SCES-00510) OPEN.EXE keep the same 64-byte frame.
- A `static inline` six-parameter key-on helper (KF1's shape), a public
  `inline` definition, and a K&R definition all give 40 bytes.
- An unused 24-byte local reproduces the frame, but no sibling shows such a
  local. KF1's `audio_play_voice` keeps none, and KF2 GAME uses a different,
  table-driven key-on routine. It is therefore not applied, per the
  evidence-gated leftover rule in `kf2-end-template-residue.md`.

The 24 bytes equal GCC's incoming-argument block for a six-parameter inline
callee (`FUNCTION_ARGS_SIZE`, `integrate.c`). That block is allocated only
when the inlined body references a parameter still resident in its stack
slot. No natural source form producing it has been found yet.
