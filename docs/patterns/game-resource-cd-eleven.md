# GAME resource startup and CD control: eleven-function verdict

This focused set follows the startup archive loader into transition phase
controls and the CD functions it calls. The two startup functions were
reviewed against retail disassembly, CFG, calls, strings, data references,
relocation pairs, their main-loop callers, neighboring phase functions,
and the KF1 resource loader. The three CD functions and allocator were
checked in a fresh one-unit strict report with the other 53 exact CD
siblings as controls.

| GAME address | Role | Direct strict verdict |
| --- | --- | --- |
| `0x80015d58` | Open seven archives and unpack startup resources | WIP, 89.03145% |
| `0x80015fd4` | Pump resource transition and install TMD slot zero | WIP, 89.710144% |
| `0x800167bc` | Set transition phase 1 | exact, 100% |
| `0x800167d0` | Set transition phase 3 | exact, 100% |
| `0x800167e4` | Set transition phase 2 | exact, 100% |
| `0x800167f8` | Set transition phase 4 | exact, 100% |
| `0x8001680c` | Set transition phase 6 | exact, 100% |
| `0x80017608` | Allocate and split a CD/resource arena block | WIP, 99.78261% |
| `0x8001779c` | Yield one CD request step | exact, 100% |
| `0x800182d0` | Read an archive entry | exact, 100% |
| `0x800184d0` | Open an archive | exact, 100% |

The targeted startup and phase-unit rebuild/direct objdiff reports five
phase setters **5/5 exact** and the startup's **83/83 `.rodata` bytes exact**.
The CD-unit direct report is **56/57 exact** with its 11 initialized `.data`
and 33 `.rodata` bytes exact. Thus the selected eleven have **8 exact** and
three WIPs; the CD unit's separate 772-byte BSS/COMMON placement mismatch
is not evidence about these function bodies.

Retail `0x80015d58` is one straight-line block with 22 proven direct calls
and the seven `COM\\*.T` strings. The source matches the archive order,
length-prefixed word copies, and final session initialization. Its first
divergence is the fixed read-arena pointer `0x8009b0a0`: retail uses a
carry-adjusted `lui/addiu`, while the provisional C literal emits `lui/ori`.
Later copies and TMD loads construct `0x801d8d88`, `0x800fa0d0`, and
`0x800855a0` with reviewed signed-low pairs; those destinations have no
proved complete objects or defining translation units. The existing raw
literal pointers remain WIP rather than fabricated storage or linker
equates.

Retail `0x80015fd4` has three blocks: it saves five transition bytes,
pumps `cd_request_yield` and `0x80016820` until the active halfword clears,
sets TMD slot zero, then calls the active callback table's sixth entry.
The callback target stays indirect. Its first source/object divergence is
the TMD destination `0x8012da68`, formed by a reviewed `lui/addiu` pair.
That address is 0x10 bytes beyond the complete
`display_primitive_memory` extent and is also the destination of a later
archive read. These uses prove the pointer but do not prove the workspace's
allocation, size, or source mechanism. KF1's resource loader uses named
chunk/cursor buffers and does not settle KF2's fixed destination owner.

The allocator at `0x80017608` retains its separately documented two-word
temporary-register residue: retail uses `v0` for `available - 12` before
subtracting into `a0`; the compiler reuses `a0`. Its CFG, calls, block
split threshold, and owner write agree. No source/config edit was retained
for this batch. Verification used only targeted unit builds, direct
per-unit objdiff, and focused `kf try`; no repository tests, lint, full
linked build, broad match, or banking ran.

## Connected 21-function focused recheck (2026-10-01)

Fresh `kf try --context 0 --no-flow` rebuilds cover the startup loader,
transition controller, resource runtime, asset registry, and their arena
allocator. The **12 `SAME` listings** have previously recorded direct strict
100% verdicts; `SAME` alone is only focused listing evidence. The other nine
remain WIP. Retail calls, byte widths, switch targets, and referenced data
were checked against the existing raw dossiers before considering source
changes.

