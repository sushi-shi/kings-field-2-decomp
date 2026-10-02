# GAME map, resource, and effect ten

These ten GAME functions connect the collision grid, map-resource frame,
map-object placement, actor behavior, and effect update calls. Retail
disassembly, CFG, proven callers/callees, data referents, neighboring claims,
current source, and vendored provenance were checked under `kf sema --image game`.
KF1's effect dispatcher is a semantic analogue for `0x80042650`, but its smaller
kind table and body do not establish the KF2 source. No function in this batch
has a supported vendored attribution.

| GAME address | Retail evidence | Final verdict |
| --- | --- | --- |
| `0x8002aaa4` | A `0xb60`-byte collision dispatcher called by three height wrappers; its navigator CFG has 174 blocks and candidate internal table branches. | **WIP, unclaimed**; the table and complete query-output owner remain unproved. |
| `0x8002c670` | An eleven-block collision-mask builder calls `rcos`, `rsin`, the exact grid sampler, and scan/fill helpers. It interpolates seven signed-halfword pairs from `0x80067874`. | **WIP, unclaimed**; that initialized table and `0x801b5a70` scan state lack complete source owners. The later `0x80067890` references belong to a distinct candidate range. |
| `0x8003247c` | A 96-block resource/frame child of `0x800335a0` calls both TMD/VAB range updaters, world-model rendering, mask queries, and repeat-store helpers. | **WIP, unclaimed**; its reviewed identity matrix does not establish the full map/render record or a humane function body. |
| `0x80035894` | A 64-block placement controller called by the resource transition; it calls both exact pattern helpers and collision-height wrappers. | **WIP, unclaimed**; candidate pattern data near `0x80067890` and indirect control remain unresolved. |
| `0x80036ed4` | A 329-block map-event dispatcher called by `game_main_loop`, with proven spatial sound, collision, placement, and effect calls. | **WIP, unclaimed**; two indirect table paths and the event-record field family are incomplete. |
| `0x8003d184` | A 410-block actor-behavior dispatcher called by the actor frame service, with proven animation, collision, sound, and effect successors. | **WIP, unclaimed**; three indirect branches and broad actor-event data ownership prevent a truthful C claim. |
| `0x8003fa68` | The current-effect cooldown gate branches by four collision modes and retail makes four separate calls to `0x8002b9d4`. | **WIP, 70.73333% strict**; the pinned compiler merges those calls into one arm (14 retail versus 15 compiled CFG blocks). No synthetic side effect or call suppression was retained. |
| `0x8003fb94` | The 15-argument effect precursor has the retail damage/actor call set and 11/11 CFG blocks. | **WIP, 87.79105% strict**; prologue save and mask/argument scheduling differ. Reversing the two mask declarations worsened the focused listing and was discarded; its nine adjacent effect-update functions remain exact. |
| `0x80040308` | A 116-block effect constructor calls the exact pool initializers, spatial sound, and `rand`. | **WIP, unclaimed**; the indirect jump through candidate table `0x8001249c` and variant record/data owner are incomplete. |
| `0x80042650` | A 446-block effect dispatcher uses a 123-entry kind table and a five-entry phase table, with 206 decoded direct calls from the effect sweep. | **WIP, unclaimed**; the two indirect dispatches, candidate relocation rows, and provisional collision-cache words prevent a complete source claim. Its table evidence is detailed in [the dispatcher dossier](kf2-game-effect-dispatch-map.md). |

The fresh `kf match --image game` pass relinked 147/147 target units. It
confirmed the two sourced WIP scores and all nine exact siblings of
`game.effect_update`. The global edge-check still stops on the three known,
unrelated TMD/map-object `.rodata` addends. No source/config change, repository
tests, bank, or commit were made in this batch.

## Current-source map and resource ten-function follow-up

The earlier unclaimed rows above describe the pilot before the present source
claims. Fresh isolated strict objects and focused quick builds now give these
ten GAME WIP verdicts. All quoted CFG and branch counts are retail/probe.

