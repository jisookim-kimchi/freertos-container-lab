#include "ring_buffer.h"

extern void DataMemoryBarrier();
extern void DataSyncBarrier();
extern void ISB_flush();

/*
    @brief  : 
*/
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