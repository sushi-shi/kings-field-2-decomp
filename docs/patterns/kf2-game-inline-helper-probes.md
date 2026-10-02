# GAME inline-helper probes

The pinned Psy-Q 3.0 C headers contain no `inline` function bodies. The
current project has seven `static inline` definitions with seventeen lexical
call sites: three prefix-copy helpers in `menu_status_render.c`, one prefix
copy in `menu_attribute_render.c`, glyph setup in `menu_draw_string.c`,
notification dequeue in `notify_enqueue.c`, and angle folding in `math.h`.
These are current reconstruction spellings, not recovered original source.
`include/kf/lib/enum.h` also spells C++20 `inline` operators inside modern
checking-only macros; its retail C branch expands those macros to nothing,
so those operators provide no evidence for the original GAME source.

The exact GAME notification updater `0x80033284` embeds the dequeue loop; its
retail callees include `menu_format_number` and four calls to
`notification_digit_set_v`, with no separate dequeue call. Under the pinned
probe, removing `inline` from `notification_dequeue_group` emits an out-of-line
`jal` and changes the focused caller from SAME to 87.5%. Restoring the typed
inline helper returns all three functions in `game.notify_enqueue` to SAME.
A direct macro expansion also changes the dequeue path's address and load
schedule, giving 91.3%; it was reverted and all three focused listings are
again SAME.

The exact GAME status renderer `0x8001e94c` embeds three eight-byte, one
ten-byte, and six twelve-byte template copies. Removing `inline` from only
`menu_copy_prefix8` emits helper calls and lowers the caller to 75.7%. A
direct `#define` expansion of `memcpy(destination, source, 8)` keeps the copy
in the caller but emits aligned `lw`/`sw` pairs at the three sites where
retail has `lwl`/`lwr` and `swl`/`swr`; its focused score is 92.2%. The same
one-helper-at-a-time direct-macro probe gives 98.9% for the ten-byte copy and
88.1% for the six twelve-byte copies, with the same aligned versus unaligned
instruction distinction. Restoring all three `s16 *` typed inline wrappers
returns `game.menu_status_render` to SAME.

The pointer boundary in these wrappers preserves the weaker alignment known
to the compiler and therefore matters to the matched instruction selection.

These probes exclude an ordinary out-of-line helper under the current
compiler profile and distinguish this typed inline wrapper from a direct
macro expansion. They do not prove the original author used the `inline`
keyword: another source form could have produced the same typed pointer
boundary and embedded operations. No alternative spelling was retained.

The same eight-byte wrapper occurs in exact `menu_attribute_render`
`0x8001f008`. Replacing only that wrapper with the direct `memcpy` macro left
its focused listing SAME. This sibling does not distinguish the two source
spellings; the status renderer's three unaligned copy sites provide the
stronger constraint. The inline wrapper was restored in both units.

The WIP glyph renderer `menu_draw_string` `0x800210ac` calls the current
`menu_begin_text_glyph` helper three times. Retail has three primitive-buffer
begin calls and no separate glyph-setup call. A direct macro expansion of the
helper body leaves the focused result at 90.4%, with the same 56-versus-48-byte
frame and glyph-UV register differences as the typed inline form. This probe
cannot attribute the original spelling; the typed inline form was restored.

KF1's exact `menu_draw_string` uses an indexed glyph loop, while the KF2
source advances a glyph pointer. An off-tree KF2 probe using the KF1 loop
shape, including indexed glyph reads and a signed glyph local, lowered the
direct strict result from 99.66904% to 76.08541%. Isolating the changes gave
98.65836% for an indexed loop with the existing unsigned glyph and 75.98933%
for a signed glyph with the existing pointer loop. The 12-bit glyph mask is
nonnegative either way, but signed division selects a different instruction
sequence under this probe. KF1's spelling does not explain KF2's remaining
frame and UV-register residue; the KF2 source remains unchanged.

Across the seven current C helper definitions, six were checked against a
direct macro expansion. Four macro spellings changed an exact listing and
were rejected (the three status copy widths and notification dequeue). The
attribute copy remained SAME, and glyph setup kept its existing 90.4% WIP
listing; neither result proves macro origin. The seventh helper,
`angle_error_magnitude`, belongs to an exact player unit and was left to its
owner without a source probe. These counts concern reconstruction alternatives,
not the separate pinned-SDK macro census.
