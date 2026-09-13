ENTRY(_start)

SECTIONS
{
    . = 0x7F000000;

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
    . = 0x7F010000;
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
    . = 0x7F020000;
    .stack :
    {
        _stack_bottom = .;
    }
    . = 0x7F030000;

    _stack_top = .;

}