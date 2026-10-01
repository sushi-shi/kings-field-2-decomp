# GAME graphics packet and GPU-transfer evidence batch

This 25-function batch follows the display primitive buffer, its POLY_FT4
callers, the TMD preparation path, the frame driver, and the TIM/VRAM transfer
path. Every address is in GAME.EXE. The current `kf sema match` report provides
the exact scores below; focused `kf try` compares were run for the textured
quad and TIM-transition units. Exact siblings are controls, not new claims.

| Function | Retail connection | Verdict |
| --- | --- | --- |
| `0x80021f10` | Initializes the current FT4 packet for UI primitives. | Exact, 100%. |
| `0x80021f60` | Commits the FT4 packet into the ordering table. | Exact, 100%. |
| `0x80021fb0` | Adjacent menu-list initializer; menu-owned control. | Exact, 100%. |
| `0x8002d0a4` | Sets fog near distance in the display/GTE state. | Exact, 100%. |
| `0x8002d0d4` | Initializes GPU environments and geometry state. | Exact, 100%. |
| `0x8002d248` | Resets two primitive buffers and shared render state. | Exact, 100%. |
| `0x8002d32c` | Selects the next primitive buffer and clears its OT. | Exact, 100%. |
| `0x8002d3c4` | Synchronizes GPU, swaps environments, draws the OT. | Exact, 100%. |
| `0x8002d458` | Selects a TMD asset slot. | Exact, 100%. |
| `0x8002d484` | Returns an object from the current TMD. | Exact, 100%. |
| `0x8002d4a8` | Selects the projected vertex pointer. | Exact, 100%. |
| `0x8002d4b8` | Selects an object's vertex array. | Exact, 100%. |
| `0x8002d4f4` | Updates the view position, rotation, and matrices. | Exact, 100%. |
| `0x8002ff5c` | `0x80030c18` passes asset, object index, and a 4096-byte prepared TMD buffer; retail has 14 call sites to `resource_copy_words`. | Unclaimed WIP. The 1248-byte frame contains a 1024-byte midpoint workspace at `sp+64..sp+1088`, equivalent to 128 eight-byte `SVECTOR` entries. Retail forms x/z interior pointers at `sp+64` and `sp+68`, then copies two words per appended midpoint into the output TMD; the Ghidra candidate's two arrays are an artifact of that interior view. Packet-control and remaining scalar extents still need a humane source body. |
| `0x800311b0` | Four exact graphics callers pass fifteen O32 arguments to build and enqueue a textured quad. | WIP, 92.14815% current report; focused listing DIFF. Packet and call semantics agree, while retail saves another register and schedules the packet-code store in a branch delay slot. No unsupported steering edit retained. |
| `0x800312f4` | First sliding textured panel calls `0x800311b0`. | Exact, 100%. |
| `0x80031384` | Second sliding textured panel calls `0x800311b0`. | Exact, 100%. |
| `0x80031414` | Color-byte graphics path calls `0x800311b0`. | Exact, 100%. |
| `0x800314d4` | Sets four adjacent graphics color/control bytes. | Exact, 100%. |
| `0x800314fc` | Three-channel collision-overlay path calls `0x800311b0`. | Exact, 100%; collision owner retains its source. |
| `0x8003247c` | Frame driver calls the 96-block actor/resource/placed-object dispatcher. | Initially unclaimed WIP; a later source claim models its resource passes. Its first loop is `actor_state.actors` at `0x8016b600`, advances `sizeof(KfActor)==124` for 200 actors, and needs no incoming arguments. Entry `repeat_store_word` calls clear 32 and 16 words at `sp+88` and `sp+408` (128- and 64-byte regions); a later call clears 80 words from `sp+88`, establishing a 320-byte maximum cleared extent up to the second region. The first region is indexed by actor asset ID. Retail stores three rotation halfwords at `sp+56..60` and passes `sp+56` as the fourth argument to `0x80031850`; its fifth argument is the `SVECTOR` scale pointer at `actor+72`, stored in the outgoing slot at `sp+16`. Its last loop starts at graphics runtime `+0x170f0` and advances 24 bytes for 128 `KfMapPlacedEntry` records, matching the existing type. The source remains WIP. |
| `0x800335a0` | Per-frame render driver calls `0x8003247c`. | Exact, 100%. |
| `0x8003494c` | TIM iterator uploads CLUT and image rectangles with `LoadImage`. | Exact, 100%; focused listing SAME. |
| `0x800349bc` | Emits four textured fade quads, presents frames, and polls pad input. | WIP, 92.34296% current report; focused listing DIFF. Retail's 72-byte frame spills the return state, while the current 64-byte probe keeps it in a saved register. The source is semantically faithful; no fabricated spill was added. |
| `0x80034e10` | Archive TIM transfer snapshots VRAM with `StoreImage`/`MoveImage`, then restores it with `LoadImage`. | Exact, 100% current report and focused listing SAME. |

The `0x8003247c` identity now records the observed `void func_8003247c(void)`
signature. Its `0x7c` actor stride agrees with the source-owned `KfActor`
layout and the 200-record limit in retail; it does not imply that the later
map, audio, and render sections share a historical translation unit with the
actor definition. The fixed arena address in the nearby `0x800321d8` TMD
archive reader remains unresolved and was not changed.

The prepared-TMD copier handles packet mode bytes `0x2c/0x2e` as FT4 and
`0x24/0x26` as FT3, preserving the semitransparency bit in either pair. For
each matched source packet it emits four packets: 128 bytes and five new
vertices for FT4, or 96 bytes and three new vertices for FT3. Its final
`resource_copy_words` call copies two 32-bit words per new vertex from the
same midpoint workspace into the output asset. The caller's prepared buffer
has a 12-byte TMD header, an object beginning at `+12`, and its packet stream
at `+40`; the selected input object has 28-byte stride. These are output
extents, not evidence for a separate global packet buffer.

The [follow-up packet and caller ledger](game-render-packet-followup-25.md)
records the layout-identical typed header/color refinements and focused
regression checks after this survey.
