#include "nrf.h"
#include "nrf_log.h"
#include "nordic_common.h"
#include "app_error.h"
#include "app_timer.h"
#include "EPD_2in9.h"
#include "DEV_Config.h"
#include "GUI_Paint.h"
#include "imagedata.h"
#include "progress_gui.h"

APP_TIMER_DEF(m_fsm_timer_id);
APP_TIMER_DEF(m_trigger_loop_timer_id);

static bool fsm_timer_expired = false;

//static UWORD Imagesize = ((EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1)) * EPD_HEIGHT;
static UBYTE ImageCache[EPD_WIDTH/8*EPD_HEIGHT];

void EPD_Display_Test(void)
{
    #if 0
    if(EPD_Init(lut_full_update) != 0) {
        NRF_LOG_RAW_INFO("e-Paper init failed\r\n");
    }
    EPD_Clear();
    DEV_Delay_ms(400);

    // NRF_LOG_RAW_INFO("Paint_NewImage\r\n");
    Paint_NewImage(ImageCache, EPD_WIDTH, EPD_HEIGHT, 270, WHITE);
    /*show image for array*/
    NRF_LOG_RAW_INFO("show image for array\r\n");
    Paint_SelectImage(ImageCache);
    Paint_Clear(WHITE);

    Paint_DrawBitMap(gImage_2in9);
    EPD_Display(ImageCache);
    DEV_Delay_ms(400);
#endif

#if 0
    if(EPD_Init(lut_full_update) != 0) {
        NRF_LOG_RAW_INFO("e-Paper init failed\r\n");
    }
    EPD_Clear();
    DEV_Delay_ms(400);

    // NRF_LOG_RAW_INFO("Paint_NewImage\r\n");
    Paint_NewImage(ImageCache, EPD_WIDTH, EPD_HEIGHT, 270, WHITE);
    // Drawing on the image
    //1.Select Image
    NRF_LOG_RAW_INFO("SelectImage:ImageCache\r\n");
    Paint_SelectImage(ImageCache);
    Paint_Clear(WHITE);

    // 2.Drawing on the image
    NRF_LOG_RAW_INFO("Drawing:ImageCache\r\n");
    Paint_DrawPoint(10, 80, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(20, 70, 70, 120, BLACK, LINE_STYLE_SOLID, DOT_PIXEL_1X1);
    Paint_DrawLine(70, 70, 20, 120, BLACK, LINE_STYLE_SOLID, DOT_PIXEL_1X1);    
    Paint_DrawRectangle(20, 70, 70, 120, BLACK, DRAW_FILL_EMPTY, DOT_PIXEL_1X1);
    Paint_DrawRectangle(80, 70, 130, 120, BLACK, DRAW_FILL_FULL, DOT_PIXEL_1X1);
    Paint_DrawCircle(45, 95, 20, BLACK, DRAW_FILL_EMPTY, DOT_PIXEL_1X1);
    Paint_DrawCircle(105, 95, 20, WHITE, DRAW_FILL_FULL, DOT_PIXEL_1X1);
    Paint_DrawLine(85, 95, 125, 95, BLACK, LINE_STYLE_DOTTED, DOT_PIXEL_1X1);
    Paint_DrawLine(105, 75, 105, 115, BLACK, LINE_STYLE_DOTTED, DOT_PIXEL_1X1);
    Paint_DrawString_EN(10, 0, "waveshare", &Font16, BLACK, WHITE);
    Paint_DrawString_EN(10, 20, "hello world", &Font12, WHITE, BLACK);
    Paint_DrawNum(10, 33, 123456789, &Font12, BLACK, WHITE);
    Paint_DrawNum(10, 50, 987654321, &Font16, WHITE, BLACK);
    Paint_DrawString_CN(130, 0, "ÄãºÃ", &Font12CN, BLACK, WHITE);
    Paint_DrawString_CN(130, 20,"Î¢Èí", &Font20CN, WHITE, BLACK);

    NRF_LOG_RAW_INFO("EPD_Display\r\n");
    EPD_Display(ImageCache);
    DEV_Delay_ms(400);
#endif

#if 0
    //Partial refresh, example shows time    
    if(EPD_Init(lut_partial_update) != 0) {
        NRF_LOG_RAW_INFO("e-Paper init failed\r\n");
    }
    EPD_Clear();
    DEV_Delay_ms(400);

    // NRF_LOG_RAW_INFO("Paint_NewImage\r\n");
    Paint_NewImage(ImageCache, EPD_WIDTH, EPD_HEIGHT, 270, WHITE);
    Paint_SelectImage(ImageCache);
    Paint_Clear(WHITE);

    Paint_DrawRectangle(0, 0, 120, 40, BLACK, DRAW_FILL_FULL, DOT_PIXEL_1X1);
    PAINT_TIME sPaint_time;
    sPaint_time.Hour = 12;
    sPaint_time.Min = 34;
    sPaint_time.Sec = 56;
    for (;;) {
        sPaint_time.Sec = sPaint_time.Sec + 1;
        if (sPaint_time.Sec == 60) {
            sPaint_time.Min = sPaint_time.Min + 1;
            sPaint_time.Sec = 0;
            if (sPaint_time.Min == 60) {
                sPaint_time.Hour =  sPaint_time.Hour + 1;
                sPaint_time.Min = 0;
                if (sPaint_time.Hour == 24) {
                    sPaint_time.Hour = 0;
                    sPaint_time.Min = 0;
                    sPaint_time.Sec = 0;
                }
            }
        }
        Paint_ClearWindows(15, 65, 15 + Font20.Width * 7, 65 + Font20.Height, WHITE);
        Paint_DrawTime(15, 65, &sPaint_time, &Font20, WHITE, BLACK);

        EPD_Display(ImageCache);
        DEV_Delay_ms(500);//Analog clock 1s
    }
#endif

#if 0
    //Partial refresh
    if(EPD_Init(lut_partial_update) != 0) {
        NRF_LOG_RAW_INFO("e-Paper init failed\r\n");
    }
    
    EPD_Clear();
    DEV_Delay_ms(400);
    // NRF_LOG_RAW_INFO("Paint_NewImage\r\n");
    Paint_NewImage(ImageCache, EPD_WIDTH, EPD_HEIGHT, 270, WHITE);
    Paint_SelectImage(ImageCache);
    Paint_Clear(WHITE);

    Paint_DrawCircle(30, 75, 15, BLACK, DRAW_FILL_EMPTY, DOT_PIXEL_1X1);
    Paint_DrawCircle(266, 75, 15, BLACK, DRAW_FILL_EMPTY, DOT_PIXEL_1X1);
    Paint_DrawLine(30, 60, 266, 60, BLACK, DRAW_FILL_EMPTY, DOT_PIXEL_1X1);
    Paint_DrawLine(30, 90, 266, 90, BLACK, DRAW_FILL_EMPTY, DOT_PIXEL_1X1);
    
    Paint_DrawCircle(31, 75, 14, WHITE, DRAW_FILL_FULL, DOT_PIXEL_1X1);
    Paint_DrawCircle(265, 75, 14, WHITE, DRAW_FILL_FULL, DOT_PIXEL_1X1);
    Paint_ClearWindows(30, 60, 45, 89, WHITE);
    Paint_ClearWindows(251, 60, 266, 89, WHITE);

    NRF_LOG_RAW_INFO("EPD_Display\r\n");
    EPD_Display(ImageCache);
    DEV_Delay_ms(400);
#endif
}

static void add_outline_overlay(UBYTE *image, uint16_t array_len)
{
    for (uint16_t i = 0; i < array_len; i++) {
        if (gBar_Outline[i] != 0xFF) {
            image[i] &= gBar_Outline[i];
        }
    }
}

static void update_progressbar(uint16_t percent)
{
/*  The progress bar is limited by two circles(arcs)
    Paint_DrawCircle(30, 75, 15, BLACK, DRAW_FILL_EMPTY, DOT_PIXEL_1X1);
    Paint_DrawCircle(266, 75, 15, BLACK, DRAW_FILL_EMPTY, DOT_PIXEL_1X1);
    */
#define ARC_RADIUS              15
#define PROGRESSBAR_START       (30-ARC_RADIUS)
#define PROGRESSBAR_END         (266+ARC_RADIUS)
#define PROGRESSBAR_LEN         (PROGRESSBAR_END-PROGRESSBAR_START)
#define LEFT_HALF_ARC_CENTER    (PROGRESSBAR_START+ARC_RADIUS)
#define RIGHT_HALF_ARC_CENTER   (PROGRESSBAR_START+PROGRESSBAR_LEN-ARC_RADIUS)

    uint16_t filled_len;
    filled_len = (uint16_t)((float)percent/100*PROGRESSBAR_LEN);
    
    Paint_ClearWindows(PROGRESSBAR_START, 60, PROGRESSBAR_END, 89, WHITE);
    Paint_DrawCircle(30, 75, ARC_RADIUS, BLACK, DRAW_FILL_FULL, DOT_PIXEL_1X1);
    if (filled_len <= ARC_RADIUS) {
        Paint_ClearWindows(LEFT_HALF_ARC_CENTER-(ARC_RADIUS-filled_len), 60, LEFT_HALF_ARC_CENTER+ARC_RADIUS, 89, WHITE);
    } else if (filled_len >= (PROGRESSBAR_LEN-ARC_RADIUS)) {
        if (filled_len > PROGRESSBAR_LEN) filled_len = PROGRESSBAR_LEN;
        Paint_DrawRectangle(30, 60, RIGHT_HALF_ARC_CENTER, 90, BLACK, DRAW_FILL_FULL, DOT_PIXEL_1X1);
        Paint_DrawCircle(266, 75, ARC_RADIUS, BLACK, DRAW_FILL_FULL, DOT_PIXEL_1X1);
        Paint_ClearWindows(RIGHT_HALF_ARC_CENTER+(filled_len-(PROGRESSBAR_LEN-ARC_RADIUS)), 60, RIGHT_HALF_ARC_CENTER+ARC_RADIUS, 89, WHITE);
    } else {
        Paint_ClearWindows(LEFT_HALF_ARC_CENTER, 60, LEFT_HALF_ARC_CENTER+ARC_RADIUS, 89, WHITE);
        Paint_DrawRectangle(30, 60, LEFT_HALF_ARC_CENTER+(filled_len-ARC_RADIUS), 90, BLACK, DRAW_FILL_FULL, DOT_PIXEL_1X1);
    }
    add_outline_overlay(ImageCache, sizeof(ImageCache));
}

static void show_time(UWORD Xstart, UWORD Ystart, sFONT* Font, uint16_t mins)
{
    uint8_t h, m;
    uint16_t char_dist;

    h = mins/60;
    m = mins%60;
    if (h > 99) h = 99;
    char_dist = Font->Width-2;
    Paint_ClearWindows(30, Ystart, 200, Ystart+34, WHITE);
    if (h < 10) {
        Paint_DrawNum_COMPACT(Xstart, Ystart, 0, Font, WHITE, BLACK);
        Paint_DrawNum_COMPACT(Xstart+char_dist, Ystart, h, Font, WHITE, BLACK); 
    } else {
        Paint_DrawNum_COMPACT(Xstart, Ystart, h, Font, WHITE, BLACK);
    }
    Paint_DrawChar(Xstart+char_dist*2, Ystart, ':', Font, WHITE, BLACK);
    if (m < 10) {
        Paint_DrawNum_COMPACT(Xstart+char_dist*3, Ystart, 0, Font, WHITE, BLACK);
        Paint_DrawNum_COMPACT(Xstart+char_dist*4, Ystart, m, Font, WHITE, BLACK); 
    } else {
        Paint_DrawNum_COMPACT(Xstart+char_dist*3, Ystart, m, Font, WHITE, BLACK);
    }
}

static void trigger_loop_timer_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    // NRF_LOG_RAW_INFO("#\r\n");
//    {nrf_gpio_pin_write(31, 1); nrf_delay_us(50); nrf_gpio_pin_write(31, 0);}
}

