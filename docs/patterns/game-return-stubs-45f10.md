# GAME return stubs at 0x80045f10 and 0x80045f18

The Japanese GAME payload has two consecutive `jr $ra; nop` pairs at
`0x80045f10..0x80045f1f`, between the complete `0x80045e5c` return
(`jr $ra` at `0x80045f08`, stack-restore delay slot at `0x80045f0c`)
and the `0x80045f20` function prologue. The four raw words are
`03e00008 00000000 03e00008 00000000`. The 16-byte range was previously
one unclassified data row. These are two independently complete eight-byte
return bodies; their source signatures and roles are not established by the
instructions alone.

The semantic navigator has no incoming references to either entry, and the
curated relocation inventory has no target at either address. Neither lies
in the confirmed Sony/Psy-Q archive spans recorded for the later GAME
library region. They are therefore provisional GAME function identities,
not semantic callback names. Each is claimed by a separate empty C function
in one contiguous WIP unit. A focused safe carve withheld no function or
relocation; the focused listing is 2/2 SAME. Fresh direct objdiff matches
the entire unit's 16/16 `.text` bytes at strict 100%, with no relocations.

This admits two more functions into the GAME eligible and started counts.
The byte-exact result proves the current C emits the retail return bodies;
it does not prove the original parameter lists, return types, or TU owner.
