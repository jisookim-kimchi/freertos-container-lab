// mmu.S

.global enable_mmu
.global disable_mmu
.global flush_tlb           //Translation Lookaside Buffer.
.global switch_mmu_table    //change MMU table.

//MSR: Move to System Register.
//MRS: Move Register from System Register.
enable_mmu:
    MSR TTBR0_EL1, x0       // 1. LO root page table physic's address register.
    MSR TCR_EL1, x1         // 2. 48 bit VA / 4kb page table size inform to MMU.
    MSR MAIR_EL1, x2        // 3. MAIR register write, where cache on/off
    ISB                     // Synchronization.

    MRS x3, SCTLR_EL1       // 4. read current SCTLR_EL1 register.
    ORR x3, x3, #1          // 5. MMU switch on.
    MSR SCTLR_EL1, x3       // 6. SCTLR_EL1 register write
    ISB

    RET

disable_mmu:
    MRS x3, SCTLR_EL1       // 1. read current SCTLR_EL1 register.
    BIC x3, x3, #1          // 2. MMU switch off.
    MSR SCTLR_EL1, x3       // 3. SCTLR_EL1 register write.
    ISB                     // Synchronization.

    RET

flush_tlb:
    DC IVAC, x0             // Data Inner & Outer Invalidate
    DSB SY                  // Data Synchronization Barrier
    TLBI VMALLE1IS          // TLB Invalidate
    ISB                     

    RET

switch_mmu_table:
    MSR TTBR0_EL1, x0       // 1. LO root page table physic's address register.
    ISB

    RET
