#ifndef MEMORY_H
#define MEMORY_H

#include "../config.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static uint8_t L0_bitmap;
static uint8_t L1_bitmap;
static uint8_t L2_bitmap;
static uint16_t L3_bitmap;

#define L0_PAGE_TABLE_BASE 0x40100000
#define L1_PAGE_TABLE_BASE 0x40200000
#define L2_PAGE_TABLE_BASE 0x40300000
#define L3_PAGE_TABLE_BASE 0x40400000

/*
    *base : starting address of memory area.
    *size : size of memory area.
    *permission : permission of memory area.
    text (code) -> .data/.bss -> stack
      r/x       ->    r/w     ->  r/w
*/
struct MemoryArea
{
    uintptr_t phy_base;
    uintptr_t virt_base;
    size_t size;
    uint8_t permission;
};

/*
    level 0 ->
    level 1 ->
    level 2 ->
    level 3 ->
    output physical address.
*/
struct PageTableEntry
{
    uintptr_t next_level_phy_base;
};

struct PageTable
{
    uintptr_t phy_base;
    struct PageTableEntry entries[PT_ENTRIES];
};

static struct PageTable L0_pool[MAX_L0_TABLES];
static struct PageTable L1_pool[MAX_L1_TABLES];
static struct PageTable L2_pool[MAX_L2_TABLES];
static struct PageTable L3_pool[MAX_L3_TABLES];

void PageTableMap
(
    struct PageTable *L0,
    struct PageTable *L1,
    struct PageTable *L2,
    struct PageTable *L3,
    uintptr_t virt_addr,
    uintptr_t phy_addr
);

#endif