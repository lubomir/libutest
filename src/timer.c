#include "timer.h"

#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include <stdlib.h>

struct timer {
    struct timespec total;
    bool running;
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
    if (t) {
        t->total.tv_sec = 0;
        t->total.tv_nsec = 0;
        t->running = false;
    }
    return t;
}

void timer_free(Timer *t)
{
    free(t);
}

void timer_start(Timer *timer)
{
    if (timer->running) {
        fprintf(stderr, " *** WARNING: timer is already running\n");
    }
    struct timespec tmp = { 0, 0 };
    clock_gettime(CLOCK_REALTIME, &tmp);
    time_sub(&timer->total, &tmp);
    timer->running = true;
}

void timer_stop(Timer *timer)
{
    if (!timer->running) {
        fprintf(stderr, " *** WARNING: timer is not running\n");
    }
    struct timespec tmp = { 0, 0 };
    clock_gettime(CLOCK_REALTIME, &tmp);
    time_add(&timer->total, &tmp);
    timer->running = false;
}

double timer_get_elapsed(Timer *timer)
{
    if (timer->running) {
        fprintf(stderr, " *** WARNING: timer is running\n");
    }
    return timer->total.tv_sec + timer->total.tv_nsec / 1000000000.0;
}
