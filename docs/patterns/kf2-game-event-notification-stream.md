# GAME event, pose, and notification stream

This 27-function family follows the scene controllers' confirmed calls to the
pose projection and interpolation helpers, their notification counter and
collision-channel calls, and the shared event-state arena. Verdicts are per
function. The exact rows below are strict 100% source matches; the non-exact
controllers remain WIP even where their callees are exact. Earlier campaign
notes that list `0x80045f20` as WIP predate its typed reconstruction.

| GAME address | Verdict | Evidence or remaining issue |
| --- | --- | --- |
| `0x80045cc0` | Exact | Resets the 128-slot effect pool. |
| `0x80045cf0` | Exact | Copies the 64 magic records into effect state. |
| `0x80045d1c` | Exact | Sweeps live effect slots and dispatches updates. |
| `0x80045e18` | Exact | Plays fixed sound `0x40`. |
| `0x80045e3c` | Exact | Plays a requested sound at volume 100. |
| `0x80045e5c` | Exact function | Trigonometric terrain probe reads the shared collision-result pointer; direct strict text is 100%, while the enclosing BSS ownership remains provisional. |
| `0x80045f20` | **New exact** | Eight-argument camera-relative pose projection, 180/180 code bytes and four reviewed referents. |
| `0x80045fd4` | Exact | Optional position and angle interpolation; preserved after the contiguous pose merge. |
| `0x800460a0` | WIP, 99.268295% strict | Animation phase helper has matching call graph but interchanged saved registers for step and half-step. |
| `0x80046144` | Exact | Finds an `f2` marker in a candidate byte stream. |
| `0x800461a0` | WIP, 99.12676% strict | Typed actor-group and event-state stream scan has 14/14 CFG blocks and 5/5 branches; two record-cursor registers and a final increment schedule remain exchanged. |
| `0x800462bc` | WIP, 98.68132% strict text | Script stream calls, CFG, and referents agree; one extra probe load-delay `nop` shifts two jump-table addends. |
| `0x80046700` | Exact | Spawns a map object for an event slot. |
| `0x8004678c` | Exact | Scene controller's typed magic-record update, command branches, text, data, rodata, and ordered relocations now match strictly. |
| `0x800473e0` | Exact | Decrements a nonzero event-counter byte. |
| `0x80047434` | Exact | Increments a counter through 99 and enqueues a notification. |
| `0x800474c4` | Exact | Seven-argument collision-channel transition. |
| `0x800475d8` | WIP, 98.56481% strict | Scene interaction calls the pose projector, channel transition, and notification path; saved-register and instruction scheduling remain open. |
| `0x80047c98` | WIP, 99.81618% strict | Event dispatcher calls the terrain probe, notification queue, and `0x80034e10` image transition; its shared linked-object notification block now follows retail, leaving saved-register and one commutative-add residue. |
| `0x800482f8` | Exact | Clears and initializes typed event-state control, sentinels, and arena regions; 100% strict. |
| `0x800483a8` | Exact | Invokes the active callback with a zero argument. |
| `0x800483d8` | Exact | Saves event-arena offsets through the typed pointer table; 100% strict. |
| `0x80048428` | Exact | Rebases event-arena links by a signed delta. |
| `0x80048498` | Exact | Restores the typed event-arena pointer table; 100% strict. |
| `0x800484e4` | Exact | Rebases event-arena links for the reverse traversal. |
| `0x80048554` | Exact | Saves the selected event, actor, and map-object stream records. |
| `0x800489ac` | WIP, 98.82883% strict | Restores the stream through typed actor, target-group, and map-object records; initial sentinel/base register assignment differs. |

A focused raw-first recheck of the four adjacent event functions kept their
individual WIP verdicts. `0x800460a0` has 6/6 CFG blocks and 2/2 branches;
its even step and half-step occupy exchanged saved registers. `0x800461a0`
has 14/14 blocks and 5/5 branches; the record and marker cursors use exchanged
argument registers and marker loads use equivalent offsets. `0x800462bc` has
46/46 blocks and 21/21 branches; retail loads the actor's phase before its
saved state at `0x80046524`, while the current probe reverses those independent
loads and inserts one load-delay `nop`. The resulting four-byte shift changes
only the jump-table pointer at row `+0x0c` from retail text addend `0x2c0` to
candidate `0x2c4`; the 64-byte table and other ordered referents are intact.
`0x80047c98` has 85/85 blocks and 55/55 branches; its rotation pointer and
constant-one saved registers are exchanged, and one commutative `addu` reverses
operands. Existing source-order trials do not establish a different source
fact, so no C or table edit was retained. Each recheck used a focused quick
build; no repository tests or broad build were run.

