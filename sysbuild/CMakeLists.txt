# sysbuild/CMakeLists.txt
# Set up custom board root for sysbuild so it can locate sanc_thingy53

list(APPEND BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)

# Pre-populate EXTRA_DTC_OVERLAY_FILE for sanc_thingy53 in the correct order.
# Because the sysbuild add_overlay macro skips files already in the list,
# this guarantees that flash_sim_enable.overlay is evaluated last.
set(sanc_thingy53_EXTRA_DTC_OVERLAY_FILE
  "/home/louiey/ncs/v3.4.0/nrf/modules/mcuboot/flash_sim_pm.overlay"
  "${CMAKE_CURRENT_SOURCE_DIR}/../flash_sim_enable.overlay"
  CACHE STRING "Extra DTC overlays for sanc_thingy53" FORCE
)

# Initialize sysbuild
find_package(Sysbuild REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(sysbuild LANGUAGES)

