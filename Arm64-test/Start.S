.global _start

_start:
    ldr x0, =_bss_start
    ldr x1, =_bss_end
    ldr x0, =_stack_top
    mov sp, x0

1:
    cmp x0, x1
    b.ge 2f            
    str xzr, [x0], #8
    b 1b

2:
    bl main

3:
    b 3b
