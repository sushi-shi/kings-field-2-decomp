# GAME initialized-data and BSS owner audit

The generated README's 29/34 data count predates new GAME claims. A read-only
`kf verify data --image game --detail` over existing artifacts reported 33/62,
but those objects were not uniformly current. Fresh **single-unit** safe
delinks, pinned compiles, and `data_match` comparisons rechecked each reported
initialized-data failure without a full build or repository tests. The retail
image was hash-checked by the safe delinker for every fresh carve.

All rechecked initialized sections have the expected size and data identities.
The ordinary `.data` payloads and relocations are byte-exact. Three old
`.rodata` failures were stale artifacts: `game.resource_startup` is 83/83 B,
`game.player_reaction` is 76/76 B, and `game.actor_behavior_dispatch` is
964/964 B. The old `game.memory_card_events` object-placement failure is also
stale: its fresh one-byte `.data` claim has no placement issue.

Nine current `.rodata` sections have complete byte extents and ordered
referents, but table-pointer **addends** differ because the compiled case
targets have different `.text` offsets:

| Unit | Retail / source `.rodata` size | First differing `.text` addend |
| --- | ---: | --- |
| `game.player_select_magic_action` | 100 / 100 B | `+0x0`: `0x148` / `0x144` |
| `game.collision_shape_dispatch` | 196 / 196 B | `+0x0`: `0x1cc` / `0x1b0` |
| `game.map_object_action_update` | 956 / 956 B | `+0x1c`: `0x1230` / `0x1228` |
| `game.actor_candidate_score` | 524 / 524 B | `+0x0`: `0x42c` / `0x46c` |
| `game.actor_group_effects` | 492 / 492 B | `+0x0`: `0x53c` / `0x554` |
| `game.effect_constructor` | 492 / 492 B | `+0x0`: `0x390` / `0x37c` |
| `game.effect_update_dispatch` | 516 / 516 B | `+0x0`: `0xfd4` / `0xf9c` |
| `game.event_target_stream` | 64 / 64 B | `+0xc`: `0x2c0` / `0x2c4` |
| `game.player_state_equipment` | 244 / 244 B | `+0x24`: `0x1120` / `0x1108` |

The first row has an especially tight control: `func_8002722c` matches
99.09091% in direct objdiff. Retail begins with `addiu sp,sp,-8` and ends
with `addiu sp,sp,8` in the return delay slot; the current probe omits that
frame. Its otherwise aligned body and ordered relocations place the same
first table case at source offset `0x144` versus retail `0x148`. The table
identity is sound; forcing the pointer addend would conceal a code-layout
difference. The unproved frame source is left WIP.

Two `.data` failures are genuine **placement** conflicts despite matching
section bytes. `game.menu_item_model` emits 28/28 B, but its claims at
`0x8006d68c` and `0x8006d694` interleave the separately owned
`input_idle_counter` at `0x8006d690`, while its three preview globals form a
second contiguous group at `0x8006da00..0x8006da13`.
`game.memory_card_directory` emits 311/311 B and its `.rodata` is 6/6 B;
its `0x80066630` prefix/assets group and the seven-byte
`DAT_8006d6a8` wildcard require different section bases. The identities
and bytes are supported, but neither original defining TU boundary is proved.
In particular, 17 nearby vendored LIBCD address pairs are negative controls
for attributing more storage to the card wildcard. No ownership was moved
merely to raise the placement score.

