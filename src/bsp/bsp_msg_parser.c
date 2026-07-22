/*
 * @file : bsp_msg_parser.c
 *
 * @brief : Message parser for the BSP
 *
 * @author : louiey, louiey@thountech.com
 *
 * @date : 2026-07-10
 * @copyright : Copyright (c) 2026
 *
 * @section : History
 *            <Date>   <Description of Changes>
 */
#include <stdlib.h>
#include "bsp.h"
#include "bsp_msg_parser.h"
#include <app_version.h>
/** DEFINES (#define xx) **/

/*****************************************************************/

/** EXTERNS (extern xx) **/
extern BSP_ST g_Bsp;
/*****************************************************************/

/** STATICS (static xx) **/
static void msg_parser(uint8_t msg_id, const uint8_t *data, uint8_t len);
/*****************************************************************/

/** LOCALS (m_xx) **/
LOG_MODULE_REGISTER(bsp_msg_parser, CONFIG_LOG_DEFAULT_LEVEL);
/*****************************************************************/

/** FUNCTIONS (int bsp_xx()) **/

/**
 * @brief Parse a message received via NUS.
 * @param msg The message to parse.
 * @param len The length of the message.
 * @return 0 on success, negative error code on failure.
 */
int bsp_msg_parser(const char *msg, size_t len)
{
    if (msg == NULL || len == 0)
    {
        LOG_ERR("Invalid message or length");
        return -EINVAL;
    }
    // Parse the message and extract relevant information
    if (len < sizeof(ble_frame_t))
    {
        LOG_ERR("Invalid message length");
        return -EINVAL;
    }
    //////////////////////////////////////////////////////
    const ble_frame_t *frame = (const ble_frame_t *)msg;
    // LOG_INF("0x%02x, 0x%02x, 0x%02x, 0x%02x", sys_be16_to_cpu(frame->magic_hdr), frame->magic_hdr, MAGIC_WORD, sys_be16_to_cpu(MAGIC_WORD));

    if (sys_be16_to_cpu(frame->magic_hdr) != MAGIC_WORD)
    {
        LOG_ERR("Invalid magic word..0x%04x", frame->magic_hdr);
        return -EINVAL;
    }
    LOG_INF("Total Len : %d, total msg count : %d", frame->total_len, frame->msg_cnt);

    const uint8_t *p = frame->messages;
    const uint8_t *msg_end = (const uint8_t *)msg + len;

    for (int i = 0; i < frame->msg_cnt; i++)
    {
        // Ensure we can safely read the block header (2 bytes)
        if (p + sizeof(ble_msg_block_t) > msg_end)
        {
            LOG_ERR("Incomplete message block header");
            return -EBADMSG;
        }

        const ble_msg_block_t *block = (const ble_msg_block_t *)p;

        // Ensure we can safely read the block payload
        if (p + sizeof(ble_msg_block_t) + block->msg_len > msg_end)
        {
            LOG_ERR("Message block payload exceeds packet bounds");
            return -EBADMSG;
        }

        // 데이터 접근
        msg_parser(block->msg_id, block->msg_data, block->msg_len);

        // 다음 메시지로 포인터 이동 (ID 1바이트 + LEN 1바이트 + 데이터 길이)
        p += sizeof(ble_msg_block_t) + block->msg_len;
    }
    //////////////////////////////////////////////////////

    return 0;
}

/**
 * @brief Parse a BLE message block from Centrals
 * 
 * @param msg_id 
 * @param data 
 * @param len 
 */