| GAME address | Focused verdict | First remaining difference or control |
| --- | --- | --- |
| `0x80015d58` | DIFF, 87.1% | Fixed read arena and three unowned copy destinations use literal `lui/ori` instead of retail's relocatable signed-low address form. |
| `0x80015fd4` | DIFF, 96.1% | Unowned TMD workspace `0x8012da68` address form and dependent setup. |
| `0x80016260` | DIFF, 71.6% | Stack-byte register assignments and sentinel branch layout; five unsigned controls and three signed offsets remain typed. |
| `0x800167bc` | SAME | Phase-1 callback control. |
| `0x800167d0` | SAME | Phase-3 callback control. |
| `0x800167e4` | SAME | Phase-2 callback control. |
| `0x800167f8` | SAME | Phase-4 callback control. |
| `0x8001680c` | SAME | Phase-6 callback control. |
| `0x80016820` | DIFF, 98.4% | Seven-phase dispatch is retained; `0x8019e138` callback destination and `0x8012da68` TMD workspace have no proved defining owners. |
| `0x80017608` | DIFF, 96.0% | Two-word allocator residue: retail uses `v0` for the available-size subtraction before writing `a0`; the probe reuses `a0`. |
| `0x80031fa0` | SAME | Registry getter. |
| `0x80032008` | SAME | TMD read-completion callback. |
| `0x80032040` | SAME | Single-cell layer-mask query. |
| `0x800320b0` | DIFF, 26.9% | Raw Z then X coordinate setup is retained; row/column induction and register assignments differ. |
| `0x80032174` | DIFF, 37.9% | View-cell comparisons and referents agree; result register and final move differ. |
| `0x800321d8` | DIFF, 95.8% | Arena boundary `0x8009b0a0` emits `lui/ori` from the provisional literal; retail uses a relocatable `lui/addiu`. |
| `0x80032274` | DIFF, 43.6% | Retail recomputes the VAB slot offset in a 48-byte frame; the probe retains an induction offset and a 56-byte frame. |
| `0x80032364` | SAME | TMD range updater. |
| `0x800339fc` | SAME | TMD archive registry loader. |
| `0x80033ab4` | SAME | Registry setter. |
| `0x80033afc` | SAME | Registry selector. |

The broader `game.cd_memory` rebuild retained **56/57 `SAME` listings**,
including `resource_copy_words`, `cd_archive_queue_read`, and
`cd_archive_read`; only the counted allocator above differs. The five phase
callbacks and three asset-registry functions each retained all matching
listings. No new source owner can yet be justified for the fixed RAM
destinations, so no source/config edit was retained in this recheck. No
repository tests, lint, broad match, or full linked build ran.

## Resource-runtime eight-function recheck

A fresh single-unit compile and isolated direct objdiff retained four exact
functions: registry getter `0x80031fa0`, TMD read completion `0x80032008`,
single-cell mask `0x80032040`, and TMD range update `0x80032364`. The four
WIPs remain `0x800320b0` **73.95918%**, `0x80032174` **93.6%**,
`0x800321d8` **98.4359%**, and `0x80032274` **90.933334%** strict. Focused
listings are respectively 26.9%, 37.9%, 95.8%, and 43.6%.

Retail `0x800320b0` computes Z then X before its 24-wide row offset, tests
signed row/column bounds, and returns the OR mask narrowed to a byte. The
current source preserves those values and loop exits; its different induction
registers remain unattributed. Retail `0x80032174` uses the same signed view
cell comparisons and five-block return path as the source, with a result
register difference. At `0x80032274`, retail recomputes the eight-byte VAB
slot offset each iteration and uses a 48-byte frame; the probe retains an
offset induction register and a 56-byte frame while preserving the slot state
transitions and sole audio call. Recasting the VAB `while` as a `for` loop
with the same entry/slot increments compiled byte-identically, so it was
discarded. The arena address at `0x8009b0a0` still lacks an original source
owner, and no literal or storage trick was added for `0x800321d8`.

## Transition request and step focused control

Fresh isolated objects give GAME `0x80016260` **98.790085% strict**
(71.6% focused listing) and `0x80016820` **99.193474% strict**
(98.4% focused). The request keeps the eight byte-valued arguments,
six direct callers, sentinel choices, critical-section wait, and CD-yield
path; its first difference assigns the seventh and eighth stack-byte values
to different saved registers, followed by branch-layout residue. The step's
seven-phase dispatch, direct calls, and callback table remain in place.
Its focused differences are the address constructors for RAM workspaces
`0x8019e138` and `0x8012da68`: retail uses reviewed signed-low relocation
pairs, while the provisional C uses literal `lui/ori` addresses. Neither
workspace has a proved complete source object, so no definition or source
change was added. The five adjacent phase callbacks remain exact controls.

