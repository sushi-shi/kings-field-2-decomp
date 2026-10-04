# Handoff: where the decomp stands and how to continue

State as of `42bb8f6` (2026-09-30). The authoritative live numbers are the
README match-status block (`kf match` / `kf bank` refresh it).

## Current state

| Image | Exact | Notes |
| :---- | ----: | :---- |
| `PSX.EXE` | 1 / 1 | done |
| `END.EXE` | 16 / 16 | done; see `docs/patterns/kf2-end-template-residue.md` |
| `OPEN.EXE` | 24 / 25 | `audio_play_voice`: 24 unexplained frame bytes (same in US/EU, same under GCC 2.4.1/2.5.7/2.6.0) |
| `GAME.EXE` | 97 / 501 | 100 started; campaign below |

All exact results are banked in `config/match_baseline.tsv`.

### GAME work in progress (committed, not exact)

- `angle_velocity_step` (`src/game/matrix_rotation.c`, 92.9%): register
  allocation plus a dead `subu v0,s2,s1` delay slot; no source shape found.
- `vector_distance_to_point` (`src/game/vector_distance.c`, 88%): retail keeps
  the two height-test fail exits apart (`beqz ok; j fail` in the then-arm,
  `bnez fail` in the else-arm). Our build cross-jumps them into one. A `goto
  in_range` form reached 90.5% but left a register difference in the
  `SquareRoot0` tail. The plain form is kept.
- `cd_archive_open` (`src/game/cd_archive.c`, 99.75%): 64 unexplained frame
  bytes.

### Modelled GAME objects

The extents come from `game_main_loop`'s startup clears (`repeat_store_word`).

- `game_graphics_runtime` 0x8017d140 (0x17cf0): `KfGraphicsRuntimeGame` in
  `include/kf/game/graphics.h`.
- `player_state` 0x801984d0 (0x160): `KfPlayerState` in
  `include/kf/game/player.h`.
- `player_level_growth_table` 0x800758f0: 100 rows of 12 bytes.
- `cd_state` 0x801b5d60 (0x2a4): `KfCdState` in `include/kf/game/cd.h`.
- Not yet modelled:
  - audio state 0x80197630 (0xe9c);
  - 0x8016b600 (0x93cc);
  - 0x801749d0 (0x8744);
  - 0x8019b6a8 (0x2a8c);
  - 0x801b2140 (0x3918);
  - 0x801c7540 (0x11844);
  - 0x8017d118 (0x1c).

GAME's initialized data ends at 0x8006d9e4. Everything above the layout's
`load_end` (0x8006e800) is `.bss`: declare it `extern` and curate an identity
for it, but never define it (`-fcommon`).

### Codecs

`.T` sector archives are implemented three ways:

- the Rust `tools/kf-codec`;
- the reconstructed GAME C (`src/game/cd_*.c`);
- the retail consumer, run by `scripts/kf/sector_archive_oracle.py` on
  `parser_machine` (Unicorn).

The last full oracle run was interrupted by host memory pressure, not by a
failure. Re-run it before relying on it.

## Remaining KF1-similar GAME functions (paused campaign)

These come from `scripts/survey/kf1_similarity.py GAME.EXE`. Its output goes to
`build/survey/kf1_similarity_game.tsv`; set `KF1_REPO` and `KF1_RETAIL_DIR`
before running it. Every function below has similarity ≥ 0.70 and is not yet
claimed. The KF1 name is a template, not an identity: check the KF2 body.