| Address | Strict text | CFG / branches | Remaining evidence-bound gap |
| --- | ---: | --- | --- |
| `0x80015918` | 95.49419% | 41/41; 24/24 | Ballistic discriminant and time selection agree; result-register and arithmetic scheduling differ. Both adjacent helpers are exact. |
| `0x80015d58` | 89.03145% | 1/1; 0/0 | Seven-archive startup has the reviewed calls; six retail-only HI16/LO16 pairs target fixed workspaces without proved source owners. Its 83-byte literal section is exact. |
| `0x80015fd4` | 89.710144% | 3/3; 1/1 | Transition pump keeps its control and calls; the unowned TMD workspace address still compiles as a literal form. |
| `0x80016260` | 98.790085% | 67/67; 49/49 | Request byte widths and 135 ordered referents agree; stack-byte register assignment and branch layout remain. |
| `0x80016820` | 99.193474% | 62/62; 32/32 | Phase control and 128-byte callback table are exact; three signed-low workspace pairs are absent from candidate literals because complete owners remain unproved. |
| `0x8002c670` | 91.624245% | 11/11; 4/4 | Mask-sweep call sequence and 28-byte shape table agree; state-byte store and loop-address scheduling differ. |
| `0x80034f90` | 97.86822% | 14/14; 7/7 | Signed pattern interpolation and layer writes agree; saved-register/address order remains. |
| `0x80035194` | 89.59545% | 51/51; 26/26 | Both rotated occupancy layers and field masks agree; retail uses a 40-byte frame against the probe's 32, and the probe hoists repeated mask tests. |
| `0x80036190` | 98.56115% | 15/15; 8/8 | Interaction calls and conditions agree; angle-call result move and independent stack load exchange order. |
| `0x80036464` | 95.32258% | 12/12; 3/3 | Effect-pool/sequence flow and calls agree; object ID and height offset use other saved registers. |

The current `game.map_object_init_records` object is strict 100% for its
2,020-byte body, 270-byte initialized pattern claim, and 1,016-byte switch
table, matching its existing exact dossier. The two other `map_object`
helpers and two trajectory helpers remain strict exact. No new source fact
supports an edit or exact claim in this ten-function follow-up. Only focused
quick builds and isolated strict objdiff were run.

The transition request's `state_8017d118` is a 28-byte typed object by its
layout check and retail BSS symbol. The pinned GCC emits it as a 32-byte
COMMON allocation; a focused assembly/object audit shows that the extra four
bytes are allocation rounding, not evidence for another C field. Its shared
type remains unchanged.

## Current-source event and effect eleven-function follow-up

A further isolated strict/focused pass covered five event/message functions
and six connected actor/effect functions. The current sources preserve the
reviewed calls, byte and halfword fields, and data identities. The following
are final bounded verdicts for this pass; CFG and branch counts are
retail/probe, and indirect jumps remain unresolved where noted.

| GAME address | Strict text | CFG / branches | Verdict |
| --- | ---: | --- | --- |
| `0x8003a318` | 99.86911% | 26/26; 13/13 | Two incoming stack values use exchanged volatile registers; five visible instruction differences follow that assignment. |
| `0x8003a614` | 96.91011% | 6/6; 3/3 | Damage fields and four calls agree; retail rematerializes the player-state address where the probe retains a base. Its sibling `0x8003a778` is exact. |
| `0x8003c3e0` | 99.64539% | 23/23; 11/11 | Yaw-normalization intermediates use exchanged volatile registers; three group-position siblings remain exact. |
| `0x8003c614` | 86.041916% | 45/45; 15/15 | Retail retains eleven position-helper and nine constructor call sites; the probe merges them to seven and ten. Its 123-row, 19-class table identity is intact, but code-layout addends differ. The indirect table jump limits CFG reachability. |
| `0x8003fa68` | 70.73333% | 14/15; 6/6 | Four retail collision calls with distinct modes fold to one compiled call despite four source expressions. No forced side effect was added. |
| `0x800460a0` | 99.268295% | 6/6; 2/2 | Animation phase and two calls agree; step/half-step saved-register assignment remains. |
| `0x800461a0` | 99.12676% | 14/14; 5/5 | Marker cursor setup exchanges argument registers; its marker leaf sibling is exact. |
| `0x800462bc` | 98.68132% | 46/46; 21/21 | Event byte stream and 57 text referents agree; one load-delay `nop` shifts a 64-byte jump-table addend. The indirect jump is unresolved. |
| `0x800475d8` | 99.166664% | 55/54; 29/29 | The probe schedules `remove_object = 0` into a branch delay slot rather than retail's fallthrough block; 62 text referents agree. |
| `0x80047c98` | 99.81618% | 85/85; 55/55 | World event calls and 72 referents agree; rotation and constant-one saved registers exchange roles. |
| `0x800489ac` | 98.82883% | 23/23; 7/7 | Restore interpreter exchanges actor/sentinel argument registers; its 64-byte jump table is exact. The indirect jump is unresolved. |

