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
static int cmd_led_pwm(const struct shell *sh, size_t argc, char **argv)
{
    // For pirnts usage
    if (argc < 2) {
        shell_error(sh, "Usage: led_pwm r/g/b/color/on/off/br/bg/bb/bc ...");
        return -EINVAL;
    }
    
    if( (strcmp(argv[1], "r") == 0 ) || (strcmp(argv[1], "g") == 0) || (strcmp(argv[1], "b") == 0) )
    {
        if (argc < 3) {
            shell_error(sh, "Usage: led_pwm r/g/b <brightness>");
            return -EINVAL;
        }
        if( strcmp(argv[1], "r") == 0 )
        {
            if(bsp_led_pwm_red(atof(argv[2])) < 0) {
                shell_error(sh, "Failed to set RED LED brightness");
                return -EINVAL;
            }
        }
        else if( strcmp(argv[1], "g") == 0 )
        {
            if(bsp_led_pwm_green(atof(argv[2])) < 0) {
                shell_error(sh, "Failed to set GREEN LED brightness");
                return -EINVAL;
            }
        }
        else if( strcmp(argv[1], "b") == 0 )
        {
            if(bsp_led_pwm_blue(atof(argv[2])) < 0) {
                shell_error(sh, "Failed to set BLUE LED brightness");
                return -EINVAL;
            }
        }
        shell_print(sh, "LED %s brightness set to %d.%02d", argv[1], LOG_FLOAT_VAL(atof(argv[2])));
        return 0;
    } else if(strcmp(argv[1], "color") == 0) {
        if (argc < 5) {
            shell_error(sh, "Usage: led_pwm color <r> <g> <b>");
            return -EINVAL;
        }
        if (argc == 5) {
            if(bsp_led_pwm_set_color(atof(argv[2]), atof(argv[3]), atof(argv[4])) < 0) {
                shell_error(sh, "Failed to set LED color");
                return -EINVAL;
            }
            shell_print(sh, "LED color set to R: %d.%02d, G: %d.%02d, B: %d.%02d", LOG_FLOAT_VAL(atof(argv[2])), LOG_FLOAT_VAL(atof(argv[3])), LOG_FLOAT_VAL(atof(argv[4])));
            return 0;
        }
    } else if(strcmp(argv[1], "on") == 0) {
        bsp_led_pwm_on();
        shell_print(sh, "LEDs turned on with full brightness");
        return 0;
    } else if(strcmp(argv[1], "off") == 0) {
        bsp_led_pwm_off();
        shell_print(sh, "LEDs turned off");
        return 0;
    } else if(strcmp(argv[1], "br") == 0) {
        if (argc < 3) {
            shell_error(sh, "Usage: led_pwm br <brightness> <up> <down>");
            return -EINVAL;
        }
        if (argc == 5)
        {
            bsp_led_pwm_blink_red(atof(argv[2]), atoi(argv[3]), atoi(argv[4]));
            shell_print(sh, "Red LED blinked brightness - %d.%02d, up: %d, down: %d", LOG_FLOAT_VAL(atof(argv[2])), atoi(argv[3]), atoi(argv[4]));
            return 0;
        }
        else
        {
            shell_error(sh, "input params are wrong, %d", argc);
            return -EINVAL;
        }
    } else if(strcmp(argv[1], "bg") == 0) {
        if (argc < 3) {
            shell_error(sh, "Usage: led_pwm bg <brightness> <up> <down>");
            return -EINVAL;
        }
        if (argc == 5)
        {
            bsp_led_pwm_blink_green(atof(argv[2]), atoi(argv[3]), atoi(argv[4]));
            shell_print(sh, "Green LED blinked brightness - %d.%02d, up: %d, down: %d", LOG_FLOAT_VAL(atof(argv[2])), atoi(argv[3]), atoi(argv[4]));
            return 0;
        }
        else
        {
            shell_error(sh, "input params are wrong, %d", argc);
            return -EINVAL;
        }
    } else if(strcmp(argv[1], "bb") == 0) {
        if (argc < 3) {
            shell_error(sh, "Usage: led_pwm bb <brightness> <up> <down>");
            return -EINVAL;
        }
        if (argc == 5)
        {
            bsp_led_pwm_blink_green(atof(argv[2]), atoi(argv[3]), atoi(argv[4]));
            shell_print(sh, "Blue LED blinked brightness - %d.%02d, up: %d, down: %d", LOG_FLOAT_VAL(atof(argv[2])), atoi(argv[3]), atoi(argv[4]));
            return 0;
        }
        else
        {
            shell_error(sh, "input params are wrong, %d", argc);
            return -EINVAL;
        }
    } else if(strcmp(argv[1], "bc") == 0) {
        if (argc < 3) {
            shell_error(sh, "Usage: led_pwm bc <r> <g> <b> <up> <down>");
            return -EINVAL;
        }
        if (argc == 7)
        {
            bsp_led_pwm_blink_color(atof(argv[2]), atof(argv[3]), atof(argv[4]), atoi(argv[5]), atoi(argv[6]));
            shell_print(sh, "Color LED blinked brightness - r %d.%02d, g %d.%02d, b %d.%02d, up: %d, down: %d", LOG_FLOAT_VAL(atof(argv[2])), LOG_FLOAT_VAL(atof(argv[3])), LOG_FLOAT_VAL(atof(argv[4])), atoi(argv[5]), atoi(argv[6]));
            return 0;
        }
        else
        {
            shell_error(sh, "input params are wrong, %d", argc);
            return -EINVAL;
        }
    } else {
        shell_error(sh, "Invalid command. Use 'toggle', 'set', or 'color'");
        return -EINVAL;
    }
    return 0;
}

// Register command: "led <offset>"
SHELL_CMD_REGISTER(led_pwm, NULL, "Control an RGB LED using PWM", cmd_led_pwm);
