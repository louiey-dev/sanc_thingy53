/*
 * @file : shell_sensors.c
 * 
 * @brief : 
 * 
 * @author : louiey, louiey@thountech.com
 * 
 * @date : 2026-07-13
 * @copyright : Copyright (c) 2026
 *
 * @section : History
 *            <Date>   <Description of Changes>
 */
#include <stdlib.h>
#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/i2c.h>
#include "bsp.h"

static void sensor_status_print(const struct shell *sh)
{
    shell_print(sh, "--- SENSOR BUS STATUS ---");
    shell_print(sh, "3.3V Power status:    %s", bsp_board_power_3v3_status() ? "ON" : "OFF");
    shell_print(sh, "Sensor Power (P0.31): %s", bsp_sensor_power_status() ? "ON" : "OFF");

    const struct device *reg_sens = DEVICE_DT_GET(DT_NODELABEL(sensor_pwr_ctrl));
    const struct device *reg_3v3 = DEVICE_DT_GET(DT_NODELABEL(regulator_3v3));
    shell_print(sh, "Regulator sensor_pwr_ctrl ready: %s", device_is_ready(reg_sens) ? "YES" : "NO");
    shell_print(sh, "Regulator regulator_3v3 ready:   %s", device_is_ready(reg_3v3) ? "YES" : "NO");

    shell_print(sh, "Device adxl362 ready: %s", device_is_ready(DEVICE_DT_GET(DT_NODELABEL(adxl362))) ? "YES" : "NO");
    shell_print(sh, "Device bh1749 ready:  %s", device_is_ready(DEVICE_DT_GET(DT_NODELABEL(bh1749))) ? "YES" : "NO");
    shell_print(sh, "Device bme688 ready:  %s", device_is_ready(DEVICE_DT_GET(DT_NODELABEL(bme688))) ? "YES" : "NO");
    shell_print(sh, "Device bmi270 ready:  %s", device_is_ready(DEVICE_DT_GET(DT_NODELABEL(bmi270))) ? "YES" : "NO");
    shell_print(sh, "Device bmm150 ready:  %s", device_is_ready(DEVICE_DT_GET(DT_NODELABEL(bmm150))) ? "YES" : "NO");

    const struct device *i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c1));
    bool i2c_ready = device_is_ready(i2c_dev);
    shell_print(sh, "I2C1 Device ready:    %s", i2c_ready ? "YES" : "NO");

    if (i2c_ready) {
        shell_print(sh, "Scanning I2C1 bus...");
        int found = 0;
        for (uint8_t addr = 0x08; addr <= 0x77; addr++) {
            struct i2c_msg msgs[1];
            uint8_t dst;
            msgs[0].buf = &dst;
            msgs[0].len = 0;
            msgs[0].flags = I2C_MSG_WRITE | I2C_MSG_STOP;

            int ret = i2c_transfer(i2c_dev, msgs, 1, addr);
            if (ret == 0) {
                shell_print(sh, "  Found device at address 0x%02X", addr);
                found++;
            }
        }
        shell_print(sh, "I2C Scan complete: %d device(s) found.", found);
    }
}

/** DEFINES (#define xx) **/

/*****************************************************************/

/** EXTERNS (extern xx) **/
extern BSP_ST g_Bsp;
/*****************************************************************/

/** STATICS (static xx) **/

/*****************************************************************/

/** LOCALS (m_xx) **/

/*****************************************************************/

/** FUNCTIONS (int bsp_xx()) **/

