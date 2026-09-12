#include "utils/print.h"

void uart_putc(char c)
{
    volatile unsigned int *uart = (volatile unsigned int *)UART0_BASE;
    *uart = (unsigned int)c;
}

void uart_puts(const char *str)
{
    if (str == 0)
        return;
    while (*str)
    {
        uart_putc(*str);
        str++;
    }
}

void uart_write(const void *buf, size_t len)
{
    if (buf == 0)
        return;
    const uint8_t *p = (const uint8_t *)buf;
    while (len--)
    {
        uart_putc((char)*p++);
    }
}

void print_number(uint64_t nb)
{
    char buf[32];
    int i = 0;
    if (nb == 0)
    {
        uart_putc('0');
        return;
    }
    while (nb > 0)
    {
        buf[i++] = (nb % 10) + '0';
        nb /= 10;
    }
    while (i > 0)
    {
        uart_putc(buf[--i]);
    }
}

static void ft_puthex(uint64_t n)
{
    if (n >= 16)
        ft_puthex(n / 16);
    
    uart_putc("0123456789ABCDEF"[n % 16]);
}

void print_hex(uint64_t val)
{
    uart_puts("0x");
    ft_puthex(val);
}