# GAME collision-shape opcode switch

GAME `0x8002aaa4` is a game collision-shape dispatcher with three proven
callers at `0x8002b604`, `0x8002b67c`, and `0x8002b7f8`. Its C claim is
`game.collision_shape_dispatch`; the current body remains WIP.

Retail `0x8002ac34` subtracts `0x10` from the unsigned record opcode and
rejects indices above `0x30`. The following table-base load at
`0x8002ac54/0x8002ac58` and indirect `jr` at `0x8002ac68` select one of
49 initialized pointer words at `0x8001134c..0x8001140c` (Japanese EXE file
offsets `0xb4c..0xc0c`). All 49 raw words equal the curated target addresses.
Their 13 distinct values are block heads within `0x8002aaa4..0x8002b603`.
The 37 default entries point to `0x8002b5c4`. These facts validate the
pointer data and its extent; the semantic CFG still treats the `jr` edge
as indirect, without promoting individual case successors to proven calls
or branches.

The non-default table indices correspond to opcodes `0x10`, `0x11`, `0x18`,
`0x19`, `0x20`–`0x23`, `0x30`–`0x32`, and `0x40`; all other in-range opcodes
select the common count-decrement block. The record cursor advances by one
halfword in the switch's out-of-range delay slot at `0x8002ac4c`, before
any case body. Opcode `0x31` has a conditional payload: if result flag 1 is
clear, `0x8002b3ec` jumps straight to the common count decrement without
the `t0 += 10` at `0x8002b3f4`. The C claim preserves this one-halfword
advance on that path and the six-halfword advance when the flag is set.

The cursor increments bound the recognized command records independently of
the still-unknown on-disc table contents: `0x10`, `0x18`, and `0x19` consume
two halfwords; `0x11` consumes three; `0x20`–`0x23` consume five; `0x30`
and `0x32` consume seven. Opcodes outside the table's handled cases consume
only the opcode halfword. Opcode `0x40` switches the selected map layer and
restarts the shape lookup, rather than advancing this record cursor. These
lengths include the common `+2` at `0x8002ac4c` plus the case-specific
increments at `0x8002ac80`, `0x8002acbc`, `0x8002ad38`, `0x8002ae2c`,
`0x8002af04`, `0x8002b004`, `0x8002b0cc`, and `0x8002b2d8`.

The phase-one loader in `0x80016820` copies `0x600` words (`0x1800` bytes)
to `bss_801c7540+0x10000`, the bank base used by this dispatcher. The copy
ends at +`0x11800`, before the collision-cache tail. It bounds the loaded
region but does not prove the variable-length record layout or a separate
source definition for that interior address. The range overlaps the
provisional map-cell and equipment views of this complete BSS object, so a
second global or a permanent field split is not yet justified.

At `0x8002ab5c` the dispatcher reads an unsigned byte from the selected
cell layer, doubles it at `0x8002ab64`, and loads an unsigned halfword from
the bank base plus that index at `0x8002ab74`. It then adds the halfword
offset to the same bank base and reads the selected shape header with `lh`.
Thus the bank begins with an addressable 256-entry halfword-offset table;
the variable-length records and their placement remain unresolved.

The 49 `mips32_candidate` rows at `0x8001134c..0x8001140c` are now
`pointer-reviewed`/`reviewed`. A safe GAME one-VA carve for `0x8002aaa4`
reports zero withheld relocations and emits 49 `R_MIPS_32` entries in
the module's `.rel.rodata`; the module has 144 total relocations, versus
95 in the text-only carve. Both retail and source objects have a 196-byte
`.rodata` table with 49 `R_MIPS_32` rows and the same 37-entry default
grouping. The source uses a named interior view and a layout-checked
256-halfword offset-table prefix for the proven loaded bank range. This
defines no second BSS object and makes no claim about the variable-length
record layout.

The raw table points to the `0x32` body at retail `+0x818`, before the
`0x31` body at `+0x944`; `0x18`/`0x19` also lie after the `0x20`–`0x32`
family. Moving those intact C cases into that retail-backed order preserves
the decoded behavior and raised isolated direct objdiff `.text` from
31.652473% to 47.447803%. Retail then placed the `0x40` second-layer body
at `+0xa34`, immediately before the `0x18` and `0x19` bodies. Moving its
existing C body into `case 0x40` preserves behavior and gives it the same
relative placement: direct strict `.text` is 49.43956%, `.rodata` 26.785713%,
and compiled text is 2848 B versus retail 2912 B. Both objects retain all 49
`R_MIPS_32` table rows and the same opcode equivalence groups, including 37
default entries. Focused listing similarity falls from 15.1% to 13.1% and the
CFG is 174/164 versus 174/165 before this move, with 99/97 branches. The
source remains WIP; the placement and strict gains do not establish the
original compiler schedule. An earlier off-tree copy of the second-layer
body likewise reduced focused similarity and was discarded.

