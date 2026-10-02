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
