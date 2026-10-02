# GAME menu and memory-card follow-up

This 25-function GAME campaign follows direct calls from the menu frame,
item-model, and memory-card flows. Each function's retail extent, disassembly,
CFG, callers, direct calls, strings, and prior match state was inspected. The
address span is a survey boundary; it is not evidence of a single original TU.

| GAME address | Retail role and decisive evidence | Verdict |
| --- | --- | --- |
| `0x8001876c` | top menu controller; enters display mode, draws windows, dispatches to item/card flows | WIP, **96.36646%** recorded strict; 34-block source claim, saved-register and return reload residue |
| `0x800189f0` | formats player values into a six-digit glyph row, then draws it | **exact**, 216/216 bytes in later focused comparison |
| `0x80018ac8` | item menu controller; list setup, item-model loading, frame and input calls | unclaimed; selection record and state flow WIP |
| `0x80018d08` | filters a 24-byte glyph-row table using a caller byte mask and writes two output streams | unclaimed; table at `0x80064c30` has no reviewed extent/identity |
| `0x80018dec` | related 24-byte glyph-row filter with player-dependent exclusions | **exact in later follow-up**; player-state equipped IDs establish the exclusion owner |
| `0x80018f8c` | menu action updates player equipment/stat state and emits sound cue | unclaimed; player-state transition model WIP |
| `0x800217f0` | nine-slice menu panel using sprite descriptors 11–19 | **exact**, 624/624 bytes |
| `0x80021c8c` | enters menu display mode, snapshots graphics buffers and music state | WIP, **99.956985%**; only 32-byte retail versus 24-byte compiled stack frame remains |
| `0x80021e00` | restores display buffers and sequence state on menu exit | **exact**, 272/272 bytes |
| `0x800221e8` | loads menu item-model archive entry into TMD slot 3 and resets preview vectors | **exact**, 212/212 bytes |
| `0x800222bc` | releases allocated menu item model | already exact |
| `0x80022468` | initializes memory-card events | already exact |
| `0x80022550` | shuts down memory-card events | already exact |
| `0x800225b0` | starts card services | already exact |
| `0x800225d8` | stops card services and restores pad | already exact |
| `0x80022600` | probes and removes stale card file with event waits | already exact |
| `0x800226ec` | enumerates card directory, parses save names and slot numbers | WIP; 40-byte SDK `DIRENTRY` input is modeled, but byte-load and initialization order differ |
| `0x800228c8` | reads card files and decodes header title digits | WIP; typed 0x280-byte header and CFG agree, but byte-load and digit-loop schedule differ |
| `0x80022b48` | calls SDK card format wrapper | already exact |
| `0x80022b74` | reads a card save and checks payload checksum | WIP; typed header checksum and buffer owner agree, but frame/register residue remains |
| `0x80022ca0` | creates/writes card save; draws icon and fills card metadata | WIP; typed header fields and seven-palette asset agree, but initialization order differs |
| `0x80023178` | writes Shift-JIS player experience and level digits into the card title | WIP, **93.529410%** in the prior strict run; CFG and signed divisions agree, register/order residue remains |
| `0x80023288` | sums card payload bytes | already exact |
| `0x800232ac` | waits for one of four card events | already exact |
| `0x8002332c` | clears card event states | already exact |

`0x800217f0` uses the reviewed 20-entry, 12-byte sprite table at
`0x80063e80`; its calls, six arguments, panel geometry, and 19 direct
relocation sites match retail. The display entry and exit now share the
24-byte typed primitive-buffer snapshot at `0x8006dbe8` and the one-byte
music snapshot at `0x8006d9e8`. Both snapshots are BSS. The exit function is
strict exact. The entry function's instruction listing, CFG, calls, and
referents agree with retail except for the stack-frame size; no unsupported
local was added to force the frame.

The item-model loader uses the KF1 counterpart's resource lifecycle as a
guide, but follows GAME's separate archive-size, allocation, archive-read,
and TMD-registration calls. A byte item ID, the player's menu-enable byte,
and early-return control flow produce an identical retail instruction listing.
Its strict result is 100%. The preview translation and rotation at
`0x8006da00` and `0x8006da08` are each SDK `SVECTOR`s, followed by the
32-bit rotation step at `0x8006da10`; prior 2-byte Ghidra fragments were
interior fields, not independent globals.

