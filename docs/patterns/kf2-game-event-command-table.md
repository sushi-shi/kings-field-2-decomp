# GAME scene-command switch table

`func_8004678c` at `0x8004678c` dispatches command bytes `0x52..0x74` through
the 35-word table at `0x800128d0..0x8001295b`. Retail subtracts `0x52`,
checks the unsigned index against `0x22`, scales by four, loads a word from
the table, and jumps through it. Every curated pointer equals the corresponding
word in the hash-validated Japanese `GAME.EXE`; all 35 targets are aligned
basic-block starts within this function, with 21 distinct destinations.
The table's 35 `mips32_candidate` rows are now `table-reviewed`/`reviewed`.
A one-VA safe carve emitted all 35 `R_MIPS_32` rows in `.rel.rodata` and
withheld zero relocations. These are reviewed table targets, not direct-call
edges or proof of the original source spelling.

The jump table shows shared command handlers. `0x63..0x66`, `0x68`, and
`0x6a..0x6d` enter the same map-object marker scan at `0x800467f8`;
`0x6f..0x71` set offsets `0x28`, `0x2c`, and `0x30` before a shared event-state
path. `0x5a..0x5e` select five eight-byte ID lists at `0x800679a0..0x800679c7`
and join one magic-record scan. `0x53`, `0x5f..0x62`, `0x69`, and `0x6e`
enter the common no-op/exit block. These destinations explain some of the
retail CFG but do not make the existing partial C body complete.

The source now models the five ID-list arms and claims their five contiguous
eight-byte load-data records in the event-command unit. Direct retail bytes
are `07 08 09 0a ff 00 00 00`, `0e 0f 00 0d ff 00 00 00`,
`10 01 02 03 ff 00 00 00`, `04 11 05 06 ff 00 00 00`, and
`12 13 0b 0c ff ff ff ff`. Each list scans 26-byte magic records until
the first unavailable ID, sets its byte flag, allocates map-object slot
`0x15e..0x167`, and animates the object between the two camera-relative
positions with the retail `0..4096` fraction and three per-frame service
loops. The source's identity names remain address-derived, and original
data TU ownership is unresolved; this is a provisional unit ownership claim.

The focused probe reported **33.3%** listing similarity after the magic-ID
arm, up from **16.4%** before it. An isolated one-VA safe carve and direct section
comparison confirm the new `.data` contribution matches **40/40 bytes**;
the 140-byte `.rodata` jump table still differs in code-target addends because
the switch body is incomplete.

The `0x6f..0x71` transition path is also now modeled from the distinct
`0x469fc/0x46a04/0x46a0c` table entries: it spends ten attack-charge units,
waits for two resource transitions, moves the player to a map object's pose,
and queues asset-registry slot `0x181` if absent. That slot is the typed
`game_graphics_runtime.asset_registry_entries[0x181]`, whose offset is
`0x10720`, matching the retail load. After this arm the C emitted 3140 bytes;
the later switch-status form emits 3172 bytes against 3156 retail. Reordering
the case groups into the raw
retail body sequence raises focused listing similarity from **28.5% to 61.9%**
without changing behavior. Spelling the shared object-action status as a
`switch` instead of an if-chain then raises it to **66.0%**; isolated strict
objdiff now gives **82.368820%** `.text` and **100%** `.data`. The first
remaining difference is the saved-register frame/register setup, followed
by CFG and referent residues. The 35 table
entries now have explicit C actions or verified no-op behavior, but the strict
whole-function verdict remains WIP. KF1 `master`'s
`src/game/map_scripts.c` (including history commit `bf051cfa`) gives useful
typed examples of magic-record updates, map-object acquisition, and blocking
animation loops; KF1's matched switch-source ordering example also predicted
the block-order improvement. Its command topology and parameters differ, so
all kept fields, constants, calls, and loop bounds here come from KF2 retail.

Fourteen previously absent adjacent HI16/LO16 pairs inside this dispatcher
are now reviewed: map-object, player, and event-state bases at raw sites
`0x4683c..0x472a0`. Each target/addend is inside an established typed owner;
the one-VA safe carve emits all fourteen pairs with zero withheld rows. These
curations restore real source referents in the target object rather than
removing the C references to accommodate an incomplete delink.

