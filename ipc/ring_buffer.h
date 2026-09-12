#ifndef RING_BUFFER_H
#define RING_BUFFER_H

/*
    @brief  : between Linux and FreeRTOS domains.
                SPSC Single Producer Single Consumer but i would say it is SWSR.
                On Linux side there are multi core and one of them must dedicate to manage the priority of tasks.
    @Lockfree : Linux manages head, FreeRTOS manages tail.

*/
#include <stddef.h>
#include <stdint.h>

#define BUFFER_SIZE 4096
#define BUFFER_MASK (BUFFER_SIZE - 1)
struct RingBuffer
{
    volatile uint32_t head;
    volatile uint32_t tail;
    uint8_t buffer[BUFFER_SIZE];
};

struct SharedMemoryBus
{
    // 1. Linux -> rtos RingBuffer : linux only write, rtos only read
    struct RingBuffer linux_to_rtos;
    // 2. rtos -> Linux RingBuffer : rtos only write, linux only read
    struct RingBuffer rtos_to_linux;
};

void SharedMemoryBusInit(struct SharedMemoryBus *bus);
void RingBufferInit(struct RingBuffer *buffer);

size_t RingBufferWrite(struct RingBuffer *r_buffer, void *data, size_t size);
size_t RingBufferRead(struct RingBuffer *r_buffer, void *data, size_t size);

size_t RingBufferWrite_Atomic(struct RingBuffer *r_buffer, void *data, size_t size);
size_t RingBufferRead_Atomic(struct RingBuffer *r_buffer, void *data, size_t size);

#endif