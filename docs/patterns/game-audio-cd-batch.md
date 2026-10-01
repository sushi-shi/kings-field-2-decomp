# GAME audio and CD request batch

This GAME.EXE campaign selected functions through calls, the CD request ring,
the audio state, and archive entry readers. The exact rows have strict
objdiff `100.000000000%` results; `kf try` also reports identical listings.
WIP means inspected but not yet matched, not a vendored claim.

| GAME VA | Verdict | Evidence or next ownership question |
| --- | --- | --- |
| `0x800139c4` | WIP | Main-loop audio initialization calls eight Sony sound APIs, clears VAB/voice slots, and seeds seven stream buffers; fixed RAM addresses and neighboring CD/audio state owners still need attribution. |
| `0x80013ae4` | Exact | `audio_start_sequence`: opens and plays the selected sequence when music is enabled. |
| `0x80013b7c` | Exact | `audio_stop_sequence`: stops and closes the active sequence. |
| `0x80013bd4` | Exact | `audio_shutdown`: closes the active sequence and the 130 VAB slots. |
| `0x80013c8c` | Exact | Spatial audio core scales position deltas by eight before `SquareRoot0`, samples map elevation through `func_8002a988`, adjusts attenuation from the collision-cache halfword, then calls the angle/trig helpers and `audio_key_on`. The KF1 counterpart lacks this KF2 map-height path. |
| `0x80013fb8` | Exact | `audio_key_off_handle`: resolves voice parameters and VAB slot before `SsUtKeyOff`. |
| `0x80014030` | Exact | Listener update copies position and rotation to audio state and calls `func_8002a988`; the halfword at `0x801d8d4a` is accessed through the temporary collision-cache view. |
| `0x80014100` | Exact | `audio_refresh_voice_handles`: frees handles whose SPU keys are no longer active. |
| `0x80014164` | Exact | `audio_allocate_voice_handle`: selects a free, same-sound, or lowest-age handle. |
| `0x80014278` | Exact | `audio_key_on`: resolves sound parameters, allocates a voice, then calls `SsUtKeyOn`. |
| `0x80014394` | Exact | `audio_vab_stream_callback`: moves completed CD stream chunks into the VAB transfer. |
| `0x800144b8` | WIP | `cd_request_service_vab` is reconstructed with the retail calls and CFG, but GCC selects different constants for the retry/state paths. |
| `0x800145f4` | Exact | `audio_acquire_vab_stream_slot`: closes an occupied VAB slot and returns a free stream slot. |
| `0x800146d0` | Exact | `audio_queue_vab_stream`: queues the selected VAB archive entry for streaming. |
| `0x800167bc` | Exact | `resource_transition_set_phase_1`: callback writes phase 1 to the shared transition state. |
| `0x800167d0` | Exact | `resource_transition_set_phase_3`: callback writes phase 3. |
| `0x800167e4` | Exact | `resource_transition_set_phase_2`: callback writes phase 2. |
| `0x800167f8` | Exact | `resource_transition_set_phase_4`: callback writes phase 4. |
| `0x8001680c` | Exact | `resource_transition_set_phase_6`: callback writes phase 6. |
| `0x80016f10` | Exact | `cd_stream_limit_chunk`: caps a kind-`0x40` transfer at sixteen sectors and reduces its remaining count. |
| `0x80016f4c` | Exact | `cd_request_service_stream`: image-stream parser, partial `LoadImage` rows, header checks, seek, and completion now strictly match. |
| `0x800171c8` | Exact | `resource_copy_words`: copies a counted word run and returns the advanced source pointer. |
| `0x800171f8` | Exact | `resource_copy_halfwords`: the paired halfword copy loop. |
| `0x80017228` | Exact | `repeat_store_word`: fills a counted word run and returns the advanced destination pointer. |
| `0x8001724c` | Exact | `repeat_store_halfword`: the paired halfword fill loop; its 0x24-byte body was previously an unclassified data gap. |
| `0x80017270` | Exact | `memory_arena_coalesce_free`: merges adjacent free block headers when the resource allocator searches the chain. |
| `0x800172f4` | Exact | `memory_arena_free`: marks a block free, clears its tracked owner pointer, and removes the back-reference. |
| `0x80017314` | Exact | `memory_arena_find_block`: first searches for a free block, then reclaims a sufficient run of kind-1 blocks through the coalescing/free helpers. |
| `0x8001746c` | Exact | `memory_arena_wait_pending` waits through kind-3 blocks with `cd_request_wait_idle`; a nested `while` preserves the retail reread and top test. |
| `0x80017504` | Exact | `memory_arena_compact`: waits for pending CD blocks, frees kind-1 blocks, copies live blocks toward the arena start, and updates their owner pointers. |
| `0x800175e8` | Exact | `memory_arena_initialize_blocks`: initializes the first free block and final sentinel at the caller-provided arena boundary. |
| `0x80017608` | WIP | `memory_arena_allocate_block`: call set, CFG, owner update, and block split match; a two-step header subtraction now leaves only one temporary-register choice (99.78261%). |
| `0x800177d4` | Exact | `cd_vsync_handler`: increments both CD counters. |
| `0x80017804` | Exact | `cd_wait_two_vsyncs`: waits with critical sections and `VSync(0)`. |
| `0x80017864` | Exact | `cd_request_advance`: clears and advances the ring, then seeks or pauses. |
| `0x800178d0` | Exact | `cd_complete_handler`: dispatches read kinds and phases; kind `0x10` completion passes the destination to its callback, while other read kinds pass the request. |
| `0x80017a1c` | Exact | `cd_data_ready_handler`: transfers one sector, invokes callback, advances request. |
| `0x80017a98` | Exact | `cd_error_handler`: empty callback body in retail. |
| `0x80017c2c` | Exact | `cd_request_wait_idle`: waits for empty ring and idle current slot. |
| `0x80017ca8` | Exact | `cd_request_wait_done`: waits for a specified slot to become idle. |
| `0x80017d54` | Exact | `cd_request_enqueue`: writes both locations, destination, sector count, callback, and advances tail. |
| `0x80017e6c` | Exact | `cd_archive_entry_extent`: computes CD location and byte extent. |
| `0x80017ef4` | Exact | `cd_archive_queue_read`: queues a kind `0x10` entry read. |
| `0x80017f48` | Exact | `cd_archive_queue_read_kind_20`: the previously unclassified 0x54-byte gap is a parallel kind-`0x20` entry-read wrapper with identical call structure. |
| `0x80017f9c` | Exact | `cd_archive_queue_stream_read`: queues a kind `0x30` entry read. |
| `0x80017ff0` | Exact | `cd_archive_read_chunked`: queues a kind `0x40` read, capped at sixteen sectors. |
| `0x80031fa0` | Exact | `resource_registry_get`: retrieves a registered asset, checking its memory-block kind for dynamic IDs. |
| `0x80032008` | Exact | `resource_tmd_read_complete`: prepares primitive indices and marks the loaded block ready. |
| `0x800321d8` | WIP | `resource_tmd_queue_read`: call set and CFG match; its fixed arena address is emitted as `lui`/`ori` by the probe, where retail has `lui`/`addiu`. GAME main-loop initialization also passes `0x8009b0a0` to `func_800175e8`, but the source origin remains unresolved. |
| `0x80032274` | WIP | `resource_vab_update_range`: correct audio VAB slot and call set; the probe strength-reduces the slot index and saves one extra register. |
| `0x80032364` | Exact | `resource_tmd_update_range`: queues missing TMD blocks and changes memory-block kind according to the resource flags. |
| `0x80036e24` | WIP | Call-connected frame loop services both CD streams and renders each brightness step; the focused object has only s2/s3/s4 assignment differences for mode, endpoint, and step. |

All rows above touch GAME-owned CD/audio state or call its helpers. No row was
attributed to a Sony archive body. The nearby `0x80045e5c` was excluded after
disassembly: it is an effect trajectory using `rsin` and `rcos`, not a sound
wrapper.

The pinned assembler emits a section-relative `R_MIPS_26 .text` when
`cd_archive_queue_read` directly calls the preceding function in its unit.
The target and compiled objects have the same five ordered relocations:
`cd_archives` HI16/LO16, `cd_location_add` MIPS26, `.text` MIPS26,
`cd_request_enqueue` MIPS26. The delinker now applies this same-unit direct-call
rule in addition to the existing same-unit function-address rule.

The initial GAME target relink passed for all units when these functions were
matched. Later shared-tree checks may also encounter in-progress module and
data ownership changes; those do not change the focused strict objdiff
verdicts above.

The fill-loop carve completed a contiguous memory run from `0x800171c8` to
`0x800177d4`. All 22 functions now share `src/game/memory.c`; 21 are strict
exact. Only the allocator split at `0x80017608` remains WIP.

The later CD/memory follow-up joined the adjacent stream service with that
memory run. `game.memory` now owns all 26 contiguous functions from
`0x80016ed4` through `0x8001779c`; the module ends at `0x800177d4`.
Strict exact addresses are `0x80016ed4`, `0x80016ee0`, `0x80016f10`,
`0x800171c8`, `0x800171f8`, `0x80017228`, `0x8001724c`, `0x80017270`,
`0x800172f4`, `0x80017314`, `0x8001746c`, `0x80017504`, `0x800175e8`,
`0x800176c0`, `0x800176e0`, `0x800176e8`, `0x800176f4`, `0x800176fc`,
`0x80017708`, `0x80017710`, `0x8001771c`, `0x80017754`, `0x8001777c`,
and `0x8001779c`. The two WIPs retain their previous strict scores:
`cd_request_service_stream` `0x80016f4c` is 96.61006%, with 29/29 CFG blocks
and the height-load/delay-slot and frame-size residue; allocator
`0x80017608` is 96.934784%, with 8/8 CFG blocks and the block-split
arithmetic ordering residue. No listing regressed, and one module was removed.

The kind-`0x20` carve closed the archive-wrapper gap. The contiguous
`0x800177d4`–`0x80018764` CD request, location, checksum, and archive run now
shares `src/game/cd_archive.c` in one module, eliminating seven separate
modules. All 31 functions are strict exact. For `cd_archive_open` at
`0x800184d0`, the retail stack places the read table at `sp+0x10` and the next
`CdlFILE` local at `sp+0x850`, establishing a `0x840`-byte table-local extent.
Giving that local the observed extent restored retail's 2232-byte frame and
kept the other archive functions exact. The original reason for reserving the
extra 64 bytes beyond one CD sector is still unknown.

The complete memory and archive runs are now one contiguous
`game.cd_memory` source from `0x80016ed4` to `0x80018764`, removing one more
GAME module. Focused comparison is 55/57 identical listings; strict objdiff
preserves all 55 exact functions and leaves the same two WIPs at
`0x80016f4c` (96.61006%) and `0x80017608` (96.934784%). The archive's
initialized DATA (11 bytes) and RODATA (33 bytes) remain 100% exact.
For the current 26-function memory/CD-stream subcampaign, all 24 exact
addresses enumerated above remain exact after the merge; the two WIPs retain
their prior scores. The source-backed experiment of branching directly on the
rectangle height emitted the same non-exact stream-service listing and was
reverted.

The previously unclassified `0x800167bc`–`0x80016820` gap contains five
0x14-byte phase setters whose addresses are passed by the adjacent resource
dispatcher. Each stores a distinct halfword to
`state_8017d118.transition_phase`; all five are strict exact after curating
the BSS relocation pairs.

The follow-up survey also inspected these confirmed-call neighbors. Each
remains WIP because its referenced runtime object or full control flow is not
yet reconstructed; none is counted as an exact match.