The remaining reported storage failures are not initialized-payload errors.
Fresh target objects carry `.bss` while the current GCC/assembler probe emits
tentative globals as COMMON for `game.main`, `audio_runtime`,
`resource_transition_request`, `menu_frame_begin`, `menu_display_state`,
`player_core_run`, `map_mask_window_sweep`, `display`, `actor_pool_clear`,
`effect_constructor`, `effect_scatter`, `effect_reset`, `event_counter`, and
`event_state`. The large `cd_memory` and `map_object_reset` source objects
likewise contain explicit `.comm` directives; their full-module focused
carves cannot use the current VA-list report directory because its name
exceeds the filesystem component limit. A fresh narrow carve resolves the
reported `game.player_state_equipment` extent gap into the same storage-class
issue. Its retail `.bss` has three named rows in order: private
`player_weapon_asset_buffer` 49,152 B at `+0`, exported
`player_weapon_records` 1,224 B at `+0xc000`, and exported `bss_801c7540`
71,748 B at `+0xc4c8`, totaling 122,124 B. The candidate has the exact
private 49,152-byte `.bss` row; the two exported definitions are COMMON
requests of 1,224 and 71,752 bytes (the latter rounded four bytes above
the C type). Thus the missing 72,972 B of `.bss` is already declared in C
under the curated WIP identities, but is unplaced. Fifteen of the unit's
sixteen functions remain direct strict 100%; `func_80025a18` is 95.85052%,
and its 92-byte initialized DATA payload compares exactly. These BSS and
COMMON claims need allocation/toolchain attribution, not invented initialized
bytes or a second overlapping global.

The fresh symbol tables isolate a repeatable COMMON-size effect. Across 15
fresh isolated GAME units, all 24 same-name retail `.bss`/candidate COMMON
symbol pairs have candidate size equal to the retail size rounded up to an
eight-byte multiple. The one-byte `menu_saved_music_enabled` is requested
as eight bytes; the other deltas are zero or four. Retail
`actor_state` is a 37,836-byte `.bss` symbol, while the current C emits a
37,840-byte COMMON request; retail `bss_801c7540` is 71,748 bytes versus a
71,752-byte COMMON request. The same `+4` appears for the 28-byte
`state_8017d118` and four-byte `DAT_80198630` and
`display_frame_cleared_word` (`32`, `8`, and `8` bytes of COMMON,
respectively). Eight-byte-multiple controls stay unchanged:
`render_mask_scan_state` is 32/32, `player_state` is 352/352, and
`event_state` is 14,616/14,616. The candidate assembly itself spells
`.comm actor_state,37840` and `.comm display_frame_cleared_word,8`; this is
the probe's tentative-definition emission, not evidence that the typed C
objects lack four bytes or that retail owns a larger overlapping object.

An isolated compiler control on tiny `game.menu_frame_begin` rules out a
simple unit-wide `-fno-common` switch. With the pinned default probe,
`current_poly_ft4` is `.comm current_poly_ft4,8`, the two code claims are
strict 100%, and `.data` is the expected 8 B. Adding only `-fno-common`
emits that tentative pointer in `.data` instead: candidate `.data` becomes
12 B against retail 8 B, `.bss` remains absent against retail 4 B, and
`menu_frame_begin` falls to 99.893616% (its eight-byte sibling stays exact).
This control changed no shared profile or source.

A separate off-tree GCC 2.6.0 `-O2 -G0` control distinguishes the available
zero-storage spellings: `int tentative;` emits `.comm tentative,4`,
`int explicit_zero = 0;` emits a `.word 0` in `.data`, and
`static int static_zero;` emits `.lcomm static_zero,4`. Thus adding explicit
zero initializers to the current exported definitions would move their bytes
to `.data`, not recreate the retail `.bss` symbols. File-local storage is a
plausible original form only where all users shared a defining TU; the linked
image alone does not identify that boundary. This control changed no project
source or profile.

The audit produced no safe new initializer or TU-owner edit. Keep the table
referents and data identities until the original source ownership or a
code-layout correction is independently supported.

## Resource arena boundary at `0x8009b0a0`

