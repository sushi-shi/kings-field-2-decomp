# Native port plan

King's Field II gets a Linux and WebAssembly port in the same way King's Field
did: the reconstructed programs are the game, and only platform boundaries
change. This document records what the King's Field (KF1) port provides, what
differs in King's Field II (KF2), the milestones and the branch layout. The
bootstrap lives on `port-bootstrap`. Its operational notes are in that branch's
`docs/port-notes.md`.

## 1. How the KF1 port works

KF1's `port` branch (about 30,000 lines of C++20) is a direct source port. The
original sources run the game: movement, AI, combat, menus, scripts and
progression. Neither a second gameplay runtime nor a replacement Psy-Q SDK is
the product. No library call remains. The game calls a small `kf::` interface
instead.

| Subsystem | KF1 implementation |
| --- | --- |
| Window, events, timing | `src/platform/host.cpp`: SDL3, GLES 3.0 / WebGL2 context, one 60 Hz host clock with absolute deadlines, focus pause, `UpdatePacer` for 20 Hz gameplay and 22 Hz cutscenes. WASM keeps the original blocking loops through Asyncify. |
| Input | `controls.cpp` maps keyboard, mouse and gamepad controls to actions. `host_read_buttons()` turns actions into the pad bits that the game reads. Mouse look feeds view angles. |
| Files | The launcher imports the user's ISO or BIN/CUE (`assets.cpp`, `disc.cpp`) into a verified file tree with 428 files and a fixed SHA-256. The game reads files by path (`files.cpp`). The browser caches the tree in IndexedDB. |
| Rendering | GLES3/WebGL2, not a software rasterizer. Producers submit whole `DrawFace`s. The renderer sorts them by depth with LIFO order for equal depths (as the ordering table did), splits quads after sorting, and rejects oversized triangles. It draws into a 320x240 target. Shaders reproduce RGB5 output, dithering, raw and modulated texturing and the four blend modes. A CPU texel store mirrors VRAM for TIM uploads. GTE work is explicit CPU arithmetic: the seeded reciprocal projection, depth cue and lighting with the original saturation points. |
| Audio | A software SPU and sequencer feed an SDL audio stream: 24 voices, ADSR, ADPCM, VAB/SEQ decoding, and a substitute reverb. Audio is rendered on the main thread up to the host clock. |
| Saves | Versioned little-endian save files with three slots, written by temp file, fsync and rename. The browser saves through strict IndexedDB transactions. No memory-card emulation. |
| Programs | One executable. `main()` runs the intro cutscene, then loops `game_play()` and the cutscene (intro or ending). Module state is reset with `restore_initial_value<global>()`. Colliding names were renamed by hand (`cutscene_*`). |
| Build | CMake presets `linux`, `sanitize` and `wasm`. The Nix flake provides the package, the app (`KF_DISC`), a NixOS module and a dev shell with emscripten. `web/shell.html` handles disc import and storage. |

**Reusable unchanged:** `renderer/*` (except the Image/hash coupling through
`platform/assets.h`), `audio/{sound,codec}.cpp`, `platform/{controls,files}`,
`module_state.h` and fixed-point math (`lib/fixed_math`, `random`, `memory`).

**Reusable with game constants changed:** `host.cpp` (title, bindings, pad bit
meanings), `disc.cpp`/`assets.cpp` (volume ID `SLPS-00017`, the file count and
hashes), `saves.cpp` (names and slot policy), `entry.cpp` (the coordinator),
`web/shell.html` and the flake.

**KF1-specific:** everything under `src/game`, `src/cutscene` and the
resource-decoding parts of `src/lib`, plus the English translation pipeline.

**History:** KF1's `port` diverged from a September `source` snapshot with one
large commit ("run original game on Linux and WebAssembly", about 13,000 lines),
then dozens of review-driven cleanups. It is not regenerated. Fixes that may
apply to the reconstruction go through the reconstruction's own evidence rules.

## 2. Sharing code with KF1

