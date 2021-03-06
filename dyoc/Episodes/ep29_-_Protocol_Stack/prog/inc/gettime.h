#ifndef _GETTIME_H_
#define _GETTIME_H_

typedef enum {
   CLOCK_MONOTONIC = 0
} clockid_t;

struct timespec {
        time_t   tv_sec;        /* seconds */
        long     tv_nsec;       /* nanoseconds */
};

int clock_gettime(clockid_t clk_id, struct timespec *tp);

#endif // _GETTIME_H_

