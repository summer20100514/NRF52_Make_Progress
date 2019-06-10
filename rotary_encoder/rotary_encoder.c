#include "nrf.h"
#include "nrf_drv_gpiote.h"
#include "app_error.h"
#include "boards.h"
#include "rotary_encoder.h"
#include "nrf_log.h"
#include "app_util_platform.h"
#include "app_timer.h"

APP_TIMER_DEF(m_key_pressed_timer_id);  

static int key_value = KEY_UNPRESSED, rotary_state = ROTARY_STATE_INVALID;

static void key_pressed_timer_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    key_value = KEY_PRESSED_LONG;
    // NRF_LOG_RAW_INFO("Long press\r\n");
}

void rotary_encoder_pin_key_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    // NRF_LOG_RAW_INFO("Key Pressed!\r\n");
    uint32_t level, diff;
    static uint32_t timer_cnt = 0;

    CRITICAL_REGION_ENTER();
    level = nrf_gpio_pin_read(ROTARY_ENCODER_PIN_KEY);
    if (level == 0) {
        timer_cnt = app_timer_cnt_get();
        app_timer_start(m_key_pressed_timer_id, APP_TIMER_TICKS(KEY_PRESS_LONG_TIMEOUT), NULL);
    } else if (key_value != KEY_PRESSED_LONG) {
        app_timer_stop(m_key_pressed_timer_id);
        diff = app_timer_cnt_get() - timer_cnt;
        if (diff >= APP_TIMER_TICKS(KEY_PRESS_SHORT_TIMEOUT) &&
              diff < APP_TIMER_TICKS(KEY_PRESS_LONG_TIMEOUT)) {
            key_value = KEY_PRESSED_SHORT;
            // NRF_LOG_RAW_INFO("Short press, diff %d\r\n", diff);
        }
    }
    CRITICAL_REGION_EXIT();
}

void rotary_encoder_pin_a_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    static int pulse_cnt = 0, direction = 0, direction_last = 0;
    uint32_t level;

    level = nrf_gpio_pin_read(ROTARY_ENCODER_PIN_B);
    // NRF_LOG_RAW_INFO("A-0 B-%d\r\n", level);
    /* A-0 B-0 clockwise A-0 B-1 counterclockwise */
    if (level == 0) {
        direction = 1;
    } else {
        direction = -1;
    }
    
    if (direction_last != 0 && direction_last != direction) { // direction changed
        pulse_cnt = 0;
    }

    pulse_cnt += direction;
    if ((pulse_cnt > 0 && (pulse_cnt % PULSE_CNT_THRESHOLD) == 0) ||
          (pulse_cnt < 0 && (pulse_cnt % (-PULSE_CNT_THRESHOLD)) == 0)) {
        rotary_state = (pulse_cnt > 0) ? (ROTARY_STATE_CLOCKWISE) : (ROTARY_STATE_COUNTERCLOCKWISE);
        // NRF_LOG_RAW_INFO("%s\r\n", (rotary_state == ROTARY_STATE_CLOCKWISE) ? "CLOCKWISE" : "COUNTERCLOCKWISE");
    }

    direction_last = direction;
}

int get_key_value(void)
{
    int value;

    CRITICAL_REGION_ENTER();
    value = key_value;
    key_value = KEY_UNPRESSED;
    CRITICAL_REGION_EXIT();

    return value;
}

int get_rotary_encoder_state(void)
{
    int state;

    CRITICAL_REGION_ENTER();
    state = rotary_state;
    rotary_state = ROTARY_STATE_INVALID;
    CRITICAL_REGION_EXIT();

    return state;
}

/**
 * @brief Function for configuring: PIN_IN pin for input, PIN_OUT pin for output,
 * and configures GPIOTE to give an interrupt on pin change.
 */
void rotary_encoder_init(void)
{
    ret_code_t err_code;

    if (!nrf_drv_gpiote_is_init()) {
        err_code = nrf_drv_gpiote_init();
        APP_ERROR_CHECK(err_code);
    }

    nrf_drv_gpiote_in_config_t in_config_key = GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
    in_config_key.pull = NRF_GPIO_PIN_NOPULL;
    err_code = nrf_drv_gpiote_in_init(ROTARY_ENCODER_PIN_KEY, &in_config_key, rotary_encoder_pin_key_handler);
    APP_ERROR_CHECK(err_code);
    nrf_drv_gpiote_in_event_enable(ROTARY_ENCODER_PIN_KEY, true);

    nrf_drv_gpiote_in_config_t in_config_a = GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
    in_config_a.pull = NRF_GPIO_PIN_NOPULL;
    err_code = nrf_drv_gpiote_in_init(ROTARY_ENCODER_PIN_A, &in_config_a, rotary_encoder_pin_a_handler);
    APP_ERROR_CHECK(err_code);
    nrf_drv_gpiote_in_event_enable(ROTARY_ENCODER_PIN_A, true);

    nrf_gpio_cfg_input(ROTARY_ENCODER_PIN_B, NRF_GPIO_PIN_NOPULL);

    err_code = app_timer_create(&m_key_pressed_timer_id,
                            APP_TIMER_MODE_SINGLE_SHOT,
                            key_pressed_timer_handler);
    APP_ERROR_CHECK(err_code);
}