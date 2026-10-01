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

## Connected 21-function focused recheck (2026-10-01)

Fresh `kf try --context 0 --no-flow` rebuilds cover the startup loader,
transition controller, resource runtime, asset registry, and their arena
allocator. The **12 `SAME` listings** have previously recorded direct strict
100% verdicts; `SAME` alone is only focused listing evidence. The other nine
remain WIP. Retail calls, byte widths, switch targets, and referenced data
were checked against the existing raw dossiers before considering source
changes.

| GAME address | Focused verdict | First remaining difference or control |
| --- | --- | --- |
| `0x80015d58` | DIFF, 87.1% | Fixed read arena and three unowned copy destinations use literal `lui/ori` instead of retail's relocatable signed-low address form. |
| `0x80015fd4` | DIFF, 96.1% | Unowned TMD workspace `0x8012da68` address form and dependent setup. |
| `0x80016260` | DIFF, 71.6% | Stack-byte register assignments and sentinel branch layout; five unsigned controls and three signed offsets remain typed. |
| `0x800167bc` | SAME | Phase-1 callback control. |
| `0x800167d0` | SAME | Phase-3 callback control. |
| `0x800167e4` | SAME | Phase-2 callback control. |
| `0x800167f8` | SAME | Phase-4 callback control. |
| `0x8001680c` | SAME | Phase-6 callback control. |
| `0x80016820` | DIFF, 98.4% | Seven-phase dispatch is retained; `0x8019e138` callback destination and `0x8012da68` TMD workspace have no proved defining owners. |
| `0x80017608` | DIFF, 96.0% | Two-word allocator residue: retail uses `v0` for the available-size subtraction before writing `a0`; the probe reuses `a0`. |
| `0x80031fa0` | SAME | Registry getter. |
| `0x80032008` | SAME | TMD read-completion callback. |
| `0x80032040` | SAME | Single-cell layer-mask query. |
| `0x800320b0` | DIFF, 26.9% | Raw Z then X coordinate setup is retained; row/column induction and register assignments differ. |
| `0x80032174` | DIFF, 37.9% | View-cell comparisons and referents agree; result register and final move differ. |
| `0x800321d8` | DIFF, 95.8% | Arena boundary `0x8009b0a0` emits `lui/ori` from the provisional literal; retail uses a relocatable `lui/addiu`. |
| `0x80032274` | DIFF, 43.6% | Retail recomputes the VAB slot offset in a 48-byte frame; the probe retains an induction offset and a 56-byte frame. |
| `0x80032364` | SAME | TMD range updater. |
| `0x800339fc` | SAME | TMD archive registry loader. |
| `0x80033ab4` | SAME | Registry setter. |
| `0x80033afc` | SAME | Registry selector. |

The broader `game.cd_memory` rebuild retained **56/57 `SAME` listings**,
including `resource_copy_words`, `cd_archive_queue_read`, and
`cd_archive_read`; only the counted allocator above differs. The five phase
callbacks and three asset-registry functions each retained all matching
listings. No new source owner can yet be justified for the fixed RAM
destinations, so no source/config edit was retained in this recheck. No
repository tests, lint, broad match, or full linked build ran.
