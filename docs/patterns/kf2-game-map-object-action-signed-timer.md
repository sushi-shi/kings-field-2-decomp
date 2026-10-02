# GAME map-object action counters and state resets

`func_80036ed4` at GAME `0x80036ed4` dispatches map-object actions through
three bounded switch tables. In action 4, the prior value of the 16-bit timer
at object `+0x40` is incremented and stored, then interpreted as signed for
the `<32`, `>=300`, and `<332` gates. Retail loads it with `lhu` at
`0x80037464`, stores the increment at `0x80037470`, and sign-extends the
*prior* value with `sll`/`sra` at `0x80037474..78`. The source now gives the
local `previous` type `s16`; redundant casts on those three comparisons were
removed. The field remains the existing halfword view, and the increment and
all call paths are unchanged.

This one local type correction improves the fresh focused listing from 82.6%
to 89.8%. Manual isolated strict objdiff improves `.text` from 96.54460% to
96.796036% over 7,668 retail bytes and `.rodata` from 39.989517% to
94.706500% over 956 bytes. The probe now also emits exactly 7,668 text bytes,
matching retail length. Direct RODATA word comparison finds 219 of 239 words
identical; the twenty remaining switch-pointer addends differ by only four or
eight bytes. All 32 initialized `.data` bytes and the four
individual vector data claims remain exact. The signed source produces 252
`.text` and 238 `.rodata` relocations, equal to retail counts; all 238 ordered
RODATA relocation sites/types match. The 180/180 branch count remains, while
retail/candidate CFG block counts remain 329/327. Later text relocation order
and some switch pointer addends still differ, so the function is WIP and has
not been banked. Removing the redundant casts emitted a hash-identical object.

The action-81 timer-zero arm has a separate raw-backed write gap. Retail at
`0x800381a4..0x800381b8` stores timer 2, clears the object halfword at
`+0x0a`, writes flag 1 at `+0x01`, then overwrites the halfword with `0x0fff`
in a jump delay slot. The earlier C omitted the zero store. Spelling those
writes in order reproduces the retail local sequence. Keeping the
`KfMapObjectTemplatePoseView` pointer local to the later pose-query block also
removes the probe's extra template-pointer move before the timer switch;
three direct casts in a trial emitted an identical object. This source stage
retains the retail 7,668-byte text length, and isolated strict `.text`
improves to **97.08555%** while `.rodata` remains **94.7065%** and initialized
`.data` remains **100%**. Focused listing rises to **90.0%**; CFG moves from
329/327 to 329/328 blocks, with 180/180 branches. The 252 text and 238
RODATA relocation counts remain equal to retail; all 238 ordered RODATA
relocation sites/types/names agree. The function is still WIP because late
text order and switch-pointer addends differ.

Action 5's timer-zero branch has another directly observed read: retail
reloads the linked object's halfword index from object `+0x3a` at
`0x80037950` before calling `map_object_set_property`. The previous source
passed a local value read before an independent linked-object update. Passing
the object field in that later call reproduces the retail `lhu`, the branch
delay-slot argument setup, and the local call-site sequence. The retained
source stage reaches **90.5% focused listing** and **97.49452% isolated strict
`.text`**, with `.rodata` **94.7065%**, initialized `.data` and all four vector
claims **100%**, and a 7,668-byte text body equal to retail. CFG remains
329/328 blocks with 180/180 branches. The function remains WIP; no exact
claim is made.

Action 83 timer case 9 had a missing state transition. Retail at
`0x80037b60` branches to the shared jump at function `+0x224`; its delay slot
clears `action_timer` whether or not the object byte equals 3. The earlier C
only set `unknown_0a` to `0x0fff` when that byte equalled 3, then left the
timer unchanged. Clearing `action_timer` after the conditional reproduces the
retail `0x80037b58..0x80037b6c` instruction sequence exactly. The latest
focused listing is **90.6%**, with 329/328 CFG blocks and 180/180 branches;
isolated strict `.text` is **97.23943%** and `.rodata` **94.7065%**. The strict
text percentage falls from the preceding stage because switch case offsets
and later code layout change; the timer transition is directly proved by the
retail jump and delay-slot store, so this semantic correction is retained.

Verification used the affected `kf try` unit and isolated target-versus-probe
objdiff only. No repository tests, lint, full build, broad match, or README
update was run.

