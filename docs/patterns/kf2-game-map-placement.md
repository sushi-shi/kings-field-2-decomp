# GAME map placement expansion

GAME `0x80034818` is strict objdiff `100.000000000%` in
`game.map_placed_expand`. The map loader at `0x80016820` passes a section's
16-byte rows to it; it writes 128 24-byte rows at
`game_graphics_runtime+0x170f0`. An input ID of `0xffff` leaves the row
inactive. Active rows combine region bytes and local halfwords into X/Z
coordinates, use `func_8002b67c` to obtain the map height, add the input
height offset, and initialize a frame index from `rand()` and the input frame
count. The renderer at `0x8003247c` later increments that index when its global
counter reaches the row's frame period and wraps it at the frame count.

The source keeps the function's address-derived identity because the placed
row's later consumers have not established a semantic name. The complete
16-byte and 24-byte layouts have size and offset checks in
`include/kf/game/map_placed.h`; opaque fields remain opaque. Direct calls at
`0x800348c8` and `0x800348dc` and the internal jump at `0x80034900` were
reviewed against their decoded targets before accepting the strict match.

The renderer passes each runtime row's `VECTOR position` to `0x80032040`.
That helper is also strict objdiff `100.000000000%`: it shifts X and Z by
11, adds the signed grid origins at `game_graphics_runtime+0x14e14/+0x14e18`,
returns zero outside the unsigned `0..23` range, and otherwise reads a byte
from the 24-by-24 layer-mask grid at `+0x14e28`. The renderer ANDs that byte
with the row's layer, which supports the field name without assuming a more
specific map-cell meaning.

The adjacent `0x800320b0` takes a world position and a radius. Retail walks
the same grid over a `(2 * radius + 1)` square, ORing in-bounds cells and
returning the low byte. Its source has the same 10 CFG blocks, six branches,
grid referents, and return behavior, but its instruction listing differs from
the pinned compiler probe, so it remains WIP. The first difference is the
initial radius/mask register assignment, followed by a different order for
loading the X and Z origins; there is no evidence yet for attributing this
residue to a particular compiler mechanism.

The next contiguous helper, `0x80032174`, checks whether the view cell lies
within the X and Z radii of a world position. The renderer's caller passes
two unsigned byte radii from its map row. Its source has the same five CFG
blocks, three branches, and both view-cell data referents, but its instruction
register choices differ, so this is also WIP.