void trigger_loop_timer_init(void)
{
    ret_code_t err_code;
    err_code = app_timer_create(&m_trigger_loop_timer_id,
                            APP_TIMER_MODE_SINGLE_SHOT,
                            trigger_loop_timer_handler);
    APP_ERROR_CHECK(err_code);
}

void trigger_loop_timer(void)
{
    app_timer_stop(m_trigger_loop_timer_id);
    app_timer_start(m_trigger_loop_timer_id, APP_TIMER_MIN_TIMEOUT_TICKS*2, NULL);
}

static void fsm_timer_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    fsm_timer_expired = true;
    // NRF_LOG_RAW_INFO("->\r\n");
//    {nrf_gpio_pin_write(31, 1); nrf_delay_us(50); nrf_gpio_pin_write(31, 0); nrf_delay_us(50); nrf_gpio_pin_write(31, 1); nrf_delay_us(50); nrf_gpio_pin_write(31, 0);}
}

static bool fsm_timer_fired(void)
{
    bool fired = false;
    CRITICAL_REGION_ENTER();
    fired = fsm_timer_expired;
    fsm_timer_expired = false;
    CRITICAL_REGION_EXIT();

    return fired;
}

static void fsm_timer_start(uint32_t timeout_ticks)
{
    app_timer_stop(m_fsm_timer_id);
    app_timer_start(m_fsm_timer_id, timeout_ticks, NULL);
}

