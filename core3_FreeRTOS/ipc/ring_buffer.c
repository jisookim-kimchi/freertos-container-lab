#include "ring_buffer.h"

extern void DataMemoryBarrier();
extern void DataSyncBarrier();
extern void ISB_flush();
extern uint32_t atomic_cas(uint32_t *ptr, uint32_t old_val, uint32_t new_val);

void SharedMemoryBusInit(struct SharedMemoryBus *bus)
{
    if (bus == NULL)
        return;
    RingBufferInit(&bus->linux_to_rtos);
    RingBufferInit(&bus->rtos_to_linux);
    // bus->linux_heartbeat = 0;
    // bus->rtos_heartbeat = 0;
    DataMemoryBarrier();
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
    return buffer->head == buffer->tail;
}

size_t RingBufferWrite(struct RingBuffer *r_buffer, void *data, size_t size)
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
    if (size > occupied)
        return 0;
    size_t read = size;
    while (read--)
    {
        *dest++ = r_buffer->buffer[tail++];
        tail &= BUFFER_MASK;
    }
    DataMemoryBarrier();
    r_buffer->tail = tail;

    return size;
}


size_t RingBufferWrite_Atomic(struct RingBuffer *r_buffer, void *data, size_t size)
{
    if (r_buffer == NULL || data == NULL || size == 0)
        return 0;
        
    uint32_t old_head, next_head;
    while (1)
    {
        old_head = r_buffer->head;
        uint32_t tail = r_buffer->tail;
        size_t occupied = (old_head - tail) & BUFFER_MASK;
        size_t free_space = (BUFFER_SIZE - 1) - occupied;
        if (size > free_space)
            return 0;
        next_head = (old_head + size) & BUFFER_MASK;
        if (atomic_cas((uint32_t *)&r_buffer->head, old_head, next_head) == old_head)
        {
            break; 
        }
    }
    const uint8_t *src = (const uint8_t *)data;
    for (size_t i = 0; i < size; i++)
    {
        r_buffer->buffer[(old_head + i) & BUFFER_MASK] = src[i];
    }
    DataMemoryBarrier();
    return size;
}

size_t RingBufferRead_Atomic(struct RingBuffer *r_buffer, void *data, size_t size)
{
    if (r_buffer == NULL || data == NULL || size == 0)
        return 0;
    uint32_t old_tail, next_tail;
    while (1)
    {
        old_tail = r_buffer->tail;
        uint32_t head = r_buffer->head;
        size_t occupied = (head - old_tail) & BUFFER_MASK;
        if (size > occupied)
            return 0; 
        next_tail = (old_tail + size) & BUFFER_MASK;
        if (atomic_cas((uint32_t *)&r_buffer->tail, old_tail, next_tail) == old_tail)
        {
            break;
        }
    }
    uint8_t *dest = (uint8_t *)data;
    for (size_t i = 0; i < size; i++)
    {
        dest[i] = r_buffer->buffer[(old_tail + i) & BUFFER_MASK];
    }
    DataMemoryBarrier();
    return size;
}