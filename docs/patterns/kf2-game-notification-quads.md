# GAME notification quads

GAME `0x80032fec` and `0x80033140` are contiguous strict objdiff
`100.000000000%` matches in `game.notification_quad`.
The drawing helper uses Psy-Q's inline `setPolyFT4` and `setSemiTrans` macros,
fills an authentic 40-byte `POLY_FT4`, and links it to ordering-table slot
one. Its caller traverses seven 18-byte `KfNotificationQuad` records until
the last record's `0xff` sentinel, draws each active record twice with
texture-page flags `0x20` and `0x40`, and replicates
`game_graphics_runtime.notification_brightness` into its RGB color.

Retail bytes at `0x80066808..0x80066886` initialize six textured rectangles
and the sentinel. The table's fragmented seed identities were merged into
one `DATA` claim in `game.notify_enqueue`, whose adjacent notification state
code updates each record's kind and digit texture position. The drawing unit
uses the table as an external global, matching the named cross-unit
relocation; its earlier same-unit placement emitted a `.data` section
referent instead. Both drawing functions and all three functions in
`game.notify_enqueue` are strict exact, and the global report verifies all
7/7 data-owning units.

The pinned Psy-Q 3.0 `setXYWH` and `setUVWH` macros were tested for the
source-coordinate and texture-coordinate groups in `notification_draw_quad`.
Their argument expansion repeats source loads and changes the retail store
order; the focused listing fell to 74.0% with both substitutions. The exact
chained assignments were restored, and focused `game.notification_quad`
returned SAME 2/2. This rejects those two macro spellings for this function,
without claiming that the original source lacked other inline helpers.
