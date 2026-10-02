# GAME effect constructor: kind 101 argument access

`func_80040308` (`game.effect_constructor`, GAME `0x80040308`) remains WIP.
Retail kind 101 loads its fourth optional word (render ID) at body `+0xec8`
and first optional word (scale) at `+0xecc`, then calls
`effect_pool_initialize_scaled` at `+0xed0`. Only after that call does it load
the second, third, and fifth optional halfwords at `+0xed8`, `+0xee4`, and
`+0xef0`, storing them at record offsets `+0x40`, `+0x0e`, and `+0x42`.
The prior source eagerly read all five `va_arg` values before the call.

The retained case-local `const s32 *` view indexes the pinned toolchain's
word-aligned `va_list` cursor at `[3]` and `[0]` before initialization, then
`[1]`, `[2]`, and `[4]` afterward. This preserves the same five argument
values and the retail call/read ordering without changing the other switch
arms. The focused quick build rises from 40.3% to 80.0% listing similarity,
with 116/116 CFG blocks, 22/22 branches, and 26/26 known return frontiers.
The isolated one-unit strict report on byte-identical source rises from
89.12569% to 92.25609% `.text`; `.rodata` is 32.825203% versus 32.92683%
before. The candidate still has 157 text and 123 switch-table relocation
rows, 21 direct `effect_play_spatial_sound` calls, and the same
62 target-equivalence classes across all 123 table entries; seven addends are
exact. The remaining cursor-origin offset is retail `sp+88` versus candidate
`sp+92`, and the constructor is not exact.

A separate kind-111 early-read control reproduced the retail load-before-store
schedule at that entry and raised `.text` to 92.67950%, but produced 22 direct
sound-call sites against retail's 21 and lowered `.rodata` to 30.08130%.
Combining that change with shared kind-2/kind-23 sound tails restored 21
sites, but reduced `.rodata` to 8.63821% or 26.01626%, depending on where
the third halfword was stored. Those off-tree controls were discarded.

A four-named-argument signature with the direction pointer consumed by
`va_arg` was also rejected. It homed `$a3` before the prologue, used a
64-byte frame instead of retail's 72 bytes, and reduced strict `.text` to
90.974075% and `.rodata` to 12.296748%. The five named arguments remain
supported by the raw entry schedule and current callers.
Indexing case 101 from `&direction` instead of `va_list` retained the
candidate's `sp+92` cursor and lowered `.text` to 92.23645%; it was also
discarded.

The pinned Psy-Q 3.0 `STDARG.H` uses an advance-then-read `va_arg` macro.
An off-tree single-unit compile with that authentic header preserved the
72-byte frame and 21 sound calls but still initialized the cursor at
`sp+92`; strict `.text` fell to 91.904945% while `.rodata` rose to
45.63008%. Header selection for this retail function remains unresolved.
The same macro form, with `NULL` supplied for its `va_end`, keeps the three
functions and 126 data bytes of `game.notify_enqueue` strict exact, so that
small exact control does not distinguish the two header forms.