## Resource and render strict recheck (19 functions)

Fresh isolated objects and focused unit builds leave **9 exact and 10 WIP**
functions across seven GAME units. The five transition-phase callbacks and
four `resource_runtime` helpers remain strict exact. The remaining direct
strict scores are:

| Unit | WIP functions and strict `.text` scores | Relocation control |
| --- | --- | --- |
| `resource_startup` | `0x80015d58` 89.03145%; `0x80015fd4` 89.710144% | 115 candidate/127 retail; six retail-only HI16/LO16 workspace pairs |
| `resource_transition_request` | `0x80016260` 98.790085% | 135/135 in ordered type and symbol identity; 67/67 CFG blocks |
| `resource_transition_step` | `0x80016820` 99.193474% | 229/235; three retail-only HI16/LO16 workspace pairs; 62/62 CFG blocks |
| `resource_runtime` | `0x800320b0` 73.95918%; `0x80032174` 93.6%; `0x800321d8` 98.4359%; `0x80032274` 90.933334% | 41/41 in ordered type and symbol identity |
| `render_animated_object` | `0x80031d8c` 94.65414% | 22/22 in ordered type and symbol identity; 7/7 CFG blocks |
| `render_resource_dispatch` | `0x8003247c` 92.18579% | 84 candidate/82 retail; three direct `player_state` loads replace one retained camera-position base; 96/96 CFG blocks |

The startup and transition-step missing pairs all target fixed RAM workspaces
whose complete source allocations and TU owners remain unproved. The map-cell
radius helper retains the supported bounds and mask reduction despite different
row/column induction; the animated renderer retains its call and referent set
but reloads a late argument where retail saves it. The resource dispatcher
retains its reviewed actor, map-object, effect, and placed-record traversal;
its camera pointer is still folded into direct loads by the pinned probe.
These are not grounds to invent storage or force register lifetimes. No C or
inventory edit was retained from this recheck; no tests, lint, broad match,
or full build ran.

## VAB range updater exact closure

GAME `0x80032274` is now **100% strict** over its 240-byte body. Retail forms
the eight-byte VAB slot offset inside each loop iteration and uses a 48-byte
frame. Taking a typed `KfAudioVabSlot *` for the indexed slot before loading
its stream pointer expresses that object access directly; the pinned compiler
then emits the retail loop instead of carrying an offset induction register.
The source still makes the same three state transitions and sole audio queue
call. The five ordered relocations have identical function-relative offsets,
types, and targets. A focused `resource_runtime` build and fresh isolated
strict comparison retain all four previous exact siblings, so the unit is now
**5/8 exact**. The other WIP strict scores are unchanged: `0x800320b0`
73.95918%, `0x80032174` 93.6%, and `0x800321d8` 98.4359%.

## Fresh 29-function resource/workspace control

A pinned-profile isolated compile of eight connected GAME units covers 29
selected functions: main (2), startup (2), transition request (1), phase
setters (5), transition step (1), seven CD/allocator controls, resource
runtime (8), and asset registry (3). Twenty are strict exact. They are
`main`; all five `resource_transition_set_phase_*` callbacks;
`cd_request_service_stream`, `cd_request_yield`, `cd_request_advance`,
`cd_archive_entry_extent`, `cd_archive_queue_read`, `cd_file_load_into`;
`resource_registry_get`, `resource_tmd_read_complete`,
`map_cell_layer_mask`, `resource_vab_update_range`,
`resource_tmd_update_range`; and `asset_registry_load_tmd_archive`,
`asset_registry_set`, `asset_registry_select`.

