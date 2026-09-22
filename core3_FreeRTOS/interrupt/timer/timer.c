#include "../GIC.h"
#include "../../FreeRTOSConfig.h"
#include "timer.h"

/**
    @brief : FreeRTOS Tick handler called per 10ms.
    @param : timer_freq : 62.5Mhz, 62500000Hz from ARM timer.
    @param : configTICK_RATE_HZ : 100Hz from RTOS.
    @param : ticks : 625000 counts per tick(10ms). 0.01second
*/
void timer_handler()
{
    uint64_t timer_freq = get_timer_clock_frequency();
    uint64_t ticks = timer_freq / configTICK_RATE_HZ;
    set_timer_interval(ticks);
    FreeRTOS_Tick_Handler();
}

void timer_init()
{
    uint64_t timer_freq = get_timer_clock_frequency();
    uint64_t ticks = timer_freq / configTICK_RATE_HZ;
    set_timer_interval(ticks);
    enable_timer();
}
