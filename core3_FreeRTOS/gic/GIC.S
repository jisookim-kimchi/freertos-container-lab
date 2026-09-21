
.global get_gic_dist_base
.global get_gic_cpu_base

get_gic_dist_base:
    ldr x0, =0x08000000
    ret

get_gic_cpu_base:
    ldr x0, =0x08010000
    ret