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
## History

- 2026.07.10
    - basic framework is done
        - zephyr shell
        - bsp_xx
        - app.overlay for dts
        - Tx/Rx via BLE
        - led control via IO and PWM
- 2026.07.13
    - add buzzer/adc for batt/gpio
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