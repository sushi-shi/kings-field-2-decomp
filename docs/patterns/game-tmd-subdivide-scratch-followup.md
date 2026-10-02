# GAME prepared-TMD subdivision scratch follow-up

GAME `0x8002ff5c` remains **57.638040% strict** against the safe target with
the pinned `probe-gcc257-o2-g0` profile. The retail and candidate bodies have
11/11 known CFG blocks, 6/6 branches, all 14 ordered `resource_copy_words`
calls, and 16 ordered relocation rows. Retail reserves 1,248 stack bytes;
the retained candidate reserves 1,232. No source change or exact claim follows
from this audit.

At the first FT4 copy call, retail saves `$t4` through `$t9` across
`resource_copy_words`; the candidate saves only `$t4` through `$t6` at that
site. Those three additional retail word spills are consistent with the
16-byte frame gap after alignment, but do not by themselves establish the
original declaration layout.

Retail reuses `sp+0x440` as a four-byte scratch home: it stores the packet
header there before a byte-mode read, stores packed FT4/FT3 vertex-index words
before signed halfword reads, and stores generated halfword indices there
before reading their two bytes for child packets. The candidate instead
extracts the header mode in a register and reads indices directly from packet
halfwords. A typed pointer to the local header's mode field emitted the
`lw`/`sw`/`lbu` shape but lowered strict text to **57.004910%** and still
missed retail's duplicate top-of-loop word load. A typed packed-word union
for the first FT4 vertex index compiled byte-identically to the retained
source. Separating the bottom packet-length header into its own scope also
compiled identically.

A shared `u16` serialization local expresses the raw halfword-to-byte value
flow and emitted `sh`/`lbu`, but strict text fell to **55.053990%**; its
scratch home was `sp+0x468` and its frame shrank to 1,224 bytes. Macro-local,
function-local, and branch-local declarations did not recover the retail
home or schedule. These trials were left off-tree.

Retail also packs midpoint U/V values through masked register-word updates:
it has 16 `or` instructions versus two in the retained candidate, which
keeps separate byte locals. Off-tree `unsigned short` and `unsigned int`
eight-bit-field scratch types emitted 10 and 18 `or` instructions, respectively,
but strict text fell to **52.164417%** and **54.042946%**, and their frames
grew to 1,400 and 1,320 bytes. The bitfield forms do not prove an original
type or storage owner. The retained typed packet header and U/V byte pair
remain the clearest supported source; the shared stack home is a raw codegen
fact, not yet a recovered C declaration.
