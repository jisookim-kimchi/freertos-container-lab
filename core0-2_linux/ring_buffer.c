#include "ring_buffer.h"

static inline void DataMemoryBarrier(void)
{
    asm volatile("dmb ish" ::: "memory");
}

void RingBufferInit(struct RingBuffer *buffer)
{
    if (buffer == NULL)
        return;
    buffer->head = 0;
    buffer->tail = 0;
    DataMemoryBarrier();
}

int RingBufferIsEmpty(struct RingBuffer *buffer)
{
    if (buffer == NULL)
        return 1;
    return buffer->head == buffer->tail;
}

size_t RingBufferWrite(struct RingBuffer *r_buffer, const void *data, size_t size)
{
    if (r_buffer == NULL || data == NULL || size == 0)
        return 0;

    const uint8_t *src = (const uint8_t *)data;
    uint32_t head = r_buffer->head;
    uint32_t tail = r_buffer->tail;

    size_t occupied = (head - tail) & BUFFER_MASK;
    size_t free_space = (BUFFER_SIZE - 1) - occupied;
    if (size > free_space)
        return 0;

    size_t written = size;
    while (written--)
    {
        r_buffer->buffer[head++] = *src++;
        head &= BUFFER_MASK;
    }
    DataMemoryBarrier();
    r_buffer->head = head;

    return size;
}

size_t RingBufferRead(struct RingBuffer *r_buffer, void *data, size_t size)
{
    if (r_buffer == NULL || data == NULL || size == 0)
        return 0;
    
    uint8_t *dest = (uint8_t *)data;
    uint32_t head = r_buffer->head;
    uint32_t tail = r_buffer->tail;

    size_t occupied = (head - tail) & BUFFER_MASK;
    if (occupied == 0)
        return 0;

    size_t to_read = size;
    if (size < occupied)
        to_read = size;
    else
        to_read = occupied;

    size_t read = to_read;
    while (read--)
    {
        *dest++ = r_buffer->buffer[tail++];
        tail &= BUFFER_MASK;
    }
    DataMemoryBarrier();
    r_buffer->tail = tail;

    return to_read;
}