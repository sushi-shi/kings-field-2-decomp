# GAME floor-item capture and allocation: 25 strict verdicts

This cohort follows the five `game_main_loop` calls that create captured GPU
floor items, the slot finder and frame updater, the display/runtime owner,
and the arena-allocation path. The 25 rows are current isolated strict
`objdiff-cli diff` results after focused `kf try` builds of five GAME units.
They are connected call/data controls, not a proposed common translation unit.

| GAME address | Function | Strict text | Verdict |
| --- | --- | ---: | --- |
| `0x80013634` | `main` | 100% | Exact startup entry control. |
| `0x8001369c` | `game_main_loop` | 99.675530% | WIP fixed-arena address form after the five exact floor-item call sites. |
| `0x80017270` | `memory_arena_coalesce_free` | 100% | Exact arena coalescing control. |
| `0x80017314` | `memory_arena_find_block` | 100% | Exact free-block search. |
| `0x80017504` | `memory_arena_compact` | 100% | Exact allocation retry control. |
| `0x80017608` | `memory_arena_allocate_block` | 99.782610% | WIP: two arithmetic-result register assignments differ; allocation branches and calls agree. |
| `0x800176c0` | `memory_block_release` | 100% | Exact release control. |
| `0x8001771c` | `memory_malloc_checked` | 100% | Exact checked allocation wrapper. |
| `0x80017754` | `memory_allocate` | 100% | Exact callee of floor-item capture. |
| `0x8001777c` | `memory_free` | 100% | Exact free wrapper. |
| `0x8002ce2c` | `func_8002ce2c` | 100% | Exact 24-byte floor-item free-slot scan. |
| `0x8002ce68` | `func_8002ce68` | 65.851850% | WIP floor-item GPU capture: 40-byte retail versus 56-byte probe frame. |
| `0x8002cf40` | `func_8002cf40` | 100% | Exact animated image update. |
| `0x8002d0a4` | `fog_set_near` | 100% | Exact display control. |
| `0x8002d0d4` | `display_initialize` | 100% | Exact graphics-runtime initialization. |
| `0x8002d248` | `display_reset` | 100% | Exact floor-item/runtime reset. |
| `0x8002d32c` | `display_begin_frame` | 100% | Exact frame setup. |
| `0x8002d3c4` | `display_present_frame` | 100% | Exact frame present. |
| `0x8002d458` | `tmd_select` | 100% | Exact display object selector. |
| `0x8002d484` | `tmd_get_object` | 100% | Exact display object getter. |
| `0x8002d4a8` | `tmd_set_current_vertices` | 100% | Exact vertex pointer setter. |
| `0x8002d4b8` | `tmd_select_object_vertices` | 100% | Exact vertex selector. |
| `0x8002d4f4` | `func_8002d4f4` | 100% | Exact display sibling. |
| `0x80033584` | `display_toggle_buffer_index` | 100% | Exact frame-buffer toggle. |
| `0x800335a0` | `func_800335a0` | 100% | Exact direct caller of floor-item update. |

**Twenty-two functions are strict exact; three remain WIP.** Retail
`0x8002ce68` has five CFG blocks, two branches, and four ordered calls:
`func_8002ce2c`, `memory_allocate`, Sony `StoreImage`, and Sony `DrawSync`.
The only five proven incoming calls are at `0x80013814`, `0x80013834`,
`0x80013854`, `0x80013874`, and `0x80013894`; all pass kind 1. The next-frame
helper `0x8002cf40` has a proven direct call at `0x800335b0` in the exact
frame controller. SDK GPU functions remain library boundaries, not game
source claims.

The allocator's O32 arguments 5–7 are read after the free-slot call in
retail: `lbu` and `lw` from caller slot `sp+56` distinguish the stored kind
byte from the full-width kind-1 comparison; `lw sp+60` supplies the shifted
width, and `lhu sp+64` supplies the stored height. The allocation product
sign-extends the two halfwords before multiplication, then shifts left one.
The current seven-argument C signature and `KfFloorItem` layout express these
widths and the four calls. The probe instead loads arguments 5–7 into saved
registers before calling the slot finder, saving more registers in a 56-byte
frame. This is an unattributed lifetime/frame residue, not evidence for a
different value, field, or extra call. No volatile carrier, fake local, or
signature narrowing was added to steer code generation.

The other WIPs are independent. `game_main_loop`'s two differing words use
retail `lui/addiu` for the unowned `0x8009b0a0` arena address, whereas the
fixed C literal emits `lui/ori`; its five floor-item calls match. In
`memory_arena_allocate_block`, the size-minus-header intermediate uses `$v0`
in retail and `$a0` in the probe before the same subtraction. Neither
observation proves a source change. KF1's floor-item placement loader walks a
different placement-record stream, so it offers no seven-argument GPU-capture
body or ABI correction for KF2. This pass retains all source and inventory
claims unchanged.
