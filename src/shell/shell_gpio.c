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
        shell_error(sh, "Usage: gpio init/3v/sens");
        return -EINVAL;
    }
    
    if(strcmp(argv[1], "init") == 0 ) {
        ret = bsp_gpio_init();
    } else if(strcmp(argv[1], "3v") == 0) {
        ret = bsp_gpio_3v3_control(atoi(argv[2]));
    } else if(strcmp(argv[1], "sens") == 0) {
        ret = bsp_gpio_sens_pwr_control(atoi(argv[2]));
    } else {
        shell_error(sh, "Invalid command. Use 'init/3v/sens'");
        return -EINVAL;
    }
    return 0;
}

// Register command: "adc <offset>"
SHELL_CMD_REGISTER(gpio, NULL, "GPIO controls", cmd_gpio);
