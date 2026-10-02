# GAME player focused ten

Ten disjoint GAME player units were compiled individually from the current
sources with their complete pinned manifest profiles, then compared to the
existing safe-delink target objects using isolated strict objdiff. No broad
build, repository test, source edit, or relocation edit was made.

Eight units are fully exact: `player_camera_turn` (2 functions),
`player_weapon_transform_power` (2), `player_distance_margin`,
`player_reset_view`, `player_damage_reaction`, `player_status_cap`,
`player_core_run` (10), and `player_collision_bounds`. These 19 exact
functions preserve target/candidate `.rel.text` counts of 180/180, 30/30,
3/3, 46/46, 45/45, 14/14, 344/344, and 22/22 respectively.
The two neighboring `player_collision_sound` helpers at `0x80027928` and
`0x80027988` are also exact, making 21 existing exact claims in this screen.

| Function | Fresh strict text | Module `.rel.text`, target/candidate | Verdict |
| --- | ---: | ---: | --- |
| `0x80027f78` collision response | 95.91228% | 51/59 | WIP. Prior raw audit has 27/27 CFG blocks, 14/14 branches, and six direct calls in order. The four extra candidate HI16/LO16 pairs rematerialize `player_state` fields; they do not indicate a distinct missing retail referent. The earlier pointer-scope trial regressed and was discarded. |
| `0x800279cc` collision sound/controller | 98.23967% | 141/137 | WIP. Prior raw audit has 70/70 CFG blocks and 37/37 branches. Retail materializes two more `player_state` HI16/LO16 pairs; the candidate reuses an earlier base. The two adjacent sound/death helpers remain exact. |

The current player-collision sources already encode the known calls, typed
fields, and control joins. These relocation-count differences reflect address
materialization frequency, so neither target rows nor real C references were
removed merely to equalize counts. No additional exact result was banked.
