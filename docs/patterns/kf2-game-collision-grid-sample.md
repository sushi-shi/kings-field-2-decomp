# GAME collision grid sample and fallback cell

`func_8002a988` takes X, Y, and Z. The third argument was initially hidden
at its two collision-wrapper call sites because `$a2` survived unchanged
across those calls. Declaring and passing Z explicitly made the adjacent
`0x8002b604` wrapper strict 100%, while preserving the existing exact
`0x8002b7f8` wrapper.

The sampler bounds unsigned X/Z to 80 cells of 2048 units, selects one of two
five-byte map-cell layers by comparing their elevation bytes with the negated
Y/128 height, writes the selected layer and scaled elevation to the shared
collision cache, then returns the cached height. Outside the grid it points
the cache at `collision_default_cell` at `0x800667fc` and clears those fields.
The ten fallback bytes form two `KfMapOccupancyLayer` records, each
`{0xff, 0, 0, 0x3f, 0}`. Their source-owned `DATA` claim matches retail
10/10 bytes; the surrounding unclassified span was split narrowly.

The cache resides inside the complete startup-cleared `bss_801c7540` claim.
`include/kf/game/collision_cache.h` shares temporary interior accessors with
the collision callers and audio listener code, without declaring an
overlapping global while the equipment/cache boundary remains uncertain.

The sampler is strict exact at 100% (284/284 bytes). Retail places the two
layer-selection blocks before the alternate elevation comparison, which
branches backward into either shared block. Expressing those shared paths
with labels preserves the decoded CFG and produces an identical focused
listing. The subsequent strict GAME report confirms the match while the
source-owned fallback cell remains byte-exact at 10/10. The cache/equipment
boundary is still provisional; this function's exact code does not prove a
separate cache allocation.
