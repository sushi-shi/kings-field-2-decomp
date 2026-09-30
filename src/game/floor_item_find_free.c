#include <kf/lib/address.h>
#include <kf/game/graphics.h>
#include <kf/game/memory.h>

ADDRESS(0x8002ce2c, 0x3c)
KfFloorItem *func_8002ce2c(void)
{
    KfFloorItem *item = game_graphics_runtime.floor_items;
    s32 remaining = KF_FLOOR_ITEM_CAPACITY;

    do {
        if (item->kind == 0xff) {
            return item;
        }
        item++;
    } while (--remaining != 0);

    return 0;
}

ADDRESS(0x8002ce68, 0xd8)
void func_8002ce68(s32 x, s32 y, u8 value_01, u8 value_03,
                   s32 kind, s32 width_bytes, u16 height)
{
    KfFloorItem *item = func_8002ce2c();

    if (item != 0) {
        item->unknown_04 = 0;
        item->unknown_01 = value_01;
        item->unknown_03 = value_03;
        item->unknown_02 = 0;
        item->rect.x = x;
        item->rect.y = y;
        item->kind = kind;
        if (kind == 1) {
            item->rect.w = width_bytes >> 2;
            item->rect.h = height;
            item->pixels = (u_long *)memory_allocate(
                (s16)item->rect.w * (s16)height * 2);
        }
        StoreImage(&item->rect, item->pixels);
        DrawSync(0);
    }
}

ADDRESS(0x8002cf40, 0x164)
void func_8002cf40(void)
{
    KfFloorItem *item = game_graphics_runtime.floor_items;
    s32 remaining = KF_FLOOR_ITEM_CAPACITY - 1;
    RECT rect;

    do {
        if (item->unknown_02 == 0) {
            item->unknown_02 = item->unknown_01;
            if (item->kind == 1) {
                item->unknown_04 += item->unknown_03;
                if ((s16)item->unknown_04 >= item->rect.h) {
                    item->unknown_04 -= item->rect.h;
                }
                setRECT(&rect, item->rect.x, item->rect.y + item->unknown_04,
                        item->rect.w, item->rect.h - item->unknown_04);
                LoadImage(&rect, item->pixels);
                if ((s16)item->unknown_04 != 0) {
                    u_long *pixels = (((s16)rect.w * (s16)rect.h) >> 1) + item->pixels;
                    rect.y = item->rect.y;
                    rect.h = item->unknown_04;
                    LoadImage(&rect, pixels);
                }
            }
        } else {
            item->unknown_02--;
        }
        item++;
    } while (--remaining != -1);
}
