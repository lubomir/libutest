#ifndef TIMER_H
#define TIMER_H

/**
 * Timer allows the program to measure real time. This is an opaque structure
 * and should be used only by provided functions.
 */
typedef struct timer Timer;

/**
 * Create new timer.
 */
Timer * timer_new(void);

/**
 * Release all memory used by a timer.
 */
void timer_free(Timer *t);

/**
 * Start the timer. This function marks the timer as running, so that time is
 * being recorded.
 */
void timer_start(Timer *timer);

/**
 * Stop the timer. This puts the timer into not running state. Time elapsed
 * since starting the timer is added to the total.
 */
void timer_stop(Timer *timer);

/**
 * Get total elapsed time when timer was running. Returned value is in seconds.
 */
double timer_get_elapsed(Timer *timer);

#endif /* end of include guard: TIMER_H */
