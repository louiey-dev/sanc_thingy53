/*
 * @file : bsp_sensor_bmi270.c
 *
 * @brief : BMI270 accelerometer and gyroscope sensor driver implementation via SPI with trigger mode
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
#undef CONFIG_BMI270_TRIGGER
/*****************************************************************/

/** EXTERNS (extern xx) **/
extern BSP_ST g_Bsp;

/* Manually declare Zephyr driver internal register write function */
int bmi270_reg_write(const struct device *dev, uint8_t reg, const uint8_t *data, uint16_t length);
/*****************************************************************/

/** STATICS (static xx) **/

/*****************************************************************/

/** LOCALS (m_xx) **/
LOG_MODULE_REGISTER(bsp_bmi270, LOG_LEVEL_INF);
/*****************************************************************/

/** FUNCTIONS (int bsp_xx()) **/

#ifdef CONFIG_BMI270_TRIGGER
static void bmi270_trigger_handler(const struct device *dev,
                                   const struct sensor_trigger *trigger)
{
    struct sensor_value accel[3];
    struct sensor_value gyro[3];

    int ret = sensor_sample_fetch(dev);
    if (ret < 0)
    {
        LOG_ERR("BMI270 trigger fetch failed: %d", ret);
        return;
    }

    sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    sensor_channel_get(dev, SENSOR_CHAN_GYRO_XYZ, gyro);

    LOG_INF("[TRIG] Accel: X=%d.%06d, Y=%d.%06d, Z=%d.%06d m/s²",
            accel[0].val1, abs(accel[0].val2),
            accel[1].val1, abs(accel[1].val2),
            accel[2].val1, abs(accel[2].val2));
    LOG_INF("[TRIG] Gyro:  X=%d.%06d, Y=%d.%06d, Z=%d.%06d rad/s",
            gyro[0].val1, abs(gyro[0].val2),
            gyro[1].val1, abs(gyro[1].val2),
            gyro[2].val1, abs(gyro[2].val2));
}
#endif /* CONFIG_BMI270_TRIGGER */

/**
 * @brief Initize the BMI270 sensor
 * 
 * @return int 
 */
int bsp_sensor_bmi270_init(void)
{
    const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(bmi270));

    if (!device_is_ready(dev))
    {
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
    if (ret < 0)
    {
        LOG_ERR("Failed to set BMI270 accelerometer frequency: %d", ret);
        return ret;
    }

    // Set sampling frequency to enable gyroscope (200 Hz)
    attr.val1 = 200;
    ret = sensor_attr_set(dev, SENSOR_CHAN_GYRO_XYZ, SENSOR_ATTR_SAMPLING_FREQUENCY, &attr);
    if (ret < 0)
    {
        LOG_ERR("Failed to set BMI270 gyroscope frequency: %d", ret);
        return ret;
    }

    LOG_INF("BMI270 sampling enabled (Accel=100Hz, Gyro=200Hz)");

#ifdef CONFIG_BMI270_TRIGGER
    // Register trigger for Data Ready interrupts
    struct sensor_trigger trig = {
        .type = SENSOR_TRIG_DATA_READY,
        .chan = SENSOR_CHAN_ACCEL_XYZ,
    };

    ret = sensor_trigger_set(dev, &trig, bmi270_trigger_handler);
    if (ret < 0)
    {
        LOG_ERR("Failed to register BMI270 data-ready trigger: %d", ret);
        return ret;
    }

    // Re-route DRDY from INT2 to INT1 pin (which is physically connected to P0.23)
    // INT_MAP_DATA register = 0x58
    // DRDY to INT1 value = BIT(2) = 0x04
    uint8_t int_map_data = 0x04;
    ret = bmi270_reg_write(dev, 0x58, &int_map_data, 1);
    if (ret < 0)
    {
        LOG_ERR("Failed to map BMI270 DRDY interrupt to INT1: %d", ret);
        return ret;
    }

    LOG_INF("BMI270 interrupt/trigger mode enabled successfully.");
#else
    LOG_INF("Polling mode active — use 'sensor bmi270 read' command to fetch.");
#endif

    return 0;
}

/**
 * @brief Read the BMI270 accelerometer and gyroscope
 * 
 * @param accel 
 * @param gyro 
 * @return int 
 */
int bsp_sensor_bmi270_read(struct sensor_value *accel, struct sensor_value *gyro)
{
    const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(bmi270));

    if (!device_is_ready(dev))
    {
        return -ENODEV;
    }

    int ret = sensor_sample_fetch(dev);
    if (ret < 0)
    {
        return ret;
    }

    sensor_channel_get(dev, SENSOR_CHAN_ACCEL_XYZ, accel);
    sensor_channel_get(dev, SENSOR_CHAN_GYRO_XYZ, gyro);

    g_Bsp.sensor.accX = sensor_value_to_float(&accel[0]);
    g_Bsp.sensor.accY = sensor_value_to_float(&accel[1]);
    g_Bsp.sensor.accZ = sensor_value_to_float(&accel[2]);
    g_Bsp.sensor.gyroX = sensor_value_to_float(&gyro[0]);
    g_Bsp.sensor.gyroY = sensor_value_to_float(&gyro[1]);
    g_Bsp.sensor.gyroZ = sensor_value_to_float(&gyro[2]);

    return 0;
}