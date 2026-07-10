cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(sanc_thingy53)

target_sources(app PRIVATE 
    src/main.c
    src/bsp/bsp.c
    src/bsp/bsp_led.c
    src/bsp/bsp_led_pwm.c
    src/bsp/bsp_gpio.c
    src/bsp/bsp_adc.c
    src/bsp/bsp_msg_parser.c
    src/shell/shell_led.c
    src/shell/shell_led_pwm.c
    src/shell/shell_system.c
)

target_include_directories(app PRIVATE
    src/bsp
    src/shell
)
