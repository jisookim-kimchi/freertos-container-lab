#ifndef UART_H
#define UART_H

#include <stddef.h>
#include <stdint.h>

#define UART0_BASE 0x09000000

void uart_putc(char c);
void uart_puts(const char *str);
void uart_write(const void *buf, size_t len);
void print_number(uint64_t n);
void print_hex(uint64_t val);

#endif /* UART_H */