Current GAME retail `_SsInit` at `0x80050c74` constructs `0x8009a8a0` and
clears 32 rows of 16 words. This is the `0x800`-byte LIBSND `MarkCallback`
COMMON array, whose final written word is at `0x8009b09c`; the next byte is
the resource arena base `0x8009b0a0`. The curated `audio_sequence_table`
ends at `0x8009a7f8`, before the SDK callback array. The older address
examples for this array and other SDK globals in `sdk-data-ownership.md`
do not describe the current GAME image. Current `SsSetTableSize` constructs
its score-pointer table at `0x801d9588`, not the older `0x800a06e0` claim.
Those stale examples cannot establish an SDK object overlapping the arena.

Retail `game_main_loop` at `0x8001389c` passes `0x8009b0a0` and `0x5f000`
to `memory_arena_initialize_blocks`. Its exact body writes the first block
header at the base and the terminal `0xff` byte at `0x800fa09c`, establishing
the runtime extent `[0x8009b0a0, 0x800fa0a0)`. Resource startup at
`0x80015d64` reads an archive into the same base before initialization, and
`resource_tmd_queue_read` at `0x80032200` allocates from it later. All three
sites form the base with `lui 0x800a` followed by signed `addiu -0x4f60`.
An adjacent HI/low-opcode scan of current GAME code found these three direct
base constructors and no direct interior constructor; this scan does not rule
out derived or nonadjacent references. The first separately observed copy
destination after the arena is `0x800fa0d0`, 0x30 bytes beyond its end, and
`display_primitive_memory` starts at `0x800fba58`.

The array-end adjacency and exact allocator capacity support a reserved RAM
span, but do not distinguish a declared game BSS array from a linker boundary
or another allocation mechanism. It is specifically **not** the current
`BSS_END` label: `config/retail/link_labels.tsv` places that GAME boundary at
`0x801da018`, and retail `main` constructs that separate address at
`0x8001364c/50` before passing it to `InitHeap`.
KF1's memory arena is allocated through `malloc`, so its source shape cannot
identify KF2's original arena declaration either. No current
`data_identities.tsv` row defines the arena, and its three raw base pairs have
no `relocs.tsv` rows. The current fixed-literal C macro emits `lui/ori`
instead of retail's signed-low pair.
Without a defining object, complete source extent, and owning TU, this audit
does not introduce an overlapping global, bind an extern, or alter metadata.

A 28-claim caller/allocator control reinforces that limit. Isolated strict
comparisons and the allocator owner's focused exact control identify 20 exact
functions: `main`, ten `player_core_run` consumers,
three `asset_registry` helpers, five `resource_runtime` siblings, and
`memory_arena_initialize_blocks`. Eight remain WIP: `game_main_loop`
(99.67553%), the two resource-startup bodies (89.03145%, 89.710144%),
the radius, visibility, and TMD-queue runtime bodies (74.061226%, 93.6%,
98.4359%), `memory_arena_allocate_block` (99.78261%), and frame-step CD
service (98.86364%). Fresh safe carves for the selected units withheld no
relocations; the allocator result is an independent focused control from its
current owner. The three raw constructors at GAME `0x8001389c/8a0`,
`0x80015d64/68`, and `0x80032200/04` use `lui 0x800a` then signed
`addiu -0x4f60`; the current literal uses `lui 0x8009` then `ori 0xb0a0`.

The pinned Psy-Q 3.0 `LIBSND.LIB` member `SSINIT.OBJ` independently declares
`MarkCallback` as a native `XBSS` symbol of size `0x800`. Its public
`LIBSND.H` declares `SsSetMarkCallback` and the callback type, but not the
`MarkCallback` object. Thus array-end adjacency does not establish that GAME
C referred to the SDK-private symbol to derive the arena. The exact
initializer establishes the `0x5f000` **used capacity**, not a complete C
object size: the next separately observed destination begins 0x30 bytes
after that capacity. No pinned linker command/map or game defining object
proves a named symbol or placement at this base. The truthful fixed pointer
stays in source, and the missing original address mechanism remains open.

