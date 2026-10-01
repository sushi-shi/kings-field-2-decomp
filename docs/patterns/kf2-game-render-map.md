# GAME map packet emitter

GAME `0x8002f194` is a 0x41c-byte map TMD emitter. Its only proven caller,
`0x80030c18`, selects an object and passes its unsigned-halfword index after
setting geometry matrices. The emitter gets that object, projects its vertices,
and walks its prepared primitive packets. Packet mode `0x2c` emits a 52-byte
GT4; mode `0x24` emits a 40-byte GT3. Other modes advance without emission.
Clipping rejection advances to the next packet; primitive-buffer overflow
returns immediately. A signed vertex-depth average plus 240 selects a slot
only when the unsigned result is below 8192. The packet's GPU tag length is
written as one byte, retaining the existing tag link. NormalClip,
NormalColorCol, DpqColor and AddPrim are separately attributed Psy-Q calls.

The initialized four-byte color at `0x8006d6d0` is `{128,128,128,0}`. It is
used by this emitter and the neighboring TMD emitters, and ends before the
separately written word at `0x8006d6d4`. The defining TU is provisional;
`game.render_map` owns the data claim while other emitters declare it extern.
The `KfScreenVertex` first word is a packed GTE XY pair, and the prepared
halfword indices are byte offsets into the projected array. FT packet views
and SDK GT packet views are checked for their consumed sizes.

The KF1 GAME emitter supplied a source-shape comparison, not an identity
assumption. KF2's retail body is four bytes longer. The focused probe reached
the same CFG and call/relocation order before its last five differences, which
exchanged the real packet-header and normal-stream stack slots. Declaring the
header before the normal pointer reproduced retail's lifetime and stack
schedule. Strict objdiff now reports 1052/1052 code bytes and 4/4 data bytes
matching; the GAME target relink verifies the unit. Global `kf match` still
fails its separate known-reference ownership closure on unclaimed data and a
cross-owner relocation pair. No repository tests were run, per user request.

## Map-cell object caller

GAME `0x80030c18` accepts one five-byte cell layer, an SVECTOR position, and
word-sized render flags. Two direct calls in `0x80030de4` pass the first and
second layers of an existing ten-byte grid cell. Bytes +0/+1/+2/+4 select the
object, elevation, quarter-turn orientation, and collision lighting row. This
layer type is shared with the occupancy writer and map-object marker setter;
they use the same +0 and +2 bytes. The renderer transforms the position
through the view matrix, selects a lighting matrix from an 80-row collision
table, sets fog/back-color from the row, checks the transition-state guard,
and routes the object to the map emitter or an alternate prepared-TMD path.
That path uses a real 4096-byte stack TMD asset; its header and first object
record are typed, while payload capacity is inferred only from the frame.

The current source has the same 11-block/6-branch CFG counts, return-frontier
count, and direct call set as retail. Strict objdiff is 96.521736%, while its
focused listing is 98.7% similar:
four independent entry setup instructions differ in order before
SetRotMatrix. No source-only carrier or padding is retained. This function
remains WIP until strict objdiff is 100%. Two signed-low BSS address pairs
for transition state +0/+0x16 were curated after decoding their `lui`/`lh`
and `lui`/`lbu` instructions, both pointing into state_8017d118.

## Clipped map polygon helper

GAME `0x8002f5b0` is the sole direct helper of the alternate textured map
emitter at `0x8002f808`. The latter passes the clipped vertex count, normal,
CLUT, texture page, packet mode bit, and depth bias. The helper rejects
non-positive winding, shades the first two clipped vertices, then emits a GT3
for each triangle in the fan. It advances the primitive cursor by 40 bytes,
returns on overflow, and adds each packet at the three signed depths' average
divided by 12 plus the caller bias, clamped to a minimum slot of 16.

The SDK clip calls receive the graphics runtime's `EVECTOR **` result table.
GAME reads the pointed records at offsets +16/+20/+24/+28/+32 as depth,
perspective term, packed XY, color, and UV. Psy-Q 3.0's `LIBGTE.H` places
`EVECTOR.sxyz.vz`, `sxyz.pad`, `sxy`, `rgb`, and `txuv` at exactly those offsets.
The source now uses that complete 44-byte SDK record rather than a partial
game-side view. Its pointer-table start at graphics runtime +0x14994 and the
consumed SDK offsets have layout checks.

The helper follows `render_enqueue_map` contiguously in the GAME image and
shares its packet color datum, so both claims now live in one `game.render_map`
source unit. This also removes one GAME module from the PSYLINK input list.

The initial source claim reproduces all seven branches, thirteen CFG blocks,
three return frontiers, direct calls, and reviewed referents. Strict objdiff
reports 95.833336% for the 0x258-byte function, so it remains WIP. Its first
remaining observable differences are register assignment and one UV/color
load schedule; these are unattributed codegen residue, not evidence of a
different type or referent. The exact `render_enqueue_map` control remains
byte-identical after the shared graphics-header refinement.

## Alternate clipped map emitter

GAME `0x8002f808` follows the clipping helper and completes the contiguous
`game.render_map` claim through `0x8002ff5c`. It accepts an object index, an
ordering-table depth bias, and an optional prepared TMD asset. The prepared
path uses that asset's vertex, normal, and primitive offsets; the ordinary
path resolves the object through `tmd_get_object`. Both project vertices with
`0x8002da94` and traverse FT4/FT3 packets. Edge-distance and depth checks
choose between ordinary GT4/GT3 emission and the SDK `Clip4FTP`/`Clip3FTP`
calls, whose inputs are the original TMD vertices. The clipping calls pass
their result count to `0x8002f5b0` when at least three vertices remain.

The source keeps the packet count's post-decrement loop, packet-mode bit,
original-versus-projected vertex distinction, signed edge limits, and
primitive-buffer overflow return. Strict objdiff reports 63.362473% for its
0x754-byte body; the focused listing is 50.3% similar with 51/51 CFG blocks
and 35/34 branches. The retail frame is 168 bytes versus the probe's 112,
and the packet-count and header live-value schedules diverge before polygon
handling. These are observable residues, not an attributed compiler cause.
The neighboring `render_enqueue_map` remains strict 100% in the merged unit.