In action 88, retail `0x80037b70..0x80037ba4` dispatches timers 1, 2, and 3
with signed compare branches, and `0x80037ba8..0x80037bd8` handles the two
selector values before the nine-argument spawn call. Replacing the selector's
equivalent `if`/`else if` with a `switch` made the focused listing worse
(90.6% to 85.7%) despite aligning the CFG block count at 329/329. It also
changed code far beyond that selector. The trial was reverted; the remaining
branch placement is not evidence that the selector semantics are wrong.

Action 84 has two independently visible scale-store triples. Retail writes
Z, Y, then X at function offsets near `+0x12e0` and `+0x13d0`, putting X in a
jump delay slot; the earlier C wrote X, Y, then Z. Reordering those independent
stores reproduces both local instruction sequences. The retained source now
gives 91.0% focused similarity, 329/328 CFG blocks and 180/180 branches.
Isolated strict text is **97.24152%** over 7,668 bytes, RODATA remains
**94.7065%**, and DATA remains **100%**. The function is still WIP because
other control placement, address arithmetic registers, and table addends
differ.

Action 34 computes a target yaw from the high spawn byte. Retail shifts the
zero-extended byte left four bits and then negates it. The earlier expression
negated the byte before multiplication by 16, and the probe emitted `negu`
before `sll`. Grouping the multiplication under the negation reproduces the
retail instruction order without changing the angle's value. With both action
84 store-order corrections retained, the fresh focused listing reaches
**91.1%** and isolated strict text **97.345856%**; the 7,668-byte function,
956-byte RODATA, 32-byte DATA, CFG, and branch counts remain as above. This
is still a WIP function.

For action 19, retail schedules the linked-object `unknown_38` zero store
between the source-byte load and destination-byte store. Reordering the two
ordinary C assignments caused wider control-layout changes and lowered the
focused listing from 91.1% to 84.7%, so that trial was reverted. The store
schedule alone does not establish a different field or state transition.

The action-84 map-depth argument comes from an unsigned spawn byte times
negative 128. Retail computes `negu` on the byte before `sll` by seven; the
earlier C grouped multiplication before negation and emitted the reverse
order. Grouping the negation first preserves the bounded value and reproduces
the local instruction sequence. The latest focused listing remains **91.1%**;
isolated strict text rises to **97.4674%** on the same 7,668-byte body, with
unchanged 94.7065% RODATA and exact DATA.

The same action-84 argument setup loads the depth byte, X/Z cell centers,
width, and height as distinct source values. Separating the two center bytes
from the final centered coordinates moves the first center load into the
retail position. The remaining center/width loads still use exchanged
registers and order, so this does not close the arm. The retained stage has
**97.4747%** isolated strict text and **91.1%** focused listing; RODATA and
DATA scores are unchanged.

The raw action-84 call at `0x80037ee4..0x80037f1c` resolves the two bytes of
`unknown_3a` more strongly than the earlier field guess. Retail puts byte
`+0x3b` in the width argument (`$a2`) and subtracts half of that width from
the X center at `+0x39`; byte `+0x3a` is the Z center in the `$a1` delay-slot
subtraction. The earlier C reversed width and Z center. Correcting those
roles removes the local register/field mismatch and raises isolated strict
text to **97.47992%**; focused listing is **91.2%**. The function remains WIP.

Action 225's success path tests the returned occupancy result, then checks its
one-shot byte before dispatching one of three operations. Retail's first
branch at the end of this case uses `beqz` toward the clear path. Expressing
success first in C gives that branch direction while preserving the same
source behavior. The retained stage rises to **92.4%** focused listing,
**97.62389%** isolated strict text, and **95.80713%** RODATA; DATA stays exact.
The candidate still places the clear block and several action-225 joins at
different offsets, so no exact claim is made.

The action-83 timer-one subdispatch has the same four byte-valued cases in
retail and C. Retail's `0x80037a34..0x80037a64` comparison layout routes
values 0/1 to a shared timer store. Moving the existing 0/1 C case labels
ahead of 2/3 preserves the field writes and calls while removing that local
comparison-layout difference. A fresh focused build rises from **92.4%** to
**92.7%**; isolated strict text rises from **97.62389%** to **97.77204%** on
the same 7,668-byte body. RODATA remains **95.80713%** and DATA **100%**.
CFG remains 329/328 blocks with 180/180 branches, so this is WIP.

