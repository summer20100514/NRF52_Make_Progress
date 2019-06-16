#ifndef __PROGRESS_GUI_H_
#define __PROGRESS_GUI_H_

#include <stdint.h>
#include <string.h>

typedef enum {
    fsm_rt_err = -1,
    fsm_rt_cpl = 0,
    fsm_rt_ongoing = 1,
} fsm_rt_t;


void EPD_Display_Test(void);
void trigger_loop_timer_init(void);
void trigger_loop_timer(void);
void fsm_timer_init(void);
fsm_rt_t show_progress_hm_time_with_power_down(uint16_t cur_mins, uint16_t set_mins);
fsm_rt_t show_settings_screen(void);
void update_settings_screen(uint16_t mins);
void epd_display_init(void);

#endif