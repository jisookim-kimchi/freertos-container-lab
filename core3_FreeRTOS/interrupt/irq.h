#ifndef IRQ_H
#define IRQ_H

#include "../FreeRTOSConfig.h"

typedef void (*isr_func_t)(void);
extern isr_func_t isr_table[1020];

void gic_irq_handler(void);
#endif
