#include "GIC.h"

extern uint64_t get_timer_clock_frequency(void);
extern uint64_t get_current_physical_count(void);

typedef void (*isr_func_t)(void);
isr_func_t isr_table[1020];
/*
    Internal Isolation & Safety
    Purpose : Nevertheless the Core 0 crashed or get interrupt Storm,
              Core 3(FREERTOS) can maintain Isolation from the Core 0's failure.
*/

/*
    @brief GIC distributor initialize function
    @1. Distributor OFF
    @2. Disable All interrupts
    @3. Clear All Pending IRQs
    @4. Set priority all interrupts to 0xA0 (middle priority) 1 register has 4 IRQ(8bits * 4) == (8*(GICD_TYPER.ITLinesNumber+1))
    @5. Interrupt Processor Target Registers[i] : Set Target Core to 0x8 (Core 3) 1 register has 4 IRQ(8bits * 4) core 0 = 1, core 1 = 2, core 2 = 4, core 3 = 8
    @6. Distributor ON
*/
void gic_dist_init()
{
    GIC_DISTRIBUTOR_OFF();
    for (int i = 0; i < 32; i++)
    {
        *((volatile uint32_t *)GICD_ICENABLER(i)) = 0xFFFFFFFF;  //interrupt off
        *((volatile uint32_t *)GICD_ICPENDR(i))  = 0xFFFFFFFF; // Clear All Pending IRQs
    }
    for (int i = 0; i < 255; i++)
    {
        *((volatile uint32_t *)GICD_IPRIORITYR(i)) = 0xA0A0A0A0; // Set Priority to 0xA0 (middle priority)
    }
    for (int i = 0; i < 255; i++)
    {
        *((volatile uint32_t *)GICD_ITARGETSR(i)) = 0x08080808;  // Set Target Core to 0x8 (Core 3)
    }
    GIC_DISTRIBUTOR_ON();
}

/*
    @brief GIC CPU interface initialize function
    @1. Set Priority all interrupts to 0xA0 (middle priority)
    @2. Set Binary Point Register to 0x0 (all bits = group priority) Preemption Enable
    @3. Set Running Priority Register to 0x0
    @4. Interrupt Acknowledge Register to 0x0
*/
void gic_cpu_init()
{
    GIC_CPU_PRIO_SET(255);
    GIC_CPU_SET_BPR(0);
    GIC_INTERFACE_ON();
}

/**
    @brief : 
*/
void timer_handler()
{
    uint64_t timer_freq = get_timer_clock_frequency();
    uint64_t ticks = timer_freq / configTICK_RATE_HZ;
    
}

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