# GAME target-candidate scorer pilot

At the initial pilot, `func_80039108` was an unclaimed 0x4c0-byte GAME
function at `0x80039108`; it is now source-claimed and remains WIP.
The source-ownership questions in this opening pilot section describe that
earlier state; the current strict source verdict appears below.
Its only proven direct caller is `actor_select_best_target`. The candidate
signature is `s32 func_80039108(KfTargetCandidate *target, s32 player_distance)`;
the first two arguments and signed result use are supported by that caller and
the body, while the full candidate-record field family remains incomplete.

Retail reads `target->type`, returns zero for `0xff`, then bounds-checks
`type - 2` against `0x82` and indexes words at `0x80011cd8`. The bounds check
proves 131 indexed words at `0x80011cd8..0x80011ee0` (0x20c bytes). Reading
every retail word yields ten unique instruction targets, all inside this
function; 114 rows select the default block at `0x8003953c`. The indexed
extent and decoded targets are proven retail facts. The table's original source
owner and its candidate relocation rows remain unvalidated. The following
word at `0x80011ee4` is zero before the next dispatch table at `0x80011ee8`;
its padding owner is unproved. The current
`data.tsv` and identity inventories still split the words into address-derived
pointer rows; a source reconstruction needs one unit RODATA claim after
ownership and delinker validation.

The nondefault target types are `0x02`, `0x03`, `0x04`, `0x05`, `0x09`, `0x0b`,
`0x0d`, `0x12`, `0x13`, `0x14`, `0x16`, `0x17`, `0x18`, `0x19`, `0x1b`, `0x70`,
and `0x84`. Repeated rows share cases; this list identifies dispatch shape,
not semantic names for the target kinds.

Several cases load an unsigned halfword at candidate-relative `+0x1a`, beyond
the current 0x16-byte `KfTargetCandidate` common-prefix view. Other cases read
both bytes and halfwords at `+0x0c..+0x10`. This supports type-specific or
larger candidate payloads, but does not yet prove one fixed record stride.
The `0x70` row returns `-1` directly; other named rows reach distance, facing,
or random-score gates before the shared return. These are case behaviors, not
confirmed gameplay labels.

The body loads `actor_state+0x93ac` as the current actor context and reads
`player_state+0xd8` position fields in several case paths. Direct calls include
`func_800157ac`, `rand`, `vector_xz_to_angle`, `angle_within_tolerance`, and
`func_80015574`. A separate `jalr` at `0x80039564` loads
`state_8017d118.active_table` from `0x8017d124`, then a pointer at table byte
offset `0x40` (slot 16). It passes the candidate in `$a0` and distance in
`$a1`; the callback target remains unresolved. The actor damage dispatcher at
`0x80039c94` uses the same active table's byte offset `0x48` (slot 18) for its
own unresolved `jalr`. That call presents an actor pointer in `$a0`, a scaled
value in `$a1`, two more register arguments, and six caller-stack words; the
destination and its source-level signature remain unproved.

No strings are referenced. The case paths perform distance checks, angle tests,
random gating, and score selection, with a shared return at `0x800395ac`.

Initial pilot verdict (historical): **WIP, unclaimed**. At that point there
was no source object or strict score.
The next concrete ownership step is validation of the switch table's source
owner and a typed candidate-record model. The indirect branch's row values
are decoded, while the callback's destination is still unknown.

A one-VA focused carve of `80039108` uses only five curated relocation rows
and withholds 28 case-path MIPS26 calls and jumps after the indirect switch;
the delinker marks them `non-reachable-code-channel`. Source matching requires
the switch table and those case edges to be curated together. This does not
promote the indirect callback destination to a known function.

The adjacent magic recipient `80039c94` is a more tractable source pilot:
its focused carve withholds no relocations and its two sourced callers pass
thirteen arguments. Retail masks the actor index to sixteen bits, selects an
actor from the 200-entry pool, and may follow signed actor `+0x22` when the
slot state is three. It then loads the actor's group and reads eight unsigned
halfwords at group `+0x20..+0x2f`, passing each in order to the exact curve
helper `80039c14`. The shared group type now models that span as
`unknown_20[8]` without assigning an unsupported gameplay name. The summed
curve result is capped at `0x68db7`, scaled by a caller halfword and 5000,
then passed to active callback-table slot 18 with ten observed arguments.
That callback target remains unresolved. Later paths select actor targets,
award player training or experience, and build motion toward a supplied
position. A contiguous source claim now follows the exact curve helper in
`actor_fixed_curve.c`; its direct call set and three reviewed BSS relocation
pairs are represented, while the decompiler candidate remains only a guide.
KF1's `combat_calculate_damage_component` and `actor_apply_damage` show a
comparable squared-difference curve and training/experience sequence. They
support the source shape, but do not identify the KF2 callback or original TU.
Retail `0x8003a170` loads byte +2 from the selected target group for the
unlinked motion divisor; the linked branch instead loads the linked actor's
group index. Retail compares the zero-extended byte using signed `slti`, so
the C source holds the loaded value in an `s32` motion divisor.
Focused comparison is **WIP, 50.3% listing**: retail/source have 72/71 CFG
blocks, 46/46 branches, and the same seven incoming return edges. The first
real structural residue is one block, followed by broad register and stack
allocation differences. The exact `80039c14` sibling remains SAME. The
slot-18 callback target and several actor field meanings remain unresolved.

## Connected actor-behavior dispatch tables

The behavior controller `func_8003c614` tests unsigned `mode - 1 <= 0x7a`
before indexing `0x80011ee8`. This proves 123 indexed words through
`0x800120d0` (0x1ec bytes). The retail words name 19 distinct internal
targets between `0x8003c7c8` and `0x8003d060`; 100 select the common tail
at `0x8003d060`. `0x800120d4` is a zero word outside this indexed extent,
before the next table at `0x800120d8`; its source-level padding owner is
unproved. Only 23 mode values select nondefault targets, from the sparse set
`0x01`, `0x02`, `0x04`, `0x07`, `0x09`, `0x0c`, `0x16..0x18`, `0x1a..0x1d`,
`0x1f..0x21`, `0x28`, `0x6c`, `0x6e`, `0x70`, and `0x78`, `0x79`, `0x7b`.
The controller remains unclaimed because its complete mode and record contract
is unresolved.

Each table base has one decoded incoming address reference, from its owning
dispatcher. The three `lui/addiu` pairs at `80039158`, `8003c7ac`, and
`8003d350` are reviewed target references, but the indexed words remain
candidate table data without a source claim. Focused one-VA carving withholds
67 control relocations after the indirect switch in `8003c614`, and 249 after
the larger switch in `8003d184`.
These counts are an evidence gap in the current curated model, not evidence
that the case bodies are dead code.

The larger actor update dispatcher `func_8003d184` reads actor byte `+0x0e`,
accepts values through `0xf0`, and indexes 241 words at
`0x800120d8..0x80012498` (0x3c4 bytes). All decoded words point inside its
`0x8003d184..0x8003f610` body, with 28 distinct targets; 212 select
`0x8003f3ac`. The next word at `0x8001249c` begins an independently modeled
effect table. Nondefault actor-byte values are `0x00..0x05`, `0x09..0x1e`,
and `0xf0`; `0x0c`/`0x10` and `0x0d`/`0x11` each share a target. Its two
`jalr` sites at `0x8003d5e0` and `0x8003f3c0` load
`state_8017d118.active_table` slots 19 and 17 respectively; neither callback
destination is proved. Slot 19 receives the current actor pointer in `$a0`;
slot 17's call site does not freshly populate argument registers. These indexed
extents and row values are retail facts,
but neither table has a validated original source owner or source claim.

## Current GAME source comparison

The earlier unclaimed verdict above predates the current
`game.actor_candidate_score` source and its 131-word `RODATA` claim. The
current candidate record has a checked halfword/byte union at `+0x0c`.
Retail case `0x19` loads the high byte with `lbu` at `0x80039430`, then
shifts it four places for `angle_within_tolerance`. The prior C expression
shifted the full halfword and compiled as `lhu` plus `srl`; using
`word_0c.bytes.high` emits `lbu a2,13(s0)` at compiled offset `+0x2f4`.
The same union preserves halfword reads in the distance cases and the low
byte read in the facing case.

An isolated direct objdiff of the same 1,216-byte target shows the source-only
old halfword expression at **85.72369%** strict `.text` and the retained byte
expression at **89.0625%**. Its 524-byte `.rodata` remains 37.309162%; the
case-layout difference still changes jump-table targets. Focused `kf try`
remains DIFF, 35.1% listing: the first mismatch is saved-register assignment
after the prologue, and later switch-case placement differs. The direct call
set and reviewed address pairs are unchanged. The indirect callback-table
slot 16 and switch `jr` remain unresolved as before. No exact claim follows
from the improved strict score.

The affected `game.actor_fixed_curve` exact helper `0x80039c14` remains
focused SAME; its sibling `0x80039c94` remains WIP at 62.1% focused.
`game.event_target_stream` remains WIP at 90.5% focused. The actor behavior
dispatcher uses the same typed `+0x0c` view and is still WIP at 18.2% focused.

The connected 26-function target/caller/control pass below used focused
per-unit builds after the shared field migration. “Exact control” means the
prior strict report was 100% and the current focused listing, including
ordered relocations, is SAME. Scores on WIP rows are current focused listing
similarity except for the scorer's direct strict result above.

