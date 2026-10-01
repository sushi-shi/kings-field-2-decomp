# GAME prepared-TMD subdivision and render controls

This 25-function set follows the confirmed `0x80030c18 -> 0x8002ff5c`
prepared-object call, the TMD packet walkers, and the renderer that consumes
their packet and projection state. Addresses are GAME.EXE identities. Exact
controls were certified in earlier strict reports and remained focused `SAME`
at the last source check; percentages below are focused listing comparisons,
which do not establish strict closure. No source claim was added for the
prepared-object builder.

| Function | Verdict | Connection or unresolved evidence |
| --- | --- | --- |
| `0x8002d4f4` | Exact control | Updates the view matrices and map-cell coordinates read by the renderer. |
| `0x8002d5dc` | Exact, direct strict 100%; focused SAME | One typed packet-body pointer handles both eight-mode index conversion and next-packet advance; `.text`, switch `.rodata`, and all 39 relocation rows match retail. |
| `0x8002d918` | Exact control | Projects TMD vertices with fog-dependent depth. |
| `0x8002da94` | Exact control | Projects alternate vertices for the prepared-object renderer. |
| `0x8002dd28` | Exact control | Converts TMD vertices into the shared projected-vertex array. |
| `0x8002ddb4` | WIP, focused 93.6% | Textured packet walker: retail/probe CFG 44/42 blocks and 28/26 branches. |
| `0x8002e4dc` | WIP, focused 92.0% | Paired textured walker has the same two depth-check branch gap. |
| `0x8002ebe0` | WIP, focused 88.7% | Lit packet walker has one depth-tail block not reproduced by the source. |
| `0x8002f194` | Exact control | Enqueues prepared FT3/FT4 packets through typed GT3/GT4 GPU views. |
| `0x8002f5b0` | WIP, focused 82.6% | Clipped fan uses the complete SDK `EVECTOR`; UV/color store order differs. |
| `0x8002f808` | WIP, focused 50.5% | Prepared-asset renderer has 51/51 CFG blocks and 35/35 branches, but the retail frame is 168 bytes against 112. |
| `0x8002ff5c` | Unclaimed WIP | Four-packet FT4/FT3 subdivision has 14 retail copy calls and an unresolved packet-field/local-record model. |
| `0x80030c18` | WIP, focused 98.7% | Sole prepared-builder caller has matching 11/11 blocks and 6/6 branches; independent byte load and argument move swap order. |
| `0x80030de4` | Exact control | Selects the map-cell layers that call the prepared-object path. |
| `0x80030f5c` | Exact control | Scans the render mask after the TMD selection. |
| `0x80031024` | Exact control | Walks the render grid and calls the map-cell layers. |
| `0x800311b0` | WIP, focused 57.5% | Builds an FT4 screen quad; the semitrans/code store and saved-register schedule differ. |
| `0x800312f4` | Exact control | First textured sliding-panel caller of the quad writer. |
| `0x80031384` | Exact control | Paired sliding-panel caller of the quad writer. |
| `0x80031414` | Exact control | Color-byte overlay caller of the quad writer. |
| `0x800314d4` | Exact control | Supplies the overlay's four graphics control/color bytes. |
| `0x80031850` | WIP, focused 68.9% | World-model renderer has 40/40 blocks and 16/16 branches, with different early successor order. |
| `0x80031d8c` | WIP, focused 79.5% | Animated-object renderer has 7/7 blocks and 2/2 branches; frame and register allocation differ. |
| `0x8003247c` | Unclaimed WIP | Frame child reaches both TMD/resource updaters and repeated world-model draws; the mixed workspace remains incomplete. |
| `0x800335a0` | Exact control | Per-frame driver calls the render/resource child. |

The focused one-function object for `0x8002ff5c` is `0xcbc` bytes with an
11-block, six-branch CFG and a 1248-byte frame. Entry multiplies the unmasked
32-bit object-index argument by the 28-byte object stride; the current `s32`
formal does not add a callee-side halfword truncation. It copies the first
FT4 child packet for eight words, a four-word texture record, and then the
other three children for eight words each. FT3 uses a six-word packet and a
three-word texture record. A generic packet copy and three final vertex,
midpoint, and normal copies bring the static `resource_copy_words` call count
to 14. FT4 appends five midpoint `SVECTOR`s and 128 packet bytes; FT3 appends
three midpoint vectors and 96 packet bytes. The caller limits source
primitive count below 16, bounding generated midpoints at 75 and generated
packet bytes at 1920. The full 4096-byte fit also depends on source vertex
and normal counts, which this call-site check alone does not prove.

