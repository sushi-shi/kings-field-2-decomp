# GAME player sound and reaction: fresh strict controls

A current narrow safe delink, focused rebuild, and isolated strict objdiff
cover 20 claims in `game.player_collision_sound` and `game.player_reaction`.
Neither target withholds a relocation or function. This is a verdict refresh;
the source and curated identities were not changed. The fuller raw field and
call audit is in `kf2-game-player-actor-trajectory.md`.

| Unit | Strict exact claims | Remaining WIP |
| --- | --- | --- |
| `player_collision_sound` | `0x80027928`, `0x80027988` | `0x800279cc` is 98.23967% over 1,452 bytes. Its 70/70 CFG blocks and 37/37 branches agree. Target/candidate `.rel.text` counts are 141/137: the four-row deficit is two `player_state` HI16/LO16 pairs, while the same player fields are still referenced through a retained base. |
| `player_reaction` | `0x80028ec0`, `0x80028fa8`, `0x80029014`, `0x80029168`, `0x800291d0`, `0x800291ec`, `0x800293d4`, `0x80029428`, `0x80029464`, `0x800294f8`, `0x80029570`, `0x800295f8`, `0x80029624`, `0x800296e8`, `0x8002975c`, `0x800297b4` | `0x8002985c` is 99.26569% over 4,396 bytes. Its 142/142 CFG blocks, 79/79 branches, and 696/696 ordered text relocation type/symbol rows agree; 672 relocation sites are identical. The first raw differences change the clamp immediate's register and subsequent instruction schedule. |

The reaction unit's 32-byte DATA and 76-byte RODATA are strict exact. No
source-backed field, width, call, referent, or control-flow correction emerged
from this refresh, so both WIP bodies retain their existing C.

A later safe 26-claim control adds `player_interval_71_80` (two claims),
`player_damage_reaction` (one), and `event_counter` (three) to the same
reaction/sound graph. Three focused carves admitted 2,139 target relocations
with none withheld; direct strict comparison gives **23 exact and three WIP**.
The damage reaction and all three event-counter functions are exact. The
interval unit's `0x80028998` caller is exact; its 28-byte `0x8002897c`
predicate remains 88.57143%. Retail branches with zero moved to `v0` in the
delay slot and returns with a nop; the current source emits the same 71–80
tests but carries the result through `v1` and moves it in the return delay
slot. Natural early-return, explicit `if`/`else`, and late-return off-tree
spellings scored 55%, 55%, and 47.857143% strict, so none was retained.
The reaction and sound WIPs retain the raw first differences above; no new
source-backed correction emerged. The reaction's 99.26569% figure used the
pre-promotion full-unit target. After 41 direct-call rows were reviewed, a
fresh isolated one-VA target measured the unchanged `0x8002985c` source at
99.25660%. That target-vintage difference is not a source regression; the
post-promotion score supersedes the earlier number.