The card-label probe at `0x80023178` preserves the two decimal loops and
Shift-JIS byte layout, using the player's 32-bit experience and 8-bit level.
The level must promote to signed `int`: retail emits signed division and its
zero/overflow checks. The current object still differs in register selection
and some scheduling; the source is WIP, not an exact claim. No function in
this batch was attributed to vendored code.

A later pass established the card file's 0x280-byte header prefix. Its
two-byte `SC` magic begins at zero, icon type `0x13` is at +2, two-block
count at +3, title at +4, icon palette at +0x60, three 0x80-byte
icon frames at +0x80, and payload checksum at +0x200. The writer zeroes the
0x400-byte block header before copying this prefix. A shared typed view with
static size/offset checks now serves the header reader, checksum reader,
title-digit writer, and file writer;
the focused listing retains the exact `memory_card_format` control and the
four directory-unit WIP verdicts above. The distinct `DIRENTRY` entry type
comes from the pinned Psy-Q `KERNEL.H` and has a checked 40-byte stride.
The `firstfile` pattern at `0x8006d6a8` spans seven bytes including its
retail NUL terminator. Its reviewed `0x80022744` `lui`/`addiu` pair computes
that address immediately before the direct `firstfile` call. The seven-byte
wildcard is now claimed by `memory_card_directory.c`; the rebuilt unit matches
all 311 initialized-data bytes and all six `.rodata` bytes. Its exact bytes
do not occur in the supplied Psy-Q 3.0 library archives or card sample
sources. The original TU boundary and any allocation beyond the NUL remain
unresolved.
The neighboring bytes at `0x8006d6a4` and `0x8006d6a5` are `0x20, 0x00`.
Retail loads both as the initial two-byte slot-digit buffer in `0x800226ec`,
`0x800228c8`, and `0x80022ca0`. Their common two-byte role is supported by
all three xref pairs, but the original object boundary and source owner remain
unproved, so their address-derived identities stay separate.

The carved directory module's initialized sections contain its 0x130-byte
file-prefix/card-asset data and six-byte path literal; neither section owns
`0x8006d6a4` or `0x8006d6a5`. A pooled local `" "` initializer is compatible
with the two bytes and three callers, but the pinned GCC 2.5.7 probe places
that initializer's padded four bytes in this unit's `.rodata`, ahead of its
six-byte `bu00:` literal. The retail target instead has only the six-byte
literal in this unit and keeps the seed at a separate late data address.
That probe does not prove the historical compiler or the seed's defining TU;
the focused source keeps the two external byte identities pending an owner.

The first four header fields follow the KF1 `KfPsxSaveHeader` layout, while
KF2's own `SC 13 02` retail stores, three icon frames, and two-block `FCREAT`
argument support each field independently. Focused directory, wait, payload,
events, and probe comparisons retained all prior exact controls after the
layout-identical field split. The writer now uses the same two-block constant
for its header byte and `FCREAT` argument; the focused directory and probe
listings remain at their baseline.

In a later focused card-choice check, `0x8001aa9c` became **strict exact**
without a source edit. A fresh isolated compile and one-unit objdiff comparison
match all 484 `.text` bytes and all 17 ordered relocations in the carved
retail module; the unit owns no `.data` or `.rodata` claims. The older
97.305786% report is stale and does not describe this object pair.
The adjacent `game.menu_card_panel` unit is likewise **strict exact** in a
fresh isolated comparison: 1,316/1,316 `.text` bytes and 75/75 ordered
relocations match, with no owned data sections. Its `0x8001b030` (284 bytes)
and `0x8001b14c` (400 bytes) controls remain exact, and `0x8001b2dc`
(632 bytes) is newly exact despite its older 95.601265% report.

Focused `kf match` verified the new item-model function at strict 100% and
reported the card-label probe at 93.529410%, with GAME target relink 163/163.
The command exits at the repository-wide known-reference data-ownership
closure. No full build or repository tests were run in this focused pass.

