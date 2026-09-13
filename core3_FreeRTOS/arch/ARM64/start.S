.global _start

_start:
    ldr x2, =_stack_top
    mov sp, x2

    // Enable FP/SIMD (Floating Point & NEON)
    mov x0, #(3 << 20)
    msr cpacr_el1, x0
    isb

    // Clear BSS
    ldr x0, =_bss_start
    ldr x1, =_bss_end
1:
    cmp x0, x1
    b.ge 2f            
    str xzr, [x0], #8
    b 1b

2:
    bl main

3:
    b 3b