The startup copy destination `0x801d8d88` has a similarly suggestive but
incomplete boundary. Curated `bss_801c7540` ends at `0x801d8d84`, four bytes
before it. Current retail `SsSetTableSize` forms its separate LIBSND
`_ss_score` table base at `0x801d9588`, exactly `0x800` bytes beyond the copy
destination. Only the startup `0x80015ea4/a8` pair directly constructs an
address in the intervening span in the adjacent-pair scan. This supports a
candidate separate copy buffer, but the length-prefixed archive record's
maximum size and original definition are unavailable; the `0x800` gap is not
promoted to a proved C array extent.

The second embedded TMD archive copied by resource startup uses destination
`0x800855a0`. Its two direct constructors are at `0x80015f94` and
`0x80015fac`; the next address found by the adjacent-pair scan is a vendored
SDK datum at `0x8009a5a0`, exactly `0x15000` bytes later. The scan
found no direct interior constructor. This is a strong candidate maximum
workspace span, but the variable archive record length and original owner
remain unproved. `asset_registry_load_tmd_archive` stores pointers into both
copied archives, so both workspaces must remain live after startup. The first
TMD destination at `0x800fa0d0` lacks an equally clean end boundary: its
next directly referenced SDK storage is
`0x800fba38`, `0x1968` bytes later, before `display_primitive_memory`.

## Audio workspace boundaries

Retail GAME `func_800139c4` stores `0x80198640` in
`audio_state.sequence_buffer`. The next address found by the adjacent-pair
scan is `0x8019b640`, exactly `0x3000` bytes later, used by exact vendored
LIBCD/SYS `CdReadCallback` as its callback word. LIBSND/VMANAGER references
start at `0x80198638`, eight bytes before the sequence pointer. The KF1
game audio source independently requests a `0x3000`-byte sequence allocation.
Together these support a strong `0x3000` candidate span, while the GAME
archive entry's maximum size, original allocation class, and defining TU
remain unproved.

The same startup routine seeds seven VAB slot pointers at `0x80165a68` in
`0x1000` increments, then overrides slot 5 with `0x80194e30` and slot 6
with `0x80164a68`. The retained slots 0..4 span
`[0x80165a68, 0x8016aa68)`, and the slot-6 pointer sits in the preceding
`0x1000`-byte interval. Current vendored LIBSND `_SsInit` directly touches
`0x8016aa68`, exactly after those six contiguous chunks. The slot-5
override equals the end of `game_graphics_runtime` and lies `0x2800` bytes
before `audio_state`. The separate transition TMD workspace pointer
`0x8012da68` starts `0x10` bytes after the complete
`display_primitive_memory` extent; vendored SDK code directly addresses the
intervening `0x8012da58` and `0x8012da60` words. The TMD pointer lies
exactly `0x37000` bytes before the slot-6 pointer, with no directly
constructed interior address in the adjacent-pair scan. These are useful
non-overlap and capacity bounds; they do not prove whether the six chunks
share one C object, whether slot 5 owns the entire `0x2800` interval, or
whether the TMD workspace fills its `0x37000`
gap. Its `cd_archive_queue_read` producer writes sector-counted entries, so a
nonempty TMD read requires at least `0x800` writable bytes at this base. The
defining source modules also remain open. Literal workspace pointers
and their unbound relocation targets remain unchanged.

The GAME loaded callback destination `0x8019e138` has another useful bounded
placement. The complete startup-cleared `effect_state` ends at `0x8019e134`,
four bytes before it; the next complete startup-cleared `event_state` starts
at `0x801b2140`, a gap of `0x14008` bytes. A `0x14000`-byte loaded region
would leave eight bytes before that next object, but neither the CD archive
entry's maximum length nor its original definition is known. The initialized
fallback table has 32 pointers, and current GAME callers use loaded slots
through index 19, proving at least an 80-byte callable prefix. The exact
`cd_archive_queue_read` path computes its read size from the entry's sector
offset difference in 2,048-byte units, so any nonempty loaded table writes at
least one sector there. Neither fact proves that the entire intervening gap
is one source-level array.

