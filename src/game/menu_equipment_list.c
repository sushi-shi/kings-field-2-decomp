#include <kf/lib/address.h>
#include <kf/game/menu.h>
#include <psyq/libc.h>

typedef struct KfMenuEquipmentList {
    KfMenuList list;
    KfMenuLabelSuffix *initial_rows;
    KfMenuLabelSuffix *current_rows;
    u8 unknown_2c[8];
} KfMenuEquipmentList;

typedef char kf_menu_equipment_list_size[sizeof(KfMenuEquipmentList) == 52 ? 1 : -1];

extern KfMenuLabelSuffix menu_equipment_labels_64910[10];
extern void func_80019ce4(KfMenuLabelSuffix *rows);
extern void func_80019ed4(s32 category);
extern void func_8001a2f4(void);
extern void func_8001a4f0(void);

ADDRESS(0x80019ac4, 0x220)
void func_80019ac4(void)
{
    KfMenuEquipmentList menu;
    KfMenuLabelSuffix initial_rows[10];
    KfMenuLabelSuffix current_rows[10];
    s32 mode = 0;
    s32 result = -99;
    s32 frame;
    u32 choice;

    memcpy(initial_rows, menu_equipment_labels_64910, sizeof initial_rows);
    func_80019ce4(current_rows);
    menu_list_init(&menu.list, 0, 2);
    menu.list.entry_count = 10;
    menu.list.visible_rows = 10;
    menu.initial_rows = initial_rows;
    menu.current_rows = current_rows;
    menu.list.list_y = 39;

    for (;;) {
        if (mode != 0 || result != -99)
            input_wait_release();

        if (mode == 1) {
            choice = menu.list.selected_index;
            if (choice == 1)
                func_8001a2f4();
            else if (choice == 9)
                func_8001a4f0();
            else
                func_80019ed4(choice);
            func_80019ce4(current_rows);
        }

        if (result != -99)
            break;

        func_8001e484(&menu.list, 0, &mode, &result);
        if (mode == 1)
            func_80022300(17);
        for (frame = 0; frame < 2; frame++) {
            menu_frame_begin();
            func_8001fc94(&menu, 3);
            menu_present_frame();
        }
    }
}

DATA(0x80064910, 0xc8)
KfMenuLabelSuffix menu_equipment_labels_64910[10] = {
    {{118, 119, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{120, 121, -1, 0, 0, 0, 0, 0, 0, 0}},
    {{122, 123, 57, 124, -1, 0, 0, 0, 0, 0}},
    {{122, 123, 57, 125, -1, 0, 0, 0, 0, 0}},
    {{122, 123, 57, 126, -1, 0, 0, 0, 0, 0}},
    {{122, 123, 57, 127, -1, 0, 0, 0, 0, 0}},
    {{122, 123, 57, 128, -1, 0, 0, 0, 0, 0}},
    {{0, 1, 18, 32, 230, -1, 0, 0, 0, 0}},
    {{0, 1, 18, 32, 231, -1, 0, 0, 0, 0}},
    {{58, 4125, 15, 39, -1, 0, 0, 0, 0, 0}},
};
