# GAME effect dispatcher at 0x80042650

The retail GAME function `effect_update_dispatch` spans `0x80042650..0x80045cc0`
(`0x3670` bytes). Its sole proven call is from `effect_pool_sweep` at
`0x80045dcc`, after that function publishes `effect_state.current_record` and
`current_magic`. The entry reads the current record pointer from `0x8019e12c`,
the current magic pointer from `0x8019e128`, kind byte `record+1`, and phase byte
`record+7`. It takes no arguments and returns no value. The frame is 224 bytes;
the sole return at `0x80045cb8` restores that frame in its delay slot.

This is game code. It updates 72-byte effects, spawns child effects, queries
collision and actors, adjusts status, and plays effect sounds. There is no
vendored-function attribution, SDK dispatcher shape, or string reference.
The neighboring `0x80042634`-ending scatter helper and `effect_pool_reset` at
`0x80045cc0` establish code boundaries but do not prove original TU ownership.
The King's Field I source `src/game/effect_dispatch.c` is a semantic comparison:
its 49-entry table and `0x180c`-byte function are materially smaller and do not
provide a direct KF2 body.

## Dispatch tables

At `0x80042688` the code bounds-checks the unsigned kind against 122. The
subsequent `lui/addiu`, scaled index, `lw`, and `jr v0` at `0x80042694..a8`
index `0x8001268c`. All 123 little-endian words at `0x8001268c..0x80012878`
are aligned addresses inside this function, with 59 distinct destinations;
unlisted kinds below 123 reach the common return. `0x80012878` is a zero word.
At `0x80044f60..84`, phase is separately bounded to 0..4 and indexes five
in-body pointers at `0x8001287c..0x80012890`:

| Phase | Target |
| ---: | ---: |
| 0 | `0x80044f8c` |
| 1 | `0x80044ff4` |
| 2 | `0x800451c0` |
| 3 | `0x800453c0` |
| 4 | `0x80045408` |

The next word at `0x80012890` targets `0x800463cc` and belongs to a different
table. `data.tsv` and `data_identities.tsv` now represent the two complete
ranges as one pointer table each instead of 128 isolated Ghidra four-byte
objects. All 128 table pointer rows in `relocs.tsv` are reviewed: each raw
word matches its curated target, and the focused target emits the expected
`R_MIPS_32` site and in-function addend. The current dispatcher source
claims `0x8001268c..0x80012890` as one RODATA range (`0x204` bytes, including
the zero separator), without separate table globals. Focused target and source
objects each contain 516 bytes of RODATA and 128 ordered `R_MIPS_32` rows.
The code-offset addends remain WIP; the table words' target-equivalence
classes agree for all 123 kind entries and all five phase entries.

The kind table's occupied handler entries are:

| Kind indices | Entry address | Kind indices | Entry address |
| --- | --- | --- | --- |
| 0 | `0x80043624` | 1 | `0x80043374` |
| 2 | `0x80043c18` | 3 | `0x80045918` |
| 4 | `0x80042b14` | 5 | `0x80043f44` |
| 6 | `0x80044f60` | 7, 49 | `0x80042858` |
| 8 | `0x80044868` | 9 | `0x8004441c` |
| 10 | `0x80044c7c` | 11, 54 | `0x80043af8` |
| 12 | `0x80043cd0` | 13, 32 | `0x800428b4` |
| 14 | `0x80045700` | 15 | `0x80045628` |
| 16 | `0x80045680` | 17 | `0x80045654` |
| 19 | `0x800457c4` | 20 | `0x80043cb8` |
| 22 | `0x800458e0` | 23 | `0x80042934` |
| 24 | `0x80045a58` | 25 | `0x80042b98` |
| 26–27 | `0x80043460` | 28 | `0x8004336c` |
| 29, 31, 48 | `0x800426b0` | 30, 47 | `0x800426b8` |
| 33 | `0x800445e8` | 34–35 | `0x80042b90` |
| 38 | `0x80043130` | 39 | `0x8004310c` |
| 40 | `0x800430d4` | 42 | `0x80042bfc` |
| 45 | `0x80042f08` | 46 | `0x80042dc8` |
| 50 | `0x80043264` | 51 | `0x80043b98` |
| 52 | `0x80043bb0` | 53 | `0x800445e0` |
| 100 | `0x80043e2c` | 101 | `0x80045580` |
| 102 | `0x800455dc` | 103, 121 | `0x80043700` |
| 104, 122 | `0x800439a8` | 105 | `0x800441cc` |
| 106 | `0x80044748` | 107 | `0x800454a8` |
| 109 | `0x80045b6c` | 111 | `0x80043508` |
| 113 | `0x80042d14` | 114 | `0x8004593c` |
| 115 | `0x80042c30` | 116 | `0x80042f20` |
| 117 | `0x80043008` | 118 | `0x80043bd4` |
| 119 | `0x80043bdc` | 120 | `0x80045bb0` |

