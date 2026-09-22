#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdint.h> 

extern uint64_t get_far_el1(void);
extern uint64_t get_esr_el1(void);
extern uint64_t get_elr_el1(void);

/*
    esr_el1
*/
#define ESR_EC(esr)  (((esr) >> 26) & 0b111111)          // get exception class.
#define ESR_DFSC(esr)        ((esr) & 0b111111)          // get fault status code.
#define ESR_WNR(esr)         (((esr) >> 6) & 0b1)        // bit6 (write not read)

#define DATA_ABORT_EL1       0b100101                   // at EL1 Data accesses, Alignemnt fault, Hardware memory error : External abort / ECC.
#define DATA_ABORT_EL0       0b100100                   // at EL0 
#define SP_ALIGN_FAULT       0b100110                   // SP alignment fault.
#define INSTRUCT_ABORT_EL1   0b100000                   // Instruction Abort from a lower Exception level.
#define INSTRUCT_ABORT_EL0   0b100000                   // EL0 Instruction Abort.
#define SERROR_INTERRUPT     0b101111                   // SYstem Error interrupt.

#define FAULT_TRANSLATION_L0    0b000100                   // Level 0 Translation Fault (Page Fault)
#define FAULT_TRANSLATION_L1    0b000101                   // Level 1 Translation Fault
#define FAULT_TRANSLATION_L2    0b000110                   // Level 2 Translation Fault
#define FAULT_TRANSLATION_L3    0b000111                   // Level 3 Translation Fault
#define FAULT_PERMISSION_L1     0b001101                   // Level 1 Permission Fault (w/r/x)
#define FAULT_PERMISSION_L2     0b001110                   // Level 2 Permission Fault
#define FAULT_PERMISSION_L3     0b001111                   // Level 3 Permission Fault
#define FAULT_ADDRESS_SIZE_L0   0b000000                   // Level 0 Address size fault
#define FAULT_ADDRESS_SIZE_L1   0b000001                   // Level 1 Address size fault
#define FAULT_ADDRESS_SIZE_L2   0b000010                   // Level 2 Address size fault
#define FAULT_ADDRESS_SIZE_L3   0b000011                   // Level 3 Address size fault



/*
    
*/

#endif