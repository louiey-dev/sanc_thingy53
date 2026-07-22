/*
 * @file : bsp_state_machine.c
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

#include "bsp.h"
#include "bsp_msg_parser.h"
#include "cbor_msg_encode.h"

/*
    bh1749 : color sensor
    bme688 : pressure/humi/temp/gas sensor
    bmi270 : Accelerometer and gyroscope
    bmm150 : Magnetometer
*/
/** DEFINES (#define xx) **/

/*****************************************************************/

/** EXTERNS (extern xx) **/
extern BSP_ST g_Bsp;
/*****************************************************************/

/** STATICS (static xx) **/
static struct k_work_delayable bsp_sm_work;
static uint32_t m_tick = 0;

static void send_telemetry(char* p);
static void send_cbor(int mode);
/*****************************************************************/

/** LOCALS (m_xx) **/
LOG_MODULE_REGISTER(bsp_sm, LOG_LEVEL_INF);
char telemetry_json[MAX_PAYLOAD] = {0};
/*****************************************************************/

/** FUNCTIONS (int bsp_xx()) **/

/**
 * @brief  BSP state machine work handler
 * 
 * @param work 
 */
static void bsp_state_machine_work_handler(struct k_work *work)
{
    if (!g_Bsp.state_machine_en)
    {
        LOG_INF("BSP State Machine stopped");
        return;
    }

    if (g_Bsp.sensor.init)
    {
        if (g_Bsp.sensor.bh1749_en)
        {
            // Read bh1749 color sensor
            if (bsp_sensor_bh1749_read(&g_Bsp.sensor.red, &g_Bsp.sensor.green, &g_Bsp.sensor.blue, &g_Bsp.sensor.ir) < 0)
                LOG_ERR("bsp_sensor_bh1749_read failed");
        }
        if (g_Bsp.sensor.bme688_en)
        {
            // Read bme688 temp/press/hum/gas sensor
            if (bsp_sensor_bme688_read(&g_Bsp.sensor.temp, &g_Bsp.sensor.press, &g_Bsp.sensor.hum, &g_Bsp.sensor.gas) < 0)
                LOG_ERR("bsp_sensor_bme688_read failed");
        }
        if (g_Bsp.sensor.bmi270_en)
        {
            // Read bmi270 IMU
            if (bsp_sensor_bmi270_read(g_Bsp.sensor.accel, g_Bsp.sensor.gyro) < 0)
                LOG_ERR("bsp_sensor_bmi270_read failed");
        }
        if (g_Bsp.sensor.bmm150_en)
        {
            // Read bmm150 magnetometer
            if (bsp_sensor_bmm150_read(g_Bsp.sensor.magn) < 0)
                LOG_ERR("bsp_sensor_bmm150_read failed");
        }
        bsp_adc_battery_mv(&g_Bsp.sensor.batt_mv);
#if 1
        if(g_Bsp.isCborEnable){
            send_cbor(0);
        }
        else{
            send_telemetry(NULL);
        }
        
#else
        int len = snprintf(telemetry_json, sizeof(telemetry_json),
                           "data: {\"ts\":0,\"seq\":%d,\"d\":{\
\"r\":%d,\"g\":%d,\"b\":%d,\"ir\":%d,\
\"temp\":%d.%01d,\"press\":%d.%01d,\"hum\":%d.%01d,\"gas\":%d,\
\"acc_x\":%d.%01d,\"acc_y\":%d.%01d,\"acc_z\":%d.%01d,\
\"gyro_x\":%d.%01d,\"gyro_y\":%d.%01d,\"gyro_z\":%d.%01d,\
\"mag_x\":%d.%01d,\"mag_y\":%d.%01d,\"mag_z\":%d.%01d}}",
                           m_tick++,
                           g_Bsp.sensor.red.val1, g_Bsp.sensor.green.val1, g_Bsp.sensor.blue.val1, g_Bsp.sensor.ir.val1,
                           g_Bsp.sensor.temp.val1, abs(g_Bsp.sensor.temp.val2), g_Bsp.sensor.press.val1, abs(g_Bsp.sensor.press.val2), g_Bsp.sensor.hum.val1, abs(g_Bsp.sensor.hum.val2), g_Bsp.sensor.gas.val1,
                           g_Bsp.sensor.accel[0].val1, abs(g_Bsp.sensor.accel[0].val2), g_Bsp.sensor.accel[1].val1, abs(g_Bsp.sensor.accel[1].val2), g_Bsp.sensor.accel[2].val1, abs(g_Bsp.sensor.accel[2].val2),
                           g_Bsp.sensor.gyro[0].val1, abs(g_Bsp.sensor.gyro[0].val2), g_Bsp.sensor.gyro[1].val1, abs(g_Bsp.sensor.gyro[1].val2), g_Bsp.sensor.gyro[2].val1, abs(g_Bsp.sensor.gyro[2].val2),
                           g_Bsp.sensor.magn[0].val1, abs(g_Bsp.sensor.magn[0].val2), g_Bsp.sensor.magn[1].val1, abs(g_Bsp.sensor.magn[1].val2), g_Bsp.sensor.magn[2].val1, abs(g_Bsp.sensor.magn[2].val2));
        if (len >= sizeof(telemetry_json))
        {
            LOG_ERR("Telemetry JSON truncated! Formatted len: %d", len);
        }
        else
        {
            bsp_msg_send(MSG_RES_SM, (const uint8_t *)telemetry_json, strlen(telemetry_json));
            LOG_INF("Telemetry JSON %d bytes: %s", len, telemetry_json);
        }
#endif
/******************************/
    }
    else
    {
        LOG_ERR("Sensor monitor is disabled");
    }

    // Reschedule the work for the next period
    k_work_reschedule(&bsp_sm_work, K_MSEC(g_Bsp.sm_duration));
}