| GAME VA | WIP evidence and missing model |
| --- | --- |
| `0x80015d58` | Opens seven `.T` archives, starts a map stream and VAB stream, then copies startup assets; the source data and session-state owners remain incomplete. |
| `0x80015fd4` | Starts the callback-state transition, pumps `cd_request_yield` and `func_80016820`, and installs a TMD; the object at fixed RAM `0x8012da68` lacks an owner and source mechanism. |
| `0x800160e8` | Rebases three coordinates across actor, map-object, and placed-resource arrays; their shared extents and member layouts need stronger ownership evidence. |
| `0x80016260` | Dispatches map/session transitions, including `audio_start_sequence`, `cd_request_yield`, and the save serializer; its large state machine remains unmodelled. |
| `0x80016820` | Uses a seven-entry jump table for the transition phase, queues CD archive reads, and calls the five exact phase callbacks; its resource state and switch ownership remain open. |
| `0x8002360c` | Repeats CD yield/wait and resource transition calls across a map change; player and render state also participate. |
| `0x80031850` | Called repeatedly by the map-resource update at `0x8003247c`; the render/registry state it mutates is not yet modelled here. |
| `0x80031d8c` | Another `0x8003247c` callee in the map-resource path; its mixed data and call references require an owner model. |
| `0x8003247c` | Large map-resource update calls both exact TMD/VAB range helpers, `repeat_store_word`, and sound playback; its map-cell and render state are separate ongoing campaigns. |
| `0x8004678c` | Event workflow calls CD services and spatial/sound wrappers, with an unresolved indirect jump and multiple state families. |
| `0x800475d8` | Paired event workflow also pumps CD services; its state branches and data ownership are not yet reconstructed. |
| `0x800482f8` | WIP, strict 90.38636%: clears the control and arena portions of the now-typed 0x3918-byte `event_state`, initializes the arena, ten saved offsets, and `game_counter_bytes`. CFG/calls agree; the first residue is the choice of retained control-field base and store order. |
| `0x800483a8` | Exact: invokes active callback-table slot 1 with zero. Its earlier exact listing survives consolidation into `game.event_state`. |
| `0x800483d8` | WIP, strict 89%: expands ten saved u16 offsets to pointers relative to the arena base. CFG and referents agree; base/sentinel register allocation differs. |
| `0x80048428` | Exact: adjusts tracked arena-owner pointers by a positive relocation delta. |
| `0x80048498` | WIP, strict 88.68421%: contracts ten saved pointers to u16 offsets. CFG and referents agree; base/sentinel register allocation differs. |
| `0x800484e4` | Exact: adjusts tracked arena-owner pointers by a negative relocation delta. |
| `0x80048554` | Serializes actor, registry, and map-object records and calls `memory_arena_allocate_block`; its large packed format is not yet typed. |
| `0x800489ac` | Decodes that packed format into map objects through a switch table and `map_object_reset`; the record schema remains open. |

The event-counter follow-up proved one BSS owner at `0x8009a5e8` from the
`0x800482f8` clear of 30 words. Seven event-workflow calls pass byte indices
to `0x800473e0`; the menu reads byte `+0x60`, so the former standalone
`0x8009a648` identity is an interior reference. Both `0x800473e0` (decrement
unless zero) and `0x80047434` (increment unless at 99, then notify) are strict
exact in one contiguous unit. The latter calls active callback-table slot 6
only after increment. The menu owner's updated interior read also remains
strict exact. The counter unit has seven reviewed BSS relocation pairs and
reviewed control-flow relocations in address order.

The adjacent seven-argument channel transition at `0x800474c4` is now in the
same contiguous `game.event_counter` unit. All three functions retain strict
100% listings after the merge, reducing the GAME module count by one.

The event-state follow-up proved a separate 0x3918-byte BSS owner at
`0x801b2140`: startup clears the whole object, while `0x800482f8` clears the
control region at +0x04 and the arena at +0x104, then seeds ten u16 offsets
at +0x3904. A shared `KfEventState` declaration records those extents and the
known arena-block field; unknown control fields remain address-derived. Six
contiguous event functions now share `src/game/event_state.c`; three are strict
exact, including the preserved callback and the two arena pointer adjusters.

The same follow-up restored strict exactness to `0x8001746c`: the retail
top-tested kind-3 wait loop performs an additional byte load before the first
wait, which the natural nested `while` emits. The other 21 memory-unit
functions kept their prior verdicts; only `0x80017608` remains non-exact.

The contiguous `0x80016ed4`–`0x800171c8` CD stream unit now contains the two
strict-exact wrappers, the strict-exact chunk limiter, and the reconstructed
image-stream service, eliminating one separate module. For `0x80016f4c`, the
focused probe has the same 29 blocks, 16 branches, one return, and ordered call
set as retail; the strict objdiff score is 96.610060000%, so it remains WIP.
Its first instruction divergence is
the signed height load at +0x5c: retail loads into `a1` and moves it to `s3`
in the following branch's delay slot; the probe loads into `s3`, inserts a
NOP, and shifts later blocks four bytes. The probe allocates 40 stack bytes
versus retail's 48. This is an unattributed codegen residue; no artificial
local or padding was retained.

## Event and listener call-graph survey

This follow-up followed direct calls from the event controllers into sound,
CD, collision, and save-state helpers. The exact rows below are existing
source-owned results from the responsible neighboring campaigns, verified as
strict 100%; this survey does not claim their source. WIP rows identify the
remaining owner or model needed before a source claim can be made safely.

| GAME VA | Verdict | Retail evidence or next model |
| --- | --- | --- |
| `0x800139c4` | WIP | Sound startup calls eight `Ss*` APIs, then seeds the VAB and stream slot arrays; fixed RAM table/buffer source origins remain open. |
| `0x80014030` | WIP | Listener update calls the map-cell height lookup and uses the unresolved collision cache near `0x801d8d40`. |
| `0x8001586c` | Exact | Event-linked 12-bit angle interpolation leaf; current owner is the fixed-interpolation unit. |
| `0x8002a988` | WIP | Signed x/y/z map-cell height lookup reads the two five-byte layer elevations. Its fallback at `0x800667fc` is an unclassified loaded-data interior, and the collision cache at `0x801d8d40`–`0x801d8d4c` overlaps a provisional player equipment extent. |
| `0x8002aaa4` | WIP | 0xb60-byte collision dispatcher called by the listener and event restore; its switch data and output schema need a separate owner model. |
| `0x8002b604` | WIP | Five-argument collision wrapper calls the height lookup and dispatcher, then reads a byte through the unresolved collision-cache pointer. |
| `0x8002b7f8` | Exact | Height-adjusted collision probe calls the same lookup and dispatcher; existing collision unit is strict exact. |
| `0x8002c670` | WIP | 0x7bc-byte render/map scan calls the height lookup and many mask helpers; graphics-runtime field family remains separate. |
| `0x800335a0` | WIP | 0x3f4-byte event frame presentation callee with 31 data/control references; source ownership remains unclassified. |
| `0x80034e10` | WIP | Event controller calls this 0x180-byte archive-slot/entry operation; shared map-resource ownership remains unresolved. |
| `0x80036190` | WIP | Existing map-object spatial query has a non-exact focused listing; map-object owner retains it. |
| `0x800368b4` | WIP | Event controller calls this 0x90-byte indirect-jump dispatch with a candidate table at `0x800118c4`. |
| `0x80038f20` | WIP | Event controller calls this 0xd0-byte actor helper; one indirect call remains unresolved. |
| `0x8003a778` | WIP | Event controller calls this 0x27c-byte actor target search, which calls vector distance and `rand`. |
| `0x80045e5c` | WIP | Event trajectory helper calls `rsin`, `rcos`, and the collision wrapper; its final byte read uses the unresolved collision-cache pointer. |
| `0x80045f20` | Exact | First event pose interpolation helper; existing contiguous event-pose unit is strict exact. |
| `0x80045fd4` | Exact | Paired event pose helper calls angle interpolation twice; same unit is strict exact. |
| `0x800460a0` | WIP | Existing actor animation phase-seek unit scores 99.268295%; actor owner retains its residual listing. |
| `0x80046144` | Exact | Event message-stream marker search; existing message unit is strict exact. |
| `0x800462bc` | WIP | 0x444-byte event command dispatcher calls the message scanner and actor helpers through an unresolved jump table and indirect callback. |
| `0x80046700` | Exact | Event map-object spawn wrapper; existing map-object event unit is strict exact. |
| `0x8004678c` | WIP | 0xc54-byte event controller directly calls CD service, sound playback, counter helpers, map-object, and actor functions; its command switch and multiple shared states are not yet typed. |
| `0x800474c4` | Exact | Seven-argument collision-channel transition and frame presentation; now consolidated with the two adjacent event counters, with all three strictly exact. |
| `0x800475d8` | WIP | 0x6c0-byte event controller calls CD services, `PadRead`, event-pose interpolation, and the counter incrementer. |
| `0x80047c98` | WIP | 0x660-byte paired controller calls event trajectory, pose transition, notification, and save serializer. |
| `0x80048554` | WIP | 0x458-byte save serializer calls all four typed event-state pointer/offset helpers and the arena allocator; packed record schema and switch table remain open. |
| `0x800489ac` | WIP | 0x378-byte save decoder calls offset expansion, map-object reset, and the height lookup; its switch table and record schema remain open. |
| `0x80048d24` | WIP | 0x5b8-byte event/save walker has no decoded direct calls; its field and data referents need classification. |
| `0x800492dc` | WIP | 0x5e0-byte paired walker likewise has no decoded direct calls; its control/data layout remains unresolved. |

`0x800498bc` and `0x800498c4` are already attributed to Sony `NONE2.OBJ`;
`0x800498d4` onward begins Sony `MALLOC.OBJ`. Their proximity to the event
walkers does not make them game-owned.

## Listener and collision-mask follow-up

This 25-function follow-up follows the listener's height query into the shared
collision cache, then the collision-row and render-mask operations used by the
same map/event path. The six adjacent row/default helpers were combined into
`game.collision_defaults`; their bodies and initialized `collision_default_rows`
table still match retail exactly. The merge removed three source modules.
Scores below are strict objdiff scores after a focused rebuild. Exact results
outside that merged unit belong to the indicated neighboring source owners.

