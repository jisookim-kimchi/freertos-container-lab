.global read_cycle_count

read_cycle_count:
    mrs x0, cntvct_el0 
    ret
