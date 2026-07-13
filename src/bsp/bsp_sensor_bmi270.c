/*
 * @file : bsp_sensor_bmi270.c
 * 
 * @brief : BMI270 accelerometer and gyroscope sensor driver implementation via SPI
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
#define BMI270_INTERRUPT_MODE
/*****************************************************************/

/** EXTERNS (extern xx) **/
extern BSP_ST g_Bsp;
/*****************************************************************/

/** STATICS (static xx) **/

/*****************************************************************/

/** LOCALS (m_xx) **/
LOG_MODULE_REGISTER(bsp_bmi270, LOG_LEVEL_INF);
/*****************************************************************/

/** FUNCTIONS (int bsp_xx()) **/

int bsp_sensor_bmi270_init(void)
{
    const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(bmi270));

    if (!device_is_ready(dev)) {
        LOG_ERR("BMI270 device not ready");
        return -ENODEV;
    }
    LOG_INF("BMI270 device found: %s", dev->name);

    // Set sampling frequency to enable accelerometer (100 Hz)
    struct sensor_value attr = {
        .val1 = 100,
        .val2 = 0,
    };
    int ret = sensor_attr_set(dev, SENSOR_CHAN_ACCEL_XYZ, SENSOR_ATTR_SAMPLING_FREQUENCY, &attr);
    if (ret < 0) {
        LOG_ERR("Failed to set BMI270 accelerometer frequency: %d", ret);
        return ret;
    }

    // Set sampling frequency to enable gyroscope (200 Hz)
    attr.val1 = 200;
    ret = sensor_attr_set(dev, SENSOR_CHAN_GYRO_XYZ, SENSOR_ATTR_SAMPLING_FREQUENCY, &attr);
    if (ret < 0) {
        LOG_ERR("Failed to set BMI270 gyroscope frequency: %d", ret);
        return ret;
    }

    LOG_INF("BMI270 sampling enabled (Accel=100Hz, Gyro=200Hz)");

#ifdef BMI270_INTERRUPT_MODE

#else
    LOG_INF("Polling mode — reading every 1000 ms");

    while (1) {
        struct sensor_value accel[3];
        struct sensor_value gyro[3];
        ret = bsp_sensor_bmi270_read(accel, gyro);
        if (ret < 0) {
            LOG_ERR("BMI270 read error: %d", ret);
        } else {
            LOG_INF("BMI270 Accel: X=%d.%06d, Y=%d.%06d, Z=%d.%06d m/s²", 
                    accel[0].val1, abs(accel[0].val2),
                    accel[1].val1, abs(accel[1].val2),
                    accel[2].val1, abs(accel[2].val2));
            LOG_INF("BMI270 Gyro: X=%d.%06d, Y=%d.%06d, Z=%d.%06d rad/s", 
                    gyro[0].val1, abs(gyro[0].val2),
                    gyro[1].val1, abs(gyro[1].val2),
                    gyro[2].val1, abs(gyro[2].val2));
        }
        k_sleep(K_MSEC(1000));
    }
#endif
    return 0;
}

int bsp_sensor_bmi270_read(struct sensor_value *accel, struct sensor_value *gyro)
{
    const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(bmi270));

    if (!device_is_ready(dev)) {
        return -ENODEV;
    }

    int ret = sensor_sample_fetch(dev);
    if (ret < 0) {
        return ret;
    }

    sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    sensor_channel_get(dev, SENSOR_CHAN_GYRO_XYZ, gyro);

    return 0;
}