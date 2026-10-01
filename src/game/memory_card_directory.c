#include <kf/lib/address.h>
#include <kf/game/card.h>
#include <psyq/kernel.h>
#include <psyq/libc.h>
#include <CONVERT.H>
#include <psyq/sdk.h>
#include <sys/fcntl.h>

RODATA(0x80011120, 0x6)

DATA(0x80066670, 0x10)
char memory_card_file_prefix[16] = "BISLPS-00069";

typedef struct KfCardAssets {
    char title[0x40];
    u16 icon_palette[7][16];
} KfCardAssets;
typedef char kf_card_assets_size[sizeof(KfCardAssets) == 0x120 ? 1 : -1];

DATA(0x80066680, 0x120)
KfCardAssets DAT_80066680 = {
    "\202j\202h\202m\202f\201f\202r\201@\202e"
    "\202h\202d\202k\202c\201@\202Q\201|\201@"
    "\201@\202d\202w\202o\201@\201@\201@\201@"
    "\201@\201@\201@\202k\202u\201@\201@",
    {
        {0x0000, 0x2942, 0x2d63, 0x35a4, 0x39c5, 0x4206, 0x4a47, 0x4e68,
         0x56a9, 0x5eea, 0x1ce0, 0x0012, 0x0015, 0x0857, 0x211a, 0x4400},
        {0x0000, 0x431c, 0x4b5d, 0x579f, 0x4f2f, 0x5771, 0x63f4, 0x7ab5,
         0x7b18, 0x7b7b, 0x7355, 0x7b98, 0x7bbb, 0x3af7, 0x435a, 0x5968},
        {0x0000, 0x0dc3, 0x1a05, 0x2648, 0x328a, 0x3ecd, 0x4b10, 0x0018,
         0x14bb, 0x295f, 0x00c0, 0x7fda, 0x7fdc, 0x3f18, 0x4b7b, 0x57de},
        {0x0000, 0x00b2, 0x0d15, 0x1d99, 0x2e1d, 0x3ed8, 0x4b3b, 0x1665,
         0x1ee7, 0x2769, 0x7def, 0x7ab3, 0x7314, 0x0a12, 0x390a, 0x4400},
        {0x0000, 0x0142, 0x01a4, 0x0206, 0x0268, 0x02ea, 0x0011, 0x0015,
         0x0019, 0x001e, 0x00e0, 0x000b, 0x7314, 0x0a12, 0x390a, 0x4400},
        {0x0000, 0x42d7, 0x4b19, 0x575c, 0x5b7d, 0x198d, 0x1108, 0x0884,
         0x0000, 0x7bde, 0x1dae, 0x154b, 0x10e8, 0x08a5, 0x0442, 0x0000},
        {0x0000, 0x1675, 0x1a96, 0x22d8, 0x2b1a, 0x335c, 0x3b9e, 0x6000,
         0x6ca5, 0x7d4a, 0x7def, 0x7fda, 0x012b, 0x0a12, 0x390a, 0x4400}
    }
};

extern s8 DAT_8006d6a4;
extern s8 DAT_8006d6a5;
extern char DAT_8006d6a8[7];
extern void func_800492dc(const u8 *payload);
extern void func_80048d24(u8 *payload);

ADDRESS(0x800226ec, 0x1dc)
s32 func_800226ec(struct DIRENTRY *entries, s32 *matching_count)
{
    struct DIRENTRY ordered[15];
    struct DIRENTRY *entry;
    char slot_digit[2];
    s32 total_size = 0;
    s32 i;
    s32 slot;

    slot_digit[0] = DAT_8006d6a4;
    slot_digit[1] = DAT_8006d6a5;
    entry = entries;
    memset(entries, 0, sizeof(ordered));
    *matching_count = 0;
    if (firstfile(DAT_8006d6a8, entry) == entry) {
        do {
            total_size += entry->size;
            if (strncmp(entry->name, memory_card_file_prefix, 12) == 0) {
                ++*matching_count;
            }
            ++entry;
        } while (nextfile(entry) == entry);
    }

    entry = entries;
    memset(ordered, 0, sizeof(ordered));
    for (i = 0; i < 15; ++i) {
        if (strncmp(entry->name, memory_card_file_prefix, 12) == 0) {
            slot_digit[0] = entry->name[12];
            slot = atoi(slot_digit) - 1;
            ordered[slot] = *entry;
        }
        ++entry;
    }
    memcpy(entries, ordered, sizeof(ordered));
    return total_size > 0x1a000;
}

