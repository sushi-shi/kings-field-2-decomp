#ifndef KF_LIB_GPU_H
#define KF_LIB_GPU_H

/* ResetGraph modes used by the programs: a full reset at display start-up,
 * and mode 3, which reinitializes the GPU but keeps the display environment,
 * before an overlay hands over. */
enum {
    KF_GPU_RESET_FULL = 0,
    KF_GPU_RESET_KEEP_DISPLAY = 3
};

#endif
