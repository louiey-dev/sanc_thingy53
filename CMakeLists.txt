cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(sanc_thingy53)

target_sources(app PRIVATE 
    src/main.c
    src/bsp/bsp.c
    src/shell/shell_led.c
    src/shell/shell_system.c
)

target_include_directories(app PRIVATE
    src/bsp
    src/cli
)
