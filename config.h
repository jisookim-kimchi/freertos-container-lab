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

#endif