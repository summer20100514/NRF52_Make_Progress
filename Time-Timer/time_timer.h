#ifndef __TIME_TIMER_H_
#define __TIME_TIMER_H_

#include <stdint.h>

//#define TIME_TIMER_PERIOD_MS  10000

void time_timer_init(void);
void set_time(uint64_t secs);
uint64_t get_time(void);
//uint32_t get_unix_time(void);

#endif

