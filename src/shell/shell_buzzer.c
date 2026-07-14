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

static int cmd_buzzer(const struct shell *sh, size_t argc, char **argv)
{
    int ret = 0;

    SHELL_CHECK_ARGC(sh, argc, 2, "Usage: buzzer init/tone/beep/scale");
    
    if(strcmp(argv[1], "init") == 0 ) {
        ret = bsp_buzzer_init();
    } else if(strcmp(argv[1], "tone") == 0) {
        SHELL_CHECK_ARGC(sh, argc, 3, "Usage: buzzer tone <frequency in hz>");
        SHELL_CHECK_RET(bsp_buzzer_tone(atoi(argv[2])), sh, "Invalid frequency, %d", atoi(argv[2]));
    } else if(strcmp(argv[1], "beep") == 0) {
        SHELL_CHECK_ARGC(sh, argc, 3, "Usage: buzzer beep <frequency in hz> <duration in ms>");
        SHELL_CHECK_RET(bsp_buzzer_beep(atoi(argv[2]), atoi(argv[3])), sh, "Invalid duration, %d, %d", atoi(argv[2]), atoi(argv[3]));
    } else if(strcmp(argv[1], "scale") == 0) {
        SHELL_CHECK_ARGC(sh, argc, 3, "Usage: buzzer scale <0 ~ 7, do/rae/mi/pa/sol/ra/si/do2>");
        SHELL_CHECK_RET(bsp_buzzer_scale(atoi(argv[2]), atoi(argv[3])), sh, "Invalid scale, %d", atoi(argv[2]));
    } else {
        shell_error(sh, "Invalid command. Use 'init/tone/beep/scale'");
        return -EINVAL;
    }
    return 0;
}

// Register command: "buzzer <offset>"
SHELL_CMD_REGISTER(buzzer, NULL, "Buzzer controls", cmd_buzzer);