ADDRESS(0x800228c8, 0x280)
s32 func_800228c8(const char *filename, s32 *experience, s32 *level,
    s32 *slot)
{
    KfCardHeader header;
    char path[40] = "bu00:";
    char slot_digit[2];
    s16 encoded;
    s32 handle;
    s32 weight;
    s32 i;

    slot_digit[0] = DAT_8006d6a4;
    slot_digit[1] = DAT_8006d6a5;
    if (strncmp(filename, memory_card_file_prefix, 12) != 0) {
        return 1;
    }
    strcat(path, filename);
    handle = open(path, FREAD);
    if (handle == -1 || read(handle, &header, sizeof(header)) != sizeof(header)) {
        return 1;
    }
    close(handle);

    *experience = 0;
    weight = 100000;
    for (i = 0; i < 6; ++i) {
        ((u8 *)&encoded)[0] = header.title[0x28 + i * 2];
        ((u8 *)&encoded)[1] = header.title[0x29 + i * 2];
        if (encoded != 0x4081) {
            encoded = ((s32)encoded >> 8) - 79;
            *experience += encoded * weight;
        }
        weight /= 10;
    }

    *level = 0;
    weight = 10;
    for (i = 0; i < 2; ++i) {
        ((u8 *)&encoded)[0] = header.title[0x3a + i * 2];
        ((u8 *)&encoded)[1] = header.title[0x3b + i * 2];
        if (encoded != 0x4081) {
            encoded = ((s32)encoded >> 8) - 79;
            *level += encoded * weight;
        }
        weight /= 10;
    }

    slot_digit[0] = filename[12];
    *slot = atoi(slot_digit);
    return 0;
}

ADDRESS(0x80022b48, 0x2c)
s32 memory_card_format(void)
{
    return format("bu00:") != 1;
}

ADDRESS(0x80022b74, 0x12c)
s32 func_80022b74(s32 slot)
{
    char path[40] = "bu00:";
    s32 attempt;
    s32 handle;
    s32 status;
    u8 *buffer;
    u32 checksum;

    attempt = 0;
    do {
        strcat(path, memory_card_file_prefix);
        path[17] = slot + '0';
        path[18] = 0;
        handle = open(path, FREAD);
        if (handle == -1 || read(handle, memory_card_buffer, KF_CARD_BLOCK_BYTES)
                != KF_CARD_BLOCK_BYTES) {
            status = 1;
        } else {
            close(handle);
            buffer = memory_card_buffer;
            checksum = memory_card_payload_byte_sum(buffer + KF_CARD_HEADER_BYTES);
            if (((KfCardHeader *)buffer)->payload_checksum == checksum) {
                func_800492dc(memory_card_buffer + KF_CARD_HEADER_BYTES);
                memory_card_loaded_slot = slot;
                return 0;
            }
            status = 2;
        }
    } while (attempt++ < 2);
    return status;
}

ADDRESS(0x80022ca0, 0x4d8)
s32 func_80022ca0(s32 slot)
{
    struct DIRENTRY entries[15];
    KfCardHeader header;
    char path[40] = "bu00:";
    s32 occupied[15];
    RECT icon_rect;
    char slot_digit[10];
    s32 matching_count;
    s32 card_full;
    s32 present = 0;
    s32 index;
    s32 entry_slot;
    s32 handle;

    slot_digit[0] = DAT_8006d6a4;
    slot_digit[1] = DAT_8006d6a5;
    memset(slot_digit + 2, 0, 8);
    memset(occupied, 0, sizeof(occupied));
    memset(entries, 0, sizeof(entries));
    card_full = func_800226ec(entries, &matching_count);

    for (index = 0; index < 15; index++) {
        if (strncmp(entries[index].name, memory_card_file_prefix, 12) == 0) {
            slot_digit[0] = entries[index].name[12];
            entry_slot = atoi(slot_digit);
            occupied[entry_slot - 1] = 1;
            if (entry_slot == slot)
                present = 1;
        }
    }

    if (!present) {
        if (card_full == 1)
            return 2;
        for (index = 0; index < 15; index++) {
            if (occupied[index] == 0) {
                slot = index + 1;
                index = 15;
            }
        }
    }

    strcat(path, memory_card_file_prefix);
    path[17] = slot + '0';
    path[18] = 0;
    header.magic[0] = 'S';
    header.magic[1] = 'C';
    header.magic[2] = 0x13;
    header.magic[3] = 2;
    strcpy(header.title, DAT_80066680.title);
    func_80023178(&header, slot);
    /* Retail indexes the seven stored palettes directly with the one-based slot. */
    memcpy(header.icon_palette, DAT_80066680.icon_palette[slot - 1],
        sizeof(header.icon_palette));

    setRECT(&icon_rect, 800, 240 + slot * 16, 4, 16);
    StoreImage(&icon_rect, (u_long *)header.icon_frames[0]);
    icon_rect.x = 804;
    StoreImage(&icon_rect, (u_long *)header.icon_frames[1]);
    icon_rect.x = 808;
    StoreImage(&icon_rect, (u_long *)header.icon_frames[2]);

    memset(memory_card_buffer, 0, KF_CARD_BLOCK_BYTES);
    func_80048d24(memory_card_buffer + KF_CARD_HEADER_BYTES);
    header.payload_checksum = memory_card_payload_byte_sum(
        memory_card_buffer + KF_CARD_HEADER_BYTES);
    memcpy(memory_card_buffer, &header, sizeof(header));

    if (!present) {
        handle = open(path, FCREAT | (2 << 16));
        if (handle == -1)
            return 1;
        close(handle);
    }
    handle = open(path, FWRITE);
    if (handle == -1)
        return 1;
    if (write(handle, memory_card_buffer, KF_CARD_BLOCK_BYTES) != KF_CARD_BLOCK_BYTES)
        return 1;
    close(handle);
    memory_card_loaded_slot = slot;
    return 0;
}
