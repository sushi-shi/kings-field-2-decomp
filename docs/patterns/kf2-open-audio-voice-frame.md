# OPEN voice wrapper frame residue

`OPEN.EXE` `audio_play_voice` at `0x80013450` is the sole non-exact function in
the current 25-function OPEN source census. Retail has an 0x88-byte body. Its
two proven callers are in `opening_poll_pad`; its sole proven callee is the
vendored Psy-Q 3.0 `SsUtKeyOn`. It has no string or data referent. The adjacent
`tim_upload_images` in `open.resources` remains exact.

The six-argument `s16` signature agrees with the two callers and the retail
stack halfword loads. The source preserves the all-zero program/tone/note
gate and forwards zero fine pitch plus the two signed volumes. The focused
`kf try --unit open.resources --context 0 --no-flow` rebuild leaves the TIM
function SAME. The voice wrapper has the same instruction sequence and control
flow except five stack-frame immediates: retail reserves 64 bytes, saves `$ra`
at +56, loads the two incoming stack arguments at +80/+84, and restores a
64-byte frame; the current GCC 2.5.7 plain O2 probe reserves 40 bytes, saves
at +32, loads at +56/+60, and restores 40 bytes. The direct `jal` target and
argument stores remain unchanged. The current strict report is 99.82353%,
which is WIP, not a byte-exact match.

An off-tree GCC 2.6.0 O2 compilation of the same source also reserves 40 bytes
and changes more of the schedule, so that simple compiler-version swap does
not account for retail's 64-byte frame. No source-backed local or alternate
prototype explains the 24-byte extra reservation. The current source stays
unchanged; this is an unattributed frame-layout residue, not a reason to add
padding, dead locals, or an assembly wrapper.

Fresh image-qualified retail review confirms the two `opening_poll_pad` calls
at `0x80013028` and `0x80013074`: each passes signed `audio_vab_id`, program
10, tone zero, note 60, and two stack volume arguments of 64. The wrapper
loads those volumes with `lhu` from its incoming stack slots, sign-extends
them before `SsUtKeyOn`, and has no data or string referent. Its only outgoing
relocation is the direct `jal` at `0x800134c0` to `SsUtKeyOn`; the raw call is
proven, while the curated relocation row remains candidate. A fresh focused
`open.resources` comparison again gives `tim_upload_images` SAME and the
voice wrapper DIFF only at the five frame-dependent immediates (64 versus 40
bytes). The prior strict 99.82353% verdict remains WIP; no source or
relocation edit is supported by this pass.
