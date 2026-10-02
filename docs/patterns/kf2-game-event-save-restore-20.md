# GAME event save/restore stream: 20 connected verdicts

The event saver at `0x80048554` serializes actor lifecycle bytes, target
group bytes, and packed map-object records. The paired restore interpreter
at `0x800489ac` reads the two `0xff`-terminated lists before a 16-entry
opcode switch. A fresh focused rebuild and safe 18-VA carve produced
**1,300 relocations with none withheld**. All 18 event functions were
compared by isolated strict objdiff, including the exact saver and its
six exact state/arena helpers. Two direct connected controls add the
restore collision-grid callee and a save-calling player reaction path;
their safe carve produced 524 relocations with none withheld. The table
below gives each function's retail text size and current strict verdict.

| GAME VA | Function or role | Bytes | Strict text |
| --- | --- | ---: | ---: |
| `0x8002985c` | `func_8002985c`, save-calling player reaction | 4,396 | 99.25660% WIP |
| `0x8002a988` | `func_8002a988`, restore collision-grid callee | 284 | 100% |
| `0x80045f20` | `func_80045f20`, pose interpolation | 180 | 100% |
| `0x80045fd4` | `func_80045fd4`, pose interpolation | 204 | 100% |
| `0x800462bc` | `func_800462bc`, target stream | 1,092 | 98.68132% WIP |
| `0x80046700` | `func_80046700`, map-object spawn | 140 | 100% |
| `0x8004678c` | `func_8004678c`, event commands | 3,156 | 100% |
| `0x800473e0` | `func_800473e0`, counter | 84 | 100% |
| `0x80047434` | `func_80047434`, counter | 144 | 100% |
| `0x800474c4` | `func_800474c4`, counter | 276 | 100% |
| `0x800475d8` | `func_800475d8`, map-object controller | 1,728 | 99.166664% WIP |
| `0x80047c98` | `func_80047c98`, world dispatcher | 1,632 | 99.81618% WIP |
| `0x800482f8` | `func_800482f8`, event-state helper | 176 | 100% |
| `0x800483a8` | `callback_invoke_slot_04_zero` | 48 | 100% |
| `0x800483d8` | `func_800483d8`, saved-slot setup | 80 | 100% |
| `0x80048428` | `func_80048428`, saved-slot helper | 112 | 100% |
| `0x80048498` | `func_80048498`, saved-slot helper | 76 | 100% |
| `0x800484e4` | `func_800484e4`, saved-slot helper | 112 | 100% |
| `0x80048554` | `func_80048554`, save writer | 1,112 | 100% |
| `0x800489ac` | `func_800489ac`, restore interpreter | 888 | 98.82883% WIP |

The saver matches both text and its 0x294-byte RODATA claim at 100%.
The restore interpreter has equal 888-byte bodies and an exact 0x40-byte
switch table. Its raw differences are confined to the actor and group
sentinel loops before `+0xcc`: retail holds the `0xff` sentinel in `$a2`
and actor-state base in `$a1`, while the candidate exchanges those registers
and schedules the group-base load four bytes earlier. Both use the same
base referents, 0xff termination, field writes, and later opcode handlers.
The first source correction would only steer registers, so none was made.

The target stream has one extra candidate load-delay `nop`: retail loads
`lhu` from `record+24`, then an independent `lbu` from `record+12` before
the branch; the candidate reverses the loads. This shifts later code and
its 64-byte pointer table; RODATA remains 92.1875%, with row identities
retained. The world dispatcher has equal body size and a 99.81618% residue
consisting of saved-register choices and a commuted integer addition.
The map-object controller's first differences likewise exchange live
saved-register roles; one later `nop`/move and four-byte tail shift remain
unattributed without a distinct field, call, or branch outcome. The
save-calling player reaction is a separate 99.25660% WIP and received no
source edit in this stream campaign.

KF1's `save_system.c` copies a fixed save payload, and its map-event code
does not provide an equivalent packet decoder for these KF2 byte streams.
This analogue check does not establish source-level types beyond the raw
KF2 packet widths. No source or relocation-inventory edit was retained.
