# END.EXE: template residue in frames and reloads (SLPS-00069)

END.EXE's `main`, `ending_open_audio` and `ending_play_movie` were copied from
OPEN.EXE siblings and then trimmed. After every instruction matched, three
residues remained, each in frame layout or in whether a global is reloaded.
GCC 2.4.1, 2.5.7 and 2.6.0 (the Psy-Q 3.0 kit's two compilers plus the probe)
all produce the same frames and the same register reuse from the trimmed
source. The residues therefore come from the source, not from the compiler
version.

## Mechanism

GCC decides some storage while it first generates RTL, before jump optimisation
and flow analysis remove dead code:

- **Outgoing argument area.** A call with a fifth argument grows
  `current_function_outgoing_args_size` to 20 bytes. The frame keeps that size
  even when the call is later deleted as unreachable.
- **Address-taken or aggregate locals.** A local whose address is taken (for
  example `CdControlB(CdlSetmode, &mode, 0)`), or an unused aggregate such as
  `RECT`, gets a stack slot when it is expanded. The slot stays if every use
  disappears.
- **Memory CSE after a store.** After `global = p; call(global)` GCC passes `p`
  in a register. It reloads the global only when `p` changed between the store
  and the use. The reload therefore records a pointer update, dead or not,
  that happened in between.

Controls (cc1psx-257, -O2 -G0): an unused `RECT` gives `vars= 8`; an unused
address-taken pair used only in `if (0)` code gives `vars= 8`; an unused pair
of scalars gives `vars= 0`.

## Applied reconstructions (user-approved, evidence-gated)

| Function | Residue | Sibling evidence | Source |
| --- | --- | --- | --- |
| END `ending_play_movie` | 72-byte frame: fifth-argument slot at +16, 8 bytes at +48 | OPEN's movie loop has the same 72-byte frame; after its stream it clears callbacks, calls `CdControlB(CdlSetmode, &mode, 0)` (mode at +48) and four 5-argument `SetDef*Env` calls | OPEN's cleanup kept as unreachable code after END's `for (;;)` |
| END `ending_open_audio` | reload of `audio_vab_header` and `audio_sequence_data` after storing them | OPEN's VAB/SEQ loader advances its cursor past every section | the cursor advances past each section before the next call; the final advance is the sequence size, which runs to the end of ED.D (262932 bytes) |
| END `main` | 8 unused bytes at +16 | OPEN `main` keeps a `RECT` and an address-taken pair in its frame | an unused `RECT` declaration |

These are hypotheses about the original trimmed template. They are not
permission to add unused storage elsewhere: each needs a sibling that shows
the original construct. KF1's unattributed frame residues (see
`menu-backdrop-stack-frame.md`) may deserve a search for unreachable
template code of the same kind.

## Count-down loops

Retail `cd_file_load_into` and `ending_load_data` decrement a counter and
compare it with -1. None of GCC 2.4.1, 2.5.7 or 2.6.0 turns a counting-up
`for` loop into that form (a DOSBox control of the kit compilers), so the
source spells it: `while (--sectors != -1)` and
`for (attempt = N - 1; attempt != -1; attempt--)`.
