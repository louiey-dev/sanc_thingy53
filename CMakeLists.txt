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
    src/bsp/bsp_buzzer.c
    src/bsp/bsp_msg_parser.c
    src/bsp/bsp_utils.c
    src/bsp/bsp_state_machine.c
    src/bsp/bsp_sensor_adxl362.c
    src/bsp/bsp_sensor_bh1749.c
    src/bsp/bsp_sensor_bme688.c
    src/bsp/bsp_sensor_bmi270.c
    src/bsp/bsp_sensor_bmm150.c
    src/shell/shell_led.c
    src/shell/shell_led_pwm.c
    src/shell/shell_system.c
    src/shell/shell_adc.c
    src/shell/shell_gpio.c
    src/shell/shell_sensors.c
    src/shell/shell_buzzer.c
)

target_include_directories(app PRIVATE
    src/bsp
    src/shell
)
