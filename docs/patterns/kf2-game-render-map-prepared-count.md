# GAME prepared-TMD renderer count guard

GAME `0x8002f808` handles an optional prepared asset, projects its vertices,
then dispatches FT3 and FT4 packets. Its packet count is a 32-bit field of
`KfTmdObject`. Retail loads the count, stores it to a stack slot, reloads it,
copies and decrements it, branches on the original zero value, and stores the
decremented value in the branch delay slot. On a nonzero count it enters the
packet loop; on zero it returns without processing a packet.

The earlier source expressed this as a separate `if (remaining != 0)` guard
followed by `remaining--`. GCC branched directly on the saved register before
the stack reload. Expressing the same live count with
`if (remaining-- != 0)` recovers the retail `sw`/`lw`/`nop`/`move`/`addiu`
`-1`/`beqz`/`sw` sequence at the first differing control. The zero-case
decrement wraps only a dead local; no packet is read or written. The existing
postdecrement loop still processes exactly the original count of packets.

Fresh focused `kf try --unit game.render_map` retains 51/51 CFG blocks,
35/35 branches, four return frontiers and matching known successor lists;
the listing for `0x8002f808` rises from 58.9% to 59.2%. Isolated strict
objdiff rises from 91.176970% to **92.038376%** for its 1876-byte body.
The exact `render_enqueue_map` sibling remains 100%, and the clipped-fan
`0x8002f5b0` remains 95.833336%. Retail and candidate each have 100
`.rel.text` rows across the unit in identical ordered relocation-type and
target-symbol pairs. The owned four-byte `.data` remains strict 100%.

The renderer is still WIP. Its first remaining focused difference is the
168-byte retail versus 120-byte candidate frame and corresponding stack
offsets. The retail extra 48 bytes have no proved source object. FT4 edge
checks run in the same semantic order in both objects despite different
register assignments; the clipped fan already spells its UV2 halfword store
before the first color word, but GCC schedules those stores oppositely.
Neither residue justifies padding, volatile accesses, or forced registers.