| GAME function | Strict text | Final bounded verdict |
| --- | ---: | --- |
| `game_main_loop` | 99.67553% | Its arena-base constructor still uses the fixed literal `0x8009b0a0` where retail uses signed-low address construction. The exact allocator establishes the `0x5f000` runtime span, not its source definition. |
| `80015d58` startup loader | 89.03145% | Six target-only HI16/LO16 pairs name four unowned workspaces: `DAT_801d8d88` once, `DAT_8012da68` once, and `DAT_800fa0d0`/`DAT_800855a0` twice each. The archive/copy calls and strings retain their retail order. |
| `80015fd4` startup pump | 89.710144% | Its TMD slot-zero pointer uses the unowned `0x8012da68` workspace. |
| `80016260` transition request | 98.790085% | All 135 target/candidate relocations have the same type and symbol order, and CFG is 67/67 blocks. The first raw difference assigns the seventh/eighth stack-byte values to different saved registers; later independent state-byte stores and a branch delay slot schedule differ. |
| `80016820` transition step | 99.193474% | The seven-phase dispatch retains 62/62 CFG blocks. Retail has two HI16/LO16 pairs for `DAT_8019e138` at `+0x104`/`+0x1f4` and one for `DAT_8012da68` at `+0x22c`; fixed C literals supply none. Its 128-byte DATA callback table and 28-byte RODATA switch table are both strict exact. |
| `memory_arena_allocate_block` | 99.78261% | A two-word temporary-register choice differs; the block split threshold, owner write, and calls agree. |
| `map_cell_layer_mask_radius` | 74.061226% | The raw-backed scalar-width radius correction is retained; remaining row/column induction and coordinate scheduling differ. |
| `map_cell_visible` | 93.60000% | Same view-cell comparisons and five-block return path; result-register choice remains. |
| `resource_tmd_queue_read` | 98.43590% | Its only material address-form gap is the unowned arena base `0x8009b0a0`. |

All other selected units have equal relocation counts; request and runtime
also retain ordered type/symbol identity. The target request unit owns
`state_8017d118` as 28 bytes of `.bss` aligned to eight, whereas the pinned
candidate requests 32 bytes of COMMON, the repository-wide tentative-symbol
rounding already controlled in the [data-owner audit](kf2-game-data-owner-fresh-audit.md).
Read-only `kf sema --image game addr` lookups for `0x8019e138` and
`0x8012da68` each report outside-load storage, no binding, and two incoming
value references. The same check finds no binding for the startup copy
destinations `0x801d8d88`, `0x800fa0d0`, and `0x800855a0` (one, two, and
two incoming value references). The arena base `0x8009b0a0` also has no
binding or navigator reference row despite its three raw address constructors;
the semantic index does not infer that address's source allocation. None of
these lookups establishes allocation size or a defining translation unit.
This batch retains no source, inventory, or compiler-profile edit; focused
objects and isolated strict comparisons were the only checks.

### Regional retail address-motion control

The local JP, US, and EU `GAME.EXE` files all contain `COM\\FDAT.T` at
`0x80011024`. Its unique adjacent signed-low address reference identifies
the corresponding startup archive-open sequence at JP `0x80015df8`, US
`0x80015e74`, and EU `0x8001627c`. Scanning the ordered `lui/addiu`
constructors around those sites gives:

| Startup pointer role | JP GAME | US GAME | EU GAME |
| --- | ---: | ---: | ---: |
| Resource arena base | `0x8009b0a0` | `0x8009bfe4` | `0x8009e11c` |
| Third copied resource | `0x801d8d88` | `0x801d9ccc` | `0x801dd5b8` |
| First TMD archive | `0x800fa0d0` | `0x800fb014` | `0x800fd11c` |
| Second TMD archive | `0x800855a0` | `0x800864e4` | `0x80088e44` |
| Transition TMD slot-zero pointer | `0x8012da68` | `0x8012e9ac` | `0x80130a90` |
| Transition loaded-callback destination | `0x8019e138` | `0x8019f07c` | `0x801a1190` |

The JP-to-US values all move by `+0xf44`; EU values move by differing
amounts, consistent with a changed layout. This is evidence against one
cross-region invariant literal address. It does not distinguish relocated
C objects from generated build constants, linker symbols, or a memory-policy
layout maintained separately for each program. In particular it proves no
complete array size or source translation-unit owner; the JP source literals
and candidate identities remain provisional.

