/*****************************************************************************
* | File      	:   DEV_Config.c
* | Author      :   Waveshare team
* | Function    :   Hardware underlying interface
* | Info        :
*                Used to shield the underlying layers of each master 
*                and enhance portability
*----------------
* |	This version:   V2.0
* | Date        :   2018-10-30
* | Info        :
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "DEV_Config.h"
//#include "stm32f1xx_hal_spi.h"
#include "nrf_drv_spi.h"
#include "app_util_platform.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "boards.h"
#include "app_error.h"

#define SPI_INSTANCE  0 /**< SPI instance index. */
static const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(SPI_INSTANCE);  /**< SPI instance. */
static volatile bool spi_xfer_done;  /**< Flag used to indicate that SPI instance completed the transfer. */
static volatile bool spi_initialized = false;

/**
 * @brief SPI user event handler.
 * @param event
 */
void spi_event_handler(nrf_drv_spi_evt_t const * p_event,
                       void *                    p_context)
{
    spi_xfer_done = true;
    // NRF_LOG_RAW_INFO("*");
}

void spi_and_gpio_init(void)
{
    // already initialized
    if (spi_initialized) return; 

    nrf_gpio_cfg_input(EPD_BUSY_PIN, NRF_GPIO_PIN_NOPULL);
    //nrf_gpio_cfg_input(EPD_BUSY_PIN, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_output(EPD_DC_PIN);
    nrf_gpio_cfg_output(EPD_POWER_PIN);

    nrf_drv_spi_config_t spi_config = NRF_DRV_SPI_DEFAULT_CONFIG;
    spi_config.ss_pin   = EPD_CS_PIN;
    spi_config.miso_pin = NRFX_SPI_PIN_NOT_USED;
    spi_config.mosi_pin = EPD_MOSI_PIN;
    spi_config.sck_pin  = EPD_CLK_PIN;
    APP_ERROR_CHECK(nrf_drv_spi_init(&spi, &spi_config, spi_event_handler, NULL));

    spi_initialized = true;
}

void spi_and_gpio_uninit(void)
{
    if (!spi_initialized) return;

    nrf_drv_spi_uninit(&spi);

    nrf_gpio_cfg_default(EPD_DC_PIN);
    nrf_gpio_cfg_default(EPD_CS_PIN);
    nrf_gpio_cfg_default(EPD_BUSY_PIN);
    nrf_gpio_cfg_default(EPD_MOSI_PIN);
    nrf_gpio_cfg_default(EPD_CLK_PIN);

/**
 * [89] GPIOTE: Static 400 ¦ÌA current while using GPIOTE
 * This anomaly applies to IC Rev. Rev 2, build codes QFAA-Ex0, CIAA-Ex0, QFAB-Ex0.
 * It was inherited from the previous IC revision Rev 1.
 *
 * Symptoms
 * Static current consumption between 400 ¦ÌA and 450 ¦ÌA 
 * when using SPIM or TWIM in combination with GPIOTE.
 *
 * Conditions
 * GPIOTE is configured in event mode
 * TWIM/SPIM utilizes EasyDMA
 *
 * Workaround
 * Turn the TWIM/SPIM off and back on after it has been disabled. 
 * To do so, write 0 followed by 1 to the POWER register (address 0xFFC) 
 * of the TWIM/SPIM that must be disabled
 */
    *(volatile uint32_t *)0x40003FFC = 0;
    *(volatile uint32_t *)0x40003FFC;
    *(volatile uint32_t *)0x40003FFC = 1;    

    spi_initialized = false;
}

//extern SPI_HandleTypeDef hspi1;
void DEV_SPI_WriteByte(UBYTE value)
{
    //HAL_SPI_Transmit(&hspi1, &value, 1, 1000);
    spi_xfer_done = false;

    APP_ERROR_CHECK(nrf_drv_spi_transfer(&spi, &value, 1, NULL, 0));

    while (!spi_xfer_done);

/*    while (!spi_xfer_done)
    {
        __WFE();
    }
    */
}