| GAME VA | Verdict |
| --- | --- |
| `0x80038cc8` | Exact control, SAME. |
| `0x80038d04` | Exact control, SAME. |
| `0x80038dc4` | Exact control, SAME. |
| `0x80038e38` | Exact control, SAME. |
| `0x80038efc` | Exact control, SAME. |
| `0x80038f20` | Exact control, SAME. |
| `0x80038ff0` | Exact control, SAME. |
| `0x80039048` | Exact control, SAME. |
| `0x80039080` | Exact control, SAME. |
| `0x800390d0` | Exact control, SAME. |
| `0x80039108` | WIP, 89.0625% strict `.text`; 35.1% focused listing. |
| `0x800395c8` | Exact caller control, SAME. |
| `0x800396c4` | Exact control, SAME. |
| `0x80039710` | Exact control, SAME. |
| `0x80039758` | Exact control, SAME. |
| `0x800397a8` | Exact control, SAME. |
| `0x800397d8` | Exact control, SAME. |
| `0x80039804` | Exact control, SAME. |
| `0x8003983c` | WIP, 87.0% focused. |
| `0x80039b58` | WIP, 79.2% focused. |
| `0x80039c14` | Exact sibling control, SAME. |
| `0x80039c94` | WIP, 62.1% focused. |
| `0x8003f610` | Exact sibling control, SAME. |
| `0x8003f7ec` | WIP, 93.8% focused; sentinel setup and `addu` operand order. |
| `0x8003f860` | Exact sibling control, SAME. |
| `0x800462bc` | WIP, 90.5% focused; first instruction residue is byte-load timing. |

For `0x80039b58`, retail keeps both the actor base and an actor-state field
pointer live through its 200-slot scan. A source-only trial with an explicit
second pointer preserved behavior but worsened focused similarity from 79.2%
to 67.3%, and the compiler still did not use retail's `lbu -7(s1)` group-index
form. It was discarded; the retained source has one actor cursor. For
`0x80039c94`, the eight direct curve calls and typed stack halfwords already
match the retail call set; its 72/71 block residue remains at the final
linked-actor target-type guard. There is no supported source correction from
this review.

## Group-effect switch follow-up

`game.actor_group_effects` at `0x8003c614` remains WIP. Its 123-word retail
switch table at `0x80011ee8` maps kind `0x7b` to `0x8003c7c8`, where the
second script word is loaded and a nonzero value branches directly to the
common effect path at `0x8003c7ec`. A zero value falls through the spatial
audio call at `0x8003c7d8`; kinds `7` and `0x20` enter that audio path
directly. The retained source now puts the `0x7b` guard before the audio
case and uses one common effect label. Focused listing moved 26.9% to 27.1%,
with retail/compiled CFG blocks 45/48 to 45/47 and branches 15/18 to 15/17.

The same retail table sends kinds `9` and `0x21` to `0x8003c9d4`, where the
call to `func_80040308` receives an extra stack argument `0xfe` at `20(sp)`.
Kind `0x16` enters `0x8003ca8c`, which calls the same effect routine without
that argument. Their formerly shared C case hid this call-argument difference;
the retained source separates the cases. This semantic split alone yielded
26.7% focused listing DIFF, down from 27.1%; the raw call-site evidence
governs the source.

The decoded jump table and raw body addresses also support the lexical case
order `0x7b`, `0x79`, `4`, `0x28`, `9`/`0x21`, `0x18`, `2`, `0x16`, `0x17`,
`0x6c`, `1`/`0x1c`, `0x1a`/`0x1b`, `0x0c`, `0x78`, `0x6e`, `0x70`,
`0x1d`/`0x1f`. A source-only reorder to this sequence raised focused listing
similarity from 26.7% to 41.1%, and the compiled `0x7b` entry still
directly loads the second script word before the audio/common path. Retail
sets its effective kind register to `0x20` in the delay slot of the
`0x7b` guard branch at `0x8003c7d4`. Modeling that as a local `kind = 0x20`
before the guard, instead of a ternary at the later effect call, raised the
focused listing to 41.2% and narrowed retail/compiled CFG counts to 45/46
blocks and 15/16 branches. The final six-iteration trajectory loop at
`0x8003cf54..0x8003cfdc` decrements its counter in the delay slot of the
trajectory-result branch, then skips prediction when the decremented count
is zero. Expressing that order as a `do` loop with an explicit decrement
after the solver preserves six iterations and removes the probe's extra
branch. It first raised focused listing to 54.8% and aligned CFG at 45/45
blocks and 15/15 branches.

Retail kind `0x79` at `0x8003c8ec` and kind `4` at `0x8003c928` both jump
to the common constructor call at `0x8003ccc8`, after setting direction and
the extra stack words. A shared C call now gives kind `4` the retail `-1`,
`0x400`, and `1` varargs and kind `0x79` its computed nonnegative travel
time, `0x400`, and `1`. Raw kinds `9`/`0x21`, `0x18`, `0x16`, `1`/`0x1c`,
and `0x1a`/`0x1b` also jump to that same constructor block after writing
their direction and stack words; they now share the same C call, with the
retail `0xfe` duration for kinds `9`/`0x21` and `-1` for the others. This
preserves 45/45 blocks and 15/15 branches. The two spawn arms at
`0x8003cd88..0x8003cdb0` and `0x8003ceac..0x8003ced8` store only actor
position X/Y/Z at offsets `+44`/`+48`/`+52`. The source now copies those
three words individually instead of copying the full 16-byte `VECTOR`,
which would also write the unobserved fourth word. Focused listing is
**49.4%**; isolated direct objdiff reports **80.99701%** strict `.text`
(`2672` retail versus `2744` compiled bytes) and **37.80488%** `.rodata`
(`492` bytes on each side), up from 7.9268293% before the shared call.
The lower fuzzy text score than the pre-shared-call probe is outweighed by
seven proved jumps to one retail call, a closer switch table, and the
three-word spawn-position stores. The first remaining CFG successor difference is the
kind-`0x79` nonnegative travel-time branch target; frame and register
assignments also differ. `.rel.text` sizes are 728/704 bytes and ordered
relocation equality is unproved. This function remains WIP; exact
neighboring actor helpers were unaffected.

The connected 22-function actor-group call/control pass used ten quick,
focused GAME unit builds. `SAME` below means current listing equality, not
a fresh isolated strict section audit. The WIP scores are focused listing
similarities; the group-effect strict object result is given above.

| GAME VA | Relation to group-effect path | Focused verdict |
| --- | --- | --- |
| `0x80013f50` | Spatial audio callee | SAME |
| `0x80015034` | Pitch/yaw vector callee | SAME |
| `0x80015104` | Rotation callee | SAME |
| `0x80015188` | Vector scale callee | SAME |
| `0x80015468` | Two-axis length callee | SAME |
| `0x800154a8` | Three-axis length callee | SAME |
| `0x800154fc` | Pitch/yaw calculation callee | SAME |
| `0x8001584c` | Fixed interpolation callee | SAME |
| `0x8001586c` | Adjacent interpolation control | SAME |
| `0x800158b4` | Adjacent interpolation control | SAME |
| `0x80015918` | Trajectory solver callee | WIP, 85.2% |
| `0x80015bc8` | Adjacent trajectory control | SAME |
| `0x80015ce0` | Predicted target callee | SAME |
| `0x80038cc8` | Actor pool callee | SAME |
| `0x80038e38` | Group initializer callee | SAME |
| `0x80039758` | Group target selector callee | SAME |
| `0x8003c000` | Vertex-position callee | SAME |
| `0x8003c10c` | Adjacent position control | SAME |
| `0x8003c220` | Adjacent position control | SAME |
| `0x8003c3e0` | Direction solver callee | WIP, 95.4% |
| `0x8003c614` | Group-effect switch | WIP, 49.4% |
| `0x80040308` | Effect constructor callee | WIP, 28.0% |

The direction solver `0x8003c3e0` has the retail call set, typed
`player_state+0xe8` referent, and a matching 80-byte frame. Its current
focused diff consists of an `a2`/`v1` register assignment exchange across
the yaw-error fraction calculation at retail `0x8003c498..0x8003c4d8`;
the three exact siblings in `actor_group_position.c` remain SAME. No source
fact justifies a register-steering edit, so this WIP is unchanged.

The trajectory solver `0x80015918` has matching retail/compiled CFG counts
(41 blocks, 24 branches, one return), the same calls to `SquareRoot0` twice
and `vector_xz_to_angle` once, and its first difference is the destination
register of the second discriminant subtraction at retail `0x800159d8`.
Retail checks positive longer-time before rejecting nonpositive shorter-time
at `0x80015a70..0x80015a80`. A temporary C rewrite expressing that order
kept the focused listing at 85.2% and both exact siblings SAME; it was
discarded because it did not explain the remaining register assignments.

## Connected effect-constructor switch

The same group-effect switch calls `func_80040308` in GAME at `0x80040308`.
Its 123-word retail jump table at `0x8001249c` has 62 distinct in-body
targets. The current C has 48 case-body groups and 68 explicit kind labels;
kind `6` and `102` still have unresolved buffer/timer ownership and only
placeholder bodies. Decoding the retail target words gives a concrete body
sequence, beginning kinds `7`/`49`, `32`, `4`, `28`/`1`, `26`, `27`, and
`111`, and ending `109`, `120`, then default. A source-only reorder of the
existing case-body groups to those retail addresses preserves each case
body and label, without inventing the missing kinds.

