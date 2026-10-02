#ifndef KF_GAME_CARD_H
#define KF_GAME_CARD_H
#include <kf/lib/types.h>

/* Memory-card BIOS events, as returned by memory_card_wait_event. */
enum {
    KF_CARD_EVENT_IO_END = 0,
    KF_CARD_EVENT_TIMEOUT = 1,
    KF_CARD_EVENT_NEW_DEVICE = 2,
    KF_CARD_EVENT_ERROR = 3
};

enum {
    KF_CARD_BLOCK_BYTES = 0x4000,
    KF_CARD_HEADER_BYTES = 0x400,
    KF_CARD_PAYLOAD_BYTES = KF_CARD_BLOCK_BYTES - KF_CARD_HEADER_BYTES
};

enum {
    KF_CARD_ICON_TYPE_THREE_FRAMES = 0x13,
    KF_CARD_FILE_BLOCKS = 2
};

/* The file header copies this 0x280-byte prefix; the rest of the 0x400-byte
 * block header remains zero in memory_card_buffer. */
typedef struct KfCardHeader {
    u8 magic[2];
    u8 icon_type;
    u8 block_count;
    char title[0x40];
    u8 reserved_44[0x1c];
    u16 icon_palette[16];
    u8 icon_frames[3][0x80];
    u32 payload_checksum;
    u8 reserved_204[0x7c];
} KfCardHeader;
typedef char kf_card_header_size[sizeof(KfCardHeader) == 0x280 ? 1 : -1];
typedef char kf_card_header_icon_type_offset[
    (u32)&((KfCardHeader *)0)->icon_type == 2 ? 1 : -1];
typedef char kf_card_header_block_count_offset[
    (u32)&((KfCardHeader *)0)->block_count == 3 ? 1 : -1];
typedef char kf_card_header_title_offset[
    (u32)&((KfCardHeader *)0)->title == 4 ? 1 : -1];
typedef char kf_card_header_palette_offset[
    (u32)&((KfCardHeader *)0)->icon_palette == 0x60 ? 1 : -1];
typedef char kf_card_header_frames_offset[
    (u32)&((KfCardHeader *)0)->icon_frames == 0x80 ? 1 : -1];
typedef char kf_card_header_checksum_offset[
    (u32)&((KfCardHeader *)0)->payload_checksum == 0x200 ? 1 : -1];

extern long memory_card_io_end_event;
extern long memory_card_timeout_event;
extern long memory_card_new_device_event;
extern long memory_card_error_event;
/* Set to 1 after nonzero PadRead; cleared by input-release/menu handlers. */
extern s32 input_idle_counter;
extern s32 menu_cursor_animation_frame;
/* Memory-card file I/O buffer and the pointer the card code reads through. */
extern u8 *memory_card_buffer;
extern u8 memory_card_buffer_storage[KF_CARD_BLOCK_BYTES];
extern char memory_card_file_prefix[16];
extern u8 memory_card_loaded_slot;
/* The adjacent slot-digit seed bytes still have unresolved storage ownership. */
extern s8 DAT_8006d6a4;
extern s8 DAT_8006d6a5;

struct DIRENTRY;
s32 func_800226ec(struct DIRENTRY *entries, s32 *matching_count);
void input_wait_release(void);
u32 input_read_mark_active(void);
void memory_card_initialize(void);
void memory_card_shutdown_events(void);
void memory_card_start(void);
void memory_card_stop(void);
s32 memory_card_probe_temporary_file(void);
s32 memory_card_format(void);
s32 func_80022b74(s32 slot);
void func_80023178(KfCardHeader *header, s32 slot_glyph);
u32 memory_card_payload_byte_sum(const u8 *payload);
s32 memory_card_wait_event(void);
void memory_card_clear_events(void);
void func_800492dc(const u8 *payload);
void func_80048d24(u8 *payload);

#endif