The unchanged `game.effect_update` ten functions and
`game.effect_spawn_motion` two functions were reconfirmed strict 100% and
focused `SAME`; these are existing exact controls, not new closures. None of
the WIP differences above establishes a missing field, call, or table row, so
no C or curated metadata was changed. This pass used focused quick builds
and isolated strict comparisons only.

## Live structural-gap recheck

`game.event_command_dispatch` is already strict exact in the current tree:
its 3,156-byte text, five eight-byte initialized rows, 40-byte `.data`, and
140-byte switch table all compare at 100%. The older event-command WIP prose
predates that closure and is not a new exact claim. An independent check of
`game.player_collision_bounds` also confirms its 172-byte function is exact.

Three apparent GAME control gaps remain source-stable after fresh retail and
focused inspection. `0x8003fa68` is 70.73333% strict (14/15 blocks): retail
uses four separate collision calls with modes `0xa1`, `0x31`, `0xb1`, and 1,
while GCC merges the four ordinary C switch arms into one call. The modes,
call arguments, cooldown gate, and case domain agree. `0x800475d8` is
99.166664% strict (55/54 blocks): the sole extra retail fallthrough block
holds a local zero assignment that GCC puts in the preceding branch delay
slot; all 62 ordered referent identities agree. `0x8002c670` is 91.624245%
strict (11/11 blocks, four branches): all 26 direct calls and its 28-byte
shape table agree, while the probe forms two scan-state flag stores from
global address pairs rather than the saved `render_mask_scan_state+0x0c`
pointer and schedules loop-field updates differently. Earlier natural
shared-pointer trials regressed the focused object. Two fresh off-tree
spelling controls also regressed strict text from 91.624245%: a shared typed
state pointer for setup fields gave 86.79394%, and the same pointer for only
the two layer-mask flags gave 88.935356%. These observations do
not justify duplicate calls, artificial locals, or raw-offset field views;
no source or metadata edit was retained. Only focused quick builds and
isolated strict comparisons were run.

The released player collision controller `0x800279cc` was also rechecked at
**98.23967%** strict with 70/70 CFG blocks, 37/37 branches, and the two
adjacent collision-sound helpers exact. Retail reserves 72 stack bytes to
the probe's 64 and rematerializes the same `player_state+0x110` landing-bob
address twice where the probe carries a base pointer. The halfword read and
write, twelve direct calls, and branch domain remain aligned. No new field
access or source edit follows from those two relocation pairs.

The actor fixed-curve recipient `0x80039c94` now compares at **98.11751%**
strict (1,668 retail versus 1,660 probe bytes), with 72/72 CFG blocks and
46/46 branches. Its prior 72/71 CFG note predates the committed source
correction; the adjacent 128-byte curve helper remains exact. All 36
ordered relocation classes and external targets agree; only five local
jump addends shift with code layout. This includes eight direct curve calls
and the indirect slot-18 callback. The first remaining differences
assign incoming halfwords and the aggregate to other saved registers;
the speed-cap tail also keeps its value in a different register. The
callback target is still dynamic and unproved, but its observed argument
slots are modeled. No type, call, or field correction is supported here.

The neighboring actor vertical-motion controller `0x8003b5d0` remains
**96.42041%** strict and 94.3% focused, with 40/39 blocks and 21/21
branches; its three preceding functions are exact. Retail's state-`0x10`
settle arm jumps through a shared state-byte reset, while GCC places that
zero store in a direct return jump's delay slot. The state-`0x20` arm also
orders its speed store before a separate cache load where the probe
schedules those independent operations differently. The previously tried
shared-reset label regressed both strict and focused results, so the
retail-backed state branches and field widths remain unchanged.

