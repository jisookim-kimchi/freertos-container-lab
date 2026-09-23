#include "ring_buffer.h"
#include "../config.h"
#include "../utils/print.h"

void ipc_rx_handler()
{
    struct RingBuffer *rtos_read_rb = (struct RingBuffer *)(RTOS_SHARED_READ_ONLY_BASE);
    char recv_buffer[64] = {0};
    
    size_t read_bytes = RingBufferRead(rtos_read_rb, recv_buffer, sizeof(recv_buffer) - 1);
    if (read_bytes > 0)
    {
        recv_buffer[read_bytes] = '\0';
        uart_puts("IPC_Rx : Received: ");
        uart_puts(recv_buffer);
    }
}

// void ipc_tx_handler()
// {
    
// }