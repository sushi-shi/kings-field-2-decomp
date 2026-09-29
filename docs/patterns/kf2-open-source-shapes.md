# OPEN.EXE: source shapes and one open residue (SLPS-00069)

OPEN.EXE's 25 game functions were reconstructed under
`probe-gcc257-o2-plain`. 24 match exactly. These source facts decided the
last differences.

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
