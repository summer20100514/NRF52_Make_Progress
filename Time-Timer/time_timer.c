#include "time_timer.h"
#include "app_timer.h"
#include "app_util_platform.h"
#include "app_error.h"
#include "nrf_log.h"

extern uint8_t get_time_zone(void);
extern uint16_t get_tai_utc_delta(void);

APP_TIMER_DEF(m_time_timer);
static uint64_t m_time_secs = 0;
//const nrf_drv_timer_t TIMER_TIME = NRF_DRV_TIMER_INSTANCE(1);

void set_time(uint64_t secs)
{
    CRITICAL_REGION_ENTER();
    m_time_secs = secs;
    CRITICAL_REGION_EXIT();
}

uint64_t get_time(void)
{
    uint32_t time_ticks, capture;
    uint64_t secs;
    CRITICAL_REGION_ENTER();
    secs = m_time_secs;
    CRITICAL_REGION_EXIT();

    return secs;
}

/*
uint32_t get_unix_time(void)
{
    uint32_t secs;
    CRITICAL_REGION_ENTER();
    secs = m_time_secs;
    secs += 946684800UL - get_tai_utc_delta();
    CRITICAL_REGION_EXIT();

    return secs;
}
*/

static void update_time(void)
{
    CRITICAL_REGION_ENTER();
    m_time_secs++;
    CRITICAL_REGION_EXIT();    
}

static void time_timer_handler(void *p_unused)
{
    update_time();
    // NRF_LOG_RAW_INFO("time: %ld\r\n", (uint32_t)get_time());
}

void time_timer_init(void)
{
    ret_code_t err_code;
    err_code = app_timer_create(&m_time_timer,
                            APP_TIMER_MODE_REPEATED,
                            time_timer_handler);
    APP_ERROR_CHECK(err_code);
    err_code = app_timer_start(m_time_timer, APP_TIMER_TICKS(1000), NULL);
    APP_ERROR_CHECK(err_code);
}

