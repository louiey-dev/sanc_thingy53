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
#include <zephyr/drivers/pwm.h>
#include <hal/nrf_gpio.h>
#include <zephyr/init.h>
#include "bsp.h"

/**
 * @brief Early sensor power on
 * 
 * @return int 
 */
static int early_sensor_power_on(void)
{
    // Enable 3.3V regulator (P0.15 is active-low)
    nrf_gpio_cfg_output(15);
    nrf_gpio_pin_clear(15);

    // Enable Sensor load switch (P0.31 is active-high)
    nrf_gpio_cfg_output(31);
    nrf_gpio_pin_set(31);

    // Wait 10 ms for power to stabilize and sensors to POR
    k_busy_wait(10000);

    printk("--- EARLY SENSOR POWER ON RUNNING ---\n");
    return 0;
}

SYS_INIT(early_sensor_power_on, PRE_KERNEL_1, 0);

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



static const struct gpio_dt_spec sens_pwr_ctrl_pin = GPIO_DT_SPEC_GET(DT_NODELABEL(sens_pwr_ctrl_pin), gpios);

static const struct gpio_dt_spec button0 = GPIO_DT_SPEC_GET(DT_ALIAS(sw0), gpios);
static const struct gpio_dt_spec button1 = GPIO_DT_SPEC_GET(DT_ALIAS(sw1), gpios);
/*****************************************************************/

/** LOCALS (m_xx) **/
LOG_MODULE_REGISTER(bsp_gpio, CONFIG_LOG_DEFAULT_LEVEL);
/*****************************************************************/

/** CALLBACKS **/
/* Required only if you are using Interrupt-driven configuration (Method B) */
static struct gpio_callback button0_cb_data;
static struct gpio_callback button1_cb_data;
/*****************************************************************/

/** FUNCTIONS (int bsp_xx()) **/

/**
 * @brief Button pressed handler from external INT
 * 
 * @param port 
 * @param cb 
 * @param pins 
 */
static void button_pressed_handler(const struct device *port, struct gpio_callback *cb, gpio_port_pins_t pins)
{
    if (pins & BIT(button0.pin)) {
        LOG_INF("Button 1 (sw0) pressed");
    }
    if (pins & BIT(button1.pin)) {
        LOG_INF("Button 2 (sw1) pressed");
    }
}

/**
 * @brief Initialize GPIO pins
 * @return int 
 */
int bsp_gpio_init(void)
{
    int ret = 0;

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


    // Sensor interrupt pins are managed automatically by their respective drivers.
    // Do not configure them manually here to avoid overwriting their configurations.
    
    // Buzzer

    // FSA2466UMX : Accelerometer disconnect switch
    MAC_CONFIG_GPIO_OR_RETURN(sens_pwr_ctrl_pin, GPIO_OUTPUT_ACTIVE);
    // VM3011@0x60 : Microphone

    // External Flash memory : MX25R6435F

    // Buttons
    MAC_CONFIG_GPIO_OR_RETURN(button0, GPIO_INPUT);
    MAC_CONFIG_GPIO_OR_RETURN(button1, GPIO_INPUT);

    // Configure interrupt trigger (edge to active state)
    ret = gpio_pin_interrupt_configure_dt(&button0, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret < 0) {
        LOG_ERR("Failed to configure interrupt on button0: %d", ret);
        return ret;
    }
    ret = gpio_pin_interrupt_configure_dt(&button1, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret < 0) {
        LOG_ERR("Failed to configure interrupt on button1: %d", ret);
        return ret;
    }

    // Initialize and register callbacks
    gpio_init_callback(&button0_cb_data, button_pressed_handler, BIT(button0.pin));
    gpio_add_callback(button0.port, &button0_cb_data);

    gpio_init_callback(&button1_cb_data, button_pressed_handler, BIT(button1.pin));
    gpio_add_callback(button1.port, &button1_cb_data);
    
    return 0;
}

/**
 * @brief 3v3 voltage enable/disable
 * @param state 
 * @return int 0 : Success, -1 : Failure
 */