The collision-height rasterizer `0x8002bfd4` remains **73.91262%** strict
and 35.5% focused, with 22/22 blocks and 11/11 branches. Retail loads both
unsigned 16-bit map origins before forming absolute start/end coordinates;
the probe cancels the common origins while computing deltas and allocates an
otherwise unused 16-byte leaf frame. Both axis loops, grid-byte stores,
step signs, and local jump targets agree. Its 3,520-byte default-row datum
and eleven exact sibling functions remain unchanged. The nearby collision
snapshot `0x8002b874` likewise keeps its retail fields and branches: retail
stores each selected radius before loading interaction height, while the
probe schedules the height load first. Previously tested branch-local and
height-width spellings did not improve it. Neither function has a supported
source correction from the observed instruction order.
An off-tree spelling with four explicit shifted endpoint locals compiled to
the same 73.91262% rasterizer object and left every exact sibling unchanged;
the optimizer still cancels the common origins in the deltas.
Using signed rather than unsigned shifted endpoint locals gave the same
object, so the current unsigned shift and signed low-half tests remain the
only instruction-backed width facts.

## Current GAME source-claim census

The image-qualified GAME rows in
`config/retail/functions.tsv` contain **860 distinct starts**. Comparing
their VAs with every `ADDRESS(0xVA, ...)` claim in `src/game/**/*.c` and the
GAME rows in `config/retail/functions_vendored.tsv` gives **522 source
claims**, **338 vendored exclusions**, **zero overlap**, and **zero unclaimed
non-vendored starts**. The check used Python `csv.DictReader` on both TSVs
after filtering comment lines, a recursive `Path('src/game').rglob('*.c')`
scan with `re.findall(r'ADDRESS\(0x([0-9a-fA-F]+),', source)`, and set
differences/intersection keyed by integer VA under `GAME.EXE`. It counts
curated starts and claims, not strict exact objects; the matching campaign
continues on the sourced WIPs. No generated inventory overwrote the curated
files.

## Transition workspace referents

GAME `resource_transition_step` at `0x80016820` remains **99.193474%**
strict, with 62/62 CFG blocks and 32/32 branches. Retail forms `0x8019e138`
twice with `lui 0x801a; addiu -7880`, for the phase-1 archive destination
and phase-2 active callback table. Phase 3 forms `0x8012da68` with
`lui 0x8013; addiu -9624` for the TMD archive destination. These three
reviewed HI16/LO16 pairs account for the six missing candidate relocation
rows (229 versus 235). The phase calls and seven-entry switch are already
aligned. `resource_transition_request` stays **98.790085%** with 135/135
ordered referents and 67/67 CFG blocks.

The callback destination starts four bytes after the curated `effect_state`
extent, but its archive record length is unknown. The TMD destination starts
16 bytes after `display_primitive_memory`; the next observed VAB slot gives
only a `0x37000` maximum gap, not an allocation size. Neither address has a
proved complete object, owning TU, or original definition mechanism. The
current literal C pointers remain provisional; binding them to new globals
or relocation symbols would assert unsupported ownership. No C or metadata
edit was retained.

## Mask-window layer selection control

GAME `func_8002c670` remains **91.624245%** strict with its 28-byte shape
table exact. At retail `0x8002c81c..0x8002c840`, each layer arm stores the
first mask byte through the saved scan-state pointer, then a single join
stores the second mask byte. The current source has the same values and
reachable paths, but GCC duplicates the second store and materializes its
global address. A natural local for the second value, tested with both `u8`
and `s32`, lowered strict text to **85.943436%**. A narrowly scoped typed
scan-state pointer lowered it to **88.935356%**. Both trials preserved the
exact table and were reverted; no C change or new owner claim remains.

A fresh direct strict recheck retains **91.624245%** over the 1,980-byte
retail body. The first raw difference at body `+0x74` is the register chosen
for the interpolated halfword sum. At `+0x1ac..+0x1cc`, retail joins for one
second mask-byte store; the candidate duplicates that store and rematerializes
the scan-state address. An off-tree whole-function typed pointer to the same
scan state reduced strict text to **80.004040%** (1,828 candidate bytes)
without changing the layer values. It was discarded; source and data remain
unchanged.
