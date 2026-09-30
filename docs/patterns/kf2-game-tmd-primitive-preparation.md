# GAME TMD primitive-index preparation

GAME `tmd_prepare_primitive_indices` at `0x8002d5dc` is a 0x2d4-byte
call-connected precursor to the exact `tmd_register` unit. Four direct callers
are decoded at `0x8002d8d8`, `0x8003201c`, `0x80033a70`, and `0x80033ae4`.
Its body has no direct calls or strings. It reads a TMD object's word-width
count and packet offsets, advances packets by the input-length byte, masks
the mode with `0xfd`, and shifts normal and vertex halfword indices by three
for eight polygon modes. The 29 pointer rows at `0x80011410`–`0x80011484`
form its source-owned switch table. Eight decoded in-body jumps and the
table-base HI16/LO16 pair are reviewed.

The C uses typed packet bodies and the exact King's Field I parser as a source
shape reference, with the GAME II pointer argument and observed word-width
counts. It lives in a separate unit because joining it to `tmd_register`
changes that exact caller's external call relocation into a `.text`-relative
one. The source is WIP: the first strict comparison was 88.342545% for its
724-byte body, and its 116-byte RODATA claim had the right size but a table
entry addend difference (`+0x2c` retail versus `+0x2bc` compiled at row +4).
The entry/count schedule and several case block placements still differ.
Retail decrements both the object and primitive counts before comparing them
with `-1`; spelling both loops that way yields 17/17 focused CFG blocks and
4/4 branches, versus 17/18 and 4/5 for the initial source. Strict objdiff
improves to 96.1326% for the revised 724-byte body, preserving the exact
neighboring `tmd_register` function. The switch table still differs at its
first row: retail `.text` addend `+0x80`, reconstructed `+0x84`. This follows
the four-byte shift in the compiled switch setup, so the table remains WIP.

The current source models the decoded operations; it does not claim an exact
toolchain attribution or table byte match. `kf-retail-validate` and
`kf inventory check` pass for the typed source and coalesced data identity.
A controlled compile of the same source under the pinned GCC 2.6.0 probe
had a lower focused listing similarity than GCC 2.5.7 (36.9% versus 38.6%)
and the same initial extra `move`; this does not establish the historical
compiler.
Among the pinned GCC 2.5.7 profiles, `-G0` and `-G8` produced the same
focused listing; `plain` and `nosched` probes were substantially farther
from retail. None made the parser exact.
