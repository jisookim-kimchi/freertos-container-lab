#include "memory.h"

static uint8_t L0_bitmap = 0;
static uint8_t L1_bitmap = 0;
static uint8_t L2_bitmap = 0;
static uint16_t L3_bitmap = 0;

static struct PageTable L0_pool[MAX_L0_TABLES];
static struct PageTable L1_pool[MAX_L1_TABLES];
static struct PageTable L2_pool[MAX_L2_TABLES];
static struct PageTable L3_pool[MAX_L3_TABLES];

void MemoryAreaInit( struct MemoryArea *area, void *virt_base, void *phy_base, size_t size, uint8_t permission)
{
    if (area == NULL || size == 0)
        return;
    area->virt_base = (uintptr_t)virt_base;
    area->phy_base = (uintptr_t)phy_base;
    area->size = size;
    area->permission = permission;
}

bool IsInMemoryArea(struct MemoryArea *area, void *addr)
{
    if (area == NULL || addr == NULL)
        return false;
    uintptr_t start = area->virt_base;
    uintptr_t end = start + area->size;
    uintptr_t address = (uintptr_t)addr;

    return address >= start && address < end;
}

bool IsAccessAllowed(struct MemoryArea *area, uint8_t permission)
{
    if (area == NULL)
        return false;
    return ((area->permission & permission) == permission);
}

struct PageTable *PageTableAllocate(uint8_t level)
{   
    if (level == 0)
        {
            for (int i = 0; i < MAX_L0_TABLES; i++)
            {
                if (!(L0_bitmap & (1 << i)))
                {
                    L0_bitmap |= (1 << i);
                    L0_pool[i].phy_base = L0_PAGE_PHY_TABLE_BASE + i * CONTAINER_PAGE_SIZE;
                    return &L0_pool[i];
                }
            }
    }
    else if (level == 1)
    {
        for (int i = 0; i < MAX_L1_TABLES; i++)
        {
            if (!(L1_bitmap & (1 << i)))
            {
                L1_bitmap |= (1 << i);
                L1_pool[i].phy_base = L1_PAGE_PHY_TABLE_BASE + i * CONTAINER_PAGE_SIZE;
                return &L1_pool[i];
            }
        }
    }
    else if (level == 2)
    {
        for (int i = 0; i < MAX_L2_TABLES; i++)
        {
            if (!(L2_bitmap & (1 << i)))
            {
                L2_bitmap |= (1 << i);
                L2_pool[i].phy_base = L2_PAGE_PHY_TABLE_BASE + i * CONTAINER_PAGE_SIZE;
                return &L2_pool[i];
            }
        }
    }
    else if (level == 3)
    {
        for (int i = 0; i < MAX_L3_TABLES; i++)
        {
            if (!(L3_bitmap & (1 << i)))
            {
                L3_bitmap |= (1 << i);
                L3_pool[i].phy_base = L3_PAGE_PHY_TABLE_BASE + i * CONTAINER_PAGE_SIZE;
                return &L3_pool[i];
            }
        }
    }
    return NULL;
}

void PageTableFree(uint8_t level, struct PageTable *table)
{
    if (level == 0)
    {
        uint8_t index = table - L0_pool;

        if (index < MAX_L0_TABLES)
        {
            L0_bitmap &= ~(1 << index);
        }
    }
    else if (level == 1)
    {
        uint8_t index = table - L1_pool;
        if (index < MAX_L1_TABLES)
        {
            L1_bitmap &= ~(1 << index);
        }
    }
    else if (level == 2)
    {
        uint8_t index = table - L2_pool;
        if (index < MAX_L2_TABLES)
        {
            L2_bitmap &= ~(1 << index);
        }
    }
    else if (level == 3)
    {
        uint8_t index = table - L3_pool;
        if (index < MAX_L3_TABLES)
        {
            L3_bitmap &= ~(1 << index);
        }
    }
}

struct PageTable *PageTableGet(uint8_t index, uint8_t level)
{
    if (level == 0)
    {
        if (index >= MAX_L0_TABLES)
            return NULL;
        return &L0_pool[index];
    }

    if (level == 1)
    {
        if (index >= MAX_L1_TABLES)
            return NULL;
        return &L1_pool[index];
    }

    if (level == 2)
    {
        if (index >= MAX_L2_TABLES)
            return NULL;
        return &L2_pool[index];
    }

    if (level == 3)
    {
        if (index >= MAX_L3_TABLES)
            return NULL;
        return &L3_pool[index];
    }

    return NULL;
}

struct PageTable *GetTableFromPA(uint8_t level, uintptr_t pa)
{
    if (pa == 0)
        return NULL;

    if (level == 1)
    {
        uint32_t idx = (pa - L1_PAGE_PHY_TABLE_BASE) / CONTAINER_PAGE_SIZE;
        return PageTableGet(idx, 1);
    }
    else if (level == 2)
    {
        uint32_t idx = (pa - L2_PAGE_PHY_TABLE_BASE) / CONTAINER_PAGE_SIZE;
        return PageTableGet(idx, 2);
    }
    else if (level == 3)
    {
        uint32_t idx = (pa - L3_PAGE_PHY_TABLE_BASE) / CONTAINER_PAGE_SIZE;
        return PageTableGet(idx, 3);
    }

    return NULL;
}

void page_table_map(struct PageTable *root_table, uint8_t level, struct MemoryArea *area)
{
    // Get required_page_count */
    uintptr_t virt_mem = area->virt_base;
    uintptr_t phy_mem = area->phy_base;
    size_t required_page_count = (area->size + CONTAINER_PAGE_SIZE - 1) / CONTAINER_PAGE_SIZE;

    
    for (size_t i = 0; i < required_page_count; i++)
    {
        uintptr_t va = virt_mem + (i * CONTAINER_PAGE_SIZE);
        uintptr_t pa = phy_mem + (i * CONTAINER_PAGE_SIZE);
        uint64_t L0_entry_index = (va >> 39) & 0b111111111;
        uint64_t L1_entry_index = (va >> 30) & 0b111111111;
        uint64_t L2_entry_index = (va >> 21) & 0b111111111;
        uint64_t L3_entry_index = (va >> 12) & 0b111111111;
        struct PageTable *L1 = NULL;
        if (root_table->entries[L0_entry_index].next_level_phy_base == 0)
        {
            L1 = PageTableAllocate(1);
            root_table->entries[L0_entry_index].next_level_phy_base = L1->phy_base;
        }
        else
        {
            L1 = GetTableFromPA(1, root_table->entries[L0_entry_index].next_level_phy_base);
        }
        struct PageTable *L2 = NULL;
        if (L1->entries[L1_entry_index].next_level_phy_base == 0)
        {
            L2 = PageTableAllocate(2);
            L1->entries[L1_entry_index].next_level_phy_base = L2->phy_base;
        }
        else
        {
            L2 = GetTableFromPA(2, L1->entries[L1_entry_index].next_level_phy_base);
        }
        struct PageTable *L3 = NULL;
        if (L2->entries[L2_entry_index].next_level_phy_base == 0)
        {
            L3 = PageTableAllocate(3);
            L2->entries[L2_entry_index].next_level_phy_base = L3->phy_base;
        }
        else
        {
            L3 = GetTableFromPA(3, L2->entries[L2_entry_index].next_level_phy_base);
        }
        L3->entries[L3_entry_index].next_level_phy_base = pa;
        L3->entries[L3_entry_index].permission = area->permission;
    }
    return ;
}

