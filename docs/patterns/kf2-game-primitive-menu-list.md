# GAME primitive quad helpers and menu-list initializer

The contiguous GAME run at `0x80021f10..0x80022058` is three exact C functions
under `probe-gcc257-o2-g0`: `primitive_buffer_begin_poly_ft4`,
`primitive_buffer_commit_poly_ft4`, and `menu_list_init`. Strict objdiff reports
3/3 functions and 328/328 text bytes at 100%; `kf try` also reports three
identical instruction listings. This is a productive probe, not attribution of
the original compiler.

The first helper calls the Psy-Q `SetPolyFT4` function, then writes `0x68` to
the current quad's three RGB bytes. The second calls `AddPrim` with the active
ordering-table entry and current quad, advances the pointer by the 40-byte
`POLY_FT4` extent, then publishes it to the active primitive buffer's cursor.
The direct call targets and four/five ordered global references match retail.
The SDK headers declare both called routines; these helpers are game policy,
not vendor bodies.

The initializer copies ten halfwords from a loaded positioned glyph row, then
sets list geometry and cursor bytes at offsets `0x1c..0x23`. Its destination
starts at `list+4`; the source row's codes start four bytes after the row base.
The copy's source and destination cursors, incremented independently, reproduce
the retail load delay, branch delay slot, and return slot. Each loaded row is
28 bytes: two position halfwords and twelve glyph-code halfwords. A window is
one title plus ten rows, or 308 bytes. Retail bytes and the 308-byte index
stride support eight windows at `0x80063f70..0x80064910`; the instruction's
address pair points to the first selectable row at `0x80063f8c`, an interior
referent. The table's source TU is still unknown, so this unit declares it
externally and does not claim its data.

The shared current quad pointer is at `0x8006d9e0`. The file bytes there are
inside a CPE marker in the PS-X EXE's page-rounded load tail: `CPE\x01`
starts at `0x8006d9dc`, followed by a CPE record header. They are not a
credible pointer initializer. The nearby `0x80021a68` setup writes the
pointer before a representative caller invokes the quad helpers. This supports
a BSS identity, but the original data TU remains unresolved. Defining it in
this helper unit as tentative storage emits an 8-byte COMMON allocation with
the pinned compiler, while the retail identity is four bytes; the final unit
therefore keeps only an external declaration. Broader write-before-read
coverage remains part of the data ownership campaign.

`kf-retail-validate` passes. `kf match --unit game.primitive_buffer` regenerates
the strict report and verifies target relinking; the command still exits nonzero
because unrelated known-reference data ownership across GAME is incomplete.
The full `kf build` reaches the linker and still fails for the known WIP SDK
symbols, beginning with GAME's `InitCARD`.
