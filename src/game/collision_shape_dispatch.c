#include <kf/lib/address.h>
#include <kf/game/collision_cache.h>

/* Interior cache words and runtime shape bank remain WIP ownership views. */
#define COLLISION_CACHE_HEIGHT_LIMIT (*(s32 *)((u8 *)&bss_801c7540 + 0x11814))
#define COLLISION_CACHE_UPPER_BOUND (*(s32 *)((u8 *)&bss_801c7540 + 0x1181c))

RODATA(0x8001134c, 0xc4)

ADDRESS(0x8002aaa4, 0xb60)
s32 func_8002aaa4(s32 x, s32 y, s32 z, s32 radius, s32 height)

{
  s32 visited_second_layer;
  s32 special_floor_found;
  u32 height_flags;
  int saved_height_limit;
  u16 record_value;
  int quotient;
  int candidate_height;
  u32 case_value;
  u16 *record;
  u16 *operand;
  u16 *next_record;
  u32 result_flags;
  u32 z_fraction;
  u32 x_fraction;
  int radius_complement;
  int bottom_y;
  int records_left;
  u32 x_remaining;
  u32 z_remaining;
  s32 x_minus_z;
  s32 z_minus_x;
  s32 x_plus_z;
  s32 neg_x_minus_z;
  s32 x_plus_cell;
  s32 z_plus_cell;
  u8 *selected_layer;
  u8 *shape_bank = (u8 *)&bss_801c7540 + 0x10000;

  result_flags = 0;
  special_floor_found = 0;
  visited_second_layer = 0;
  KF_COLLISION_CACHE_RESULT = 100000;
  KF_COLLISION_CACHE_LOWER_BOUND = 100000;
  COLLISION_CACHE_UPPER_BOUND = 100000;
  COLLISION_CACHE_HEIGHT_LIMIT = KF_COLLISION_CACHE_HEIGHT + -40000;
  KF_COLLISION_CACHE_SHAPE = (u8 *)KF_COLLISION_CACHE_CELL + KF_COLLISION_CACHE_LAYER;
  selected_layer = KF_COLLISION_CACHE_SHAPE;
  height_flags = (u32)height & 0xf0000000;
  height &= 0x0fffffff;
LAB_8002ab5c:
  record_value = *(u16 *)(shape_bank + (u32)selected_layer[3] * 2);
  bottom_y = y - height;
  records_left = *(s16 *)((shape_bank + 2) + record_value) + -1;
  radius = radius * *(s16 *)(shape_bank + record_value) >> 0xc;
  if (records_left == -1) {
    return result_flags;
  }
  radius_complement = 0x800 - radius;
  x_fraction = x & 0x7ff;
  x_remaining = 0x800 - x_fraction;
  z_fraction = z & 0x7ff;
  z_remaining = 0x800 - z_fraction;
  x_minus_z = (s32)x_fraction - (s32)z_fraction;
  z_minus_x = (s32)z_fraction - (s32)x_fraction;
  x_plus_z = (s32)x_fraction + (s32)z_fraction;
  neg_x_minus_z = -(s32)x_fraction - (s32)z_fraction;
  x_plus_cell = (s32)x_fraction + 0x800;
  z_plus_cell = (s32)z_fraction + 0x800;
  record = (u16 *)((shape_bank + 4) + record_value);
  do {
    operand = record + 1;
    next_record = operand;
    saved_height_limit = COLLISION_CACHE_HEIGHT_LIMIT;
    switch ((s16)*record) {
    case 0x10:
      next_record = record + 2;
      if (!special_floor_found) {
        candidate_height = KF_COLLISION_CACHE_RESULT;
        if ((s16)*operand + KF_COLLISION_CACHE_HEIGHT < KF_COLLISION_CACHE_RESULT) {
          candidate_height = (s16)*operand + KF_COLLISION_CACHE_HEIGHT;
        }
LAB_8002b3b8:
        KF_COLLISION_CACHE_RESULT = candidate_height;
        if (KF_COLLISION_CACHE_RESULT < y) {
          result_flags = result_flags | 4;
        }
      }
      break;
    case 0x11:
      next_record = record + 3;
      COLLISION_CACHE_HEIGHT_LIMIT = (s16)*operand + KF_COLLISION_CACHE_HEIGHT;
      if (bottom_y < COLLISION_CACHE_HEIGHT_LIMIT) {
        saved_height_limit = (s16)record[2] + KF_COLLISION_CACHE_HEIGHT;
        if (saved_height_limit < bottom_y) {
          result_flags = result_flags | 8;
        }
        else {
          if (saved_height_limit < KF_COLLISION_CACHE_RESULT) {
            KF_COLLISION_CACHE_RESULT = saved_height_limit;
          }
          COLLISION_CACHE_HEIGHT_LIMIT = -100000;
        }
      }
      goto LAB_8002b5c8;
    case 0x18:
      KF_COLLISION_CACHE_LOWER_BOUND = (s16)*operand + KF_COLLISION_CACHE_HEIGHT;
      next_record = record + 2;
      if (((s32)height_flags < 0) && (KF_COLLISION_CACHE_RESULT = KF_COLLISION_CACHE_LOWER_BOUND, KF_COLLISION_CACHE_LOWER_BOUND < y)) {
        result_flags = result_flags | 4;
        special_floor_found = 1;
      }
      if (((height_flags & 0x40000000) != 0) && (saved_height_limit = KF_COLLISION_CACHE_LOWER_BOUND, bottom_y <= KF_COLLISION_CACHE_LOWER_BOUND)) {
        result_flags = result_flags | 8;
      }
      break;
    case 0x19:
      next_record = record + 2;
      COLLISION_CACHE_UPPER_BOUND = (s16)*operand + KF_COLLISION_CACHE_HEIGHT;
      break;
    case 0x20:
      next_record = record + 5;
      record_value = (u16)selected_layer[2] + record[4] & 3;
      case_value = 5;
      if (record_value == 1) {
        candidate_height = radius_complement - (s16)*operand;
LAB_8002af70:
        next_record = record + 5;
        case_value = 5;
        if (candidate_height <= (int)z_fraction) goto LAB_8002ada4;
      }
      else if (record_value < 2) {
        if (record_value == 0) {
          if ((int)x_fraction <= (s16)*operand + radius) goto LAB_8002ada4;
          break;
        }
      }
      else if (record_value == 2) {
        if (radius_complement - (s16)*operand <= (int)x_fraction) goto LAB_8002ada4;
      }
      else if (record_value == 3) {
        candidate_height = (s16)*operand + radius;
LAB_8002afc4:
        next_record = record + 5;
        case_value = 5;
        if ((int)z_fraction <= candidate_height) goto LAB_8002ada4;
      }
      goto LAB_8002b5c8;
    case 0x21:
      next_record = record + 5;
      record_value = (u16)selected_layer[2] + record[4] & 3;
      case_value = 5;
      if (record_value == 1) {
        candidate_height = radius_complement - (s16)*operand;
        if ((int)z_fraction < candidate_height) {
LAB_8002af9c:
          next_record = record + 5;
          case_value = 5;
          if ((int)x_fraction < candidate_height) goto LAB_8002b5c8;
        }
      }
      else if (record_value < 2) {
        if (record_value != 0) goto LAB_8002b5c8;
        candidate_height = radius_complement - (s16)*operand;
        if ((s16)*operand + radius < (int)x_fraction) goto LAB_8002af70;
      }
      else if (record_value == 2) {
        candidate_height = (s16)*operand + radius;
        if ((int)x_fraction < radius_complement - (s16)*operand) goto LAB_8002afc4;
      }
      else {
        if (record_value != 3) goto LAB_8002b5c8;
        candidate_height = (s16)*operand + radius;
        if (candidate_height < (int)z_fraction) {
LAB_8002aff0:
          next_record = record + 5;
          case_value = 5;
          if (candidate_height < (int)x_fraction) goto LAB_8002b5c8;
        }
      }
LAB_8002ada4:
      next_record = record + 5;
      candidate_height = (s16)record[3] + KF_COLLISION_CACHE_HEIGHT;
      saved_height_limit = (s16)record[2] + KF_COLLISION_CACHE_HEIGHT;
      if (((bottom_y < (s16)record[2] + KF_COLLISION_CACHE_HEIGHT) &&
          (saved_height_limit = COLLISION_CACHE_HEIGHT_LIMIT, candidate_height < KF_COLLISION_CACHE_RESULT)) && (KF_COLLISION_CACHE_RESULT = candidate_height, candidate_height < y))
      {
        result_flags = result_flags | case_value;
      }
      break;
    case 0x22:
      next_record = record + 5;
      record_value = (u16)selected_layer[2] + record[4] & 3;
      if (record_value == 1) {
        candidate_height = radius_complement - (s16)*operand;
        if (candidate_height <= (int)z_fraction) goto LAB_8002af9c;
      }
      else if (record_value < 2) {
        if (record_value != 0) goto LAB_8002b5c8;
        candidate_height = radius_complement - (s16)*operand;
        if ((int)x_fraction <= (s16)*operand + radius) goto LAB_8002af70;
      }
      else if (record_value == 2) {
        candidate_height = (s16)*operand + radius;
        if (radius_complement - (s16)*operand <= (int)x_fraction) goto LAB_8002afc4;
      }
      else {
        if (record_value != 3) goto LAB_8002b5c8;
        candidate_height = (s16)*operand + radius;
        if ((int)z_fraction <= candidate_height) goto LAB_8002aff0;
      }
      break;
    case 0x23:
      next_record = record + 5;
      record_value = (u16)selected_layer[2] + record[4] & 3;
      case_value = 6;
      if (record_value == 1) {
        candidate_height = (int)(s16)*operand + radius + -0x1000;
        saved_height_limit = neg_x_minus_z;
      }
      else if (record_value < 2) {
        if (record_value != 0) goto LAB_8002b5c8;
        candidate_height = (int)(s16)*operand + radius + -0x800;
        saved_height_limit = x_minus_z;
      }
      else if (record_value == 2) {
        candidate_height = (int)(s16)*operand + radius + -0x800;
        saved_height_limit = z_minus_x;
      }
      else {
        if (record_value != 3) goto LAB_8002b5c8;
        candidate_height = (s16)*operand + radius;
        saved_height_limit = x_plus_z;
      }
      if (saved_height_limit <= candidate_height) goto LAB_8002ada4;
      goto LAB_8002b5c8;
    case 0x30:
      record_value = (u16)selected_layer[2] + record[4] & 3;
      next_record = record + 7;
      if (record_value == 1) {
        if (((int)z_fraction <= (radius + 0x800) - (int)(s16)record[2]) &&
           (radius_complement - (s16)record[3] <= (int)z_fraction)) {
          candidate_height = (int)(s16)record[6];
          quotient = (int)x_fraction / candidate_height;
          goto LAB_8002b168;
        }
      }
      else if (record_value < 2) {
        if (record_value != 0) goto LAB_8002b5c8;
        if (((s16)record[2] - radius <= (int)x_fraction) &&
           ((int)x_fraction <= (s16)record[3] + radius)) {
          candidate_height = (int)(s16)record[6];
          quotient = (int)z_fraction / candidate_height;
          goto LAB_8002b168;
        }
      }
      else if (record_value == 2) {
        if (((int)x_fraction <= (radius + 0x800) - (int)(s16)record[2]) &&
           (radius_complement - (s16)record[3] <= (int)x_fraction)) {
          candidate_height = (int)(s16)record[6];
          quotient = (int)z_remaining / candidate_height;
LAB_8002b168:
          candidate_height = (quotient + 1) * (int)(s16)record[5];
          goto LAB_8002b38c;
        }
      }
      else {
        if (record_value != 3) goto LAB_8002b5c8;
        if (((s16)record[2] - radius <= (int)z_fraction) &&
           ((int)z_fraction <= (s16)record[3] + radius)) {
          candidate_height = (int)(s16)record[6];
          quotient = (int)x_remaining / candidate_height;
          goto LAB_8002b168;
        }
      }
      break;
    case 0x31:
      if ((result_flags & 1) != 0) {
        next_record = record + 6;
        record_value = (u16)selected_layer[2] + record[5] & 3;
        case_value = x_fraction;
        if (record_value != 1) {
          if (record_value < 2) {
            case_value = z_fraction;
            if (record_value == 0) goto LAB_8002b450;
          }
          else {
            case_value = z_remaining;
            if ((record_value == 2) || (case_value = x_remaining, record_value == 3)) goto LAB_8002b450;
          }
          goto LAB_8002b5c8;
        }
LAB_8002b450:
        if (((s16)*operand + radius <= (int)(s16)case_value) &&
           ((int)(s16)case_value <= (s16)record[2] - radius)) {
          KF_COLLISION_CACHE_RESULT = (s16)record[3] + KF_COLLISION_CACHE_HEIGHT;
          saved_height_limit = (s16)record[4] + KF_COLLISION_CACHE_HEIGHT;
          if (bottom_y < saved_height_limit) {
            result_flags = result_flags | 8;
          }
          if (y <= KF_COLLISION_CACHE_RESULT) {
            result_flags = result_flags & 0xfffffffa;
          }
        }
      }
      break;
    case 0x32:
      record_value = (u16)selected_layer[2] + record[4] & 3;
      next_record = record + 7;
      if (record_value == 1) {
        candidate_height = x_plus_z;
      }
      else if (record_value < 2) {
        if (record_value != 0) goto LAB_8002b5c8;
        candidate_height = z_plus_cell - (s32)x_fraction;
      }
      else if (record_value == 2) {
        candidate_height = x_plus_cell - (s32)z_fraction;
      }
      else {
        if (record_value != 3) goto LAB_8002b5c8;
        candidate_height = (0x1000 - z_fraction) - x_fraction;
      }
      quotient = (int)(s16)record[2];
      if ((candidate_height < quotient) || (quotient = (int)(s16)record[3], quotient < candidate_height)) {
        candidate_height = quotient;
      }
      quotient = (int)(s16)record[6];
      candidate_height = ((candidate_height - (s16)record[2]) / quotient) * (int)(s16)record[5];
LAB_8002b38c:
      next_record = record + 7;
      candidate_height = ((s16)*operand + KF_COLLISION_CACHE_HEIGHT) - candidate_height;
      if (candidate_height < KF_COLLISION_CACHE_RESULT) goto LAB_8002b3b8;
      break;
    case 0x40:
      goto scan_second_layer;
    }
    COLLISION_CACHE_HEIGHT_LIMIT = saved_height_limit;
LAB_8002b5c8:
    records_left = records_left + -1;
    record = next_record;
    if (records_left == -1) {
      return result_flags;
    }
  } while( 1 );
scan_second_layer:
  if (visited_second_layer) {
    return result_flags;
  }
  KF_COLLISION_CACHE_LAYER = -(u16)(KF_COLLISION_CACHE_LAYER == 0) & 5;
  selected_layer = (u8 *)KF_COLLISION_CACHE_CELL + KF_COLLISION_CACHE_LAYER;
  KF_COLLISION_CACHE_HEIGHT = (u32)selected_layer[1] * -0x80;
  visited_second_layer = 1;
  goto LAB_8002ab5c;
}