All other kinds from 0..122 point to `0x80045ca0`, the common epilogue.
The two kind-29/30 prefixes select different constants before joining at
`0x800426bc`; repeated entries are genuine shared handlers, not duplicate
source functions.

## Reconstruction status

The static disassembly has 446 navigator blocks, two unresolved switch `jr`
sites, one return, and 206 direct `jal` sites. The most frequent calls are
`func_80040308` (27), `rand` (22), `func_80042298` (20), `func_80041e94`
(14), `func_8003feb0` (13), and `func_80041e0c` (10). All 206 direct call
targets are decoded. Of 336 current in-body relocation rows, 334 `mips26`
control transfers are reviewed and the two HI16/LO16 table-base pairs remain
candidate. The Ghidra decompiler proposal removes ten purported
unreachable blocks and supplies incorrect speculative arguments to `rand` and
other calls; it is a guide to inspect, not trustworthy C source.

The current `src/game/effect_update_dispatch.c` claims the full retail body
at `0x80042650 / 0x3670` and models many kind and phase arms. A fresh
isolated pinned-probe build emits 13,804 text bytes against retail's 13,936;
direct strict objdiff reports **95.181404%**, so this remains a substantive
WIP.
The first compiled instructions already differ in frame size and saved-register
setup. Retail kind 6 uses a five-entry phase jump table and its phase-1 path
jumps directly to the phase-2 handler; the nested C switch and join now emit
the second table. Five further case-entry splits restore distinct retail
prefixes for kinds 28, 30/47, 34/35, 39, and 119. The 123-entry kind table
now has the same 59 target-equivalence classes in the same order as retail,
but their code offsets differ. The collision-cache words around
`0x801d8d40..0x801d8d5c` remain provisional because the current equipment
record extent overlaps them; several dispatcher arms read those words. Do not
create overlapping globals or collapse those arms to improve the metric.
Retail entry loads `current_record`, `current_magic`, the unsigned kind byte,
and the phase byte before dispatch. Kind zero compares that saved phase after
several calls, and shared kind-103/104 handlers distinguish the saved kind.
The source now captures those entry values before the switch instead of
reloading them inside later arms. Raw branches in kinds 5, 6, 7/49, 8, 10,
12, 13/32, 23, 29–31/47–48, 100, 104/122, and 105–107 consume the saved
phase; the source now uses that entry byte at those sites. Kind 103/121
instead loads the current phase field at its entry, while kind 104/122 later
reloads that field for its sound calculation, so those reads stay fresh.
The focused prelude consequently loads and
keeps magic, kind, and phase before the table, though its frame and register
allocation still differ from retail. KF1's smaller `effect_dispatch.c` also
captures current record, magic, kind, and phase before its switch; this is a
source-shape lead, with the KF2 prelude and kind-zero branch providing the
independent evidence here.

The fresh isolated strict objects each have **206** direct `jal` sites,
including **27** calls to `func_80040308`. The earlier 205/206 count is
superseded. Around retail offset `+0x1258`, objdiff aligns a constructor
`jal` with the candidate's corresponding call two instructions earlier;
the apparent delete/insert is argument-setup scheduling, not a missing call.
Kind 114's source constructor is also present in the compiled body. No
duplicate source call is supported by this evidence.

The first non-register/layout divergence in the current aligned text is at
retail offset `+0x2d4`: retail stores the third scale halfword, increments
the phase byte, and jumps to the return path with the phase store in its
delay slot. The candidate places the scale store in the jump delay slot and
reaches a shared phase-increment tail. Both paths perform the same field
writes. This is a physical tail-layout difference; adding a second phase
update would change the modeled behavior.