| KF2 VA | Size | KF1 counterpart | Ratio |
| :----- | ---: | :-------------- | ----: |
| 0x80016ee0 | 0x30 | (wrapper shape; CD stream) | 0.80 |
| 0x80020748 | 0xf4 | menu_draw_two_option | 0.95 |
| 0x80020b50 | 0x1d0 | menu_blit_sprite_translucent | 0.94 |
| 0x80021be0 | 0xac | display_present_frame | 0.84 |
| 0x80021f10 | 0x50 | primitive_buffer_begin_poly_ft4 | 0.95 |
| 0x80021f60 | 0x50 | primitive_buffer_commit_poly_ft4 | 1.00 |
| 0x800222bc | 0x44 | menu_release_item_model | 0.88 |
| 0x8002545c | 0x240 | player_set_equipment_slot | 0.80 |
| 0x8002f194 | 0x41c | render_enqueue_map | 0.90 |
| 0x800331d0 | 0xa4 | notify_enqueue (identity already set; varargs, id 0x15 payload) | 0.79 |
| 0x8003494c | 0x70 | tim_upload_images | 0.93 |
| 0x80035504 | 0x30 | (object spatial-sound wrapper) | 0.70 |
| 0x800363bc | 0x20 | map_object_start_action_if_idle | 1.00 |
| 0x80038cc8 | 0x3c | actor_pool_find_free | 0.93 |
| 0x80038efc | 0x24 | (wrapper shape) | 0.82 |
| 0x80039080 | 0x50 | actor_pool_clear | 0.83 |
| 0x800397a8 | 0x30 | (wrapper shape) | 0.70 |
| 0x8003ad90 | 0x34 | actor_advance_animation_wrapped | 1.00 |
| 0x8003adc4 | 0x5c | actor_advance_animation_clamped | 1.00 |
| 0x8003ae20 | 0x30 | actor_animation_crossed_phase | 1.00 |
| 0x8003fa2c | 0x3c | audio_play_spatial_default_range shape | 0.74 |
| 0x8003fdac | 0x24 | effect_magic_power | 1.00 |
| 0x80040220 | 0x44 | effect_pool_find_free | 0.94 |
| 0x80045cc0 | 0x30 | effect_pool_reset | 0.92 |
| 0x80045e18 | 0x24 | `audio_play_sound(0x40, 100)` wrapper | 0.82 |
| 0x80045e3c | 0x20 | `audio_play_sound(a0, 100)` wrapper | 0.88 |
| 0x800483a8 | 0x30 | indirect call through 0x8017d124 | 0.70 |
| 0x80058298 | 0x10 | tmd_set_current_vertices shape | 0.75 |

Some easy neighbours of finished units are also still open:

- the three byte setters at 0x800253fc, 0x8002540c and 0x80025434, which write
  `player_state` +0x97/+0x98/+0x99;
- `player_recalculate_combat_stats` at 0x80023984 (0x6b0);
- `player_death_begin` at 0x80029570 (0x88).

## How to continue (per function family)

Work inside `nix develop`, with retail files initialized by `kf init`.

1. **Read retail.** `python scripts/survey/disasm_range.py GAME.EXE START END`.
   Decode globals as `lui 0x801a` plus a signed low half. Read the KF1
   counterpart's source in the KF1 checkout.
2. **Model the data first.** Add or extend a header under `include/kf/game/`
   with a size check (`typedef char name[sizeof(T) == N ? 1 : -1];`). Leave
   unproven spans as `unknown_XX`. Don't invent names.
3. **Write the source.** Use a small unit `src/game/<family>.c` with
   `ADDRESS(va, size)` claims in ascending order. The unit must own a
   contiguous run.
4. **Admit identities.** Write a JSON spec for
   `scripts/survey/admit_functions.py SPEC.json`; it covers names and
   signatures, and callees need names too.
5. **Curate relocations.**
   - Data: `scripts/survey/curate_refs.py SPEC.json` with `range` set to **only
     the reconstructed functions' own ranges**. A wide range marks hundreds of
     unrelated pairs as reviewed.
   - Calls: `scripts/survey/review_mips26.py GAME.EXE START END OWNER` for each
     range.
6. **Enroll the unit** in `config/units.toml` in address order, with
   `profile = "probe-gcc257-o2-g0"`. GAME uses that profile; OPEN and END use
   `probe-gcc257-o2-plain`.
7. **Add structure rows.** `python scripts/survey/structure_rows.py "EVIDENCE"`
   adds rows for new header structs. Run `kf configure` after adding or moving
   sources.
8. **Match.** Run `kf match --image game`, then read the non-exact functions
   from `build/objdiff/report.json`.
9. **Diff at the instruction level.** Use `kf try --unit game.<unit>`, or
   objdump both `build/delink/game/modules/<va>_<unit>.o` and
   `build/objdiff/game/base/<va>_<unit>.o`. Triage in this order:
   referent/relocation, call set, CFG, then types.
10. **Bank.** Stage the campaign, then run `kf bank`; it refuses dirty
    unstaged inputs.
11. **Update test counts.** When curated counts change, bump them in
    `tests/test_inventory.py` (functions, data, structures).
12. **Commit.** Use `match: ...` with the co-author line.

Expected noise:

- `kf match` always ends with "known-reference data ownership is incomplete".
  The SDK data is unowned, so judge by the exact count and "target relink N/N
  units verified".
- The cleanliness ratchets must stay at their floors. For example, a new
  `func_XXXXXXXX` reference in source fails the ratchet, so name the callee.
- The PSX data divergence (`DAT_80010024` against `.rodata`) predates this
  work.
- OPEN/END `kf build` link failures are expected WIP.
- Run pytest and ruff only when `scripts/`, `tests/` or the flake changed.

## Publishing rules (public repo)

- Never commit retail EXEs or disc files, `build/`, SDK media, or local
  absolute paths.
- Before every push, audit the staged diff: no binary numstat rows and no
  home-directory paths. The survey scripts take KF1 locations only from
  environment variables.
