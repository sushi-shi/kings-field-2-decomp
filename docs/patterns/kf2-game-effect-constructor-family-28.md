# GAME effect constructor and pool/spawn family: 28-function verdict

The GAME constructor at `0x80040308` remains WIP. A fresh focused rebuild,
safe one-VA target, and isolated strict comparison reproduce **98.43441%**
text and **51.016262%** RODATA on equal 5,092-byte bodies. Its CFG has
116/116 blocks and 22/22 branches. All 157 ordered text relocations retain
their type and target; the 123 pointer rows retain 62 target classes and
65 exact addends. The first raw text gap is kind 26's repeated zero-byte
store in a jump delay slot. Later kind-6 pointer movement, kind-102 signed
halfword reload, and a two-word load order differ without a new field,
call, or branch destination. Those are not grounds to add duplicate writes
or change the typed arguments.

For 26 related functions, eleven focused unit builds all reported SAME.
A fresh safe 26-VA carve produced **294 relocations, none withheld**;
eleven independently compiled unit objects each compared at strict 100%
text for every function below. These are preserved exact controls, not new
closures. Sizes are retail function bytes.

| GAME VA | Function | Bytes | Strict verdict |
| --- | --- | ---: | --- |
| `0x8003c614` | `func_8003c614` actor-group caller | 2,672 | 86.041916% WIP |
| `0x8003fa2c` | `effect_play_spatial_sound` | 60 | 100% |
| `0x8003fb94` | `func_8003fb94` | 536 | 100% |
| `0x8003fdac` | `effect_magic_power` | 36 | 100% |
| `0x8003fdd0` | `func_8003fdd0` | 224 | 100% |
| `0x8003feb0` | `func_8003feb0` | 104 | 100% |
| `0x8003ff18` | `func_8003ff18` | 424 | 100% |
| `0x800400c0` | `func_800400c0` | 244 | 100% |
| `0x800401b4` | `func_800401b4` | 108 | 100% |
| `0x80040220` | `effect_pool_find_free` | 68 | 100% |
| `0x80040264` | `effect_pool_initialize_scaled` | 64 | 100% |
| `0x800402a4` | `effect_pool_initialize_fixed` | 100 | 100% |
| `0x80040308` | `func_80040308` constructor | 5,092 | 98.43441% WIP |
| `0x800416ec` | `effect_rotate_scale_offset_y` | 144 | 100% |
| `0x8004177c` | `func_8004177c` | 480 | 100% |
| `0x8004195c` | `func_8004195c` | 440 | 100% |
| `0x80041b14` | `func_80041b14` | 444 | 100% |
| `0x80041cd0` | `func_80041cd0` | 172 | 100% |
| `0x80041d7c` | `func_80041d7c` | 144 | 100% |
| `0x80041e0c` | `func_80041e0c` | 136 | 100% |
| `0x80041e94` | `func_80041e94` | 664 | 100% |
| `0x8004212c` | `func_8004212c` | 364 | 100% |
| `0x80042298` | `func_80042298` | 396 | 100% |
| `0x80042424` | `func_80042424` | 204 | 100% |
| `0x800424f0` | `func_800424f0` | 352 | 100% |
| `0x80045cc0` | `effect_pool_reset` | 48 | 100% |
| `0x80045cf0` | `magic_load_records` | 44 | 100% |
| `0x80045d1c` | `effect_pool_sweep` | 252 | 100% |

The related actor-group caller was independently rebuilt against a safe
one-VA target (305 relocations, none withheld). Its strict text and RODATA
remain **86.041916%** and **37.906506%**; CFG is 45/45 blocks and 15/15
branches. Retail physically calls the direction helper eleven times versus
seven in the candidate, and the constructor nine versus ten. Its 123
switch rows retain all 19 target classes. Earlier case-local and shared
call-tail controls show GCC merging source-expressed calls; several
higher-scoring variants passed wrong variadic arguments. No source edit was
retained in this 28-function pass.

The exact pool scan, initializers, spawns, collision/motion helpers, and
pool sweep constrain the constructor's call ABI. They do not prove the
retail constructor's source spelling or the original compiler profile.
