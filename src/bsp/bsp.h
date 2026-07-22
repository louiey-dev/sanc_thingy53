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
extern "C"
{
#endif

#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include <zephyr/kernel.h> // for bool
#include <zephyr/logging/log.h>

#include <zephyr/drivers/sensor.h>

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

#define BSP_DEFAULT_NUS_DURATION 60 // Default NUS duration in seconds
#define BSP_DEFAULT_SM_DURATION 1000*2

/* Octave 5 Frequencies (Recommended for Thingy:53 buzzer) */
#define NOTE_DO 523
#define NOTE_RE 587
#define NOTE_MI 659
#define NOTE_FA 698
#define NOTE_SOL 784
#define NOTE_LA 880
#define NOTE_SI 988
#define NOTE_DO2 1047

typedef struct BSP_SENSORS_S
{
    uint32_t init;

    // adxl362 sensor

    // bh1749 Color sensor
    struct sensor_value red;
    struct sensor_value green;
    struct sensor_value blue;
    struct sensor_value ir;
    uint16_t lightRed;
    uint16_t lightGreen;
    uint16_t lightBlue;
    uint16_t lightIR;

    // bme688 temp/pressure/humidity/gas sensor
    struct sensor_value temp;
    struct sensor_value press;
    struct sensor_value hum;
    struct sensor_value gas;
    float temperature;
    float pressure;
    float humidity;
    int32_t gas_resistance;

    // bmi270 Accelerometer and gyroscope sensor
    struct sensor_value accel[3];
    struct sensor_value gyro[3];

    // bmm150 Magmeto sensor
    struct sensor_value magn[3];

    // enable flag
    uint8_t bh1749_en;
    uint8_t bme688_en;
    uint8_t bmi270_en;
    uint8_t bmm150_en;

    float accX;
    float accY;
    float accZ;
    float gyroX;
    float gyroY;
    float gyroZ;
    float magX;
    float magY;
    float magZ;

    int32_t batt_mv;
} __aligned(4) BSP_SENSORS_ST;

typedef struct BSP_S
{
    BSP_SENSORS_ST sensor; // 4-byte aligned sub-structure (92 bytes)

    uint32_t led_pwm_period; // 2-byte aligned
    uint16_t nus_duration;   // 2-byte aligned
    uint16_t sm_duration;    // sleep duration time in msec

    uint8_t state_machine_en; // 1 : state machine is enabled, 0 : disabled
    uint8_t nus_notif_enabled; // 1 : NUS notification is enabled, 0 : disabled

    uint8_t isInit;
    uint8_t isSensorLogEnable;
    uint8_t isCborEnable;   // CBOR format message treansfer

} __aligned(4) BSP_ST;

enum BSP_ERROR_EN
{
    BSP_OK = 0,
    BSP_ERR = -1,
    BSP_NG = -2
};

/***************************************************
 * MACROs
 **************************************************/
#define MAC_CONFIG_GPIO_OR_RETURN(spec, flags)                   \
    do                                                           \
    {                                                            \
        if (!gpio_is_ready_dt(&(spec)))                          \
        {                                                        \
            LOG_ERR("%s is not ready", #spec);                   \
            return -ENODEV;                                      \
        }                                                        \
        int __ret = gpio_pin_configure_dt(&(spec), (flags));     \
        if (__ret < 0)                                           \
        {                                                        \
            LOG_ERR("Failed to configure %s: %d", #spec, __ret); \
            return __ret;                                        \
        }                                                        \
    } while (0)
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
int bsp_gpio_fem_control(int mode, int state);
int bsp_gpio_pmic_control(int mode, int state);

int bsp_buzzer_init(void);
int bsp_buzzer_scale(int scale, uint32_t duration_ms);
int bsp_buzzer_beep(uint32_t frequency_hz, uint32_t duration_ms);
int bsp_buzzer_tone(uint32_t frequency_hz);

int bsp_led_pwm_init(void);
int bsp_led_pwm_red(float brightness);
int bsp_led_pwm_green(float brightness);
int bsp_led_pwm_blue(float brightness);
int bsp_led_pwm_set_color(float r, float g, float b, float brightness);
int bsp_led_pwm_on(void);
int bsp_led_pwm_off(void);
int bsp_led_pwm_blink_red(float brightness, int32_t up, int32_t down);
int bsp_led_pwm_blink_green(float brightness, int32_t up, int32_t down);
int bsp_led_pwm_blink_blue(float brightness, int32_t up, int32_t down);
int bsp_led_pwm_blink_color(float r, float g, float b, int32_t up, int32_t down);

int bsp_msg_parser(const char *msg, size_t len);
int bsp_msg_send(uint8_t msg_id, const uint8_t *data, uint8_t len);

int bsp_adc_init(void);
int bsp_adc_battery_err(void);
int bsp_adc_battery_charging(void);
int bsp_adc_battery_raw(int32_t *raw_val_out);
int bsp_adc_battery_mv(int32_t *mv_val_out);

int bsp_sensor_adxl362_init(void);
int bsp_sensor_adxl362_read(struct sensor_value *accel);
int bsp_sensor_bh1749_init(void);
int bsp_sensor_bh1749_read(struct sensor_value *red, struct sensor_value *green, struct sensor_value *blue, struct sensor_value *ir);
int bsp_sensor_bme688_init(void);
int bsp_sensor_bme688_read(struct sensor_value *temp, struct sensor_value *press, struct sensor_value *hum, struct sensor_value *gas);
int bsp_sensor_bmi270_init(void);
int bsp_sensor_bmi270_read(struct sensor_value *accel, struct sensor_value *gyro);
int bsp_sensor_bmm150_init(void);
int bsp_sensor_bmm150_read(struct sensor_value *magn);
int bsp_sensor_power_status(void);
int bsp_board_power_3v3_status(void);

int bsp_state_machine_start(void);
int bsp_state_machine_stop(void);

bool bsp_uart_gets(uint8_t *pbLine);
int bsp_reset(void);
int bsp_nus_send(const void *data, uint16_t len);
void print_buffer(const void *data, uint16_t len);
int bsp_statistics(uint32_t* cpu_usage, uint32_t* heap_allocated, uint32_t* heap_free);
/**************************************************/

#ifdef __cplusplus
}
#endif

#endif // BSP_H