A fresh direct strict comparison of the event-state/save/restore family found
13 exact functions and three WIPs (`0x800475d8`, `0x80047c98`, and
`0x800489ac`). In `0x80047c98`, initializing the map-object scan index before
the two independent pool-pointer assignments moves the zero assignment into
the retail branch delay slot and raises the strict result from `97.37745%` to
`98.05147%`. Retail still keeps the rotation argument and the constant one in
different saved registers, and two instructions of control layout remain
unattributed. A separate actor-index local lowered the result and was
reverted. The exact KF1 `map_interaction_dispatch` uses a `for` header for the
analogous object-search increment. Spelling KF2's loop the same way emits an
instruction-identical `98.05147%` object, so the source retains that form.
The controller and restore-stream differences begin with saved-register
assignments; their existing typed calls, fields, and exact adjacent
event-state functions provide no source-backed correction yet.

A later focused pass rebuilt the twelve-function event stream subset under the
current shared headers. The six event-state helpers, event save stream
`0x80048554`, and both pose helpers `0x80045f20`/`0x80045fd4` remain identical
listings. Direct strict comparison puts `0x800462bc` at 98.68132% text and
92.1875% rodata: retail loads actor animation phase (`lhu` at `0x80046524`)
before its saved state byte (`lbu` at `0x80046528`), filling the load delay.
The probe reverses those loads and inserts one `nop`; its two differing
jump-table pointer addends are exactly four bytes later as a consequence.
Swapping the two ordinary C assignments in an off-tree probe emitted the same
listing, so the source remains unchanged. Direct strict `0x80047c98` is
99.81618% text after spelling the shared notification block explicitly.
Retail shares `notify_enqueue(6)` at `0x80048160` before the linked-object
lookup and branches back to it when the linked ID is `0xff`; the retained C
now emits that order. Focused similarity rose from 86.3% to 97.1%, with
85/85 blocks and 55/55 branches. The remaining listing differences exchange
the rotation argument and constant-one saved registers and reverse the
operands of one commutative `addu`; they do not establish a source correction.
Direct strict `0x800489ac` remains 98.82883% text with
100% rodata; the actor-state base and `0xff` sentinel use the opposite
argument registers. None of these residues establishes a different C field,
call, or source operation.

The five callers of `0x80045f20` establish its eight O32 arguments: three
local coordinates, pitch, yaw, vertical and depth offsets, and a `VECTOR *`
result. The retail body makes an `SVECTOR`, negates pitch in a six-byte angle
record, calls `vector_rotate_yxz`, then adds the player's camera position with
the 1600-unit vertical adjustment. A second source expression matched the
retail register schedule exactly. Its three camera-position HI16/LO16 pairs
and the direct call were reviewed against decoded instructions. The new
function and adjacent `0x80045fd4` are now one contiguous unit, with both
listings identical and `0x80045f20` reported at strict 100%.

`0x800461a0` is a WIP reconstruction in the contiguous marker-scanner unit.
Retail selects the first target-candidate pointer in the actor's group, scans
four-byte `f1`/`fe` records, compares one script byte against the event-state
control region, and returns a pointer into the candidate byte stream. The
actor-group, event-state, helper-call, and four internal jump referents were
decoded and curated. The current C expresses those observed fields and returns
with the retail 14-block, five-branch control shape; only two cursor-register
assignments remain exchanged. The preceding `0x80046144` remains identical.

A focused probe initialized the independent marker pointer before the cursor.
It changed only their register assignment and raised direct objdiff similarity
to `99.23943%`, without bringing either pointer's retail register or increment
order into agreement. The simpler cursor-derived pointer was restored and
rebuilt; `0x80046144` remains strict exact.