| GAME VA | Verdict | Retail evidence or remaining model |
| --- | --- | --- |
| `0x80014030` | WIP | Listener update calls the map-cell elevation sampler; the cache fields it consumes still lack a proved owner boundary. |
| `0x8002a988` | WIP | Signed x/y/z sampler reads either elevation byte of a 10-byte map cell; its fallback at loaded-data interior `0x800667fc` and cache writes at `0x801d8d40`–`4c` remain unowned. |
| `0x8002aaa4` | WIP | Large collision dispatcher consumes the sampled cell and writes shared query results; its switch/output schema remains open. |
| `0x8002b604` | WIP, 99.333336% | Five-argument height/collision wrapper in the adjacent unit; root owns its remaining codegen residue. |
| `0x8002b67c` | WIP, 94.895836% | Cell-layer selection wrapper in the same unit; root owns its residue. |
| `0x8002b73c` | WIP, 98.404260% | Bounded cell occupancy update in the same unit; root owns its residue. |
| `0x8002b7f8` | Exact | Height-adjusted collision query remains strict 100% in the adjacent wrapper unit. |
| `0x8002b874` | WIP | No-argument result snapshot selects a 16-byte record from static, actor, or object storage and writes `0x801d8d70`–`83`; root has its read-only dossier for reconstruction. |
| `0x8002b9d4` | WIP | Six-argument bitmask dispatcher calls the height wrapper, two actor queries, an object query, and player distance; it writes collision-cache words at `0x801d8d40`, `60`, `64`, and `68`. Root has its read-only dossier. |
| `0x8002bc18` | Exact | Copies the 80 initialized default records into mutable collision rows and clears graphics controls; merged unit's code and DATA match. |
| `0x8002bd3c` | Exact | Rebuilds three rotated matrices per collision row when the shared dirty flag is set; strict 100% after merge. |
| `0x8002bdbc` | Exact | Interpolates selected collision-row motion, rotation, and filter fields; strict 100% after merge. |
| `0x8002be9c` | Exact | Applies interpolation over 62 rows, excluding index 38; strict 100% after merge. |
| `0x8002bf38` | Exact | Constructs a filter payload from three byte kinds, an angle, and an amount; strict 100% after merge. |
| `0x8002bfac` | Exact | Clears all 576 map-cell layer-mask bytes as 144 words; strict 100% after merge. |
| `0x8002bfd4` | WIP, 54.15534% | Typed mask-segment rasterizer has eleven branches and correct mask referents, but loop instruction order differs. |
| `0x8002c170` | WIP, 89.8% | Two-state row scanner has the retail 11 blocks, five branches, and one return; its row-pointer and state register allocation differs. |
| `0x8002c1d4` | Exact | Fills between the first and last selected mask cells in each 24-cell row; strict 100% in the mask-fill unit. |
| `0x8002c290` | WIP, 71.32674% | Map-layer mask update still has a different early-branch topology and an unresolved scan-state owner at `0x801b5a70`. |
| `0x8002c424` | WIP | Related mask/line rasterizer has no source claim; the scan-state owner remains unallocated. |
| `0x8002c670` | WIP | Large map/collision-mask dispatcher calls the height sampler and mask helpers; its graphics-runtime/scan-state model remains open. |
| `0x8002ce2c` | Exact | Finds the first free floor-image item among eight; strict 100% in the floor-item unit. |
| `0x8002ce68` | WIP, 65.85185% | Seven-argument GPU image capture has matching calls and CFG; early stack-argument register allocation differs. |
| `0x8002cf40` | WIP, 92.91011% | Eight-item image rotation has matching calls and referents, with pointer induction and register-allocation residue. |
| `0x80045e5c` | Exact | Event trajectory uses trigonometry and the collision wrapper; actor-owned source uses the provisional complete BSS view for the final shape read. |

The cache begins 18.75 provisional 0x20-byte equipment records after the
equipment-array base. Retail equipment constructors guard the byte ID only
against `0xff`, so they do not prove an upper bound that would separate the
two ranges. This remains a data-ownership question, not a reason to add an
overlapping global or literal address to the source.

The audio follow-up also joined three contiguous source runs. `game.audio_spatial`
now owns 0x80013c8c–0x800140dc and is strict 5/5 exact after reconstructing
the spatial core and listener update; `game.audio_play_sound` owns
0x800140dc–0x80014394 and remains strict 4/4 exact. The VAB callback, service,
slot, and queue functions at 0x80014394–0x800147a0 now share
`game.audio_vab_stream`: the callback, slot acquisition, and queue remain
strict exact, while `cd_request_service_vab` remains WIP at 94.87342% with
matching 11-block control flow and the previously observed constant/state
store residue. These three merges remove five GAME modules in total.

The complete 16-function audio run from `0x80013ae4` to `0x800147a0` now
shares `game.audio_runtime`. All 15 earlier exact functions remain strict
100%, including the five spatial/listener functions; only
`cd_request_service_vab` at `0x800144b8` remains WIP at 94.87342%. This
contiguous merge removes three further GAME modules, and target relink is
136/136 units verified.

## Map-resource audio/CD call family

The map-resource controller at `0x8003247c` uses the same CD archive/VAB and
sound path. Its direct calls and the immediately required TMD/render helpers
form this 27-function bounded follow-up. The controller's 0xb70-byte body
contains 50 decoded outgoing references, including two VAB/TMD range updates,
five calls to the 0x80031850 renderer, four word clears, and one sound call.
An exact row owned by another source is a strict result observed in the shared
report, not a new source claim in this audio campaign.

| GAME VA | Verdict | Direct role or remaining evidence |
| --- | --- | --- |
| `0x800140dc` | Exact | Plays the controller's sound cue through the typed voice path. |
| `0x80014f64` | Exact | Builds the model's YXZ rotation matrix. |
| `0x8001584c` | Exact | Interpolates a scalar used in the renderer's fog transition. |
| `0x800158b4` | WIP, 98% | Nine-halfword interpolation called by the renderer; actor/math owner retains the residue. |
| `0x80017228` | Exact | Clears counted resource words before each map pass. |
| `0x8002d0a4` | Exact | Sets the near fog value used by the renderer. |
| `0x8002d484` | Exact | Selects a TMD object for the render pass. |
| `0x8002d4b8` | Exact | Selects that object's vertex rows. |
| `0x8002d5dc` | WIP | TMD primitive-index preparation is called by the exact read callback, but has no GAME source claim yet. |
| `0x8002ddb4` | WIP | Projection helper called at the second render path; signature and packet schema remain candidates. |
| `0x8002e4dc` | WIP | First render path's geometry helper; its packet/control model remains open. |
| `0x8002f808` | WIP, 63.362473% | Alternate clipped TMD packet path has a typed but non-exact render-map source. |
| `0x80031850` | WIP | 0x53c-byte model render routine drives GTE matrices, fog/color interpolation, TMD selection, and packet output; graphics state is not fully modelled. |
| `0x80031d8c` | WIP | 0x214-byte paired model render routine drives matrix/color setup and TMD projection; graphics state remains open. |
| `0x80031fa0` | Exact | Retrieves a registered resource and checks dynamic block kind. |
| `0x80032008` | Exact | CD TMD-read callback prepares primitive indices and marks the block ready. |
| `0x80032040` | Exact | Gets one map-cell layer mask for a world position. |
| `0x800320b0` | WIP, 73.95918% | Combines masks over a radius; existing map-cell source remains non-exact. |
| `0x80032174` | WIP, 93.6% | Tests map-cell visibility against the view cell; existing source remains non-exact. |
| `0x800321d8` | WIP, 98.4359% | Queues a TMD archive read; the unresolved arena source at `0x8009b0a0` changes the address-form relocation. |
| `0x80032274` | WIP, 90.933334% | Queues missing VAB streams; index induction and saved-register allocation differ. |
| `0x80032364` | Exact | Queues missing TMD blocks and sets their memory-block kind. |
| `0x8003247c` | WIP | Map-resource controller has an unclaimed large body and mixed map, actor, graphics, CD, and sound state. |
| `0x80033afc` | Exact | Selects the registry entry needed for TMD rendering. |
| `0x80034070` | Exact | Updates the cached sparse vertices for the selected asset. |
| `0x80036ad8` | Exact | Tests the camera against a rectangular map region. |
| `0x8003c10c` | Exact | Supplies an actor or group-adjusted position to the resource controller. |

The eight contiguous resource helpers at `0x80031fa0`–`0x8003247c` now share
`game.resource_runtime`. The registry getter, TMD callback, single-cell mask,
and TMD range update remain strict exact. The radius mask, visibility test,
TMD read queue, and VAB range update retain their documented WIP residues.
This merge removes two more GAME modules without changing the four exact
listings; GAME target relinks 154/154 units.

## Spatial playback and listener update

| GAME VA | Verdict | Evidence |
| --- | --- | --- |
| `0x80013c8c` | Exact, 100% | Spatial playback uses listener position/rotation, sampled map layer, distance attenuation, directional pan, and the voice key-on call. Retail passes volume as a full `s32`; a separate listener-angle offset reproduces its load-delay schedule. |
| `0x80013f50` | Exact, 100% | Default-range wrapper preserved. |
| `0x80013f84` | Exact, 100% | Explicit-range wrapper preserved. |
| `0x80013fb8` | Exact, 100% | Voice key-off wrapper preserved. |
| `0x80014030` | Exact, 100% | Listener update copies the camera pose and rotation, samples the map layer, and stores that layer in audio state. |

The strict report matches all five functions and all 1104 text bytes in the
contiguous unit. Nine reviewed BSS relocation pairs cover the listener and
collision-cache references. The global match command still fails on data
edge-check differences in unrelated renderer/map-object units; its generated
objdiff report verifies this unit at 100%.

## Audio initialization, playback, and VAB follow-up

This 23-function call/data family has 19 strict exact listings and four WIPs.
The strict report was checked after the spatial-core and listener changes.

| GAME VA | Verdict | Role or remaining evidence |
| --- | --- | --- |
| `0x800139c4` | WIP, unclaimed | Initializes Psy-Q sound, clears VAB slots and stream slots, and stores fixed workspace pointers. The workspaces at `0x8009a6a0`, `0x80198640`, and `0x80165a68` need owner/extent evidence before source. |
| `0x80013ae4` | Exact | Starts the active sequence. |
| `0x80013b7c` | Exact | Stops the active sequence. |
| `0x80013bd4` | Exact | Shuts down audio. |
| `0x80013c8c` | Exact | Computes spatial attenuation and pan, checks the sampled map layer, and starts a voice. |
| `0x80013f50` | Exact | Plays with the default distance range. |
| `0x80013f84` | Exact | Plays with an explicit distance range. |
| `0x80013fb8` | Exact | Turns off a tracked voice. |
| `0x80014030` | Exact | Updates listener pose and map layer. |
| `0x800140dc` | Exact | Plays a sound cue. |
| `0x80014100` | Exact | Refreshes active voice handles. |
| `0x80014164` | Exact | Allocates a voice handle. |
| `0x80014278` | Exact | Starts an SPU voice. |
| `0x80014394` | Exact | Handles the VAB stream callback. |
| `0x800144b8` | WIP, 94.87342% | Services a VAB request; the CFG and calls agree, but the pinned compiler keeps `-1` instead of the retail shared `1` register, moving two stores. No source-backed reason for the constant allocation is known. |
| `0x800145f4` | Exact | Acquires a VAB stream slot. |
| `0x800146d0` | Exact | Queues a VAB stream read. |
| `0x800321d8` | WIP, 98.4359% | Queues a TMD read; the fixed arena at `0x8009b0a0` has no proven source owner, leaving one address-form difference. |
| `0x80032274` | WIP, 90.933334% | Updates VAB slots over a flagged range; retail recomputes the slot offset each iteration while the current compiler retains an induction offset and extra saved register. |
| `0x80032364` | Exact | Updates TMD slots over a flagged range. |
| `0x80045e18` | Exact | Plays sound 0x40 at volume 100. |
| `0x80045e3c` | Exact | Plays a supplied sound at volume 100. |
| `0x80045e5c` | Exact | Probes collision shape ahead of a position and tests it against 0x20; actor-owned source appended beside the sound wrappers. |

## Resource-transition VAB/CD call family

This 29-function follow-up starts from the proven `audio_queue_vab_stream`
callers at 0x80015d58 and 0x80016820. The latter dispatches five adjacent
phase callbacks, queues archive reads, and traverses map/actor resource state.
The current strict report has 20 exact functions and nine WIPs in the bounded
call family. Exact rows owned by other units are observations, not new claims.

