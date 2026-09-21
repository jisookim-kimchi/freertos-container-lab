#include "irq.h"
#include "GIC.h"

isr_func_t isr_table[1020];
extern void timer_handler(void);

/** 
    @brief : 1020~1023 is special Interrupt.
*/
void gic_irq_handler()
{
    uint32_t iar = *(volatile uint32_t *)GICC_IAR;
    uint32_t irq_id = iar & 0x3ff;
    if (irq_id >= 1020)
        return ;
    if (irq_id == 30)
        timer_handler();

    *(volatile uint32_t *)GICC_EOIR = iar;
}