**Recommendation: copy now, extract later.** Copy the KF1 platform, renderer
and audio files into KF2's `port` branch. Adapt them there, keep their history
references in commit messages, and do not change KF1 for KF2's sake. Once both
ports have stable interfaces (after KF2's audio and save milestones), extract
the game-agnostic part into a shared library: renderer, software SPU and
sequencer, VAB/SEQ/TIM codecs, controls, disc import, save storage and host
clock. Publish it as a flake input that both ports pin. Do not do that
extraction as part of the KF2 bring-up. The interfaces will move while KF2's
needs are discovered: a frame-buffer-sized target, the 640-wide title screen,
MDEC output and memory-card files.

The bootstrap already copies `controls`, the core of `host`, `renderer` and
`textures`, and the GTE arithmetic. The renderer was generalized from
the 320x240 target to the whole 1024x512 frame buffer, and that is the first
change a shared version must absorb.

## 3. Differences that matter in KF2

| Topic | KF2 situation | Consequence for the port |
| --- | --- | --- |
| Disc layout | 18 files. Seven `CD/COM/*.T` sector archives (`MO`, `TALK`, `VAB`, `FDAT`, `RTIM`, `RTMD`, `ITEM`). A `.T` header is a u16 count followed by u16 sector offsets. Each entry ends with a `0x12345678` plus word-sum checksum. OPEN and END read `OP/OP.D` and `OP/ED.D`. | The bootstrap serves sectors from the user's image and resolves paths through ISO9660. That keeps archive offsets, checksums and movie streams intact without a file-tree format. The KF1-style verified import (one file count and hash) replaces it at the web milestone. Direct boundaries later turn archive reads into bounded entry reads. |
| Programs | PSX.EXE runs OPEN, GAME and END in turn through `Load`/`Exec`. The only state passed between them is the request byte at `0x800102f0`. GAME, OPEN and END each define `display_initialize`, `display_begin_frame`, `audio_initialize` and others, and `src/lib` units are compiled for both OPEN and END. | Each unit is compiled inside `namespace kf_<program>` through a generated wrapper, so no names need renaming. Program-owned sections let `Exec` reload initialized data and clear BSS, which replaces KF1's per-global snapshots. PSX.EXE's loop stays the coordinator. |
| Region code modules | FDAT entry `3r+2` is MIPS code linked at `0x8019e138` with a 32-entry callback table. GAME calls it for map-object setup, event commands, actor behavior and magic. These modules are not part of the four reconstructed executables. | **New reconstruction scope.** The region modules (about 16, each 4-8 KiB) need source. Add them to `master` as further images with their fixed load address, then register their tables in the port. Until then the port uses the no-op callback table. |
| Memory card | `bu00:BISLPS-00069N` files of 2 blocks (`0x4000` bytes): a 0x280-byte header with a Shift-JIS title and three icon frames captured from VRAM, and a 0x3c00-byte payload with a byte-sum checksum. BIOS file calls and card events. | The bootstrap maps `bu00:` to a save directory with retail file sizes, so the game's own slot logic, summaries and checksums work unchanged. Later, as in KF1, replace card events with direct per-slot storage, durable writes and IndexedDB. Optional: export and import `.mcr`/`.mcd` cards. |
| Movies | `OP/OP.S` (10,890 sectors, 1,085 frames) and `OP/ED.S` (21,410 sectors, 2,136 frames). STR v2, 320x240, 16-bit, double speed. No XA audio: OP.D/ED.D SEQ music plays over them. | MDEC decoding (VLC, IDCT, YCbCr) is a self-contained runtime piece. The bootstrap assembles stream frames correctly but outputs black slices. |
| SDK | Psy-Q 3.0 (March 1995 runtime). KF1 used Release 2.5. | The port replaces the SDK, so the version matters only where behavior is reproduced: `LIBGTE` 3.0 adds the `Clip*` near-plane functions that GAME uses, `catan` and the `SquareRoot*` tables, and the `LIBSND` sequencer and `SsUtKeyOn` voice semantics. KF1's sine table (from 2.5 `GEO.OBJ`) is used. Its identity with 3.0 should be checked. |
| `-G8` small data | Nine units use the `-G8` profiles. | No effect on the port: the C++ build has no gp-relative data. It matters only for `classic` and matching. |
| Typed enums | Master defines domains with `KF_ENUM_*`. The C++ view (`source`) emits `enum class`, `KfEnumStorage` and `KfBoolStorage`. | The port compiles the C++ view unchanged, so enum domains stay type-checked on the host. The bootstrap found one generator gap: classic cleaning does not expand `KF_ENUM_VALUE`. That fix belongs to the export generator (lane H), not to the port. |
| Pointer width | `s32`/`u32` are `long`, library vectors use `long`, and about 600 static layout checks include structures with pointers. | The bootstrap targets ILP32: i686 Linux and wasm32. Moving to LP64 needs host representations at the resource, save and pool boundaries, as KF1 built, and comes after gameplay works. |

## 4. Milestones

| | Milestone | Status on `port-bootstrap` |
| --- | --- | --- |
| a | The export compiles natively as C++ against a platform layer, with SDK calls mapped | **Reached.** All 41 units (4 programs) compile for i686. 140 library calls link to `src/runtime`. `nix build .#unwrapped` passes. |
| b | Boots to the title | **Reached.** The PSX loop runs OPEN. OP.D loads through the CD path, and the TIM/ordering-table title renders at 640x240. The idle attract movie runs with black frames. |
| c | GAME loads the first map and renders | **Reached, with gaps.** Start enters GAME. FDAT/RTMD/RTIM/VAB archives load through the event-driven request queue. The shore map, HUD and compass render, and the player moves and turns. Missing: near-plane clipping (`Clip3FTP`/`Clip4FTP`) and region modules. |
| d | Input | Gameplay-aware bindings (KF1 action table: WASD, mouse look), menu context, and verification of KF2's button semantics. |
| e | Audio | Connect `Ss*`/`SsUtKeyOn`/`SpuGetAllKeysStatus` to KF1's SPU mixer and sequencer, including streamed VAB bodies (`SsVabTransBodyPartly`) and OP.D/ED.D music. |
| f | Saves | Exercise the card path through the original menus. Then direct slot storage, durability and IndexedDB. |
| g | Web | emscripten build (wasm32 is ILP32 too), Asyncify yields in `VSync`, the disc import page and caching from KF1's `web/shell.html`. |
| h | Movies | Software MDEC for OP.S/ED.S. |
| i | Region modules | Reconstruct the FDAT callback modules on `master` and register them in the port. |
| j | Direct boundaries | Retire `src/runtime` subsystem by subsystem: producers enqueue faces directly, resources are read by entry, audio is called directly. Then the LP64 build. |

Each milestone ends with a run against the retail disc: the bootstrap's
`KF_PAD_SCRIPT`, `KF_CAPTURE` and offscreen SDL make runs repeatable without a
keyboard. User play-testing remains the gameplay acceptance check, as in KF1.

## 5. Branches

```text
              master
                 |
     +-----------+-----------+
     |                       |
     v                       v
  source                  classic
     |
     v
   port
```

- `master`: reconstruction. Only this plan and generator fixes land here.
  Platform changes never do.
- `source`: a single generated root commit from `kf clean --publish source`.
  Regenerated, never edited. Push with an explicit `--force-with-lease`.
- `port`: owns all platform changes, with history on top of a `source` root.
  After each regeneration, rebase with
  `git rebase --onto <new source> <old source> port`.
  Port edits to generated files are few, marked, and listed in
  `docs/port-notes.md` so conflicts stay small.

The bootstrap branch is `port-bootstrap`. Its root is a `source` snapshot
generated locally from master `d886a3f` plus a one-line generator workaround
(`KF_ENUM_VALUE` in the classic view). When the build-hygiene fix lands and
`source` is published from master, rebase the bootstrap onto it and rename it
to `port`.