Two later action exits share a latch-clear tail in retail. The zero result
from action 81's `func_8002b9d4` call branches at function `+0x1488`, and the
zero result from action 225's `func_80036ad8` call branches at `+0x1b5c`.
Both target `+0x1c24`, whose jump delay slot clears
`extra_40.bytes[0]`. Joining the two source paths at a single labelled clear
reproduces that branch and delay-slot shape. The isolated strict text score
rises from **97.77204%** to **98.012%** with the same 7,668-byte body.

With that shared exit present, action 88 timer case 1 exposes a separate
two-value dispatch. Retail function `+0x0cd4..+0x0d08` compares selector
values 0 and 1, shares one timer store and sound call, and falls through to
the spawn call for other values. A source `switch` over those values now
reproduces that entire instruction range, including the branch delay slots.
The earlier switch trial above was on an older source layout and remains a
valid negative control for that stage. The retained focused object now has
**98.30673%** isolated strict text and exact 32-byte DATA. Its candidate
text is 7,672 bytes versus retail's 7,668, with a RODATA pointer-addend
difference beginning at `+0x10`; it is still WIP.

Action 34 has a further control-flow correction. Retail branches on the
`func_80036ad8` result at function `+0x1c48`: zero goes to the late latch
clear at `+0x1d6c`, while success checks the one-shot byte and performs the
calls. Expressing the success arm first in C preserves the single call and
field writes and reproduces that branch direction and late clear block.
Focused strict text rises to **98.598854%**. The candidate text is now 7,676
bytes against retail's 7,668; DATA remains byte exact and RODATA still has
an addend difference beginning at `+0x10`.

Action 84 timer case 0 had one incorrect state transition. After
`func_80036ad8` returns zero, retail at function `+0x1054..+0x1060` compares
the marker byte with `0xff`; a different value branches directly to the
next object. The earlier C incremented `action_timer` on that path through
its `else` arm. Removing that increment preserves the successful action
path, changes the failed-query branch to the retail exit, and raises focused
isolated strict text to **98.60146%**. The small score change reflects the
large unchanged body; the removed state transition is required by raw flow.

Action 89 timer cases 2 and 5 both index the occupancy grid using the
object's Z row and X column. Retail at function `+0xe60..+0xe8c` and
`+0xf6c..+0xf84` forms the row base before shifting the column, whereas the
earlier compound two-dimensional C expression caused the probe to shift the
column first. Naming the typed row pointer in each case preserves the cell
and layer identities and reproduces the retail address-construction order.
The first change raises isolated strict text from **98.60146%** to
**98.80229%**; both together reach **99.00313%** over the 7,668-byte retail
body, with **88.8%** focused listing. Retail and candidate each have 329 CFG
blocks, 180 branches, 252 text relocations, and 238 RODATA relocations. The
ordered relocation type/symbol sequences agree in both sections, all 238
RODATA relocation sites agree, and the first text relocation-site mismatch
now occurs at row 144. The 32-byte DATA aggregate and four individual vector
claims remain exact. RODATA is **44.18239%** in strict comparison because
switch target addends track the remaining code-layout differences; its table
identity and ordered referents have not changed. The function remains WIP.

The two action-84 pattern calls show a further argument-evaluation order.
After the occupancy check, retail loads the object effect ID, position,
rotation, template pattern byte, and low selector byte in that order before
`func_80034f90`. In the reset arm, it first clears the three state fields,
then loads those call inputs. The earlier C computed a `pattern_index` local
before each call; the probe hoisted its two loads ahead of the object
arguments and, in the reset arm, ahead of the clears. Spelling that same
typed pattern expression at the call sites restores both raw load sequences
without changing either call's values. Fresh tracked focused comparison is
89.1% with 329/329 CFG blocks and 180/180 branches; its isolated strict text
rises from **99.00313%** to **99.55399%** on the 7,668-byte retail body.
The 32-byte DATA stays exact and RODATA remains **44.18239%** because the
remaining switch-target addends differ. The tracked object is byte-identical
to the off-tree trial. No repository tests, lint, or full build ran.

A later action-19 retry on this updated source gives a different result from
the older negative control above. Retail loads the linked object's source
byte before clearing `unknown_38`, then stores that byte to the linked
object. Exchanging the two independent C assignments reproduces the raw
`lbu; sb zero; sb byte` sequence at function `+0x17ec` and shortens the
candidate from 7,676 to 7,672 bytes. The retained, freshly rebuilt unit has
**99.63745%** isolated strict text against 7,668 retail bytes and exact
32-byte DATA. The 956-byte jump-table RODATA is **43.396225%** because
remaining target addends move with code layout; its referents are unchanged.
The function remains WIP. No repository tests, lint, or full build ran.

