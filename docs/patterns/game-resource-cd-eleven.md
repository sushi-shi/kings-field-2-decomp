# GAME resource startup and CD control: eleven-function verdict

This focused set follows the startup archive loader into transition phase
controls and the CD functions it calls. The two startup functions were
reviewed against retail disassembly, CFG, calls, strings, data references,
relocation pairs, their main-loop callers, neighboring phase functions,
and the KF1 resource loader. The three CD functions and allocator were
checked in a fresh one-unit strict report with the other 53 exact CD
siblings as controls.

| GAME address | Role | Direct strict verdict |
| --- | --- | --- |
| `0x80015d58` | Open seven archives and unpack startup resources | WIP, 89.03145% |
| `0x80015fd4` | Pump resource transition and install TMD slot zero | WIP, 89.710144% |
| `0x800167bc` | Set transition phase 1 | exact, 100% |
| `0x800167d0` | Set transition phase 3 | exact, 100% |
| `0x800167e4` | Set transition phase 2 | exact, 100% |
| `0x800167f8` | Set transition phase 4 | exact, 100% |
| `0x8001680c` | Set transition phase 6 | exact, 100% |
| `0x80017608` | Allocate and split a CD/resource arena block | WIP, 99.78261% |
| `0x8001779c` | Yield one CD request step | exact, 100% |
| `0x800182d0` | Read an archive entry | exact, 100% |
| `0x800184d0` | Open an archive | exact, 100% |

The targeted startup and phase-unit rebuild/direct objdiff reports five
phase setters **5/5 exact** and the startup's **83/83 `.rodata` bytes exact**.
The CD-unit direct report is **56/57 exact** with its 11 initialized `.data`
and 33 `.rodata` bytes exact. Thus the selected eleven have **8 exact** and
three WIPs; the CD unit's separate 772-byte BSS/COMMON placement mismatch
is not evidence about these function bodies.

Retail `0x80015d58` is one straight-line block with 22 proven direct calls
and the seven `COM\\*.T` strings. The source matches the archive order,
length-prefixed word copies, and final session initialization. Its first
divergence is the fixed read-arena pointer `0x8009b0a0`: retail uses a
carry-adjusted `lui/addiu`, while the provisional C literal emits `lui/ori`.
Later copies and TMD loads construct `0x801d8d88`, `0x800fa0d0`, and
`0x800855a0` with reviewed signed-low pairs; those destinations have no
proved complete objects or defining translation units. The existing raw
literal pointers remain WIP rather than fabricated storage or linker
equates.

Retail `0x80015fd4` has three blocks: it saves five transition bytes,
pumps `cd_request_yield` and `0x80016820` until the active halfword clears,
sets TMD slot zero, then calls the active callback table's sixth entry.
The callback target stays indirect. Its first source/object divergence is
the TMD destination `0x8012da68`, formed by a reviewed `lui/addiu` pair.
That address is 0x10 bytes beyond the complete
`display_primitive_memory` extent and is also the destination of a later
archive read. These uses prove the pointer but do not prove the workspace's
allocation, size, or source mechanism. KF1's resource loader uses named
chunk/cursor buffers and does not settle KF2's fixed destination owner.

The allocator at `0x80017608` retains its separately documented two-word
temporary-register residue: retail uses `v0` for `available - 12` before
subtracting into `a0`; the compiler reuses `a0`. Its CFG, calls, block
split threshold, and owner write agree. No source/config edit was retained
for this batch. Verification used only targeted unit builds, direct
per-unit objdiff, and focused `kf try`; no repository tests, lint, full
linked build, broad match, or banking ran.
