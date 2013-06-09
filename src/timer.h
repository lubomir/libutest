#ifndef TIMER_H
#define TIMER_H

#define _POSIX_C_SOURCE 199309L

#include <time.h>

typedef struct timer Timer;

Timer * timer_new(void);
void timer_free(Timer *t);
void timer_start(Timer *timer);
void timer_stop(Timer *timer);

double timer_get_elapsed(Timer *timer);

#endif /* end of include guard: TIMER_H */