The retained case order moves focused listing similarity from 11.4% to
27.9%. More decisively, isolated direct objdiff on the 5,092-byte retail
function raises strict `.text` similarity from 38.156322% to 69.41712%;
compiled `.text` is 4,456 bytes. The 492-byte `.rodata` switch table
falls from 21.036585% to 11.99187% because the case targets still differ;
both objects have equal `.rel.text` and `.rel.rodata` section sizes but
ordered relocations remain unproved. Focused CFG is retail/compiled 116/103
blocks and 22/23 branches before the next correction. Retail kind `28` at
`0x8004058c` sets three scales to `0x800`, then falls through to kind `1`
at `0x8004059c`. Replacing the shared C arm's `if (kind == 28)` with the
same fall-through structure raises focused listing to **28.0%** and reduces
the compiled CFG to 101 blocks and 22 branches. The final isolated direct
objdiff is **69.49961%** `.text` (4,444 compiled bytes) and 16.666668%
`.rodata`. This is a body-layout improvement, not closure,
and the unresolved live variants and frame/argument allocation remain WIP.

The two placeholder paths were reviewed without a source claim. Kind `6`
starts at `0x800409e4`, uses `DAT_8006d704` as a modulo-four slot counter,
and computes `0x801d9628 + slot * 576`; it then copies 24 entries of 24
bytes each from the current record into that ring slot. The implied four-slot
extent is 2,304 bytes, but `0x801d9628` is outside the GAME load and lacks a
proved complete owner. Kind `102` starts at `0x80041204` and reads/writes
`0x8009a5a8`, a timer word within a mixed Sony malloc/Psy-Q CD neighborhood.
The owner of that word is also unresolved. Neither raw address is replaced
with an invented global or build placement.

Kind `54` enters `0x80040bcc` with render ID `0x30`, jumps to shared stores
at `0x80040bd8`, while kind `11` enters `0x80040bd4` with ID `0x11`. A
temporary C shared-label rewrite of the current grouped ternary retained
28.0% focused listing similarity but changed the compiled CFG from 22 to
21 branches against retail's 22. Isolated `.text` increased only from
69.49961% to 69.58601% while `.rodata` fell from 16.666668% to 9.857724%.
The trial was discarded; the retained grouped source still models the two
render IDs but does not yet reproduce their raw control layout.

The analogous kind `16` versus `14`/`19` duration choice enters retail
`0x80041378` versus `0x80041384`, with a shared sound path at
`0x8004138c`. A temporary shared-duration label emitted only 21 compiled
branches and 100 blocks against retail's 22 and 116, with no focused score
change from 28.0%; it was also discarded.

## Actor motion and collision follow-on

A separate 11-function GAME actor-state/motion family was checked with two
focused quick unit builds. `SAME` means current listing equality; no new
strict exact claim is made from the focused control alone.

| GAME VA | Current verdict |
| --- | --- |
| `0x8003b33c` | SAME, collision motion sibling |
| `0x8003b520` | SAME, trajectory motion sibling |
| `0x8003b5bc` | SAME, state-step sibling |
| `0x8003b5d0` | WIP, 66.5% at this historical pass; see current verdict below |
| `0x8003b9a4` | SAME, motion sibling |
| `0x8003bae4` | SAME, motion sibling |
| `0x8003bba0` | SAME, motion sibling |
| `0x8003bcd0` | SAME, motion sibling |
| `0x8003bd40` | WIP, 68.2% |
| `0x8003be38` | SAME, motion sibling |
| `0x8003bf74` | SAME, motion sibling |

At this historical pass, `0x8003b5d0` had matching retail/compiled CFG
counts (40 blocks, 21 branches), the same five direct calls and
actor-state/collision-cache referents, but the dispatch tested state `0x20`
first in retail and `0x10` first in the probe. The later source correction
made state `0x20` first; its current 40/39 CFG result is recorded below.
`0x8003bd40` has matching CFG counts (9 blocks, 5 branches) and the same
three direct calls. Its first difference is the register/address-load order
around the actor position and yaw calculation. Neither WIP received a
source edit in this pass.

## Actor target and damage helper follow-on

An 18-function GAME helper pass followed the actor target/home call family
through the player-damage sibling unit. `SAME` denotes a focused listing
match, not a new isolated strict object claim.

| GAME VA | Current verdict |
| --- | --- |
| `0x800157ac` | SAME, random scalar helper |
| `0x800157f8` | SAME, random scalar sibling |
| `0x80038d04` | SAME, home-position setter |
| `0x80038dc4` | SAME, actor group defaults |
| `0x80038efc` | SAME, home wrapper |
| `0x80038f20` | SAME, home wrapper sibling |
| `0x80038ff0` | SAME, preparation helper |
| `0x80039048` | SAME, preparation sibling |
| `0x80039080` | SAME, actor pool clear |
| `0x800390d0` | SAME, set target |
| `0x800395c8` | SAME, target selector |
| `0x800396c4` | SAME, target-distance helper |
| `0x80039710` | SAME, target finder |
| `0x8003a318` | WIP, 97.5% focused / 99.86911% strict |
| `0x8003a614` | WIP, 81.2% focused / 96.91011% strict |
| `0x8003a778` | strict 100%, damage-unit sibling |
| `0x8003d084` | SAME, spatial sound helper |
| `0x8003d0e8` | SAME, spatial sound sibling |

The two WIP damage functions retain their retail call sets and ordered
validated actor/player-state referents. At `0x8003a318`, retail and probe
both have 26 CFG blocks and 13 branches; the first differing instructions
load the `amount_and_flags` word and `falloff` halfword from stack into
opposite temporary registers. The later use of those values follows that
assignment. At `0x8003a614`, both have six blocks and three branches, but
retail separately loads the `player_state.camera_position.vz` address through
HI16/LO16 while the probe reuses a previously computed typed
`player_state.camera_position` base. Its first difference also permutes
three independent argument shifts and saved-register assignments. The source
already expresses the correct fields, widths, calls, and branch conditions;
no source-backed change was retained for either residue. The adjacent
`0x8003a778` is 100.0% in an isolated direct native objdiff (636/636
function bytes). The two WIPs have equal target/probe sizes of 764 and
356 bytes; their direct strict values are listed above. The full unit's
1,756-byte `.text` section reports 99.31663%.

The linked seven-function actor animation/collision-motion unit adds six
focused SAME controls and one WIP. It was checked with a quick focused GAME
build after the source/history and retail call neighborhood review. Isolated
direct native objdiff reports all six controls at 100.0% function similarity
with equal target/probe sizes of 360, 344, 220, 52, 92, and 48 bytes,
respectively. The `0x8003ae50` function remains 99.31746% strict by direct
objdiff (1,260 bytes on both sides), and the whole 2,376-byte unit `.text`
is 99.63805%; no exact claim is made for that WIP.

| GAME VA | Current verdict |
| --- | --- |
| `0x8003a9f4` | strict 100%, actor collision search |
| `0x8003ab5c` | strict 100%, related collision search |
| `0x8003acb4` | strict 100%, current-actor binder |
| `0x8003ad90` | strict 100%, wrapped phase advance |
| `0x8003adc4` | strict 100%, clamped phase advance |
| `0x8003ae20` | strict 100%, phase crossing predicate |
| `0x8003ae50` | WIP, 99.2% focused / 99.31746% strict |

At `0x8003ae50`, the first visible difference is the masked collision angle
at retail `0x8003b0fc..0x8003b10c`: the retail code computes the chosen
`±1024` angle in `v0` and masks it into `s1` before an independent
`mult s3,s3`; the probe computes the chosen angle in `a1` and schedules the
mask after the multiply. Its source already masks before the sine/cosine
uses. No independent source fact supports an instruction-order edit, and all
six neighboring exact listings remain unchanged.

The related KF1 player-collision code masks the whole conditional angle
expression. A source-only KF2 probe using that spelling preserved the six
exact controls but inserted an extra `move s1,a1` after the multiply and
lowered focused similarity to 91.7%; it was discarded.

The nearby two-function fixed-curve/damage unit remains one focused SAME
(`0x80039c14`) and one WIP (`0x80039c94`, 62.1%). A temporary probe removing
the `linked` pointer's zero initializer in the WIP shrank its frame from
168 to 160 bytes and lowered the focused result to 58.2%; it was discarded.
The retained source initializes the pointer and does not invent an actor
record or a new global to account for the remaining register layout.

## Current actor target/math strict controls

After the shared actor `+0x72` field view was synchronized, eight focused
GAME object rebuilds and direct strict per-unit comparisons covered 17 claims.
Eleven are exact: `0x80015bc8`, `0x80015ce0`, `0x800396c4`,
`0x800397a8`, `0x8003c000`, `0x8003c10c`, `0x8003c220`,
`0x8003d084`, `0x8003d0e8`, `0x8003f610`, and `0x8003f860`.

