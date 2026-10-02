# GAME map, collision, and effect storage: 26-claim audit

This is a `GAME.EXE` owner audit. An identity's owner below means the current C
source that defines or claims it, not a recovered original translation unit.
The retail program has linked bytes but no surviving data symbols. Sizes and
source placement are kept at their existing evidence tiers; a focused listing
match does not settle BSS allocation or switch-table code-offset addends.

| Retail VA and extent | Current claim and evidence | Verdict |
| --- | --- | --- |
| `0x8001134c`, `0xc4` | `collision_shape_dispatch.c` has 49 raw-reviewed kind pointers. | Source RODATA owner supported; dispatcher body WIP. |
| `0x80011484`, `0x3f8` | `map_object_init_records.c` has 254 object-record switch pointers. | Source RODATA owner supported; body WIP. |
| `0x8001187c`, `0x44` | `map_object.c` covers 17 bounded template-kind pointers. | Source RODATA owner supported; original TU WIP. |
| `0x800118c4`, `0x54` | `map_object_spawn_scatter.c` covers 21 bounded action pointers. | Source RODATA owner supported; original TU WIP. |
| `0x8001191c`, `0x3bc` | `map_object_action_update.c` covers 224 primary, nine secondary, and five tertiary pointers with one zero separator. | One 239-word source RODATA span; dispatcher body and code-offset addends WIP. |
| `0x8001249c`, `0x1ec` | `effect_constructor.c` claims the 123-kind switch table. | Current C owner aligned in this pass; table code-offset addends and constructor WIP. |
| `0x8001268c`, `0x1ec` | `effect_update_dispatch.c` claims 123 kind pointers. | Supported first table inside one `0x204`-byte source RODATA span; body WIP. |
| `0x8001287c`, `0x14` | The same unit claims five bounded phase pointers, after a zero word at `0x80012878`. | Supported second table; no overlapping new global. |
| `0x800128d0`, `0x8c` | `event_command_dispatch.c` claims 35 map-scene command pointers. | Supported separate table beginning after the effect unit; event body WIP. |
| `0x800667fc`, `0xa` | `collision_grid_sample.c` defines two five-byte default layers. | Initialized source datum already byte-exact. |
| `0x80066ab4`, `0xdc0` | `collision_height_wrappers.c` defines 80 mutable 44-byte default rows; startup can replace them from the archive. | Initialized source datum already byte-exact; collision wrapper WIPs do not change it. |
| `0x80067874`, `0x1c` | `map_mask_window_sweep.c` defines seven signed near/far pairs used by a bounded walk. | Initialized source datum already byte-exact; sweep body WIP. |
| `0x80067890`, `0x10e` | `map_object_init_records.c` defines nine groups of three cell patterns. | Initialized source datum already byte-exact; original TU WIP. |
| `0x8006d6e4`, `0x8` | First SDK `SVECTOR` passed as a start offset to `func_80036b68`. | Source-defined in `map_object_action_update.c`; first of four byte-exact adjacent vectors. |
| `0x8006d6ec`, `0x8` | First end offset, with its own reviewed HI16/LO16 pair. | Same current C owner; original linkage and TU WIP. |
| `0x8006d6f4`, `0x8` | Second start offset, with its own reviewed pair. | Same current C owner; original linkage and TU WIP. |
| `0x8006d6fc`, `0x8` | Second end offset; the next distinct datum begins at `0x8006d704`. | Same current C owner; all four vectors fill exactly `0x20` bytes. |
| `0x8006d704`, `0x4` | `effect_constructor.c` defines a zero-initialized modulo-four trail index. | Observed scalar and source address; original loaded allocation/TU candidate. |
| `0x8006d708`, `0x8` | `effect_spawn_zero_direction.c` passes this zero `SVECTOR` to the exact constructor wrapper. | Initialized source bytes and wrapper are exact. |
| `0x8009a5a8`, `0x4` | Constructor reads/writes a kind-102 sound cooldown near SDK/CD storage. | Keep original BSS owner candidate; source definition does not prove its TU or neighbors. |
| `0x801749d0`, `0x8744` | `map_object_reset.c` defines 396 68-byte records plus the typed prefix/trailer; startup clear and field offsets bound it. | Complete runtime span supported; original TU WIP. |
| `0x8019b6a8`, `0x2a8c` | `effect_reset.c` defines a typed state bounded by the `0xaa3`-word startup clear and 128-record pool. | Current C owner aligned in this pass; focused reset 3/3 SAME. GCC requests `0x2a90` COMMON bytes, so strict allocation remains unresolved. |
| `0x801b5a70`, `0x20` | `map_mask_window_sweep.c` defines a typed cursor; direct mask/render consumers reach its observed fields. | Current C owner known, but original extent/allocation/TU remain candidate. |
| `0x801c7068`, `0x8` | `effect_scatter.c` uses signed motion halfwords and an SDK `SVECTOR` view. | Three scatter texts are strict exact; original allocation boundary remains address-only. |
| `0x801d9628`, `0x900` | Constructor selects four 576-byte slots and copies 24 rows per slot. | Modeled minimum trail span; no independent clear proves the original allocation end. |
| `0x801c7540`, `0x11844` | Startup clears the shared player/map region; collision uses owner-relative shape-bank and cache offsets through `+0x11842`. | Existing candidate owner retained. Map-cell row bound and shape-bank allocation still overlap in the provisional type, so no narrower global is claimed. |

