# GAME effect constructor: fifth-slot argument walker

`func_80040308` (`game.effect_constructor`, GAME `0x80040308`) remains WIP.
The five named arguments end with a direction pointer in the fifth O32 stack
slot. Retail sets `$s1 = sp+88` in the free-slot call's delay slot, loads the
direction pointer separately from that slot, then reads optional arguments at
`4($s1)`, `8($s1)`, and later word offsets. The exact KF1 constructor at
`GAME 0x80036f44` uses the same five-argument, fifth-slot `s32 *` walker;
its standard `va_start` controls moved the cursor past the direction slot.

The retained source uses `s32 *va = (s32 *)&direction` and indexes optional
words from `va[1]`. Each of the 40 former `va_arg` reads maps to the same
argument position, including six reads in kind 12. The current C also keeps
kind 101's retail read order: render ID and scale before
`effect_pool_initialize_scaled`, then the other three halfwords afterward.
Kind 111 reads its halfword before writing the record fields, matching the
retail load-before-store sequence. Kinds 2 and 23 each read and store their
first two halfwords, then join at a shared third-halfword store and spatial
sound call. Retail makes the same case-2 jump into the case-23 tail.
Kind 12 now copies rotation and writes the three scales before reading its
remaining five optional halfwords. Retail interleaves each `lhu` with its
record store; the previous source loaded all five before copying rotation.
Kind 100 clears `rotation.vz` after its second `rand` call, as retail does;
the earlier C order cleared it before that call.
Kind 46 loads its optional byte before clearing `direction.vy` and stores
the byte in the return jump's delay slot. A short `u8` local expresses that
retail ordering without changing the field value.
Kind 5 likewise reads its optional byte before setting the update count;
retail then stores that byte in the sound call's delay slot.
Kinds 38/39 draw the second rotation random value before clearing rotation
and scale fields, then apply that value afterward. The retail instruction
sequence places the second `rand` call before those field stores; a local
holds the returned value across them.
Kinds 33/53 use the same ordering for the third direction random value:
retail calls `rand`, writes update count and cooldown, then applies the
returned value to `direction.vz`. The shared switch tail uses a scoped local
to keep that call sequence.
Kind 114 writes its 100/600/100 halfwords to `direction` at record offsets
52/54/56. The earlier C wrote the same values to `rotation` at 36/38/40;
the corrected fields match the raw stores. Retail writes X and Z before Y,
so the source keeps that order. A trial moving the direction writes ahead of
the position update worsened the pinned probe and was discarded.
Kind 2 clears `scale_z` before `scale_x` at record offsets 48 and 44. The
source now spells that raw store order ahead of its three optional halfwords.
Kind 3 sets the animation clip between its two one-byte flags, matching
retail's ordered stores at offsets 8, 4, and 9.
Kind 23 clears direction Z, Y, then X at offsets 56, 54, and 52 after its
initializer. That order matches the raw stores before the three optional
halfwords and shared sound tail. It now writes each of the first two
optional halfwords directly after its corresponding load. The earlier
two-local form let the probe load both before either store; retail
interleaves the first load/store pair with the second.
The pinned probe now reproduces the complete retail prologue through `+0x70`
and all **45 ordered optional-stack load width/offset pairs** (`lw`, `lhu`,
`lbu`). The pointer walk is an O32-specific source model; linked bytes alone
do not prove the historical C spelling.

A fresh tracked focused quick build reports 88.3% listing similarity,
116/116 CFG blocks, 22/22 branches, and 26/26 known return frontiers.
Isolated strict objdiff reports `.text` **97.58131%** and `.rodata`
**18.394308%**. Kind 12's corrected load/store sequence raised text from
93.75177% to 94.9725%; kinds 100, 46, 5, 38/39, 33/53, 114, 2, 3, and 23
raised it further. The
shorter body shifted many jump-table target addends and lowered the data
percentage from 40.243904% without changing table identity.
The candidate retains 157 `.text` and 123 switch-table relocation rows,
21 direct `effect_play_spatial_sound` sites, and all 69 external calls in
the retail target order. All 123 table entries preserve the retail's 62
target-equivalence classes; seven pointer addends are exact. Call placement,
the shared-tail delay-slot schedule, and switch-target addends still differ,
so this is no exact claim.

The case-local kind-101 correction first established the initializer/read
order, improving focused similarity from 40.3% to 80.0% and strict `.text`
from 89.12569% to 92.25609%; it left the cursor at `sp+92`. A four-named
argument signature with direction consumed by `va_arg` instead homed `$a3`
before the prologue, used a 64-byte rather than 72-byte frame, and reduced
strict `.text` to 90.974075%. Reading kind 101 from `&direction` alone still
left the probe cursor at `sp+92` and gave 92.23645% `.text`.

The pinned Psy-Q 3.0 `STDARG.H` advances its cursor before reading the prior
slot. An off-tree constructor compile with that authentic macro kept the
72-byte frame and 21 sound calls but also used `sp+92`; strict `.text` was
91.904945% and `.rodata` 45.63008%. With `NULL` supplied for `va_end`, the
same header leaves the three functions and 126 data bytes in the exact
`game.notify_enqueue` control unchanged. The constructor's original header
or manual access spelling remains uncertain.

A kind-111 early-read alone matched the retail load-before-store schedule
but emitted 22 sound calls versus retail's 21. A first shared-tail trial
shadowed the third-halfword local in case 23; its apparent improvement was
invalid and discarded. Removing the shadow and writing the first two
halfwords before reading the third restored all 45 retail loads and 21 calls.
The probe still stores the third halfword before the call, whereas retail
places that store in the call's delay slot. Both execute it before the callee.
A separate shared-call-only form preserved the 45 loads but reduced strict
`.rodata` to 15.752032%; it was discarded.

After branch-target addends, the first raw word gap is case 26 at body
`+0x304`: retail writes the final scale halfword, jumps to the return path,
and repeats a zero byte store in the jump delay slot. The probe places that
scale store in the delay slot and has only the earlier zero byte store.
Both paths leave the same record fields. In cases 33/53, retail stores
`updates_remaining` and `cooldown` at `0x800408c8..0x800408d4`, after the
third `rand` call. The current source and focused candidate preserve that
order. Moving both stores before the third call in an off-tree trial moved
them between the second and third calls, away from retail, so it was
discarded.
