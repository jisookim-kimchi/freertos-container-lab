#ifndef TIMER_H
#define TIMER_H

#include "../GIC.h"

void timer_handler(void);
void timer_init(void);
extern void enable_timer(void);     
extern void FreeRTOS_Tick_Handler(void);
extern uint64_t get_timer_clock_frequency(void);
extern void set_timer_interval(uint64_t ticks);

#endif