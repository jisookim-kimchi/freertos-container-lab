#ifndef IRQ_H
#define IRQ_H

#include "../FreeRTOSConfig.h"
#include <stdint.h>

/*
    ID0-ID15 are used for SGIs(software generated interrupt)
    ID16-ID31 are used for PPIs(private peripheral interrupt)
    ID32-ID1019 SPI(shared peripheral interrupts)
*/
#define ISR_TABLE_SIZE 1020
#define TIMER_IRQ 30    // FreeRTOS heartbeat.
#define UART_IRQ 33
#define RTC_IRQ 34      // Real Time Clock
#define GPIO_IRQ 39
#define SGI_IRQ3 3      // ipc linux <---> FreeRTOS.

typedef void (*isr_func_t)(void);
extern isr_func_t isr_table[ISR_TABLE_SIZE];

void gic_irq_handler();
void register_isr(uint32_t irq_id, isr_func_t handler);
#endif
