# GAME collision and floor-item ten

These ten previously non-exact GAME functions form a map-height and collision-mask
run, followed by the floor-item image capture that consumes the same rendering
state. Retail disassembly, CFG, callers, callees, address/data references,
strings, current source, match state, and the Psy-Q vendored inventory were
checked for each. None has a supported vendored attribution.

| GAME address | Retail evidence | Final verdict |
| --- | --- | --- |
| `0x8002b67c` | Selects one of two 10-byte map-cell elevation layers, writes the collision cache, and calls the five-argument collision dispatcher. | **WIP, 94.895836% strict**; the compiled form keeps the cached height live rather than reloading it for the call. |
| `0x8002b73c` | Adds a scaled value across a bounded rectangle of 80-column map cells. | **WIP, 98.40426% strict**; 8/8 CFG blocks and 4/4 branches agree, with Z-coordinate register scheduling left over. |
| `0x8002b874` | Copies a player, actor, or map-object position and stores its radius and interaction height. | **WIP, 93.52273% strict**; branch-local stores now follow retail order, while actor/template load scheduling and one CFG join differ. |
| `0x8002b9d4` | Dispatches grid, actor, object, and player collision channels according to mode flags. | **WIP, 89.4% strict**; call set and 23/23 CFG blocks agree, but saved-register/argument scheduling and some branch joins differ. |
| `0x8002bfd4` | Rasterizes a line into the 24-by-24 collision layer-mask grid. | **WIP, 54.15534% strict**; the first-pass C still differs in the two raster-loop shapes and branch schedule. |
| `0x8002c170` | Scans a row for a contiguous run of one mask value, stepping in either direction. | **WIP, 89.8% strict**; 11/11 CFG blocks and 5/5 branches agree, but the compiler reuses the input row register for its running pointer. |
| `0x8002c290` | Updates one layer-mask byte using the two occupancy layers and scan cursor state. | **WIP, 71.32674% strict**; 15/15 CFG blocks and 10/10 branches, but return edges and layer tests differ. |
| `0x8002c424` | Walks map and mask cursors over a line, calling the per-cell update path from the larger mask builder. | **WIP, 55.789116% strict**; source control joins and cursor updates remain unlike retail. |
| `0x8002c670` | Builds the collision mask using `rcos`/`rsin`, one occupancy sample, four line draws, then eight scan/update pairs. | **WIP, unclaimed**; the 0x7bc-byte body depends on the unresolved complete mask-scan state and `DAT_80067874` owner. |
| `0x8002ce68` | Takes a free floor item, writes its byte fields and image rectangle, conditionally allocates pixel storage, then calls `StoreImage` and `DrawSync`. | **WIP, 65.85185% strict**; the compiled form hoists three stack arguments across the free-slot call, inflating its frame from 40 to 56 bytes. |

For `0x8002b874`, retail stores the interaction height in each selected
branch. Expressing those stores directly improved its focused listing from
85.8% to 92.4% and strict score from 91.5% to 93.52273%. The remaining
load/store order is not justified by a different object layout or a volatile
qualifier, so no artificial codegen carrier was added. A source-order probe
for `0x8002b73c` made the listing worse and was discarded. The unclaimed
`0x8002c670` body was not forced through an incomplete scan-state model.

The strict GAME pass relinked 146/146 target units. The nine exact functions
adjacent to the collision WIPs, both exact floor-item neighbors, and the
3,520/3,520 initialized collision-default bytes remain exact. Global
edge-check still reports the three existing unrelated TMD/map-object
`.rodata` addend mismatches. No repository tests, bank, or commit were run.