The separate GAME `cd_stream_work_buffer` at `0x801b6064` has a tighter
lower bound than its four-byte candidate inventory anchor suggests. Resource
transition phase one copies `0x3e80` words from `buffer + 4`, a readable
range of at least `0xfa04` bytes from the base. The exact archive-queue path
expresses loaded entry lengths in `0x800`-byte CD sectors. If the entry
provides that whole copied range, its read is at least `0x10000` bytes after
sector rounding. The next independently used BSS address is the
`DAT_801c7068` motion-vector view, `0x11004` bytes after the buffer base.
This bounds a plausible stream allocation between a 64-KiB sector-rounded
load and that next live address; the archive's embedded second range and the
original C definition are still unknown. No buffer extent is promoted in the
inventory.

The address-gap inventory is a triage aid, not a set of new C array claims:

| GAME base and use | Next boundary or referenced address | Gap | Status |
| --- | --- | ---: | --- |
| `0x800855a0` second TMD copy | `0x8009a5a0` SDK datum | `0x15000` | Candidate maximum |
| `0x8009b0a0` resource arena | `0x800fa0a0` allocator end | `0x5f000` | Proven runtime span; source owner open |
| `0x800fa0d0` first TMD copy | `0x800fba38` SDK datum | `0x1968` | Candidate maximum |
| `0x8012da68` transition TMD | `0x80164a68` VAB slot 6 | `0x37000` | At least one CD sector; candidate maximum |
| `0x80164a68` slot 6 plus slots 0..4 | `0x8016aa68` SDK datum | `0x6000` | Six observed `0x1000` chunks; object split open |
| `0x80194e30` VAB slot 5 | `0x80197630` audio state | `0x2800` | Candidate maximum |
| `0x80198640` sequence data | `0x8019b640` SDK datum | `0x3000` | Candidate maximum; KF1 size agrees |
| `0x8019e138` loaded callbacks | `0x801b2140` event state | `0x14008` | At least one CD sector written and 20 pointers used; maximum/definition open |
| `0x801b6064` map stream | `0x801c7068` motion vector | `0x11004` | `0xfa04` bytes read by phase one; valid sector-rounded input at least `0x10000` |
| `0x801d8d88` startup copy | `0x801d9588` SDK score table | `0x800` | Candidate maximum |

KF1's audio startup requests a `0x3000`-byte sequence buffer through its
arena allocator, which agrees with the KF2 sequence-address gap as a capacity
analogue. It is not evidence that KF2 declared a `0x3000`-byte static array at
`0x80198640`; that base's original definition and allocation mechanism remain
open.

## Prepared TMD target freshness control

A focused GAME safe delink of `0x8002ff5c` from the hash-checked retail image
produces module target SHA-256
`cb01ce8f749c3697dbf004a27ccc1c3dc99d5d9de25efab52e1a9b4aca8f8d51`,
byte-identical to the existing `build/delink` module target. It has 16 ordered
text relocations. The current `tmd_prepared_subdivide.c` was independently
compiled off-tree with its complete manifest GCC 2.5.7 `-O2 -G0`
**`-mcpu=r2000`** probe: direct strict objdiff is **55.263805%** over 3,260
retail bytes and 2,788 candidate bytes.
The focused listing has 11/11 CFG blocks, 6/6 branches, and all 14 proven
`resource_copy_words` calls; retail/current frames are 1,248/1,232 bytes.

For a like-for-like source comparison, the parent of source commit `de6fc1b`
was compiled with the same complete profile, headers, and target. Its strict
result is **52.31411%** over 2,724 candidate bytes, so that commit improves
the controlled comparison to **55.263805%**. The earlier dossier's
55.263805% score is confirmed, although its 2,796-byte candidate size and
1,240-byte candidate frame are stale against this fresh object. Omitting the
profile's `-mcpu=r2000` flag instead yields a different 51.295704% control;
that incomplete-profile result is not the project's match score. This pass
changed no source, profile, or inventory; packet scratch layout remains WIP.