Retail forms an interior cache-height pointer at `0x8002abc0` and reads its
adjacent height, result, and height-limit words. A typed three-field local
view of those words was tried against the moved-case source. It reduced
direct strict `.text` to 48.817307% and `.rodata` to 19.387754%; the view was
reverted. The raw pointer's use is narrower: only the case bodies before
`0x31` read the three fields through it. The case `0x11` alternate-limit
store at `0x8002ad24` instead materializes the BSS address directly, as do
the later `0x31`, `0x40`, `0x18`, and `0x19` bodies. A scoped three-field view
for the early cases, derived from the same bank base plus its proven
`0x1800`-byte copy extent and the cache height's `+0xc` offset, follows that
provenance. Retail separately materializes the bank base for the offset-table
load and the selected header, then derives the cache pointer from the latter.
Spelling the two bank references separately in C gives that ordered
materialization. At retail `0x8002b51c/0x8002b520`, the selected layer's
unsigned elevation is negated before shifting seven bits. The signed,
bounded spelling `-(s32)elevation * 0x80` emits the same order; the previous
`(u32)elevation * -0x80` emitted shift before negation. The isolated strict
result is now 51.95055% `.text` and
27.806122% `.rodata`, with 81 candidate versus 95 retail `.rel.text` rows.
Focused listing similarity is 15.2%; CFG and branch counts remain 174/164
and 99/97. The candidate and retail each have 49 ordered `R_MIPS_32` table
rows. Retail's `0x18` lower-bound case reuses its calculated value for the
result, limit, and tests; using the existing scalar in C removes a duplicate
candidate BSS load. The complete ordered `.rel.text` data-reference sequence
now agrees: 24 HI16/LO16 pairs with the same symbol/type order and all 23
ordered BSS low addends equal. The remaining text-relocation count gap is
internal `R_MIPS_26` jumps (47 retail, 33 candidate). The cache's complete
source ownership remains unresolved.

An off-tree post-decrement spelling of the record-count loop preserved
174/165 CFG blocks and 99/97 branches but lowered focused listing similarity
from 14.0% to 11.5%; it was discarded. The current first control difference
is still the initial count exit: retail decrements the signed count then
compares it with `-1`, while the compiler recognizes the equivalent
zero-count check before decrement. That difference alone does not establish
an omitted command or a new source owner.

An off-tree unsigned `switch (*record)` trial lowered focused listing similarity
from 14.0% to 13.7% and was discarded. The retained signed switch already emits
retail's `lhu` opcode load, subtracts `0x10`, then sign-extends before the
unsigned range check. Thus the C cast does not account for the 174/165 CFG
gap; command-width changes should be grounded in another consumer or record
definition rather than the switch's load opcode alone.

Opcode `0x22` checks two cell-local axes for each quarter-turn. Retail's
rotation-1 body at `+0x4e0` compares the Z fraction with the near bound and
then the X fraction with that same bound; its rotation-3 body at `+0x534`
similarly compares Z and X with the far bound. Expressing those two
rectangular tests directly in their case arms, rather than routing each
second comparison through another opcode's shared label, preserves the
observed geometry and raises the candidate's known branch count from 97 to
**99**, equal to retail. Its CFG grows from 164 to **166** blocks against
174 retail. The focused listing stays at 15.2%; isolated strict text becomes
**52.618134%** on the 2,912-byte retail body, and RODATA is **26.530613%**.
All 49 table rows retain the same 13 target classes; `.rel.text` retains 24
ordered HI16/LO16 pairs and 33 internal jumps. Equivalent direct tests for
the other two rotations moved table addends farther from retail and were
reverted. The rotation-1 correction alone scored 52.843407% text and
27.806122% RODATA, but left one branch absent; adding the raw-backed
rotation-3 test shifts pointer addends and lowers both percentages while
reproducing the retail branch count. This remains WIP; the matching branch
count does not establish complete control-flow equivalence.

The opcode-`0x20` quarter-turn-1 arm is a branch target in retail: its first
rotation compare at body `+0x2ac` branches forward to that arm, while the
other rotations follow through. Reordering the same four C arms around that
condition changes the candidate's first rotation compare from `bne` to the
observed `beq` without changing its geometry. Isolated strict text rises from
52.618134% to 52.887363%; RODATA remains 26.530613%, and focused CFG and
branch counts remain 174/166 and 99/99. Other opcode and table-placement
residues remain WIP.

Off-tree opcode-`0x21` controls also put its rotation-1 arm behind the retail
`beq`, but the compiler placed its shared X-bound continuation differently
from retail. The best such trial reached 52.855770% strict text with unchanged
RODATA; a cross-case shared-label variant reached 52.767857%. Neither improved
the retained 52.887363% result, so the opcode-`0x21` source was left alone.

Opcode `0x31` starts from the already-advanced operand pointer in retail:
the entry delay slot copies that pointer, and body `+0x950` adds ten bytes
for the conditional six-halfword record. Spelling the source cursor as
`operand + 5` emits the matching operand-relative `addiu` rather than a
record-relative twelve-byte increment. Focused CFG/branch counts stay
174/166 and 99/99; isolated strict text is 52.881866%, a small metric
decrease from 52.887363% despite the directly supported pointer provenance.

The opcode cursor is advanced once for every record in retail's range-check
delay slot at body `+0x1a8`; all case-specific increments then start from its
operand pointer. The source now uses a post-increment opcode read and
operand-relative record lengths, including the default one-halfword record.
The probe schedules its pointer increment earlier at `+0x160` rather than in
that delay slot, but its cursor state and command lengths are equivalent.
Focused CFG and branch counts remain 174/166 and 99/99. Isolated strict text
rises to 53.690933%, while RODATA falls to 19.132652% because case-body
addends move; all 49 pointer rows retain the same 13 target classes and
ordered class membership. The candidate has 34 internal text jumps against
47 retail, and all 24 ordered HI16/LO16 data-reference pairs remain.
