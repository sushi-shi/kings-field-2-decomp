# SDK interfaces

The headers in `include/` **are used by game code**. They provide guarded
SDK includes, missing API declarations and case-sensitive include shims for
the pinned Psy-Q 3.0 headers. Their public include names remain `psyq/*.h`,
`sys/*.h` and the lower-case shims (`asm.h`, `r3000.h`); the compiler searches
`vendor/include/` before the separately supplied SDK headers.

The executable build links the original Psy-Q 3.0 objects and libraries. No
Sony library routines are reconstructed here; SDK functions are identified in
`config/retail/functions_vendored.tsv` and excluded from game progress.
Original project licensing does not grant rights to Sony/Psy-Q material.
