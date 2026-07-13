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

/* Octave 5 Frequencies (Recommended for Thingy:53 buzzer) */
#define NOTE_DO   523
#define NOTE_RE   587
#define NOTE_MI   659
#define NOTE_FA   698
#define NOTE_SOL  784
#define NOTE_LA   880
#define NOTE_SI   988
#define NOTE_DO2  1047

typedef struct BSP_S
{
    bool isInit;

    uint32_t nus_duration; // NUS duration in seconds
    uint32_t led_pwm_period; // LED PWM period in mili seconds
    /************************************/

} BSP_ST;

enum BSP_ERROR_EN
{
    BSP_OK = 0,
    BSP_ERR = -1,
    BSP_NG = -2
};

/***************************************************
 * MACROs
 **************************************************/
#define MAC_CONFIG_GPIO_OR_RETURN(spec, flags)                           \
        do {                                                            \
            if (!gpio_is_ready_dt(&(spec))) {                           \
                LOG_ERR("%s is not ready", #spec);                      \
                return -ENODEV;                                         \
            }                                                           \
            int __ret = gpio_pin_configure_dt(&(spec), (flags));        \
            if (__ret < 0) {                                            \
                LOG_ERR("Failed to configure %s: %d", #spec, __ret);    \
                return __ret;                                           \
            }                                                           \
        } while (0)

#define LOG_FLOAT_FMT "%d.%02d"
#define LOG_FLOAT_VAL(f) \
    ((int)(f)), \
    ((int)(((f) - (int)(f)) * 100.0f < 0 ? -(((f) - (int)(f)) * 100.0f) : (((f) - (int)(f)) * 100.0f)))

/**************************************************/

/***************************************************
 * BSP APIs
 **************************************************/
int bsp_init(void);
int bsp_led_init(void);
int bsp_led_toggle(int led_offset);
int bsp_led_control(int led_offset, bool state);

int bsp_gpio_init(void);
int bsp_gpio_3v3_control(int state);
int bsp_gpio_sens_pwr_control(int state);
int bsp_gpio_battery_control(int state);

int bsp_buzzer_init(void);
int bsp_buzzer_scale(int scale, uint32_t duration_ms);
int bsp_buzzer_beep(uint32_t frequency_hz, uint32_t duration_ms);
int bsp_buzzer_tone(uint32_t frequency_hz);

int bsp_led_pwm_init(void);
int bsp_led_pwm_red(float brightness);
int bsp_led_pwm_green(float brightness);
int bsp_led_pwm_blue(float brightness);
int bsp_led_pwm_set_color(float r,float g, float b);
int bsp_led_pwm_on(void);
int bsp_led_pwm_off(void);
int bsp_led_pwm_blink_red(float brightness,int32_t up,int32_t down);
int bsp_led_pwm_blink_green(float brightness,int32_t up,int32_t down);
int bsp_led_pwm_blink_blue(float brightness,int32_t up,int32_t down);
int bsp_led_pwm_blink_color(float r,float g, float b,int32_t up,int32_t down);

int bsp_msg_parser(const char *msg, size_t len);

int bsp_adc_init(void);
int bsp_adc_battery_err(void);
int bsp_adc_battery_charging(void);
int bsp_adc_battery_raw(int32_t *raw_val_out);
int bsp_adc_battery_mv(int32_t *mv_val_out);

bool bsp_uart_gets(uint8_t *pbLine);
int bsp_reset(void);
/**************************************************/


#ifdef __cplusplus
}
#endif

#endif // BSP_H
