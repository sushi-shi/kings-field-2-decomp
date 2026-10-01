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
at `0x80042650 / 0x3670` and models many kind and phase arms. A focused
rebuild emits 13,984 text bytes against 13,936 retail bytes; direct strict
objdiff reports 0.0%, so this is a substantive WIP, not an exact function.
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

The direct external-call multiset is still unequal: source has 209 `jal`
sites against retail's 206. Relative to retail, source has one extra
`func_8003feb0`, two extra `func_80041cd0`, one extra `func_80041e94`, and
one fewer `func_80040308`. Retail kind 20 at `0x80043cb8` prepares
`(0x4000, 0x100, 0x20)` and jumps into the shared `func_80041cd0` call at
`0x80044f30`; the same join receives a kind-12 phase path. That call's
continuation increments record halfword `+0x26`, which is rotation Y. The
source's former kind-20 Z increment was corrected to Y; the focused object
still compiles at 13.3% listing similarity. The remaining shared-call shape
and other call-count differences need direct path-by-path reconstruction.

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
