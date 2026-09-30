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

extern long memory_card_io_end_event;
extern long memory_card_timeout_event;
extern long memory_card_new_device_event;
extern long memory_card_error_event;
/* Reset by any pad input; its increment site is not yet reviewed. */
extern s32 input_idle_counter;
extern s32 menu_cursor_animation_frame;
/* Memory-card file I/O buffer and the pointer the card code reads through. */
extern u8 *memory_card_buffer;
extern u8 memory_card_buffer_storage[];
extern char memory_card_file_prefix[16];
extern u8 memory_card_loaded_slot;

void input_wait_release(void);
u32 input_read_mark_active(void);
void memory_card_initialize(void);
void memory_card_shutdown_events(void);
void memory_card_start(void);
void memory_card_stop(void);
s32 memory_card_probe_temporary_file(void);
s32 memory_card_format(void);
s32 func_80022b74(s32 slot);
void func_80023178(u8 *label, s32 slot_glyph);
u32 memory_card_payload_byte_sum(const u8 *payload);
s32 memory_card_wait_event(void);
void memory_card_clear_events(void);

#endif