## Effect-constructor BSS linkage control

GAME `effect_constructor` references `DAT_8009a5a8` and `DAT_801d9628` in
its current C. No other source unit or current `kf sema` xref was found for
either symbol. A single off-tree change made both tentative definitions
`static`: the pinned candidate then emitted `.bss` size `0x908`, with a
four-byte cooldown word at offset `0` and the 2,304-byte trail array at
offset `+8`. Both object symbols scored 100% in isolated objdiff, matching
the current delink target's sizes and offsets. The baseline tentative
definitions instead emit COMMON symbols with no candidate `.bss` section;
explicit zero initializers emit `.data` and do not model the target section.

The target object's `.bss` has alignment 8 and its curated symbols are
GLOBAL. The `static` probe has alignment 4 and LOCAL symbols. Those target
binding choices are part of the working delink model, not recovered retail
symbol facts, but the control still does not establish the original defining
translation unit or complete extent. In particular, the `0x900` trail array
is an observed minimum span. Both identities remain `candidate`, and no
source, identity, relocation, or profile change was retained.

## Resource/event ten-unit owner control

A fresh manifest-profile compile and isolated strict objdiff of ten GAME
resource/event units covers 22 functions: 13 already exact and nine WIP.
These are direct object comparisons against the existing delink targets;
no broad build or source/config change was made. Per-function verdicts:

| Unit | Strict function verdicts |
| --- | --- |
| `resource_startup` | `0x80015d58` 89.03145% WIP; `0x80015fd4` 89.710144% WIP |
| `resource_transition_request` | `0x80016260` 98.790085% WIP |
| `resource_transition_step` | `0x80016820` 99.193474% WIP |
| `resource_runtime` | `resource_registry_get`, `resource_tmd_read_complete`, `map_cell_layer_mask`, `resource_vab_update_range`, and `resource_tmd_update_range` exact; `map_cell_layer_mask_radius` 74.061226%, `map_cell_visible` 93.6%, and `resource_tmd_queue_read` 98.4359% WIP |
| `event_target_stream` | `0x800462bc` 98.68132% WIP |
| `event_command_dispatch` | `0x8004678c` exact |
| `event_world_dispatch` | `0x80047c98` 99.81618% WIP |
| `event_state` | `0x800482f8`, `callback_invoke_slot_04_zero`, `0x800483d8`, `0x80048428`, `0x80048498`, and `0x800484e4` exact |
| `event_save_stream` | `0x80048554` exact |
| `event_restore_stream` | `0x800489ac` 98.82883% WIP |

The startup target has 127 `.rel.text` rows against 115 in the pinned
candidate. Its **six** target-only HI16/LO16 pairs are one each for
`DAT_801d8d88` and `DAT_8012da68`, and two each for `DAT_800fa0d0` and
`DAT_800855a0`. Target object instructions at offsets `+0x14c`, `+0x200`,
`+0x228`, `+0x23c`, `+0x254`, and `+0x354` use `lui` followed by signed-low
`addiu`. The corresponding C call arguments are fixed address literals;
the candidate has no substitute relocation. Transition step has 196/190
`.rel.text` rows: two target-only pairs for `DAT_8019e138` at `+0x104` and
`+0x1f4`, and one for `DAT_8012da68` at `+0x22c`. They likewise use
`lui/addiu`, while the C uses fixed literals. All other type/symbol
multiplicities in those two text relocation tables agree. These nine pairs
support the referent addresses and expose the source-binding gap; they do
not establish complete object extents or defining translation units, so no
tentative workspace was promoted to a fabricated global.

