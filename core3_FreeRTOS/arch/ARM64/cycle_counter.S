.global cycle_counter

cycle_counter:
    mrs x0, cntvct_el0 
    ret