The 37 Ghidra-split pointer candidates at `0x80067a48..0x80067adb`
are outside this game-data cohort. Their direct code xrefs lie in
FID-supported Sony `LIBCD.LIB` functions (`CdReadSync`, `getintr`, `CD_init`
and neighbors), with raw words pointing into adjacent CD data. They must not
be claimed as game-owned pointers without an archive/object attribution.

A range filter over curated GAME relocation targets found no references into
the five gaps `0x801749cc..0x801749d0`, `0x8017d114..0x8017d118`,
`0x8019e134..0x8019e138`, `0x801b5a90..0x801b5d60`, or
`0x801c7070..0x801c7078`. This is a negative result for the current
relocation model, not proof that linked RAM contains no SDK or linker-owned
allocation there; no gap is merged into a neighboring source object.

For the two identity owner changes in this pass, a safe focused carve of
`0x80040308` and `0x80045cc0` withheld no relocations or functions.
`game.effect_reset` remained 3/3 `SAME` focused listings;
`game.effect_constructor` remained 0/1 `SAME` and its switch-table offsets
remain WIP. No C body or storage extent changed. No repository tests, lint,
broad match, full build, or link build were run.

An isolated strict `objdiff` comparison of the related ten source units found
24/27 function texts at 100%. This compiles each current C source to a
temporary object and compares it with its focused retail target object; it is
not a full-image link verdict.

| Unit | Strict function verdict | Data verdict |
| --- | --- | --- |
| `game.map_object_spawn_scatter` | Four of four 100% (`0x800365d8`, `0x800366fc`, `0x800368b4`, `0x80036944`). | `.rodata` 84/84 bytes 100%. |
| `game.map_object_motion` | `0x80036b68` 100%. | No owned initialized section in this comparison. |
| `game.map_object_vertex_world` | `0x800369b8`, `0x80036ad8` 100%. | No owned initialized section in this comparison. |
| `game.map_object` | `0x800363bc`, `0x800363dc` 100%; `0x80036190` 98.56115%, `0x80036464` 95.32258% WIP. | `.rodata` 68/68 bytes 100%. |
| `game.map_object_reset` | Six of six 100% (`0x80035504..0x800357a0`). | `.bss` 34,628 bytes reports 0%; allocation form unresolved. |
| `game.effect_spawn_zero_direction` | `0x80041d7c`, `0x80041e0c` 100%. | `.data` 8/8 bytes 100%. |
| `game.effect_reset` | Three of three 100% (`0x80045cc0..0x80045d1c`). | `.bss` 10,892 bytes reports 0%; candidate COMMON allocation is four bytes larger than the retail state span. |
| `game.effect_scatter` | Three of three 100% (`0x80042298..0x800424f0`). | `.bss` 8 bytes reports 0%; allocation boundary remains candidate. |
| `game.map_mask_window_sweep` | `0x8002c670` 91.624245% WIP. | `.data` 28/28 bytes 100%; `.bss` 32 bytes reports 0%. |
| `game.map_object_init_records` | `0x80035894` 100%. | `.data` 270/270 bytes and `.rodata` 1,016/1,016 bytes 100%. |

These three text WIPs retain their real source behavior; the strict scores
do not authorize changing an owner, width, branch, or call without retail
evidence. A zero BSS section score does not invalidate the exact function
texts or prove a different runtime extent.
