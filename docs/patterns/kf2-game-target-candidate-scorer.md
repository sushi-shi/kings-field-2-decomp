# GAME target-candidate scorer pilot

`func_80039108` is an unclaimed 0x4c0-byte GAME function at `0x80039108`.
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

Final verdict: **WIP, unclaimed**. There is no source object or strict score.
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
before the guard, instead of a ternary at the later effect call, raises the
retained focused listing to **41.2%** and narrows retail/compiled CFG counts
to 45/46 blocks and 15/16 branches. It remains WIP: the first CFG divergence
is still in the dispatch prelude; frame and register assignments also differ.
The neighboring exact actor helpers in the table above were unaffected by
this source-only change. An isolated current object comparison reports
80.967064% strict `.text` similarity (`2672` retail bytes versus `2792`
compiled) and 7.9268293% `.rodata` similarity (`492` bytes on each side);
the matching `.rel.text` and `.rel.rodata` section sizes alone do not prove
ordered relocation equality.

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
| `0x8003c614` | Group-effect switch | WIP, 41.2% |
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
