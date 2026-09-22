.balign 2048

.global get_vector_table
.global set_vector_table

vector_table:
    //
    // Group 1: Current EL with SP0 (EL1 with sp0)
    //

    .balign 128
    b   sync_exception_handler      // 0x000: Synchronous 
    .balign 128
    b   irq_exception_handler       // 0x080: IRQ Interrupt Request
    .balign 128
    b   .                           // 0x100: FIQ Fast Interrupt Reqeust
    .balign 128
    b   sync_exception_handler      // 0x180: SError System error

    //
    // Group 2: Current EL with SPx (EL1 with SP1...)
    //

    .balign 128
    b   sync_exception_handler      // 0x200: Synchronous
    .balign 128
    b   irq_exception_handler       // 0x280: IRQ
    .balign 128
    b   .                           // 0x300: FIQ
    .balign 128
    b   sync_exception_handler      // 0x380: SError
    
    //
    // Group 3: Lower EL using AArch64 (EL0 user mode)
    //

    .balign 128
    b   sync_exception_handler      // 0x400: Synchronous
    .balign 128
    b   irq_exception_handler       // 0x480: IRQ
    .balign 128
    b   .                           // 0x500: FIQ
    .balign 128
    b   sync_exception_handler      // 0x580: SError

    //
    // Group 4: Lower EL using AArch32 (32bit mode)
    //

    .balign 128
    b   .                           // 0x600: Synchronous
    .balign 128
    b   .                           // 0x680: IRQ
    .balign 128
    b   .                           // 0x700: FIQ
    .balign 128
    b   .                           // 0x780: SError


get_vector_table:
    mrs x0, vbar_el1
    ret

set_vector_table:
    adr x0, vector_table
    msr vbar_el1, x0
    ISB
    ret


    