/*
 * @file : bsp_sensor_adxl362.c
 *
 * @brief : Interrupt mode is not available. PCB NET is disconnected for both INT lines
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

/** DEFINES (#define xx) **/
#define ADXL362_INTERRUPT_MODE
/*****************************************************************/

/** EXTERNS (extern xx) **/
extern BSP_ST g_Bsp;
/*****************************************************************/

/** STATICS (static xx) **/

/*****************************************************************/

/** LOCALS (m_xx) **/
LOG_MODULE_REGISTER(bsp_adxl362, LOG_LEVEL_INF);
/*****************************************************************/

/** FUNCTIONS (int bsp_xx()) **/

#ifdef ADXL362_INTERRUPT_MODE
static void adxl362_trigger_handler(const struct device *dev,
									const struct sensor_trigger *trigger)
{
	struct sensor_value accel[3];

	int ret = sensor_sample_fetch(dev);
	if (ret < 0)
	{
		LOG_ERR("Fetch failed: %d", ret);
		return;
	}

	sensor_channel_get(dev, SENSOR_CHAN_ACCEL_X, &accel[0]);
	sensor_channel_get(dev, SENSOR_CHAN_ACCEL_Y, &accel[1]);
	sensor_channel_get(dev, SENSOR_CHAN_ACCEL_Z, &accel[2]);

	LOG_INF("[TRIG] X: %d.%06d  Y: %d.%06d  Z: %d.%06d m/s²",
			accel[0].val1, abs(accel[0].val2),
			accel[1].val1, abs(accel[1].val2),
			accel[2].val1, abs(accel[2].val2));
}
#endif /* CONFIG_ADXL362_TRIGGER */

/**
 * @brief Initialize the ADXL362 sensor with interrupt handler
 *
 * @return int
 */
int bsp_sensor_adxl362_init(void)
{
	int ret;
	const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(adxl362));

	if (!device_is_ready(dev))
	{
		LOG_ERR("ADXL362 device not ready");
		return -ENODEV;
	}
	LOG_INF("ADXL362 device found: %s", dev->name);

	/* Optional: set output data rate */
	struct sensor_value odr = {.val1 = 100, .val2 = 0}; /* 100 Hz */
	ret = sensor_attr_set(dev, SENSOR_CHAN_ACCEL_XYZ,
						  SENSOR_ATTR_SAMPLING_FREQUENCY, &odr);
	if (ret < 0)
	{
		LOG_WRN("Could not set ODR: %d (continuing with default)", ret);
	}

#ifdef ADXL362_INTERRUPT_MODE
#if 0
	/* Set up data-ready trigger */
	struct sensor_trigger trig = {
		.type = SENSOR_TRIG_DATA_READY,
		.chan = SENSOR_CHAN_ACCEL_XYZ,
	};

	ret = sensor_trigger_set(dev, &trig, adxl362_trigger_handler);
	if (ret < 0) {
		LOG_ERR("Trigger set failed: %d", ret);
		LOG_INF("Falling back to polling mode");
	} else {
		LOG_INF("Trigger mode enabled — data-ready interrupt active");
		/* In trigger mode, just sleep; the callback does the work */
		// while (1) {
		// 	k_sleep(K_FOREVER);
		// }
	}
#endif
#else
	/* Polling mode */
	LOG_INF("Polling mode — reading every %d ms", ADXL362_SAMPLE_INTERVAL_MS);

	while (1)
	{
		struct sensor_value accel[3];

		ret = sensor_sample_fetch(dev);
		if (ret < 0)
		{
			LOG_ERR("Fetch error: %d", ret);
		}
		else
		{
			sensor_channel_get(dev, SENSOR_CHAN_ACCEL_X, &accel[0]);
			sensor_channel_get(dev, SENSOR_CHAN_ACCEL_Y, &accel[1]);
			sensor_channel_get(dev, SENSOR_CHAN_ACCEL_Z, &accel[2]);

			LOG_INF("X: %d.%06d  Y: %d.%06d  Z: %d.%06d m/s²",
					accel[0].val1, abs(accel[0].val2),
					accel[1].val1, abs(accel[1].val2),
					accel[2].val1, abs(accel[2].val2));
		}

		k_sleep(K_MSEC(ADXL362_SAMPLE_INTERVAL_MS));
	}
#endif
	return 0;
}

int bsp_sensor_adxl362_read(struct sensor_value *accel)
{
	const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(adxl362));

	if (!device_is_ready(dev))
	{
		return -ENODEV;
	}

	int ret = sensor_sample_fetch(dev);
	if (ret < 0)
	{
		return ret;
	}

	sensor_channel_get(dev, SENSOR_CHAN_ACCEL_X, &accel[0]);
	sensor_channel_get(dev, SENSOR_CHAN_ACCEL_Y, &accel[1]);
	sensor_channel_get(dev, SENSOR_CHAN_ACCEL_Z, &accel[2]);

	return 0;
}