| GAME address | Direct strict verdict | Remaining evidence limit |
| --- | ---: | --- |
| `0x80015918` | WIP, 95.49419% | Discriminant and midpoint/time register assignment; 41-block trajectory CFG and calls agree. |
| `0x80039108` | WIP, 89.06250% | Shared `word_1a.value` access is retained; the candidate scorer still has broad case-layout residue. |
| `0x8003983c` | WIP, 99.19598% | Actor/current-chance register assignment with supported calls and referents. |
| `0x80039b58` | WIP, 90.95744% | Actor base and actor-state field register lifetimes. |
| `0x8003c3e0` | WIP, 99.64539% | Yaw-error fraction register assignment; three siblings remain exact. |
| `0x8003f7ec` | WIP, 85.86207% | Sentinel setup and independent pointer-add operand order; neighboring scan/load bodies remain exact. |

These are direct strict results, separate from the focused-listing percentages
above. KF1 shapes and current retail control/referent checks supplied no new
source fact for these residues, so this pass retained no actor source edit.

For `0x80039108`, direct objdiff exposes 131 `R_MIPS_32` rows in the
524-byte switch table on each side. Each table has ten distinct in-body
targets, and canonicalizing target offsets by their first occurrence gives
the same class at all 131 indices. The table's low byte-level similarity
comes from changed body offsets, not a missing case label or a different
case-to-body mapping. The source body remains 52 bytes shorter than retail
(1,164 versus 1,216 bytes), so the text-layout cause is still open; no table
row was rewritten to improve a score.

A fresh focused rebuild after the shared actor-tail view changes preserves
`0x80039108` at 89.06250% direct strict text and its sole proven caller
`actor_select_best_target` (`0x800395c8`) at 100%. At the first structural
gap, retail case 9 has its own `vector_xz_to_angle` and
`angle_within_tolerance` sequence at `0x80039260..0x8003929c`; the current
object joins that equivalent angle check with the later case 11 sequence,
omitting 15 instructions there. The raw case-9 success and failure edges,
including the `rand` gate, are represented by the C. No KF1 analogue or
retail caller establishes a different condition or field width, so the source
keeps its defined zero score and the separate retail call placement remains
an unattributed source/codegen residue.

Later focused follow-up: spelling case 9's failed tolerance as an early exit
and its success as a jump to the shared score tail keeps that retail call pair
separate. The source still has the same test, outputs, and callback behavior.
The pinned object now has the retail 0x4c0-byte text extent, five separately
ordered angle/tolerance call pairs, and 59/59 CFG blocks; its 36 branches
remain one short of retail's 37. Isolated direct objdiff rises from the cached
89.06250% to **93.30592% strict text**, while focused listing is 34.0%.
All 64 text relocations and 131 switch-table relocations retain their ordered
kinds and referents. The 131 pointer rows retain all ten target equivalence
classes, with 120 addends now byte-identical. The eleven remaining addend
rows belong only to types `2/3/22` (`+0x42c` retail versus `+0x4a0` probe),
`4/18/23/24/132` (`+0x1a0` versus `+0x19c`), `5/13` (`+0x6c` versus `+0x70`),
and `9` (`+0x110` versus `+0x114`). Retail branches directly from
case 9's successful tolerance test to the score tail; the probe still shares
one post-call Boolean branch with the later indirect callback, so this remains
WIP. Reversing the C test into a positive branch restored the old 59/58 CFG
and was discarded. Moving the zero-score initialization into every switch
arm reached 93.75% strict text but moved most switch-pointer addends, dropping
`.rodata` similarity to 38.93%; that repetitive source was also discarded.

An isolated follow-up confirmed the specific branch merge. Retail case 9
branches from its tolerance result directly to the score tail and jumps to
the return path on failure. The current object instead jumps to the Boolean
branch after the indirect callback. Assigning the tolerance result to the
existing `score` local emitted the retail 37 branches, but lowered strict
text from 93.30592% to 93.1579% and the 131-row table's `.rodata` similarity
from 94.75191% to 37.5%; it was reverted. Directly returning `score` on
the callback failure emitted the baseline object, while returning it on
case-9 failure collapsed the candidate to 58 CFG blocks. GCC 2.5.7 with
`-fno-cse-skip-blocks` emitted a byte-identical object to the baseline, so
that flag does not explain this branch sharing. The unchanged source retains
the separate case-9 angle/tolerance calls and all table target classes.

A later raw-tail review found that type `27` falls through to the same
zero-score block as types `2`, `3`, and `22` when its distance is below the
candidate bound. Retail's type-`27` test at `+0x418` branches to the shared
scoring path on success; the failed path enters the jump at `+0x42c`, whose
delay slot clears the score. The source now expresses this fallthrough and
explicit zero assignment. A focused build remains DIFF with 59/59 CFG blocks
and 37/36 retail/probe branches. Isolated strict `.text` improves from
93.30592% to **93.93092%**, and `.rodata` from 94.75191% to **96.183205%**.
The 64 ordered text relocations and 131 table relocations still match by kind
and referent. All 131 pointer rows retain the exact ten target classes; 123
addends are byte-identical, up from 120. The remaining eight differ by four
bytes at types `4/18/23/24/132`, `5/13`, and `9`. The case-9/callback branch
sharing and saved-register assignment remain unattributed codegen residue, so
this function is still WIP.

A fresh off-tree test after that type-27 correction reversed case 9's
equivalent tolerance guard to branch positively to `score_target`, as retail
does at body `+0x190`. The pinned probe instead moved the case bodies and
switch targets farther away: isolated strict `.text` fell from **93.93092%**
to **89.68750%**, and `.rodata` from **96.183205%** to **37.02290%**. The
source already has the separate case-9 calls and correct success/failure
behavior, so this trial was discarded; the existing guard remains WIP.

## Magic recipient linked-actor pointer (2026-10-02)

GAME `0x80039c94` had a dead `linked = 0` initializer. The pointer receives
the indexed actor before either of its guarded uses; the intervening angle,
vector, square-root, and scale helpers write only their explicit local
outputs, not the actor's link flag. Retail's initial slot-state branch at
`0x80039d1c` has a `nop` delay slot, while the former candidate placed
`move s8,zero` there for that initializer. Removing it improves fresh isolated
strict text from **91.85132%** to **95.41007%**. The exact `0x80039c14`
fixed-curve sibling remains **100%**, and the focused object is byte-identical
to the off-tree trial. The candidate frame becomes 160 bytes against retail's
168, so the remaining layout is still WIP; no padding or register carrier was
added. Verification used only a focused unit build and isolated strict compare.

At the `update_motion` linked-actor arm, retail forms one `actor_state` base
and uses it for both the actor array and target-group array. The previous C
spelled those as separate global expressions and emitted two address pairs.
A local typed `KfActorStateGame *` scoped to that arm restores one base for
both complete-object fields. The candidate function's relocation inventory
falls from 38 to the retail **36** rows, with all types and referents aligned
in order. Fresh isolated strict text rises further to **96.755394%**; the exact
fixed-curve sibling remains 100%. The 160-versus-168-byte frame and residual
register/branch layout are still WIP. The retained focused object is
byte-identical to the off-tree trial.

The next raw width check resolves that frame gap. Retail loads the caller's
`amount` stack word with `lhu` at entry, stores the halfword at local `+0x48`,
and reloads that local for the applied amount. Declaring the parameter `u16`
instead of `s32` reproduces the 168-byte frame. The candidate body is still
1,660 bytes against retail's 1,668-byte claim.
Isolated strict text rises from **96.755394%** to **97.84173%**; the exact
`0x80039c14` sibling remains 100%. All 36 callee relocation rows retain their
ordered kinds and referents. The three disjoint caller declarations and the
curated signature were synchronized; their focused strict results remain
unchanged, including ten exact functions in `game.effect_update`. The active
effect-dispatch owner synchronized the fourth caller declaration and confirmed
its focused strict result remained unchanged. The
remaining actor-curve mismatch is WIP; no frame padding was introduced.

Retail keeps one `remaining` value across the depleted and target-scan arms:
it clears that value on depletion and stores the same value to actor `+0x1a`
after either arm (or in the scan-success jump delay slot). The source now
expresses that shared store, rather than a separate literal-zero store in
the depleted arm. It preserves all branches' state updates and improves
isolated strict text again to **98.11751%**, with the fixed-curve sibling at
100%. The tracked focused object matches the off-tree trial byte-for-byte.
The retail zero assignment is scheduled before its depletion guard whereas
the candidate puts it in that guard's delay slot; the remaining scan-loop
constant/register schedule is still WIP.

## Group-effect player-field referents (2026-10-02)

In GAME `0x8003c614` kind `0x78`, retail rematerializes three signed-low
addresses for `player_state.camera_position` X, Z, and Y after the two `rand`
calls. The earlier source used a pointer kept from function entry, so the
candidate lacked all three address pairs at this call site. Spelling those
three typed fields directly restores the retail referents and raises the
candidate `.rel.text` inventory from 83 to 89 rows against retail's 91.
An off-tree isolated comparison moves strict `.text` from **85.93563%** to
**86.041916%**. The constructor call arguments and two-call kind-`0x78`
sequence remain unchanged. The missing two rows are not assigned by this
field correction; four raw `func_8003c3e0` call sites still converge in the
candidate, while retail emits them separately. Kind `0x79` computes and
clamps its travel time before joining the retail shared constructor tail,
but GCC spills its value to a local slot and joins a different pre-call
block in the candidate. No supported signature, field, or condition change
was found for that separate-call placement.

### Connected actor-group controls

A fresh isolated, focused pass covered eighteen functions in eleven actor
units connected by the group-effect actor state, target, or animation path.
These are direct strict function results, not broad-build certificates:

| GAME function | Strict text verdict |
| --- | ---: |
| `actor_pool_find_free` | 100% |
| `actor_initialize_from_group` | 100% |
| `actor_prepare_and_initialize`, `func_80039048` | 100% each |
| `actor_pool_clear` | 100% |
| `actor_find_target_of_type` | 100% |
| `actor_select_target_type_in_own_group` | 100% |
| `actor_reset_target_and_reselect` | 100% |
| `func_800397d8`, `func_80039804` | 100% each |
| `func_8003a9f4`, `func_8003ab5c`, `actor_bind_current` | 100% each |
| `actor_advance_animation_wrapped`, `actor_advance_animation_clamped`, `actor_animation_crossed_phase` | 100% each |
| `func_8003ae50` | WIP, 99.31746% |
| `func_800460a0` | WIP, 99.268295% |

The two WIPs retain their documented register/schedule residues: the actor
phase advance has matching calls and CFG, and the phase seeker exchanges
saved registers for step and half-step with matching calls, widths, and CFG.
No source edit was supported. The `actor_pool_clear` function's text is exact,
while its separately audited 37,836-byte BSS claim is absent from the
candidate object; this is not an initialized-data mismatch or a reason to
change the exact function body.

### Vector and effect downstream controls

The next connected pass rebuilt 23 vector/trajectory functions across six
small GAME units. All thirteen `game.vector_math` functions, both
`game.vector_actor_helpers` functions, `vector_distance_to_point`,
`func_80015698`, and all three `game.actor_fixed_interpolation` functions
are direct strict **100%**. In `game.actor_trajectory_math`, `func_80015bc8`
and `func_80015ce0` are 100%; `func_80015918` remains **95.49419%** with
its previously verified 41-block CFG and two square-root plus one angle call.
The remaining discriminant register schedule supplies no typed-field or
control-flow correction.

Sixteen downstream effect functions in ten units were also freshly focused
and direct-strict checked. `effect_play_spatial_sound`,
`effect_rotate_scale_offset_y`, `func_8004177c`, `func_8004195c`,
`func_80041b14`, `func_80041cd0`, both `game.effect_spawn_zero_direction`
functions, both `game.effect_spawn_motion` functions, all three
`game.effect_scatter` functions, and all three `game.effect_reset` functions
are text **100%**. The scatter candidate defines `DAT_801c7068` as an
8-byte COMMON symbol against a retail 8-byte `.bss` claim. Reset's
`effect_state` candidate is 10,896-byte COMMON against a retail 10,892-byte
`.bss` claim (the proven C type is 10,892 bytes); the extra four bytes are
consistent with COMMON allocation rounding. An isolated source-only `effect_state = {0}`
control put 10,892 bytes in initialized `.data`, not `.bss`, and lowered the
three exact text scores to 99.166664%, 99.09091%, and 99.68254%. It was
discarded. The existing COMMON/BSS owner and compiler-attribution question
remains separate from these exact function bodies.

### Player damage and collision caller controls

A third focused, direct-strict pass checked nineteen connected GAME player
functions in twelve units around actor damage, movement, collision response,
and magic dispatch. Each function has a final verdict from the current
isolated objects:

| Function(s) | Strict text verdict |
| --- | ---: |
| `func_80024498`, `player_cap_status_components`, `func_800248a8`, `func_80024ca4` | 100% each |
| `func_80026330`, `player_has_power_and_magic_60` | 100% each |
| `func_80027928`, `func_80027988` | 100% each |
| `func_800279cc` | WIP, 98.23967% |
| `func_80027f78` | WIP, 95.91228% |
| `func_80028224`, `func_8002851c` | 100% each |
| `func_8002897c` | WIP, 88.57143% |
| `func_80028998`, `func_800316c8` | 100% each |
| `func_8002722c` | WIP, 99.09091% |
| `player_move_horizontal` | WIP, 89.8524% |
| `func_80026498` | 100% |
| `func_8002665c` | WIP, 98.67857% |

The six WIPs retain their previously documented player-state base reuse,
stack argument, and saved-register residues with matching known calls and
field widths; this pass found no new semantic correction. The initialized
32-byte damage, 28-byte apply-damage, and 52-byte magic-dispatch RODATA
claims compare exactly. The player-select-magic unit's 100-byte table has a
four-byte body-layout addend difference, not a changed table identity. No
source or data-owner claim was altered from this control batch.

The group-effect switch table was independently checked as 123 relocatable
pointer words on each side. Both have 19 distinct in-body target classes, and
all 123 indices select the same class in retail and candidate order. Only one
raw addend word is byte-identical because the case bodies occupy different
offsets. This is a body-layout gap, not evidence to change any table row.

For the separate actor-state allocation question, an off-tree
`-fno-common` compile of exact `actor_pool_clear` changes its 37,836-byte
tentative definition into 37,836 bytes of initialized `.data`, whereas the
retail target owns 37,836 bytes of `.bss`. The default candidate is a
37,840-byte COMMON symbol. Thus this flag does not establish the missing
BSS source mechanism; no profile or owner claim was changed.

### Graphics and TMD residual controls

Eleven further GAME WIPs in nine isolated units were freshly compiled and
compared with direct strict objdiff. Retail `sema` address, block disassembly,
xrefs/callees, strings, and match records were reread for every row. The
current target and candidate `.rel.text` inventories have equal row counts in
all nine units except `render_resource_dispatch` (82 target, 84 candidate).
The following are final verdicts for this pass; they are not closure claims.

| Function | Strict text | Retail evidence and remaining difference |
| --- | ---: | --- |
| `func_8002ddb4` | 97.434494% | 44-block packet walker; both sides have 28 branches, but the FT3/GT3 positive-depth exits occupy different positions. Eleven proven calls and the unit's 169 text relocation rows are retained. |
| `func_8002e4dc` | 97.38307% | Paired 44-block packet walker with the same depth-exit placement residue; eleven proven calls and 169 unit text relocation rows. |
| `func_8002ebe0` | 95.27945% | 26-block blended packet walker; target checks fixed depth at the packet tail, while the candidate reuses an earlier guard. Twelve proven calls. |
| `func_8002ff5c` | 55.263805% | Prepared packet copier with all fourteen proven copy calls, eleven CFG blocks, and sixteen text relocations; the unresolved retail frame is 1248 bytes versus 1240 candidate bytes. |
| `render_map_cell_object` | 96.521736% | Eleven CFG blocks and sixteen proven calls; only the independent prologue load/address order differs before `SetRotMatrix`. Its three unit siblings remain exact. |
| `func_800311b0` | 92.14815% | Eight-block textured-quad builder with the single `AddPrim` call; packet-code store scheduling and saved-register allocation differ. |
| `func_80031850` | 99.29851% | Forty-block world renderer, with all 68 unit text relocations; first difference is saved-register/evaluation order after the typed lighting and map-cell referents. |
| `func_80031d8c` | 94.65414% | Seven-block animated renderer, with all 22 unit text relocations; retail retains a forwarded render argument in `s7`, candidate reloads it. |
| `func_8003247c` | 92.18579% | Ninety-six-block render dispatcher; two extra candidate address relocations accompany rematerialized `player_state` camera base. Its 32-byte identity matrix remains byte exact. |
| `func_800349bc` | 96.31408% | Four-quad menu fade with fourteen CFG blocks and 96 unit text relocations; target spills the pad state in a 72-byte frame, candidate uses a register and 64 bytes. Two siblings and eight initialized bytes stay exact. |
| `func_80036e24` | 98.86364% | Five-block frame/CD service loop; all five calls and text relocations agree, with a cyclic assignment of three saved argument registers. |

The blended packet walker's one-branch difference remains a structural
question, but current C already spells the observed depth check; earlier
equivalent branch and scope probes did not establish the original source
shape. The other rows supplied no new field, call, constant, or referent
correction. No source, identity, or relocation row was changed in this pass.

### Resource, actor, and event follow-up

A second fresh isolated batch checked fourteen GAME WIPs in twelve units.
Every function received the same retail address, CFG disassembly, xref,
callee, string, and match review. Strict results and final verdicts are:

| Function | Strict text | Verdict |
| --- | ---: | --- |
| `func_80015d58` | 89.03145% | One-block startup loader, 22 proven calls; the candidate lacks relocatable constructors for unowned arena/copy destinations. |
| `func_80015fd4` | 89.710144% | Three-block transition setup; the `0x8012da68` TMD workspace remains an unbound object address. Together the startup unit has 127 retail versus 115 candidate text relocations. |
| `func_80016260` | 98.790085% | Sixty-seven-block transition request; all 135 text relocation rows are present. Byte-valued controls are typed, with argument-register/branch schedule still different. Its 28-byte retail `state_8017d118` `.bss` symbol is a 32-byte candidate COMMON symbol. |
| `func_80016820` | 99.193474% | Sixty-two-block transition step; the candidate lacks three HI16/LO16 workspace pairs, two for `0x8019e138` and one for `0x8012da68` (196 retail versus 190 candidate text relocations). |
| `func_8003983c` | 99.19598% | Thirty-seven-block actor lifecycle handler; call and referent families agree, with actor/chance saved-register assignment remaining. |
| `func_80039b58` | 90.95744% | Nine-block actor-group scan; two proven calls agree, but the actor base and field cursor have different register lifetimes. |
| `func_8003ae50` | 99.31746% | Fifty-eight-block animation updater; retained calls and referents, with a local register/schedule residue. Its six unit siblings remain exact. |
| `func_8003c3e0` | 99.64539% | Twenty-three-block group-position helper; known calls/field reads agree, with yaw-normalization register order remaining. Its three unit siblings remain exact. |
| `func_800460a0` | 99.268295% | Six-block animation phase seeker; call set and field widths agree, with step/half-step saved-register assignment remaining. |
| `func_800461a0` | 99.12676% | Fourteen-block marker stream search; branch topology and thirteen relocation rows agree, with record-cursor register order remaining. Its marker leaf sibling is exact. |
| `func_800462bc` | 98.68132% | Forty-six-block event target interpreter; all 57 text relocations agree. One candidate load-delay `nop` moves a single 64-byte jump-table addend by four bytes; retail table identity is unchanged. |
| `func_800475d8` | 99.166664% | Fifty-five-block map-object event controller; 62 text relocations agree, but the candidate schedules `remove_object = 0` into a branch delay slot instead of a retail fallthrough block. |
| `func_80047c98` | 99.81618% | Eighty-five-block world event dispatcher; all 72 text relocations agree, with rotation argument and constant-one saved registers exchanged. |
| `func_800489ac` | 98.82883% | Twenty-three-block restore interpreter; the actor base and `0xff` sentinel exchange argument registers; its 64-byte jump table is exact. |

