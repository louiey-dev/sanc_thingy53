#!/bin/bash
# Helper script to flash sanc_thingy53/nrf5340/cpuapp

# Source the nRF Connect SDK environment
if [ -f "/home/louiey/ncs/ncs_env.sh" ]; then
    source /home/louiey/ncs/ncs_env.sh
else
    echo "Error: ncs_env.sh not found at /home/louiey/ncs/ncs_env.sh"
    exit 1
fi

# Flash the dual-core images using west with the nrfjprog runner
echo "Flashing sanc_thingy53..."
west flash --runner nrfjprog
