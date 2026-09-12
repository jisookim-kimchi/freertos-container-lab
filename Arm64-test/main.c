#include "../container/memory.h"
#include "../config.h"
#include "../container/ring_buffer.h" 

#define UART0_BASE 0x09000000

void uart_putc(char c)
{
     volatile unsigned int *uart = (volatile unsigned int *)UART0_BASE;
    *uart = c;
}

void uart_puts(const char *str)
{
    while (*str)
    {
        uart_putc(*str);
        str++;
    }
}

void main(void)
{
    uart_puts("Hello MMU OFF\n");
    struct PageTable *root_table = PageTableAllocate(0);

    struct MemoryArea ram_area, uart_area;
    MemoryAreaInit(&ram_area, (void *)0x40000000, (void *)0x40000000, 0x200000, MEM_READ | MEM_WRITE | MEM_EXEC);
    MemoryAreaInit(&uart_area, (void *)0x09000000, (void *)0x09000000, 0x1000, MEM_READ | MEM_WRITE);
    uart_puts("1\n");

    page_table_map(root_table, 0, &ram_area);
    page_table_map(root_table, 0, &uart_area);
    uart_puts("2\n");
    enable_mmu((uintptr_t)root_table, TCR_VALUE, MAIR_VALUE); //TODO error need to debug.
    uart_puts("3\n");
    uart_puts("MMU ON\n");
    /*
        ring buffer test
    */
    static struct RingBuffer test_rb;
    RingBufferInit(&test_rb);
    char send_msg[] = "hello Empfanger!\n";
    char recv_msg[64] = {0};
    RingBufferWrite(&test_rb, send_msg, sizeof(send_msg));
    uart_puts("Write to RingBuffer Success!\n");
    RingBufferRead(&test_rb, recv_msg, sizeof(send_msg));
    uart_puts("Read from RingBuffer: ");
    uart_puts(recv_msg);

    while (1)
    {
    }
}