The nine missing resource relocation pairs reflect real signed-low workspace
address construction in retail, but the complete defining objects and source
mechanism are not proved. The actor/event rows retain their already supported
calls, widths, and referents; no field or CFG correction emerged from this
pass. No C, metadata, or profile edit was retained.

### Main, audio, menu, and player controls

A third disjoint focused screen checked ten GAME WIPs in nine units, with the
retail `sema` address, block, xref, callee, string, and match records reread
for each. Fresh isolated strict results and final verdicts are:

| Function | Strict text | Verdict |
| --- | ---: | --- |
| `game_main_loop` | 99.67553% | Five-block loop and 46 proven direct calls; the fixed `0x8009b0a0` arena constructor still differs in signed-low opcode/source origin. Adjacent `main` is exact. |
| `func_800139c4` | 89.30556% | Seven-block audio startup with eight proven sound calls; four unowned sequence/VAB workspace addresses account for eight retail-only text relocation rows. |
| `cd_request_service_vab` | 94.87342% | Eleven-block request service with eleven proven calls; retry and state-path constant/register placement remains. Fifteen audio-unit siblings are exact. |
| `menu_draw_window` | 99.78788% | Ten-block window painter; retail uses a 48-byte frame versus 40 candidate bytes. Both initialized tables, 2704 bytes total, remain byte exact. |
| `menu_draw_string` | 99.66904% | Eight-block glyph painter; retail uses a 56-byte frame versus 48 candidate bytes and a different UV temporary register. |
| `menu_format_number` | 97.39% | Thirty-six-block numeric formatter; its seven text relocations agree, while frame and branch-delay placement remain. |
| `func_80025a18` | 95.85052% | Ninety-nine-block equipment dispatcher; its fifteen siblings are exact. The 92-byte initialized DATA is exact, the 244-byte table has one four-byte code-layout addend difference, and the larger retail BSS versus candidate COMMON placement is separately audited. |
| `func_800279cc` | 98.23967% | Seventy-block collision/sound handler; retail has two more `player_state` HI16/LO16 address pairs (141 versus 137 unit text relocs). Known call and typed field families agree. |
| `func_80027f78` | 95.91228% | Twenty-seven-block collision response; candidate rematerializes four more `player_state` address pairs (59 versus 51 unit text relocs). Earlier pointer-scope source control regressed and was discarded. |
| `func_8002ce68` | 65.85185% | Five-block, seven-argument floor-item constructor; target loads stack arguments after the free-slot call in a 40-byte frame, whereas the candidate hoists them into saved registers and uses 56 bytes. Both siblings are exact. |

The sequence/VAB workspace extents and original defining owners are not
proved. The other differences above follow stack/register lifetime or table
body layout; none supplies a new width, call, field, or semantic correction.
No source or metadata edit was retained from this batch.

For `0x8002ff5c`, raw GAME `0x800303ec..0x8003043c` writes two index
halfwords at `sp+1088` and `sp+1090`, reads their packed word, then replaces
only the lower halfword and reads the packed word again for the first FT4
child's packet offsets 24 and 28. This is a genuine local two-index scratch,
not packet padding. Three off-tree typed views tested whether the source
could recover that exact memory form: direct struct-halfword union, direct
array-halfword union, and pointer-to-union. Their isolated strict results
were **9.764418%**, **9.764418%**, and **55.05031%** against the retained
**55.263805%**. The direct forms enlarged the frame and shifted unrelated
spills; the pointer form emitted indirect halfword stores instead of the
retail direct stack stores. All kept 16 text relocations. None was retained;
the original source spelling of the scratch remains unproved.

### Fresh mask-line, actor-motion, and cell-pattern controls

Focused quick builds and isolated strict objdiff from the current source
supersede older CFG counts for these GAME WIPs:

| Function | Direct strict text | Current CFG and verdict |
| --- | ---: | --- |
| `func_8002c424` | 98.29932% | **23/23 blocks**, 14/14 branches, and 2/2 return frontiers. The older 23/22 block note predates the indexed-cursor correction. The first remaining difference assigns the `-11` byte offset, BSS base, and `0xff` sentinel to different saved registers; eleven unit siblings remain exact. |
| `func_8003b5d0` | 96.42041% | 40/39 blocks and 21/21 branches. Retail keeps a separate state-`0x20` zero-state store and exit block; the candidate shares an exit. Its signed collision-height/speed guard and stores agree with raw instructions. A nested short-circuit spelling compiled byte-for-byte identically off-tree. The other three unit functions remain exact. |
| `func_80034f90` | 97.86822% | 14/14 blocks and 7/7 branches; rotated pattern fields and referents agree, with register and independent address scheduling residue. |
| `func_80035194` | 89.59545% | **51/51 blocks**, 26/26 branches, and 3/3 return frontiers; older 51/49 and 86.086365% notes are stale. The nine-argument rectangle copy has the supported field masks, width/height guards, and eight known callers. Retail reserves 40 stack bytes versus 32 candidate bytes; stack-argument allocation and mask/row scheduling remain. |

No source change was retained for these four functions. Both a nested
short-circuit and an inverted positive guard for state `0x20` produced
SHA256-identical objects off-tree; neither supplies a second source model.

The same current-source isolated build rechecked four adjacent GAME units.
The floor-item constructor `0x8002ce68` remains 65.85185% strict with its
`0x8002ce2c` and `0x8002cf40` siblings exact; raw still reloads its fifth
through seventh O32 arguments after free-slot acquisition. The 40-group
target fixup `0x8003f7ec` remains 85.86207% with two exact siblings; only
sentinel setup and commutative pointer-addition order differ. Lifecycle
functions `0x8003983c` and `0x80039b58` remain 99.19598% and 90.95744%
with matching calls and fields. Frame/CD helper `0x80036e24` remains
98.86364% with all five direct calls and only cyclic saved-register
assignments. None yielded a source-backed edit or new exact function.

An off-tree ABI probe widened only the floor-item constructor's third and
fourth formals from `u8` to `s32`. It made the four raw `$s1`/`$s2` moves and
byte stores agree and raised strict text from 65.85185% to 66.22222%, while
both exact siblings stayed exact. The five known callers pass only small
constants, and retail does no observable full-width use of either formal;
their original declared widths therefore remain unproved. The frame and
late stack-argument gap did not change, so the typed source was retained.
Compiling the tracked source with the supplied GCC 2.5.7 probe instead of
GCC 2.6.0 produced a byte-identical object, including both exact siblings.

The released map-render unit was rebuilt separately. Its exact
`render_enqueue_map` control stays 1052/1052 bytes, the clipped-fan helper
`0x8002f5b0` stays 95.833336%, and `0x8002f808` is **92.038376%** strict
with 51/51 CFG blocks, 35/35 branches, and 4/4 return frontiers. The older
35/34 branch note is stale. Retail spills the full packet header at `sp+96`
and extracted mode at `sp+120`, reloading both; the candidate uses one stack
slot plus a saved register and reserves 120 versus retail's 168 stack bytes.
The source already retains both header uses and the ordered clipping/shading
calls. No complete missing live object or source change is evidenced.

### Constructor and independent GAME controls

A fresh focused constructor build has 116/116 CFG blocks, 22/22 branches,
and 26/26 known return frontiers. Isolated strict comparison is **98.05656%**
text, **18.394308%** RODATA, and exact DATA. Its first local residues are the
retail case-26 repeated zero-byte delay-slot store and the case-102 record
halfword reload; the candidate eliminates both through value reuse. The
switch arms retain the documented 123 table rows and ordered external calls.
No extra live field, call, or source-level control distinction is proved, so
the constructor was left unchanged.

Ten other current-source focused unit builds covered 34 GAME functions:
`menu_card_browser`, `memory_card_wait`, `menu_card_format_flow`,
`menu_transition`, `actor_lifecycle_target`, `player_reaction`,
`frame_step_cd_service`, `event_restore_stream`, `event_target_stream`, and
`actor_motion`. All prior exact siblings remain listing-identical; no new
exact function appeared. Every extracted WIP CFG remains matched where
available: respectively 33/33, 19/19, 14/14, 37/37 and 9/9, 142/142,
5/5, 23/23, 46/46, and 9/9 blocks. The card-format flow's indirect/overlap
boundary prevents a reliable CFG count. The first differences remain the
already documented register, frame, and independent load schedules; none
supports a new typed field, call, referent, or branch edit.

