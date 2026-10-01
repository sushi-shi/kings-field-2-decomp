# GAME render-cache and placed-model exact pass

These ten GAME.EXE functions connect TMD vertex-cache selection, the record
pool, and placed-model expansion to the renderer. Actor-animation ownership
was checked before this pass. Fresh focused builds reported SAME for all ten,
and direct objdiff of each rebuilt unit confirmed strict 100%.

| VA | Function | Unit | Verdict |
| --- | --- | --- | --- |
| `0x80034070` | `func_80034070` | `game.asset_vertex_count` | Exact, direct strict 100% |
| `0x80034344` | `func_80034344` | `game.asset_vertex_count` | Exact, direct strict 100% |
| `0x800345e4` | `asset_vertex_count` | `game.asset_vertex_count` | Exact, direct strict 100% |
| `0x80034644` | `pool_reset` | `game.pool` | Exact, direct strict 100% |
| `0x80034674` | `pool_mark_allocated` | `game.pool` | Exact, direct strict 100% |
| `0x800346b0` | `pool_record_release` | `game.pool` | Exact, direct strict 100% |
| `0x800346f8` | `pool_release_all` | `game.pool` | Exact, direct strict 100% |
| `0x80034764` | `pool_release_stale` | `game.pool` | Exact, direct strict 100% |
| `0x800347d0` | `pool_allocate` | `game.pool` | Exact, direct strict 100% |
| `0x80034818` | `func_80034818` | `game.map_placed_expand` | Exact, direct strict 100% |

Their current typed source, calls, data referents, and relocation models need
no correction on this probe. No tests, full build, broad match, or banking
were run.