int bsp_state_machine_start(void)
{
    static bool sm_init = false;
    if (!sm_init)
    {
        k_work_init_delayable(&bsp_sm_work, bsp_state_machine_work_handler);
        sm_init = true;
    }

    if (g_Bsp.state_machine_en)
    {
        LOG_WRN("BSP State Machine is already running");
        return 0;
    }

    LOG_INF("BSP State Machine Start");
    g_Bsp.state_machine_en = 1;
    m_tick = 0;
    k_work_reschedule(&bsp_sm_work, K_NO_WAIT);
    return 0;
}

int bsp_state_machine_stop(void)
{
    if (!g_Bsp.state_machine_en)
    {
        LOG_WRN("BSP State Machine is not running");
        return 0;
    }

    g_Bsp.state_machine_en = 0;
    k_work_cancel_delayable(&bsp_sm_work);
    LOG_INF("BSP State Machine Stopped");
    return 0;
}

static void send_telemetry(char* p){
    /* make telemetry json format */
    uint8_t offset = 0;
    memset(telemetry_json, 0, sizeof(telemetry_json));

    telemetry_json[offset++] = g_Bsp.sensor.lightRed>>8;
    telemetry_json[offset++] = g_Bsp.sensor.lightRed;
    telemetry_json[offset++] = g_Bsp.sensor.lightGreen>>8;
    telemetry_json[offset++] = g_Bsp.sensor.lightGreen;
    telemetry_json[offset++] = g_Bsp.sensor.lightBlue>>8;
    telemetry_json[offset++] = g_Bsp.sensor.lightBlue;
    telemetry_json[offset++] = g_Bsp.sensor.lightIR>>8;
    telemetry_json[offset++] = g_Bsp.sensor.lightIR;
    
    memcpy(&telemetry_json[offset], &g_Bsp.sensor.temperature, sizeof(float));
    offset += sizeof(float);
    memcpy(&telemetry_json[offset], &g_Bsp.sensor.pressure, sizeof(float));
    offset += sizeof(float);
    memcpy(&telemetry_json[offset], &g_Bsp.sensor.humidity, sizeof(float));
    offset += sizeof(float);
    telemetry_json[offset++] = g_Bsp.sensor.gas_resistance>>24;
    telemetry_json[offset++] = g_Bsp.sensor.gas_resistance>>16;
    telemetry_json[offset++] = g_Bsp.sensor.gas_resistance>>8;
    telemetry_json[offset++] = g_Bsp.sensor.gas_resistance;

    memcpy(&telemetry_json[offset], &g_Bsp.sensor.accX, sizeof(float));
    offset += sizeof(float);
    memcpy(&telemetry_json[offset], &g_Bsp.sensor.accY, sizeof(float));
    offset += sizeof(float);
    memcpy(&telemetry_json[offset], &g_Bsp.sensor.accZ, sizeof(float)); 
    offset += sizeof(float);
    memcpy(&telemetry_json[offset], &g_Bsp.sensor.gyroX, sizeof(float));
    offset += sizeof(float);
    memcpy(&telemetry_json[offset], &g_Bsp.sensor.gyroY, sizeof(float));
    offset += sizeof(float);
    memcpy(&telemetry_json[offset], &g_Bsp.sensor.gyroZ, sizeof(float));
    offset += sizeof(float);
    memcpy(&telemetry_json[offset], &g_Bsp.sensor.magX, sizeof(float));
    offset += sizeof(float);
    memcpy(&telemetry_json[offset], &g_Bsp.sensor.magY, sizeof(float));
    offset += sizeof(float);
    memcpy(&telemetry_json[offset], &g_Bsp.sensor.magZ, sizeof(float));
    offset += sizeof(float);
    
    telemetry_json[offset++] = g_Bsp.sensor.batt_mv>>8;
    telemetry_json[offset++] = g_Bsp.sensor.batt_mv;

    bsp_msg_send(MSG_PKT_PAYLOAD, telemetry_json, offset);

    if(g_Bsp.isSensorLogEnable){
        LOG_INF("temp : %.2f, press : %.2f, hum : %.2f, gas :%d, accel : %.2f, %.2f, %.2f, gyro : %.2f, %.2f, %.2f, magn : %.2f, %.2f, %.2f, r : %d, g : %d, b : %d, ir : %d, batt : %d",
                        (double)g_Bsp.sensor.temperature, (double)g_Bsp.sensor.pressure, (double)g_Bsp.sensor.humidity, g_Bsp.sensor.gas_resistance,
                        (double)g_Bsp.sensor.accX, (double)g_Bsp.sensor.accY, (double)g_Bsp.sensor.accZ,
                        (double)g_Bsp.sensor.gyroX, (double)g_Bsp.sensor.gyroY, (double)g_Bsp.sensor.gyroZ,
                        (double)g_Bsp.sensor.magX, (double)g_Bsp.sensor.magY, (double)g_Bsp.sensor.magZ,
                        g_Bsp.sensor.lightRed, g_Bsp.sensor.lightGreen, g_Bsp.sensor.lightBlue, g_Bsp.sensor.lightIR,
                        g_Bsp.sensor.batt_mv);
    }
}

static void send_cbor(int mode){

}
