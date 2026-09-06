#ifndef MEMORY_H
#define MEMORY_H

#include "../config.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define L0_PAGE_PHY_TABLE_BASE 0x40100000
#define L1_PAGE_PHY_TABLE_BASE 0x40200000
#define L2_PAGE_PHY_TABLE_BASE 0x40300000
#define L3_PAGE_PHY_TABLE_BASE 0x40400000

/*
    Memory Area structure
*/
struct MemoryArea
{
    uintptr_t phy_base;
    uintptr_t virt_base;
    size_t size;
    uint8_t permission;
};

/*
    Page Table Entry
*/
struct PageTableEntry
{
    uintptr_t next_level_phy_base;
    uint8_t permission;
};

/*
    Page Table Structure (512 entries per table)
*/
struct PageTable
{
    uintptr_t phy_base;
    struct PageTableEntry entries[PT_ENTRIES];
};

void MemoryAreaInit(struct MemoryArea *area, void *virt_base, void *phy_base, size_t size, uint8_t permission);
bool IsInMemoryArea(struct MemoryArea *area, void *addr);
bool IsAccessAllowed(struct MemoryArea *area, uint8_t permission);

struct PageTable *PageTableAllocate(uint8_t level);
void PageTableFree(uint8_t level, struct PageTable *table);
struct PageTable *PageTableGet(uint8_t index, uint8_t level);
struct PageTable *GetTableFromPA(uint8_t level, uintptr_t pa);

void page_table_map(struct PageTable *root_table, uint8_t level, struct MemoryArea *area);

/* Hardware MMU Assembly Functions (mmu.S) */
extern void enable_mmu(uintptr_t L0_table_pa, uint64_t tcr, uint64_t mair);
extern void disable_mmu(void);
extern void flush_tlb(void);

#endif 
