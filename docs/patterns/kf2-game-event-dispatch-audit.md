# GAME event-command neighbor audit

The GAME `0x8004678c` command dispatcher uses the reviewed 35-word table at
`0x800128d0`. Commands `0x5a..0x5e` jump directly to five retail stubs at
`0x80046f00/10/20/30/40`. Each stub loads its distinct eight-byte magic ID
list at `0x800679a0..0x800679c7`; the first four jump to the common scan at
`0x80046f4c` with `li a3,0xff` in the delay slot. The fifth falls through.
An inner C `switch(command)` inserted a second comparison ladder, so the
outer switch cases need separate table loads and a shared body. This is a
source-shape/CFG finding, not an absent list or effect-state referent. The
subsequent scan loads `effect_state` at retail `0x80046f4c/50`, reads a byte
ID at `0x80046f58`, multiplies it by the 26-byte record stride, then checks
and sets the byte at record +0. The existing `u8` list and 26-byte typed
record explain those raw loads; the probe's extra byte-mask instruction did
not establish a different source width.

The dispatcher transition path's two `func_80016260` arms at
`0x80046ab4..0x80046b08` retain the same five unsigned-byte controls and
three signed-byte offsets as the shared header. The three service loops in
the magic branch have the retail `cd_request_service_vab`,
`cd_request_service_stream`, and `func_800335a0` call sets. A shared
`decay_update` entry after the first animation loop now reproduces the raw
forward exit at `0x800470b8`, separate back-jump and stack-argument delay
slot at `0x800470c0/0x800470c4`, and the decay service backedge from
`0x80047104` to `0x800470c8`. The growth update at `0x80047130` similarly
precedes its service backedge to `0x80047114`. The terminal `jalr` at
`0x80047388` still has a loaded callback-table slot, not a proved fixed
callee.

Command `0x52` reaches `func_80047434(0x4d)` at `0x800471dc`. Its nonzero
outcome branches to the shared state store with `li v0,1` in the branch
delay slot at `0x800471e8`; its zero outcome calls `notify_enqueue(0x16)`
and `func_800473e0(0x52)`, then jumps to the same store with `li v0,1` at
`0x80047200`. The source now states the literal-one write explicitly.
These source-backed dispatcher corrections first reached 98.56781% strict
text similarity in an isolated comparison. The final branch-local record
write described below closes the remaining residue.

The adjacent event family was reviewed with focused unit comparisons:

| GAME function | Verdict and first useful residue |
| --- | --- |
| `0x80045f20`, `0x80045fd4` | Previously verified exact pose projection/interpolation callees. |
| `0x80046144` | Previously verified exact marker scanner. |
| `0x800461a0` | Previously verified WIP, 99.12676% strict; cursor-register/increment order. |
| `0x800462bc` | Focused DIFF, 90.5% listing; the script execution gate loads actor +24 halfword and candidate +12 byte in the opposite order, with one extra candidate load-delay `nop`. No width mismatch is proved. |
| `0x80046700` | Previously verified exact event-object spawn callee. |
| `0x8004678c` | Exact command controller. The five-case, decay-join, command `0x52` state-one, and branch-local magic-record write corrections reproduce retail text, data, rodata, and ordered relocations. |
| `0x800473e0`, `0x80047434`, `0x800474c4` | Previously verified strict exact counter and transition callees. |
| `0x800475d8` | Focused DIFF, 89.4% listing; retail keeps the selected map template in `$s1`, probe in `$s0`, affecting the pose-loop saved-register lifetimes. |
| `0x80047c98` | Focused DIFF, 97.1% listing; 85/85 CFG blocks and 55/55 branches now agree. Retail keeps the rotation argument in `$s4` and constant one in `$s5`; the probe exchanges them. Calls and typed referents remain present. |
| `0x800482f8`, `0x800483a8`, `0x800483d8`, `0x80048428`, `0x80048498`, `0x800484e4` | Focused 6/6 identical listings in the event-state unit. |
| `0x80048554` | Focused identical listing in the event-save unit. |
| `0x800489ac` | WIP, 98.82883% previously recorded strict and 95.1% current focused listing; retail puts the actor-state base in `$a1` and the `0xff` sentinel in `$a2`, while the probe exchanges them. Calls, referents, and branches remain present. |