A temporary C probe reproduces all 14 call sites and six branches, but emits
12 CFG blocks, a 1232-byte frame, and only 10.2% normalized listing
similarity. The extra block is on the FT3-to-common-tail path. Reordering
entry assignments, deriving packet pointers through the object records, and
spelling the loop as a precheck plus bottom decrement did not recover retail
control or frame placement. A byte-UV-pair probe compiled to 12 blocks and
9.1% similarity. A source-only packed-word write for the first FT4 child’s
two generated indices reproduced one retail store width but left the same
12-block CFG and reached only 10.4%. These probes remained in `/tmp`; no
game source was changed.
At retail loop entry `+0xa0` and `+0xac`, the header word is loaded twice
from the same source pointer and stored twice to the same four-byte stack
view before its mode byte is read. There is no intervening call or store to
the packet. The C spelling or alias relationship behind this duplication is
not established, and a forced duplicate read would be unsupported.

The first retail FT4 child writes generated UV components individually with
`sb` at packet offsets `+8..9`, `+12..13`, and `+16..17`, then writes packed
index pairs with `sw` at `+24` and `+28`. Later children again copy the source
packet before overwriting selected UV and index bytes. The temporary source
still emits halfword index stores and has no demonstrated local view that
explains the mixed byte/word writes. The existing `KfTmdFt3`/`KfTmdFt4`
read views and `KfTmdPreparedAsset` output extent remain supported, while
the builder's child-packet write type and 16-byte frame difference remain WIP.

The common loop tail derives the next source-packet address from the packet
header's byte at offset `+1`: it adds one, shifts by two, and advances the
source pointer by that word count. The unrecognized-mode arm copies exactly
that many words and advances the output pointer by the same byte count. Each
split arm advances its output pointer by its four-child extent before joining
the same source-pointer tail. The final three copies then append original
vertices, generated midpoint vertices, and normals in that order. These
pointer relationships are supported by the retail instruction sequence;
they do not establish the original C union or packet write type.

The builder reads source vertex offsets with `lh`; downstream prepared-packet
walkers read their remapped indices with `lhu`. A future raw-source view must
keep that consumer-specific signedness without changing the exact prepared
walker types.

The corner-vertex scratch has a separate alignment constraint. Copying an SDK
`SVECTOR` aggregate made this compiler use `lwl/lwr` and `swl/swr`, while
retail reads two aligned words and stores two aligned words for each corner.
A temporary four-entry union with an `SVECTOR` field and two-word view, using
one typed source pointer per corner, reproduced that local `lw/lw/sw/sw`
shape. It reduced the temporary frame from 1232 to 1224 bytes, farther from
retail's 1248 bytes, and did not fix the entry order or complete packet writes.
The word view remains a source hypothesis rather than a retained claim.

A second temporary probe combined that aligned corner view with the retail
bytewise UV and index writes. The first FT4 child writes paired generated
indices as two full words at packet `+24` and `+28`; the second word also
replaces the nominal padding halfword with the previously generated index.
The other child indices are assembled through a four-byte stack scratch and
stored as individual bytes. This is a real packet-byte difference from the
first probe's halfword field assignments. Direct one-function objdiff improved
from 27.071165% for the 0x984-byte first probe to 33.385277% for the
0xcb0-byte byte-write probe, versus a 0xcbc-byte retail body. Both have the
14 direct copy calls and six conditional branches; the newer probe still has
a 1192-byte frame against 1248 retail and remains unclaimed.

Keeping the five midpoint UV pairs as computed local values and reading their
individual bytes at each child write improved a further temporary probe to
43.1227% direct objdiff. It still has 14 copy calls and six conditional
branches; its body is 0xb84 bytes and its frame 1208 bytes. This supports the
retail value reuse across child-copy calls but leaves 40 frame bytes and much
of the instruction order unexplained. No version was installed in the game
source.
Changing only those cached pair locals from 16-bit to 32-bit values gave
43.646626% direct objdiff with the same 0xb84-byte body. The retail masked
word operations support testing this width, but the small score movement is
not proof of the original declaration; the wider probe is also discarded.

A later temporary probe advanced one `SVECTOR *` cursor after each generated
midpoint instead of addressing `midpoints[count + n]`. This follows retail's
sequential eight-byte writes and raised direct objdiff to 46.58282%. Advancing
the destination packet cursor after each 32-byte FT4 or 24-byte FT3 child
copy, as the retail stores and copy calls do, raised it to 49.0454%. Moving
the input word count from loop entry to the generic arm and common loop tail
raised it to 49.46135%. That last probe has a 0xbbc-byte body and a
1224-byte frame, against retail's 0xcbc bytes and 1248-byte frame. The
14-copy call set and six conditional branches agree, but the first remaining
instruction difference is the frame/local setup, followed by packet scratch
register and store order. An equivalent two-pointer view of the midpoint
`x` and `z` halfwords did not improve alignment enough to justify a source
claim. All variants remain temporary and the function remains unclaimed.