void fsm_timer_init(void)
{
    ret_code_t err_code;
    err_code = app_timer_create(&m_fsm_timer_id,
                            APP_TIMER_MODE_SINGLE_SHOT,
                            fsm_timer_handler);
    APP_ERROR_CHECK(err_code);
}

static fsm_rt_t fsm_delay(uint32_t timeout_ticks)
{
    static enum {
        DELAY_STATE_START = 0,
        DELAY_STATE_START_TIMER,
        DELAY_STATE_CHECK_TIMER
    } s_tDelayState = DELAY_STATE_START;

    switch (s_tDelayState) {
        case DELAY_STATE_START:
            s_tDelayState = DELAY_STATE_START_TIMER;
            //break;
        case DELAY_STATE_START_TIMER:
            fsm_timer_start(timeout_ticks);
            s_tDelayState = DELAY_STATE_CHECK_TIMER;
            break;
        case DELAY_STATE_CHECK_TIMER:
            if (fsm_timer_fired()) {
                s_tDelayState = DELAY_STATE_START;
                return fsm_rt_cpl;
            }
            break;
        default: break;
    }

    return fsm_rt_ongoing;
}

#define PERCENT_NUMBER_OFFSET   160
#define PIXEL_BETWEEN_CHARS     12
// full update
fsm_rt_t show_progress_hm_time_with_power_down(uint16_t cur_mins, uint16_t set_mins)
{
    uint16_t percent, per;
    percent = (uint16_t)((float)cur_mins*100/set_mins);

    static enum {
        SHOW_PROGRESS_START = 0,
        SHOW_PROGRESS_POWER_ON,
        SHOW_PROGRESS_POWER_ON_DELAY,
        SHOW_PROGRESS_INIT_AND_DISPLAY,
        SHOW_PROGRESS_DISPLAY_STABLE_DELAY,
        SHOW_PROGRESS_POWER_OFF
    } s_tState = SHOW_PROGRESS_START;

    switch (s_tState) {
        case SHOW_PROGRESS_START:
            s_tState = SHOW_PROGRESS_POWER_ON;
            // break;
        case SHOW_PROGRESS_POWER_ON:
            spi_and_gpio_init();
            EPD_POWER_ON();
            trigger_loop_timer(); // for low power wakeup
            s_tState = SHOW_PROGRESS_POWER_ON_DELAY;
            break;
        case SHOW_PROGRESS_POWER_ON_DELAY:
            if (fsm_delay(APP_TIMER_TICKS(200)) == fsm_rt_cpl) {
                trigger_loop_timer(); // for low power wakeup
                s_tState = SHOW_PROGRESS_INIT_AND_DISPLAY;
            }
            break;
        case SHOW_PROGRESS_INIT_AND_DISPLAY:
            if(EPD_Init(lut_full_update) != 0) {
                NRF_LOG_RAW_INFO("e-Paper init failed\r\n");
            }
            // NRF_LOG_RAW_INFO("now full update!\r\n");
            Paint_ClearWindows(20, 25, 280, 59, WHITE);
            show_time(75, 25, &Font24, cur_mins);
            if (percent < 10) {
                Paint_DrawChar(PERCENT_NUMBER_OFFSET, 25, '(', &Font20, WHITE, BLACK);
                Paint_DrawNum_COMPACT(PERCENT_NUMBER_OFFSET+PIXEL_BETWEEN_CHARS, 25, percent, &Font20, WHITE, BLACK);
                Paint_DrawChar(PERCENT_NUMBER_OFFSET+PIXEL_BETWEEN_CHARS*2, 25, '%', &Font20, WHITE, BLACK);
                Paint_DrawChar(PERCENT_NUMBER_OFFSET+PIXEL_BETWEEN_CHARS*3, 25, ')', &Font20, WHITE, BLACK);
                per = percent;
            } else if (percent < 100) {
                Paint_DrawChar(PERCENT_NUMBER_OFFSET, 25, '(', &Font20, WHITE, BLACK);
                Paint_DrawNum_COMPACT(PERCENT_NUMBER_OFFSET+PIXEL_BETWEEN_CHARS, 25, percent, &Font20, WHITE, BLACK);
                Paint_DrawChar(PERCENT_NUMBER_OFFSET+PIXEL_BETWEEN_CHARS*3, 25, '%', &Font20, WHITE, BLACK);
                Paint_DrawChar(PERCENT_NUMBER_OFFSET+PIXEL_BETWEEN_CHARS*4, 25, ')', &Font20, WHITE, BLACK);
                per = percent;
            } else if (percent == 100) {
                Paint_DrawChar(PERCENT_NUMBER_OFFSET, 25, '(', &Font20, WHITE, BLACK);
                Paint_DrawNum_COMPACT(PERCENT_NUMBER_OFFSET+PIXEL_BETWEEN_CHARS, 25, percent, &Font20, WHITE, BLACK);
                Paint_DrawChar(PERCENT_NUMBER_OFFSET+PIXEL_BETWEEN_CHARS*4, 25, '%', &Font20, WHITE, BLACK);
                Paint_DrawChar(PERCENT_NUMBER_OFFSET+PIXEL_BETWEEN_CHARS*5, 25, ')', &Font20, WHITE, BLACK);
                per = percent;
            } else if (percent < 110) {
                Paint_DrawChar(PERCENT_NUMBER_OFFSET, 25, '(', &Font20, WHITE, BLACK);
                Paint_DrawChar(PERCENT_NUMBER_OFFSET+PIXEL_BETWEEN_CHARS, 25, '+', &Font20, WHITE, BLACK);
                Paint_DrawNum_COMPACT(PERCENT_NUMBER_OFFSET+PIXEL_BETWEEN_CHARS*2, 25, percent-100, &Font20, WHITE, BLACK);
                Paint_DrawChar(PERCENT_NUMBER_OFFSET+PIXEL_BETWEEN_CHARS*3, 25, '%', &Font20, WHITE, BLACK);
                Paint_DrawChar(PERCENT_NUMBER_OFFSET+PIXEL_BETWEEN_CHARS*4, 25, ')', &Font20, WHITE, BLACK);
                per = 100;
            } else if (percent < 200) {
                Paint_DrawChar(PERCENT_NUMBER_OFFSET, 25, '(', &Font20, WHITE, BLACK);
                Paint_DrawChar(PERCENT_NUMBER_OFFSET+PIXEL_BETWEEN_CHARS, 25, '+', &Font20, WHITE, BLACK);
                Paint_DrawNum_COMPACT(PERCENT_NUMBER_OFFSET+PIXEL_BETWEEN_CHARS*2, 25, percent-100, &Font20, WHITE, BLACK);
                Paint_DrawChar(PERCENT_NUMBER_OFFSET+PIXEL_BETWEEN_CHARS*4, 25, '%', &Font20, WHITE, BLACK);
                Paint_DrawChar(PERCENT_NUMBER_OFFSET+PIXEL_BETWEEN_CHARS*5, 25, ')', &Font20, WHITE, BLACK);
                per = 100;
            } else {
                Paint_DrawChar(PERCENT_NUMBER_OFFSET-10, 25, '(', &Font20, WHITE, BLACK);
                Paint_DrawChar(PERCENT_NUMBER_OFFSET-10+PIXEL_BETWEEN_CHARS, 25, '+', &Font20, WHITE, BLACK);
                Paint_DrawNum_COMPACT(PERCENT_NUMBER_OFFSET-10+PIXEL_BETWEEN_CHARS*2, 25, (percent-100)>999 ? 999 : (percent-100), &Font20, WHITE, BLACK);
                Paint_DrawChar(PERCENT_NUMBER_OFFSET-10+PIXEL_BETWEEN_CHARS*5, 25, '%', &Font20, WHITE, BLACK);
                Paint_DrawChar(PERCENT_NUMBER_OFFSET-10+PIXEL_BETWEEN_CHARS*6, 25, ')', &Font20, WHITE, BLACK);
                per = 100;
            }
            update_progressbar(per);
            show_time(118, 98, &Font20, set_mins);
            EPD_Display(ImageCache);
            trigger_loop_timer(); // for low power wakeup
            s_tState = SHOW_PROGRESS_DISPLAY_STABLE_DELAY;
            break;
        case SHOW_PROGRESS_DISPLAY_STABLE_DELAY:
            if (fsm_delay(APP_TIMER_TICKS(200)) == fsm_rt_cpl) {
                trigger_loop_timer(); // for low power wakeup
                s_tState = SHOW_PROGRESS_POWER_OFF;
            }
            break;
        case SHOW_PROGRESS_POWER_OFF:
            spi_and_gpio_uninit();
            EPD_POWER_OFF();
            s_tState = SHOW_PROGRESS_START;
            return fsm_rt_cpl;
            break;
        default: break;
    }
    
    return fsm_rt_ongoing;
}

