#include <kf/lib/null.h>
#include <kf/lib/overlay.h>
#include <kf/lib/types.h>
#include <psyq/kernel.h>

const char *overlay_path_table[3] = {
    "cdrom:OPEN.EXE;1",
    "cdrom:GAME.EXE;1",
    "cdrom:END.EXE;1",
};

long overlay_index = KF_OVERLAY_OPEN;

u8 *overlay_next_request = &kf_psx_overlay_request;

struct EXEC overlay_header;

extern "C" void main(void)
{
    SetMem(KF_MAIN_RAM_MEGABYTES);
    _96_remove();
    _96_init();
    *overlay_next_request = KF_OVERLAY_OPEN;
    for (;;) {
        if (Load(overlay_path_table[overlay_index], &overlay_header) == 1) {
            _96_remove();
            overlay_header.s_addr = 0;
            overlay_header.s_size = 0;
            EnterCriticalSection();
            Exec(&overlay_header, 0, NULL);
            overlay_index = *overlay_next_request;
            _96_init();
        }
    }
}
