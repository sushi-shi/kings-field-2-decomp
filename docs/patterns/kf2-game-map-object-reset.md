# GAME map-object reset

GAME `0x80035590` is strict objdiff `100.000000000%` in
`game.map_object_reset`. Nine proven callers include map-object creation and
resource decoding. The leaf resets one `KfMapObject`'s flags and action,
clears three rotation components, initializes three scale components to
`0x1000`, and clears three other halfword fields. Its final halfword store
occupies the `jr $ra` delay slot. The typed `SVECTOR` rotation and scale
views preserve the established 0x44-byte object size and the +0x24/+0x2c
offsets.

The contiguous `0x800355d8` property setter is also a strict objdiff
`100.000000000%` match. Retail homes its variadic
arguments in the caller stack area, selects one of four property operations,
and indexes a 320-entry, 24-byte template table before updating a 0x44-byte
map-object record. The source reproduces the byte and halfword argument
loads using `va_arg` and the typed records. Its first comparison differed
only because the curated relocation inventory lacked the HI16/LO16 pair at
`0x800355fc/0x80035600` for `map_object_state.objects`; opcode and referent
review supplied that pair, after which focused and strict global listings
both became exact.

The adjacent `0x800356ac` map occupancy marker helper is WIP. It selects
between writing a caller marker and clearing scale, or writing `0xfe` and
restoring unit scale. Its branch gate is `player_state.unknown_6a` at
`0x8019853a`: the retail `lui 0x801a` followed by signed `lh -31430`
requires the carried low address. The helper indexes the startup-cleared
`bss_801c7540` region with an 800-byte row stride and 10-byte cell stride;
the first or second five-byte layer receives the marker. The typed prefix
models 88 rows by 80 cells, with that extent provisional. Reviewed
HI16/LO16 pairs at `0x800356b4`, `0x800356f4`, and `0x8003575c` fix the
target referents. Focused comparison has the same nine CFG blocks, four
branches, and return frontiers, with remaining address-arithmetic register
and instruction-order residue. No exact match is claimed.

The next contiguous pool pass, `0x800357a0`, is strict objdiff
`100.000000000%`. It visits all 396 objects, handling action `5` and
`0x51` through the property and cell-marker helpers. The typed template
fields at +0x0c/+0x17 supply their respective marker bytes; the object's
+0x3a halfword and high byte are overlapping views of one field. The
reviewed `0x800357c0/0x800357c4` HI16/LO16 pair identifies the object
pool base at `map_object_state+0x1e00`. Reversing the two independent loop
updates matched retail's delay-slot schedule without changing semantics.

The preceding `0x80035534` pool reset is a strict objdiff
`100.000000000%` match. It initializes all 396 objects'
IDs and actions to `0xff`, clears their three overlapping tail words, then
clears three halfword counters at `map_object_state+0x873e/+0x8740/+0x8742`.
Its `u16` countdown explains the `andi 0xffff` loop check and decrement in
the branch delay slot. Four reviewed HI16/LO16 pairs establish the object
tail base and final three counter referents.

GAME `0x80036190` is a humane WIP in `game.map_object`. It scans the
396-object pool from a caller-supplied index, tests ordinary templates by
distance, and tests kind-4 templates at a rotated forward point before
checking facing tolerance. Its six callers pass a position, added radius,
point height, angle, and tolerance; the source preserves those widths and
uses the shared object/template fields. Reviewed BSS and eight direct
control relocations establish the pool, template, calls, and internal jumps.
Focused comparison is about `82.9%` with 15/15 CFG blocks and 8/8 branches,
but the return frontier and some delay-slot scheduling differ. No exact
match is claimed.
