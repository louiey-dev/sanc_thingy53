/*
 * @file : bsp.c
 * 
 * @brief : 
 * 
 * @author : louiey, louiey@thountech.com
 * 
 * @date : 2026-07-10
 * @copyright : Copyright (c) 2026
 *
 * @section : History
 *            <Date>   <Description of Changes>
 */
#include "bsp.h"
#include <zephyr/console/console.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/reboot.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>

/** DEFINES (#define xx) **/

/*****************************************************************/

/** EXTERNS (extern xx) **/

/*****************************************************************/

/** STATICS (static xx) **/

//not calibrated, fixed for specific board
#define PWM_PERIOD PWM_MSEC(20)
/*****************************************************************/

/** LOCALS (m_xx) **/
LOG_MODULE_REGISTER(bsp, CONFIG_LOG_DEFAULT_LEVEL);

/*****************************************************************/

BSP_ST g_Bsp = {0};

/**
 * @brief Initialize the BSP.
 * @return int 
 */
int bsp_init(void)
{
    g_Bsp.isInit = true;
    g_Bsp.nus_duration = BSP_DEFAULT_NUS_DURATION;

    bsp_led_init();
    bsp_led_pwm_init();
    bsp_gpio_init();
    bsp_adc_init();
    bsp_buzzer_init();
    
    return 0;
}

/**
 * @brief Get a line of text from the UART.
 * @param pbLine The buffer to store the received line.
 * @return true on success, false on failure.
 */
bool bsp_uart_gets(uint8_t *pbLine)
{
    static bool console_initialized = false;
    if (!console_initialized) {
        console_getline_init();
        console_initialized = true;
    }

    char *line = console_getline();
    if (line) {
        strcpy((char *)pbLine, line);
        return true;
    }
    return false;
}

/**
 * @brief Reset the system.
 * @return int 
 */
int bsp_reset(void)
{
    LOG_INF("System reset requested");
    // sys_reboot(SYS_REBOOT_COLD);
    NVIC_SystemReset();
    return 0; // This line will not be reached, but is here to satisfy the function signature
}