| GAME VA | Verdict | Direct evidence or limitation |
| --- | --- | --- |
| `0x80013ae4` | Exact | Restarts sequence playback from the transition path. |
| `0x800146d0` | Exact | Both loader/controller paths queue VAB streams here. |
| `0x80015d58` | WIP, unclaimed | Opens seven CD archives, reads map data, queues a VAB, and copies ten resource ranges; several fixed arena/data owners remain unresolved. |
| `0x80015fd4` | WIP, unclaimed | Sets transition values, yields CD work until completion, sets a TMD slot, then invokes a callback-table entry; the TMD address at 0x8012da68 lacks an owner. |
| `0x800160e8` | WIP, unclaimed | Translates three live object pools by input x/y/z; their full record extents and state owners need confirmation. |
| `0x80016260` | WIP, unclaimed | Runs a large color/fade transition, calls sequence start, yields CD work, and invokes 0x16820; the eight-argument API and state family remain under review. |
| `0x800167bc` | Exact | Sets transition phase 1. |
| `0x800167d0` | Exact | Sets transition phase 3. |
| `0x800167e4` | Exact | Sets transition phase 2. |
| `0x800167f8` | Exact | Sets transition phase 4. |
| `0x8001680c` | Exact | Sets transition phase 6. |
| `0x80016820` | WIP, unclaimed | 0x6b4-byte controller with a reviewed seven-entry indirect switch and a later unresolved indirect callback, map and actor fixups, CD queues, sound sequence changes, and VAB queuing. |
| `0x80016ee0` | Exact | Queues the map stream read. |
| `0x800171c8` | Exact | Copies resource words to the destinations set by the loader. |
| `0x8001777c` | Exact | Releases a memory block during transition. |
| `0x8001779c` | Exact | Yields CD requests from both transition paths. |
| `0x80017ef4` | Exact | Queues archive reads from 0x16820. |
| `0x800182d0` | Exact | Reads a complete archive entry from 0x15d58. |
| `0x800184d0` | Exact | Opens the seven CD archives. |
| `0x80025184` | Exact | Initializes the game session after resource loading. |
| `0x8002b73c` | WIP, 98.40426% | Collision-height helper in root's active collision unit; current strict report is non-exact. |
| `0x8002d8f0` | Exact | Sets the TMD slot after transition completion. |
| `0x800339fc` | Exact | Loads TMD archive state during initial resource loading. |
| `0x800346b0` | Exact | Releases pooled map records during transition. |
| `0x80034818` | Exact | Expands placed map objects. |
| `0x80035894` | WIP, unclaimed | Large map-placement controller called by 0x16820; belongs with map occupancy ownership. |
| `0x8003f7ec` | WIP, 85.86207% | Actor group target fixup is source-owned by the actor campaign. |
| `0x8003f860` | Exact | Adjacent actor fixup helper. |
| `0x800489ac` | WIP, unclaimed | Large event controller callback used by 0x16820; event-state schema is incomplete. |

No source was added for the large transition functions: fixed storage and
indirect callback targets are not yet fully owned, and the neighboring
collision, actor, and player functions belong to their respective campaigns.

## Consolidated resource-runtime call family

This 26-function bounded batch follows `resource_vab_update_range` and
`resource_tmd_update_range` through their archive/VAB calls and the nearby
asset registry. The current strict report has 18 exact functions and eight
WIPs. The first eight functions form the newly consolidated
`game.resource_runtime` unit; four remain exact, four retain their prior
residues, and two linked modules were removed.

| GAME VA | Verdict | Direct role or residue |
| --- | --- | --- |
| `0x80014394` | Exact | VAB stream callback after archive transfer. |
| `0x800144b8` | WIP, 94.87342% | VAB request service retains a shared-constant/register residue. |
| `0x800145f4` | Exact | Allocates or reclaims a VAB stream slot. |
| `0x800146d0` | Exact | Queues a VAB archive stream. |
| `0x80015d58` | WIP, unclaimed | Startup archive/resource loader calls the VAB queue; fixed copy destinations lack complete owners. |
| `0x80016820` | WIP, unclaimed | Resource transition controller calls the VAB queue and CD/archive helpers; indirect control remains unresolved. |
| `0x800171c8` | Exact | Copies resource words during loader/transition work. |
| `0x8001779c` | Exact | Yields pending CD work. |
| `0x80017864` | Exact | Advances the current CD request. |
| `0x80017d00` | Exact | Checks transferred CD sectors. |
| `0x80017d54` | Exact | Enqueues one CD request. |
| `0x80017e6c` | Exact | Finds an archive entry extent. |
| `0x80017ef4` | Exact | Queues an archive entry read. |
| `0x800180b4` | Exact | Returns archive entry size. |
| `0x80031fa0` | Exact | Gets a registry asset, checking dynamic block kind. |
| `0x80032008` | Exact | Completes a TMD archive read. |
| `0x80032040` | Exact | Gets one map-cell layer mask. |
| `0x800320b0` | WIP, 73.95918% | Radius mask loop has matching CFG and referents, but loop induction and register allocation differ. |
| `0x80032174` | WIP, 93.6% | View-cell visibility has matching CFG and referents, with return-register allocation residue. |
| `0x800321d8` | WIP, 98.4359% | TMD queue uses an unresolved fixed arena source at 0x8009b0a0. |
| `0x80032274` | WIP, 90.933334% | VAB range loop retains an extra induction register/frame. |
| `0x80032364` | Exact | TMD range update. |
| `0x8003247c` | WIP, unclaimed | Large map-resource controller drives both range updates and mixed renderer/map state. |
| `0x800339fc` | Exact | Registers TMD archive entries. |
| `0x80033ab4` | Exact | Stores a registry asset. |
| `0x80033afc` | Exact | Selects a registry asset. |

Focused comparison of `game.resource_runtime` is 4/8 identical listings;
strict objdiff confirms the same four 100% functions and GAME target relink
154/154. The global edge-check still reports unrelated map-object RODATA
addend differences.

## Ten-function spatial-audio caller audit

This bounded audit follows the proven `audio_play_spatial_default_range` and
`audio_play_spatial_range` calls into map objects, actors, effects, and the
event controller. The current strict objdiff report confirms eight exact
source-owned functions. The two unclaimed callers have no strict comparison;
their jump-table ownership and complete state models are still open.

| GAME VA | Verdict | Evidence or remaining issue |
| --- | --- | --- |
| `0x80013c8c` | Exact, 100% | Spatial attenuation and listener-relative panning core. |
| `0x80013f50` | Exact, 100% | Default-range wrapper called by map-object and actor sound paths. |
| `0x80013f84` | Exact, 100% | Explicit-range wrapper called by actor, effect, and event paths. |
| `0x80014030` | Exact, 100% | Copies the listener pose and cached map layer used by the spatial core. |
| `0x800140dc` | Exact, 100% | Shared direct sound entry used by player, resource, effect, and sound wrappers. |
| `0x80035504` | Exact, 100% | Map-object wrapper calls the default-range spatial entry. |
| `0x8003c614` | WIP, unclaimed | 0xa70-byte actor controller calls the default-range entry at 0x8003c7e4. Retail uses an indirect jump through candidate table 0x80011ee8; the table's owner and actor command state are unresolved. |
| `0x8003d0e8` | Exact, 100% | Actor-triggered sound path calls both spatial wrappers. |
| `0x8003fa50` | Exact, 100% | Effect wrapper calls the explicit-range spatial entry. |
| `0x8004678c` | WIP, unclaimed | 0xc54-byte event controller calls the explicit-range entry at 0x80046ef0, two direct-sound wrappers, and both CD service functions. Its candidate switch table at 0x800128d0, later indirect call, and mixed event/actor state need complete ownership. |

The resource-radius loop's source-order probe improved focused listing
similarity but reduced strict objdiff from 73.95918% to 67.85714%; it was
reverted. The restored `game.resource_runtime` report is 4/8 exact with the
radius function at 73.95918%. The GAME target relink verified 135/135 units;
global edge-check remains blocked by two unrelated map-object RODATA addends.

## Ten-function event CD/audio service audit

These ten functions are proven direct callees of the unclaimed event
controller at 0x8004678c, or a frame/CD helper that it calls. The strict
report confirms four existing exact listings and three source-owned WIPs;
the remaining three have no reconstruction unit. The controller itself is
covered in the preceding ten-function audit and is not counted again here.

| GAME VA | Verdict | Evidence or remaining issue |
| --- | --- | --- |
| `0x800144b8` | WIP, 94.87342% | VAB CD service has matching CFG and calls, with a shared-constant/register placement residue. The event controller calls it three times. |
| `0x80016260` | WIP, unclaimed | 0x55c-byte color/fade transition calls sequence start, save serialization, CD yield, and the resource controller; event dispatch calls it twice. The eight-argument state/API model is incomplete. |
| `0x80016820` | WIP, unclaimed | 0x6b4-byte resource transition has an indirect jump at 0x80016884 and indirect call at 0x80016c00; it queues CD reads and VAB streaming. Event dispatch calls it twice. |
| `0x80016f4c` | WIP, 96.61006% | Stream CD service has matching 29-block CFG and referents, but the current source emits a 40-byte frame versus retail's 48-byte frame and shifts one live halfword. Event dispatch calls it three times. |
| `0x8001779c` | Exact, 100% | CD request yield used in both transition paths. |
| `0x800335a0` | WIP, unclaimed | 0x3f4-byte frame driver is called three times by event dispatch and by 0x80036e24; it reaches collision-mask, renderer, notification, and CD-vsync helpers with 31 data/control references. |
| `0x80036e24` | WIP, 98.86364% | Source-owned brightness/frame loop calls both CD service functions and the frame driver; the strict residue is assignment of mode, endpoint, and step to saved registers. |
| `0x80045e18` | Exact, 100% | Fixed sound 0x40 wrapper called by event dispatch. |
| `0x80045e3c` | Exact, 100% | Supplied sound at volume 100 wrapper called by event dispatch. |
| `0x800473e0` | Exact, 100% | Decrements a nonzero event counter byte; event dispatch calls it in several command branches. |

The 0x80036e24 focused rebuild still has five matching CFG blocks and only
mode/phase saved-register differences, with no independently evidenced source
correction. No source claim was added for the three large controllers.

## Ten-function event transition and save audit

This batch follows the event controller's direct map, pose, and counter calls,
then its paired event path and transition save call. Seven source-owned
functions are strict 100%; one source-owned map-object query remains non-exact;
the paired event controller and serializer remain unclaimed.

| GAME VA | Verdict | Evidence or remaining issue |
| --- | --- | --- |
| `0x800314d4` | Exact, 100% | Sets four graphics control/color bytes during the event transition. |
| `0x80036190` | WIP, 89.78417% | Event controller calls the map-object distance/facing query; its owner retains the non-exact source. |
| `0x800368b4` | Exact, 100% | Resolves map-object action/marker state through its switch table; event controller calls it. |
| `0x80038f20` | Exact, 100% | Scans tagged actors and invokes their callback slot 19 before lifecycle advance. |
| `0x80045f20` | Exact, 100% | Projects camera-relative event offsets into world coordinates. |
| `0x80045fd4` | Exact, 100% | Interpolates event position and angles; both large event controllers call it. |
| `0x80046700` | Exact, 100% | Acquires a map object for an event slot. |
| `0x80047434` | Exact, 100% | Increments an event counter through 99 and enqueues a notification; the paired event controller calls it. |
| `0x800475d8` | WIP, unclaimed | 0x6c0-byte paired event controller services VAB/stream CD requests, draws four frame steps, reads pad input, and uses the pose helpers. Its four live O32 arguments, event state, and branch family need full modeling. |
| `0x80048554` | WIP, unclaimed | 0x458-byte transition serializer has a 3152-byte frame, copies packed actor/map/resource records, calls arena allocation and pointer-offset helpers, and dispatches through candidate table 0x80012960. The packed record format and switch ownership remain open. |