## Fresh card graph verdicts

A separate isolated 14-unit comparison compiled only the 30 GAME card
functions below against their current carved retail modules. Direct objdiff
reports **23/30 strict exact**, 9,604/13,720 exact code bytes, and 337/337
data bytes. The seven remaining functions are WIP; their percentages are
fuzzy comparison scores, not closure.

| Function addresses | Final strict verdict |
| --- | --- |
| `0x8001aa9c`, `0x8001ac80`, `0x8001af30` | all exact |
| `0x8001b030`, `0x8001b14c`, `0x8001b2dc` | all exact; panel unit 1,316/1,316 code bytes and 75/75 ordered relocations |
| `0x8001b554` | WIP, 98.478264%; probe-branch register and delay-slot order differ |
| `0x8001b834`, `0x8001ba80`, `0x8001bb94`, `0x8001bcfc` | all exact |
| `0x8001bf68`, `0x8001c12c` | WIP 97.123890%; exact, respectively |
| `0x80022438`, `0x80022468`, `0x80022550`, `0x800225b0`, `0x800225d8` | all exact; event unit's one initialized data byte also matches |
| `0x80022600` | exact; 26/26 claimed `.rodata` bytes match |
| `0x800226ec`, `0x800228c8`, `0x80022b48`, `0x80022b74`, `0x80022ca0` | WIP 93.605040%; WIP 85.156250%; exact; WIP 93.666664%; WIP 95.896774%, respectively; directory unit's 310/310 initialized data and `.rodata` bytes match |
| `0x80023178`, `0x80023288`, `0x800232ac`, `0x8002332c` | WIP 93.529410%; three exact wait/checksum helpers |
| `0x80048d24`, `0x800492dc` | both exact; payload unit 2,968/2,968 code bytes and 226/226 ordered relocations |

The newly verified exact `0x8001aa9c` has 484/484 code bytes and 17/17
ordered relocations. The earlier browser, label, event, and payload exact
controls remain exact. No repository tests, full build, or banking accompanied
this isolated comparison.

The seven WIPs have these first unresolved instruction differences; their
source call sets and identified data referents remain aligned with retail:

| Address | First unresolved difference |
| --- | --- |
| `0x8001b554` | retail copies the probe result to `a0` before two delayed branches; the current object branches on `v0` |
| `0x8001bf68` | retail keeps the probe status in `v1`; the current object saves it in `s0` and uses a different constant register |
| `0x800226ec` | retail loads the slot-seed bytes with `lb` and places the initialization around `memset` differently |
| `0x800228c8` | retail loads the slot seed earlier, uses `lb` for title bytes, and advances a byte offset through the two digit loops |
| `0x80022b74` | retail uses an 80-byte frame and preserves the slot in `s3` before copying it to `s2`; the current object uses a 72-byte frame |
| `0x80022ca0` | retail orders the slot-seed loads, zero fill, and saved-register setup differently |
| `0x80023178` | retail and current object have the same signed decimal divisions and loop exits, with different argument and temporary registers |

An isolated 14-unit rebuild after the shared header changes retained all
23/30 strict exact card functions, 9,604/13,720 exact code bytes, and
337/337 initialized data bytes. Bounded GCC 2.5.7 plain, R3000, and
no-scheduling controls on the four WIP units produced no new exact function;
plain and no-scheduling also lost the exact `0x8001c12c` sibling. GCC 2.6.0
lost every exact sibling in those four units. An alternate typed header-base
offset loop for `0x800228c8` decreased its strict comparison score and was not
retained. These results limit the current probes without proving the original
compiler or treating a fuzzy score as source truth.

The payload pair has one proven external caller each: writer `0x80022ca0`
calls serializer `0x80048d24` at `0x80023020`, and reader `0x80022b74`
calls deserializer `0x800492dc` at `0x80022c5c`. The other xrefs to each
payload body are internal validated branches, so this pair adds no unresolved
neighboring game call to the card graph.
The browser, format-flow, and wait WIP units retain the same ordered
relocation kinds and target identities as retail. The directory unit has the
same 137 relocation count, but the `0x800228c8` compiled reader places the
card-prefix `.data` pair before the two slot-seed pairs; retail orders the
slot seeds first. This is a reference-order difference, not evidence for a
new data identity.

