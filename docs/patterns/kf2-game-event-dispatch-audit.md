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
`cd_request_service_stream`, and `func_800335a0` call sets. Their branch
layout differs from the focused C probe, but no missing service call or
different loop bound was found. The terminal `jalr` at `0x80047388` still
has a loaded callback-table slot, not a proved fixed callee.

The adjacent event family was reviewed with focused unit comparisons:

| GAME function | Verdict and first useful residue |
| --- | --- |
| `0x80045f20`, `0x80045fd4` | Previously verified exact pose projection/interpolation callees. |
| `0x80046144` | Previously verified exact marker scanner. |
| `0x800461a0` | Previously verified WIP, 99.12676% strict; cursor-register/increment order. |
| `0x800462bc` | Focused DIFF, 90.5% listing; the script execution gate loads actor +24 halfword and candidate +12 byte in the opposite order, with one extra candidate load-delay `nop`. No width mismatch is proved. |
| `0x80046700` | Previously verified exact event-object spawn callee. |
| `0x8004678c` | WIP command controller; the direct five-case correction above was communicated to its source owner. |
| `0x800473e0`, `0x80047434`, `0x800474c4` | Previously verified strict exact counter and transition callees. |
| `0x800475d8` | Focused DIFF, 89.4% listing; retail keeps the selected map template in `$s1`, probe in `$s0`, affecting the pose-loop saved-register lifetimes. |
| `0x80047c98` | Focused DIFF, 86.3% listing; retail keeps the rotation argument in `$s4`, probe in `$s5`. The linked-object notify branch is ordered differently; calls and typed referents remain present. |

KF1 `master` `src/game/map_scripts.c`, including history commit `bf051cfa`,
has related magic-record updates and blocking scene-animation loops, while
`src/game/map_event.c` and `map_events.c` show typed event/record ownership.
None has the KF2 35-command topology. The KF1 examples are source-shape leads;
the addresses, list values, record stride, call sites, and CFG claims above
come from KF2 retail bytes and focused comparisons. No source edit in the
neighboring event units follows from the remaining register and scheduling
residues.
