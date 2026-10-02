# OPEN and END function and storage audit (SLPS-00069)

Hash-validated `retail/jp/END.EXE` was compared with eight fresh, isolated
`probe-gcc257-o2-plain` unit compiles. Strict objdiff reports **16/16 function
symbols at 100%**. The complete `.text` bytes also match in every unit, as do
the counts and ordered targets of the 306 text relocations. These are object
results, not a linked-EXE equality claim. The target BSS sections below are
curated delink models, not recovered original symbols or TU boundaries.
The adjacent `__main`/`__SN_ENTRY_POINT` pair and `_ExpAllocArea` are
archive-signature vendored controls in `functions_vendored.tsv`, outside the
16 game-function denominator.

| Unit | Function verdict | Initialized storage | Target BSS / candidate |
| --- | --- | --- | --- |
| `end.main` | `main`, `ending_load_data`: 100% each | 9-byte DATA exact | 4 B / `ending_data` COMMON 8 B |
| `end.display_init` | `display_initialize`: 100% | none | 24,816 B / three COMMON requests |
| `end.audio_init` | `audio_initialize`: 100% | none | 344 B / `audio_sequence_table` COMMON 344 B |
| `end.audio` | `ending_open_audio`: 100% | none | 26 B / four COMMON requests of 8 B each |
| `end.display_frame` | `display_begin_frame`, `display_present_frame`: 100% each | none | 4 B / `current_poly_ft4` COMMON 8 B |
| `end.cd_file` | `cd_file_load_into`: 100% | 11-byte DATA exact | none |
| `end.movie` | `ending_play_movie`: 100% | 31-byte RODATA exact | none |
| `end.movie_stream` | `strSetDefDecEnv`, `strInit`, `strCallback`, `strNextVlc`, `strNext`, `strSync`, `strKickCD`: 100% each | 8-byte DATA exact | 408,624 B / five same-sized COMMON requests |

The display target BSS has `display_current` at +0 (4 B), `display_buffers`
at +8 (8,424 B), and `display_primitives` at +8,432 (16,384 B). The audio
target has `audio_sequence_data`, `audio_vab_id`, `audio_vab_header`, and
`audio_sequence_id` at +0/+8/+16/+24, with sizes 4/2/4/2 B. Movie-stream
BSS is exactly `vlcbuf0` 163,840 B, `vlcbuf1` 163,840 B, `imgbuf` 15,360 B,
`Ring_Buff` 65,536 B, and `dec` 48 B, sequentially from `0x8003ae50` to
`0x8009ea80`. These complete claimed extents and the exact code references
support the current source objects. The compiler still emits exported COMMON
requests rather than those fixed sections; source declarations alone do not
prove the original allocation mechanism or placement.

The OPEN `0x800ac618` and END `0x800a93f8` `audio_sequence_table` identities
had stale `display` ownership and display-function evidence. Each retail image
has one validated incoming HI16/LO16 pair, at OPEN `0x8001221c/20` and END
`0x80011c98/9c`, in `audio_initialize` immediately before
`SsSetTableSize(table, 2, 1)`. The source defines the 344-byte SDK workspace
in `src/lib/audio_init.c`. Both identity rows now say owner `audio`, evidence
`audio_initialize`; the two reviewed relocation provenance fields now say
`manual:audio_initialize`. Addresses, sizes, confidence, relocation kind,
referent, and source are unchanged. Focused recompiles gave identical listings;
isolated strict objdiff kept both OPEN and END `audio_initialize` at 100%,
while their BSS sections remain 0%.

KF1's homologous audio workspace is private and has the same sole source use.
An off-tree KF2 `static` probe for both overlays emitted a 344-byte `.bss`
section and retained 100% function instructions, but its symbol is local and
its HI16/LO16 pair targets `.bss`; the current target model has a global named
symbol and the BSS section still scores 0%. The original KF2 linkage and
allocation contract remain unproved, so no C, profile, or BSS identity change
was retained. A distinct off-tree global `audio_sequence_table[...] = {0}`
control in both overlays kept GLOBAL binding but moved all 344 bytes into
`.data`; the address pair then targeted `.data`, and each strict initializer
score fell to 99.62963%. Explicit zero initialization cannot explain this
target BSS model, so the tentative source was retained.

The connected OPEN overlay was also freshly compiled and compared in ten
isolated units. Its **24/25** current function claims are strict 100%:

| OPEN unit | Function verdict |
| --- | --- |
| `main` | `main`, `opening_fade_out`, `opening_load_data`: 100% each |
| `display_init`, `audio_init`, `audio` | `display_initialize`, `audio_initialize`, `opening_open_audio`: 100% each |
| `title` | `opening_draw_title`, `opening_draw_banner`, `opening_draw_prompt`, `opening_poll_pad`, `primitive_buffer_begin_poly_ft4`, `primitive_buffer_commit_poly_ft4`: 100% each |
| `display_frame` | `display_begin_frame`, `display_present_frame`: 100% each |
| `cd_file` | `cd_file_load_into`: 100% |
| `resources` | `tim_upload_images`: 100%; `audio_play_voice`: 99.82353% WIP |
| `movie` | `opening_play_movie`: 100% |
| `movie_stream` | `strSetDefDecEnv`, `strInit`, `strCallback`, `strNextVlc`, `strNext`, `strSync`, `strKickCD`: 100% each |

Nine OPEN units have byte-identical complete text; `resources` differs only
in `audio_play_voice`'s 64-byte retail frame versus 40-byte candidate frame
(stack references and prologue/epilogue). All 779 ordered OPEN text relocation
tuples match. Its initialized DATA/RODATA sections are byte exact; six OPEN
units retain the same target BSS versus candidate COMMON class gap as END.
The voice frame has no independently evidenced source local to fill 24 bytes,
so it remains unattributed and unchanged.

Verification used focused compiles, strict object diffs, raw ELF
section/symbol/relocation review, and `git diff --check`; no repository tests,
lint, full build, or linked-EXE build were run.