Retail kind 20 at `0x80043cb8` prepares
`(0x4000, 0x100, 0x20)` and jumps into the shared `func_80041cd0` call at
`0x80044f30`; the same join receives a kind-12 phase path and a kind-10 path.
That call's
continuation increments record halfword `+0x26`, which is rotation Y. The
source's former kind-20 Z increment was corrected to Y, and the three source
paths now join before one call. The kind-12 collision continuation jumps to
the kind-8 `func_80041e94` call at `0x80044aa4`, so those source paths now
share one call. Kinds 7/49 and 13/32 share the retail
`func_80042424`/`func_8003feb0` block at `0x800428d8` and the growth tail at
`0x80042908`; the source now models that join. Conversely, kind 103/121 has
two distinct `func_80042424` sites at `0x80043760` and `0x80043774`, and
the source spells them in their respective phase branches. The focused
listing was 13.0% similar at that intermediate stage; the lower intermediate
listing score did not falsify those directly decoded paths. The current
isolated strict score is the 95.181404% reported above.

The subsequent primary-target pass checked the retail body and all incoming
and outgoing xrefs again. The first kind handler, at `0x80043624`, increments
record +0x36 by 20, raises the three scales at +0x2c/+0x2e/+0x30 by 256 while
the first is below 3072, and calls `func_80042298(180, 0, -300)`.
On a nonzero result it may call `func_8003feb0` after a zero-argument `rand()`
test, latches record +0x40 on result bit 0x10, and either frees the record or
sets the now-typed phase byte at +0x07 to one on result bits 0/2. That latter
path copies the signed boundary word at `0x801d8d50` into position Y and
stores -200 at +0x36 before calling the exact `func_80041e0c` helper. The
`rand()` call's delay slot is a nop; the Ghidra candidate incorrectly supplies
it an effect-position argument. `KfEffectRecord.phase` and its checked
inventory row now reflect the directly decoded five-phase dispatch and the
existing zero-direction spawner's write of phase two. Its layout and two exact
spawner listings are unchanged. The collision boundary still lacks a
non-overlapping source owner, so this arm remains a source-model sketch rather
than a fabricated dispatcher C claim.

The kind-six handler at `0x80044f60` independently bounds the phase byte to
0..4 before the five-entry indirect jump. Its phase-zero arm at
`0x80044f8c..0x80044ff0` loops eight times, calls the effect constructor with
type `0x0a` and kind `0x6b`, writes the parent effect index from
`effect_state.current_index` to each child at +0x40 and the loop ordinal to
+0x41, selects render ID `0x17` for the first and `0x18` for the last child,
then advances the parent's phase to one. Thus record +0x40 is an effect-kind
dependent auxiliary byte: the kind-zero handler uses it as a collision latch,
while this handler uses it as a parent index. A single global semantic field
name would be misleading; the record tail remains opaque pending a supported
variant view.

## Adjacent focused controls

The 11 neighboring effect-helper units were rebuilt as isolated objects and
direct strict-compared after the dispatcher changes. Every listed function
has a 100% text verdict:

| Unit | Function VAs | Data verdict |
| --- | --- | --- |
| `effect_spatial_sound` | `0x8003fa2c` | exact |
| `effect_update` | `0x8003fb94`, `0x8003fdac`, `0x8003fdd0`, `0x8003feb0`, `0x8003ff18`, `0x800400c0`, `0x800401b4`, `0x80040220`, `0x80040264`, `0x800402a4` | exact |
| `effect_rotate_scale_offset_y` | `0x800416ec` | exact |
| `effect_move_probe` | `0x8004177c` | exact |
| `effect_aim_and_move` | `0x8004195c` | exact |
| `effect_target_motion` | `0x80041b14` | exact |
| `effect_scale_step` | `0x80041cd0` | exact |
| `effect_spawn_zero_direction` | `0x80041d7c`, `0x80041e0c` | exact |
| `effect_spawn_motion` | `0x80041e94`, `0x8004212c` | exact |
| `effect_scatter` | `0x80042298`, `0x80042424`, `0x800424f0` | retail `.bss` 8 bytes; source COMMON/no section |
| `effect_reset` | `0x80045cc0`, `0x80045cf0`, `0x80045d1c` | retail `.bss` 10,892 bytes; source COMMON/no section |

The two BSS placement differences are strict-data WIPs. Their source
definitions are the supported `DAT_801c7068` and `effect_state` objects;
neither difference justifies a fabricated initializer or relocated owner.
The compiler's `.def` for `effect_state` records its typed 10,892-byte extent,
but its `.comm` directive rounds the tentative allocation to 10,896 bytes.
This compiler/placement behavior remains separate from the retail object's
10,892-byte BSS claim. The King's Field I analogue in `effect_pool.c` also
uses a tentative `effect_state` definition, a useful lead but not proof of
KF2's original declaration form.
