/*
 * @file : bsp_led_pwm.c
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
#include <zephyr/drivers/pwm.h>
#include <zephyr/drivers/gpio.h>

/** DEFINES (#define xx) **/
#define PWM_PERIOD PWM_MSEC(20)

#define CHECK_PWM_INIT_OR_RETURN() \
        do { \
            if (!m_isInit) { \
                LOG_ERR("LED PWM not initialized"); \
                return -1; \
            } \
        } while (0)
/*****************************************************************/

/** EXTERNS (extern xx) **/
extern BSP_ST g_Bsp;
/*****************************************************************/

/** STATICS (static xx) **/
static bool m_isInit = false;

static const struct pwm_dt_spec red_pwm_led =	PWM_DT_SPEC_GET(DT_ALIAS(pwm_led0));
static const struct pwm_dt_spec green_pwm_led =	PWM_DT_SPEC_GET(DT_ALIAS(pwm_led1));
static const struct pwm_dt_spec blue_pwm_led =	PWM_DT_SPEC_GET(DT_ALIAS(pwm_led2));
/*****************************************************************/

/** LOCALS (m_xx) **/
LOG_MODULE_REGISTER(bsp_led_pwm, CONFIG_LOG_DEFAULT_LEVEL);
/*****************************************************************/

/** FUNCTIONS (int bsp_xx()) **/

/**
 * @brief Initialize the LED PWM.
 * @param pwm_led The PWM device specification for the LED.
 * @return int
 */
static int init_led_pwm(const struct pwm_dt_spec pwm_led){
	int ret = 0;
	if (!device_is_ready(pwm_led.dev)) {
		LOG_ERR("PWM LED Init error '%s' device_is_ready()\n",pwm_led.dev->name);
		return ret;
	}

    ret = pwm_set_dt(&pwm_led, g_Bsp.led_pwm_period, 0);
    if (ret) {
        LOG_ERR("Error %d: failed to set pulse width for %s\n", ret,pwm_led.dev->name);
        return ret;
    }
    return ret;
}

/**
 * @brief Set the led object
 * @param pwm_led led object
 * @param brightness brightness level in float (0.0 to 1.0)
 * @return int 
 */
static int set_led(const struct pwm_dt_spec pwm_led,float brightness){
    int ret = 0;
    uint32_t pulse = brightness * g_Bsp.led_pwm_period;
	ret = pwm_set_pulse_dt(&pwm_led, pulse);
	if (ret < 0) {
		LOG_ERR("LED Init error pwm_set_pulse_dt()\n");
		return ret;
	}
    return ret;
}

/**
 * @brief Initialize the led object
 * @return int 0 : success, -1 : error 
 */
int bsp_led_pwm_init(void)
{
    g_Bsp.led_pwm_period = PWM_PERIOD;

    init_led_pwm(red_pwm_led);
    init_led_pwm(green_pwm_led);
    init_led_pwm(blue_pwm_led);

    m_isInit = true;
    LOG_INF("LED PWM initialized successfully");
    return 0;
}

/**
 * @brief Set the red led brightness
 * @param brightness 0.0 ~ 1.0
 * @return int 0 : success, -1 : error 
 */
int bsp_led_pwm_red(float brightness)
{
    CHECK_PWM_INIT_OR_RETURN();
	pwm_set_pulse_dt(&green_pwm_led, 0);
	pwm_set_pulse_dt(&blue_pwm_led, 0);

    LOG_INF("Setting RED LED brightness to %.2f", (double)brightness);
	return set_led(red_pwm_led, brightness);
}

/**
 * @brief Set the green led brightness
 * 
 * @param brightness 
 * @return int 0 : success, -1 : error 
 */
int bsp_led_pwm_green(float brightness)
{
    CHECK_PWM_INIT_OR_RETURN();
	pwm_set_pulse_dt(&red_pwm_led, 0);
	pwm_set_pulse_dt(&blue_pwm_led, 0);
    LOG_INF("Setting GREEN LED brightness to %.2f", (double)brightness);
	return set_led(green_pwm_led, brightness);
}

/**
 * @brief Set the blue led brightness
 * 
 * @param brightness 0.0 ~ 1.0
 * @return int 0 : success, -1 : error 
 */
int bsp_led_pwm_blue(float brightness)
{
    CHECK_PWM_INIT_OR_RETURN();

	pwm_set_pulse_dt(&red_pwm_led, 0);
	pwm_set_pulse_dt(&green_pwm_led, 0);
    LOG_INF("Setting BLUE LED brightness to %.2f", (double)brightness);
	return set_led(blue_pwm_led, brightness);
}

