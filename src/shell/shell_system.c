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
#include "shell_sanc.h"

#include <app_version.h>

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
    // For prints usage
    SHELL_CHECK_ARGC(sh, argc, 2, "Usage: sys nus_du/nus_stat/reset ...");

    if (strcmp(argv[1], "nus_du") == 0)
    {
        SHELL_CHECK_ARGC(sh, argc, 3, "Usage: sys nus_du <duration in seconds>");

        g_Bsp.nus_duration = atoi(argv[2]);
        shell_print(sh, "NUS duration set to %d seconds", g_Bsp.nus_duration);
        return 0;
    }
    if (strcmp(argv[1], "nus_stat") == 0)
    {
        shell_print(sh, "NUS Notifications: %s", g_Bsp.nus_notif_enabled ? "Enabled" : "Disabled");
        return 0;
    }
    if (strcmp(argv[1], "reset") == 0)
    {
        SHELL_CHECK_ARGC(sh, argc, 3, "Usage: sys reset <duration in seconds>");

        shell_print(sh, "System reset");
        k_msleep(atoi(argv[2])); // sleep with given duration in msec
        bsp_reset();
        return 0;
    }
    if (strcmp(argv[1], "ver") == 0)
    {
        shell_print(sh, "System version");
        shell_print(sh, "App Ver : %s", APP_VERSION_STRING);
        return 0;
    }
    else
    {
        shell_error(sh, "Invalid command. Use 'nus_du', 'nus_stat', 'reset', or 'ver'");
        return -EINVAL;
    }
    return 0;
}

// Register command: "sys <offset>"
SHELL_CMD_REGISTER(sys, NULL, "Config system parameters", cmd_system);
