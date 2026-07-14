/*
 * @file : shell_adc.c
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
#include "bsp.h"
#include "shell_sanc.h"
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

static int cmd_adc(const struct shell *sh, size_t argc, char **argv)
{
    int ret = 0;
    int32_t value = 0;

    // For pirnts usage
    SHELL_CHECK_ARGC(sh, argc, 2, "Usage: adc init/r_mv/r_raw/charge/err");

    if(strcmp(argv[1], "init") == 0 ) {
        SHELL_CHECK_RET(bsp_adc_init(), sh, "Init failed, %d", ret);
    } else if(strcmp(argv[1], "r_mv") == 0) {
        SHELL_CHECK_RET(bsp_adc_battery_mv(&value), sh, "Batt mV read failed, %d", ret);
    } else if(strcmp(argv[1], "r_raw") == 0) {
        SHELL_CHECK_RET(bsp_adc_battery_raw(&value), sh, "Batt raw read failed, %d", ret);
    } else if(strcmp(argv[1], "charge") == 0) {
        SHELL_CHECK_RET(bsp_adc_battery_charging(), sh, "Batt charging status read failed, %d", ret);
    } else if(strcmp(argv[1], "err") == 0) {
        SHELL_CHECK_RET(bsp_adc_battery_err(), sh, "Batt err status read failed, %d", ret);
    } else {
        shell_error(sh, "Invalid command. Use 'init/r_mv/r_raw/charge/err'");
        return -EINVAL;
    }
    return 0;
}

// Register command: "adc <offset>"
SHELL_CMD_REGISTER(adc, NULL, "Read ADC values", cmd_adc);
