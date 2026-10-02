# GAME effect pool, actor group, and BSS owner check (28 functions)

This pass follows the actor-group effect caller into the effect constructor,
pool helpers, motion/scatter helpers, and pool sweep. It uses current
`GAME.EXE` identities and the `probe-gcc257-o2-g0` profile selected by these
units. A fresh compile of 13 individual units to temporary objects and an
isolated strict objdiff report reproduced **26 exact functions and two WIPs**.
Focused `kf try --context 0 --no-flow` listings independently reported SAME
for the 26 controls and DIFF for the two WIPs. The listed percentages are
strict objdiff `.text` scores, not the focused listing similarity.

| GAME VA | Function | Bytes | Strict verdict |
| --- | --- | ---: | --- |
| `0x8003c614` | `func_8003c614` actor-group caller | 2,672 | WIP, 86.041916% |
| `0x8003fa2c` | `effect_play_spatial_sound` | 60 | exact, 100% |
| `0x8003fb94` | `func_8003fb94` | 536 | exact, 100% |
| `0x8003fdac` | `effect_magic_power` | 36 | exact, 100% |
| `0x8003fdd0` | `func_8003fdd0` | 224 | exact, 100% |
| `0x8003feb0` | `func_8003feb0` | 104 | exact, 100% |
| `0x8003ff18` | `func_8003ff18` | 424 | exact, 100% |
| `0x800400c0` | `func_800400c0` | 244 | exact, 100% |
| `0x800401b4` | `func_800401b4` | 108 | exact, 100% |
| `0x80040220` | `effect_pool_find_free` | 68 | exact, 100% |
| `0x80040264` | `effect_pool_initialize_scaled` | 64 | exact, 100% |
| `0x800402a4` | `effect_pool_initialize_fixed` | 100 | exact, 100% |
| `0x80040308` | `func_80040308` constructor | 5,092 | WIP, 98.434410% |
| `0x800416ec` | `effect_rotate_scale_offset_y` | 144 | exact, 100% |
| `0x8004177c` | `func_8004177c` | 480 | exact, 100% |
| `0x8004195c` | `func_8004195c` | 440 | exact, 100% |
| `0x80041b14` | `func_80041b14` | 444 | exact, 100% |
| `0x80041cd0` | `func_80041cd0` | 172 | exact, 100% |
| `0x80041d7c` | `func_80041d7c` | 144 | exact, 100% |
| `0x80041e0c` | `func_80041e0c` | 136 | exact, 100% |
| `0x80041e94` | `func_80041e94` | 664 | exact, 100% |
| `0x8004212c` | `func_8004212c` | 364 | exact, 100% |
| `0x80042298` | `func_80042298` | 396 | exact, 100% |
| `0x80042424` | `func_80042424` | 204 | exact, 100% |
| `0x800424f0` | `func_800424f0` | 352 | exact, 100% |
| `0x80045cc0` | `effect_pool_reset` | 48 | exact, 100% |
| `0x80045cf0` | `magic_load_records` | 44 | exact, 100% |
| `0x80045d1c` | `effect_pool_sweep` | 252 | exact, 100% |

The constructor's strict RODATA score remains 51.016262% across its
492-byte switch table. The actor-group caller's 492-byte table remains
37.906506%. Their case-code offsets differ; the existing
[constructor call analysis](kf2-game-effect-constructor-case-local-calls.md)
records the distinct call-count and first raw-instruction differences. The
unchanged strict scores and current focused DIFF results provide no new reason
to steer either C body or table addend.

## Owner bounds and emitted storage

Retail is linked and has no surviving BSS symbols or TU boundaries. The
following retail symbol sizes and `.bss` sections are **delinker model**
outputs. They can be tested against raw startup clears and consumer address
pairs, but are not independent proof of the original source definition.

| Curated GAME identity | Consumer/extent evidence | Fresh candidate emission | Bounded verdict |
| --- | --- | --- | --- |
| `actor_state`, `0x8016b600..0x801749cc` (`0x93cc`) | `game_main_loop` constructs the base at `0x80013728/2c`; its direct `repeat_store_word` call passes zero and `0x24f3` words (`0x93cc` bytes). Two hundred actor records have `0x7c` stride; actor-pool reset and the group caller use fields inside the same span. The delinked `actor_pool_clear` module has a `0x93cc` `.bss` symbol. | Source type is `0x93cc`; GCC emits `COMMON actor_state` of `0x93d0` bytes, four bytes larger. | Complete shared runtime span is supported. The original defining TU and COMMON-to-BSS placement mechanism remain open. Do not pad the C type. |
| `effect_state`, `0x8019b6a8..0x8019e134` (`0x2a8c`) | Startup clear constructs the base at `0x8001373c/40` and clears `0xaa3` words. Its `0x680` magic-record prefix, 128 records at 72-byte stride, and three trailer fields exactly exhaust the span. Pool search/reset/sweep and exact scatter helpers use owner-relative addresses. The delinked `effect_reset` module has a `0x2a8c` `.bss` symbol. | Source type is `0x2a8c`; GCC emits `COMMON effect_state` of `0x2a90` bytes, four bytes larger. | The complete shared object is supported; its provisional `effect_reset` source placement is not historical TU proof. Do not split interior fields into globals. |
| `DAT_801c7068`, curated `0x8` | Scatter at `0x80042298` writes signed motion halfwords and passes the address to `copyVector`; `0x80042424` reads offsets `0`, `2`, and `4`. The delinked scatter module models eight `.bss` bytes. | `SVECTOR` emits an eight-byte COMMON request; all three scatter function texts are strict exact. | The motion components and address identity are observed; the original allocation boundary and defining TU are unproved. Preserve the address-derived identity. |
| `DAT_801d9628`, curated `0x900` | Constructor's reviewed signed-low pair at `0x80040a50/54` constructs the trail base. Four slots at a 576-byte stride with 24-byte rows establish the modeled 2,304-byte span. The delinked constructor module groups this at `.bss+8` after a distinct four-byte cooldown claim and four alignment bytes. | Source array emits a 2,304-byte COMMON request. The separate cooldown word emits eight COMMON bytes; the constructor text remains WIP. | The row/slot use is supported, but no independent clear or complete allocation boundary proves this original array extent or TU. Do not promote the candidate owner or infer contiguity from the synthetic module. |

`DAT_8006d704` is the constructor's separate four-byte initialized ring
index. The constructor reads/writes it modulo four; its address pair and zero
load image do not make it part of the trail BSS array. The unrelated
`DAT_8009a5a8` cooldown word likewise remains a separate candidate even
though the delinker places it before the trail array in one synthetic module.

The current probe's tentative definitions consistently request COMMON
storage, including the two complete objects whose sizes round upward to the
next eight-byte multiple. The linked addresses and exact consumer text do not
select a compiler flag, original TU, or linker allocation rule. No source,
identity, relocation, or build-configuration edit follows from this pass.
Only focused compiles, isolated strict objdiff, symbol-section inspection,
reviewed address pairs, and `git diff --no-index --check` were used; no linked EXE,
repository tests, lint, broad match, or full build ran.
