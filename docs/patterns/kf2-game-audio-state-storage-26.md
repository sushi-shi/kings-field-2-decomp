# GAME audio-state and VAB storage: 26 current strict controls

On 2026-10-02, eight connected GAME units were freshly compiled in isolation
under their current manifest `probe-gcc257-o2-g0` profiles and compared with
the delinked objects using strict `objdiff-cli diff`. The 26 selected sound,
voice, VAB, and direct caller functions below retain **24 exact matches and
two WIPs**. This pass tests storage ownership as well as text; it does not
promote the pinned probe to a proven historical compiler.

| GAME VA | Function | Strict verdict |
| --- | --- | --- |
| `8001398c` | `game_shutdown` | 100% |
| `800139c4` | `func_800139c4` | 89.30556% WIP; four unbound workspace constructors |
| `80013ae4` | `audio_start_sequence` | 100% |
| `80013b7c` | `audio_stop_sequence` | 100% |
| `80013bd4` | `audio_shutdown` | 100% |
| `80013c8c` | `audio_play_spatial` | 100% |
| `80013f50` | `audio_play_spatial_default_range` | 100% |
| `80013f84` | `audio_play_spatial_range` | 100% |
| `80013fb8` | `audio_key_off_handle` | 100% |
| `80014030` | `audio_update_listener` | 100% |
| `800140dc` | `audio_play_sound` | 100% |
| `80014100` | `audio_refresh_voice_handles` | 100% |
| `80014164` | `audio_allocate_voice_handle` | 100% |
| `80014278` | `audio_key_on` | 100% |
| `80014394` | `audio_vab_stream_callback` | 100% |
| `800144b8` | `cd_request_service_vab` | 94.87342% WIP; phase/retry register schedule |
| `800145f4` | `audio_acquire_vab_stream_slot` | 100% |
| `800146d0` | `audio_queue_vab_stream` | 100% |
| `80022300` | `func_80022300` | 100% |
| `80032274` | `resource_vab_update_range` | 100% |
| `80035504` | `map_object_play_spatial_sound` | 100% |
| `8003d084` | `func_8003d084` | 100% |
| `8003d0e8` | `func_8003d0e8` | 100% |
| `8003fa2c` | `effect_play_spatial_sound` | 100% |
| `80045e18` | `audio_play_sound_64` | 100% |
| `80045e3c` | `audio_play_sound_at_volume_100` | 100% |

The six selected units outside `audio_runtime` that have exact whole text
sections also match their ordered relocation site/type/referent tuples.
`resource_runtime` has unrelated map/TMD WIPs, so only its exact VAB-range
function is selected. The initialized `input_idle_counter` in the menu cue
unit matches all four `.data` bytes. The map-object unit's separate
`map_object_state` BSS/COMMON discrepancy is outside this owner verdict.

The GAME `audio_state` base is `0x80197630`. Retail main-loop startup clears
`0x3a7` words, exactly `0xe9c` bytes. The typed source layout reaches that
same end: a 0x2c-byte sequence/listener prefix, 130 eight-byte VAB records
through `+0x43c`, ten four-byte voice handles through `+0x464`, 256 ten-byte
voice parameter records through `+0xe64`, and seven eight-byte stream slots
through `+0xe9c`. Exact source functions independently address the sequence
fields, listener, handles, parameter rows, VAB IDs and stream slots. Thus the
runtime object has a complete supported extent `[0x80197630,0x801984cc)`;
the next curated `player_state` begins four bytes later at `0x801984d0`.
The retail target models one global `.bss` `audio_state` of 3,740 bytes,
whereas the candidate emits a global COMMON request of 3,744 bytes. The
four-byte request rounding and BSS/COMMON section difference are object
construction facts; retail linked bytes do not prove the original C
declaration, defining TU, or linker allocation rule.

The same target unit models a 344-byte global `.bss`
`audio_sequence_table` immediately before `audio_state`, giving a 4,084-byte
`.bss` section. The candidate emits a separate 344-byte global COMMON
request. `SsSetTableSize(table,2,1)` and pinned Psy-Q `SS_SEQ_TABSIZ=172`
support that 344-byte SDK-required workspace. The address and minimum size
are supported; the original declaration/linkage and any larger allocation
remain open. KF1's homolog is a private 344-byte table, while its sequence
buffer is dynamically requested at `0x3000`; neither source pattern proves
KF2's storage mechanism.

The initializer's four remaining fixed workspace pointers are independently
decoded signed-low `lui/addiu` pairs in retail. They are absent as symbols
and relocation pairs in the current candidate, whose integer literals emit
`lui/ori`. The fresh `.rel.text` counts are 178 target versus 170 candidate;
the multiset difference is exactly one HI16/LO16 pair per address, with no
candidate-only referent:

| Role | GAME address | Direct bound; original owner status |
| --- | --- | --- |
| Sequence data pointer | `0x80198640` | Next exact vendored SDK datum at `0x8019b640`, a `0x3000` candidate maximum; archive entry length and source owner unknown. |
| VAB stream slots 0–4 seed | `0x80165a68` | Five retained `0x1000` chunks end at `0x8016aa68`; combined with slot 6, six contiguous chunks occupy `[0x80164a68,0x8016aa68)`, but one versus multiple C objects is open. |
| VAB stream slot 5 override | `0x80194e30` | Starts at the end of `game_graphics_runtime`; next complete `audio_state` begins `0x2800` bytes later, a candidate maximum. |
| VAB stream slot 6 override | `0x80164a68` | Immediately precedes the slot 0 seed by `0x1000`; the direct pointer does not establish an allocation declaration. |

JP-to-US and JP-to-EU retail audio initializers preserve these four roles
while their addresses move with each regional image, corroborating referent
identity without establishing source/linker provenance. The first raw
initializer divergence at object `+0x64` is the `0x80198640` signed-low
pair versus a fixed literal; later register/order changes follow the same
missing-owner boundary. In `cd_request_service_vab`, retail retains phase
value `1` in `$s3` while the candidate retains retry sentinel `-1`; the
316-byte extent, call set, CFG, and ordered data referents agree. No
source-backed change follows from either WIP. No C, identity, relocation, or
profile edit was made, and no linked-EXE equality is claimed.

## Workspace owner follow-up

A full GAME.EXE word scan for **adjacent** `lui` plus signed-low consumers
found only the four initializer constructions of these exact bases:
`0x80013a28/2c` for `0x80198640`, `0x80013a90/94` for `0x80165a68`,
`0x80013ab4/b8` for `0x80194e30`, and `0x80013ac4/c8` for
`0x80164a68`. Later game code reaches the buffers through
`audio_state.sequence_buffer` or the stream-slot pointer fields.
`resource_transition_step` passes the sequence pointer to
`cd_archive_queue_read`; `audio_queue_vab_stream` passes a selected slot
pointer to `cd_archive_queue_stream_read`. Both CD helpers obtain their
request size from archive entry metadata, which is not present in the four
available retail EXEs. This establishes the consumers, not a maximum
payload size or a defining C array.

The pinned Psy-Q 3.0 `LIBSND.H` declares `SsSeqOpen` with a caller-supplied
`unsigned long *` and `SsVabTransBodyPartly` with a caller-supplied byte
pointer. The GAME-linked functions match the corresponding Psy-Q 3.0 archive
members `SSOPEN` (`SsSeqOpen`), `VS_VH` (`SsVabOpenHead`), and `VS_VTBP`
(`SsVabTransBodyPartly`); those members contain no BSS
definition for these game workspaces; `SSINIT` has its own distinct SDK
BSS symbols and accepts the separate 344-byte `SsSetTableSize` table.
Those member facts do not identify which game TU declared the four
buffers, whether their candidate gaps are full object extents, or whether
an original linker symbol or derived address was used. No source global,
identity, or additional relocation was invented.

The candidate upper boundaries repeat across all three regional GAME
images. Raw adjacent signed-low references occur at the address exactly
`+0x6000` after the slot-6 base (`0x8016aa68` JP, `0x8016b9ac` US,
`0x8016da90` EU) and at `+0x3000` after the sequence base
(`0x8019b640`, `0x8019c584`, `0x8019e630`). The corresponding
`audio_state` base lies `+0x2800` after the slot-5 override in each image
(`0x80197630`, `0x80198574`, `0x8019a628`), with four adjacent-pair
references to that base per region. These stable gaps strengthen the
capacity hypotheses. In JP, the `+0x6000` boundary is referenced by exact
LIBSND `_SsInit` and `SsUtKeyOn` code, and the `+0x3000` boundary by exact
LIBCD `CdReadCallback` and `CD_readm` code. Those xrefs identify the next
SDK data family, but unreferenced intervening storage and the original game
defining TU remain unproved.

The current safe 17-claim audio-runtime carve admitted all 178 module text
relocations and kept 15 functions exact; `func_800139c4` remains
89.30556% and `cd_request_service_vab` 94.87342%. Four connected sound
wrappers in a separate six-object safe carve were strict exact, bringing
this follow-up to 21 function verdicts: **19 exact, two WIP**. The
unresolved initializer is a source/owner question rather than an absent
raw address reference.

An unchanged-source off-tree GCC 2.6.0 O2 control lowered the audio-runtime
unit to 78.51635% strict text. The initializer fell from 89.30556% to
75.97222%, and all fifteen exact audio siblings became non-exact. This
compiler substitution neither proves a workspace owner nor warrants a
profile change; it was discarded.