The fresh target `resource_transition_request` `.bss` is 28 bytes with
`state_8017d118` at `+0`; the candidate instead emits a 32-byte COMMON
request, one instance of the already measured tentative-definition rounding.
Fresh target `event_state` `.bss` is 14,616 bytes; the candidate requests a
same-sized COMMON symbol. Both have exact text siblings where listed above,
so the section-class difference is independent of their function scores.

Initialized section controls are narrow and positive: startup `.rodata`
is 83/83 bytes exact; transition step `.data` is 128/128 bytes with all
32 pointer relocations and `.rodata` 28/28 bytes with seven table relocations
exact; event command's five eight-byte DATA objects, 40-byte section, and
35-row switch table are exact; event save/restore tables are exact. Event
target stream retains 16/16 table pointer relocations and the same 64-byte
extent, but the two raw pointer addends at `+0xc` and `+0x3c` are each four
bytes later in the candidate (`0x2c0/0x2c4` and `0x2dc/0x2e0`). Its raw
retail action path lacks a candidate load-delay nop, explaining the text
layout shift; the target table entries are not adjusted to hide that gap.

## Adjacent event/CD owner control

Seven more disjoint GAME units were compiled under their manifest profiles
and compared in isolation. Of their 71 function claims, 68 are already
strict exact. The complete unit verdicts are:

| Unit | Function verdict |
| --- | --- |
| `resource_transition_phase` | All five phase callbacks exact |
| `cd_memory` | All 57 functions exact except `memory_arena_allocate_block` 99.78261% WIP |
| `event_pose_interpolate` | Both functions exact |
| `message_stream_find_marker` | `0x80046144` exact; `0x800461a0` 99.12676% WIP |
| `event_map_object_spawn` | `0x80046700` exact |
| `event_counter` | All three functions exact |
| `event_map_object_controller` | `0x800475d8` 99.166664% WIP |

Each unit's target and candidate `.rel.text` counts agree. The allocator's
single instruction difference at object offset `+0x78c` is a temporary
register choice: retail uses `addiu v0,a1,-12` then subtracts from `v0`,
while the candidate computes the same value directly in `a0`; its calls,
CFG, and 46-instruction extent match. The marker function's pointer
register/origin differs while retaining the same accesses and 71-instruction
extent. The event controller has matching 432-instruction extents and
ordered relocation counts, but a late shared-path layout/register gap; this
screen found no independently supported field or referent correction.

The `cd_memory` target's 11-byte `.data` and 33-byte `.rodata` are exact,
including `cd_path_prefix` and `cd_version_suffix`. Its target `.bss` is
772 bytes: `cd_state` 676 bytes at `+0`, followed by `cd_archives` 96 bytes
at `+0x2a4`. The candidate instead requests separate COMMON symbols of
680 and 96 bytes. This is the same rounded tentative-definition/placement
issue as other data owners, not an initialized-data mismatch. Likewise the
target `event_counter` has a 120-byte `.bss` `game_counter_bytes`, while
the candidate requests a 120-byte COMMON symbol. No C or inventory owner
was changed to accommodate those section differences.

## Two map-grid owner controls

Fresh isolated objects leave `map_mask_window_sweep` `0x8002c670` at
91.624245% strict, and `map_cell_pattern_place` `0x80034f90` and
`0x80035194` at 97.86822% and 89.59545%. The 28-byte signed-pair
`DAT_80067874` DATA object is byte exact. The target mask unit puts its
32-byte `render_mask_scan_state` in `.bss`, while the pinned candidate
requests a same-sized COMMON symbol. Target/candidate text relocation
counts are 82/86: the four extra candidate rows are precisely two
HI16/LO16 pairs to that **same** scan-state object, consistent with
address-base reuse rather than a missing identity. The pattern-placement
unit has 11/11 text relocations, no owned initialized storage, and retains
its known frame/induction differences. No map C or data-owner claim was
changed.

For the separately linked overlays, see the focused
[OPEN/END data-owner audit](kf2-open-end-data-owner-focused.md). It confirms
initialized sections independently from their BSS/COMMON placement gaps.
