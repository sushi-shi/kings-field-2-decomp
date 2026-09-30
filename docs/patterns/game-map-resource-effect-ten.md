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
