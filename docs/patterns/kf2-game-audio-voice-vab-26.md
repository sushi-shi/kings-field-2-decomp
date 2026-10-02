# GAME audio voice and VAB call family: 26 strict verdicts

The current Japanese `GAME.EXE` audio call family has **24 strict-exact
functions and two WIPs** among the 26 selected claims below. This is a fresh
focused comparison after the `4a1608b` checkpoint, not a promotion of the
older KF1 names or a loose listing score. No source, identity, relocation,
compiler-profile, or bank change follows from this pass.

`kf init --retail-dir retail/jp` verified the pinned retail files. For every
selected GAME address, the `kf sema --image game` address, block disassembly,
incoming and outgoing xrefs, strings, and match views were read. The functions
form a direct call/data family: the runtime owns `audio_state`, its sequence,
voice, and VAB slots; shutdown, menu, actor, effect, and map-object sound
callers reach that runtime; the resource range updater reaches the VAB queue.
The unit/source history and adjacent claims were checked before considering
a C change. No selected function appears in `functions_vendored.tsv`; the
Sony `Ss*`, `Spu*`, `Cd*`, and `VSync` routines are callees, while these claims
implement game-specific policy around them. None of the selected bodies has a
local string reference.

Focused `kf try --unit ... --context 0 --no-flow` rebuilt only
`game.game`, `game.audio_runtime`, `game.menu_sound_cue`,
`game.resource_runtime`, `game.map_object_reset`,
`game.actor_spatial_sound`, `game.effect_spatial_sound`, and
`game.audio_sound_wrappers`. An isolated eight-unit `objdiff-cli report
generate` project used the fresh candidate objects and delinked retail target
objects with `functionRelocDiffs: all`. The table uses those **strict** scores;
`SAME` from `kf try` alone is not its verdict.

| GAME VA | Current identity | Strict verdict | Retail/source evidence |
| --- | --- | --- | --- |
| `8001398c` | `game_shutdown` | Exact 100% | Direct audio shutdown followed by CD, pad, and graphics teardown. |
| `800139c4` | `func_800139c4` | WIP 89.30556% | Eight Sony sound calls agree; four fixed workspace addresses lack source owners and candidate relocations. |
| `80013ae4` | `audio_start_sequence` | Exact 100% | Music-enable/ready gates, sequence open, volume, play, and master volume agree. |
| `80013b7c` | `audio_stop_sequence` | Exact 100% | Active sequence stop/close and state clear agree. |
| `80013bd4` | `audio_shutdown` | Exact 100% | Sequence shutdown, 130 VAB slots, and `SsEnd` agree. |
| `80013c8c` | `audio_play_spatial` | Exact 100% | Distance, layer attenuation, pan, and voice call agree with all ordered referents. |
| `80013f50` | `audio_play_spatial_default_range` | Exact 100% | Signed halfword volume and fixed range forwarder agree. |
| `80013f84` | `audio_play_spatial_range` | Exact 100% | Signed halfword volume and stack argument forwarding agree. |
| `80013fb8` | `audio_key_off_handle` | Exact 100% | Handle's sound parameter and VAB guard lead to the Sony key-off call. |
| `80014030` | `audio_update_listener` | Exact 100% | Position, collision layer, and rotation updates agree. |
| `800140dc` | `audio_play_sound` | Exact 100% | Mono volume is forwarded to both voice channels. |
| `80014100` | `audio_refresh_voice_handles` | Exact 100% | SPU key status retires inactive handles. |
| `80014164` | `audio_allocate_voice_handle` | Exact 100% | Free, same-sound, then oldest-handle selection agrees. |
| `80014278` | `audio_key_on` | Exact 100% | Effect enable, VAB readiness, note clamp, and `SsUtKeyOn` agree. |
| `80014394` | `audio_vab_stream_callback` | Exact 100% | CD corruption retry, VAB header open, and next transfer phase agree. |
| `800144b8` | `cd_request_service_vab` | WIP 94.87342% | Call set and 11-block CFG agree; retail retains phase value `1` in `$s3`. |
| `800145f4` | `audio_acquire_vab_stream_slot` | Exact 100% | Free-slot scan and completed-slot/VAB reclaim agree. |
| `800146d0` | `audio_queue_vab_stream` | Exact 100% | VAB slot assignment and CD archive stream callback agree. |
| `80022300` | `func_80022300` | Exact 100% | Menu cue branches call voice and sequence tick; cue 13 pans across two VSync waits. |
| `80032274` | `resource_vab_update_range` | Exact 100% | Flagged VAB range queues missing streams or changes stream state. |
| `80035504` | `map_object_play_spatial_sound` | Exact 100% | Map-object position and volume 120 forward to spatial audio. |
| `8003d084` | `func_8003d084` | Exact 100% | Actor sound note offset clamps a signed byte and adds bounded randomness. |
| `8003d0e8` | `func_8003d0e8` | Exact 100% | Target sound byte selects normal or explicit spatial range. |
| `8003fa2c` | `effect_play_spatial_sound` | Exact 100% | Effect position and fixed volume/range forward to spatial audio. |
| `80045e18` | `audio_play_sound_64` | Exact 100% | Fixed sound `0x40` and volume 100 forwarder. |
| `80045e3c` | `audio_play_sound_at_volume_100` | Exact 100% | Supplied sound and fixed volume 100 forwarder. |

