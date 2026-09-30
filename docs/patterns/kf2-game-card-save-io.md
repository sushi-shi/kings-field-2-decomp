# GAME card-save I/O and menu callers

This 25-function GAME survey follows the item-model menu into card-event
setup, directory scans, save reading, checksum handling, and the adjacent
player state flow. For each address, the retail extent, disassembly and CFG,
callers, callees, strings, data references, relocation evidence, and current
match state were reviewed. Address adjacency is a survey boundary, not proof
of original source-file ownership.

| Address | Evidence and role | Final verdict |
| --- | --- | --- |
| `0x80022058` | decimal menu number formatter | WIP, 90.94%; loop/frame codegen residue |
| `0x800221e8` | loads menu item model from archive | exact, 100% |
| `0x800222bc` | releases loaded item model | exact, 100% |
| `0x80022300` | dispatches selection sound and frame timing | WIP, 89.972980%; seven retail CFG blocks versus six compiled |
| `0x80022394` | marks controller input active | exact, 100% |
| `0x800223cc` | input/menu control helper | exact, 100% |
| `0x80022438` | waits for controller button release | exact, 100% |
| `0x80022468` | initializes card event handles | exact, 100% |
| `0x80022550` | closes card event handles | exact, 100% |
| `0x800225b0` | starts card services | exact, 100% |
| `0x800225d8` | stops card services | exact, 100% |
| `0x80022600` | probes and removes a temporary card file | exact, 100% |
| `0x800226ec` | enumerates directory entries and save-slot names | unclaimed; card-entry record and directory extent WIP |
| `0x800228c8` | reads card files and validates header/name fields | unclaimed; card record and failure-state model WIP |
| `0x80022b48` | calls the SDK format routine | exact, 100% |
| `0x80022b74` | reads a save, checks the payload checksum, restores its data | WIP, 93.666664% strict; call set and referents match |
| `0x80022ca0` | creates a save and writes icon, metadata, and payload | unclaimed; 24-block state flow and card layout WIP |
| `0x80023178` | writes experience/level digits into the card label | WIP, 93.529410%; register/order residue |
| `0x80023288` | sums payload bytes for the card checksum | exact, 100% |
| `0x800232ac` | waits for one of four card events | exact, 100% |
| `0x8002332c` | clears card event states | exact, 100% |
| `0x80023384` | computes player collision vertical margins and death condition | WIP, 50.744186%; player-state/cache ownership still under review |
| `0x80023430` | computes player distance with a margin | exact, 100% |
| `0x80023484` | resets the player view | exact, 100% |
| `0x80023570` | restores equipment effects | exact, 100% |

The new `0x80022b74` source has the retail nine-block control-flow shape:
three read attempts, distinct I/O and checksum failure codes, a checksum of
the bytes after the 0x400-byte card header, and a successful restore call. It
uses the SDK file I/O and string APIs and the shared 0x4000-byte card buffer.
The compiler's frame and register choices still differ: retail saves four
`s` registers in an 80-byte frame; the probe saves three in a 72-byte frame.
The loop increment/branch schedule also differs. The source remains WIP;
no unsupported padding or carrier local was added to force the frame.

The file prefix at `0x80066670` is a 16-byte initialized datum with the
literal `BISLPS-00069`. The successful-load slot byte at `0x8006d6a0` is
owned by the card-event unit. They occupy disjoint retail `.data` ranges, so
the separate unit definitions preserve honest placement. Strict objdiff
matches all 22 data bytes in the format unit (six literal and sixteen prefix
bytes) and the one event-unit byte. All five card-event functions remain
strict exact, as does the adjacent format wrapper. The focused GAME relink
verified 140/140 targets; its remaining command failure was the global
known-reference closure and unrelated map-object `.rodata` addends.

No function in this survey has verified vendored attribution. The parent
campaign owner handles the full build; repository tests were not run for this
batch.