The last two retail bodies and callers were inspected without a source claim;
their current identity signatures are candidate-only. Existing exact source
units belong to the map-object, actor, event-pose, graphics, and counter owners.

## Ten-function CD arena and TMD-read audit

This batch follows the proven `resource_tmd_queue_read` call through archive
size lookup and the GAME arena allocator. Eight listings are strict 100%; the
allocator and TMD queue remain source-owned WIPs. The allocator's original
source is retained after focused probes failed to preserve downstream exact
callback relocations in the contiguous CD/memory unit.

| GAME VA | Verdict | Evidence or remaining issue |
| --- | --- | --- |
| `0x80017314` | Exact, 100% | Finds or coalesces a sufficient free block, calling the arena free/coalesce helpers. |
| `0x8001746c` | Exact, 100% | Waits for pending CD-backed arena blocks through `cd_request_wait_idle`. |
| `0x80017504` | Exact, 100% | Compacts the arena after waiting for CD work and freeing reclaimable blocks. |
| `0x800175e8` | Exact, 100% | Initializes the first free block and terminal sentinel. |
| `0x80017608` | WIP, 96.934784% | Allocates a block, compacting and retrying if needed. Retail subtracts the 12-byte header before requested size; the kept probe folds that arithmetic. |
| `0x800176c0` | Exact, 100% | Releases one block through `memory_arena_free`. |
| `0x80017754` | Exact, 100% | Heap allocation wrapper through `memory_malloc_checked`. |
| `0x8001777c` | Exact, 100% | Heap free wrapper. |
| `0x800180b4` | Exact, 100% | Returns archive-entry size from the reviewed `cd_archives` table. |
| `0x800321d8` | WIP, 98.4359% | Gets entry size, allocates in the fixed arena, tags the block, then queues the CD read. The source mechanism for arena address 0x8009b0a0 remains unproved; its literal emits `lui/ori` against retail `lui/addiu`. |

Splitting allocator remainder subtraction into two assignments matched its
operation order and left only one temporary-register difference in focused
comparison. That probe added four bytes and shifted callback addends in
`cd_initialize`, which would regress an existing exact listing. A named
usable-capacity local also remained non-exact. Both probes were reverted;
`game.cd_memory` returned to 55/57 identical focused listings and its prior
strict scores. No fixed-arena source identity was invented to force the TMD
queue's address form.

## Ten-function CD stream and archive-queue match

The selected GAME functions are exactly `0x80016ed4`, `0x80016ee0`,
`0x80016f10`, `0x80016f4c`, `0x8001779c`, `0x80017864`, `0x80017d00`,
`0x80017d54`, `0x80017e6c`, and `0x80017ef4`. Each was inspected through
retail disassembly, callers, callees, strings, and match state. All ten now
have strict objdiff 100%; `cd_request_service_stream` is the new exact.

| GAME VA | Final verdict | Retail role or direct link |
| --- | --- | --- |
| `0x80016ed4` | Exact, 100% | Marks a CD stream request complete; callback pointer from `cd_map_stream_read`. |
| `0x80016ee0` | Exact, 100% | Queues a map stream read into `cd_stream_work_buffer`. |
| `0x80016f10` | Exact, 100% | Limits a kind-0x40 stream chunk to sixteen sectors. |
| `0x80016f4c` | **New exact, 100%** | Services completed image-stream chunks, partial `LoadImage` rows, header validation, seek, and request completion. |
| `0x8001779c` | Exact, 100% | Yields the VAB and image-stream CD services, `DrawSync`, and `VSync`. |
| `0x80017864` | Exact, 100% | Advances the request ring and issues the next CD command. |
| `0x80017d00` | Exact, 100% | Checks sector data before VAB, archive, or completion handling. |
| `0x80017d54` | Exact, 100% | Enqueues a request and its location, destination, sector count, and callback. |
| `0x80017e6c` | Exact, 100% | Resolves an archive entry's CD location and byte extent. |
| `0x80017ef4` | Exact, 100% | Queues a kind-0x10 archive entry read through the extent and enqueue helpers. |

Retail loads the image rectangle height with `lh` and keeps its signed
halfword value through the stream loop. Changing the local to `s16`, spelling
the pixel count as a left shift, and adding completed rows before the saved Y
coordinate reproduced the stream-service instructions. The adjacent arena
allocator's two-step subtraction reproduces retail's arithmetic order and
maintains same-unit callback offsets. The final strict report has
`game.cd_memory` **56/57 exact**; only `memory_arena_allocate_block` remains
WIP at 99.78261%, with a single temporary-register choice. The source-owned
DATA and RODATA remain exact. GAME target relink verified 136/136 units;
global edge-check is still blocked by unrelated TMD and map-object RODATA
addends.

## Ten non-exact audio, CD, and resource functions

This batch selected only functions that were non-exact or unclaimed at the
start: GAME `0x800139c4`, `0x800144b8`, `0x80015d58`, `0x80015fd4`,
`0x800160e8`, `0x80017608`, `0x800320b0`, `0x80032174`, `0x800321d8`, and
`0x80032274`. Retail disassembly, CFG, callers, callees, strings, data
references, adjacent claims, and match state were checked for each.

| GAME VA | Final verdict | Evidence or remaining issue |
| --- | --- | --- |
| `0x800139c4` | WIP, unclaimed | Initializes the Sony sound engine, voice table, and game sound pools. The fixed workspace boundaries at `0x8009a6a0`, `0x80198640`, and `0x80165a68` lack proven source owners. |
| `0x800144b8` | WIP, 94.87342% | Services a VAB CD request with `SsVabTransBodyPartly` and seek/retry handling. Calls, CFG, and relocs align; the remaining difference is temporary constant/register selection. |
| `0x80015d58` | WIP, unclaimed | Opens seven archive slots and copies length-prefixed resources. Several fixed copy destinations and the `0x8009b0a0` arena boundary still lack source-mechanism evidence. |
| `0x80015fd4` | WIP, unclaimed | Saves transition state, yields while `func_80016820` runs, then calls the active callback. The TMD workspace pointer at `0x8012da68` has no proven source owner. |
| `0x800160e8` | **New exact, 100%** | Translates player camera, 128 effect records, 396 map objects, and 200 actors by the three signed call-site offsets. The typed BSS owners and eight address pairs are reviewed. |
| `0x80017608` | WIP, 99.78261% | Arena allocation has matching CFG and referents; retail uses a separate temporary register for the 12-byte header subtraction. |
| `0x800320b0` | WIP, 73.95918% | Aggregates map-cell masks over a radius. CFG and referents align; row/column induction and register scheduling differ. |
| `0x80032174` | WIP, 93.6% | Checks view-cell visibility. CFG and referents align; the return and temporary registers differ. A direct-return source form worsened CFG and was reverted. |
| `0x800321d8` | WIP, 98.4359% | Queues a TMD read. Its fixed arena literal emits `lui/ori`, while retail uses signed-low `lui/addiu`; the arena's original source mechanism is unresolved. |
| `0x80032274` | WIP, 90.933334% | Updates flagged VAB slots. The probe retains an extra induction register and larger frame, while retail recomputes slot offsets per iteration. |

GAME `0x800139c4` passes `0x8009a6a0` to `SsSetTableSize` with two sequences
and one track. Exact OPEN and END audio initializers use the same call shape
with curated 0x158-byte `audio_sequence_table` workspaces, and KF1 defines a
static table of that size. This supports a 0x158-byte minimum for GAME's
workspace, not its original definition, linkage, or complete extent.
The same startup routine stores `0x80198640` as the sequence-buffer pointer,
0x10 bytes beyond `player_state`. KF1 uses a 0x3000-byte sequence buffer; that
size would leave 0x68 bytes before GAME's `effect_state`, but GAME's buffer
extent and defining source remain candidates. The resource controller later
queues archive slot 4, entry `320 + (values_10[4] % 100)`, into this pointer;
slot 4 was opened as VAB.T, and its entry size is unavailable here.
Five unbound relocation pairs preserve the direct constructors for the sound
table (`0x800139e0/e4`), sequence buffer (`0x80013a28/2c`), initial VAB
buffer (`0x80013a90/94`), and final slot-5/slot-6 overrides
(`0x80013ab4/b8`, `0x80013ac4/c8`). The intermediate VAB slot values are
transient; these pairs do not establish object extents or source owners.
The slot-6 override `0x80164a68` is exactly one `0x1000` chunk before the
loop seed `0x80165a68`. The five loop pointers retained for slots 0..4 end
at `0x8016aa68`, before the curated `actor_state` base `0x8016b600`;
slots 5 and 6 are replaced immediately. The slot-5 override `0x80194e30`
equals the end of curated `game_graphics_runtime`. These boundaries suggest
separate stream workspaces, but do not prove their allocations or complete
extents, so the source keeps the unbound addresses.

The startup copy at `0x80015d58` uses signed-low address construction for two
unowned destinations: `lui 0x801e; addiu -29304` resolves to `0x801d8d88`,
four bytes beyond the curated `bss_801c7540` extent, and
`lui 0x8010; addiu -24368` resolves to `0x800fa0d0`, 0x30 bytes beyond the
arena policy end `0x8009b0a0 + 0x5f000`. Neither address has a proved source
definition or complete extent. The latter receives the first embedded TMD
archive from FDAT.T slot 5, entry 0x30, and feeds
`asset_registry_load_tmd_archive` with asset ID 0. A later embedded archive
is copied to `0x800855a0` and loaded with asset ID 0x28. The first destination
is 0x1988 bytes before `display_primitive_memory`, so it is not a member of
that object. Reviewed relocation pairs preserve the signed-low constructors at
`0x80015ea4/a8`, `0x80015f58/5c`, and `0x80015f80/84` without assigning
either workspace a symbol or source definition. The later copy and TMD loader
input pair use the third unbound destination at `0x800855a0`
(`0x80015f94/98`, `0x80015fac/b0`). The earlier `0x80015e80/84` copy targets
the candidate `player_weapon_records` base `0x801c7078`; its row likewise
leaves the symbol blank pending owner work.

The `0x80015fd4` TMD pointer `0x8012da68` is the destination of a later
RTMD.T archive-slot-1 read in `0x80016820`. It lies 0x10 bytes beyond the
complete `display_primitive_memory` end `0x8012da58`; the workspace's size
and source definition remain unproved. Both constructors use `lui 0x8013`
followed by signed `addiu -9624`: `0x800160ac/b0` passes the address to
`tmd_set_slot`, and `0x80016a4c/50` passes it to `cd_archive_queue_read`.
Their reviewed relocation rows leave the symbol unbound.

The new `world_translate.c` uses the complete player, effect, map-object, and
actor state types. Retail's X/Z/Y update order and independent unsigned
countdowns for the three pools are reflected directly in source; a single
reused countdown gave an otherwise equivalent but non-exact register choice.
`kf-retail-validate` passed, the focused listing is identical, and strict
objdiff reports `100%` for `0x800160e8`. GAME target relink verified 140/140
units. Global edge-check remains blocked by three unrelated RODATA addends in
the TMD and map-object units.

## Ten event and resource-transition functions

The fresh GAME set is exactly `0x80016260`, `0x80016820`, `0x80047c98`,
`0x800482f8`, `0x800483d8`, `0x80048498`, `0x80048554`, `0x800489ac`,
`0x80048d24`, and `0x800492dc`. All were non-exact or unclaimed at selection.
Each received a retail disassembly/CFG, caller/callee, strings, data-reference,
adjacent-boundary, and match-state review.

