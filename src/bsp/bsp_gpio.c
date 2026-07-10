/*
 * @file : bsp_gpio.c
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

#include <zephyr/drivers/gpio.h>
#include "bsp.h"

/** DEFINES (#define xx) **/

/*****************************************************************/

/** EXTERNS (extern xx) **/
extern BSP_ST g_Bsp;
/*****************************************************************/

/** STATICS (static xx) **/
static const struct gpio_dt_spec fem_mode_pin = GPIO_DT_SPEC_GET(DT_NODELABEL(fem_mode_pin), gpios);
static const struct gpio_dt_spec fem_rx_en_pin = GPIO_DT_SPEC_GET(DT_NODELABEL(fem_rx_en_pin), gpios);
static const struct gpio_dt_spec fem_tx_en_pin = GPIO_DT_SPEC_GET(DT_NODELABEL(fem_tx_en_pin), gpios);
static const struct gpio_dt_spec fem_sel_pin = GPIO_DT_SPEC_GET(DT_NODELABEL(fem_sel_pin), gpios);

static const struct gpio_dt_spec pmic_iset_pin = GPIO_DT_SPEC_GET(DT_NODELABEL(pmic_iset_pin), gpios);
static const struct gpio_dt_spec pmic_err_pin = GPIO_DT_SPEC_GET(DT_NODELABEL(pmic_err_pin), gpios);
static const struct gpio_dt_spec pmic_chg_pin = GPIO_DT_SPEC_GET(DT_NODELABEL(pmic_chg_pin), gpios);

static const struct gpio_dt_spec pwr_3v3_en_pin = GPIO_DT_SPEC_GET(DT_NODELABEL(pwr_3v3_en_pin), gpios);
static const struct gpio_dt_spec pwr_sens_pwr_ctrl_pin = GPIO_DT_SPEC_GET(DT_NODELABEL(pwr_sens_pwr_ctrl_pin), gpios);

static const struct gpio_dt_spec batt_measure_en_pin = GPIO_DT_SPEC_GET(DT_NODELABEL(batt_measure_en_pin), gpios);


static const struct gpio_dt_spec adxl_int_pin = GPIO_DT_SPEC_GET(DT_NODELABEL(adxl_int_pin), gpios);
static const struct gpio_dt_spec adxl_cs_pin = GPIO_DT_SPEC_GET(DT_NODELABEL(adxl_cs_pin), gpios);
static const struct gpio_dt_spec bmi_int_pin = GPIO_DT_SPEC_GET(DT_NODELABEL(bmi_int_pin), gpios);
static const struct gpio_dt_spec bmi_cs_pin = GPIO_DT_SPEC_GET(DT_NODELABEL(bmi_cs_pin), gpios);
static const struct gpio_dt_spec bmm_int_pin = GPIO_DT_SPEC_GET(DT_NODELABEL(bmm_int_pin), gpios);
static const struct gpio_dt_spec bmm_rdy_pin = GPIO_DT_SPEC_GET(DT_NODELABEL(bmm_rdy_pin), gpios);
static const struct gpio_dt_spec bh_int_pin = GPIO_DT_SPEC_GET(DT_NODELABEL(bh_int_pin), gpios);
static const struct gpio_dt_spec sens_pwr_ctrl_pin = GPIO_DT_SPEC_GET(DT_NODELABEL(sens_pwr_ctrl_pin), gpios);

/*****************************************************************/

/** LOCALS (m_xx) **/
LOG_MODULE_REGISTER(bsp_gpio, CONFIG_LOG_DEFAULT_LEVEL);
/*****************************************************************/

/** FUNCTIONS (int bsp_xx()) **/

/**
 * @brief Initialize GPIO pins
 * @return int 
 */
int bsp_gpio_init(void)
{
    LOG_INF("GPIO initialization started");

    // Initialize GPIO pins for FEM control
    MAC_CONFIG_GPIO_OR_RETURN(fem_mode_pin, GPIO_OUTPUT_ACTIVE);
    MAC_CONFIG_GPIO_OR_RETURN(fem_rx_en_pin, GPIO_OUTPUT_ACTIVE);
    MAC_CONFIG_GPIO_OR_RETURN(fem_tx_en_pin, GPIO_OUTPUT_ACTIVE);
    MAC_CONFIG_GPIO_OR_RETURN(fem_sel_pin, GPIO_OUTPUT_ACTIVE);
    
    // Power control pin
    MAC_CONFIG_GPIO_OR_RETURN(pmic_iset_pin, GPIO_OUTPUT_ACTIVE);
    MAC_CONFIG_GPIO_OR_RETURN(pmic_err_pin, GPIO_INPUT);
    MAC_CONFIG_GPIO_OR_RETURN(pmic_chg_pin, GPIO_INPUT);

    MAC_CONFIG_GPIO_OR_RETURN(pwr_3v3_en_pin, GPIO_OUTPUT_ACTIVE);
    MAC_CONFIG_GPIO_OR_RETURN(pwr_sens_pwr_ctrl_pin, GPIO_OUTPUT_ACTIVE);

    MAC_CONFIG_GPIO_OR_RETURN(batt_measure_en_pin, GPIO_OUTPUT_ACTIVE);


    // ADXL362 : Low power accelerometer
    MAC_CONFIG_GPIO_OR_RETURN(adxl_int_pin, GPIO_INT_EDGE_RISING);
    MAC_CONFIG_GPIO_OR_RETURN(adxl_cs_pin, GPIO_OUTPUT_ACTIVE);
    // BMI270@0x68 : Accelerometer and gyroscope
    MAC_CONFIG_GPIO_OR_RETURN(bmi_int_pin, GPIO_INT_EDGE_RISING);
    MAC_CONFIG_GPIO_OR_RETURN(bmi_cs_pin, GPIO_OUTPUT_ACTIVE);
    // BMM150@0x10 : Magnetometer
    MAC_CONFIG_GPIO_OR_RETURN(bmm_int_pin, GPIO_INT_EDGE_RISING);
    MAC_CONFIG_GPIO_OR_RETURN(bmm_rdy_pin, GPIO_INPUT);
    // BME688@0x76 : Humidity, temperature, pressure
    // BH1749NUC@0x38 : Color sensor
    MAC_CONFIG_GPIO_OR_RETURN(bh_int_pin, GPIO_INT_EDGE_RISING);
    
    // Buzzer

    // FSA2466UMX : Accelerometer disconnect switch
    MAC_CONFIG_GPIO_OR_RETURN(sens_pwr_ctrl_pin, GPIO_OUTPUT_ACTIVE);
    // VM3011@0x60 : Microphone

    // External Flash memory : MX25R6435F

    // Buttons

    return 0;
}