int bsp_gpio_3v3_control(int state)
{
    int ret = gpio_pin_set_dt(&pwr_3v3_en_pin, state);
    if (ret < 0)
    {
        LOG_ERR("Failed to set GPIO pin %d", ret);
        return -1;
    }
    LOG_INF("3V3 %s", state ? "ON" : "OFF");
    return ret;
}

/**
 * @brief Controls sensors power
 * 
 * @param state 1 : Sensor VCC on and BMI SPI enable, 0 : Sensor VCC off and BMI SPI disable
 * @return int 
 */
int bsp_gpio_sens_pwr_control(int state)
{
    int ret = gpio_pin_set_dt(&pwr_sens_pwr_ctrl_pin, state);
    if (ret < 0)
    {
        LOG_ERR("Failed to set GPIO pin %d", ret);
        return -1;
    }
    LOG_INF("Sensor power 3V %s", state ? "ON" : "OFF");
    return ret;
}

/**
 * @brief 
 * 
 * @param state 1 : Battery measure on, 0 : Battery measure off
 * @return int 
 */
int bsp_gpio_battery_control(int state)
{
    int ret = gpio_pin_set_dt(&batt_measure_en_pin, state);
    if (ret < 0)
    {
        LOG_ERR("Failed to set GPIO pin %d", ret);
        return -1;
    }
    // LOG_INF("Battery %s", state ? "ON" : "OFF");
    return ret;
}

/**
 * @brief Read sensor power status
 * 
 * @return int 0 : Off, 1 : On
 */
int bsp_sensor_power_status(void)
{
    return gpio_pin_get_dt(&pwr_sens_pwr_ctrl_pin);
}

/**
 * @brief Read board power status
 * 
 * @return int 0 : Off, 1 : On
 */
int bsp_board_power_3v3_status(void)
{
    return gpio_pin_get_dt(&pwr_3v3_en_pin);
}

/**
 * @brief Control FEM IO control
 * 
 * @param mode 0 : fem_mode_pin, 1 : fem_rx_en_pin, 2 : fem_tx_en_pin, 3 : fem_sel_pin
 * @param state 0 : Off, 1 : On
 * @return int 
 */
int bsp_gpio_fem_control(int mode, int state)
{
    const struct gpio_dt_spec *spec;
    char* label = "";

    switch(mode)
    {
        case 0: spec = &fem_mode_pin; label = "fem_mode"; break;
        case 1: spec = &fem_rx_en_pin; label = "fem_rx_en"; break;
        case 2: spec = &fem_tx_en_pin; label = "fem_tx_en"; break;
        case 3: spec = &fem_sel_pin; label = "fem_sel"; break;
        default: LOG_ERR("Invalid FEM mode %d", mode); return -1;
    }
    int ret = gpio_pin_set_dt(spec, state);
    if (ret < 0)
    {
        LOG_ERR("Failed to set FEM pin %d", ret);
        return -1;
    }
    LOG_INF("FEM %s  %s", label, state ? "ON" : "OFF");
    return 0;
}

/**
 * @brief Control PMIC control
 * 
 * @param mode 0 : pmic_iset_pin, 1 : pmic_err_pin, 2 : pmic_chg_pin
 * @param state 0 : Off, 1 : On
 * @return int 
 */
int bsp_gpio_pmic_control(int mode, int state)
{
    const struct gpio_dt_spec *spec;
    char* label = "";

    switch(mode)
    {
        case 0: spec = &pmic_iset_pin; label = "pmic_iset"; break;
        case 1: spec = &pmic_err_pin; label = "pmic_err"; break;
        case 2: spec = &pmic_chg_pin; label = "pmic_chg"; break;
        default: LOG_ERR("Invalid PMIC mode %d", mode); return -1;
    }
    int ret = gpio_pin_set_dt(spec, state);
    if (ret < 0)
    {
        LOG_ERR("Failed to set PMIC pin %d", ret);
        return -1;
    }
    LOG_INF("PMIC %s  %s", label, state ? "ON" : "OFF");
    return 0;
}
