#include "timer.h"

#include <stdlib.h>

struct timer {
    struct timespec total;
    struct timespec tmp;
};

static inline void
time_sub(struct timespec *dest, struct timespec *a)
{
    dest->tv_sec  -= a->tv_sec;
    dest->tv_nsec -= a->tv_nsec;

    if (dest->tv_nsec < 0) {
        --dest->tv_sec;
        dest->tv_nsec += 1000000000;
    }
}

static inline void
time_add(struct timespec *dest, struct timespec *a)
{
    dest->tv_sec  += a->tv_sec;
    dest->tv_nsec += a->tv_nsec;

    if (dest->tv_nsec > 1000000000) {
        ++dest->tv_sec;
        dest->tv_nsec -= 1000000000;
    }
}

Timer * timer_new(void)
{
    Timer *t = malloc(sizeof *t);
    t->total.tv_sec = 0;
    t->total.tv_nsec = 0;
    t->tmp.tv_sec = 0;
    t->tmp.tv_nsec = 0;
    return t;
}

void timer_free(Timer *t)
{
    free(t);
}

void timer_start(Timer *timer)
{
    clock_gettime(CLOCK_REALTIME, &timer->tmp);
    time_sub(&timer->total, &timer->tmp);
}

void timer_stop(Timer *timer)
{
    clock_gettime(CLOCK_REALTIME, &timer->tmp);
    time_add(&timer->total, &timer->tmp);
}

double timer_get_elapsed(Timer *timer)
{
    return timer->total.tv_sec + timer->total.tv_nsec / 1000000000.0;
}
