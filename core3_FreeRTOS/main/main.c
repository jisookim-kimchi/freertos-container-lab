#include "ipc/memory.h"
#include "ipc/ring_buffer.h"
#include "utils/print.h"
#include "config.h"
#include "../interrupt/GIC.h"
#include "../interrupt/timer/timer.h"
#include "../interrupt/irq.h"

extern void set_vector_table(void);
extern void ipc_rx_handler(void);

__attribute__((weak)) void FreeRTOS_Tick_Handler(void)
{
    static int tick_count = 0;
    if (++tick_count % 100 == 0)
    {
        uart_puts("[Core 3] 1 Second Elapsed!\n");
    }
}

extern void enable_interrupt(void);
extern void disable_interrupt(void);
extern uint64_t cycle_counter(void);

void main(void)
{
    uart_puts("Hello MMU OFF\n");
    struct PageTable *root_table = PageTableAllocate(0);

    struct MemoryArea ram_area, uart_area, shared_read_area, shared_write_area, gic_area;
    MemoryAreaInit(&ram_area, (void *)RTOS_MEMORY_BASE, (void *)RTOS_MEMORY_BASE, RTOS_MEMORY_SIZE, MEM_READ | MEM_WRITE | MEM_EXEC);
    MemoryAreaInit(&shared_read_area, (void *)RTOS_SHARED_READ_ONLY_BASE, (void *)RTOS_SHARED_READ_ONLY_BASE, RING_BUFFER_SIZE, MEM_READ | MEM_WRITE);
    MemoryAreaInit(&shared_write_area, (void *)RTOS_SHARED_WRITE_WRITE_BASE, (void *)RTOS_SHARED_WRITE_WRITE_BASE, RING_BUFFER_SIZE, MEM_READ | MEM_WRITE);
    MemoryAreaInit(&uart_area, (void *)0x09000000, (void *)0x09000000, 0x1000, MEM_READ | MEM_WRITE);
    MemoryAreaInit(&gic_area, (void*)0x08000000, (void*)0x08000000, 0x20000, MEM_READ | MEM_WRITE);

    page_table_map(root_table, 0, &ram_area);
    page_table_map(root_table, 0, &shared_read_area);
    page_table_map(root_table, 0, &shared_write_area);
    page_table_map(root_table, 0, &uart_area);
    page_table_map(root_table, 0, &gic_area);

    enable_mmu((uintptr_t)root_table, TCR_VALUE, MAIR_VALUE);
    uart_puts("MMU ON\n");

    set_vector_table();
    register_isr(TIMER_IRQ, timer_handler);
    register_isr(SGI_IRQ3, ipc_rx_handler);

    gic_dist_init();
    gic_cpu_init();
    gic_enable_irq(TIMER_IRQ);
    gic_enable_irq(SGI_IRQ3);
    gic_enable_irq(UART_IRQ);
    timer_init();
    enable_interrupt();
    uart_puts("Timer interrupt is started\n");
    while (1)
    {
        asm volatile("wfi");
    }
}