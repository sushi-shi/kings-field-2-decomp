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
