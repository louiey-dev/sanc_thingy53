/*
 * @file : bsp_sensor_bh1749.c
 * 
 * @brief : BH1749 digital color sensor driver implementation
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

/** DEFINES (#define xx) **/
#define BH1749_INTERRUPT_MODE
/*****************************************************************/

/** EXTERNS (extern xx) **/
extern BSP_ST g_Bsp;
/*****************************************************************/

/** STATICS (static xx) **/

/*****************************************************************/

/** LOCALS (m_xx) **/
LOG_MODULE_REGISTER(bsp_bh1749, LOG_LEVEL_INF);
/*****************************************************************/

/** FUNCTIONS (int bsp_xx()) **/

int bsp_sensor_bh1749_init(void)
{
    const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(bh1749));

    if (!device_is_ready(dev)) {
        LOG_ERR("BH1749 device not ready");
        return -ENODEV;
    }
    LOG_INF("BH1749 device found: %s", dev->name);
#ifdef BH1749_INTERRUPT_MODE

#else
    LOG_INF("Polling mode — reading every 1000 ms");

    while (1) {
        struct sensor_value red, green, blue, ir;
        int ret = bsp_sensor_bh1749_read(&red, &green, &blue, &ir);
        if (ret < 0) {
            LOG_ERR("BH1749 read error: %d", ret);
        } else {
            LOG_INF("RGB-IR: R=%d, G=%d, B=%d, IR=%d", 
                    red.val1, green.val1, blue.val1, ir.val1);
        }
        k_sleep(K_MSEC(1000));
    }
#endif    
    return 0;
}

int bsp_sensor_bh1749_read(struct sensor_value *red, struct sensor_value *green, struct sensor_value *blue, struct sensor_value *ir)
{
    const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(bh1749));

    if (!device_is_ready(dev)) {
        LOG_ERR("BH1749 device not ready");
        return -ENODEV;
    }

    int ret = sensor_sample_fetch(dev);
    if (ret < 0) {
        LOG_ERR("BH1749 read error: %d", ret);
        return ret;
    }

    sensor_channel_get(dev, SENSOR_CHAN_RED, red);
    sensor_channel_get(dev, SENSOR_CHAN_GREEN, green);
    sensor_channel_get(dev, SENSOR_CHAN_BLUE, blue);
    sensor_channel_get(dev, SENSOR_CHAN_IR, ir);

    return 0;
}
