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
| `0x8002b874` | Copies a player, actor, or map-object position and stores its radius and interaction height. | **WIP, 91.5% fresh direct strict**; actor/template load scheduling and one CFG join differ. |
| `0x8002b9d4` | Dispatches grid, actor, object, and player collision channels according to mode flags. | **WIP, 95.37931% fresh direct strict**; call set and 23/23 CFG blocks agree, but saved-register/argument scheduling differs. |
| `0x8002bfd4` | Rasterizes a line into the 24-by-24 collision layer-mask grid. | **WIP, 73.91262% fresh direct strict**; its two raster loops still differ in instruction schedule. |
| `0x8002c170` | Scans a row for a contiguous run of one mask value, stepping in either direction. | **Exact, 100% fresh direct strict**; the earlier WIP row was stale. |
| `0x8002c290` | Updates one layer-mask byte using the two occupancy layers and scan cursor state. | **Exact, 100% fresh direct strict**; the earlier WIP row was stale. |
| `0x8002c424` | Walks map and mask cursors over a line, calling the per-cell update path from the larger mask builder. | **WIP, 98.29932% fresh direct strict**; its remaining controls still differ. |
| `0x8002c670` | Builds the collision mask using `rcos`/`rsin`, one occupancy sample, four line draws, then eight scan/update pairs. | **WIP, unclaimed**; the 0x7bc-byte body depends on the unresolved complete mask-scan state and `DAT_80067874` owner. |
| `0x8002ce68` | Takes a free floor item, writes its byte fields and image rectangle, conditionally allocates pixel storage, then calls `StoreImage` and `DrawSync`. | **WIP, 65.85185% strict**; the compiled form hoists three stack arguments across the free-slot call, inflating its frame from 40 to 56 bytes. |

For `0x8002b874`, retail stores the radius in each selected branch and
passes the interaction height to one common halfword store at `+0x154`.
A fresh focused rebuild of the current source is 85.8% with 8 retail versus
9 compiled CFG blocks; isolated strict text is 91.5%. The earlier 93.52273%
note does not describe the current source object. The remaining load/store
order is not justified by a different object layout or a volatile qualifier,
so no artificial codegen carrier was added. A source-order probe
for `0x8002b73c` made the listing worse and was discarded. The unclaimed
`0x8002c670` body was not forced through an incomplete scan-state model.

The strict GAME pass relinked 146/146 target units. The nine exact functions
adjacent to the collision WIPs, both exact floor-item neighbors, and the
3,520/3,520 initialized collision-default bytes remain exact. Global
edge-check still reports the three existing unrelated TMD/map-object
`.rodata` addend mismatches. No repository tests, bank, or commit were run.

### Fresh floor-item capture check

A targeted `game.floor_item_find_free` rebuild and direct one-unit objdiff
confirm `0x8002ce2c` and `0x8002cf40` **strict exact**. `0x8002ce68`
remains **65.85185%**: retail has a 40-byte frame, saves `s0`–`s4`, and
loads the three stack arguments only after the free-item call and null
check. Current source emits a 56-byte frame, saves `s0`–`s8`, and hoists
those arguments before the call. Both versions use the same four proven
calls, the same five incoming main-loop calls, five retail CFG blocks, and
the same field widths: `lbu` for the kind byte, `lw` for the full-width kind
comparison, `lw` for width, and `lhu` for height. The two exact siblings
remain focused `SAME`.

An off-tree variant calculating allocation bytes before storing height
changed the late multiply/store schedule but reduced focused similarity
from 55.7% to 52.0%; an equivalent early-return spelling emitted the
same baseline listing. Both were discarded. Narrowing the fifth parameter
to `u8` is unsupported: retail compares its full 32-bit stack value with
one after storing its low byte in the item. No stack carrier or unrelated
local was added to force deferred argument loads.

An isolated unchanged-source compiler check did not recover those late
loads. GCC 2.5.7 `-O2 -fno-cse-skip-blocks` and a K&R-style seven-argument
definition emitted the same 65.85185% body; `-fno-schedule-insns` and `-O1`
reached only 66.03704% and broke the exact update sibling. GCC 2.6.0 `-O2`
fell to 33.203705% and also broke that sibling. The configured profile and
typed source remain in place.

### Rasterizer raw/frame recheck

A fresh focused build and isolated strict comparison retain `0x8002bfd4` at
**73.91262%**. Retail is frameless; the candidate reserves 16 bytes but
never accesses the stack. Both source and retail load the two unsigned
halfword grid origins, rasterize along the dominant X or Z axis, test both
24-cell bounds, and write the same byte. The first instruction-selection
difference is the candidate's algebraic cancellation of the origin while
computing the endpoint delta; retail adds the origin to each endpoint before
subtracting. No distinct referent or call is absent.

Two off-tree full-unit controls changed the origin locals from `u16` to
`s32`, and replaced the low-halfword pointer read with a well-defined
`(u16)` conversion of the owning word. Each emitted a byte-identical unit
object and kept the unused 16-byte candidate frame. The conversion is
retained because it expresses the unsigned low-halfword value without a
pointer alias; a fresh tracked-source focused build has the same whole-unit
SHA256 as baseline, preserving every exact sibling. The full-width local
trial and compiler profile remain unchanged.
