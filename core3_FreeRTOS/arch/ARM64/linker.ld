ENTRY(_start)

SECTIONS
{
    . = 0x40000000;

    .text : 
    {
        . = ALIGN(8);
        *(.text*)
    }
    .rodata : 
    {
        . = ALIGN(8);
        *(.rodata*)
    }
    . = 0x40010000;
    .data : 
    {
        . = ALIGN(8);
        *(.data*)
    }
    .bss : 
    {
        _bss_start = .;
        *(.bss*)
        *(COMMON)        
        _bss_end = .;     
    }
    . = 0x40020000;
    .stack :
    {
        _stack_bottom = .;
    }
    . = 0x40030000;

    _stack_top = .;

}