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

The audit produced no safe new initializer or TU-owner edit. Keep the table
referents and data identities until the original source ownership or a
code-layout correction is independently supported.
