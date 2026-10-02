# GAME player and actor follow-up: ten current WIPs

Each row below uses a fresh isolated safe-delink and pinned-GCC strict objdiff
on the current source, with the named image fixed to GAME. This is a bounded
source review, not an exact claim. Existing raw dossiers supply the CFG and
caller controls; the listed adjacent exact functions were rechecked in the
same isolated unit comparisons.

| GAME address | Strict text | Current verdict |
| --- | ---: | --- |
| `0x8002665c` | 98.67857% | Magic dispatch retains the 20-call order and 114/114 CFG; repeated player-state address formation is folded by the probe. The sibling and 52-byte table are exact. |
| `0x8002985c` | 99.26569% | Reaction control and its 19 jump-table rows agree; the first instruction residue assigns the clamped immediate to a different register. Seventeen sibling functions, 32-byte DATA, and 76-byte RODATA remain exact. |
| `0x8002897c` | 88.57143% | The signed inclusive interval has 3/3 CFG; the seven-word probe uses `v0`/`v1` in the opposite roles and adds a final move. The adjacent function is exact. |
| `0x800279cc` | 98.23967% | Landing response retains 70/70 CFG, 37/37 branches, and the 12-call order; two player-state address pairs are folded into a saved base. Its two siblings are exact. |
| `0x8003bd40` | 86.53226% | Motion helper retains 9/9 CFG and 5/5 branches; first divergence is saved-register assignment and an independent angle-input schedule. Six siblings are exact. |
| `0x8003b5d0` | 96.42041% | Vertical-state controller retains 21/21 branches, but probe shares one reset join (40/39 CFG). Its three siblings are exact; previously tested natural guards emitted the same or worse object. |
| `0x80039c94` | 98.11751% | Fixed-curve call and field families remain intact after the earlier width correction. Remaining differences are saved-register assignment and an eight-byte late layout shift; the sibling is exact. |
| `0x8003c3e0` | 99.64539% | Direction solver's remaining mismatches exchange `a2` and `v1` for one yaw intermediate; three siblings are exact. |
| `0x800460a0` | 99.268295% | Animation seek keeps 6/6 CFG and its two-call topology; even-step and half-step temporaries occupy different saved registers. |
| `0x8003a614` | 96.91011% | Actor-to-player damage gate retains 6/6 CFG, 3/3 branches, and four calls. The probe reuses the player-camera base for two Z loads where retail rematerializes two HI16/LO16 pairs. Its `0x8003a778` sibling remains exact. |

For the state-`0x20` arm of `0x8003b5d0`, retail independently reads the
signed velocity and its unsigned halfword view before updating position and
speed. An off-tree C spelling with named old and next velocities improved
isolated strict text to **96.88979%** but coalesced those two raw reads into
one `lhu` plus shifts, moved the cache load, and lowered focused listing
from 94.3% to 88.8%. The three exact siblings stayed exact. That trial was
discarded; the tracked source retains both retail load widths.

No row exposed an independent type, call, referent identity, or control-flow
correction on this pass. The source stayed unchanged. Verification was limited
to affected-unit compilation and isolated strict comparison; no repository
tests, lint, full build, broad matching pass, or README update ran.