The initializer's first substantive raw difference is at object `+0x64`:
retail uses a paired `R_MIPS_HI16`/`R_MIPS_LO16` `lui/addiu` reference to
`DAT_80198640`, while the C integer pointer literal emits `lui/ori` and no
relocation. The same mismatch occurs at `+0xcc`, `+0xf0`, and `+0x100` for
`DAT_80165a68`, `DAT_80194e30`, and `DAT_80164a68`. The fresh audio target
has 178 `.rel.text` entries and the candidate 170: exactly these four missing
HI16/LO16 pairs. Retail proves address use, not the original allocation
definition or complete object extents. The current initializer still has the
same Sony call order and seven-block CFG; the remaining register/order drift
is downstream of the missing referents. A guessed `DATA` or BSS owner would
misstate the source model, so the source remains unchanged.

The VAB service's retail first relevant schedule loads `1` into `$s3` at
`0x8001450c`, compares the `SsVabTransBodyPartly` result with `-1` in `$v0`,
then stores `$s3` as request phase and completed stream state. The current
probe holds retry sentinel `-1` in `$s3`, materializes phase `1` later, and
moves two state stores. Both have the 316-byte body extent, the decoded
11-block CFG, the same Sony/CD calls, and the same ordered data referents.
The first source-backed mismatch has not been found. Earlier alternative loop
spellings in `game-audio-cd-batch.md` lowered the focused comparison; no
carrier local or assembly steering is justified. This remains an unattributed
code-generation residue under the pinned probe, whose historical identity is
still unproved.

The surrounding isolated units contain unrelated claims, including current
resource map-mask/TMD WIPs; their scores are outside this 26-function audio
verdict. There was no new exact result to bank. No repository test, lint, full
build, or linked executable build was run for this bounded pass.

## VAB slot/queue relocation review

Ten formerly candidate GAME relocations in the exact VAB slot/queue pair
now have direct raw and source evidence. In `audio_acquire_vab_stream_slot`,
`0x80014628` and `0x80014690` jump to its return epilogue,
`0x80014698` calls SDK `SsVabClose`, and `0x800146a4` rejoins the slot-state
store. In `audio_queue_vab_stream`, `0x80014718` enters the default slot
path, `0x80014720` joins the fixed-slot path, `0x80014728` calls the slot
allocator, and `0x80014754` calls `cd_request_wait_done`. The adjacent
`lui a3`/`addiu a3` at `0x80014770/74` resolves exactly to
`audio_vab_stream_callback` (`0x80014394`), passed as the fourth argument
to the `cd_archive_queue_stream_read` call at `0x80014778`. The compiled
object has corresponding `R_MIPS_26` or HI16/LO16 sites at all ten
relative offsets. These ten rows were promoted to reviewed without changing
source or storage ownership.

A fresh safe 17-VA audio-runtime carve admits 356 relocations with none
withheld. Direct isolated strict comparison leaves 15/17 functions exact,
including both VAB slot/queue functions at 100%; only the initializer
(89.30556%) and VAB service (94.87342%) remain WIP. The two standalone
VAB target objects are hash-identical before and after adding their curated
callee names; no text, data, or relocation bytes changed from that metadata
refinement. The four unresolved initializer workspace referents remain
outside this reviewed ten-site set.