## Focused ten-function recheck (2026-10-01)

The ten GAME addresses below were rechecked against their retail block
disassembly, incoming and outgoing references, strings, source claims, and
focused rebuilt unit listings. The strict percentages are the prior checkpoint
reported by `kf sema match`; that report currently marks itself stale, so the
fresh focused listing is the current comparison evidence. All ten remain WIP.

| Address | Prior strict | Fresh focused | First remaining difference |
| --- | ---: | ---: | --- |
| `0x8001b554` | 98.478264% | 84.6% | Probe result remains in `v0` rather than retail's `a0`; subsequent branches shift by two instructions. |
| `0x8001bf68` | 97.123890% | 87.7% | Probe status and dialog constants occupy different registers; exact `0x8001c12c` stays unchanged. |
| `0x8001f8b8` | 94.364640% | 84.8% | Saved argument registers and the input-release exit block differ after otherwise matching glyph and input calls. |
| `0x8001fc94` | 98.449640% | 98.6% | Row-width register and card-column test scheduling differ; the lower-panel zero initialization swaps with a neighboring load. |
| `0x8002083c` | 99.658820% | 73.1% | Retail reserves 64 more stack bytes than the four live MATRIX locals explain; both exact siblings stay unchanged. |
| `0x80021c8c` | 99.956985% | 97.2% | Only the eight-byte frame and saved-`ra` slot delta remains; exact `0x80021e00` stays unchanged. |
| `0x800226ec` | 93.605040% | 88.6% | Signed slot-seed loads and the first `memset` setup/delay slot differ. |
| `0x800228c8` | 85.156250% | 67.6% | Slot-seed initialization and signed title-byte loads precede two offset-driven digit loops in retail. |
| `0x80022b74` | 93.666664% | 84.2% | Retail saves the slot in another register and reserves an 80-byte rather than 72-byte frame; exact `memory_card_format` stays unchanged. |
| `0x80022ca0` | 95.896774% | 96.7% | Slot-seed loads, zero-fill setup, and saved-register selection differ before the same card-write call path. |

The browser's nested probe-guard spelling compiled identically to its existing
compound guard. An explicit title-byte offset in `0x800228c8` also retained
the same header fields and digit values but compiled farther from retail
(67.6% to 66.1% focused). A signed-byte view of the decoded halfword left the
67.6% listing unchanged. All three probes were reverted. The unowned adjacent
slot-seed bytes at `0x8006d6a4/5` and unexplained frame space remain WIP, not
grounds for fake data owners or stack locals. No repository tests or full build
were run for this focused recheck.

A fourth probe initialized the first slot buffer from the C literal `" "`.
It emitted retail-style `lb` instructions, but its relocations targeted new
unit `.rodata` instead of the two reviewed `0x8006d6a4/5` data identities. It
also shifted the existing `bu00:` literal and made exact `memory_card_format`
non-exact. The literal probe was reverted; matching opcode shape alone does
not establish the original data owner.

A signed `s8` view of the already-typed card title in `0x800228c8` did not
recover retail's `lb` copies: the probe still emitted `lbu`, introduced an
extra saved register, and dropped from 67.6% to 62.5% focused. Moving the
card-column predicate into the row loop in `0x8001fc94` dropped its focused
listing from 98.6% to 92.4% and removed the retail-sized frame. Both probes
were reverted; `memory_card_format` remained an exact listing control.

Moving the card writer's `present = 0` initialization to just after its
directory scan left `0x80022ca0` at 96.7% focused and the exact format sibling
`SAME`. Retail initializes the value after the scan, but the probe compiler
still scheduled the zero before the slot-seed loads; the source-order probe
was reverted because it established no new data or control fact.

The KF1 sibling defines its `memory_card_root_path` as a load-image `char`
array in `src/game/save_system.c`, consistent with the KF2 `bu00:*` bytes at
`0x8006d6a8` being writable card data. It does not determine KF2's original
TU boundary or the separate `0x8006d6a4/5` seed extent.

