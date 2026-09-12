

.global DataMemoryBarrier
.global DataSyncBarrier
.global ISB_flush
 
DataMemoryBarrier:
    DMB ISH
    ret;

DataSyncBarrier:
    DSB ISH
    ret

ISB_flush:
    ISB
    ret
