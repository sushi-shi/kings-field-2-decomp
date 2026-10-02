# GAME camera and world-transform controls: 30 functions

The GAME camera path uses the shared rotation/vector helpers to form directions,
copies player view state, rotates event offsets into world space, and rebases the
camera with effect, map-object, and actor positions. These are six existing C
units, all rebuilt with focused `kf try` and compared directly with their
retail unit objects by strict objdiff. All 30 selected function bodies remain
**100% exact**; this refresh adds no new exact matches or source changes.

| GAME VA | Function | Unit | Strict verdict |
| --- | --- | --- | --- |
| `0x800147a0` | `angle_approach` | `matrix_rotation` | Exact, 100% |
| `0x80014864` | `value_approach` | `matrix_rotation` | Exact, 100% |
| `0x800148cc` | `vector_direction_scaled` | `matrix_rotation` | Exact, 100% |
| `0x80014a08` | `angle_velocity_step` | `matrix_rotation` | Exact, 100% |
| `0x80014ac0` | `angle_to_forward_xz` | `matrix_rotation` | Exact, 100% |
| `0x80014b0c` | `matrix_rotate_quarter_turns` | `matrix_rotation` | Exact, 100% |
| `0x80014d30` | `svector_rotate_quarter_turns` | `matrix_rotation` | Exact, 100% |
| `0x80014e14` | `matrix_set_rotation_x` | `matrix_rotation` | Exact, 100% |
| `0x80014e84` | `matrix_set_rotation_y` | `matrix_rotation` | Exact, 100% |
| `0x80014ef4` | `matrix_set_rotation_z` | `matrix_rotation` | Exact, 100% |
| `0x80014f64` | `matrix_set_rotation_yxz` | `matrix_rotation` | Exact, 100% |
| `0x80014fcc` | `matrix_set_rotation_xzy` | `matrix_rotation` | Exact, 100% |
| `0x80015034` | `pitch_yaw_to_forward_vector` | `vector_math` | Exact, 100% |
| `0x80015104` | `vector_rotate_yxz` | `vector_math` | Exact, 100% |
| `0x80015148` | `vector2i_scale_shift11` | `vector_math` | Exact, 100% |
| `0x80015188` | `vector3s_scale_shift12` | `vector_math` | Exact, 100% |
| `0x800151e4` | `vector2i_scale_shift12` | `vector_math` | Exact, 100% |
| `0x80015224` | `vector3s_scale_shift12_alt` | `vector_math` | Exact, 100% |
| `0x80015280` | `vector3i_add_xz` | `vector_math` | Exact, 100% |
| `0x800152ac` | `angle_within_tolerance` | `vector_math` | Exact, 100% |
| `0x800152e8` | `angle_mod_delta_le_half_turn` | `vector_math` | Exact, 100% |
| `0x800152f8` | `angle_shortest_delta` | `vector_math` | Exact, 100% |
| `0x80015318` | `vector_xz_to_angle` | `vector_math` | Exact, 100% |
| `0x80015468` | `fixed_vector2_length` | `vector_math` | Exact, 100% |
| `0x800154a8` | `fixed_vector3_length` | `vector_math` | Exact, 100% |
| `0x800160e8` | `func_800160e8` | `world_translate` | Exact, 100% |
| `0x80023484` | `player_reset_view` | `player_reset_view` | Exact, 100% |
| `0x80028224` | `func_80028224` | `player_camera_turn` | Exact, 100% |
| `0x8002851c` | `func_8002851c` | `player_camera_turn` | Exact, 100% |
| `0x80045f20` | `func_80045f20` | `event_pose_interpolate` | Exact, 100% |

The six complete units have strict exact `.text` spans of 2,196, 1,224, 376,
236, 1,880, and 384 bytes for matrix rotation, vector math, world translation,
view reset, camera turn, and event pose respectively. The last unit also
contains exact `0x80045fd4`, which is outside this 30-function count. Focused
listings are SAME for every member and for that event sibling. Retail xrefs
show three direct player-update calls to the related camera collision response
at `0x80027f78`; that WIP belongs to the separate collision/reaction campaign,
so this screen leaves it read-only. None of these controls motivates a source
or inventory edit.