static int cmd_sensors(const struct shell *sh, size_t argc, char **argv)
{
    int ret = 0;

    // For pirnts usage
    if (argc < 2) {
        shell_error(sh, "Usage: sensor adxl362/bh1749/bme688/bmi270/bmm150/status");
        return -EINVAL;
    }
    
    if(strcmp(argv[1], "status") == 0) {
        sensor_status_print(sh);
        return 0;
    } else if(strcmp(argv[1], "adxl362") == 0 ) {
        if (argc < 3) {
            shell_error(sh, "Usage: sensor adxl362 init/read");
            return -EINVAL;
        }
        if(strcmp(argv[2], "init") == 0) {
            ret = bsp_sensor_adxl362_init();
        } else if(strcmp(argv[2], "read") == 0) {
            struct sensor_value accel[3];
            ret = bsp_sensor_adxl362_read(accel);
            if (ret < 0) {
                shell_error(sh, "Read error: %d", ret);
            } else {
                shell_print(sh, "Accel: X=%d.%06d, Y=%d.%06d, Z=%d.%06d m/s²",
                            accel[0].val1, abs(accel[0].val2),
                            accel[1].val1, abs(accel[1].val2),
                            accel[2].val1, abs(accel[2].val2));
            }
        }
    } else if(strcmp(argv[1], "bh1749") == 0) {
        if (argc < 3) {
            shell_error(sh, "Usage: sensor bh1749 init/read");
            return -EINVAL;
        }
        if(strcmp(argv[2], "init") == 0) {
            ret = bsp_sensor_bh1749_init();
        } else if(strcmp(argv[2], "read") == 0) {
            struct sensor_value red, green, blue, ir;
            ret = bsp_sensor_bh1749_read(&red, &green, &blue, &ir);
            if (ret < 0) {
                shell_error(sh, "Read error: %d", ret);
            } else {
                shell_print(sh, "Red: %d, Green: %d, Blue: %d, IR: %d",
                            red.val1, green.val1, blue.val1, ir.val1);
            }
        }
    } else if(strcmp(argv[1], "bme688") == 0) {
        if (argc < 3) {
            shell_error(sh, "Usage: sensor bme688 init/read");
            return -EINVAL;
        }
        if(strcmp(argv[2], "init") == 0) {
            ret = bsp_sensor_bme688_init();
        } else if(strcmp(argv[2], "read") == 0) {
            struct sensor_value temp, press, hum, gas;
            ret = bsp_sensor_bme688_read(&temp, &press, &hum, &gas);
            if (ret < 0) {
                shell_error(sh, "Read error: %d", ret);
            } else {
                shell_print(sh, "Temp: %d.%06d °C, Press: %d.%06d kPa, Hum: %d.%06d %%, Gas: %d ohm",
                            temp.val1, abs(temp.val2),
                            press.val1, abs(press.val2),
                            hum.val1, abs(hum.val2),
                            gas.val1);
            }
        }
    } else if(strcmp(argv[1], "bmi270") == 0) {
        if (argc < 3) {
            shell_error(sh, "Usage: sensor bmi270 init/read");
            return -EINVAL;
        }
        if(strcmp(argv[2], "init") == 0) {
            ret = bsp_sensor_bmi270_init();
        } else if(strcmp(argv[2], "read") == 0) {
            struct sensor_value accel[3];
            struct sensor_value gyro[3];
            ret = bsp_sensor_bmi270_read(accel, gyro);
            if (ret < 0) {
                shell_error(sh, "Read error: %d", ret);
            } else {
                shell_print(sh, "Accel: X=%d.%06d, Y=%d.%06d, Z=%d.%06d m/s²",
                            accel[0].val1, abs(accel[0].val2),
                            accel[1].val1, abs(accel[1].val2),
                            accel[2].val1, abs(accel[2].val2));
                shell_print(sh, "Gyro: X=%d.%06d, Y=%d.%06d, Z=%d.%06d rad/s",
                            gyro[0].val1, abs(gyro[0].val2),
                            gyro[1].val1, abs(gyro[1].val2),
                            gyro[2].val1, abs(gyro[2].val2));
            }
        }
    } else if(strcmp(argv[1], "bmm150") == 0) {
        if (argc < 3) {
            shell_error(sh, "Usage: sensor bmm150 init/read");
            return -EINVAL;
        }
        if(strcmp(argv[2], "init") == 0) {
            ret = bsp_sensor_bmm150_init();
        } else if(strcmp(argv[2], "read") == 0) {
            struct sensor_value magn[3];
            ret = bsp_sensor_bmm150_read(magn);
            if (ret < 0) {
                shell_error(sh, "Read error: %d", ret);
            } else {
                shell_print(sh, "Magn: X=%d.%06d, Y=%d.%06d, Z=%d.%06d uT",
                            magn[0].val1, abs(magn[0].val2),
                            magn[1].val1, abs(magn[1].val2),
                            magn[2].val1, abs(magn[2].val2));
            }
        }
    } else {
        shell_error(sh, "Invalid command. Use 'adxl362/bh1749/bme688/bmi270/bmm150/status'");
        return -EINVAL;
    }
    return 0;
}

// Register command: "adc <offset>"
SHELL_CMD_REGISTER(sensor, NULL, "Sensors control", cmd_sensors);