The object-action status switch now spells `case 0` explicitly. Retail tests
status 1, then `status < 2`, then 3 and 4; the GCC 2.5.7 probe emits that
same ordered test only with the explicit zero case. The focused listing rises
from 66.0% to 66.1%, and isolated strict `.text` from 82.368820% to
83.257286% (3156 retail bytes, 3172 candidate bytes); the 40-byte `.data`
claim remains exact. A separate `status < 2` guard produced only 81.665400%
strict `.text`, and moving the common state write into three result arms
produced 78.095055%; neither source form was retained. The surviving residue
starts with one saved-register/frame difference, then body placement and
shared state-write placement; this is still WIP.

In the `0x72..0x74` map-object arm, retail subtracts the map-object array
base from the selected object pointer and divides by the 68-byte record
stride before storing a halfword object index. The previous C stored the
search result directly, which is equivalent for a valid array element but
does not explain the retail signed `div`/`mflo` or its exception checks. A
natural pointer difference reproduces that instruction sequence and raises
isolated strict `.text` to 85.107735% (3220 candidate bytes); `.data` stays
40/40 exact. `.rodata` target addends shift further while later body placement
remains WIP, so this is source-structure progress, not closure.

The retail jump table enters three short stubs for commands `0x72..0x74`,
which set control offsets `0x28/0x2c/0x30` and join one object-action body.
The same pattern occurs at `0x6f..0x71` before the resource-transition body.
Spelling the six assignments and shared joins in C reproduces both sets of
constant-load stubs. KF1's matched `source-shapes-gcc257.md` records the
same GCC 2.5.7 case-body ordering and shared-label shape; the KF2 table
targets and raw constant loads independently establish these six arms.
With the pointer difference and explicit status zero
case retained, isolated strict `.text` reaches 86.069710% (3248 candidate
bytes versus 3156 retail), and the focused listing reaches 69.9%.
The candidate's 35 table addends still differ as
body placement shifts, so `.rodata` is 2.5% and this is not exact closure.

A separate probe spelled the `0x59` distance argument as an independent
`map_object_state.objects[object_index]` expression while keeping the local
object pointer for the scan. It raised isolated strict `.text` to 86.774400%
and `.rodata` to 50.357143%, but GCC then advanced an extra induction value
by 17 and tested it against 6732. Retail instead advances two object pointers
by 68 and decrements a 395-entry counter. The duplicate expression does not
establish the original C pointer lifetime, so it was not retained.

The `0x59` map-object ID filter now spells the two accepted ranges as nested
signed comparisons: reject below 82, accept below 84, otherwise reject at
or above 97 and below 90. Retail has precisely four ordered `slti` tests
for 82, 84, 97, and 90, including the branch-delay comparison slots. The
previous inequality expression compiled one unsigned subtraction/range test
for 90..96. The nested source reproduces all four instruction forms and
raises isolated strict `.text` to 87.166030% (3256 candidate bytes);
the focused listing is 70.3%. `.rodata` addends still reflect WIP body
placement (2.857143%).

The same `0x59` scan begins with 395 remaining objects and advances two
68-byte object pointers while decrementing the counter to `-1` at the loop
backedge. A pointer-and-count `for` loop reproduces that order: the probe
loads `0x18b`, emits the two pointer increments and the decrement/`bne -1`
tail in the same sequence as retail. This is also a normal object traversal
form in KF1's matched `map_events.c`; KF2's own raw loop establishes the
count and stride here. Focused listing rises from 70.3% to 76.3% and
isolated strict `.text` to 88.043090% (3268 candidate bytes), with `.data`
still exact. The loop's saved-register assignments and table target addends
remain WIP; `.rodata` is 32.857143% in this probe.

At the `0x59` sound call, retail places `nearest + 20` in `$a1`, pointing to
the selected map object's original position. It separately stores a local
position with adjusted Y at stack offsets `40..48`, in X/Z/Y order, but does
not pass that local to `audio_play_spatial_range`. The previous C passed the
adjusted local, changing runtime behavior. Keeping the three retail-backed
stores and passing `&nearest->position` reproduces the entire raw instruction
sequence from `0x80046ea8` through the call and raises isolated strict
`.text` to 88.902405% (3268 candidate bytes); the focused listing is 77.4%.
The apparently unused local is retained because retail demonstrably writes
it, although why the original source did so is unresolved. The dispatcher
remains WIP, with body placement and jump-table addends still different.