The current connected-control quick builds remain identical for all six
event-state functions, both pose functions, the save-stream function, and the
`0x80046144` marker leaf. At `0x800461a0`, retail reads the marker record's
second and third bytes as `-2(a0)` and `-1(a0)` from a payload pointer.
An equivalent payload-relative source probe introduced a third pointer in the
pinned compiler and lowered focused similarity from 86.6% to 73.9%; it was
discarded. A single payload-pointer model also changed the fallback-offset
calculation and reached only 83.4% focused. Retail keeps two independently
advanced pointers, so the existing two-pointer source and its WIP verdict
remain unchanged.

The menu preview at `0x8002083c` still has a 64-byte stack-frame extent gap.
JP and US retail both use a 224-byte frame; EU retail also has a 224-byte
frame. This regional agreement constrains the missing source aggregate but
does not identify it, so no padding was inserted. The `0x80034e10` image
transition remains WIP at 99.114586%: JP and US use the same first
buffer-boundary register, while EU uses the other register seen in the current
probe. That regional codegen difference does not prove an original source
expression or compiler attribution.

The shared TMD packet header was kept layout-identical while making its
prepared payload and packed GT3/GT4 views parseable by the checked-layout
inventory. The header parser reads a 4096-byte prepared object, 40-byte GT3,
52-byte GT4, and 18-byte notification row. The exact `render_enqueue_map`
listing remains identical. No repository tests, banking, or commit were run
for this batch. A later full `kf match` is required after concurrent source
merges settle; the last global exit was blocked by a transient stale Ninja
source dependency and the existing known-reference data closure.

The GAME `0x80012960..0x80012bf3` event-save table now has the conservative
`func_80048554_rodata` identity and `0x80048554` owner. Retail at `0x800486a8`
bounds an unsigned record kind to `0..0xa4`, multiplies it by four, and jumps
through that exact 165-word range. Every curated word equals its retail
little-endian value and targets one of five labels inside the serializer;
the 165 relocation rows were promoted from range-only candidates to reviewed
pointers without changing any address or target. A safe one-VA delink has zero
withheld relocations and the focused save-stream listing remains 1/1 SAME.
Its existing strict exact verdict is preserved, with no broad match rerun.

The paired GAME `0x80012bf8..0x80012c37` restore table is owned by
`func_800489ac`: retail bounds opcodes to `0xf0..0xff`, indexes 16 words, and
jumps through the loaded pointer. All 16 reviewed rows equal their raw retail
words and land in eight labels inside the restore body. The existing source
claims that complete 0x40-byte RODATA range. The owner identity was narrowed
without changing the table or relocation targets; safe one-VA delink reports
zero withheld relocations. Focused restore remains 95.1% listing DIFF, with
the first difference in sentinel/base register allocation; its prior strict
98.82883% WIP verdict is unchanged pending a direct strict rerun.

The GAME event opcode table at `0x80012890..0x800128cf` now names
`func_800462bc` as its curated owner. Retail subtracts `0xf0`, bounds the
unsigned index to `0..15` at `0x8004639c..0x800463a8`, and jumps through
the selected word at `0x800463c4`. All 16 raw words are aligned pointers to
11 labels inside `0x800462bc..0x800466ff`; the source claims exactly 0x40
RODATA bytes. A safe one-VA delink reports 130 relocations and zero withheld
functions or relocations. Focused comparison remains 90.5% listing DIFF with
46/46 CFG blocks and 21/21 branches. This owner correction neither proves
the original TU boundary nor adds an exact result.

The adjacent command table at `0x800128d0..0x8001295b` now names
`func_8004678c` as owner. Retail subtracts command `0x52`, bounds the
unsigned index to `0..0x22`, and dispatches through the resulting 35 words.
All raw words are aligned pointers to 21 labels inside the command handler
`0x8004678c..0x800473df`, and its source claims the full 0x8c-byte range.
Safe one-VA delinking reports 471 relocations and zero withheld functions or
relocations. An isolated pinned compile and strict objdiff show 100% `.text`
(3156 bytes), `.data` (40 bytes), and `.rodata` (140 bytes); the ordered
218 `.rel.text` and 35 `.rel.rodata` entries are identical. The focused unit
listing is also 1/1 SAME. This rechecks the existing exact verdict without
changing the exact count; older analyzed scores are stale.
