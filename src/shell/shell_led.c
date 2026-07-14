/*
 * @file : shell_led.c
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
static int cmd_led(const struct shell *sh, size_t argc, char **argv)
{
    int led_offset;

    // For pirnts usage
    SHELL_CHECK_ARGC(sh, argc, 2, "Usage: led toggle/set/color ...");
    if(strcmp(argv[1], "toggle") == 0 ) {
        SHELL_CHECK_ARGC(sh, argc, 3, "Usage: led toggle <led_offset>");
        // led toggle command requires an offset argument
        led_offset = atoi(argv[2]);
        if(bsp_led_toggle(led_offset) < 0) {
            shell_error(sh, "Failed to toggle LED%d", led_offset);
            return -EINVAL;
        }
        else
        {
            shell_print(sh, "LED%d toggled successfully", led_offset);
            return 0;
        }
    } else if(strcmp(argv[1], "set") == 0) {
        // led set command requires an offset and state argument
        SHELL_CHECK_ARGC(sh, argc, 4, "Usage: led set <led_offset> <state>");
        led_offset = atoi(argv[2]);
        if(bsp_led_control(led_offset, atoi(argv[3])) < 0) {
            shell_error(sh, "Failed to set LED%d state", led_offset);
            return -EINVAL;
        }
        else
        {
            shell_print(sh, "LED%d set to %s", led_offset, atoi(argv[3]) ? "ON" : "OFF");
            return 0;
        }
    } else if(strcmp(argv[1], "color") == 0) {
        SHELL_CHECK_ARGC(sh, argc, 3, "Usage: led color <r/g/b>");
        // led set command requires an offset and state argument
        if(strcmp(argv[2], "r") == 0){
            bsp_led_control(0, 1); // Turn on RED LED
            bsp_led_control(1, 0); // Turn off GREEN LED
            bsp_led_control(2, 0); // Turn off BLUE LED
            shell_print(sh, "LED set to RED");
            return 0;
        } else if(strcmp(argv[2], "g") == 0){
            bsp_led_control(0, 0); // Turn off RED LED
            bsp_led_control(1, 1); // Turn on GREEN LED
            bsp_led_control(2, 0); // Turn off BLUE LED
            shell_print(sh, "LED set to GREEN");
            return 0;
        } else if(strcmp(argv[2], "b") == 0){
            bsp_led_control(0, 0); // Turn off RED LED
            bsp_led_control(1, 0); // Turn off GREEN LED
            bsp_led_control(2, 1); // Turn on BLUE LED
            shell_print(sh, "LED set to BLUE");
            return 0;
        } else {
            shell_error(sh, "Invalid color. Use 'r', 'g', or 'b'");
            return -EINVAL;
        }
    } else {
        shell_error(sh, "Invalid command. Use 'toggle', 'set', or 'color'");
        return -EINVAL;
    }
    return 0;
}

// Register command: "led <offset>"
SHELL_CMD_REGISTER(led, NULL, "Control an RGB LED (0:RED, 1:GREEN, 2:BLUE)", cmd_led);
