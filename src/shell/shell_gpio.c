/*
 * @file : shell_gpio.c
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

static int cmd_gpio(const struct shell *sh, size_t argc, char **argv)
{
    int ret = 0;
    int32_t value = 0;

    // For pirnts usage
    if (argc < 2) {
        shell_error(sh, "Usage: gpio init/r_mv/r_raw/charge/err");
        return -EINVAL;
    }
    
    if(strcmp(argv[1], "init") == 0 ) {
        ret = bsp_adc_init();
    } else if(strcmp(argv[1], "r_mv") == 0) {
        ret = bsp_adc_battery_mv(&value);
    } else if(strcmp(argv[1], "r_raw") == 0) {
        ret = bsp_adc_battery_raw(&value);
    } else if(strcmp(argv[1], "charge") == 0) {
        ret = bsp_adc_battery_charging();
    } else if(strcmp(argv[1], "err") == 0) {
        ret = bsp_adc_battery_err();
    } else {
        shell_error(sh, "Invalid command. Use 'init/r_mv/r_raw/charge/err'");
        return -EINVAL;
    }
    return 0;
}

// Register command: "adc <offset>"
SHELL_CMD_REGISTER(adc, NULL, "GPIO controls", cmd_gpio);
