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
#include "bsp.h"

/** DEFINES (#define xx) **/
#define ADC_CHANNEL_ID 2
#define ADC_RESOLUTION 12
/*****************************************************************/

/** EXTERNS (extern xx) **/
extern BSP_ST g_Bsp;
/*****************************************************************/

/** STATICS (static xx) **/
static const struct device *adc_dev = DEVICE_DT_GET(DT_NODELABEL(adc));
/*****************************************************************/

/** LOCALS (m_xx) **/
LOG_MODULE_REGISTER(bsp_adc, CONFIG_LOG_DEFAULT_LEVEL);
/*****************************************************************/

/** FUNCTIONS (int bsp_xx()) **/

// Retrieve the ADC node spec

int bsp_read_battery_voltage(int32_t *raw_val_out)
{
    int16_t sample_buffer;
    
    struct adc_sequence sequence = {
        .channels    = BIT(ADC_CHANNEL_ID),
        .buffer      = &sample_buffer,
        .buffer_size = sizeof(sample_buffer),
        .resolution  = ADC_RESOLUTION,
    };

    int err = adc_read(adc_dev, &sequence);
    if (err < 0) {
        LOG_ERR("ADC read failed: %d", err);
        return err;
    }

    *raw_val_out = sample_buffer;
    return 0;
}
