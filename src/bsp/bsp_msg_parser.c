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

/** DEFINES (#define xx) **/

/*****************************************************************/

/** EXTERNS (extern xx) **/
extern BSP_ST g_Bsp;
/*****************************************************************/

/** STATICS (static xx) **/

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
    if(msg == NULL || len == 0) {
        LOG_ERR("Invalid message or length");
        return -EINVAL;
    }
    // Parse the message and extract relevant information
    
    //////////////////////////////////////////////////////

    return 0;
}