KF1's matched map-interaction dispatcher history (`e5ed4e11`) provided a
second pointer-and-count loop lead for the `0x55` object search. KF2 retail
independently starts a 395-entry counter, keeps an object pointer and a
pointer offset by 57 bytes, decrements the counter in the object-kind
branch delay slot, and advances both pointers by 68 bytes at the backedge.
Changing the indexed C loop to a pointer-and-count traversal reproduces this
shape and raises isolated strict `.text` to 89.998730%.

That control exposed a prior semantic error: retail loads the object's
16-bit ID, scales it by the 24-byte template stride, reads byte zero of the
selected template, and compares that byte with `0xe2`. The old source
compared the object ID directly with `0xe2`. Reading
`map_object_state.templates[object->object_id].collision_kind` reproduces
the raw `lhu`, 24-byte scale, base referent, byte load, and compare in order.
With both corrections, isolated strict `.text` reaches **90.822560%**
(3288 candidate bytes versus 3156 retail); the 40-byte `.data` claim stays
exact. The template byte's existing `collision_kind` name is provisional;
the load and value are proved. This is still a WIP dispatcher, not a banked
match.

The `0x6f..0x71` transition gate and subtraction both address
`player_state + 0x18` with `lhu`/`sh`. That field is the typed
`player_state.vitals.current_mp`; the previous C charged
`attack_charge_current` at `+0x1a`, a real behavior error hidden among the
dispatcher residues. Correcting the member makes the two ordered referents
and operation width agree. The isolated strict `.text` rises slightly to
**90.835236%** (3288 candidate bytes); `.data` remains exact and `.rodata`
still reflects displaced jump-table targets. KF1 supplied no authority for
the KF2 offset; the raw KF2 loads and stores decide this correction.

The `0x58` arm's retail sequence computes one 68-byte object address, then
loads ID `+6` and state byte `+56` through that same pointer. The old C
repeated indexed array expressions; the probe emitted two separate
map-object base relocations and address calculations. Naming the selected
`KfMapObject *` once restores the shared pointer and 12 retail instructions
in their original order. Isolated strict `.text` now reaches **91.287704%**
(3276 candidate bytes); `.data` remains 40/40 exact, while `.rodata` pointer
addends remain WIP.

The `0x5a..0x5e` magic-record scan calculates a 26-byte row address once,
loads `menu_available`, and stores through the same pointer if the record is
free. The old C repeated indexed references and emitted a second
`effect_state` base pair. A `KfMagicRecord *` carried from the test to the
store restores the single retail row address. Retail also uses the result of
`lbu` directly as an integer ID; the old `u8` local induced an extra `andi`
before the `0xff` test. Using `s32` for the promoted ID recovers that width
and leaves the 26-byte index math intact. Strict `.text` reaches
**91.790880%** (3260 candidate bytes), `.data` stays exact, and the 140-byte
switch `.rodata` remains WIP at 48.571430% because code labels differ.

A read-only neighboring-call audit found a larger CFG discrepancy in those
same five magic commands. Retail's 35-entry outer jump table lands directly
on five separate `DAT_800679a0..c0` address-load stubs; four stubs jump to
one scan body and load `0xff` in their jump delay slot. The old C grouped
the commands and selected the list through an inner `switch(command)`,
adding a comparison ladder absent from retail. Explicit outer cases with
a shared `magic_action` label reproduce the five stubs, joins, and delay-slot
constant placement. The isolated strict `.text` result rises to
**93.818756%** and shrinks from 3260 to 3196 candidate bytes against 3156
retail bytes; `.data` remains 40/40 exact. Its jump-table target addends
remain different because the rest of the function has not closed.

The first object-action result switch had one more source-level join error.
Retail's status 1, 3, and 4 blocks each call its sound/notification routine,
store `event_state.state_word = 1`, then jump straight to the common callback;
the status-0/default path alone increments the search index and repeats.
The prior C broke out of the inner switch and used one common state store,
which moved two stores and their branch destinations. Sending each successful
arm directly to `invoke_callback` reproduces the complete raw prefix through
the next command stubs, including all three stores and their jump delay
slots. Isolated strict `.text` rises to **95.257286%** (3204 candidate bytes
versus 3156 retail), with `.data` still exact and `.rodata` 57.857143%.
This remains WIP until later CFG and all jump-table addends close.