The current source has closed the earlier linked-object notify CFG gap. A
fresh focused comparison has matching known successor lists and only the
saved-register assignments plus one commutative `addu` operand order in its
listing differences. The prior isolated strict result was 97.37745%; no new
strict claim or source edit follows from this register schedule.

For `0x80047c98`, retail `0x80048150..0x80048190` has one
`notify_enqueue(6)` block reached when the linked index is `0xffff` or the
linked object's ID is `0xff`. Two source-equivalent branch formulations for
this kind-five handler compiled to the existing candidate listing, so the
original concise condition was retained. Reordering the two stream pointers
in `0x800461a0` likewise did not correct the retail cursor/marker register
roles and was reverted. No speculative source edit was kept for these WIPs.

KF1 `master` `src/game/map_scripts.c`, including history commit `bf051cfa`,
has related magic-record updates and blocking scene-animation loops, while
`src/game/map_event.c` and `map_events.c` show typed event/record ownership.
None has the KF2 35-command topology. The KF1 examples are source-shape leads;
the addresses, list values, record stride, call sites, and CFG claims above
come from KF2 retail bytes and focused comparisons. No source edit in the
neighboring event units follows from the remaining register and scheduling
residues.

## Exact dispatcher closure

The last mismatch was the write to `magic_record->menu_available` in the
eight-entry magic-ID scan. Retail `0x80046f80..0x80046f90` loads that byte,
branches back when it is already set, and writes one only on the available
record path before acquiring the map-object effect. Placing the write inside
the `menu_available == 0` branch expresses that control flow directly. The
earlier write after the loop preserved the game-level result but placed the
store on a different compiler path and left one instruction absent from the
retail schedule. KF1's related `map_scripts.c` loop was a source-shape lead;
the decisive branch and store order comes from KF2's own instructions.

With the reviewed one-function GAME carve and the pinned GCC 2.5.7 probe,
strict objdiff now reports `100.0%` for `func_8004678c`: 3,156/3,156 text
bytes, 40/40 data bytes, 140/140 rodata bytes, and 218/218 ordered
relocation sites. The 789 decoded instructions and all 35 jump-table rows
agree. A focused `kf try --unit game.event_command_dispatch --context 0
--no-flow` also reports `SAME`. This is an isolated object verdict; it does
not by itself claim the original compiler provenance or a full linked image.

## Message and event UI recheck (2026-10-02)

A fresh isolated strict and focused pass covered 22 GAME functions across
message-stream marker lookup, pose interpolation, event spawn/counters,
save/restore/command dispatch, notification draw/queue, and menu glyph rows.
Eighteen are strict exact, including all notification and glyph functions,
the 3,156-byte command dispatcher with its data/table claims, and the event
save encoder. Four remain WIP:

| Function | Strict text | Focused CFG | Residue |
| --- | ---: | ---: | --- |
| `0x800461a0` marker lookup | 99.12676% | 14/14 blocks, 5/5 branches | Retail and probe exchange the record-cursor and marker-pointer registers; the 13 ordered relocation rows and sites agree. Exact `0x80046144` is preserved. |
| `0x800475d8` map-object event controller | **99.166664%** | 55/54 blocks, 29/29 branches | The older 98.56481% strict score is stale. Retail at `0x80047afc..0x80047b08` leaves the `remove_object = 0` assignment in a fallthrough block; the probe schedules that independent assignment in the preceding `bne` delay slot. The taken branch skips the later use of that local. The selected template and pose-loop locals also exchange saved registers. All 62 ordered referent identities agree; 49 sites align. |
| `0x80047c98` world event dispatcher | 99.81618% | 85/85 blocks, 55/55 branches | The rotation argument and constant one exchange saved registers; all 72 relocation rows and sites agree. The final indirect callback remains unresolved. |
| `0x800489ac` event restore decoder | 98.82883% | 23/23 blocks, 7/7 branches | The actor-state base and `0xff` sentinel exchange argument registers. All 40 ordered referent identities agree; 38 sites align. The 64-byte jump table remains exact, while indirect-jump reachability is incomplete. |

The `0x800475d8` block-count gap is a delay-slot placement of the same
unobserved local assignment, not an evidenced state or call difference; no
padding, register carrier, or branch rewrite is justified. No source edit was
retained and no WIP was banked. This pass used focused quick builds and
isolated strict objdiff only, with no repository tests, lint, full build,
broad match, or README update.
