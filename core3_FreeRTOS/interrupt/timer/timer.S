
.global get_timer_clock_frequency


get_timer_clock_frequency: // frequency of 1 sec.
    mrs x0, cntfrq_el0
    ret

.global get_current_physical_count //current physical count

get_current_physical_count:
    mrs x0, cntpct_el0
    ret

.global set_timer_interval
set_timer_interval:
    msr cntp_tval_el0, x0
    ret

.global enable_timer
enable_timer:
    mov x0, #1
    msr cntp_ctl_el0, x0
    ret