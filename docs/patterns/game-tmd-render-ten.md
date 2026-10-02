# GAME TMD and render ten

These ten currently non-exact GAME functions form a TMD packet, map-cell,
world-render, and sparse-animation call family. For each, retail disassembly,
CFG, callers/callees, data references, strings, adjacent claims, current
source/history, strict state, and vendored attribution were checked. None is a
supported vendored body. Historical source owners handed off the ten bodies;
no shared source, signature, identity, or relocation edit was retained.

| GAME address | Retail/source evidence | Final verdict |
| --- | --- | --- |
| `0x8002ddb4` | One of the prepared TMD packet walkers; uses typed primitive faces and projected vertices. | **WIP, 97.34716% strict**; target/compiled CFG 44/42 blocks and 28/26 branches. Eight exact siblings in `tmd_pipeline.c` remain exact. |
| `0x8002e4dc` | Paired packet walker with the same TMD/ordering-table family. | **WIP, 96.587975% strict**; 44/42 blocks and 28/26 branches, with return branches displaced by the packet layout. |
| `0x8002ebe0` | Blended packet walker with a full-width `(u32)blend_mode << 5` tpage contribution, matching retail's word store. | **WIP, 95.27945% strict**; 26/25 blocks and 17/16 branches remain after the proven blend-width correction. |
| `0x80030de4` | Reads both layers of typed 80-column map occupancy cells, computes view-relative positions, and calls the exact map-cell object renderer. | **WIP, 89.52128% strict**; 10/10 CFG blocks agree, while the lower-layer elevation load and argument schedule differ. Exact sibling preserved. |
| `0x800311b0` | Builds a textured `POLY_FT4` packet, conditionally marks semitransparency, then adds it at a valid OT depth. | **WIP, 92.14815% strict**; 8/8 CFG blocks agree, but the retail packet-code store occupies a branch delay slot and uses a different saved-register assignment. Equivalent unconditional SDK semitrans expression worsened the listing and was discarded. |
| `0x80031850` | World-model renderer traverses TMD objects and graphics state; its call to `0x8002ddb4` uses the corrected third argument. | **WIP, 84.71045% strict**; 40/40 CFG blocks agree, with graphics-base and address evaluation order still different. |
| `0x80031d8c` | Animated-object renderer installs rotation, lighting, and projected vertices, then passes render data to `0x8002ebe0`. | **WIP, 94.65414% strict**; 7/7 CFG blocks agree, but retail retains the final render-data argument in `s7` while the probe reloads it. An equivalent local pointer alias expanded the frame and was discarded. |
| `0x80033d3c` | Sparse vertex delta blender flushes groups through `ScaleMatrix`; two neighboring sparse helpers are exact. | **WIP, 95.21839% strict**; 17/17 CFG blocks agree, but retail's skip-with-no-pending path reaches a shared far tail. A source-only label moved the branch there but remained one instruction off, so it was discarded. |
| `0x80034f90` | Applies a rotated cell pattern using the typed occupancy grid, with per-layer object, elevation, and light bytes. | **WIP, 97.86822% strict**; 14/14 CFG blocks agree, while world-origin and layer-flag saved registers are exchanged. |
| `0x80035194` | Copies selected occupancy fields across a rotated rectangle, including both layers and four quarter-turn cases. | **WIP, 89.59545% strict**; 51/51 CFG blocks agree, while retail uses a 40-byte frame against the probe's 32-byte frame and assigns several loop registers differently. |

Fresh `kf match --image game` relinked 147/147 target units and confirmed all
ten scores. Global edge-check still stops on the three known, unrelated
TMD/map-object `.rodata` addends. No repository tests, bank, or commit were run.

The sparse-animation row above is historical. A fresh safe three-VA GAME
delink and focused compile now give **100% strict** text for all three
`game.animation_sparse_vertices` functions: `0x80033bfc` 196/196 bytes,
`0x80033cc0` 124/124 bytes, and `0x80033d3c` 696/696 bytes. The seven
ordered module relocations match, including all three `ScaleMatrix` calls;
the safe carve withheld none. The current source needs no change.