The transition's first color call also had its source arms inverted relative
to retail's branch layout. Retail branches on equality to the all-`0xff`
case and falls through to the repeated `previous_value` arguments on
inequality. Spelling the C as `if (previous_value != current)` puts those
argument stores and the shared call in retail order without changing
behavior. The isolated strict `.text` result rises to **95.612170%** at the
same 3204-byte candidate size; `.rodata` remains 57.857143% and WIP.

In the same transition, retail stores the selected object's Y position into
the camera before loading its yaw, writing event state, and adding 2048 to
that yaw. Placing the yaw expression before the event-state assignment in C
restores that load/store schedule without changing the state transition.
The probe emits the retail sequence (`lw`/`nop`, camera `sw`, `lhu` yaw,
`li 1`, event-state `sw`, `addiu 2048`) and raises isolated strict `.text` to
**96.226870%** at 3204 candidate bytes; the jump-table addends remain WIP.

The `0x59` scan backedge also specifies an ordered pair of side effects:
advance the first object pointer by 68, decrement the remaining count,
compare it with `-1`, then advance the second pointer in the branch delay
slot. Reversing the C `for` increment expressions to `scan++, remaining--`
recovers all four instructions in that order. Strict `.text` reaches
**96.254750%** with unchanged candidate size and data; this is a loop-order
correction, not exact closure.

The magic animation's decay loop has one unusual but decoded CFG entry: after
the initial interpolation loop, execution skips the service calls and enters
the decrement/rotation update first. If the signed height remains positive,
it branches backward to three service calls, then falls through to the same
update. A source label on the shared update and one initial `goto` express
that fact without duplicating the body or adding a synthetic flag. The
probe now emits the retail first-loop exit (`beqz` then jump with stack-store
delay slot), the three-call service block, and the `bgtz` backedge with
`spin += 8` in its delay slot. Isolated strict `.text` rises to
**97.699620%** (retail 3156 bytes); `.data` stays 40/40 exact and
`.rodata` improves to 66.071430%, but table target addends and later paths
still differ, so the function remains WIP.

The command `0x52` result path stores literal `1` whether
`func_80047434(0x4d)` returns zero or nonzero: the nonzero branch has
`li v0,1` in its delay slot, and the zero path's jump to the shared store
has the same delay-slot load. The earlier C stored the helper's return
value, which happens to be Boolean in the currently modeled callee but did
not express the raw dispatcher write. Calling the helper in the condition
and storing literal `1` reproduces both delay slots and shared-store join.
Isolated strict `.text` rises to **98.567810%** (retail 3156 bytes),
with `.data` exact and `.rodata` still 66.071430% from displaced labels.

Both object searches initialize a 68-byte pointer and a countdown to 395.
Putting the pointer assignment before the count in the `0x55` `for` header
keeps the count setup after the actor rejection, as retail does; the compiled
text returns to the retail **3156-byte** size instead of 3152 bytes. Using
the same offset local for the `0x72..0x74` object controls and `0x6f..0x71`
transition controls matches their shared `0x28/0x2c/0x30` domain and makes
both arms use retail's saved `s1` offset register. Initializing the `0x59`
scan pointer before the nearest result, distance, and count makes GCC emit
their entire retail setup order. These are source-equivalent loop and
local-ownership corrections. At this stage the source is **99.334600%** strict
`.text` at the retail 3156-byte size and **82.142860%** `.rodata`, with
`.data` 40/40 exact. The remaining color-call join, magic-record scan
registers, and table targets keep this function WIP.

The equal-value color path was still modeled with five `0xff` channel
arguments. Retail retains the saved previous value in `$a2` on that path:
its first three arguments are `0xff, 0xff, previous_value`. Correcting that
third argument removes the candidate's extra `li a2,0xff` and reproduces the
entire first color-call join. The resulting candidate text is 3152 bytes
against retail's 3156, while isolated strict `.text` improves to
**99.493030%**; `.data` remains exact and `.rodata` is **80.357140%**.
The missing four bytes and later magic scan register/label differences remain
unattributed. The raw argument correction is retained despite the shorter
text and lower focused listing score.
