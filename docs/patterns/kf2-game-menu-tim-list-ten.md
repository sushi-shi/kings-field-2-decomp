# GAME archive-image menu and list branch

This ten-function GAME survey follows the item menu's confirmed archive-image
call, its player clamp helpers, and the neighboring list controllers through
shared row tables and direct calls. Every function received the matcher
address, block disassembly, incoming xref, callee, string, and match queries.
The address span is a survey boundary, not a claim of original TU ownership.

| Address | Retail role and decisive evidence | Final verdict |
| --- | --- | --- |
| `0x80019240` | player/menu clamp caller of the adjacent two leaves | strict exact, 100% |
| `0x800192ac` | clamp leaf called by `0x80018f8c` and `0x80019240` | strict exact, 100% |
| `0x800192dc` | related clamp leaf with the same callers | strict exact, 100% |
| `0x8001930c` | menu code selects archive 6 entry, uploads TIM, emits textured quads, waits for pad input, frees buffer | Source claimed; focused WIP at 80.2% listing similarity, with aligned calls, referents, and CFG |
| `0x80019834` | filters list records, runs input/dialog loop, redraws through `0x8001fc94` | **strict exact, 100%** in the current two-function unit |
| `0x800199d0` | exact 26-byte selection-record filter used by three list controllers | strict exact, 100% |
| `0x80019ac4` | list controller copies the reviewed 200-byte `menu_equipment_labels_64910` table, then calls row builder and three selection branches | **strict exact, 100%** in the current focused unit |
| `0x80019ce4` | builds ten 20-byte glyph rows from the two source tables | **exact in later follow-up**; selection bytes belong to typed player state |
| `0x80019ed4` | menu selection controller with direct calls and indirect dispatch | **strict exact, 100%** in the current focused unit |
| `0x8001a2f4` | list controller calls exact `0x800199d0` row selector | **strict exact, 100%** in the current focused unit |

The sole proven caller of `0x8001930c` is `0x80018ac8`, which passes a
zero-extended selected byte. The callee reads `a0`, subtracts 67, and masks
the result to a byte when selecting eight-byte archive entries. The current
source and curated signature use `void func_8001930c(s32 menu_code)` to model
the observed O32 argument; the historical formal type is still unproved. The
direct call chain is archive entry extent, allocation, archive read, TIM upload,
frame/primitive helpers, two-frame panel draw, pad wait, and free. Its
carry-adjusted HI16/LO16 pairs identify `state_8017d118+9` and
`player_state+0xd8`, both supported BSS object views rather than independent
absolute-address globals. Focused `kf try` reaches 80.2% listing similarity;
the remaining frame, saved-register, and archive-arithmetic schedule residue
is not attributed to a compiler mechanism or treated as strict closure.

The `0x80019ce4` row builder uses the already exact 120-row and 20-row
initialized glyph tables, but also reads a cluster of player-state selection
bytes near `0x80198567`. The adjacent `0x80018dec` selector was later matched
exactly using typed equipped item IDs; the later menu list pass made
`0x80019ce4` strict exact as well. All
four exact controls in the original survey were preserved at that checkpoint.
No surveyed function has verified SDK/vendor attribution. Repository tests
were not run.

Fresh focused rebuilds and direct one-unit objdiff comparisons find **9/10
strict exact** in this survey. The `0x80019834`/`0x800199d0` unit is 656/656
code bytes at 100%; `0x80019ac4` is 544/544 code bytes and 200/200 owned
data bytes, `0x80019ed4` is 1,056/1,056,
and `0x8001a2f4` is 508/508. The three `0x80019240`–`0x800192dc` clamp
functions and `0x80019ce4` also retain 100% strict scores. Only the TIM menu
preview at `0x8001930c` remains WIP at 80.2% focused: its 47/47 blocks,
27/27 branches, calls, and known successors agree, while the first arithmetic
residue reassociates the archive-index `+480` and retains the image pointer
in a different saved register. No source edit or linked build was needed for
this verification.
