/*
 * @file : bsp_sensor_bme688.c
 * 
 * @brief : BME688 environmental sensor driver implementation
 * 
 * @author : louiey, louiey@thountech.com
 * 
 * @date : 2026-07-13
 * @copyright : Copyright (c) 2026
 *
 * @section : History
 *            <Date>   <Description of Changes>
 */
#include "bsp_sensor.h"
#include "bsp.h"
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <stdlib.h>

/** DEFINES (#define xx) **/
#define BME688_INTERRUPT_MODE
/*****************************************************************/

/** EXTERNS (extern xx) **/
extern BSP_ST g_Bsp;
/*****************************************************************/

/** STATICS (static xx) **/

/*****************************************************************/

/** LOCALS (m_xx) **/
LOG_MODULE_REGISTER(bsp_bme688, LOG_LEVEL_INF);
/*****************************************************************/

/** FUNCTIONS (int bsp_xx()) **/

/**
 * @brief Initialize the BME688 sensor
 * 
 * @return int 
 */
int bsp_sensor_bme688_init(void)
{
    const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(bme688));

    if (!device_is_ready(dev)) {
        LOG_ERR("BME688 device not ready");
        return -ENODEV;
    }
    LOG_INF("BME688 device found: %s", dev->name);
#ifdef BME688_INTERRUPT_MODE

#else
    LOG_INF("Polling mode — reading every 2000 ms");

    while (1) {
        struct sensor_value temp, press, hum, gas;
        int ret = bsp_sensor_bme688_read(&temp, &press, &hum, &gas);
        if (ret < 0) {
            LOG_ERR("BME688 read error: %d", ret);
        } else {
            LOG_INF("BME688: Temp=%d.%06d °C, Press=%d.%06d kPa, Hum=%d.%06d %%, Gas=%d ohm", 
                    temp.val1, abs(temp.val2),
                    press.val1, abs(press.val2),
                    hum.val1, abs(hum.val2),
                    gas.val1);
        }
        k_sleep(K_MSEC(2000));
    }
#endif
    return 0;
}

/**
 * @brief Read BME688 sensor values
 * 
 * @param temp 
 * @param press 
 * @param hum 
 * @param gas 
 * @return int 
 */
int bsp_sensor_bme688_read(struct sensor_value *temp, struct sensor_value *press, struct sensor_value *hum, struct sensor_value *gas)
{
    const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(bme688));

    if (!device_is_ready(dev)) {
        return -ENODEV;
    }

    int ret = sensor_sample_fetch(dev);
    if (ret < 0) {
        return ret;
    }

    sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, temp);
    sensor_channel_get(dev, SENSOR_CHAN_PRESS, press);
    sensor_channel_get(dev, SENSOR_CHAN_HUMIDITY, hum);
    sensor_channel_get(dev, SENSOR_CHAN_GAS_RES, gas);

    return 0;
}
