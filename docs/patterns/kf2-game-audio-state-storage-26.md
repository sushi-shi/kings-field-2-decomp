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

The `SSINIT` member makes the slot-6 upper boundary more specific: its
`_SsInit` text patch at member offsets `+0xfc/+0x100` names the member's
8-byte XBSS `_snd_ev_flag`. The exact GAME-linked `_SsInit` begins at
`0x80050bbc`; its raw `lui`/`sw` at `0x80050cb8/bc` resolves the signed-low
address `0x8016aa68`. This identifies the datum immediately after the
candidate six-slot region, without proving how that preceding game region
was declared.

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

## Sound-action caller graph

A fresh safe carve selected 25 GAME sound-action claims outside
`audio_runtime` and withheld no relocation or function. Isolated strict
objects from the current source gave **17 exact, eight WIP**. The graph is
linked by direct `audio_play_sound`, `audio_play_spatial_*`, or their typed
map-object, effect, and actor wrappers; it includes the callers' exact
siblings as controls.

| Unit | Exact claims | WIP claim and strict text |
| --- | --- | --- |
| `actor_spatial_sound` | `0x8003d084`, `0x8003d0e8` | — |
| `player_collision_sound` | `0x80027928`, `0x80027988` | `0x800279cc`: 98.23967% |
| `player_magic_dispatch` | `0x80026498` | `0x8002665c`: 98.67857% |
| `map_object_reset` | `0x80035504`, `0x80035534`, `0x80035590`, `0x800355d8`, `0x800356ac`, `0x800357a0` | — |
| `map_object_motion` | `0x80036b68` | — |
| `map_object_action_update` | — | `0x80036ed4`: 99.63745% |
| `render_resource_dispatch` | — | `0x8003247c`: 92.18579% |
| `effect_spatial_sound` | `0x8003fa2c` | — |
| `effect_constructor` | — | `0x80040308`: 98.43441% |
| `effect_update_dispatch` | — | `0x80042650`: 95.18370% |
| `event_command_dispatch` | `0x8004678c` | — |
| `audio_sound_wrappers` | `0x80045e18`, `0x80045e3c`, `0x80045e5c` | — |
| `actor_group_effects` | — | `0x8003c614`: 86.04192% |
| `actor_behavior_dispatch` | — | `0x8003d184`: 99.93160% |

The `player_collision_sound` WIP is the clearest remaining relocation-count
gap in this caller set: retail has 141 and the candidate 137 text rows.
At GAME `0x80027d0c/10`, retail forms `player_state+0x13a` and at
`0x80027d14/18` independently forms `player_state+0x110`. At
`0x80027d40/44` it forms `+0x110` again for a store. The candidate retains
an earlier `player_state+0x138` pointer, using `lh -40(a0)` and then
`addiu a0,-40; sh 0(a0)` for the same `+0x110` field. These are the two
missing HI16/LO16 pairs. The three-function module still has 70/70 CFG
blocks, 37/37 branches, matching call roles, and two exact siblings;
there is no missing datum or source-backed reason to manufacture an address
carrier. No C or identity change was made.

Seven direct-control rows in the exact `actor_spatial_sound` two-function
module were individually decoded from GAME retail words and promoted to
reviewed: internal jumps at `0x8003d0a8` and `0x8003d148`; calls to
`rand` at `0x8003d0bc`, `func_8003d084` at `0x8003d118` and
`0x8003d150`, `audio_play_spatial_range` at `0x8003d140`, and
`audio_play_spatial_default_range` at `0x8003d168`. All seven raw `j`/`jal`
targets agree with the source and the exact isolated object. A new two-VA
safe carve admitted all 14 relocations with zero withheld, and both claims
remain strict 100%. The target module SHA-256 was unchanged from the prior
candidate-tier carve (`95edbdee5d75e3ac1592b0d7ce94dcc8aa70c5de0699a39637315f6607364cde`).

The remaining 16 candidate control rows in `player_magic_dispatch`
`0x8002665c` were also decoded individually. Every source word is a raw
`j` (`opcode 2`) whose encoded target matches its curated row and lies
inside this function. Sites `0x800266f0`, `0x80026b28`, `0x80026b5c`,
`0x80026b74`, `0x80026da0`, `0x8002710c`, and `0x800271dc` share the
`0x800271fc` destination; `0x80026710/1c` share `0x8002672c`.
The other seven sites are `0x800266b8`, `0x800267c8`, `0x800268c4`,
`0x80026bb0`, `0x80026ca4`, `0x80026e6c`, and `0x80026ec8`, each with
its individually decoded local target. Their delay-slot words were retained
in the raw audit. These rows are reviewed without changing the source:
a new two-VA safe carve admits 525 aggregate relocations, zero withheld;
the 256-row module text relocation table and target module SHA-256 remain
unchanged (`951e40262d58ba2f297c395e782697f819a1d4edd24dcfea10ea77d864fe55b5`).
The neighboring `0x80026498` stays strict exact, while `0x8002665c`
remains 98.67857% with its already bounded `player_state` address reuse.
