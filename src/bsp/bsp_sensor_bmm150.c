/*
 * @file : bsp_sensor_bmm150.c
 *
 * @brief : BMM150 geomagnetic magnetometer sensor driver implementation with trigger mode
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
#undef CONFIG_BMM150_TRIGGER
/*****************************************************************/

/** EXTERNS (extern xx) **/
extern BSP_ST g_Bsp;
/*****************************************************************/

/** STATICS (static xx) **/

/*****************************************************************/

/** LOCALS (m_xx) **/
LOG_MODULE_REGISTER(bsp_bmm150, LOG_LEVEL_INF);
/*****************************************************************/

/** FUNCTIONS (int bsp_xx()) **/

#ifdef CONFIG_BMM150_TRIGGER
static void bmm150_trigger_handler(const struct device *dev,
                                   const struct sensor_trigger *trigger)
{
    struct sensor_value magn[3];
    int ret;

    ret = sensor_sample_fetch(dev);
    if (ret < 0)
    {
        LOG_ERR("BMM150 trigger fetch failed: %d", ret);
        return;
    }

    sensor_channel_get(dev, SENSOR_CHAN_MAGN_XYZ, magn);

    LOG_INF("[TRIG BMM150] Magn: X=%d.%06d, Y=%d.%06d, Z=%d.%06d uT",
            magn[0].val1, abs(magn[0].val2),
            magn[1].val1, abs(magn[1].val2),
            magn[2].val1, abs(magn[2].val2));
}
#endif /* CONFIG_BMM150_TRIGGER */

/**
 * @brief Initialize the BMM150 sensor driver
 * 
 * @return int 
 */
int bsp_sensor_bmm150_init(void)
{
    const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(bmm150));

    if (!device_is_ready(dev))
    {
        LOG_ERR("BMM150 device not ready");
        return -ENODEV;
    }
    LOG_INF("BMM150 device found: %s", dev->name);

#ifdef CONFIG_BMM150_TRIGGER
    struct sensor_trigger trig = {
        .type = SENSOR_TRIG_DATA_READY,
        .chan = SENSOR_CHAN_MAGN_XYZ,
    };

    int ret = sensor_trigger_set(dev, &trig, bmm150_trigger_handler);
    if (ret < 0)
    {
        LOG_ERR("Failed to set BMM150 trigger: %d", ret);
        return ret;
    }
    LOG_INF("BMM150 trigger/interrupt mode enabled successfully.");
#else
    LOG_INF("Polling mode active — use 'sensor bmm150 read' to fetch.");
#ifdef BMM150_INTERRUPT_MODE

#else
    LOG_INF("Polling loop active");
    // while (1)
    // {
    //     struct sensor_value magn[3];
    //     ret = bsp_sensor_bmm150_read(magn);
    //     if (ret < 0)
    //     {
    //         LOG_ERR("BMM150 read error: %d", ret);
    //     }
    //     else
    //     {
    //         LOG_INF("BMM150 Magn: X=%d.%06d, Y=%d.%06d, Z=%d.%06d uT",
    //                 magn[0].val1, abs(magn[0].val2),
    //                 magn[1].val1, abs(magn[1].val2),
    //                 magn[2].val1, abs(magn[2].val2));
    //     }
    //     k_sleep(K_MSEC(1000));
    // }
#endif
#endif
    return 0;
}

/**
 * @brief Reads the BMM150 accelerometer
 * 
 * @param magn 
 * @return int 
 */
int bsp_sensor_bmm150_read(struct sensor_value *magn)
{
    const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(bmm150));

    if (!device_is_ready(dev))
    {
        return -ENODEV;
    }

    int ret = sensor_sample_fetch(dev);
    if (ret < 0)
    {
        return ret;
    }

    sensor_channel_get(dev, SENSOR_CHAN_MAGN_XYZ, magn);

    return 0;
}
