# OPEN voice wrapper frame residue

`OPEN.EXE` `audio_play_voice` at `0x80013450` is the sole non-exact function in
the current 25-function OPEN source census. Retail has an 0x88-byte body. Its
two proven callers are in `opening_poll_pad`; its sole proven callee is the
vendored Psy-Q 3.0 `SsUtKeyOn`. It has no string or data referent. The adjacent
`tim_upload_images` in `open.resources` remains exact.

The six-argument `s16` signature agrees with the two callers and the retail
stack halfword loads. The source preserves the all-zero program/tone/note
gate and forwards zero fine pitch plus the two signed volumes. The focused
`kf try --unit open.resources --context 0 --no-flow` rebuild leaves the TIM
function SAME. The voice wrapper has the same instruction sequence and control
flow except six stack-frame immediates: retail reserves 64 bytes, saves `$ra`
at +56, loads the two incoming stack arguments at +80/+84, and restores a
64-byte frame; the current GCC 2.5.7 plain O2 probe reserves 40 bytes, saves
at +32, loads at +56/+60, and restores 40 bytes. The direct `jal` target and
argument stores remain unchanged. The current strict report is 99.82353%,
which is WIP, not a byte-exact match.

An off-tree GCC 2.6.0 O2 compilation of the same source also reserves 40 bytes
and changes more of the schedule, so that simple compiler-version swap does
not account for retail's 64-byte frame. No source-backed local or alternate
prototype explains the 24-byte extra reservation. The current source stays
unchanged; this is an unattributed frame-layout residue, not a reason to add
padding, dead locals, or an assembly wrapper.

Fresh image-qualified retail review confirms the two `opening_poll_pad` calls
at `0x80013028` and `0x80013074`: each passes signed `audio_vab_id`, program
10, tone zero, note 60, and two stack volume arguments of 64. The wrapper
loads those volumes with `lhu` from its incoming stack slots, sign-extends
them before `SsUtKeyOn`, and has no data or string referent. Its only outgoing
relocation is the direct `jal` at `0x800134c0` to `SsUtKeyOn`; the raw call is
proven, while the curated relocation row remains candidate. A fresh focused
`open.resources` comparison again gives `tim_upload_images` SAME and the
voice wrapper DIFF only at the six frame-dependent immediates (64 versus 40
bytes). A fresh safe one-VA OPEN delink and manifest-profile compile reconfirm
99.82353% strict over the 136-byte retail body. The verdict remains WIP; no
source or relocation edit is supported by this pass.

## Current OPEN game-claim inventory (2026-10-02)

A fresh manifest-profile isolated rebuild of all ten current `open.*` units
and direct strict objdiff confirms **24/25 exact** function claims. These are
the per-function verdicts, grouped by unit:

| Unit | Strict-exact functions | WIP |
| --- | --- | --- |
| `open.main` | `main` `0x80011ac0`; `opening_fade_out` `0x80011f9c`; `opening_load_data` `0x80012060` | None |
| `open.display_init` | `display_initialize` `0x800120c8` | None |
| `open.audio_init` | `audio_initialize` `0x80012204` | None |
| `open.audio` | `opening_open_audio` `0x80012270` | None |
| `open.title` | `opening_draw_title` `0x80012560`; `opening_draw_banner` `0x80012b7c`; `opening_draw_prompt` `0x80012d7c`; `opening_poll_pad` `0x80012f3c`; `primitive_buffer_begin_poly_ft4` `0x800130ac`; `primitive_buffer_commit_poly_ft4` `0x800130fc` | None |
| `open.display_frame` | `display_begin_frame` `0x800131c8`; `display_present_frame` `0x8001322c` | None |
| `open.cd_file` | `cd_file_load_into` `0x80013284` | None |
| `open.resources` | `tim_upload_images` `0x800133e0` | `audio_play_voice` `0x80013450`, **99.82353%** |
| `open.movie` | `opening_play_movie` `0x800134f0` | None |
| `open.movie_stream` | `strSetDefDecEnv` `0x8001383c`; `strInit` `0x800138f8`; `strCallback` `0x8001396c`; `strNextVlc` `0x80013a78`; `strNext` `0x80013b0c`; `strSync` `0x80013bbc`; `strKickCD` `0x80013c30` | None |

Fresh image-qualified retail disassembly and xrefs reconfirm two proven
`opening_poll_pad` callers at `0x80013028`/`0x80013074` and the sole proven
`SsUtKeyOn` call at `0x800134c0`. Target and candidate `open.resources`
objects have the same six ordered `.rel.text` entries, including that call.
The focused unit has 5/5 CFG blocks, 3/3 branches and an exact TIM sibling;
the voice wrapper differs only at six frame-related immediates: retail
64-byte frame, `$ra` at +56 and incoming volumes at +80/+84, versus probe
40-byte frame, `$ra` at +32 and volumes at +56/+60. Its halfword loads,
sign extensions, all-zero sound gate, seven SDK arguments and call delay slot
agree. KF1's analogue calls a separate voice-slot helper and does not supply
an extra local or prototype for this direct SDK wrapper. No source-backed
frame owner was found, so the C and curated metadata stay unchanged.

The older [OPEN sentinel-sharing note](open-sentinel-sharing.md) discusses
`opening_ending_scroll_run` at `0x80014e28`; that identity and address are
absent from the current KF2 OPEN source, unit manifest and curated identity
table. Its scores are not a verdict for this 25-claim inventory.