Each region has three adjacent signed-low constructors for its own arena
base, in the same main-loop, startup-loader, and later resource-queue roles:
JP `0x8001389c/0x80015d64/0x80032200`, US
`0x800138dc/0x80015de0/0x80032f38`, and EU
`0x80013d38/0x800161d0/0x80034c90`. This co-moving three-site pattern
strengthens the address identity across regional builds; it still does not
identify the original linker or C declaration mechanism. Each main-loop
constructor is followed by `lui a1,5`; the call delay slot forms
`0x5f000` with `ori a1,0xf000`, confirming the same runtime arena
capacity in all three regional programs.
The arena end is therefore JP `0x800fa0a0`, US `0x800fafe4`, and EU
`0x800fd11c`. The first TMD archive destination is 0x30 bytes after
that end in JP and US, but exactly at the end in EU. This establishes a
version-dependent adjacency to the next workspace, not a uniform
array-declaration or linker-padding rule.
The same startup range constructs the `SsSetTableSize` workspace at
JP `0x8009a6a0` (`0x800139e0`), US `0x8009b5e4` (`0x80013a5c`), and
EU `0x8009df30` (`0x80013ebc`). With the SDK-derived `0x158`-byte
minimum table size, the gap from that table's end to the arena start
is `0x8a8` in JP/US but only `0x94` in EU. Thus both the lower and
upper arena neighbors change their padding between regional builds;
these addresses do not by themselves reveal a C declaration boundary.

The adjacent startup heap-boundary constructor moves from JP
`0x801da018` to US `0x801daf5c` and EU `0x801df038`, while the fixed
heap ceiling stays `0x801f8000`. JP and US arena and boundary
both shift by `+0xf44`; in EU the arena shifts by `+0x307c` but this
boundary shifts by `+0x5020`. Thus the arena is neither the boundary
itself nor a single invariant displacement below it across all three
programs. This narrows possible source expressions without establishing
whether the arena was a C object, linker symbol, or generated memory map.

The corresponding audio initializer likewise constructs the same four
workspace roles with signed-low pairs in each image:

| Audio workspace role | JP GAME | US GAME | EU GAME |
| --- | ---: | ---: | ---: |
| Sequence buffer | `0x80198640` | `0x80199584` | `0x8019b630` |
| First VAB slot | `0x80165a68` | `0x801669ac` | `0x80168a90` |
| Fifth-slot override | `0x80194e30` | `0x80195d74` | `0x80197e28` |
| Sixth-slot override | `0x80164a68` | `0x801659ac` | `0x80167a90` |

These four JP-to-US pointers also shift by `+0xf44`; EU shifts vary by
workspace family. Their ordered initializer uses support corresponding
runtime roles, but regional address motion still does not prove a complete
array extent or original defining translation unit. The four Japanese
source literals remain WIP, preserving the real retail referents until
that mechanism is established.

The callback destination is independently repeated twice in each regional
transition step, before its TMD destination constructor: JP
`0x80016924/0x80016a14`, US `0x800169a0/0x80016a90`, and EU
`0x80016db8/0x80016ec4`. This confirms the corresponding callback pointer
role, while leaving its maximum archive-loaded extent unresolved.
The startup copy constructor for `effect_state` moves from JP
`0x8019b6a8` to US `0x8019c5ec` and EU `0x8019e700`; each regional
callback destination lies exactly `+0x2a90` after that base. The complete
JP effect-state object ends at base `+0x2a8c`, leaving the same four-byte
gap before the callback destination. In fact each regional main-loop clear
passes exactly `0x0aa3` words in its call delay slot (JP/US
`0x8001374c`, EU `0x80013b84`), so the `0x2a8c` clear extent and four-byte
gap hold in all three builds. This repeated adjacency supports a
separate following workspace, but it still does not prove that workspace's
maximum size or original declaration.

## Startup data-consumer controls, 25 functions

Three source units connected to resource startup were compiled and compared
in isolation: `player_state_equipment` (16 functions), `event_state` (6), and
`asset_registry` (3). Twenty-four functions are strict exact. In the player
unit these are `player_get_camera_pose`, `player_reset_status`,
`player_initialize_state`, `game_initialize_session`,
`player_clear_motion`, `player_sync_position_to_map`,
`player_distance_to_point_in_cone`, `player_distance_to_point`,
`player_set_unknown_97`, `player_set_unknown_98`,
`player_set_unknown_99`, `player_set_equipment_slot`,
`player_equip_weapon`, `player_begin_weapon_attack`, and `func_80025878`.
All six event-state functions (`800482f8`, `800483a8`, `800483d8`,
`80048428`, `80048498`, `800484e4`) and all three asset-registry functions
(`asset_registry_load_tmd_archive`, `asset_registry_set`,
`asset_registry_select`) are also exact. The sole WIP is equipment update
`80025a18` at **98.02234% strict**; its owner is another active source
lane, so this is a read-only verdict.