A fresh safe-delinked single-unit comparison on the retained source confirms
**99.63745%** strict text, 329/329 CFG blocks, 180/180 branches, and no
withheld relocations. The first non-target difference is a retail `nop` at
function `+0x121c` between the final signed-byte load from a seven-argument
archive-transition call and its `jal`; the probe schedules that `jal` into
the load-delay gap. The next non-target residue, in action 98, reorders the
angle and signed-velocity halfword loads/stores but preserves their values.
No source-backed field, call, width, or control-flow correction follows from
these schedules, so the C body and table claims remain unchanged.

### Fresh related-unit certificate

A narrow safe delink and focused rebuild of the related map-object band used
the current source for 22 claims in nine units; every target had zero withheld
relocations. Direct strict comparison finds 17 exact claims and five WIPs:

| Unit | Strict verdict for its claims | Remaining bounded difference |
| --- | --- | --- |
| `map_cell_pattern_place` | `34f90` 97.86822%, `35194` 89.59545% | Both retain matching CFG and branches (14/14, 7/7; 51/51, 26/26). Retail/probe text relocations agree at all 11 ordered type, symbol, and site rows. The first function exchanges saved-register roles and one independent address instruction; the second uses a 40-byte retail frame against a 32-byte probe frame and different argument lifetimes. |
| `map_object_reset` | Six exact | The BSS/COMMON ownership difference remains separate from the six text claims. |
| `map_object_init_records` | `35894` exact | Its 270-byte DATA and 1016-byte RODATA are also exact. |
| `map_object_collision_query` | `36078` exact | No remaining text mismatch. |
| `map_object` | `36190` 98.56115%, `363bc` exact, `363dc` exact, `36464` 95.32258% | Both WIPs have matching CFG and branches (15/15, 8/8; 12/12, 3/3), and all 32 ordered text relocation type, symbol, and site rows agree. The first differs by one independent load/move order before `angle_within_tolerance`; the second uses opposite saved registers for object ID and height offset and a different reset-call delay-slot store. Its 68-byte RODATA is now strict exact; the older preceding-body addend displacement was stale. |
| `map_object_spawn_scatter` | Four exact | Its 84-byte RODATA is exact. |
| `map_object_vertex_world` | Two exact | No remaining text mismatch. |
| `map_object_motion` | `36b68` exact | No remaining text mismatch. |
| `map_object_action_update` | `36ed4` 99.63745% | 329/329 CFG blocks, 180/180 branches, and 252/252 ordered text relocation type/symbol rows agree; its 238 pointer rows preserve the 239-word target-equivalence relation. The four-byte text-length and pointer addend residue remains. |

The five WIPs retain their existing humane source. This certificate adds no
exact closure beyond the previously recorded unit results and makes no new
source-owner claim from address adjacency.

### Post-checkpoint map-object recheck (2026-10-02)

A fresh manifest-profile isolated comparison of eight map-object units covers
20 claims: 17 existing strict-exact controls and the three unchanged WIPs
`0x80036190` (**98.56115%**), `map_object_spawn_effect` (**95.32258%**),
and `0x80036ed4` (**99.63745%**). The reset, initialization, collision
query, scatter, vertex-world, and motion bodies remain exact. Their exact
addresses are `0x80035504`, `0x80035534`, `0x80035590`, `0x800355d8`,
`0x800356ac`, `0x800357a0`, `0x80035894`, `0x80036078`, `0x800363bc`,
`0x800363dc`, `0x800365d8`, `0x800366fc`, `0x800368b4`, `0x80036944`,
`0x800369b8`, `0x80036ad8`, and `0x80036b68`. For the action
dispatcher, focused control remains 329/329 CFG blocks and 180/180 branches;
its 32-byte DATA is exact and 956-byte RODATA is 43.396225% because text
layout shifts switch target addends. Direct relocation comparison finds
252 text and 238 RODATA rows on each side, with all 490 ordered type/symbol
pairs identical. The first different text site is row 145 (`0x1220` retail,
`0x121c` candidate); all 238 RODATA relocation sites align.
Reading the 956-byte table as 239 little-endian words gives the same
first-occurrence target-equivalence class at every position (36 classes,
including the leading literal word). Twenty-three words currently have
equal raw addends; the first differing pointer is word 4 at table `+0x10`
(`0x1d88` retail versus `0x1d8c` candidate). No pointer row needs a new
identity or destination class.