| GAME VA | Final verdict | Evidence or remaining issue |
| --- | --- | --- |
| `0x80016260` | WIP, unclaimed | Resource transition controller calls `audio_start_sequence`, `cd_request_yield`, and `func_80016820`. Its 0x55c-byte body and eight-argument transition API still need a supported shared state model. |
| `0x80016820` | WIP, unclaimed | Queues CD archive and VAB reads, copies resources, translates world positions, and starts sound sequences. Its seven switch targets are reviewed; the later indirect callback target remains unresolved. |
| `0x80047c98` | WIP, unclaimed | Event controller calls notification, collision, event, and resource restore routines, including `func_800475d8` and `func_80048554`. The event-object state and large control flow remain under study. |
| `0x800482f8` | WIP, 90.38636% | Clears the typed event control/arena and initializes its memory blocks and saved offsets. CFG and referents match; the probe anchors a shared address at control +0x34 where retail anchors it at +0x2c. A typed pointer experiment worsened the listing and was reverted. |
| `0x800483d8` | **New exact, 100%** | Restores ten event pointers from saved halfword offsets, with `0xffff` meaning absent. |
| `0x80048498` | **New exact, 100%** | Saves the inverse ten-pointer offset table into the same typed event arena. |
| `0x80048554` | WIP, unclaimed | Builds event/actor resource streams and dispatches by an indirect table at `0x80012960`. The current inventory only has a candidate first table word and lacks the complete switch-table owner. |
| `0x800489ac` | WIP, unclaimed | Called by the resource controller; decodes event script commands and calls the exact offset restore routine. Its indirect table at `0x80012bf8` likewise has only candidate single-word ownership. |
| `0x80048d24` | WIP, unclaimed | Serializes runtime state into a large caller-provided buffer; the many offset-specific fields and payload extent need a complete typed owner. |
| `0x800492dc` | WIP, unclaimed | Restores that runtime state from the paired buffer; its disjoint byte/halfword/word fields and payload schema are still unresolved. |

`0x80016260` has six proven direct callsites with eight scalar arguments. It
masks each register argument with 0xff as needed and loads the four caller-stack
arguments using `lbu` at caller offsets +16/+20/+24/+28. The candidate
`void(u8 × 8)` identity replaces a false four-pointer seed; the exact player
caller retains its existing `s32` declaration until caller and callee source
types can be checked together.

Retail phases in `0x80016820` constrain the resource layout without proving
the loaded buffers' definitions. Phase zero queues archive slot 5, entry
`3 * index`, into `cd_stream_work_buffer`. Phase one selects the initialized
32-pointer default callback table, queues entry `3 * index + 2` into unbound
`0x8019e138`, and copies 0x3e80 words to `bss_801c7540` followed by 0x600
words to the same object's +0x10000 interior. Phase two queues entry
`3 * index + 1` into the stream buffer and selects the loaded callback table
at `0x8019e138`. The first source range proves at least 0xfa04 readable bytes
from `cd_stream_work_buffer` (four-byte header plus 0xfa00 copied bytes).
The second source starts after an embedded length, so its total bound remains
unknown. Both destination ranges fit the curated 0x11844-byte object; the
callback table and stream buffer still lack complete extents and source owners.
Reviewed pairs at `0x8001694c/50` and `0x80016960/64` bind the two copy
destinations to `bss_801c7540` and its +0x10000 interior, respectively.
Four reviewed, blank-symbol relocation pairs at `0x800168d0/d4`,
`0x800168fc/900`, `0x800169ec/f0`, and `0x80016a7c/80` preserve the
`0x801b6064` stream-buffer base across these read, copy, and parse phases;
the existing 4-byte data identity remains only a candidate anchor.
The next curated BSS anchor is `0x801c7068`, leaving a loose `0x11004`-byte
interval from that base. Generated xrefs identify only the base and its +4
interior in the interval; this does not establish that one buffer occupies
all of it or prove the source definition.

`func_800483d8` and `func_80048498` now reference
`event_state.arena.bytes` directly in their offset expressions. This preserves
the complete BSS owner and removes a redundant local base pointer. Focused
comparison is 5/6 identical listings in `game.event_state`, and strict objdiff
confirms both new functions at 100%; the unit's three previous exacts remain
exact. GAME target relink verified 141/141 units. The global edge-check still
reports only the three pre-existing, unrelated RODATA addends.

## VAB queue and CD callback follow-up

This 30-function GAME call family follows the VAB queue from the resource
transition and map-resource callers through its stream callback, CD request
service, and phase setters. The exact rows below retain earlier strict 100%
results and were identical in the current focused listing checks; no broad
match was run for this follow-up.

| GAME VA | Verdict | Retail role |
| --- | --- | --- |
| `0x80013ae4` | Exact | Starts the selected sequence. |
| `0x80013b7c` | Exact | Stops the active sequence. |
| `0x80013bd4` | Exact | Closes sequence and loaded VABs. |
| `0x80013c8c` | Exact | Spatial sound attenuation and panning. |
| `0x80013f50` | Exact | Spatial sound with default range. |
| `0x80013f84` | Exact | Spatial sound with caller range. |
| `0x80013fb8` | Exact | Keys off a tracked voice. |
| `0x80014030` | Exact | Updates listener position and rotation. |
| `0x800140dc` | Exact | Plays a sound at equal left/right volume. |
| `0x80014100` | Exact | Refreshes voice-handle activity. |
| `0x80014164` | Exact | Allocates or reclaims a voice handle. |
| `0x80014278` | Exact | Keys on a sound using the loaded VAB. |
| `0x80014394` | Exact | Completes the queued VAB stream header. |
| `0x800144b8` | WIP | Services the partial VAB transfer; retail retains constant 1 in s3 while the probe retains -1. |
| `0x800145f4` | Exact | Finds or reclaims a VAB stream slot. |
| `0x800146d0` | Exact | Queues a VAB stream read with the callback at 0x14394. |
| `0x800167bc` | Exact | Selects resource phase 1. |
| `0x800167d0` | Exact | Selects resource phase 3. |
| `0x800167e4` | Exact | Selects resource phase 2. |
| `0x800167f8` | Exact | Selects resource phase 4. |
| `0x8001680c` | Exact | Selects resource phase 6. |
| `0x80016ed4` | Exact | Marks the CD stream complete. |
| `0x80016ee0` | Exact | Queues a map stream read. |
| `0x80016f10` | Exact | Caps a stream chunk at sixteen sectors. |
| `0x80016f4c` | Exact | Services the image-stream CD request. |
| `0x80017f9c` | Exact | Queues a kind-0x30 archive stream read. |
| `0x80015d58` | WIP, unclaimed | Opens archives and loads startup resources; copy destinations lack source owners. |
| `0x80016820` | WIP, unclaimed | Dispatches resource phases; loaded callback table and later indirect target remain unresolved. |
| `0x80032274` | WIP | Updates VAB stream states for a flagged range; current source retains an extra saved register and an induction offset. |
| `0x8003247c` | WIP, unclaimed | Builds resource flags and calls 0x32274 twice; the 0xb70-byte map/render controller has no reconstructed unit. |

At `0x80032728`, the large controller passes archive 4, entry 0x20, VAB slot
2, count 0x40, and a stack flag array to `resource_vab_update_range`. Its
second call at `0x80032c30` passes archive 4, entry 0x60, VAB slot 0x42,
count 0x40, and a second stack flag array. The updater's retail loop uses a
48-byte frame, s0-s6, and a fresh `sll v0,s1,3` each iteration; the current
source produces a 56-byte frame, s0-s7, and retains the shifted offset.
Its call, branches, halfword state accesses, and five-argument ABI agree. A
direct-member source probe worsened the listing and was discarded; no source
change was retained. `game.audio_runtime` is 15/16 SAME, the five phase
setters are 5/5 SAME, and the selected CD controls remain SAME in the focused
`game.cd_memory` run.

The queue, callback, request handlers, and phase setters manipulate GAME
audio/CD state. Their direct SDK callees are separate vendored bodies:
`SsVabTransBodyPartly` is an exact Psy-Q 3.0 `LIBSND.LIB` object-section
match; `CdControl`, `CdGetSector`, `CdRead`, and `CdRead2` are exact Psy-Q 3.0
`LIBCD.LIB` section matches. Event primitives have their own `LIBAPI.LIB`
attributions. The large 0x3247c caller is a map/render owner with VAB calls,
not evidence that the adjacent resource and audio functions share its TU.
The pinned `LIBSND.H` declares `SsVabTransBodyPartly` returning `short`, and
retail sign-extends that result before its -1 and -2 tests. The current `s16`
result in the GAME service wrapper is therefore supported; its saved-register
constant choice remains unattributed codegen residue.

The resource controller at 0x16820 selects the initialized no-op callback
table at 0x80063e00 before queuing the loaded table into 0x8019e138. The
event controller at 0x4678c later calls `active_table[2]` indirectly, but
neither that slot's destination nor the loaded table's full extent is proved.
Retail 0x38f20 also loads `active_table[19]` at 0x38fac and calls it through
`jalr` at 0x38fb4. This proves a 0x50-byte table view for the active callback
interface; it does not prove the CD-loaded object's complete extent or the
indirect target.
The configured hash-identical retail directory has the four executables but
no `CD/COM/FDAT.T`, so the archive-entry size cannot be checked from the
current local media. No BSS or indirect-call identity was added from this
candidate chain.

Nine more reviewed 0x16820 pairs target the complete `audio_state` at
`0x80016d5c/60` (+0xc), `0x80016d64/68` (+0x8), `0x80016da8/ac` (+0x4),
`0x80016dc8/cc` (+0x4), `0x80016dd8/dc` (+0x4), `0x80016dec/f0`
(+0x34), `0x80016e00/04` (+0x38), `0x80016e5c/60` (base), and
`0x80016e98/9c` (+0xc). They clear and later set +0xc to literal 0 and 1,
read the sequence ID at +4, clear VAB slot 1's stream pointer at +0x38, and
queue a read to the sequence-buffer pointer stored at the base. Thus +0xc is
a readiness flag, not a sequence-data pointer;
`KfGameAudioState.sequence_ready` now models its word-sized storage. Its
signedness is not directly observable, so `s32` follows the adjacent
`sequence_active` flag's convention. The focused audio unit retains 15/16
identical listings, with only the pre-existing VAB-service WIP.

## GAME VAB/CD controller referents after checkpoint 39dd6fd

The controller at `0x80016820` constructs eight more addresses inside already
complete objects. Retail `lui`/signed-low pairs and the safe GAME delinker
validate `map_object_state.objects` at `0x168a8/0x168ac` (+0x1e00) and
`0x16acc/0x16ad0` (+0x1e06), `actor_state.actors` at
`0x16a84/0x16a88`, and `actor_state.target_groups` at
`0x16b78/0x16b7c` (+0x60e0). The loops use the documented 0x44-byte map
object and 0x7c-byte actor strides, while the target-group copy spans
0x32c0 bytes to the proved actor-state tail. Another four pairs construct
`bss_801c7540+7` at `0x16b30/34`, `player_state.camera_position` at
`0x16b60/64` (+0xd8) and `0x16b68/6c` (+0xe0), and a byte of the same
complete player object at `0x16c0c/10` (+0xa). The byte's meaning remains
unresolved. All eight rows retain their true interior addends; no overlapping
global was introduced. Safe GAME delinking passed, and semantic disassembly
annotates the reviewed owner/addend at each pair.