## Current menu/card ten-function recheck

Focused rebuilds and direct per-unit objdiff give **2/10 strict exact** in
this disjoint controller/card set. Each WIP retains its source claim and was
left unchanged where raw control flow, calls, and referents provide no
independent correction.

| GAME address | Current strict verdict | First supported residue |
| --- | ---: | --- |
| `0x8001a898` | **exact, 516/516 bytes** | Complete item/equipment controller. |
| `0x8001a4f0` | WIP, 99.74359% | Initial 74-record clearing loop uses a different index and constant register; 22/22 CFG blocks agree. |
| `0x8001b554` | WIP, 98.478264% | Probe result stays in `v0` rather than retail's `a0`. |
| `0x8001bf68` | WIP, 97.12389% | Probe-status and dialog-constant registers differ. |
| `0x8001d3b4` | **exact, 672/672 bytes** | Complete item-sale controller. |
| `0x8002083c` | WIP, 99.65882% | Four live matrix locals explain the calls, but retail reserves 64 more stack bytes. |
| `0x800226ec` | WIP, 93.60504% | Signed slot-seed loads and first `memset` scheduling differ. |
| `0x800228c8` | WIP, 85.15625% | Signed title-byte loads and two digit-loop schedules differ. |
| `0x80022b74` | WIP, 93.666664% | Retail keeps the slot in another saved register and uses an 80-byte frame. |
| `0x80022ca0` | WIP, 95.896774% | Slot-seed and zero-fill setup order differs. |

The `0x8001bf68` neighbor `0x8001c12c`, the two-option draw and heading
helpers beside `0x8002083c`, and `memory_card_format` beside the four card
WIPs remain strict exact controls. The directory unit's current 311-byte
`.data` and six-byte `.rodata` claims are 100%. The wildcard at `0x8006d6a8`
is now source-owned; its reviewed `firstfile` referent remains intact. After
the narrow target refresh, the focused unit remains 1/5 `SAME` including
`memory_card_format`; `0x800226ec` is 88.6% focused WIP. All 137 text
relocation sites remain, with one ordered referent-pair shift in existing WIP
`0x800228c8`. A temporary signed-byte slot-buffer view left `0x800226ec`
byte-identical and was discarded. The wildcard's original TU boundary and the
adjacent seed-byte owner remain open.
No linked build, repository tests, or banking were run for this recheck.

## Card-browser and directory focused audit (2026-10-01)

This 20-function GAME batch uses fresh `kf try --context 0 --no-flow`
listings. `SAME` below means the focused listing matches; the previously
recorded strict objdiff results remain the exactness evidence. The seven
`DIFF` bodies retain their WIP status. The adjacent card-choice controller
`0x8001aa9c` belongs to a separate batch and was excluded.

| Address | Focused verdict | Remaining evidence |
| --- | --- | --- |
| `0x8001ac80` | SAME | Browser load. |
| `0x8001af30` | SAME | Browser row helper. |
| `0x8001b030` | SAME | Card panel sibling. |
| `0x8001b14c` | SAME | Card panel sibling. |
| `0x8001b2dc` | SAME | Card panel sibling. |
| `0x8001b554` | DIFF, 84.6% | 33/33 CFG blocks and 16/16 branches agree; retail moves the probe result from `v0` to `a0`, while the compiled path retains `v0`. |
| `0x8001b834` | SAME | Slot browser. |
| `0x8001ba80` | SAME | Card label helper. |
| `0x8001bb94` | SAME | Card label helper. |
| `0x8001bcfc` | SAME | Save browser. |
| `0x8001bf68` | DIFF, 87.7% | Probe-status and dialog-constant register assignments differ; retail CFG extraction is unavailable because a local jump overlaps the trial placement. |
| `0x8001c12c` | SAME | Format-flow sibling. |
| `0x800226ec` | DIFF, 88.6% | 13/13 CFG blocks and 7/7 branches agree; signed seed loads and first `memset` setup differ. |
| `0x800228c8` | DIFF, 67.6% | 24/24 CFG blocks and 13/13 branches agree; signed title-byte loads and digit-loop scheduling differ. |
| `0x80022b48` | SAME | Directory sibling. |
| `0x80022b74` | DIFF, 84.2% | 9/9 CFG blocks and 4/4 branches agree; retail uses an 80-byte frame against the compiled 72-byte frame. |
| `0x80022ca0` | DIFF, 96.7% | 24/24 CFG blocks and 14/14 branches agree; slot-seed and zero-fill setup differ. |
| `0x80023178` | DIFF, 45.1% | Wait-controller register lifetimes differ; prior direct strict verdict was 93.52941%. |
| `0x80023288` | SAME | Wait sibling. |
| `0x800232ac` | SAME | Wait sibling. |

