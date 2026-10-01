# GAME return stub at 0x80018764

Japanese GAME bytes `0x80018764..0x8001876b` are `jr $ra; nop`
(`03e00008 00000000`). They follow the complete `cd_close_events` return
at `0x8001875c/60` and precede the separate `0x8001876c` menu-controller
prologue. The eight bytes were previously an unclassified data row. The
semantic navigator and curated relocation inventory have no incoming
reference to this entry, and no SDK archive signature covers it. Its role,
parameter list, return type, and original TU owner remain unresolved.

The address-derived C claim is isolated in a one-function WIP unit. A safe
one-VA carve withheld no relocation or function; focused compilation is
SAME. Direct strict objdiff matches all 8/8 `.text` bytes with no
relocations. This admits one more GAME function without assigning a semantic
name or claiming historical source spelling.
