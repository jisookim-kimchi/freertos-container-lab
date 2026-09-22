
.global get_far_el1         //read FAR_EL1 reg.
.global get_esr_el1         //read ESR_EL1 reg.
.global get_elr_el1         //read ELR_EL1 reg.

get_far_el1:                // read FAR_EL1 reg. return fault address.
    MRS x0, FAR_EL1
    ISB
    RET

get_esr_el1:                // read ESR_EL1 reg. return error code(bit flags).
    MRS x0, ESR_EL1
    ISB
    RET

get_elr_el1:                // read ELR_EL1 reg. stores the program counter(address) of the exact instruction that caused the abort.
    MRS x0, ELR_EL1
    ISB
    RET