The additional wait sibling `0x8002332c` also remained focused `SAME` as a
regression control. A temporary `signed char` spelling for the directory
slot-digit array, including its `atoi` casts, left all five directory
listings unchanged and was discarded. The seed-byte identities and their
original defining TU remain unresolved. No source or configuration change
was retained from this batch; no repository tests or linked build were run.

## Ten-function menu/card strict recheck (2026-10-02)

All ten assigned GAME functions were rebuilt as isolated candidate objects and
compared with `objdiff-cli` against their delinked retail objects. The strict
percentages below are current, not cached report values. Focused `kf try`
listing comparisons and retail blocks/calls were also checked. None reached
strict exact; no source or ownership change was retained.

| Function | Strict | Focused | Bounded verdict |
| --- | ---: | ---: | --- |
| `0x8001b554` card browser | 98.478264% | 84.6% | Probe result stays in `v0` in the candidate where retail copies it to `a0`; later CFG and calls agree. |
| `0x8001930c` map preview | 98.263630% | 80.2% | Retail's 64-byte frame and independent map-index addition differ from the 56-byte candidate frame and reassociation. Reordering the source expression worsened focused similarity to 79.6%, so it was reverted. |
| `0x8001a4f0` item/magic controller | 99.743590% | 96.0% | Initial 74-entry loop uses `s0` for the index in retail and `a3` in the candidate; widths, constants, and loop effect agree. |
| `0x8002083c` two-option item preview | 99.658820% | 73.1% | Retail reserves 64 more stack bytes; four live matrix objects and the exact adjacent helpers do not justify padding. |
| `0x8001bf68` card format flow | 97.123890% | 87.7% | Probe-status and dialog-constant registers differ; adjacent `0x8001c12c` remains strict exact. |
| `0x8001f8b8` preview choice | 94.364640% | 84.8% | Retail positions the wait/return arm before the input loop; the natural post-draw loop trial previously regressed and was reverted. |
| `0x80022ca0` card writer | 95.896774% | 96.7% | Retail initializes the path, then reads signed slot-seed bytes before clearing the digit tail; candidate schedules the independent clear earlier. The card-write call path and 24-block/14-branch CFG agree. |
| `0x800226ec` card directory | 93.605040% | 88.6% | Retail signed byte loads and first `memset` setup differ; the 13-block/7-branch CFG agrees. |
| `0x80022b74` card reader | 93.666664% | 84.2% | Retail's frame is 80 bytes versus the candidate's 72; the 9-block/4-branch CFG agrees, with no supported additional live object. |
| `0x80023178` title digit writer | 93.529410% | 45.1% | Quotient/header register lifetimes differ; the adjacent payload sum, wait, and clear-event helpers remain focused `SAME`. |

The directory unit's six-byte `.rodata` and initialized bytes still match in
content; its data-placement ownership conflict is being audited separately.
No repository tests, lint, full build, or broad match were run.

## Ten-WIP source-shape check (2026-10-01)

Fresh focused rebuilds rechecked ten non-exact GAME menu/card functions. The
stored strict percentages remain stale while source and owner work continues;
the focused results below are listing comparisons, not exact closure.

