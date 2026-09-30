# GAME main, audio, and event ten

These ten currently non-exact GAME functions connect sound and CD setup to
resource loading, event control, and actor animation. Retail disassembly,
CFG, callers, callees, strings, data references, source history, current match
state, and the vendored inventory were checked for each. None has a supported
Sony/Psy-Q archive attribution; the `Ss*` calls below are SDK boundaries in
game-owned callers.

| GAME address | Decisive retail evidence | Final verdict |
| --- | --- | --- |
| `0x800139c4` | Initializes sound, music volume, the two-table SPU setup, reverb, then several GAME audio and resource arrays. | **WIP, unclaimed**; the 0x8009a6a0 SPU-table boundary and several BSS interiors lack complete source owners. |
| `0x800144b8` | Services a partly transferred VAB under a critical section, retrying after `SsVabClose` and advancing CD sectors. | **WIP, 94.87342% strict**; 11/11 CFG blocks and all calls agree, but the probe keeps `-1` in `s3` while retail keeps phase value `1` there. |
| `0x80015d58` | Loads map resources through archive reads, TMD registry, VAB queue, map stream, and session initialization. | **WIP, unclaimed**; the 0x27c-byte startup sequence still needs typed workspace ownership. |
| `0x80015fd4` | Stages five graphics bytes, yields to CD, repeatedly services a resource transition, sets a TMD slot, then calls an indirect callback. | **WIP, unclaimed**; the graphics-state byte family and callback target remain incomplete. |
| `0x80016260` | Drives critical-section, CD-yield, music start, transition service, and event-state loading. | **WIP, unclaimed**; the 0x55c-byte state machine and shared transition record are not proven. |
| `0x80016820` | The 0x6b4-byte resource transition service calls sequence controls, VAB queue, CD/map reads, actor fixup, and grid placement. | **WIP, unclaimed**; it has an unresolved indirect call and incomplete resource state. |
| `0x800460a0` | Advances an actor animation phase until it falls within a target tolerance, stepping the frame service each iteration. | **WIP, 99.268295% strict**; 6/6 CFG blocks and both calls agree; only saved registers for step and half-step are exchanged. |
| `0x80047c98` | The 0x660-byte event controller calls collision, map-object, audio, notification, and actor proximity helpers. | **WIP, unclaimed**; event-script state and branch-owner fields remain incomplete. |
| `0x80048554` | Allocates or releases memory blocks while copying resource words and traversing save-state event helpers. | **WIP, unclaimed**; an indirect call and the packed save payload still need ownership. |
| `0x800489ac` | Restores actor/map state through the collision sampler, map-object reset, and random selection. | **WIP, unclaimed**; the paired payload/state walker and indirect call target remain unresolved. |

A source-only VAB retry-loop variant produced the same listing and was
discarded. Changing the masked animation step from signed to unsigned did not
alter `0x800460a0`'s instruction bytes; its register assignment is still
unattributed. No shared source, identity, relocation, or unit claim changed
in this ten, and no fixed address or callback target was invented to raise a
score. The prior strict GAME report gives the two sourced WIP scores above;
the other eight have no source unit to score. No repository tests, bank, or
commit were run.
