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

static int cmd_gpio(const struct shell *sh, size_t argc, char **argv)
{
    int ret = 0;

    SHELL_CHECK_ARGC(sh, argc, 2, "Usage: gpio init/3v/sens/fem/pmic");
    
    if(strcmp(argv[1], "init") == 0 ) {
        ret = bsp_gpio_init();
    } else if(strcmp(argv[1], "3v") == 0) {
        SHELL_CHECK_ARGC(sh, argc, 3, "Usage: gpio 3v set/get <state>?");
        if(strcmp(argv[2], "set") == 0) {
            SHELL_CHECK_RET(bsp_gpio_3v3_control(atoi(argv[3])), sh, "Invalid state, %d", atoi(argv[3]));
        } else if(strcmp(argv[2], "get") == 0) {
            ret = bsp_board_power_3v3_status();
            shell_print(sh, "3V3 %s", ret ? "ON" : "OFF");
            return 0;
        } else {
            shell_error(sh, "Invalid command. Use 'set' or 'status'");
            return -EINVAL;
        }
    } else if(strcmp(argv[1], "sens") == 0) {
        SHELL_CHECK_ARGC(sh, argc, 3, "Usage: gpio sens set/get <state>?");
        if(strcmp(argv[2], "set") == 0) {
            SHELL_CHECK_RET(bsp_gpio_sens_pwr_control(atoi(argv[3])), sh, "Invalid state, %d", atoi(argv[3]));
        } else if(strcmp(argv[2], "get") == 0) {
            ret = bsp_sensor_power_status();
            shell_print(sh, "Sensor Power %s", ret ? "ON" : "OFF");
        } else {
            shell_error(sh, "Invalid command. Use 'set' or 'status'");
            return -EINVAL;
        }
    } else if(strcmp(argv[1], "fem") == 0) {
        SHELL_CHECK_ARGC(sh, argc, 3, "Usage: gpio fem mode/rx_en/tx_en/sel(0/1/2/3) <state>");
        SHELL_CHECK_RET(bsp_gpio_fem_control(atoi(argv[2]), atoi(argv[3])), sh, "Invalid mode, %d", atoi(argv[2]));
    } else if(strcmp(argv[1], "pmic") == 0) {
        SHELL_CHECK_ARGC(sh, argc, 3, "Usage: gpio pmic iset/err/chg(0/1/2) <state>");
        SHELL_CHECK_RET(bsp_gpio_pmic_control(atoi(argv[2]), atoi(argv[3])), sh, "Invalid mode, %d", atoi(argv[2]));
    } else {
        shell_error(sh, "Invalid command. Use 'init/3v/sens/fem/pmic'");
        return -EINVAL;
    }
    return 0;
}

// Register command: "adc <offset>"
SHELL_CMD_REGISTER(gpio, NULL, "GPIO controls", cmd_gpio);
