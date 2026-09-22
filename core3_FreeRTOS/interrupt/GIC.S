
.global get_gic_dist_base
.global get_gic_cpu_base
.global enable_interrupt
.global disable_interrupt


get_gic_dist_base:
    ldr x0, =0x08000000
    ret

get_gic_cpu_base:
    ldr x0, =0x08010000
    ret

enable_interrupt:
    msr daifclr, #2
    ret

disable_interrupt:
    msr daifset, #2
    ret
