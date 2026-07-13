/*
 * @file : bsp_sensor_bmm150.c
 * 
 * @brief : BMM150 geomagnetic magnetometer sensor driver implementation
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
#define BMM150_INTERRUPT_MODE
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

int bsp_sensor_bmm150_init(void)
{
    const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(bmm150));

    if (!device_is_ready(dev)) {
        LOG_ERR("BMM150 device not ready");
        return -ENODEV;
    }
    LOG_INF("BMM150 device found: %s", dev->name);
#ifdef BMM150_INTERRUPT_MODE

#else
    LOG_INF("Polling mode — reading every 1000 ms");

    while (1) {
        struct sensor_value magn[3];
        int ret = bsp_sensor_bmm150_read(magn);
        if (ret < 0) {
            LOG_ERR("BMM150 read error: %d", ret);
        } else {
            LOG_INF("BMM150 Magn: X=%d.%06d, Y=%d.%06d, Z=%d.%06d uT", 
                    magn[0].val1, abs(magn[0].val2),
                    magn[1].val1, abs(magn[1].val2),
                    magn[2].val1, abs(magn[2].val2));
        }
        k_sleep(K_MSEC(1000));
    }
#endif
    return 0;
}

int bsp_sensor_bmm150_read(struct sensor_value *magn)
{
    const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(bmm150));

    if (!device_is_ready(dev)) {
        return -ENODEV;
    }

    int ret = sensor_sample_fetch(dev);
    if (ret < 0) {
        return ret;
    }

    sensor_channel_get(dev, SENSOR_CHAN_MAGN_XYZ, magn);

    return 0;
}