Retail's UV midpoint path updates byte lanes inside word-sized temporaries
with masks and ORs, then writes their low two bytes into child packets. A
temporary four-byte union with `u8` UV fields reproduced more of that shape:
the 14-copy, moving-cursor probe reached 60.007362% direct objdiff with a
0xc94-byte body and a 1288-byte frame. Placing the output-object and
midpoint-cursor initialization before source-object reads reached 60.429447%
and a 0xc98-byte body. The 40-byte frame excess, remaining packet-store
order, and purpose of the union's unused high bytes are unresolved. A plain
four-byte struct fell to 48.98405%; a two-byte union reached 57.235584% but
expanded the frame further. These are controlled type probes, not proof of
the historical declaration. Only the computed UV bytes are read, and no
uninitialized bytes are copied to the prepared output in the probe.

The common source-packet advance exposed a real semantic error in those
earlier temporary bodies. Child-index byte writes reuse the four-byte local
that first held the packet header, overwriting its input-length byte. Retail
loads the source header again at `+0xb9c` before reading that byte and
advancing the source pointer. Adding the same reload to the temporary C
raised direct objdiff to 62.844173% and produced a 0xcbc-byte body, exactly
the retail body size, with all 14 static copy calls. The compiled frame is
still 1280 bytes against retail's 1248, and the packed UV union ownership and
many instruction-order differences remain unresolved. The body is not a
defensible production claim yet.

The FT4 and FT3 arms both derive midpoint UV pairs inside one packet loop.
Hoisting the temporary four-byte UV scratch views to that loop scope, so the
mutually exclusive arms share their AB/AC storage, improved direct objdiff
from 62.844173% to 68.00982%. Reusing FT4's fifth scratch slot for FT3's BC
pair reached 69.36073%. Both objects retained the 0xcbc-byte body and 14
copy calls; the frame shrank from 1280 to 1272 bytes, still 24 bytes larger
than retail. Hoisting the integer midpoint indices as well regressed to
62.53129%, so that narrower source shape is discarded. These temp-only
results support shared UV scratch lifetimes but do not prove the original
union declaration or the remaining frame/storage layout.

Retail spills `t5`, `t8`, and `t9` around the first packet copy before their
first visible assignments. The low UV bytes later read from those words have
their prior bits masked out before they are stored, so this does not establish
extra input arguments. It is consistent with packed-word scratch whose live
byte components are filled incrementally. The temporary source recomputes
byte averages at each child write and therefore does not preserve the same
live word values across calls. The source initializer and full packed record
are still unknown; adding undefined locals or stack padding would not be an
evidence-backed correction.

The pinned Psy-Q `LIBGPU.H` describes `TMD_PRIM` as a decoded multipurpose
record with byte UV fields and separate vertex/normal arrays; it does not
have this copied on-disk packet layout, so using it as the builder's packet
type would not explain the retail offsets.

For the paired walker at `0x8002e4dc`, a source-only nested positive-depth
condition in the FT3/GT3 arms compiled to the same focused listing as the
existing early-break form. Its eight exact TMD-pipeline siblings stayed
`SAME`; the two missing retail branch blocks are still unattributed.

Retail `0x8002ebe0` routes all four packet kinds through one shared tail at
`+0x534`: it writes the packet code byte, sign-extends the depth from a
shifted word, checks positive depth and the 8192-entry table bound with two
branches, and enqueues the packet. A temporary source-only common-tail probe
raised its focused listing from 88.7% to 89.2%, but still emitted 25 rather
than 26 blocks and 16 rather than 17 branches. A further temporary 32-bit
third-argument view with a 16-bit cast at the tail reached 89.5%; writing the
two depth exits separately recovered the retail `blez` shape but kept the same
89.5% listing. The caller passes an O32 word derived from its signed depth;
these probes do not prove the callee's original formal width. Retail stores
the blend word at `sp+32` before `tmd_get_object`, then shifts it after the
call, while both probes instead preserve it in a saved register. Retail also
uses a 96-byte frame against the retained source's 104-byte frame. No shared
header or production walker was changed for these incomplete probes; eight
exact siblings stayed `SAME` in each focused build.

The clipped fan at `0x8002f5b0` also has a typed-copy boundary. Replacing its
three aligned four-byte color copies with direct `CVECTOR` struct assignments
made the pinned compiler emit `lwl/lwr` and `swl/swr` for the byte-aligned SDK
type, while retail uses `lw/sw`. That source-only trial fell from 82.6% to
72.3% focused and preserved the exact `0x8002f194` sibling. The retained
four-byte views reflect the actual aligned stack, `EVECTOR`, and GPU packet
locations; no struct-assignment rewrite was kept.