static void msg_parser(uint8_t msg_id, const uint8_t *data, uint8_t len)
{
    LOG_INF("\tmsg id : 0x%02x, len : %d, data : %.*s", msg_id, len, len, (const char *)data);
    uint8_t *p = NULL;

    if (len > 0)
    {
        p = malloc(len);
        if (p == NULL)
        {
            LOG_ERR("malloc failed");
            return;
        }
        memcpy(p, data, len);
    }

    if (msg_id >= MSG_MAX)
    {
        LOG_ERR("msg id is invalid, msg_id : 0x%02x", msg_id);
        if (p != NULL)
            free(p);
        return;
    }

    switch (msg_id)
    {
    case MSG_GET_VER:
        bsp_msg_send(MSG_RES_VER, (const uint8_t *)APP_VERSION_STRING, (uint8_t)strlen(APP_VERSION_STRING));
        break;

    case MSG_GET_STATS:
        break;

    case MSG_SET_SENSOR_INIT:
        bsp_sensor_bh1749_init();
        bsp_sensor_bme688_init();
        bsp_sensor_bmi270_init();
        bsp_sensor_bmm150_init();
        g_Bsp.sensor.init = 1;
        g_Bsp.sensor.bh1749_en = 1;
        g_Bsp.sensor.bme688_en = 1;
        g_Bsp.sensor.bmi270_en = 1;
        g_Bsp.sensor.bmm150_en = 1;
        bsp_msg_send(MSG_RES_SENSOR_INIT, NULL, 0);
        break;

    case MSG_SET_SM:
        // g_Bsp.state_machine_en = data[0];
        LOG_INF("Set telemetry: %d", data[0]);
        if (data[0])
            bsp_state_machine_start();
        else
            bsp_state_machine_stop();
        bsp_msg_send(MSG_RES_SM, NULL, 0);
        break;

    case MSG_SET_SM_DURATION:
        break;
    
    case MSG_SET_SENSOR_LOG:
        g_Bsp.isSensorLogEnable = data[0];
        bsp_msg_send(MSG_RES_SENSOR_LOG, NULL, 0);
        LOG_INF("Set sensor log: %s", g_Bsp.isSensorLogEnable? "enabled": "disabled");
        break;
    
    case MSG_SET_CBOR:
        g_Bsp.isCborEnable = data[0];
        bsp_msg_send(MSG_RES_CBOR, NULL, 0);
        LOG_INF("Set cbor: %s", g_Bsp.isCborEnable? "enabled": "disabled");
        break;

    default:
        break;
    }
    if (p != NULL)
        free(p);
}

/**
 * @brief Sends all packets via BLE NUS
 * 
 * @param msg_id message id
 * @param data  pointer of message payload data
 * @param len   message payload length
 * @return int 
 */
int bsp_msg_send(uint8_t msg_id, const uint8_t *data, uint8_t len)
{
    if (len > 0 && data != NULL)
    {
        LOG_INF("[%s] send msg_id : 0x%02x, len : %d, data : %.*s", __func__, msg_id, len, len, (const char *)data);
    }
    else
    {
        LOG_INF("[%s] send msg_id : 0x%02x, len : %d", __func__, msg_id, len);
    }

    if (len > 0 && data == NULL)
    {
        LOG_ERR("bsp_msg_send: data is NULL but len is %u", len);
        return -EINVAL;
    }

#define MAX_PAYLOAD_DATA (MAX_PAYLOAD - sizeof(ble_frame_t) - 2)
    if (len > MAX_PAYLOAD_DATA)
    {
        LOG_ERR("bsp_msg_send: len %u exceeds maximum payload size %u", len, MAX_PAYLOAD_DATA);
        return -EMSGSIZE;
    }

    uint8_t tx_buf[MAX_PAYLOAD] __aligned(4);
    ble_frame_t *frame = (ble_frame_t *)tx_buf;

    frame->magic_hdr = sys_cpu_to_be16(MAGIC_WORD);
    frame->msg_cnt = 1;

    ble_msg_block_t *block = (ble_msg_block_t *)frame->messages;
    block->msg_id = msg_id;
    block->msg_len = len;

    if (len > 0)
    {
        memcpy(block->msg_data, data, len);
    }

    uint8_t total_len = sizeof(ble_frame_t) + 2 + len;
    frame->total_len = total_len;

    bsp_nus_send(tx_buf, total_len);
    return 0;
}