/**
 * @brief Set the color of the LED
 * 
 * @param r 0.0 ~ 1.0
 * @param g 0.0 ~ 1.0
 * @param b 0.0 ~ 1.0
 * @param brightness 0.0 ~ 1.0
 * @return int 0 : success, -1 : error 
 */
int bsp_led_pwm_set_color(float r,float g, float b, float brightness)
{
    CHECK_PWM_INIT_OR_RETURN();

    uint32_t red = r * brightness * g_Bsp.led_pwm_period;
    uint32_t green = g * brightness * g_Bsp.led_pwm_period;
    uint32_t blue = b * brightness * g_Bsp.led_pwm_period;

    pwm_set_pulse_dt(&red_pwm_led, red);
    pwm_set_pulse_dt(&green_pwm_led, green);
    pwm_set_pulse_dt(&blue_pwm_led, blue);

    LOG_INF("Setting LED color to R: %.2f, G: %.2f, B: %.2f (Brightness: %.2f)", (double)r, (double)g, (double)b, (double)brightness);
    return 0;
}

/**
 * @brief Turn LED on with full brightness
 * @return int 
 */
int bsp_led_pwm_on(void)
{
	CHECK_PWM_INIT_OR_RETURN();
	pwm_set_pulse_dt(&red_pwm_led, PWM_PERIOD);
	pwm_set_pulse_dt(&green_pwm_led, PWM_PERIOD);
	pwm_set_pulse_dt(&blue_pwm_led, PWM_PERIOD);
    LOG_INF("LEDs turned on with full brightness");
	return 0;
}

/**
 * @brief Turn LED off
 * @return int 
 */
int bsp_led_pwm_off(void)
{
    CHECK_PWM_INIT_OR_RETURN();
	pwm_set_pulse_dt(&red_pwm_led, 0);
	pwm_set_pulse_dt(&green_pwm_led, 0);
	pwm_set_pulse_dt(&blue_pwm_led, 0);
    LOG_INF("LEDs turned off");
	return 0;
}

/**
 * @brief Turn LED Red on with specified brightness
 * @param brightness 0.0 ~ 1.0
 * @param up on time in msec
 * @param down down time in msec
 * @return int 
 */
int bsp_led_pwm_blink_red(float brightness,int32_t up,int32_t down)
{
    CHECK_PWM_INIT_OR_RETURN();
	bsp_led_pwm_red(brightness);
	k_msleep(up);
	bsp_led_pwm_off();
	k_msleep(down);
    LOG_INF("RED LED blinked");
	return 0;
}

/**
 * @brief Turn LED Green on with specified brightness
 * @param brightness 0.0 ~ 1.0
 * @param up on time in msec
 * @param down down time in msec
 * @return int 
 */
int bsp_led_pwm_blink_green(float brightness,int32_t up,int32_t down)
{
    CHECK_PWM_INIT_OR_RETURN();
	bsp_led_pwm_green(brightness);
	k_msleep(up);
	bsp_led_pwm_off();
	k_msleep(down);
    LOG_INF("GREEN LED blinked");
	return 0;
}

/**
 * @brief Turn LED Blue on with specified brightness
 * @param brightness 0.0 ~ 1.0
 * @param up on time in msec
 * @param down down time in msec
 * @return int 
 */
int bsp_led_pwm_blink_blue(float brightness,int32_t up,int32_t down)
{
    CHECK_PWM_INIT_OR_RETURN();
	bsp_led_pwm_blue(brightness);
	k_msleep(up);
	bsp_led_pwm_off();
	k_msleep(down);
    LOG_INF("BLUE LED blinked");
	return 0;
}

/**
 * @brief Turn LED on with specified r/g/b brightness
 * @param r 0.0 ~ 1.0
 * @param g 0.0 ~ 1.0
 * @param b 0.0 ~ 1.0
 * @param up on time in msec
 * @param down off time in msec
 * @return int 
 */
int bsp_led_pwm_blink_color(float r,float g, float b,int32_t up,int32_t down)
{
    CHECK_PWM_INIT_OR_RETURN();
	bsp_led_pwm_set_color(r,g,b, 1.0);
	k_msleep(up);
	bsp_led_pwm_off();
	k_msleep(down);
    LOG_INF("COLOR LED blinked");
	return 0;
}