The current collision-shape dispatcher still has 174/172 CFG blocks and
99/98 branches. Raw opcode `0x11` writes zero to `$s7` at `0x8002acb4`
immediately before its extra `bnez $s7` at `0x8002acb8`; no branch targets
the latter address. That retail branch is unreachable under the decoded
flow, so reproducing it by a dead C condition would not clarify the source.
The 49 table rows and 13 target classes retain their reviewed order.

The released render/resource dispatcher has 96/96 CFG blocks, 54/54
branches, and 92.18579% fresh isolated strict text. Retail reserves 768
stack bytes versus the candidate's 760. Its 82/84 text-relocation rows
differ by two repeated candidate camera-base pairs, not by missing target
identities. The reported B62 successor-index difference reaches the same
effect-record increment in both objects. No separate eight-byte live object
or corrected source edge is proved, so both large dispatchers remain WIP
without a C edit.

The adjacent 17-claim collision-height unit was refreshed: eleven focused
listings remain identical and six remain WIP. The snapshot helper
`0x8002b874` has 8/9 CFG blocks and 3/3 branches; raw stores each branch's
unsigned radius halfword before loading its unsigned interaction height,
whereas the candidate schedules the loads differently before the same
shared height store. Prior local-width and branch-local controls compiled
identically or lost the raw common tail. No source edit was justified.

### Indexed radius-mask access

In `map_cell_layer_mask_radius` (GAME `0x800320b0`), retail advances the
column index in the inner loop and addresses each byte from the current
row base plus that index. The former C advanced a separate `cell` pointer.
The indexed load occurs only inside the `x >= 0 && (u32)x < 24` guard, so
negative and out-of-range columns do not evaluate `row[x]`. Spelling the
access this way preserves the bytes and gives the source the raw indexing
relationship. The pinned compiler still strength-reduces that expression
into a pointer induction; focused CFG remains 10/10 blocks and 6/6
branches. Isolated strict text rises only from **73.95918%** to
**74.061226%**. The five exact siblings, including the newly exact VAB
updater, remain 100%; `map_cell_visible` and `resource_tmd_queue_read`
stay at 93.6% and 98.4359%. No new exact is claimed.

The row pointer itself is formed before the row-offset guard, as retail
does. A separate off-tree flat-grid index kept pointer formation inside
the valid range and read `grid[row_offset + x]` under both guards; it
lowered isolated strict radius text to **57.938774%** while preserving all
five exact siblings. That safe C variant does not explain retail's row
pointer induction, so it was not retained. The current indexed read is
guarded, but the out-of-range pointer-formation question remains open.

The adjacent audio campaign rebuilt `game.audio_runtime` as one focused
unit: fifteen of seventeen functions remain listing-identical. Startup
`0x800139c4` has 7/7 CFG blocks and 3/3 branches; its four fixed workspace
addresses still lack complete defining owners and appear as literals where
retail has relocation pairs. VAB service `0x800144b8` has 11/11 blocks and
5/5 branches; the probe keeps retry sentinel `-1` in a saved register where
retail keeps phase value `1`. Both preserve their call and field behavior,
and no audio source or metadata edit was retained.

The post-correction player effect dispatcher `0x80025a18` was rechecked
read-only. Its 99/99 CFG blocks, 31/31 branches, 12/12 return frontiers,
16 probe and 11 constructor call sites, and 108 ordered text referents now
agree; all fifteen unit siblings remain identical. Retail still homes and
reloads `effect_id` at `sp+112` in a 112-byte frame, while the supported
`va_list` source uses a 104-byte frame and retains the argument register.
Both supplied SDK and repository `stdarg` headers emit that supported form.
Manual indexing beyond `&effect_id` was an earlier positive ABI control but
has no proven source/header provenance, so no dispatcher edit was retained.

### Player collision and reaction control screen

A fresh focused screen of `player_collision_sound`,
`player_collision_response`, `player_interval_71_80`, and `player_reaction`
covered 23 GAME claims. The 19 prior exact siblings remain listing-identical;
the four WIPs have no newly missing call or typed referent. Landing controller
`0x800279cc` still has 70/70 CFG blocks, 37/37 branches, and 3/3 return
frontiers. Its first control-word difference changes only the register holding
the same collision result; the candidate also retains a 64-byte frame versus
retail's 72. Collision response `0x80027f78` has 27/27 blocks, 14/14
branches, and 4/4 return frontiers; the `player_state` component addresses
in its motion-length arm are rematerialized in the candidate instead of
loaded from the retained structure pointer. No distinct target field is lost.

The inclusive 71–80 predicate `0x8002897c` has 3/3 blocks and its sole
branch on both sides; retail and candidate exchange the Boolean temporary
registers and the candidate adds a final move. The larger reaction dispatcher
`0x8002985c` remains 142/142 blocks, 79/79 branches, and 2/2 return
frontiers, with an unresolved indirect jump on both sides. Its first local
differences are constant and register scheduling around the same fade clamp.
Prior signed-value controls regressed, and the shared caller semantics remain
unchanged. None of these four merits a source edit on the current evidence.

The adjacent five-function card-directory unit was rechecked: four WIPs
retain matching CFG/branch counts (13/13 and 7/7, 24/24 and 13/13, 9/9 and
4/4, 24/24 and 14/14), and `memory_card_format` remains exact. An off-tree
source-equivalent aggregate initializer for the two slot-seed bytes lowered
`0x800226ec` isolated strict text from **93.60504%** to **91.60504%**;
the other functions and initialized data stayed unchanged. It was discarded.
The retail signed-byte loads in the title path remain unsupported by a
safe source change after the documented signed-view controls.

### Event and card control screen

Eleven separately focused GAME units cover 21 claims. Fourteen previously
exact listings remain `SAME`: the command, counter, pose, and map-object
spawn helpers; the card formatter, two menu-transition siblings, three
card-wait siblings, and the message marker's first helper. The seven WIPs
retain their reviewed call sets and source fields:

| GAME VA | Focused CFG / branches | First remaining difference |
| --- | --- | --- |
| `0x8001b554` | 33/33, 16/16 | Probe result in `v0` versus retail's copied `a0`, then a delay-slot address setup. |
| `0x8001bf68` | CFG unavailable at an overlapping trial target | Status/constant saved-register allocation; exact formatter sibling unchanged. |
| `0x80023178` | 19/19, 10/10 | Card header and decimal-quotient register lifetimes. |
| `0x800349bc` | 14/14, 8/8 | 72-byte retail frame versus 64-byte candidate, with independent packet-store scheduling. |
| `0x800461a0` | 14/14, 5/5 | Two stream cursors use exchanged registers and equivalent relative byte offsets. |
| `0x800462bc` | 46/46, 21/21 | Candidate loads state before phase and inserts a load-delay `nop`; the 64-byte table's row `+0x0c` consequently targets text addend `0x2c4` versus retail `0x2c0`. |
| `0x80047c98` | 85/85, 55/55 | Saved register for a constant and one for the event-record pointer exchange roles. |

For the target stream, the source already reads both fields with their raw
widths before the phase-dependent call. Reordering those independent loads
has no evidenced semantic basis. No source or metadata edit was retained
from this screen, and no new exact result is claimed.

One narrow off-tree marker probe expressed the two condition bytes relative
to the already available marker cursor (`marker[-2]` and `marker[-1]`)
instead of the equivalent stream-cursor offsets. Although retail addresses
those bytes from the marker register, that spelling lowered isolated strict
`0x800461a0` text from **99.12676%** to **96.73239%**; its exact sibling
`0x80046144` stayed 100%. Machine-code base choice alone does not prove the
original C subscript, so the readable source was preserved.

A separate off-tree menu-transition macro control reused its local
primitive-buffer pointer for the bounds check instead of rereading the
global pointer. Although no call intervenes, this changed the pinned
compiler's buffer lifetime and lowered `0x800349bc` isolated strict text
from **96.31408%** to **83.71119%**. Its two exact siblings and the
eight-byte datum remained 100%; the trial was discarded.

### Actor scorer and adjacent control screen (2026-10-02)

Eight focused GAME units cover 21 functions. Each strict percentage below
comes from a fresh manifest-profile compile into an isolated object followed
by direct objdiff against its safe-delinked retail object; the older cached
report is not used. Twelve functions remain exact, and nine remain WIP:

