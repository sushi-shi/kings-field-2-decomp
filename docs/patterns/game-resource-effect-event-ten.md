# GAME resource, effect, and event ten

These ten currently non-exact GAME functions form a call chain from map-cell
visibility and archive-backed resources through effect and event control, then
into save-state copying. For each, retail extent, block disassembly, incoming
and outgoing references, strings, vendored attribution, and match state were
checked. No address has a supported Sony/Psy-Q archive attribution.

| GAME address | Decisive retail evidence | Final verdict |
| --- | --- | --- |
| `0x800320b0` | Radius mask query walks a 24-by-24 byte map grid, ORing valid cells in a square. | **WIP, 73.95918% strict**; source has 10/10 CFG blocks and 6/6 branches, but row/column induction and register scheduling differ. |
| `0x800321d8` | Gets a TMD archive entry size, allocates it, sets block kind/tag, and queues a kind-`0x10` CD read. | **WIP, 98.4359% strict**; retail forms the fixed arena address `0x8009b0a0` with signed-low `lui/addiu`, while the provisional literal compiles to `lui/ori`. The source mechanism for that arena boundary is unproved. |
| `0x8003247c` | The 0xb70-byte map/resource controller calls both resource range updaters, mask queries, sound, and world-model rendering. | **WIP, unclaimed**; its 96-block flow and mixed map/render record need a complete type and owner. |
| `0x80036ed4` | The 0x1df4-byte map-object event dispatcher directly calls frame/CD, collision, resource, sound, and effect helpers. | **WIP, unclaimed**; broad event-object fields and indirect control remain unresolved. |
| `0x8003d184` | The 0x248c-byte actor dispatcher calls spatial sound, effect spawning, animation, collision, and target helpers. | **WIP, unclaimed**; actor event payload and indirect branches need a complete owner model. |
| `0x8003fb94` | The 15-argument effect damage precursor has four proven calls, 11 CFG blocks, and six branches. | **WIP, 87.791046% strict**; the source agrees on CFG/calls, but retail and compiled prologue saves and argument-mask scheduling differ. |
| `0x8004678c` | The 0xc54-byte event controller calls CD services, spatial audio, map-object helpers, frame rendering, and an indirect callback. | **WIP, unclaimed**; event record, switch/indirect target, and data-table owner remain incomplete. |
| `0x800475d8` | The 0x6c0-byte paired controller calls `PadRead`, CD services, pose interpolation, frame rendering, and notification. | **WIP, unclaimed**; the caller's object/state lifetime and control flow remain incomplete. |
| `0x80048d24` | A 0x5b8-byte save-state copy moves live bytes, words, and halfwords into a large caller-provided payload, including fields near payload +`0x39d4`. | **WIP, unclaimed**; the complete payload extent and field family have no proven typed owner. |
| `0x800492dc` | A 0x5e0-byte inverse copy restores the same live-state families from the payload and replicates one restored byte to four adjacent live bytes. | **WIP, unclaimed**; the paired payload/live-state schema remains incomplete. |

The two 0x800320b0 source-order probes changed only when the compiler formed
the Z row address relative to X. One improved local listing similarity but
still diverged in row-pointer lifetime, loop induction, and register choice;
neither supplied new source evidence, so both were discarded. The
`0x8003fb94` focused comparison likewise isolates early save/order differences
with unchanged CFG and ordered calls. Its existing source and all nine exact
siblings in `game.effect_update` were left intact. The unresolved fixed arena
at `0x8009b0a0` was not replaced by a guessed global or linker placement.

The strict GAME pass relinked 145/145 target units. `game.resource_runtime`
preserved four exact neighbors at 100%, the existing `0x80032174` and
`0x80032274` WIPs at 93.6% and 90.933334%, and the three source-backed WIPs
above at their previous scores. Global edge-check still stops on three
pre-existing unrelated TMD/map-object `.rodata` addends. No shared source,
identity, relocation, or unit claim changed in this ten. No repository tests,
banking, or commit were performed.
