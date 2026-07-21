# sanc_thingy53

---
## build

```bash
# set up ncs and west
get_ncs

# build full - pristine build
west build -p always -b sanc_thingy53/nrf5340/cpuapp

# build only application - incremental build
west build

# Flash the signed firmware update via USB bootloader (DFU)

## Flash application and network core together
./flash_usb_app_net.sh

## Flash application
./flash_usb.sh

# Flashing via External Debugger (JTAG)

## Flash full images
./flash_full.sh

## Flash application
./flash.sh
```
---
## OTA

- For Application-Core Update Only (Standard OTA)
    - Use signed application bin
        - build/sanc_thingy53/zephyr/zephyr.signed.bin
- Dual-Core Update (App + Net Cores)
    - build/signed_by_mcuboot_and_b0_ipc_radio.bin
---
## History

- 2026.07.10
    - basic framework is done
        - zephyr shell
        - bsp_xx
        - app.overlay for dts
        - Tx/Rx via BLE
        - led control via IO and PWM
- 2026.07.13
    - add buzzer/adc for batt/gpio/reset
    - add sensors bsp files
        - all sensors work with manual read mode
        - adxl362 : Low power accelerometer, interuppt line is not connected
        - bh1749 : color sensor so I don't feel there needs external interrupt
        - bme688 : pressure/humi/temp/gas sensor so I don't feel there needs external interrupt
        - bmi270 : Accelerometer and gyroscope, connected DRDY pin so continuously issues DRDY INT
        - bmm150 : Magnetometer, connected DRDY pin so continuously issues DRDY INT
    - add GPIO control for FEM/PMIC
- 2026.07.14
    - minor code optimized
- 2026.07.16
    - add state machine code
    - updated msg data type for sensors
    - add telemetry function in state machine
- 2026.07.21
    - add OTA feature
        - tested with dfu_application.zip and zephyr.signed.bin. Both are work with OTA
    - add Payload upload API
        - All sensors + Battery mV

---
## Info

- Author : <louiey.dev@gmail.com>
- Build Host : WSL2 Ubuntu 24.04 LTS
- IDE : VSCode
- HW : Nordic nRF Thingy53 board
- IC : nRF5340
- RTOS : Zephyr OS
- Language : C/C++
---