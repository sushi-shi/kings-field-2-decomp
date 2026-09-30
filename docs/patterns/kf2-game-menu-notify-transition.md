# GAME menu, notification, and display transition follow-up

This 27-function campaign follows the menu preview and packet helpers into
map-cell resource selection, notification presentation, and the TIM/fade
display transition. Each verdict uses the retail extent, disassembly and CFG,
callers and callees, strings, data references, relocation evidence, adjacent
functions, and the strict objdiff report. KF1's `src/game/menu_runtime.c`
provides the model-preview structure; its matrix and lighting constants are
not assumed to be KF2's.

| GAME address | Retail role and evidence | Verdict |
| --- | --- | --- |
| `0x80020748` | two-option widget, directly precedes model preview | exact |
| `0x8002083c` | gated spinning item TMD preview and GTE matrices | **new WIP, 99.65882%**; 64-byte stack-frame gap |
| `0x80020990` | numeric menu heading and counters | exact |
| `0x80020b50` | translucent sprite packet | exact |
| `0x80020d20` | sprite packet | WIP, 99.44068% |
| `0x80020ef8` | fixed-CLUT sprite packet | WIP, 99.39449% |
| `0x800210ac` | string glyph packets | WIP, 99.66904% |
| `0x80021510` | numeric glyph packets | WIP, 98.61957% |
| `0x800217f0` | nine-slice menu panel | exact |
| `0x80021a68` | begins menu frame and primitive buffer | exact |
| `0x80021be0` | presents menu frame | exact |
| `0x80021c8c` | enters menu display state | WIP, 99.956985%; frame-size residue |
| `0x80021e00` | restores menu display state | exact |
| `0x800221e8` | loads item-model TMD | exact |
| `0x80022300` | dispatches menu sound cue | WIP, 89.97298%; CFG differs |
| `0x800320b0` | ORs map-cell masks across a radius | WIP, 73.95918%; ordered address/loop schedule differs |
| `0x80032174` | tests whether a cell rectangle includes the view cell | WIP, 93.6%; register and result schedule differs |
| `0x800321d8` | allocates and queues TMD resource read | WIP, 98.4359%; fixed-arena address form |
| `0x80032274` | updates flagged VAB range | WIP, 90.933334%; slot induction/frame differs |
| `0x80032fec` | draws one notification quad | exact |
| `0x80033140` | draws active notification rows | exact |
| `0x800331d0` | enqueues notification ID and payload | exact |
| `0x80033274` | sets a notification digit V coordinate | exact |
| `0x80033284` | advances notification phase and dequeues group | exact |
| `0x8003494c` | uploads TIM image(s) | exact after merge |
| `0x800349bc` | four-quad fade and pad state | WIP, 92.34296%; CFG/frame/register residue |
| `0x80034e10` | archive image transition and buffer restore | WIP, 99.114586%; two buffer-boundary register residues |

The `0x8002083c` reconstruction has the retail guards, rotation update,
three GTE matrix calls, model render call, and final primitive cursor update.
The player-state gate at `+0xcb` is an unsigned byte; its `lui+lbu` relocation
was decoded and reviewed at `0x8002083c/40`. With that referent present, the
object differs only by a 64-byte frame gap: retail reserves 224 bytes and
places the light, transformed-light, and color matrices at stack offsets
112/144/176, while the probe reserves 160 bytes and places them at
48/80/112. The source keeps four directly used `MATRIX` locals; no unused
array or padding was introduced to manufacture the frame. The contiguous
`0x80020748`–`0x80020b50` run now lives in one `menu_two_option.c` unit,
and both previously exact siblings remain 100%.

The map-mask radius source and retail CFG each have ten blocks and six
branches, but retail calculates the Z row and retains its row offset before
the X coordinate, then decrements the inner count in a branch delay slot.
The current object schedules the independent X expression first and advances
the cell pointer separately. The visibility test has matching five-block
control topology; changing its common failure path to early returns added a
block and regressed the probe, so the prior source was retained. The audio
owner retains `0x800321d8/32274`: the former needs a proved source mechanism
for the fixed arena's `lui/addiu` address, and the latter's slot induction
changes the saved-register set and 48-byte retail frame.

The TIM uploader, fade, and image transition form one contiguous
`0x8003494c`–`0x80034f90` source unit. This saves two linked modules while
preserving the uploader's exact listing. The fade initially lacked delinked
control targets because 24 direct `j/jal` rows remained candidate
`instruction-word` references. `review_mips26.py` re-decoded every opcode and
target in just `0x800349bc`–`0x80034e10`; all 24 passed and were marked
reviewed. Its objdiff score is now 92.34296%; the remaining CFG tail and
frame/register differences are unattributed. The image transition retains
its existing 99.114586% residue. No packet code or data ownership was
distorted to improve those WIP scores.

Focused `kf try` preserved the exact menu pair and TIM uploader after both
merges. The strict report also retains the five exact notification functions,
including `0x80033284`; this campaign has fourteen exact and thirteen WIP
verdicts. GAME-wide known-reference closure remains incomplete, so the
global command can exit nonzero after producing valid focused results.
