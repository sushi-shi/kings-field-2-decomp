# GAME actor animation and blocking phase: 29-function verdict

The blocking phase helper `func_800460a0` at `0x800460a0` has three proven
direct calls, all in `event_target_stream.c`. Its fifth O32 argument is read
as a word from the caller stack, explicitly narrowed to 16 bits for the
zero guard, then masked with `0xfffe` for the step. The loop stores the
requested state and initial phase, tests a half-step tolerance through
`angle_within_tolerance`, advances the phase with a 12-bit wrap, and calls
`func_800335a0` on each frame and once after the final phase store.
The first two caller sites at body `+0x288` and `+0x2a4` store their fifth
argument at `sp+16` in the `jal` delay slot: a signed actor animation-step
halfword and an unsigned candidate step halfword respectively. The callee's
low-16-bit mask makes both call forms coherent with the current word-sized
formal parameter.

The frame callee `func_800335a0` is defined in `render_frame.c` with
`(const VECTOR *, const SVECTOR *)`. Its prototype now lives in
`graphics.h`; the phase helper, proven event caller, event counter, and
player reaction use that shared declaration instead of local `(s32, s32)`
declarations. Calls that pass two null pointers keep their source meaning.
Focused rebuilds of the phase and event-target units produce object files
byte-identical to their pre-correction candidates. The event counter retains
three strict-exact claims, and the independently checked player-reaction unit
retains 16 exact claims and its unchanged WIP. This is an interface
correction, not a new match.

The remaining typed duplicate externs were removed from `main.c`,
`frame_step_cd_service.c`, `event_command_dispatch.c`, and
`event_map_object_controller.c`; the last file now includes `graphics.h`.
Focused listings before and after agree for the first, second, and fourth
units. The event-command dispatcher remains `SAME`, as do both render-frame
claims. The header and definition are now the only declarations of this
callee under `include/` and `src/game/`.

A fresh focused build and safe one-VA target give equal 164-byte bodies,
6/6 CFG blocks, 2/2 branches, and strict **99.268295%** text. The only
five differing words exchange the `$s1` and `$s3` lifetimes for the masked
step and half-step; all calls, field widths, constants, branch targets, and
relocations agree. An off-tree `u32 step` spelling, consistent with the
logical shift, produced a byte-identical focused listing. It was discarded;
the explicit input narrowing already models the raw word load, and there
is no source-backed reason to steer saved registers.

This 29-claim campaign used focused builds of ten units and two safe retail
carves, totaling **672 relocations with none withheld**. Each function was
compared by isolated strict objdiff. The 21 exact functions below are
preserved controls, not new closures; eight remain WIP. Sizes are retail
text bytes.

| GAME VA | Function or role | Bytes | Strict text |
| --- | --- | ---: | ---: |
| `0x80033bfc` | `animation_expand_sparse_vertices` | 196 | 100% |
| `0x80033cc0` | `animation_decode_sparse_vertices` | 124 | 100% |
| `0x80033d3c` | `func_80033d3c`, sparse vertex loop | 696 | 100% |
| `0x800397d8` | `func_800397d8`, animation-state setter | 44 | 100% |
| `0x80039804` | `func_80039804`, changed-state setter | 56 | 100% |
| `0x8003983c` | `func_8003983c`, lifecycle target | 796 | 99.19598% WIP |
| `0x80039b58` | `func_80039b58`, lifecycle scan | 188 | 90.95744% WIP |
| `0x80039c14` | `func_80039c14`, curve helper | 128 | 100% |
| `0x80039c94` | `func_80039c94`, fixed-curve controller | 1,668 | 98.11751% WIP |
| `0x8003a9f4` | `func_8003a9f4`, animation helper | 360 | 100% |
| `0x8003ab5c` | `func_8003ab5c`, animation helper | 344 | 100% |
| `0x8003acb4` | `actor_bind_current` | 220 | 100% |
| `0x8003ad90` | `actor_advance_animation_wrapped` | 52 | 100% |
| `0x8003adc4` | `actor_advance_animation_clamped` | 92 | 100% |
| `0x8003ae20` | `actor_animation_crossed_phase` | 48 | 100% |
| `0x8003ae50` | `func_8003ae50`, movement animation | 1,260 | 99.31746% WIP |
| `0x8003b9a4` | `func_8003b9a4`, motion helper | 320 | 100% |
| `0x8003bae4` | `func_8003bae4`, motion helper | 188 | 100% |
| `0x8003bba0` | `func_8003bba0`, motion helper | 304 | 100% |
| `0x8003bcd0` | `func_8003bcd0`, motion helper | 112 | 100% |
| `0x8003bd40` | `func_8003bd40`, horizontal steering | 248 | 86.53226% WIP |
| `0x8003be38` | `func_8003be38`, motion helper | 316 | 100% |
| `0x8003bf74` | `func_8003bf74`, motion helper | 140 | 100% |
| `0x8003c000` | `func_8003c000`, group position | 268 | 100% |
| `0x8003c10c` | `func_8003c10c`, group position | 276 | 100% |
| `0x8003c220` | `func_8003c220`, group position | 448 | 100% |
| `0x8003c3e0` | `func_8003c3e0`, group direction | 564 | 99.64539% WIP |
| `0x800460a0` | `func_800460a0`, blocking phase | 164 | 99.268295% WIP |
| `0x800462bc` | `func_800462bc`, proven phase caller | 1,092 | 98.68132% WIP |

The six exact siblings in `actor_animation.c` protect the movement helper:
its only two raw instruction differences choose `$v0` versus `$a1` for two
independent `angle ± 0x400` values. `func_8003c3e0` has seven differences
confined to an `$a2`/`$v1` yaw-temporary exchange on equal 564-byte bodies.
`func_8003bd40` retains the three direct calls, typed O32 stack slots, and
9/9 CFG blocks; independent coordinate-load order and saved-register
lifetimes account for its 248/240-byte layout gap. The lifecycle and
fixed-curve WIPs likewise retain their raw calls and field identities;
their first gaps are saved-register lifetime, call-argument scheduling, and
one local load-delay slot rather than a new field or branch outcome.

The direct phase caller `func_800462bc` keeps its 64-byte switch table's
target classes; one extra candidate load-delay `nop` follows the two
independent actor-field loads and shifts later table addends. KF1 has a
blocking map-event animation helper at `map_event.c:0x80033820`, but it
advances a map-event phase toward a target with frame rendering, not KF2's
actor tolerance/wrap operation. It informs the frame-loop role without
proving identical source declarations. Beyond the shared frame-callee
prototype and its users, no C edit or exact claim was made for the eight WIPs.