This connected 30-function control set has 25 earlier strict exact results
and five honest WIPs. Fresh focused probes preserve 15/16 identical
`game.audio_runtime` listings, 5/5 phase setters, and 56/57 in
`game.cd_memory`. `0x17608` still differs only in the register holding the
intermediate `available - 12` value; its source was left unchanged.

| GAME address | Verdict |
| --- | --- |
| `0x80013ae4` | Exact: sequence start. |
| `0x80013b7c` | Exact: sequence stop. |
| `0x80013bd4` | Exact: audio shutdown. |
| `0x80013c8c` | Exact: spatial playback. |
| `0x80013f50` | Exact: spatial playback default range. |
| `0x80013f84` | Exact: spatial playback caller range. |
| `0x80013fb8` | Exact: tracked voice key-off. |
| `0x80014030` | Exact: listener update. |
| `0x800140dc` | Exact: equal-volume playback. |
| `0x80014100` | Exact: voice-handle refresh. |
| `0x80014164` | Exact: voice-handle allocation. |
| `0x80014278` | Exact: voice key-on. |
| `0x80014394` | Exact: VAB stream callback. |
| `0x800144b8` | WIP: shared 1/-1 register choice; calls, CFG, ABI and referents align. |
| `0x800145f4` | Exact: VAB stream-slot acquisition. |
| `0x800146d0` | Exact: VAB queue wrapper. |
| `0x80015d58` | WIP, unclaimed: startup CD copy destinations and arena source are unproved. |
| `0x80015fd4` | WIP, unclaimed: loaded TMD destination at 0x8012da68 has no owner. |
| `0x800167bc` | Exact: resource phase 1. |
| `0x800167d0` | Exact: resource phase 3. |
| `0x800167e4` | Exact: resource phase 2. |
| `0x800167f8` | Exact: resource phase 4. |
| `0x8001680c` | Exact: resource phase 6. |
| `0x80016820` | WIP, unclaimed: seven phases proved; CD-loaded callback extent and indirect call remain open. |
| `0x80016ed4` | Exact: CD stream completion callback. |
| `0x80016ee0` | Exact: map-stream read wrapper. |
| `0x80016f10` | Exact: stream chunk limiter. |
| `0x80016f4c` | Exact: image-stream CD service. |
| `0x80017608` | WIP: allocator has one two-instruction temporary-register residue. |
| `0x80017f9c` | Exact: kind-0x30 archive stream read. |

The pinned Psy-Q 3.0 `LIBSND.H` specifies the signed short return of
`SsVabTransBodyPartly`; its matching `LIBSND.LIB` body and the CD/Event SDK
callees are vendored boundaries. The GAME wrappers and allocator in this set
are game-owned. No literal workspace owner was inferred from a matching byte
sequence or a nearby BSS boundary.

Adjacent GAME `0x80016260` remains an unclaimed WIP controller with a
candidate eight-byte-valued-argument ABI. Its retail body has 58 reviewed
consecutive HI16/LO16 pairs: 56 read or write the complete
`state_8017d118` at offsets 0 through 0x19, one loads
`audio_state.sequence_active` at +8, and one constructs
`event_state.control` +4. All raw low opcodes (`lbu`, `lh`, `sb`, `sh`,
`lw`, or `addiu`) and signed addends were checked before curation. The safe
GAME delinker accepts all 58 and semantic disassembly annotates both
instructions of each pair. This resolves their referents without claiming a
source body, changing the six caller declarations, or inventing storage for
the separate startup and callback workspaces.

The six direct callers are three sites in `0x8002360c`, one in `0x80036ed4`,
and two in `0x8004678c`. They all populate eight scalar slots. The
`0x80036ed4` caller loads the first four with `lbu` and the final three with
`lb`; `0x80016260` itself reads all four stack arguments with `lbu` and
masks the register arguments with `andi 0xff` where used. Those facts prove
byte-valued behavior but do not prove the original C widths of the register
arguments. Nineteen direct `j`/`jal` words inside `0x80016260` were decoded
against their target addresses and promoted to reviewed control flow; a safe
one-function GAME delink accepted 135 relocations with none withheld. The
body and its candidate signature remain WIP.

The parameter flow is more constrained than the candidate signature alone:
`$a0`–`$a3` are controls 1–4, and caller-stack slots +16/+20/+24/+28
are controls 5–8. Control 5 equal to `0xc8` takes the sequence-start path
and returns. On a new transition, retail writes controls 1–5 into
`state_8017d118.values_10[0..4]`, controls 6–8 into the three bytes at
+0x17..+0x19, sets `transition_active` to one and `transition_phase` to
zero, and writes a +0x16 flag according to whether control 2 is `0xff`.
The five current values at +0x04..+0x08 are consulted separately before a
new transition is queued. This proves two five-byte state groups and three
tail controls, while leaving their game-specific meaning and original C
parameter spellings open.

The source-owned VAB service `0x800144b8` and arena allocator `0x80017608`
also had candidate direct-control rows despite their near-complete C bodies.
Raw GAME words verify all 14 and four `j`/`jal` targets respectively. Their
reviewed rows pass separate safe one-function delinks with 18 and four
relocations, neither withholding any function or relocation. The VAB service
still differs in its shared `1`/`-1` register choice, and the allocator in one
two-instruction temporary assignment; no source-level correction is proved.
Focused listings after the relocation review remain 15/16 identical in
`game.audio_runtime` and 56/57 in `game.cd_memory`; the two noted WIPs are the
only listing differences. This quick comparison is not a new strict match.

The allocator `0x80017608` has two proven external callers: resource TMD
queueing at `0x80032218` passes the asset-registry pointer slot as its third
argument, while the event save walker at `0x80048964` passes a saved-block
pointer slot. Both use the returned payload pointer. This supports the
`u8 **owner` contract and leaves the allocator's first two divergent
instructions (`addiu` temporary register before the size subtraction) as
codegen residue rather than an ABI or ownership gap.

The contiguous 29-function CD/memory run from `0x80016ed4` through
`0x80017864` has 28 identical focused listings and the existing
`0x80017608` allocator WIP. Its 36 remaining candidate direct-control rows
were decoded from hash-identical GAME.EXE words: each `j`/`jal` opcode and
encoded target agrees with the curated row. Calls into Psy-Q `LIBAPI.LIB`,
`LIBGPU.LIB`, and `LIBCD.LIB` are vendor boundaries; the functions in this run
are game-owned. This review confirms control flow, not historical linker
relocation records or a source-level fix for the allocator residue.
Safe GAME delinking of the ten affected functions produced ten objects with
44 relocations and zero withheld relocations or functions. Direct objdiff
against the refreshed focused objects reports 100% for each of those ten;
the separate `0x800144b8` and `0x80017608` WIPs remain 94.87342% and
99.78261% respectively.

The band retains these individual verdicts after the focused recheck. “Exact”
means a prior strict objdiff result with an identical current listing; the ten
functions affected by this relocation review were also directly rechecked at
100% against their freshly carved targets.

| GAME VA | Verdict |
| --- | --- |
| `0x80016ed4` | Exact |
| `0x80016ee0` | Exact |
| `0x80016f10` | Exact |
| `0x80016f4c` | Exact |
| `0x800171c8` | Exact |
| `0x800171f8` | Exact |
| `0x80017228` | Exact |
| `0x8001724c` | Exact |
| `0x80017270` | Exact |
| `0x800172f4` | Exact |
| `0x80017314` | Exact |
| `0x8001746c` | Exact |
| `0x80017504` | Exact |
| `0x800175e8` | Exact |
| `0x80017608` | WIP: temporary-register residue |
| `0x800176c0` | Exact |
| `0x800176e0` | Exact |
| `0x800176e8` | Exact |
| `0x800176f4` | Exact |
| `0x800176fc` | Exact |
| `0x80017708` | Exact |
| `0x80017710` | Exact |
| `0x8001771c` | Exact |
| `0x80017754` | Exact |
| `0x8001777c` | Exact |
| `0x8001779c` | Exact |
| `0x800177d4` | Exact |
| `0x80017804` | Exact |
| `0x80017864` | Exact |

The callback-state byte at `state_8017d118+0x16` now has its own neutral
`flag_16` field. The reviewed direct access set contains two writes in
`0x80016260` (zero when the second control is `0xff`, one otherwise), one
clear in `0x80016820`, and one map-cell-renderer read at `0x80030d14`;
startup clears the enclosing 0x1c-byte state. The split preserves layout and
does not assign the flag a broader meaning. Focused builds of its two sourced
consumers preserved their prior verdicts: `game.main` has `main` SAME and the
known arena-address WIP in `game_main_loop`; `game.render_map_cell` has three
SAME listings and the existing two-instruction entry-order WIP at `0x80030c18`.
The five adjacent phase setters in `game.resource_transition_phase` also remain
SAME after the shared-header change.

### Audio state dispatcher and direct-caller cohort

This GAME-only cohort follows the contiguous audio/VAB runtime through its
callback, then the shared transition state and all six proven calls to
`0x80016260`. The six calls occur at `0x80023698`, `0x80023714`,
`0x800237e8`, `0x800380f4`, `0x80046b04`, and `0x80046ca8` in three caller
functions. Each supplies four register arguments and four caller-stack words.
After its 64-byte prologue, the callee reads the latter with `lbu` at
`sp+80/+84/+88/+92`. The map-script caller loads controls 1–5 with `lbu`
and controls 6–8 with `lb`; the event caller supplies `0xff`/`0x7f`
sentinels and masks its dynamic fifth control to a byte. This supports
eight byte-valued controls, but not one proven source-level signedness for
every caller argument. The exact player caller therefore retains its current
local declaration until a source-owned callee can be compared with all sites.
The callee's decoded direct call set is `EnterCriticalSection`,
`ExitCriticalSection`, `audio_start_sequence`, `cd_request_yield`,
`func_80016820`, and event-save helper `func_80048554`. Its callers ignore
the return register, supporting the current void candidate without proving
the original declaration.

The callee's fifth control `0xc8` selects `audio_start_sequence` if a
sequence is not already active. Other controls compare against the five
current and five pending bytes in the complete `state_8017d118` owner;
new transitions set `transition_active`, reset `transition_phase`, and write
the three signed tail controls. Its +0x16 flag is now a separately typed
byte, with the four direct accesses listed above. The large callee body
remains unclaimed because its original C control structure is not proved;
neighboring startup and resource-transition workspaces also remain open.
`0x80016820` still has a seven-way
indirect phase jump at `0x80016884` and a later `jalr` at `0x80016c00` through
`state_8017d118.active_table[5]`. The scene controller uses slot two and
another effect consumer uses slot 19, proving at least 20 pointer slots but
not the table's full extent or any loaded slot target. Neither indirect target
is inferred from a neighboring address. Pinned Psy-Q audio/CD calls are vendor
boundaries; none of these GAME-owned wrapper bodies is banked as a library
routine.

The per-function verdicts below carry earlier strict exact results for the
exact controls. Current focused checks reconfirmed 15/16 audio-runtime
listings and all five phase setters; no broad match or new source claim was
made in this read-only caller pass.

