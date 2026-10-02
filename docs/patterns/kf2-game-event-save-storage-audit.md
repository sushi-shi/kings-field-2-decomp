# GAME event and save-stream storage audit

On 2026-10-02, twelve manifest-contiguous GAME units from
`game.event_pose_interpolate` through `game.event_restore_stream` were freshly
compiled with the pinned `probe-gcc260-o2-g0` profile into an isolated `/tmp`
directory and compared directly with their delinked objects using strict
`objdiff-cli diff`. This is a 21-function control cohort. It does not include
the adjacent memory-card payload unit, whose ownership is a separate campaign.
Fifteen functions are exact; six retain the source/codegen residues below.

| Unit | Function(s) | Strict verdict |
| --- | --- | --- |
| `event_pose_interpolate` | `func_80045f20`, `func_80045fd4` | 100%, 100% |
| `actor_animation_seek_phase` | `func_800460a0` | 99.268295%; first difference is `s1`/`s3` assignment for the masked phase and its half-value |
| `message_stream_find_marker` | `func_80046144`, `func_800461a0` | 100%; 99.12676%, first difference swaps the `a0`/`a1` stream cursors at actor offsets 20 and 23 |
| `event_target_stream` | `func_800462bc` | 98.68132%; candidate text is four bytes longer and two switch-table destinations move by four bytes |
| `event_map_object_spawn` | `func_80046700` | 100% |
| `event_command_dispatch` | `func_8004678c` | 100% |
| `event_counter` | `func_800473e0`, `func_80047434`, `func_800474c4` | 100%, 100%, 100% |
| `event_map_object_controller` | `func_800475d8` | 99.166664%; first register difference is `s1`/`s0`, followed by a four-byte branch-target shift |
| `event_world_dispatch` | `func_80047c98` | 99.81618%; first difference swaps `s4`/`s5` roles |
| `event_state` | `func_800482f8`, `callback_invoke_slot_04_zero`, `func_800483d8`, `func_80048428`, `func_80048498`, `func_800484e4` | 100% for all six |
| `event_save_stream` | `func_80048554` | 100% |
| `event_restore_stream` | `func_800489ac` | 98.82883%; first difference swaps `a1`/`a2` roles for the `0xff` sentinel and `actor_state` base |

The exact code controls also agree in raw `.text` bytes and ordered relocation
sites, types, and target symbols. All 253 relocations in
`event_command_dispatch`, 26 in `event_counter`, 30 in `event_state`, and 195 in
`event_save_stream` agree. The two event stream switch tables deserve a
separate verdict: `event_save_stream` has 660 identical `.rodata` bytes and 165
identical ordered table relocations; `event_restore_stream` has 64 identical
bytes and 16 identical table relocations. `event_target_stream` has the same
16 table relocation sites and a 64-byte extent, but words at table offsets
`0x0c` and `0x3c` point four bytes later in the candidate. These are internal
text addends downstream of its extra instruction, not evidence for a different
data owner. The target action path lacks the candidate's load-delay `nop`,
which accounts for the layout shift. Its 73 total relocations retain the same
referents but later text sites shift by four bytes. `event_map_object_controller` and
`event_restore_stream` likewise have relocated instruction sites shifted with
their non-exact code; no referent correction follows from the current evidence.

Five initialized eight-byte lists at GAME `0x800679a0`–`0x800679c8` are
source-defined in `event_command_dispatch`. Their ordered global object
symbols, full 40-byte `.data` contents, and the unit's 140-byte `.rodata`
switch table match exactly. Each list contains command IDs followed by `0xff`
sentinels/padding; the source's defining TU is a productive model, not proven
original linkage. There is no byte or relocation reason to move these lists.

`game_counter_bytes` at GAME `0x8009a5e8` has a bounded 120-byte extent:
`func_800482f8` clears `0x1e` words, the counter helpers use byte indices,
menu consumers use indices up to 119, and the card payload copies the whole
array. `event_state` at GAME `0x801b2140` has a bounded `0x3918`-byte extent:
the main loop clears `0xe46` words, the exact event initializer separately
clears 0x100 control bytes and a 0x3800-byte arena, and exact save helpers
address ten trailing halfword offsets at `+0x3904`. The typed source layout
checks these offsets and size. Cross-unit code reads and writes both objects,
supporting external source visibility. The fresh candidates emit global
COMMON requests of 120 and 14,616 bytes, while the curated target objects
place same-sized global symbols in `.bss` with eight-byte section alignment.
Those target BSS placements are delinker models; retail linked bytes do not
establish whether original source used tentative declarations, explicit zero
definitions, or another linker allocation mechanism. Preserve the current
source and do not promote either storage-class discrepancy to a source fix.

This audit preserves all exact functions and makes no C, identity, or compiler
profile change. It uses no linked-EXE equality claim.
