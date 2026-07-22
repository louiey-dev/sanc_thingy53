/*
 * @file : bsp_statistics.c
 * 
 * @brief : 
 * 
 * @author : louiey, louiey@thountech.com
 * 
 * @date : 2026-07-22
 * @copyright : Copyright (c) 2026
 *
 * @section : History
 *            <Date>   <Description of Changes>
 */
#include <zephyr/kernel.h>
 #include <zephyr/sys/sys_heap.h>
#include "bsp.h"

/** DEFINES (#define xx) **/

/*****************************************************************/

/** EXTERNS (extern xx) **/
extern BSP_ST g_Bsp;
/*****************************************************************/

/** STATICS (static xx) **/

/*****************************************************************/

/** LOCALS (m_xx) **/
LOG_MODULE_REGISTER(bsp_statistics, LOG_LEVEL_INF);
/*****************************************************************/

/** FUNCTIONS (int bsp_xx()) **/

extern struct k_thread z_idle_threads[];

static void print_system_info(void);

int bsp_statistics(uint32_t* cpu_usage, uint32_t* heap_allocated, uint32_t* heap_free)
{
    k_thread_runtime_stats_t total_stats_start, total_stats_end;
    k_thread_runtime_stats_t idle_stats_start, idle_stats_end;

    k_tid_t idle_thread = (k_tid_t)&z_idle_threads[0];

    // 1. Capture start baseline
    k_thread_runtime_stats_all_get(&total_stats_start);
    k_thread_runtime_stats_get(idle_thread, &idle_stats_start);

    k_sleep(K_MSEC(1000)); // Sample window (1 second)

    // 2. Capture end sample
    k_thread_runtime_stats_all_get(&total_stats_end);
    k_thread_runtime_stats_get(idle_thread, &idle_stats_end);

    // 3. Compute delta cycles
    uint64_t total_cycles = total_stats_end.execution_cycles - total_stats_start.execution_cycles;
    uint64_t idle_cycles  = idle_stats_end.execution_cycles - idle_stats_start.execution_cycles;

    if (total_cycles > 0) {
        uint32_t usage = 100 - (uint32_t)((idle_cycles * 100) / total_cycles);
        if (cpu_usage != NULL) {
            *cpu_usage = usage;
        }
        LOG_INF("CPU Usage: %u%%", usage);
    }

    size_t unused_stack;
    if (k_thread_stack_space_get(k_current_get(), &unused_stack) == 0) {
        LOG_INF("Current Thread Unused Stack: %usr bytes", unused_stack);
    }

    struct sys_heap heap;
    struct sys_memory_stats stats;
    if (sys_heap_runtime_stats_get(&heap, &stats) == 0){
        LOG_INF("heap allocated : %zu, free : %zu", stats.allocated_bytes, stats.free_bytes);
        if (heap_allocated != NULL) {
            *heap_allocated = stats.allocated_bytes;
        }
        if (heap_free != NULL) {
            *heap_free = stats.free_bytes;
        }
    }

    return 0;
}


static void print_system_info(void)
{
    // 1. CPU Clock & Uptime
    uint32_t cpu_freq_hz = sys_clock_hw_cycles_per_sec();
    int64_t uptime_ms = k_uptime_get();
    LOG_INF("CPU Frequency: %u Hz | Uptime: %lld ms", cpu_freq_hz, uptime_ms);

    // 2. Thread Stack Space (Current Thread)
    size_t unused_stack;
    if (k_thread_stack_space_get(k_current_get(), &unused_stack) == 0) {
        LOG_INF("Current Thread Unused Stack: %usr bytes", unused_stack);
    }
}
