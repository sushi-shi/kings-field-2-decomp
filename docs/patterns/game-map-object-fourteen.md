# GAME map-object placement and motion: fourteen-function verdict

This focused campaign follows map-object reset through placement, scatter,
world vertices, and motion. Retail disassembly, CFG, callers, data referents,
and the KF1 map-object pool were checked before comparing freshly rebuilt
GAME units. KF1's placement format and grid differ, so it supplied no direct
source spelling for the then-open KF2 functions.

| GAME address | Function | Direct strict verdict |
| --- | --- | --- |
| `0x80035504` | `map_object_play_spatial_sound` | exact, 100% |
| `0x80035534` | `map_object_pool_reset` | exact, 100% |
| `0x80035590` | `map_object_reset` | exact, 100% |
| `0x800355d8` | `map_object_set_property` | exact, 100% |
| `0x800356ac` | `map_object_set_cell_marker` | exact, 100% |
| `0x800357a0` | `func_800357a0` | exact, 100% |
| `0x80035894` | `func_80035894` | exact, 100% (fresh recheck) |
| `0x800365d8` | `func_800365d8` | exact, 100% |
| `0x800366fc` | `func_800366fc` | exact, 100% |
| `0x800368b4` | `func_800368b4` | exact, 100% |
| `0x80036944` | `func_80036944` | exact, 100% |
| `0x800369b8` | `func_800369b8` | exact, 100% |
| `0x80036ad8` | `func_80036ad8` | exact, 100% |
| `0x80036b68` | `func_80036b68` | exact, 100% |

The five targeted units now have **14/14 exact** functions. Their initialized
data also match: the placement unit has 270/270 `.data` and 1,016/1,016
`.rodata` bytes, and the scatter unit has 84/84 `.rodata` bytes. The
separate `map_object_state` definition emits COMMON size `0x8748` against
the retail `0x8744` BSS extent; this placement residue is not counted as
an exact data match.

The following `0x80035894` account describes the earlier 97.758415% source
probe; its WIP conclusion is superseded by the fresh exact certificate below.
At `0x800356ac`, retail forms a row pointer from the Z coordinate before
adding the X cell index in both marker branches. Two scoped typed row-pointer
locals reproduce that arithmetic and address schedule. A fresh isolated
strict comparison confirms the function and all five reset-unit siblings at
100%; the focused unit has six identical listings. At `0x80035894`, the first
listing difference is a schedule of independent
stores after template collision flags are loaded: retail stores the
collision flag before the `unknown_05` and `unknown_10` initialization,
while the compiler advances those two stores. Typed row pointers for the
initial `map_cells[region_z][region_x]` lookup and the kind-`0x59` lookup
follow the two retail row-major address calculations. The kind-`0x59` row,
cell, and layer pointers have their own scope because those values are used
only in that switch arm. The raw row multiplier is 800 bytes (80 cells of
10 bytes), followed by the ten-byte column offset at both sites. Focused
similarity improves from 92.7% to 98.7%;
fresh isolated strict similarity improves from 95.312874% to 97.758415% over
the 2,020-byte body.
The focused CFG retains 64/64 blocks, 19/19 branches, and one return frontier;
its jump-table dispatch remains an unresolved indirect jump in both objects.
The 270-byte `.data` and 1,016-byte `.rodata` sections remain strict exact.
All 54 `.text` and 254 `.rodata` relocation sites, types, and referents agree
in order. Reusing the first row local for kind `0x59` regressed the focused
listing, so that intermediate probe was reverted in favor of separate typed
locals. The remaining differences are early independent store/argument-setup
ordering and one temporary register; they lack a supported source edit.

The two scoped row-pointer source edits are retained; no retail-model edit was made.
That earlier comparison used only targeted unit rebuilds, direct per-unit
strict objdiff, and focused `kf try` for the then-remaining WIP; repository
tests, lint, full linked builds, broad matching, and banking were not run.

A fresh focused compile and safe one-VA delink of `game.map_object_init_records`
now confirm `0x80035894` at **100% strict text**, 2,020/2,020 bytes. Its
`map_object_cell_patterns` datum is also **100%**, 270/270 bytes. The safe
carve accepted 362 relocations with none withheld. This verifies the existing
later correction; it is not a new closure or a change to this source.
