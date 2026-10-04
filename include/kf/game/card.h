#ifndef KF_GAME_CARD_H
#define KF_GAME_CARD_H
#include <kf/lib/types.h>
#include <kf/lib/bool.h>

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
    KF_CARD_ICON_FRAME_COUNT = 3,
    KF_CARD_ICON_FRAME_BYTES = 0x80,
    KF_CARD_ICON_PALETTE_COLORS = 16,
    KF_CARD_SAVE_SLOT_COUNT = 7,
    KF_CARD_FILE_BLOCKS = 2,
    KF_CARD_DIRECTORY_CAPACITY = 15
};

/* The file header copies this 0x280-byte prefix; the rest of the 0x400-byte
 * block header remains zero in memory_card_buffer. */
typedef struct KfCardHeader {
    u8 magic[2];
    u8 icon_type;
    u8 block_count;
    char title[0x40];
    u8 reserved_44[0x1c];
    u16 icon_palette[KF_CARD_ICON_PALETTE_COLORS];
    u8 icon_frames[KF_CARD_ICON_FRAME_COUNT][KF_CARD_ICON_FRAME_BYTES];
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

/* Set to 1 after nonzero PadRead; cleared by input-release/menu handlers. */
extern s32 input_press_pending;
extern s32 menu_cursor_animation_frame;
/* Memory-card file I/O buffer and the pointer the card code reads through. */
extern u8 memory_card_buffer_storage[KF_CARD_BLOCK_BYTES];
extern char memory_card_file_prefix[16];
extern u8 memory_card_loaded_slot;

struct DIRENTRY;
b32 memory_card_scan_save_entries(struct DIRENTRY *entries, s32 *matching_count);
b32 memory_card_read_slot_summary(const char *filename, s32 *experience, s32 *level,
    s32 *slot_id);
s32 memory_card_write_slot(s32 slot);
void input_wait_release(void);
u32 input_read_mark_active(void);
void memory_card_initialize(void);
void memory_card_shutdown_events(void);
void memory_card_start(void);
void memory_card_stop(void);
s32 memory_card_probe_temporary_file(void);
b32 memory_card_format(void);
s32 memory_card_read_slot(s32 slot);
void memory_card_write_title_stats(KfCardHeader *header, s32 slot_glyph);
u32 memory_card_payload_byte_sum(const u8 *payload);
s32 memory_card_wait_event(void);
void memory_card_clear_events(void);
void card_payload_restore_game_state(const u8 *payload);
void card_payload_capture_game_state(u8 *payload);

#endif
