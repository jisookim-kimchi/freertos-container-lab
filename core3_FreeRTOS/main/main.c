#include "ipc/memory.h"
#include "ipc/ring_buffer.h"
#include "utils/print.h"
#include "config.h"
#include "../interrupt/timer/timer.h"

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

    enable_mmu((uintptr_t)root_table, TCR_VALUE, MAIR_VALUE);
    uart_puts("MMU ON\n");

    struct RingBuffer *rtos_read_rb = (struct RingBuffer *)(RTOS_SHARED_READ_ONLY_BASE);
    struct RingBuffer *rtos_write_rb = (struct RingBuffer *)(RTOS_SHARED_WRITE_WRITE_BASE);
    
    RingBufferInit(rtos_write_rb);

    char recv_msg[64] = {0};
    while (1)
    {
        size_t read_bytes = RingBufferRead(rtos_read_rb, recv_msg, sizeof(recv_msg) - 1);
        
        if (read_bytes > 0)
        {
            recv_msg[read_bytes] = '\0';
            uart_puts(recv_msg);
        }
    }
}