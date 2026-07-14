/*
 * @file : bsp_adc.c
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
#include <zephyr/drivers/adc.h>
#include <zephyr/drivers/gpio.h>
#include "bsp.h"

/** DEFINES (#define xx) **/
#define ADC_CHANNEL_ID 2
#define ADC_RESOLUTION 12

#define VBATT DT_PATH(vbatt)
#define ZEPHYR_USER DT_PATH(zephyr_user)
/*****************************************************************/

/** EXTERNS (extern xx) **/
extern BSP_ST g_Bsp;
/*****************************************************************/

/** STATICS (static xx) **/
// static const struct device *adc_dev = DEVICE_DT_GET(DT_NODELABEL(adc));

static const struct gpio_dt_spec m_powerGpio = GPIO_DT_SPEC_GET(VBATT, power_gpios);
static const struct gpio_dt_spec m_chargeGpio = GPIO_DT_SPEC_GET(ZEPHYR_USER, battery_charge_gpios);
static const struct gpio_dt_spec m_errGpio = GPIO_DT_SPEC_GET(ZEPHYR_USER, battery_err_gpios);
static const uint32_t m_fullOhms = DT_PROP(VBATT, full_ohms);
static const uint32_t m_outputOhms = DT_PROP(VBATT, output_ohms);
static const struct adc_dt_spec m_adc = ADC_DT_SPEC_GET(VBATT);

static int16_t m_adcBuffer = 0;
/*****************************************************************/

/** LOCALS (m_xx) **/
LOG_MODULE_REGISTER(bsp_adc, CONFIG_LOG_DEFAULT_LEVEL);

struct adc_sequence m_adcSeq = {
	.buffer = &m_adcBuffer,
	.buffer_size = sizeof(m_adcBuffer),
	.calibrate = true,
};
/*****************************************************************/

/** FUNCTIONS (int bsp_xx()) **/

/**
 * @brief Initialize adc for battery reading
 * @return int 0 : success, !0 : fail
 */
int bsp_adc_init(void)
{
    int err = 0;

	if (!device_is_ready(m_powerGpio.port)) {
		LOG_ERR("Battery measurement GPIO device not ready");
		return -ENODEV;
	}

	err += gpio_pin_configure_dt(&m_powerGpio, GPIO_OUTPUT_INACTIVE);
	if (err != 0) {
		LOG_ERR("Failed to configure battery measurement GPIO %d", err);
		return err;
	}

	if (!device_is_ready(m_adc.dev)) {
		LOG_ERR("ADC controller not ready");
		return -ENODEV;
	}

	err += adc_channel_setup_dt(&m_adc);
	if (err) {
		LOG_ERR("Setting up the ADC channel failed");
		return err;
	}

	(void)adc_sequence_init_dt(&m_adc, &m_adcSeq);

    err += gpio_pin_set_dt(&m_powerGpio, 1);
    if (err != 0) {
        LOG_ERR("Failed to enable measurement pin %d", err);
    }

	if (!device_is_ready(m_chargeGpio.port)) {
		LOG_ERR("Charge GPIO controller not ready");
		return -ENODEV;
	}

	if (!device_is_ready(m_errGpio.port)) {
		LOG_ERR("ERR GPIO controller not ready");
		return -ENODEV;
	}

	err += gpio_pin_configure_dt(&m_chargeGpio, GPIO_INPUT);
	if (err != 0) {
		LOG_ERR("Failed to configure battery charge GPIO %d", err);
		return err;
	}

	err += gpio_pin_configure_dt(&m_errGpio, GPIO_INPUT);
	if (err != 0) {
		LOG_ERR("Failed to configure battery err GPIO %d", err);
		return err;
	}
	LOG_INF("ADC Init OK");
	return 0;
}

/**
 * @brief Read ADC value and convert to millivolts
 * @param mv_val_out 
 * @return int 0 : Success, -1 : Failure 
 */
int bsp_adc_battery_mv(int32_t *mv_val_out)
{
	int32_t result = 0;
	bsp_gpio_battery_control(1);
    int ret = adc_read(m_adc.dev, &m_adcSeq);
	bsp_gpio_battery_control(0);
    if (ret == 0) {
        int32_t val = m_adcBuffer;
        adc_raw_to_millivolts_dt(&m_adc, &val);
        result = (int32_t)((int64_t)(val) * m_fullOhms / m_outputOhms);
    }else{
        LOG_ERR("ADC Fail = %d\n",ret);
		*mv_val_out = 0;
		return -1;
    }
	*mv_val_out = result;
	LOG_INF("ADC mV : %d", *mv_val_out);
	return 0;
}

/**
 * @brief Read ADC raw value and convert to millivolts
 * @param raw_val_out 
 * @return int 0 : Success, -1 : Failure  
 */
int bsp_adc_battery_raw(int32_t *raw_val_out)
{
	bsp_gpio_battery_control(1);
    int ret = adc_read(m_adc.dev, &m_adcSeq);
	bsp_gpio_battery_control(0);
    if (ret == 0) {
        *raw_val_out = m_adcBuffer;
    }else{
        LOG_ERR("ADC Fail = %d\n",ret);
		*raw_val_out = 0;
		return -1;
    }
	LOG_INF("ADC raw : %d", *raw_val_out);
	return 0;
}

/**
 * @brief read battery charge status
 * 
 * @return 0 : charging status, 1 : non-charging status
 */
int bsp_adc_battery_charging(void)
{
	int ret = gpio_pin_get_dt(&m_chargeGpio);
	if (ret == 0){
		LOG_INF("Battery charging status");
		return 0;
	}
	else
	{
		LOG_INF("Battery no charging status");
		return 1;
	}
}

/**
 * @brief read battery error status
 * 
 * @return int 0 : charger error, 1 : charger no error
 */
int bsp_adc_battery_err(void)
{
	int ret = gpio_pin_get_dt(&m_errGpio);
	if (ret == 0){
		LOG_INF("Battery charger error");
		return 0;
	}
	else
	{
		LOG_INF("Battery charger no error");
		return 1;
	}
}
