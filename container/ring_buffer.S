

.global DataMemoryBarrier
.global DataSyncBarrier
.global ISB_flush
 
//odering garantee.
DataMemoryBarrier:
    DMB ISH
    ret;

//data receving complete to the DDR RAM.
DataSyncBarrier:
    DSB ISH
    ret

// Flush instruction prefetch pipeline
ISB_flush:
    ISB
    ret
