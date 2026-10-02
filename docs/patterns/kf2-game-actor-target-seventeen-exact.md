# GAME actor target and initialization exact controls

Focused `kf try --unit` builds showed identical listings for all seventeen
functions below. Each has an independent safe one-VA GAME delink and direct
strict objdiff `.text` result of **100%**. No source or profile was changed
in this read-only pass.

| GAME VA | Unit | Retail text bytes | Strict verdict |
| --- | --- | ---: | --- |
| `0x800395c8` | `game.actor_select_best_target` | 252 | Exact |
| `0x800397a8` | `game.actor_target_reset` | 48 | Exact |
| `0x80038dc4` | `game.actor_copy_group_defaults` | 116 | Exact |
| `0x80038efc` | `game.actor_home_wrapper` | 36 | Exact |
| `0x80038f20` | `game.actor_home_wrapper` | 208 | Exact |
| `0x80038d04` | `game.actor_set_home_position` | 192 | Exact |
| `0x800157ac` | `game.actor_random_scalar` | 76 | Exact |
| `0x800157f8` | `game.actor_random_scalar` | 84 | Exact |
| `0x8001584c` | `game.actor_fixed_interpolation` | 32 | Exact |
| `0x8001586c` | `game.actor_fixed_interpolation` | 72 | Exact |
| `0x800158b4` | `game.actor_fixed_interpolation` | 100 | Exact |
| `0x800397d8` | `game.actor_byte_0c_set` | 44 | Exact |
| `0x80039804` | `game.actor_byte_0c_set_changed` | 56 | Exact |
| `0x80039080` | `game.actor_pool_clear` | 80 | Exact |
| `0x80038cc8` | `game.actor_pool_find` | 60 | Exact |
| `0x80038ff0` | `game.actor_prepare_initialize` | 88 | Exact |
| `0x80039048` | `game.actor_prepare_initialize` | 56 | Exact |

These exact results protect the target and initialization call family while
the separate actor damage, group-fixup, and effect dispatcher functions remain
WIP. The evidence establishes current-object equality, not historical
compiler or original-TU attribution.
