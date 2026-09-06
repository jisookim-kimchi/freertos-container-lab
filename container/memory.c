#include "memory.h"

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
                    L0_pool[i].phy_base = L0_PAGE_TABLE_BASE + i * CONTAINER_PAGE_SIZE;
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
                L1_pool[i].phy_base = L1_PAGE_TABLE_BASE + i * CONTAINER_PAGE_SIZE;
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
                L2_pool[i].phy_base = L2_PAGE_TABLE_BASE + i * CONTAINER_PAGE_SIZE;
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
                L3_pool[i].phy_base = L3_PAGE_TABLE_BASE + i * CONTAINER_PAGE_SIZE;
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

// void PageTableLink(struct PageTable *root_table, uint8_t level, struct MemoryArea *area)
// {
//     if (root_table == NULL || area == NULL)
//         return;

//     if (level >= PT_LEVELS)
//         return;

//     if (level == 0)
//     {
//         uintptr_t virt_addr = area->virt_base;

//         uint64_t L0_entry_index =
//             (virt_addr >> 39) & 0b111111111;
//         root_table->entries[L0_entry_index].next_level_phy_base = L1_table->phy_base;
//     }
// }