/*
 * @file : shell_system.c
 * 
 * @brief : 
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
#include <zephyr/shell/shell.h>
#include "bsp.h"

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

static int cmd_system(const struct shell *sh, size_t argc, char **argv)
{
    // For pirnts usage
    if (argc < 2) {
        shell_error(sh, "Usage: sys nus_du/reset ...");
        return -EINVAL;
    }
    
    if(strcmp(argv[1], "nus_du") == 0 ) {
        if (argc < 3) {
            shell_error(sh, "Usage: sys nus_du <duration in seconds>");
            return -EINVAL;
        }
        g_Bsp.nus_duration = atoi(argv[2]);
        shell_print(sh, "NUS duration set to %d seconds", g_Bsp.nus_duration);
        return 0;
    } if(strcmp(argv[1], "reset") == 0 ) {
        if (argc < 3) {
            shell_error(sh, "Usage: sys reset <duration in seconds>");
            return -EINVAL;
        }
        shell_print(sh, "System reset");
        k_msleep(atoi(argv[2])); // sleep with given duration in msec
        bsp_reset();
        return 0;
    } else {
        shell_error(sh, "Invalid command. Use 'nus_du'");
        return -EINVAL;
    }
    return 0;
}

// Register command: "sys <offset>"
SHELL_CMD_REGISTER(sys, NULL, "Config system parameters", cmd_system);
