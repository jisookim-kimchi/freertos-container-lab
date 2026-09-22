#include "exception.h"
#include "utils/print.h"

void sync_exception_handler()
{
    uint64_t esr = get_esr_el1();
    uint64_t far = get_far_el1();
    uint64_t elr = get_elr_el1();

    switch(ESR_EC(esr))
    {
        case INSTRUCT_ABORT_EL1:
        case INSTRUCT_ABORT_EL0:
              uart_puts("\nPANIC! : Instruction Abort (Invalid Code Fetch)\n");
              uart_puts("Fault PC (ELR): 0x");
              print_hex(elr);
              uart_puts("\n");
        break;

        case DATA_ABORT_EL1:
        case DATA_ABORT_EL0:
        {
            uint32_t dfsc = ESR_DFSC(esr);
            if (dfsc == FAULT_TRANSLATION_L0 || dfsc == FAULT_TRANSLATION_L1 || dfsc == FAULT_TRANSLATION_L2 || dfsc == FAULT_TRANSLATION_L3)
            {
                uart_puts("\nPANIC! : Translation fault!\n");
                //TODO remapping.
            }
            else if (dfsc == FAULT_PERMISSION_L1 || dfsc == FAULT_PERMISSION_L2 || dfsc == FAULT_PERMISSION_L3)
            {
                uart_puts("\nPANIC! : permission fault!\n");
            }
            else if (dfsc == FAULT_ADDRESS_SIZE_L0 || dfsc == FAULT_ADDRESS_SIZE_L1 || dfsc == FAULT_ADDRESS_SIZE_L2 || dfsc == FAULT_ADDRESS_SIZE_L3)
            {
                uart_puts("\nPANIC! : Address Size Fault!\n");
            }
            uart_puts("Access Type: ");
            uart_puts(ESR_WNR(esr) ? "WRITE\n" : "READ\n");
            uart_puts("Data Fault Status Code : 0x");
            print_hex(ESR_DFSC(esr));
            uart_puts("\n");
            uart_puts("Fault Address (FAR): 0x");
            print_hex(far);
            uart_puts("\n");
            uart_puts("Code Address (ELR): 0x");
            print_hex(elr);
            uart_puts("\n");
            break;
        }

        case SP_ALIGN_FAULT:
            uart_puts("\nPANIC! : Stack Pointer Alignment Fault!\n");
            uart_puts("Code Address (ELR): 0x");
            print_hex(elr);
            uart_puts("\n");
        break;
        
        case SERROR_INTERRUPT:
            uart_puts("\nPANIC! : SError (Asynchronous Hardware Error)!\n");
            uart_puts("ESR: 0x");
            print_hex(esr);
            uart_puts("\n");
        break;
        
        default:
            uart_puts("\nPANIC! : unknown Exception!\n");
            uart_puts("ESR_EC: 0x");
            print_hex(ESR_EC(esr));
            uart_puts("\n");
            uart_puts("FAR: 0x");
            print_hex(far);
            uart_puts("\n");
            uart_puts("ELR: 0x");
            print_hex(elr);
            uart_puts("\n");
        break;
    }
    while(1);
}