At action 98, retail adds 30 to a 16-bit velocity, tests the signed result,
and stores it in the branch delay slot. The current source stores first and
then reads the signed view. An off-tree spelling that explicitly computed
the signed new velocity before storing it emitted a SHA256-identical object
at **99.63745%** strict text. This is a scheduling residue, so that source
trial was not retained. No new exact claim or C/identity edit follows.

The action dispatcher's `0x80036e24` frame/CD-service callee was also rebuilt
as a separate direct control: **98.86364%** strict text, 5/5 CFG blocks,
2/2 branches, and all five ordered calls. Retail and candidate assign mode,
end phase, and step to different saved registers; phase arithmetic and call
arguments agree. This makes the connected screen 21 functions, 17 exact and
four WIPs, without a source-backed correction to the frame helper.

The map-object state owner remains a separate data-placement WIP. The curated
target `map_object_reset.o` defines `map_object_state` in a `0x8744`-byte `.bss`;
the current manifest probe emits a `0x8748`-byte COMMON reservation even
though the typed structure is `0x8744`. An off-tree explicit zero initializer
`= {0}` produced a `0x8744`-byte `.data` section, not retail `.bss`, while
leaving all 912 raw text bytes unchanged. A separate off-tree `-fno-common`
compiler control also emitted a `0x8744`-byte `.data` section. Neither
control reproduces the curated target allocation, so no source or profile change
was retained. The raw startup clear covers precisely `0x8744` bytes, but it
cannot establish the historical source spelling or compiler option that
placed this object in BSS.

### Floor and collision call-graph continuation (2026-10-02)

An isolated manifest-profile rebuild and direct strict comparison covers 27
additional GAME claims in eight units linked through map-object placement,
collision queries, mask updates, and the floor-item allocator. Sixteen are
already exact: `0x8002a988`, `0x8002b604`, `0x8002b7f8`, `0x8002bc18`,
`0x8002bd3c`, `0x8002bdbc`, `0x8002be9c`, `0x8002bf38`, `0x8002bfac`,
`0x8002c170`, `0x8002c1d4`, `0x8002c290`, `0x8002ce2c`, `0x8002cf40`,
`0x800314fc`, and `0x80031634`. The eleven WIPs are:

| GAME address | Strict text | Current verdict |
| --- | ---: | --- |
| `0x8002aaa4` | 57.52610% | 174/172 CFG blocks, 99/98 branches; all 49 switch rows preserve the 13 target classes. The extra retail case-`0x11` branch follows a zeroed register, while case-`0x30`/`0x32` tail placement remains different. |
| `0x8002b67c` | 94.895836% | Retail reloads the stored cache height for the shape call; source values and field widths agree. |
| `0x8002b73c` | 98.404260% | Row/column induction and saved-register assignment differ; cell write and bounds control agree. |
| `0x8002b874` | 91.5% | Player, actor, and object field sources and the common height write agree; load order differs. |
| `0x8002b9d4` | 95.379310% | Direct cache stores and result agree; probe uses a 56-byte frame against retail's 64. |
| `0x8002bfd4` | 73.912620% | Four mask-sweep calls, two axis loops, signed widths, and grid stores agree; induction and frame allocation differ. |
| `0x8002c424` | 98.299320% | 23/23 CFG and 14/14 branches; saved-register assignments differ. |
| `0x8002c670` | 91.624245% | 11/11 CFG, 4/4 branches, four rasterizer calls, and 28-byte shape data agree; two mask-state address pairs are rematerialized differently. |
| `0x8002ce68` | 65.85185% | 5/5 CFG and four ordered calls; the probe hoists three late O32 stack arguments across the free-slot call, using a 56-byte frame versus retail's 40. |
| `0x80034f90` | 97.86822% | 14/14 CFG and 7/7 branches; row-major writes agree, with register/address scheduling residue. |
| `0x80035194` | 89.59545% | 51/51 CFG and 26/26 branches; rotated two-layer copy agrees, with frame and mask-hoisting residue. |

Focused `kf try` controls on the shape dispatcher, mask sweep, and floor-item
unit confirm those structural counts and the exact floor siblings. A bounded
off-tree shape trial duplicated the case-`0x30` multiplication/result tail in
C instead of jumping to case-`0x32`'s tail. The compiled text size and
relocation-section sizes stayed unchanged, while strict text fell from
**57.52610%** to **57.251373%**; it did not reproduce the separate retail
tails, so it was discarded. The other residues above have no new independent
source fact; no C, metadata, or exact-claim change is retained.
