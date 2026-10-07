# Port technical notes

Addresses identify the original retail executables. Port corrections are not
matching claims and do not belong in `source` or `master`.

## Structure

| Path | Role |
| --- | --- |
| `src/{psx,open,game,end,lib}` | Original programs, as generated on `source` |
| `build.json` | Program membership and per-unit defines, shared with `source` |
| `cmake/program_unit.cpp.in` | Wrapper that compiles one original unit |
| `include/kf/psx/` | Port-owned declarations of the PlayStation library interface |
| `include/psx-sdk/` | The header names the original sources include |
| `src/runtime/` | Transitional implementation of that interface |
| `src/platform/` | Window, input, clock, entry point (from the King's Field port) |
| `src/renderer/` | GLES3/WebGL2 renderer (from the King's Field port) |
| `src/audio/` | Software SPU, sequencer and VAB/SEQ/ADPCM codecs (from the King's Field port) |

### One executable, four programs

CMake reads `build.json` and generates one wrapper per original unit. The
wrapper includes the library declarations at global scope, then the original
file inside `namespace kf_<program>`, under `#pragma clang section` names
`kf_<program>_data` / `kf_<program>_bss`. Program-local names such as
`display_initialize` (GAME, OPEN and END each have one) therefore stay distinct,
and the linker's `__start_`/`__stop_` symbols bound each program's globals.

`main` is renamed per program. PSX.EXE's original loop is the coordinator:
`Load` maps `cdrom:OPEN.EXE;1` and friends to a program, and `Exec` restores
that program's initialized data, clears its BSS and calls its `main`, which is
what loading the executable did. The request byte at `0x800102f0` is the
runtime-owned `kf_psx_overlay_request`.

### Library runtime

`src/runtime` implements the roughly 140 library calls the programs use, without Sony
headers or code. It is a bring-up seam, not the product: the King's Field port
replaced every library call with direct platform, renderer and audio calls, and
this port follows it subsystem by subsystem.

- **GPU**: ordering-table links hold runtime handles (24-bit indices into a
  registry), never truncated addresses. `DrawOTag` walks the table and converts
  polygon, rectangle and draw-mode packets to native faces in table order.
  The renderer's target is the whole 1024x512 frame buffer, so drawing
  environments, `LoadImage`/`StoreImage`/`MoveImage`/`ClearImage` and the
  display area share one coordinate space. Texture decoding reads a CPU mirror
  updated by image transfers; textures rendered into the frame buffer are not
  mirrored yet.
- **GTE**: library functions evaluated with the coprocessor's fixed-point
  stages (from the King's Field port): Q12 matrices, 16-bit saturation, the
  seeded reciprocal, depth cue and normal-color lighting. Transforms report
  the coprocessor FLAG bits: GAME sends every vertex whose flag is not exactly
  `0x1000` (only IR0 saturated) to the clipping path. `Clip3FTP`/`Clip4FTP`
  clip in camera space against the near plane and an `hw x vw` window at
  distance `h`; the library's exact clipping arithmetic is not reproduced.
  `catan` is a floating-point approximation.
- **CD-ROM**: sectors come from the user's BIN/CUE or ISO image; `CdSearchFile`
  walks ISO9660, so `.T` archives and movie streams use their real extents.
  Reads complete synchronously.
- **Events**: GAME's request queue runs from CD-ROM and root-counter events.
  Raising an event queues a delivery for each open, enabled match; deliveries
  run at vertical blank, when leaving a critical section and when an event is
  polled. Interrupt-mode handlers are called there; others are marked.
- **Memory card**: `bu00:NAME` is the file `NAME` in the card directory, with
  the retail block size. `format` keeps existing files.
- **Heap**: `InitHeap` gives each program a heap of its retail size
  (GAME `0x801f8000 - 0x801da018`, OPEN/END `0xf8000`).
- **Pads**: `PadRead` returns the documented digital-pad bits.
- **Sound**: `Ss*` calls drive the software SPU and sequencer from the King's
  Field port (`src/audio`). Bank, score and voice numbers follow the library,
  so `SsUtKeyOff` and `SpuGetAllKeysStatus` address the voice that
  `SsUtKeyOn` returned. Streamed bodies (`SsVabTransBodyPartly`) are collected
  until the bank is complete. Two format facts differ from King's Field data:
  King's Field II banks record a file size smaller than their sample table, so
  the table total sizes the body, and score tempo events (`FF 51`) carry three
  tempo bytes with no length byte.
- **Movies**: stream frames are assembled from the disc, but MDEC output is
  black.

## Portability edits to original sources

These are the only edits to generated files; keep them minimal and explicit.

| File | Edit |
| --- | --- |
| `src/{psx,open,end}/main.cpp`, `src/game/main.cpp` | Request byte `0x800102f0` becomes `kf_psx_overlay_request` |
| `src/{game,open,end}/main.cpp`, `include/kf/lib/overlay.h` | Heaps are runtime allocations with retail sizes |
| `src/game/cd_memory.cpp` | `memory_malloc_checked` tests for a null block, not a KSEG0 range |
| `src/game/resource_startup.cpp` | Region callback tables use the no-op table (see below) |
| `include/kf/game/{card,menu}.h` | `struct DIRENTRY` comes from the library header, not a forward declaration |
| `src/game/{floor_item_find_free,map_object}.cpp` | `va_arg` reads promoted `int` before narrowing |
| `src/game/{actor,effect}_runtime.cpp` | Two functions that fall off their end return a defined value |
| `src/game/resource_startup.cpp` | The weapon table copy stops at the table; retail writes 8 zero padding bytes into the map-cell array |
| `src/game/cd_memory.cpp` | `cd_stream_work_buffer` has a 0x1808-byte tail for the shape-bank copy that reads past it on retail |

## Memory adjacency

Retail code sometimes reads or writes across the end of one object into the
next, which was harmless in the original memory layout. The sanitizer preset
(`cmake --preset sanitize`) finds these: each is either bounded at its source
with a note above, or recorded here. Known remaining reports in a short run of
the first map, both caused by missing region modules or retail edge cases:

- `map_cell_add_layer_occupancy` indexes row `-1` of the map-cell grid near
  the map edge (inside the same retail object).
- `KF_MAP_OBJECT_OP_RECALL_SOCKET` reads map object `380 + 255` when its tail
  was not set up, which normally comes from a region callback.

## Region code modules

FDAT entry `3 * region + 2` is MIPS code linked to run at `0x8019e138` in
GAME's BSS, starting with a 32-entry callback table (for example region 0:
`0x8019e1c0`, ...). GAME calls through `resource_state.active_table` for map
object setup, events, actor behavior and magic. These modules are not part of
the four reconstructed executables. Until they are reconstructed as source,
the port installs `callback_default_table` (32 no-op callbacks), so
region-specific scripted behavior is missing.

## ILP32 requirement

`s32`/`u32` are `long`, library vectors use `long`, and about 600 static layout
checks include structures with pointers. The port therefore builds for i686
Linux (and wasm32, whose pointers are also 32-bit). Moving to LP64 requires
host representations at the resource and save boundaries first.

## Diagnostics

`KF_CAPTURE`, `KF_PAD_SCRIPT`, `KF_TRACE` and `KF_WATCHDOG` are described in the
README. Fatal signals print the faulting address and the return address at the
stack top, which identifies jumps through retail code pointers.
