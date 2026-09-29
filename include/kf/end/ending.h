#ifndef KF_END_ENDING_H
#define KF_END_ENDING_H
#include <kf/lib/types.h>
#include <sys/types.h>

/* END.EXE: loads ED.D, opens its music and plays the ending movie. */
extern u8 *ending_data;
extern char ending_data_file[5];

void ending_load_data(void);
void ending_open_audio(void);
void ending_play_movie(void);

#endif
