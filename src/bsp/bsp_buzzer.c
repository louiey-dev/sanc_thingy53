/*
 * @file : bsp_buzzer.c
 * 
 * @brief : 
 * 
 * @author : louiey, louiey@thountech.com
 * 
 * @date : 2026-07-13
 * @copyright : Copyright (c) 2026
 *
 * @section : History
 *            <Date>   <Description of Changes>
 */
#include <zephyr/drivers/pwm.h>
#include "bsp.h"

/** DEFINES (#define xx) **/

/*****************************************************************/

/** EXTERNS (extern xx) **/
extern BSP_ST g_Bsp;
/*****************************************************************/

/** STATICS (static xx) **/
/* Get the buzzer configuration from the devicetree alias */
static const struct pwm_dt_spec buzzer = PWM_DT_SPEC_GET(DT_ALIAS(pwm_buzzer));
/*****************************************************************/

/** LOCALS (m_xx) **/
LOG_MODULE_REGISTER(bsp_buzzer, CONFIG_LOG_DEFAULT_LEVEL);
/*****************************************************************/

/** FUNCTIONS (int bsp_xx()) **/

/**
 * @brief Initialize the buzzer
 * 
 * @return int 
 */
int bsp_buzzer_init(void)
{
    int ret = 0;
    if (!device_is_ready(buzzer.dev)) {
        LOG_ERR("Buzzer PWM device not ready");
        return -ENODEV;
    }
    
    /* Ensure it is initially off */
    ret = pwm_set_pulse_dt(&buzzer, 0);
    if(ret < 0) {
        LOG_ERR("Failed to initialize buzzer: %d", ret);
        return ret;
    }
    LOG_INF("Buzzer initialized successfully");
    return 0;
}

/**
 * @brief Play a tone at a specific frequency
 * @param frequency_hz The frequency in Hz. Set to 0 to stop.
 * @return 0 on success, negative error code on failure.
 */
int bsp_buzzer_tone(uint32_t frequency_hz)
{
    int ret = 0;
    if (frequency_hz == 0) {
        /* Stop the buzzer by setting duty cycle (pulse) to 0 */
        LOG_INF("Stopping buzzer");
        return pwm_set_pulse_dt(&buzzer, 0);
    }

    /* Calculate period and pulse (50% duty cycle for maximum loudness) in nanoseconds
    */
    uint32_t period_ns = 1000000000U / frequency_hz;
    uint32_t pulse_ns = period_ns / 2;

    LOG_INF("Playing tone at %d Hz", frequency_hz);
    ret = pwm_set_dt(&buzzer, period_ns, pulse_ns);
    if(ret != 0) {
        LOG_ERR("Failed to set buzzer pulse: %d", ret);
        return ret;
    }
    return 0;
}

/**
 * @brief Play a short beep
 * @param frequency_hz Tone frequency in Hz
 * @param duration_ms Play duration in milliseconds
 */
int bsp_buzzer_beep(uint32_t frequency_hz, uint32_t duration_ms)
{
    bsp_buzzer_tone(frequency_hz);
    k_msleep(duration_ms);
    bsp_buzzer_tone(0);
    return 0;
}

/**
 * @brief Play a tone with a specified scale and duration
 * 
 * @param scale do/rae/mi/pa/sol/ra/si/do2
 * @param duration_ms on time in milliseconds
 * @return int 
 */
int bsp_buzzer_scale(int scale, uint32_t duration_ms)
{
    switch (scale) {
        case 0:
            bsp_buzzer_beep(NOTE_DO, duration_ms);
            break;
        case 1:
            bsp_buzzer_beep(NOTE_RE, duration_ms);
            break;
        case 2:
            bsp_buzzer_beep(NOTE_MI, duration_ms);
            break;
        case 3:
            bsp_buzzer_beep(NOTE_FA, duration_ms);
            break;
        case 4:
            bsp_buzzer_beep(NOTE_SOL, duration_ms);
            break;
        case 5:
            bsp_buzzer_beep(NOTE_LA, duration_ms);
            break;
        case 6:
            bsp_buzzer_beep(NOTE_SI, duration_ms);
            break;
        case 7:
            bsp_buzzer_beep(NOTE_DO2, duration_ms);
            break;
        default:
            bsp_buzzer_beep(NOTE_DO, duration_ms);
            break;
    }
    return 0;
}
