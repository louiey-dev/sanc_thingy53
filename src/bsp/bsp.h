/*
 * @file : bsp.h
 * 
 * @brief : 
 * 
 * @author : louiey, louiey@thountech.com
 * 
 * @date : 2026-07-10
 * @copyright : Copyright (c) 2026
 */

#ifndef BSP_H
#define BSP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include <zephyr/kernel.h> // for bool
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#define LOGD printk

#define MSG(fmt, args...) LOGD("MSG " fmt "", ##args)
#define LOG(fmt, args...) LOGD("LOG " fmt "", ##args)
#define ERR(fmt, args...) LOGD("ERR " fmt "", ##args)

#define USER_FUNC __func__

#define FALSE 0
#define TRUE 1

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

// LED definitions
#define BSP_LED_RED 0
#define BSP_LED_GREEN 1
#define BSP_LED_BLUE 2

#define LED_RED_NODE DT_ALIAS(led0)
#define LED_GREEN_NODE DT_ALIAS(led1)
#define LED_BLUE_NODE DT_ALIAS(led2)

#define BSP_DEFAULT_NUS_DURATION 5 // Default NUS duration in seconds

extern const struct gpio_dt_spec dk_led[];
extern const size_t dk_led_size;

typedef struct BSP_S
{
    bool isInit;

    uint32_t nus_duration; // NUS duration in seconds
    /************************************/

} BSP_ST;

enum BSP_ERROR_EN
{
    BSP_OK = 0,
    BSP_ERR = -1,
    BSP_NG = -2
};

/***************************************************
 * BSP APIs
 **************************************************/
int bsp_init(void);
int bsp_led_init(void);
int bsp_led_toggle(int led_offset);
int bsp_led_control(int led_offset, bool state);

bool bsp_uart_gets(uint8_t *pbLine);
int bsp_reset(void);
/**************************************************/


#ifdef __cplusplus
}
#endif

#endif // BSP_H
