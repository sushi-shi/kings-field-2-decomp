# GAME vector and math focused screen

Ten contiguous GAME units were rebuilt individually with the manifest's pinned
GCC profile, including `-mcpu=r2000`, and compared with isolated strict
objdiff against their delinked target objects. Of 39 functions, 38 are direct
strict **100%**; the only WIP is `func_80015918` at **95.49419%**. These are
fresh controls of existing claims, not an increase in the project exact count.

| Unit | Final function verdicts |
| --- | --- |
| `game.matrix_rotation` | Exact: `angle_approach`, `value_approach`, `vector_direction_scaled`, `angle_velocity_step`, `angle_to_forward_xz`, `matrix_rotate_quarter_turns`, `svector_rotate_quarter_turns`, `matrix_set_rotation_x`, `matrix_set_rotation_y`, `matrix_set_rotation_z`, `matrix_set_rotation_yxz`, `matrix_set_rotation_xzy`. |
| `game.vector_math` | Exact: `pitch_yaw_to_forward_vector`, `vector_rotate_yxz`, `vector2i_scale_shift11`, `vector3s_scale_shift12`, `vector2i_scale_shift12`, `vector3s_scale_shift12_alt`, `vector3i_add_xz`, `angle_within_tolerance`, `angle_mod_delta_le_half_turn`, `angle_shortest_delta`, `vector_xz_to_angle`, `fixed_vector2_length`, `fixed_vector3_length`. |
| `game.vector_actor_helpers` | Exact: `func_800154fc`, `func_80015574`. |
| `game.vector_distance` | Exact: `vector_distance_to_point`. |
| `game.vector_distance_between` | Exact: `func_80015698`. |
| `game.actor_random_scalar` | Exact: `func_800157ac`, `func_800157f8`. |
| `game.actor_fixed_interpolation` | Exact: `func_8001584c`, `func_8001586c`, `func_800158b4`. |
| `game.actor_trajectory_math` | WIP: `func_80015918` 95.49419%; exact: `func_80015bc8`, `func_80015ce0`. |
| `game.world_translate` | Exact: `func_800160e8`. |
| `game.callback_default_noop` | Exact: `func_80015d50`. |

The trajectory solver retains its 41/41 CFG blocks, 24/24 branches, two
`SquareRoot0` calls and one `vector_xz_to_angle` call, with eight ordered text
relocations in both objects. The first divergent discriminant and selected-time
register lifetimes do not identify a missing field, call, branch, or referent.
All ten units have identical ordered text-relocation type and symbol
sequences. No source, identity, or profile change was retained.