// partial update
fsm_rt_t show_settings_screen(void)
{
    static enum {
        SHOW_SETTINGS_START = 0,
        SHOW_SETTINGS_POWER_ON,
        SHOW_SETTINGS_POWER_ON_DELAY,
        SHOW_SETTINGS_INIT,
        SHOW_SETTINGS_INIT_STABLE_DELAY,
        SHOW_SETTINGS_DISPLAY
    } s_tState = SHOW_SETTINGS_START;

    switch (s_tState) {
        case SHOW_SETTINGS_START:
            s_tState = SHOW_SETTINGS_POWER_ON;
            //break;
        case SHOW_SETTINGS_POWER_ON:
            spi_and_gpio_init();
            EPD_POWER_ON();
            trigger_loop_timer(); // for low power wakeup
            s_tState = SHOW_SETTINGS_POWER_ON_DELAY;
            break;
        case SHOW_SETTINGS_POWER_ON_DELAY:
            if (fsm_delay(APP_TIMER_TICKS(200)) == fsm_rt_cpl) {
                trigger_loop_timer(); // for low power wakeup
                s_tState = SHOW_SETTINGS_INIT;
            }
            break;
        case SHOW_SETTINGS_INIT:
            if(EPD_Init(lut_full_update) != 0) {
                NRF_LOG_RAW_INFO("e-Paper init failed\r\n");
            }
            EPD_Clear();
            trigger_loop_timer(); // for low power wakeup
            s_tState = SHOW_SETTINGS_INIT_STABLE_DELAY;
            break;
        case SHOW_SETTINGS_INIT_STABLE_DELAY:
            if (fsm_delay(APP_TIMER_TICKS(200)) == fsm_rt_cpl) {
                trigger_loop_timer(); // for low power wakeup
                s_tState = SHOW_SETTINGS_DISPLAY;
            }
            break;
        case SHOW_SETTINGS_DISPLAY:
            if(EPD_Init(lut_partial_update) != 0) {
                NRF_LOG_RAW_INFO("e-Paper init failed\r\n");
            }
            Paint_Clear(WHITE);
            show_time(105, 60, &Font24, 0);
            EPD_Display(ImageCache);
            s_tState = SHOW_SETTINGS_START;
            return fsm_rt_cpl;
            break;
        default: break;
    }

    return fsm_rt_ongoing;
}

void update_settings_screen(uint16_t mins)
{
    show_time(105, 60, &Font24, mins);
    EPD_Display(ImageCache);    
}

void epd_display_init(void)
{
    EPD_POWER_ON();
    if(EPD_Init(lut_full_update) != 0) {
        NRF_LOG_RAW_INFO("e-Paper init failed\r\n");
    }
    EPD_Clear();
    DEV_Delay_ms(400);
    Paint_NewImage(ImageCache, EPD_WIDTH, EPD_HEIGHT, 270, WHITE);
    Paint_SelectImage(ImageCache);
    Paint_Clear(WHITE);
}


