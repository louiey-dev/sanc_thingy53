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

/** DEFINES (#define xx) **/

/*****************************************************************/

/** EXTERNS (extern xx) **/

/*****************************************************************/

/** STATICS (static xx) **/

/*****************************************************************/

/** LOCALS (m_xx) **/
LOG_MODULE_REGISTER(bsp, CONFIG_LOG_DEFAULT_LEVEL);

/*****************************************************************/

BSP_ST g_Bsp = {0};

const struct gpio_dt_spec dk_led[] = {
    GPIO_DT_SPEC_GET(LED_RED_NODE, gpios),
    GPIO_DT_SPEC_GET(LED_GREEN_NODE, gpios),
    GPIO_DT_SPEC_GET(LED_BLUE_NODE, gpios)
};

const size_t dk_led_size = ARRAY_SIZE(dk_led);

int bsp_init(void)
{
    g_Bsp.isInit = true;
    g_Bsp.nus_duration = BSP_DEFAULT_NUS_DURATION;
    bsp_led_init();
    return 0;
}

int bsp_led_init(void)
{
    static bool initialized = false;
    if (!initialized) {
        for (size_t i = 0; i < dk_led_size; i++) {
            if (!device_is_ready(dk_led[i].port)) {
                LOG_ERR("GPIO device for LED%d not ready", i);
                return -ENODEV;
            }
            int ret = gpio_pin_configure_dt(&dk_led[i], GPIO_OUTPUT_INACTIVE);
            if (ret < 0) {
                LOG_ERR("Failed to configure LED%d (err %d)", i, ret);
                return ret;
            }
        }
        initialized = true;
    }
    return 0;
}

/**
 * @brief Toggle the state of an LED.
 * @param led_offset The offset of the LED to toggle. 0 : RED, 1 : GREEN, 2 : BLUE
 * @note The LED offset is based on the order of the LEDs defined in the device tree
 * @return 0 on success, negative error code on failure.
 */
int bsp_led_toggle(int led_offset)
{
    if (led_offset < 0 || led_offset >= dk_led_size) {
        LOG_ERR("Invalid LED offset %d", led_offset);
        return -EINVAL;
    }

    int ret = bsp_led_init();
    if (ret < 0) {
        LOG_ERR("Failed to initialize LEDs (err %d)", ret);
        return ret;
    }

    ret = gpio_pin_toggle_dt(&dk_led[led_offset]);
    if (ret < 0)
    {
        LOG_ERR("Failed to toggle LED%d (err %d)", led_offset, ret);
    }
    LOG_INF("%s led toggled", (led_offset == 0) ? "RED" : (led_offset == 1) ? "GREEN" : "BLUE");
    return 0;
}

int bsp_led_control(int led_offset, bool state)
{
    if (led_offset < 0 || led_offset >= dk_led_size) {
        LOG_ERR("Invalid LED offset %d. Max allowed is %d", led_offset, dk_led_size - 1);
        return -EINVAL;
    }

    int ret = bsp_led_init();
    if (ret < 0) {
        LOG_ERR("Failed to initialize LEDs (err %d)", ret);
        return ret;
    }

    ret = gpio_pin_set_dt(&dk_led[led_offset], state);
    if (ret < 0)
    {
        LOG_ERR("Failed to set LED%d state (err %d)", led_offset, ret);
    }
    LOG_INF("%s led %s", (led_offset == 0) ? "RED" : (led_offset == 1) ? "GREEN" : "BLUE", state ? "ON" : "OFF");
    return ret;
}

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

int bsp_reset(void)
{
    LOG_INF("System reset requested");
    // sys_reboot(SYS_REBOOT_COLD);
    NVIC_SystemReset();
    return 0; // This line will not be reached, but is here to satisfy the function signature
}
