# GAME TMD and map packet pipeline: 25 strict verdicts

This campaign connects the TMD selectors and primitive preparation to the
packet walkers, graphics-quad helpers, and map-cell pattern placement. Each
listed source unit was compiled in isolation with its manifest profile and
compared directly with its safe retail object. Focused `kf try` listings were
also checked for the six WIPs. The 19 exact controls remain 100%. One
source-backed loop-order correction was retained in the rectangle copier;
no identity change followed.

| GAME address | Function | Strict text | Final verdict |
| --- | --- | ---: | --- |
| `0x8002d458` | `tmd_select` | 100% | Exact selector control. |
| `0x8002d484` | `tmd_get_object` | 100% | Exact object getter and walker callee. |
| `0x8002d4a8` | `tmd_set_current_vertices` | 100% | Exact projected-vertex setter. |
| `0x8002d4b8` | `tmd_select_object_vertices` | 100% | Exact object-vertex selector. |
| `0x8002d5dc` | `tmd_prepare_primitive_indices` | 100% | Exact packet-index preparation. |
| `0x8002d8b0` | `tmd_register` | 100% | Exact TMD registry control. |
| `0x8002d8f0` | `tmd_set_slot` | 100% | Exact slot setter. |
| `0x8002d910` | `tmd_release_slot` | 100% | Exact slot release. |
| `0x8002d918` | `func_8002d918` | 100% | Exact TMD state helper. |
| `0x8002da94` | `func_8002da94` | 100% | Exact TMD state helper. |
| `0x8002dbd8` | `tmd_transform_vertices` | 100% | Exact vertex-transform control. |
| `0x8002dc80` | `tmd_transform_vertices_depth` | 100% | Exact depth-transform control. |
| `0x8002dd28` | `tmd_project_vertices` | 100% | Exact projection control. |
| `0x8002ddb4` | `func_8002ddb4` | 97.434494% | WIP: 44/44 CFG blocks and 28/28 branches; the two positive-depth exits and shared ordering-table tail schedule differ. |
| `0x8002e4dc` | `func_8002e4dc` | 97.383070% | WIP: 44/44 blocks and 28/28 branches; paired packet modes retain the same depth-tail layout residue. |
| `0x8002ebe0` | `func_8002ebe0` | 95.279450% | WIP: 26/25 blocks and 17/16 branches; retail tests signed fixed depth at the shared packet tail while the candidate precomputes an equivalent range condition. |
| `0x800311b0` | `func_800311b0` | 92.148150% | WIP: 8/8 blocks and 5/5 branches; saved-register/frame and packet-code delay-slot placement differ. |
| `0x800312f4` | `func_800312f4` | 100% | Exact sliding-panel quad caller. |
| `0x80031384` | `func_80031384` | 100% | Exact paired quad caller. |
| `0x80031414` | `func_80031414` | 100% | Exact color-byte quad caller. |
| `0x800314d4` | `func_800314d4` | 100% | Exact color-byte setter. |
| `0x800314fc` | `func_800314fc` | 100% | Exact collision-channel graphics helper. |
| `0x80031634` | `func_80031634` | 100% | Exact paired channel helper. |
| `0x80034f90` | `func_80034f90` | 97.868220% | WIP: 14/14 blocks and 7/7 branches; world-origin and layer-flag register allocation differs. |
| `0x80035194` | `func_80035194` | 91.672730% | WIP: 51/51 blocks and 26/26 branches; advancing the row cursors before the inner loop follows retail's early next-row calculation. The 40-byte retail versus 32-byte candidate frame remains. |

The TMD pipeline has eight exact siblings beside its three walkers. Their
ordered external call and referent sets remain aligned. For the fixed-depth
walker, a previously documented nested-guard probe restored the missing CFG
block but changed frame and instruction order without an independent source
fact; this pass did not retain it. The map-cell pair preserves rotated
coordinates, quarter-turn rectangle steps, 80-column rows, layer selection,
and field masks seen in retail. In `0x80035194`, retail calculates the next
source row (`+0x320` bytes) before entering the inner cell loop. The former C
advanced both row cursors after that loop; taking the current row pointers
first, then advancing their cursors before the inner loop, preserves all
values and moves this calculation into the retail order. Isolated strict
text rises from **89.595450%** to **91.672730%** and the candidate body grows
from 872 to 876 bytes against retail's 880. CFG remains 51/51 blocks and
26/26 branches; all 11 ordered relocation rows retain their exact offsets,
kinds, and targets. The adjacent `0x80034f90` stays **97.868220%**. Focused
listing similarity moves from 58.8% to 58.3%, so that display metric does not
capture the raw order and strict improvement. The remaining frame and loop
register differences have no proved extra source object. All six WIPs remain
open, with exact controls protected.

An off-tree cached `layer_select & 1` local reproduced one retail mask
calculation before the inner loop and gave the candidate a 40-byte frame,
but lowered strict text from **91.672730%** to **90.418180%**, saved an extra
`$s8`, and grew its body from 876 to 884 bytes against retail's 880. The
source has no independent use that requires this cached Boolean, so the
trial was discarded rather than kept as a register carrier.
