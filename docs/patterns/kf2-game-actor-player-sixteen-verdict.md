# GAME actor/player sixteen-function follow-up

This focused follow-up compares sixteen functions in six actor/player units
and five nearby damage, collision, and update units. Each candidate was
rebuilt with `kf try --unit`; the percentages below are direct isolated
strict objdiff results against safe one-VA GAME delinks. No profile or C
body was changed during this pass.

| GAME VA | Function | Strict text | Verdict |
| --- | --- | ---: | --- |
| `0x80039710` | `actor_find_target_of_type` | 100% | Exact, 72 bytes. |
| `0x800390d0` | `actor_set_target` | 100% | Exact, 56 bytes. |
| `0x800396c4` | `actor_select_target_for_player_distance` | 100% | Exact, 76 bytes. |
| `0x80039758` | `actor_select_target_type_in_own_group` | 100% | Exact, 80 bytes. |
| `0x800248a8` | `func_800248a8` | 100% | Exact, 1,020 text bytes and 28 RODATA bytes. |
| `0x80024ca4` | `func_80024ca4` | 100% | Exact, 560 bytes. |
| `0x80024498` | `func_80024498` | 100% | Exact, 844 text bytes and 32 RODATA bytes. |
| `0x8003a318` | `func_8003a318` | 99.86911% | WIP; 26/26 CFG blocks and 13/13 branches, first residual is an `andi` destination register. |
| `0x8003a614` | `func_8003a614` | 96.91011% | WIP; 6/6 CFG blocks and 3/3 branches, candidate reuses one player-state address where retail forms it twice. |
| `0x8003a778` | `func_8003a778` | 100% | Exact, 636 bytes. |
| `0x8003f610` | `func_8003f610` | 100% | Exact, 476 bytes. |
| `0x8003f7ec` | `actor_fixup_group_targets` | 85.86207% | WIP; 9/9 CFG blocks and 4/4 branches, first residual is commuted operands of the same pointer addition. |
| `0x8003f860` | `func_8003f860` | 100% | Exact, 460 bytes. |
| `0x8002aaa4` | `func_8002aaa4` | 57.52610% | WIP; 174/172 CFG blocks and 99/98 branches, with unresolved switch CFG. Its 196-byte RODATA is 27.551018%; all 49 pointer rows retain their curated referents but body addends differ. |
| `0x8002897c` | `func_8002897c` | 88.57143% | WIP; 3/3 CFG blocks and 1/1 branch, seven retail words versus seven candidate words with a Boolean-result register choice. |
| `0x80028998` | `func_80028998` | 100% | Exact, 1,320 bytes. |

The eleven strict-exact bodies remain intact. The four short non-exact
functions above have equal known call/branch topology; their first raw
differences do not support a different field, call, constant, or referent.
The large collision-shape dispatcher is a separate structural WIP: retail
and candidate each use a 49-row jump table with the same 13 target classes,
but the first known branch-target correspondence already differs after the
initial record setup, and indirect-switch reachability is incomplete. This
pass did not force any source change to improve a score.

### Disjoint actor control recheck (2026-10-02)

Nine manifest-profile GAME units were isolated and compared strictly with
retail: 31 functions, 21 existing exact controls and ten WIPs. Exact controls
are six in `actor_animation`, six in `actor_motion`, three in
`actor_motion_collision`, three in `actor_group_position`, two in
`actor_fixup_group_targets`, and one in `actor_fixed_curve`.

| WIP | Fresh strict text | Focused CFG/branches | Bounded residue |
| --- | ---: | ---: | --- |
| `0x8003ae50` animation motion | 99.31746% | 58/58, 34/34 | Store and delay-slot scheduling. |
| `0x800460a0` animation seek | 99.268295% | 6/6, 2/2 | Phase-result register order. |
| `0x8003d184` behavior dispatch | 99.931595% | 410/410, 213/213 | Repeated target-halfword load and frame/register lifetime. |
| `0x80039c94` fixed curve | 98.11751% | 72/72, 46/46 | Duplicate retail constant setup before shared store. |
| `0x8003f7ec` target fixup | 85.86207% | 9/9, 4/4 | Commuted pointer addition and sentinel scheduling. |
| `0x8003c3e0` group position | 99.64539% | 23/23, 11/11 | Yaw-normalization temporary register. |
| `0x8003983c` lifecycle | 99.19598% | 37/37, 24/24 | Actor-base/register lifetime. |
| `0x80039b58` lifecycle scan | 90.95744% | 9/9, 4/4 | Free-slot byte saved-register lifetime. |
| `0x8003bd40` actor motion | 86.53226% | 9/9, 5/5 | Typed slots and calls agree; register allocation differs. |
| `0x8003b5d0` motion collision | 96.42041% | 40/39, 21/21 | Retail keeps a separate state-`0x20` reset/exit block; a prior shared-label control regressed. |

No new call, field, width, constant, or referent fact emerged from the first
raw differences. No C edit or new exact closure follows.
