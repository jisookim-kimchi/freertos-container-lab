#include "irq.h"
#include "GIC.h"

isr_func_t isr_table[ISR_TABLE_SIZE];

/** 
    @brief : 1020~1023 is special Interrupt.
*/
void gic_irq_handler()
{
    uint32_t iar = *(volatile uint32_t *)GICC_IAR;
    uint32_t irq_id = iar & 0x3ff;
    if (irq_id >= ISR_TABLE_SIZE)
        return ;
    if (isr_table[irq_id] != 0)
        isr_table[irq_id]();

    *(volatile uint32_t *)GICC_EOIR = iar;
}

void register_isr(uint32_t irq_id, isr_func_t handler)
{
    if (irq_id < ISR_TABLE_SIZE)
    {
        isr_table[irq_id] = handler;
    }
}