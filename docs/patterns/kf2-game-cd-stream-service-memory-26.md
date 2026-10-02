# GAME CD stream service and memory boundary: 26 strict verdicts

On 2026-10-02 after the `dc488d8` checkpoint, `game.cd_memory` was freshly
compiled under its pinned manifest profile into an isolated `/tmp` object and
compared with strict `objdiff-cli diff`. This is a disjoint 26-claim cohort
from the preceding CD request/archive run: the stream callback and service,
word/halfword copies, arena/block helpers, malloc wrappers, queue yield and
VSync counter. Twenty-five functions are exact. The sole WIP is the allocator
at 99.78261%; its referents and control flow agree.

| GAME VA | Function | Strict verdict |
| --- | --- | --- |
| `80016ed4` | `cd_stream_mark_complete` | 100% |
| `80016f10` | `cd_stream_limit_chunk` | 100% |
| `80016f4c` | `cd_request_service_stream` | 100% |
| `800171c8` | `resource_copy_words` | 100% |
| `800171f8` | `resource_copy_halfwords` | 100% |
| `80017228` | `repeat_store_word` | 100% |
| `8001724c` | `repeat_store_halfword` | 100% |
| `80017270` | `memory_arena_coalesce_free` | 100% |
| `800172f4` | `memory_arena_free` | 100% |
| `80017314` | `memory_arena_find_block` | 100% |
| `8001746c` | `memory_arena_wait_pending` | 100% |
| `80017504` | `memory_arena_compact` | 100% |
| `800175e8` | `memory_arena_initialize_blocks` | 100% |
| `80017608` | `memory_arena_allocate_block` | 99.78261% WIP |
| `800176c0` | `memory_block_release` | 100% |
| `800176e0` | `memory_block_set_kind` | 100% |
| `800176e8` | `memory_block_kind` | 100% |
| `800176f4` | `memory_block_set_flags` | 100% |
| `800176fc` | `memory_block_flags` | 100% |
| `80017708` | `memory_block_set_tag` | 100% |
| `80017710` | `memory_block_tag` | 100% |
| `8001771c` | `memory_malloc_checked` | 100% |
| `80017754` | `memory_allocate` | 100% |
| `8001777c` | `memory_free` | 100% |
| `8001779c` | `cd_request_yield` | 100% |
| `800177d4` | `cd_vsync_handler` | 100% |

The sole allocator raw difference computes the same remainder through
different temporaries: retail uses `addiu v0,a1,-12; subu a0,v0,s1`, while
the candidate uses `addiu a0,a1,-12; subu a0,a0,s1`. Its 184-byte extent,
calls, CFG and ordered relocations match. The full unit has 263/263 identical
ordered relocation site/type/referent tuples, exact 11-byte initialized DATA
and exact 33-byte RODATA. This selected band did not introduce a new data
referent mismatch.

The early stream service reads CD requests through the 16-entry request ring
in `cd_state`; `cd_request_yield` calls both VAB and stream service before
GPU/VSync waits, and `cd_vsync_handler` increments the two counter words.
Their exact instructions reinforce the typed `0x2a4` state extent and the
adjacent eight-entry, `0x60`-byte `cd_archives` array. The retail target
still models 676+96 bytes in `.bss`; the current source produces COMMON
requests of 680+96. Regional JP/US/EU direct pointer constructions retain
the same `+0x2a4` state-to-archives and `+0x60` archives-to-stream spacing.
Those observations support boundaries but do not identify the original
zero-storage declaration or linker rule.

These exact memory routines also clarify the distinct source mechanisms.
`memory_arena_allocate_block` returns an arena payload and `memory_allocate`
wraps `malloc`, yet the retail GAME audio initializer calls neither: after
its eight Sony sound calls it directly constructs the four sequence/VAB
workspace addresses with signed-low pairs and stores them in `audio_state`.
Likewise the exact map stream wrapper directly passes the address following
`cd_archives` to the archive reader; it does not obtain that work buffer from
either allocator. These negative call-set facts exclude a dynamic return
value in those assignments. They do not prove whether the addresses came
from C globals, linker symbols, generated constants, or another build
mechanism. The `cd_stream_work_buffer` readable prefix remains at least
`0xfa04` bytes, with a `0x11004`-byte gap to the next independently live BSS
datum; its complete allocation extent and defining TU remain unresolved.
No C, identity, relocation, compiler-profile, or linked-EXE claim changes.
