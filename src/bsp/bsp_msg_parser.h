/*
 * @file : bsp_msg_parser.h
 *
 * @brief :
 *
 * @author : louiey, louiey@thountech.com
 *
 * @date : 2026-07-15
 * @copyright : Copyright (c) 2026
 */

#ifndef BSP_MSG_PARSER_H
#define BSP_MSG_PARSER_H

#ifdef __cplusplus
extern "C"
{
#endif

    /*
        Frame Format (Total: 244 bytes)
        Field       Size (Bytes)        Offset      Description
        MAGIC_HDR   2                   0           Protocol Signature (e.g., 0xAA55)
        TOTAL_LEN   1                   2           Total frame length (Header to CRC, Max 244)
        MSG_CNT     1                   3           Number of messages
        MESSAGES    Variable            4           Payload (Sequence of Message Blocks)
        CRC         2                   END-1       16-bit CRC of the frame


        Message Block Format (TLV Structure)
        Field       Size (Bytes)        Description
        MSG_ID      1                   Message Identifier
        MSG_LEN     1                   Length of MSG_DATA
        MSG_DATA    N                   Message content (max $244 - 6 - (total\_hdr\_size)$)
    */

    /*
        Message Format

        [Frame Format]
        +-----------+-----------+---------+------------+-------+
        | MAGIC_HDR | TOTAL_LEN | MSG_CNT |  MESSAGES  |  CRC  |
        +-----------+-----------+---------+------------+-------+
        (2 Bytes)   (1 Byte)    (1 Byte)   (N Bytes)  (2 Bytes)

        [MESSAGES: Sequence of TLV Blocks]
        +--------+---------+----------+   +--------+---------+----------+
        | MSG_ID | MSG_LEN | MSG_DATA |...| MSG_ID | MSG_LEN | MSG_DATA |
        +--------+---------+----------+   +--------+---------+----------+
        (1 Byte)  (1 Byte)  (K Bytes)     (1 Byte)  (1 Byte)  (K Bytes)
    */

    /**
     * @brief BLE Message Frame Protocol
     *
     *      Frame Format:
     *      [MAGIC_HDR(2)] [TOTAL_LEN(1)] [MSG_CNT(1)] [MESSAGES(N)] [CRC(2)]
     *
     *      - TOTAL_LEN : Includes Header + Payload + CRC (Max: 244)
     *      - MSG_DATA  : Max Payload = 244 - 6 = 238 Bytes
     */

#include <stdint.h>
#include <zephyr/sys/byteorder.h>

#define MAGIC_WORD 0xA55A
#define MAX_PAYLOAD 244

    /* MSG ID */
    typedef enum
    {
        MSG_RES_OK = 0x00,
        MSG_RES_ERROR = 0x01,
        MSG_GET_VER = 0x10,
        MSG_RES_VER = 0x11,
        MSG_GET_STATS = 0x12,
        MSG_RES_STATS = 0x13,
        MSG_SET_SENSOR_INIT = 0x14, // on/off
        MSG_RES_SENSOR_INIT = 0x15,
        MSG_SET_TELEMETRY = 0x16, // on/off
        MSG_RES_TELEMETRY = 0x17,
        MSG_SET_SM_DURATION = 0x18, // duration in ms
        MSG_RES_SM_DURATION = 0x19,

        MSG_MAX
    } msg_id_t;

/* STRUCTURES */
#pragma pack(push, 1)
    typedef struct
    {
        uint16_t magic_hdr;
        uint8_t total_len;
        uint8_t msg_cnt;
        uint8_t messages[]; // 가변 길이 배열
    } ble_frame_t;

    typedef struct
    {
        uint8_t msg_id;
        uint8_t msg_len;
        uint8_t msg_data[]; // 가변 길이 데이터
    } ble_msg_block_t;
#pragma pack(pop)
    /*********************************************************/

#ifdef __cplusplus
}
#endif

#endif // BSP_MSG_PARSER_H