The exact `game_initialize_session` consumer sits in a player unit whose
92 initialized DATA bytes are strict exact. Its target `.bss` comprises a
49,152-byte local weapon buffer, 1,224-byte `player_weapon_records`, and
71,748-byte `bss_801c7540` (122,124 bytes total). The pinned candidate
keeps the 49,152-byte local buffer in `.bss` but requests the two exported
objects as 1,224- and 71,752-byte COMMON symbols. The event-state unit's
six exact functions likewise coexist with target `.bss` 14,616 bytes and
candidate COMMON 14,616 bytes. These controls separate initialized-data
matching and function matching from the unresolved zero-storage section and
tentative-definition mechanism; they do not justify per-global placement
rules or new object extents. No source or config edit followed this pass.

### Callback-to-event-state regional boundary

The same three retail main loops each construct the beginning of the
`event_state` clear region: JP `0x801b2140` at `0x800136ec`, US
`0x801b3084` at `0x800136ec`, and EU `0x801b5198` at `0x80013b24`.
In each image this is exactly `+0x14008` after the independently repeated
loaded-callback destination (`0x8019e138`, `0x8019f07c`, and `0x801a1190`,
respectively). This is a useful adjacent-region bound, and the JP
`event_state` identity already owns its complete `0x3918` bytes. The
main-loop clear call passes `0x0e46` words in its delay slot in every
region (JP/US `0x800136fc`, EU `0x80013b34`), independently confirming
the same `0x3918`-byte event-state span. It is
**not** a proved `0x14008`-byte callback object: no archive-entry maximum
or defining source declaration has been recovered, and the gap may contain
other allocations or padding. The callback remains an unbound
workspace pointer in C and the inventory.

## CD stream and arena helper control, 25 new functions

One fresh pinned compile of `game.cd_memory` was compared against its
isolated retail target after the preceding resource pass. A one-unit safe
delink against the hash-checked JP GAME retail file, with the focused
report directory shortened off-tree to accommodate 57 addresses, produced
zero withheld functions or relocations. Its module object is SHA256-identical
to the cached target (`4af6bd82…bb711`), and direct objdiff against that
fresh object repeats the following verdicts. The module has
56/57 strict-exact functions; the only WIP remains the separately recorded
allocator at `0x80017608`. Its `0x1890`-byte text section has the same
size in both objects and all **263 ordered `.rel.text` offset/type/symbol
rows match**. The 11 initialized DATA and 33 RODATA bytes are strict
exact. These 25 newly selected stream/copy/arena helpers are each strict
100%, with no candidate source or referent difference to repair:

| GAME address | Function | Strict |
| --- | --- | ---: |
| `0x80016ed4` | `cd_stream_mark_complete` | 100% |
| `0x80016ee0` | `cd_map_stream_read` | 100% |
| `0x80016f10` | `cd_stream_limit_chunk` | 100% |
| `0x800171c8` | `resource_copy_words` | 100% |
| `0x800171f8` | `resource_copy_halfwords` | 100% |
| `0x80017228` | `repeat_store_word` | 100% |
| `0x8001724c` | `repeat_store_halfword` | 100% |
| `0x80017270` | `memory_arena_coalesce_free` | 100% |
| `0x800172f4` | `memory_arena_free` | 100% |
| `0x80017314` | `memory_arena_find_block` | 100% |
| `0x8001746c` | `memory_arena_wait_pending` | 100% |
| `0x80017504` | `memory_arena_compact` | 100% |
| `0x800175e8` | `memory_arena_initialize_blocks` | 100% |
| `0x800176c0` | `memory_block_release` | 100% |
| `0x800176e0` | `memory_block_set_kind` | 100% |
| `0x800176e8` | `memory_block_kind` | 100% |
| `0x800176f4` | `memory_block_set_flags` | 100% |
| `0x800176fc` | `memory_block_flags` | 100% |
| `0x80017708` | `memory_block_set_tag` | 100% |
| `0x80017710` | `memory_block_tag` | 100% |
| `0x8001771c` | `memory_malloc_checked` | 100% |
| `0x80017754` | `memory_allocate` | 100% |
| `0x8001777c` | `memory_free` | 100% |
| `0x800177d4` | `cd_vsync_handler` | 100% |
| `0x80017804` | `cd_wait_two_vsyncs` | 100% |

