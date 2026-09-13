#ifndef RING_BUFFER_H
#define RING_BUFFER_H

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

void RingBufferInit(struct RingBuffer *buffer);
int RingBufferIsEmpty(struct RingBuffer *buffer);

size_t RingBufferWrite(struct RingBuffer *r_buffer, const void *data, size_t size);
size_t RingBufferRead(struct RingBuffer *r_buffer, void *data, size_t size);

#endif
