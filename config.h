#ifndef CONFIG_H
#define CONFIG_H

/* Memory */
#define CONTAINER_PAGE_SIZE       4096
#define CONTAINER_MEMORY_AREAS    3

/* Permission */
#define MEM_READ                  (1 << 0)
#define MEM_WRITE                 (1 << 1)
#define MEM_EXEC                  (1 << 2)

/* Page Table */
#define PT_ENTRIES                512
#define PT_LEVELS                 4
#define PT_ENTRY_SIZE             8

#define PT_ENTRIES 512
#define PT_LEVELS  4

#define MAX_L0_TABLES   1
#define MAX_L1_TABLES   4
#define MAX_L2_TABLES   8
#define MAX_L3_TABLES   16

#define MAX_PAGE_TABLES (MAX_L0_TABLES + MAX_L1_TABLES + MAX_L2_TABLES + MAX_L3_TABLES)


/* MAIR MEM ATTRIBUTE */
#define MAIR_ATTR_NORMAL_RAM         0xFFULL // RAM cache on
#define MAIR_ATTR_DEVICE_MMIO        0x04ULL // Device cache off
#define MAIR_VALUE                   ((MAIR_ATTR_DEVICE_MMIO << 8) | (MAIR_ATTR_NORMAL_RAM << 0))

/* TCR (Translation Control Register) */
#define TCR_TT0_SIZE_48BIT           (16ULL << 0)  // 48 bit VA
#define TCR_IRGN0_WBWA               (1ULL << 8)   // Write-Back Inner cache
#define TCR_ORGN0_WBWA               (1ULL << 10)  // Write-Back Outer cache
#define TCR_SH0_INNER                (3ULL << 12)  // Inner Shareable
#define TCR_TG0_4KB                  (0ULL << 14)  // 4KB Page
#define TCR_EPD1_DISABLE             (1ULL << 23)  // TTBR1 Disable
#define TCR_IPS_48BIT_PA             (5ULL << 32)  // 48 bit PA

#define TCR_VALUE                    (TCR_TT0_SIZE_48BIT | TCR_IRGN0_WBWA | TCR_ORGN0_WBWA | \
                                      TCR_SH0_INNER | TCR_TG0_4KB | TCR_EPD1_DISABLE | TCR_IPS_48BIT_PA)

#endif