| GAME VA | Verdict | Evidence boundary |
| --- | --- | --- |
| `0x80013ae4` | Exact | Sequence start; Psy-Q calls. |
| `0x80013b7c` | Exact | Sequence stop. |
| `0x80013bd4` | Exact | Audio shutdown. |
| `0x80013c8c` | Exact | Spatial attenuation and key-on. |
| `0x80013f50` | Exact | Default-range wrapper. |
| `0x80013f84` | Exact | Explicit-range wrapper. |
| `0x80013fb8` | Exact | Tracked voice key-off. |
| `0x80014030` | Exact | Listener pose and sampled layer. |
| `0x800140dc` | Exact | Equal-volume sound playback. |
| `0x80014100` | Exact | Voice-handle refresh. |
| `0x80014164` | Exact | Voice-handle allocation. |
| `0x80014278` | Exact | SPU voice key-on. |
| `0x80014394` | Exact | VAB stream callback. |
| `0x800144b8` | WIP, 94.87342% strict | Retail holds phase value 1 in `s3`; source compiler holds retry sentinel -1 there. |
| `0x800145f4` | Exact | VAB stream-slot acquisition. |
| `0x800146d0` | Exact | VAB archive queue. |
| `0x80015d50` | Exact | Initialized callback table's default no-op. |
| `0x80015d58` | WIP, unclaimed | Startup archive copies; destination owner/extent open. |
| `0x80015fd4` | WIP, unclaimed | Transition setup; TMD workspace owner open. |
| `0x800160e8` | Exact | Three-coordinate translation over four typed object pools. |
| `0x80016260` | WIP, unclaimed | Eight byte-valued controls; original C and workspace mechanism open. |
| `0x800167bc` | Exact | Selects phase 1. |
| `0x800167d0` | Exact | Selects phase 3. |
| `0x800167e4` | Exact | Selects phase 2. |
| `0x800167f8` | Exact | Selects phase 4. |
| `0x8001680c` | Exact | Selects phase 6. |
| `0x80016820` | WIP, unclaimed | CD/VAB transition; indirect jump and callback remain unresolved. |
| `0x8002360c` | Exact | Three direct calls to the eight-control dispatcher. |
| `0x80036ed4` | WIP, unclaimed | Script-record byte loads and one direct dispatcher call. |
| `0x8004678c` | WIP, unclaimed | Two direct dispatcher calls; command switch and callback unresolved. |

### Five GAME startup and transition claims

The previously unclaimed functions `0x800139c4`, `0x80015d58`, `0x80015fd4`,
`0x80016260`, and `0x80016820` now have C `ADDRESS` claims. Each passed the
required image-specific address, disassembly/CFG, xref, call, string, and
match-state inspection before source. None is a vendored Psy-Q body; the
startup function calls Psy-Q sound APIs but owns GAME audio state initialization.
The seven-phase controller owns the reviewed 0x1c-byte switch-table range at
`0x80011058` and the initialized 32-entry no-op callback table at `0x80063e00`.

| GAME VA | Focused verdict | First material residue |
| --- | --- | --- |
| `0x800139c4` | WIP, 64.0% listing similarity | The SDK sequence-table base is now a typed 0x158-byte BSS claim with matching HI16/LO16 relocations. Sequence and VAB stream-buffer workspaces remain unbound and compile as literal `lui/ori`; their later register effects remain WIP. |
| `0x80015d58` | WIP, 87.1% listing similarity | Fixed archive arena and three copy destinations lack proven defining objects. Source now reloads each length-prefixed span after copying, matching the retail copy schedule. |
| `0x80015fd4` | WIP, 96.1% listing similarity | The TMD slot pointer at `0x8012da68` has no proved owner; retail uses a relocation and signed-low address construction. State writes, CD/controller loop, and indirect callback align. |
| `0x80016260` | WIP, 71.6% listing similarity | Five unsigned byte controls and three signed byte offsets, early returns, critical-section wait, state updates, and direct calls are modeled. The shared state-update path precedes the active wait path with a backward jump, and the conflict wait precedes the critical-section wait, matching retail's block order. Sentinel-value registers and some later branches remain WIP. |
| `0x80016820` | WIP, 98.4% listing similarity | Seven-phase switch, CD loads, actor/map cleanup, callback-table swap, sequence fade, and VAB queue are modeled. The switch has one range check, phase-specific buffer lifetimes and in-place cursor advances match retail, and the fade timer is a signed halfword. The remaining differences are the unbound fixed-address workspaces at `0x8019e138` and `0x8012da68`. |

All new units compile under focused `kf try` with the pinned probe. The 15
previously exact audio neighbors stayed `SAME` in the focused audio unit build;
`cd_request_service_vab` remains its established WIP. No strict 100% result
was claimed, and no bank or README write was made.

The current `0x80016820` object also has the retail's seven switch-table
offsets `0x6c`, `0xdc`, `0x1bc`, `0x20c`, `0x408`, `0x544`, and `0x660`, with
seven ordered `R_MIPS_32 .text` relocations. Its 32-entry initialized default
callback table has identical bytes and ordered function-pointer relocations.
The startup unit's seven archive literals match the retail `.rodata` prefix
byte for byte through `0x53`; retail's claimed `0x58`-byte range ends with five
zero bytes beyond the compiled literal extent. Their source or section-padding
mechanism remains unresolved.

GAME's `SsSetTableSize(audio_sequence_table, 2, 1)` now uses a candidate
`DATA(0x8009a6a0, 0x158)` owner. Psy-Q 3.0 defines `SS_SEQ_TABSIZ` as 172,
so the SDK-required 2-by-1 workspace is 344 bytes; OPEN and END use the same
0x158-byte identity. The focused source object emits a 344-byte COMMON symbol;
the refreshed delinker carves a 0x158-byte BSS section from the candidate
identity, so that target section size is not independent proof of retail
allocation extent. The ordered HI16/LO16 pair at GAME `0x800139e0/e4` now
matches. No other curated GAME identity overlaps `0x8009a6a0..0x8009a7f8`;
whether the original allocation reserved more than the SDK-required extent
remains open. The 15 exact audio siblings still have identical focused listings.

### Related resource-runtime audit

GAME `0x80032274` updates a range of VAB slots from a byte flag stream. Its
two proven callers in `0x8003247c` pass five O32 arguments; the sole proven
callee is `audio_queue_vab_stream`. The current C uses the validated
`audio_state+0x30` slot array, an `s16` stream state, and the retail's three
state paths: queue an absent flagged slot, mark a flagged state 2 as 1, or
mark an unflagged state 1 as 2. The focused listing is WIP at 43.6% similarity.
The first divergence is a 56-byte compiled frame with an extra saved index
register versus retail's 48-byte frame and per-iteration slot-index shift;
later branch layout differs. Moving the slot load into both flag branches,
as in the exact TMD range sibling, worsened the listing and was reverted.
No referent, call, width, or control-flow evidence supports a replacement
source claim yet. The four exact neighbors in `game.resource_runtime` remained
`SAME` throughout focused probes.

The adjacent `0x800320b0` map-radius and `0x80032174` map-visibility helpers
have validated references to `game_graphics_runtime`. Their direct callers,
constants, and signed bounds match the current source. Declaration-order and
early-return probes changed register selection or branch layout without
establishing an original source fact, so both were reverted. Their focused
listings remain WIP at 26.9% and 37.9% similarity, respectively.

Four proven `0x8003247c` calls consume `map_cell_layer_mask`'s return register
directly, without a caller-side byte mask. The exact callee itself returns zero
or a zero-extended byte load, so its value range did not establish the source
ABI width. Both map-mask declarations now return `u32`; the exact callee and
three other exact resource-runtime neighbors remain `SAME` after that shared
type correction. The radius helper's callers also consume the full register;
its source still bounds the OR result to a byte.

The `0x80016260` identity now distinguishes five unsigned byte controls from
three signed byte offsets. Its map-object caller casts the offsets to `s8`,
and retail sign-extends the first offset before comparing it with 127. The
source already used those types; the focused listing remains WIP at 71.6%.
The exact `0x8002360c` player caller has three direct calls at `0x80023698`,
`0x80023714`, and `0x800237e8`. Changing its stale all-`s32` declaration to
the shared five-`u8`/three-`s8` resource API leaves all ten functions in
`game.player_core_run` `SAME`; the transition callee also retains its 71.6%
focused listing. This establishes a consistent source declaration without
claiming a different call-site schedule.

The partial VAB transfer service at `0x800144b8` retains 11/11 CFG blocks,
5/5 branches, the same known successor order, and 4/4 return-frontier edges
in a flow-aware focused comparison. Its first control difference is retail's
`bne v1,v0` against the probe's `bne v1,s3`: retail keeps state value 1 in
`s3`, while the probe keeps retry sentinel -1 there. Spelling the retry as a
direct `while ((result = SsVabTransBodyPartly(...)) == -1)` loop emitted an
identical object and was reverted. The function remains WIP at 91.8% focused
listing similarity, with no source-backed constant or ABI correction pending.

### Ten-function resource and audio follow-up

Each selected GAME function was checked against its retail CFG, caller and
callee references, strings, source claim, and focused pinned listing. The
percentages below are focused listing similarities unless labelled strict;
none is an exact closure without `100%`.

| GAME VA | Verdict | First unresolved evidence |
| --- | --- | --- |
| `0x800139c4` | WIP, 64.0% | Four unowned sequence/VAB workspace addresses emit literal `lui/ori` rather than the retail symbol relocations; 15 audio neighbors remain `SAME`. |
| `0x80015fd4` | WIP, 96.1% | The TMD destination `0x8012da68` lacks a complete object owner; its literal address pair differs while the transition loop and callback agree. |
| `0x80016260` | WIP, 71.6% | The five control and three signed-offset values agree; saved stack-byte registers and sentinel branch layout differ. The exact player caller retains all ten `SAME` listings with the shared typed declaration. |
| `0x80016820` | WIP, 98.4% | The seven switch offsets and callback-table entries agree; only the unowned `0x8019e138` and `0x8012da68` workspace constructors and dependent scheduling differ. |
| `0x80032174` | WIP, 37.9%; 93.6% recorded strict | Both view-cell referents and five CFG blocks agree; the probe uses a different comparison/result register and adds a final move. |
| `0x800321d8` | WIP, 95.8%; 98.4359% recorded strict | The arena boundary `0x8009b0a0` emits literal `lui/ori` instead of retail carry-adjusted `lui/addiu`; calls and three CFG blocks agree. |
| `0x80032274` | WIP, 43.6%; 90.933334% recorded strict | The probe retains an extra saved register and increments the slot address; retail recomputes the eight-byte slot offset per iteration. The audio-state referent and call agree. |
| `0x80032364` | **Exact, 100% strict** | The TMD range sibling remains `SAME` with its resource calls and registry referent. |
| `0x8003ae50` | WIP, 99.2%; 99.31746% recorded strict | The 58-block collision response has all direct calls and six exact siblings; obstacle-angle mask timing and temporary register differ. |
| `0x8003c3e0` | WIP, 95.4%; 99.64539% recorded strict | The 23-block group-position solver keeps its calls and three exact siblings; yaw-error and shifted-numerator registers are exchanged. |

In the adjacent radius-mask helper `0x800320b0`, retail forms the Z row offset
and row pointer before computing the X start cell. Moving the source's X start
calculation below the row pointer reflects that order and improves
the focused listing from 26.9% to 40.4%. All four exact siblings in
`game.resource_runtime` remain `SAME`; `map_cell_visible`, the TMD queue, and
the VAB range updater retain their prior WIP listings. The remaining radius
difference begins with accumulator/span register assignment and address-load
scheduling, so no source-only register carrier was added. The previous
73.95918% strict radius score is a stale checkpoint, not a new closure claim.
For `map_cell_visible` at `0x80032174`, direct early returns preserve the
boolean result but add a jump absent from retail and lower the focused listing
from 37.9% to 26.7%; that probe was reverted. Its shared-tail source and
93.6% stale strict verdict remain WIP.