| Address | Focused | Bounded verdict |
| --- | ---: | --- |
| `0x8001a4f0` | 96.0% | Initializer index/constant registers differ; 22-block CFG remains supported. |
| `0x8001b554` | 84.6% | Retail copies the card probe result from `v0` to `a0` before its two guards; source retains the same branches and call set. |
| `0x8001bf68` | 87.7% | Probe status and dialog constants use other registers; exact `0x8001c12c` remains `SAME`. |
| `0x8001f8b8` | 84.8% | Retail places the wait-and-return arm before the input loop; a natural post-draw loop probe worsened the listing to 80.8% and was reverted. |
| `0x8001fb8c` | 83.5% | Retail reserves 48 stack bytes versus the probe's 40; no further live object is supported. |
| `0x8001fc94` | 99.9% | Two independent lower-panel color/Y setup instructions are reversed; referents and later listing align. |
| `0x8002083c` | 73.1% | Retail reserves 64 bytes beyond four live `MATRIX` locals; two-option and heading siblings remain `SAME`. |
| `0x80022058` | 78.5% | Retail's extra eight frame bytes move the fifth O32 argument load; digit/style operations align. |
| `0x800228c8` | 67.6% | Retail uses signed title-byte loads and a two-byte offset walk. A semantically equivalent offset-walk probe fell to 66.9% and was reverted. |
| `0x800349bc` | 44.8% | Retail spills the pad state, whereas the probe retains it in a saved register; the four quad paths and calls agree, and both transition siblings remain `SAME`. |

The reader's `0x8006d6a4/5` seed identities still have no proved defining TU.
None of these source-equivalent trials justified a retained edit, and none of
the ten reached exact. Only focused builds and retail queries were used.

The provisional `memory_card_buffer_storage` extent covers raw SDK-private
addresses used by exact vendored `CD_readm`, `CD_readsync`, `CdSearchFile`, and
`CD_cachefile` bodies. Their seventeen reviewed load pairs cannot be assigned
to the game card buffer from address containment alone; those referents remain
unattributed pending an owner boundary.

## Six remaining disjoint menu/card WIPs (2026-10-02)

The current strict inventory shows the proposed card-choice and item-controller
alternates already exact, so this follow-up used the six live disjoint WIPs
below. Each was rebuilt as an isolated candidate object, checked with strict
`objdiff-cli`, and compared with a focused listing. No additional source edit
was retained.

| Function | Strict | Focused | Verdict |
| --- | ---: | ---: | --- |
| `0x8001fb8c` window drawer | 99.787880% | 83.5% | Only an unexplained eight-byte frame increase remains; complete layout data and sprite definitions are exact. |
| `0x80021c8c` display-state enter | 99.956985% | 97.2% | Only frame size and saved `ra` offset differ; display-state exit remains exact. |
| `0x80022058` number formatter | 97.390000% | 78.5% | Retail reserves eight stack bytes, moving the fifth O32 argument load; style and digit operations align. |
| `0x800228c8` card reader | 85.156250% | 67.6% | Retail uses signed byte loads for two-byte encoded title glyphs and a byte-offset walk. Explicit signed-byte lvalue casts produced an identical focused listing and were reverted. |
| `0x8001876c` location menu | 96.366460% | 90.8% | Two sentinel constants use swapped saved registers; retail reloads the result from its stack slot on the exit path. Exact `0x800189f0` remains unchanged. |
| `0x800210ac` glyph drawer | 99.669040% | 90.4% | Retail reserves eight extra stack bytes and allocates the glyph arithmetic to the opposite volatile registers; call and referent sets agree. |

An off-tree union view of the reader's two-byte glyph (`s16` code with two
signed byte lanes) was also rejected. GCC combined the input bytes with
`lbu`/shift/or instead of emitting retail's two `lb`/`sb` writes followed by
a stack halfword load. The candidate grew to a 752-byte frame and direct
strict text fell from 85.156250% to 69.575000%; the exact format sibling
stayed 100%. The existing byte-lane source remains preferable.
A separate two-byte signed array with a halfword read preserved the 640-byte
body and exact format sibling but scored 83.568750%; it likewise failed to
recover retail's signed-load/store sequence, so it was discarded.

The source evidence does not justify artificial stack padding or register
carriers for these residues. No tests, lint, full build, broad match, or README
edit was run.
