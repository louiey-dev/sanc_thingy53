#!/bin/bash
# Helper script to flash sanc_thingy53 over USB using nrfutil

# Source the nRF Connect SDK environment
if [ -f "/home/louiey/ncs/ncs_env.sh" ]; then
    source /home/louiey/ncs/ncs_env.sh
else
    echo "Error: ncs_env.sh not found at /home/louiey/ncs/ncs_env.sh"
    exit 1
fi

# Step 1: Check and install the nrfutil device command if needed
if ! nrfutil list | grep -q "device"; then
    echo "Installing 'device' command for nrfutil..."
    nrfutil install device
fi

# Step 2: List connected devices
echo "Detecting connected Nordic devices..."
nrfutil device list

# Step 3: Prompt user to put device in DFU mode
echo ""
echo "=========================================================="
echo "HOW TO ENTER DFU MODE:"
echo "1. Power off the Thingy:53."
echo "2. Press and hold button (SW1) while flipping the power switch to ON."
echo "3. The LED should blink yellow/orange."
echo "=========================================================="
echo ""
read -p "Press [Enter] when the Thingy:53 is in DFU mode and connected..."

# Step 4: Program the device
echo "Programming the device with build/dfu_application.zip..."
nrfutil device program --firmware build/dfu_application.zip --traits MCUboot
