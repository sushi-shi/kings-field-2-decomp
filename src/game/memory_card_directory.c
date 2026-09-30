#include <kf/lib/address.h>
#include <kf/game/card.h>
#include <psyq/kernel.h>
#include <psyq/libc.h>
#include <CONVERT.H>

RODATA(0x80011120, 0x6)

DATA(0x80066670, 0x10)
char memory_card_file_prefix[16] = "BISLPS-00069";

extern s8 DAT_8006d6a4;
extern s8 DAT_8006d6a5;
extern char DAT_8006d6a8[6];
extern void func_800492dc(const u8 *payload);

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
    u8 header[0x280];
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
    handle = open(path, 1);
    if (handle == -1 || read(handle, header, sizeof(header)) != sizeof(header)) {
        return 1;
    }
    close(handle);

    *experience = 0;
    weight = 100000;
    for (i = 0; i < 6; ++i) {
        ((u8 *)&encoded)[0] = header[0x2c + i * 2];
        ((u8 *)&encoded)[1] = header[0x2d + i * 2];
        if (encoded != 0x4081) {
            encoded = ((s32)encoded >> 8) - 79;
            *experience += encoded * weight;
        }
        weight /= 10;
    }

    *level = 0;
    weight = 10;
    for (i = 0; i < 2; ++i) {
        ((u8 *)&encoded)[0] = header[0x3e + i * 2];
        ((u8 *)&encoded)[1] = header[0x3f + i * 2];
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
        handle = open(path, 1);
        if (handle == -1 || read(handle, memory_card_buffer, KF_CARD_BLOCK_BYTES)
                != KF_CARD_BLOCK_BYTES) {
            status = 1;
        } else {
            close(handle);
            buffer = memory_card_buffer;
            checksum = memory_card_payload_byte_sum(buffer + KF_CARD_HEADER_BYTES);
            if (*(u32 *)(buffer + 0x200) == checksum) {
                func_800492dc(memory_card_buffer + KF_CARD_HEADER_BYTES);
                memory_card_loaded_slot = slot;
                return 0;
            }
            status = 2;
        }
    } while (attempt++ < 2);
    return status;
}