The CD module's zero-storage discrepancy remains separate from these code
verdicts. Its target `.bss` is 772 bytes: `cd_state` is 676 bytes and
`cd_archives` is 96 bytes. The pinned candidate emits COMMON symbols of
680 and 96 bytes, respectively, with no `.bss` section. The four-byte
`cd_state` excess follows the previously bounded tentative-definition
rounding pattern; neither exact consumer code nor this size comparison
establishes the original source/section mechanism. No C or config edit was
made for the batch.

Regional CD-code signed-low references independently preserve this
boundary. The repeatedly constructed `cd_state + 4` pointers are JP
`0x801b5d64`, US `0x801b6ca8`, and EU `0x801ba5ac`; the corresponding
`cd_archives` starts are `0x801b6004`, `0x801b6f48`, and `0x801ba84c`.
Subtracting four from the first address gives a `0x2a4`-byte state span
before archives in all three images. The repeatedly constructed stream
buffer pointers are JP `0x801b6064`, US `0x801b6fa8`, and EU
`0x801ba8ac`, each exactly `+0x60` after the archive-array start.
Other repeated state-tail addresses move with them. This supports the
complete `KfCdState` and eight-entry archive-array extents across
regions, while the stream buffer's own extent and the original
zero-storage emission mechanism remain unresolved.

An off-tree, one-unit `-fno-common` control did recover a 676-byte
`cd_state` symbol, but the pinned compiler placed both zero objects in
`.data`, after the 11 initialized bytes, ordered `cd_archives` before
`cd_state`, and emitted no `.bss`. Its `.data` grew to `0x310` bytes;
the target has 11 DATA bytes followed by 772 BSS bytes with
`cd_state` before `cd_archives`. Sixteen previously exact CD functions
also lost strict exactness, leaving 40/57 exact. This option alone is a
negative placement control, not a reason to change the shared profile or
invent per-global build placement.

## Fresh 25-claim resource transition and CD request chain

The current GAME transition graph was compared with narrow safe targets
and focused candidate objects: two startup functions, the transition
request, five phase setters, the transition step, the frame/CD step,
and 15 CD request/archive helpers. **Twenty claims are strict exact**:
all five `resource_transition_set_phase_{1,2,3,4,6}` functions and the
CD helpers `cd_wait_two_vsyncs`, `cd_request_advance`,
`cd_complete_handler`, `cd_data_ready_handler`, `cd_error_handler`,
`cd_bcd_to_int`, `cd_int_to_bcd`, `cd_location_to_sector`,
`cd_sector_to_location`, `cd_location_add`, `cd_request_wait_idle`,
`cd_request_wait_done`, `cd_sectors_corrupt`, `cd_request_enqueue`, and
`cd_archive_entry_extent`. All 15 CD targets were independently carved
safe with 65 relocations and none withheld. The startup's 83-byte
RODATA, transition step's 128-byte DATA and 28-byte RODATA are exact.

| WIP claim | Fresh strict text | Raw/source verdict |
| --- | ---: | --- |
| Startup loader `0x80015d58` | 89.03145% | 1/1 CFG block; ordered archive/copy calls agree. Target has six signed-low workspace pairs absent from the candidate's fixed literals; the archive/arena defining TUs remain unproved. |
| Startup pump `0x80015fd4` | 89.710144% | 3/3 CFG blocks and 1/1 branch; TMD slot-zero workspace still has a target-only signed-low pair. |
| Transition request `0x80016260` | 98.790085% | 67/67 CFG blocks and 49/49 branches. Target/candidate each have 135 text relocations; stack-byte argument allocation and internal branch layout differ without a new width/call fact. |
| Transition step `0x80016820` | 99.193474% | 62/62 CFG blocks and 32/32 branches, with an unresolved indirect switch jump in both. Retail has two `DAT_8019e138` and one `DAT_8012da68` HI16/LO16 pairs missing from the fixed-literal candidate. Text relocations are 196 target versus 190 candidate. |
| Frame/CD step `0x80036e24` | 98.86364% | 5/5 CFG blocks, 2/2 branches and 5/5 text relocations; saved-register assignments differ while the display call and loop parameters agree. |

The main loop and startup both construct resource arena base
`0x8009b0a0`; retail's signed-low address form is still incompatible
with the candidate fixed literal. The previously bounded runtime
`0x5f000` arena span does not prove its original defining object or
section. No source global, relocation owner, or profile change follows
from this recheck. Only focused quick builds and isolated strict objdiff
were run; no repository tests or full build were run.
