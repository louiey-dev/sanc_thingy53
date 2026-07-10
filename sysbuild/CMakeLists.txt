# sysbuild/CMakeLists.txt
# Set up custom board root for sysbuild so it can locate sanc_thingy53

list(APPEND BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)

# Initialize sysbuild
find_package(Sysbuild REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(sysbuild LANGUAGES)