| Function | Strict text | Final verdict |
| --- | ---: | --- |
| `func_80039108` | 93.93092% | WIP: 59/59 CFG blocks, 37/36 branches; the case-9 tolerance Boolean still joins the callback branch. All 131 switch rows retain their ten target classes. |
| `actor_select_best_target` | 100% | Exact control. |
| `func_8003983c` | 99.19598% | WIP: 37/37 blocks and 24/24 branches; first difference exchanges the saved actor and constant registers. |
| `func_80039b58` | 90.95744% | WIP: 9/9 blocks and 4/4 branches; retail saves `s4` for `0xff`, while the probe materializes it at the comparison in a smaller frame. |
| `func_80039c14` | 100% | Exact fixed-curve control. |
| `func_80039c94` | 98.11751% | WIP: 72/72 blocks and 46/46 branches; eight fixed-curve calls agree, with saved argument/accumulator registers and a later eight-byte layout offset differing. |
| `func_8003a318` | 99.86911% | WIP: 26/26 blocks and 13/13 branches; two call-argument temporaries exchange registers. |
| `func_8003a614` | 96.91011% | WIP: 6/6 blocks and 3/3 branches; retail rematerializes the player-state base. |
| `func_8003a778` | 100% | Exact damage sibling. |
| `func_8003a9f4` | 100% | Exact animation control. |
| `func_8003ab5c` | 100% | Exact animation control. |
| `actor_bind_current` | 100% | Exact animation control. |
| `actor_advance_animation_wrapped` | 100% | Exact animation control. |
| `actor_advance_animation_clamped` | 100% | Exact animation control. |
| `actor_animation_crossed_phase` | 100% | Exact animation control. |
| `func_8003ae50` | 99.31746% | WIP: 58/58 blocks and 34/34 branches; remaining delay-slot/store scheduling has no supported source correction. |
| `func_8003b33c` | 100% | Exact motion/collision control. |
| `func_8003b520` | 100% | Exact motion/collision control. |
| `func_8003b5bc` | 100% | Exact motion/collision control. |
| `func_8003b5d0` | 96.42041% | WIP: 40/39 blocks and 21/21 branches; retail keeps a separate state-`0x20` reset/exit block. |
| `func_8003c614` | 86.041916% | WIP: 45/45 blocks and 15/15 branches; retail has 11 versus seven position-helper calls and nine versus ten constructor calls. |

The scorer's raw case-9 tail branches from the tolerance result at
`0x80039298` directly to the score path and jumps to the return path at
`0x800392a0` on failure. The C already expresses those outcomes with a
separate angle/tolerance call pair. The group-effect helper-call gap and the
motion reset block likewise have prior source-equivalent negative controls.
An additional off-tree case-9 spelling used a positive tolerance guard and
`break` to enter the post-switch scoring tail. It produced the same negative
result as the prior positive `goto`: strict text **89.68750%** and RODATA
**37.02290%**, versus the retained **93.93092%** and **96.183205%**.
No source or metadata change follows from this screen, and none of its twelve
exact functions is a new exact claim.

### Actor motion, group, and trajectory controls (2026-10-02)

Seven more focused units cover 21 functions. Direct objdiff of fresh isolated
manifest-profile objects confirms 15 previously exact controls and six WIPs:

| Function | Strict text | Final verdict |
| --- | ---: | --- |
| `func_8003b9a4` | 100% | Exact motion control. |
| `func_8003bae4` | 100% | Exact motion control. |
| `func_8003bba0` | 100% | Exact motion control. |
| `func_8003bcd0` | 100% | Exact motion control. |
| `func_8003bd40` | 86.53226% | WIP: 9/9 CFG blocks and 5/5 branches; pre-angle-call subtraction order and register lifetimes differ, with identical calls and typed slots. |
| `func_8003be38` | 100% | Exact motion control. |
| `func_8003bf74` | 100% | Exact motion control. |
| `func_8003c000` | 100% | Exact group-position control. |
| `func_8003c10c` | 100% | Exact group-position control. |
| `func_8003c220` | 100% | Exact group-position control. |
| `func_8003c3e0` | 99.64539% | WIP: 23/23 blocks and 11/11 branches; the masked angle value uses a different temporary register before the same comparison. |
| `func_8003d084` | 100% | Exact spatial-sound control. |
| `func_8003d0e8` | 100% | Exact spatial-sound control. |
| `func_8003d184` | 99.931595% | WIP: 410/410 known blocks and 213/213 branches; one repeated target-halfword load, 144/136-byte frame, and local/register allocation remain. All 241 switch rows and their RODATA bytes are exact. |
| `func_8003f610` | 100% | Exact target-fixup control. |
| `actor_fixup_group_targets` | 85.86207% | WIP: 9/9 blocks and 4/4 branches; sentinel scheduling and commutative pointer addition differ. |
| `func_8003f860` | 100% | Exact target-fixup control. |
| `func_800460a0` | 99.268295% | WIP: 6/6 blocks and 2/2 branches; the pre-tolerance angle values occupy different saved registers. |
| `func_80015918` | 95.49419% | WIP: 41/41 blocks and 24/24 branches; subtraction result and three-call path preserve semantics but allocate different argument temporaries. |
| `func_80015bc8` | 100% | Exact trajectory control. |
| `func_80015ce0` | 100% | Exact trajectory control. |

The near-exact behavior dispatcher was checked against its raw callsites:
the apparent shifted `vector3s_scale_shift12` call in the focused listing
comes from local-stack offsets, while the two calls and arguments keep their
retail order. Its separate case-vector scope probes already enlarged the
frame past retail and were discarded. No supported source correction or new
exact result emerged from these 21 functions.

### Menu map preview and primitive controls (2026-10-02)

Eleven focused GAME units and fresh isolated strict objects cover 18
functions linked by the map-preview's TIM upload, FT4 setup, menu drawing,
and nearby primitive helpers. Twelve were already exact; six remain WIP:

| Function | Strict text | Final verdict |
| --- | ---: | --- |
| `func_8001930c` | 98.26363% | WIP: 47/47 CFG blocks and 27/27 branches; archive/TIM/primitive calls and 69 ordered referents agree. Retail reserves 64 stack bytes and saves `s7`; the probe reserves 56 bytes. |
| `menu_draw_window` | 99.78788% | WIP: 10/10 blocks and 6/6 branches; retail frame is 48 bytes versus 40 probe bytes. |
| `menu_draw_two_option` | 100% | Exact control. |
| `func_8002083c` | 99.65882% | WIP: 4/4 blocks and 2/2 branches; four live SDK `MATRIX` locals explain the calls, but retail reserves 224 bytes versus 160 probe bytes. |
| `func_80020990` | 100% | Exact control. |
| `menu_draw_string` | 99.66904% | WIP: 8/8 blocks and 4/4 branches; retail frame is 56 bytes versus 48 probe bytes. |
| `menu_draw_number` | 100% | Exact control. |
| `primitive_buffer_begin_poly_ft4` | 100% | Exact FT4 setup control. |
| `primitive_buffer_commit_poly_ft4` | 100% | Exact FT4 commit control. |
| `menu_list_init` | 100% | Exact primitive/list control. |
| `func_800312f4` | 100% | Exact sliding-panel control. |
| `func_80031384` | 100% | Exact sliding-panel control. |
| `func_80031414` | 100% | Exact color-byte draw control. |
| `func_800314d4` | 100% | Exact color-byte setup control. |
| `tim_upload_images` | 100% | Exact TIM upload control. |
| `func_800349bc` | 96.31408% | WIP: 14/14 blocks and 8/8 branches; retail frame is 72 bytes versus 64 probe bytes, with packet-store and register scheduling differences. |
| `func_80034e10` | 100% | Exact transition control. |
| `func_800311b0` | 92.14815% | WIP: 8/8 blocks and 5/5 branches; retail saves `s2` in a 32-byte frame while the probe uses 28 bytes. |

The map-preview source already uses the SDK packet macros and preserves the
two-frame allocation, upload, draw, pad-wait, and free lifetime. Previous
source-equivalent archive-index and frame-layout probes were negative; no
additional live object is proved for any of the frame differences above.
KF1's `menu_map_viewer.c` is a related but distinct implementation: it uses
stack-owned double-buffered quads and a path string, while this KF2 retail
body writes global `current_poly_ft4` packets from an allocated archive TIM.
Its stack objects therefore do not explain the KF2 frame difference.
No source or metadata edit, exact closure, or new exact count follows from
this screen.

### Adjacent menu controller controls (2026-10-02)

Ten further focused menu units cover twelve functions. Fresh isolated strict
objdiff finds six existing exact controls and six WIPs:

| Function | Strict text | Final verdict |
| --- | ---: | --- |
| `func_8001a4f0` | 99.74359% | WIP: 22/22 CFG blocks, 12/12 branches; the 74-record initializer index uses a different register, with its widths and 28 referents aligned. |
| `func_8001b554` | 98.478264% | WIP: 33/33 blocks, 16/16 branches; probe result remains in `v0` rather than retail's copied `a0`. |
| `func_8001bf68` | 97.12389% | WIP: card-format status and constant register allocation differs; the focused CFG tool cannot analyze its overlapping trial target. |
| `func_8001c12c` | 100% | Exact card-format sibling. |
| `func_8001e378` | 100% | Exact input-poll control. |
| `func_8001e484` | 100% | Exact list-input control. |
| `func_8001e94c` | 100% | Exact status-render control. |
| `func_8001f008` | 100% | Exact attribute-render control. |
| `func_8001fc94` | 99.70803% | WIP: 55/55 blocks and 33/33 branches; an independent list-tail immediate/move schedule remains. |
| `func_80021c8c` | 99.956985% | WIP: 7/7 blocks and 3/3 branches; retail reserves eight more frame bytes. |
| `func_80021e00` | 100% | Exact display-state sibling. |
| `menu_format_number` | 97.39000% | WIP: 36/36 blocks and 19/19 branches; retail's extra eight-byte leaf frame shifts the fifth stack argument. |

The previously tested source-equivalent initializer, local-buffer, and
status-order spellings did not establish a missing field or control path.
No C change or new exact claim was retained.
