# GAME map-object placement and motion: fourteen-function verdict

This focused campaign follows map-object reset through placement, scatter,
world vertices, and motion. Retail disassembly, CFG, callers, data referents,
and the KF1 map-object pool were checked before comparing freshly rebuilt
GAME units. KF1's placement format and grid differ, so it supplies no direct
source spelling for the two KF2 WIPs.

| GAME address | Function | Direct strict verdict |
| --- | --- | --- |
| `0x80035504` | `map_object_play_spatial_sound` | exact, 100% |
| `0x80035534` | `map_object_pool_reset` | exact, 100% |
| `0x80035590` | `map_object_reset` | exact, 100% |
| `0x800355d8` | `map_object_set_property` | exact, 100% |
| `0x800356ac` | `map_object_set_cell_marker` | WIP, 88.68852% |
| `0x800357a0` | `func_800357a0` | exact, 100% |
| `0x80035894` | `func_80035894` | WIP, 95.312874% |
| `0x800365d8` | `func_800365d8` | exact, 100% |
| `0x800366fc` | `func_800366fc` | exact, 100% |
| `0x800368b4` | `func_800368b4` | exact, 100% |
| `0x80036944` | `func_80036944` | exact, 100% |
| `0x800369b8` | `func_800369b8` | exact, 100% |
| `0x80036ad8` | `func_80036ad8` | exact, 100% |
| `0x80036b68` | `func_80036b68` | exact, 100% |

The five targeted units have **12/14 exact** functions. Their initialized
data also match: the placement unit has 270/270 `.data` and 1,016/1,016
`.rodata` bytes, and the scatter unit has 84/84 `.rodata` bytes. The
separate `map_object_state` definition emits COMMON size `0x8748` against
the retail `0x8744` BSS extent; this placement residue is not counted as
an exact data match.

At `0x800356ac`, both branches perform the supported grid and marker
operations, but retail and candidate choose different arithmetic registers
and base-addition order. Its five reset-unit siblings remain exact. At
`0x80035894`, the first listing difference is a schedule of independent
stores after template collision flags are loaded: retail stores the
collision flag before the `unknown_05` and `unknown_10` initialization,
while the compiler advances those two stores. The 64-block placement
controller's switch table and initialized bytes are exact; later occupancy
index and temporary-register differences remain. Neither store scheduling
nor the different KF1 placement structure proves a source correction.

No source or retail-model edit was retained. Verification used only targeted
unit rebuilds, direct per-unit strict objdiff, and focused `kf try` for the
two WIPs; repository tests, lint, full linked builds, broad matching, and
banking were not run.
