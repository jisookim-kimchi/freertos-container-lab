# FreeRTOS Container LAB

"Linux container의 isolation/lifecycle abstraction을 RTOS primitive로 재해석하고 구현하는 프로젝트"

Linux PID namespace → FreeRTOS에서는 Task/Resource View
Linux cgroup → Resource Budget/Policy
Linux mount namespace → Filesystem View
Linux execve() → Application Loading/Task Creation
Linux process isolation → MMU -> pagae table
Linux container runtime → Container Manager + Supervisor


MMU : hardware memory management unit
MMU(Hardware) → Virtual Memory → Page Table → CR3 → TLB → Page Fault



`L0 Table` (Root)
│
├── Entry 0
│      ↓
│   `L1 Table`
│   │
│   ├── Entry 0
│   │      ↓
│   │   `L2 Table`
│   │   │
│   │   ├── Entry 0
│   │   │      ↓
│   │   │   `L3 Table`
│   │   │   │
│   │   │   ├── Entry 0 → Physical memory
│   │   │   ├── Entry 1 → Physical memory
│   │   │   ├── Entry 2 → Physical memory
│   │   │   └── ...
│   │   │
│   │   └── Entry 1 → `L3 Table`
│   │
│   └── Entry 1 → `L2 Table`
│
├── Entry 1
│      ↓
│   `L1 Table`
│   └── ...
│
└── Entry 2
       ↓
    `L1 Table`
       └── ...


## available memory page table size
`L0 Table [512 * 8 Byte]`  4kib
-> `L1 Table [512 * *512 * 8byte]` 2mib
->`L2 Table [512 * 512 * 512 * 8byte]` 1gib 
->`L3 Table [512 * 512 * 512 * 512 * 8byte]` 512gib 
-> physical page size(4kib) * 512gib = 256 TiB (maximum mapped physical memory)

L0: 1 × 4 KiB  =  4 KiB
L1: 4 × 4 KiB  = 16 KiB
L2: 8 × 4 KiB  = 32 KiB
L3: 16 × 4 KiB = 64 KiB

total = 116 
KiB= 16 × 2 MiB = 32 MiB(max mapped memory)


[47:39] [38:30] [29:21] [20:12] [11:0]
   L0      L1      L2      L3    offset


`L0_PAGE_TABLE_BASE` = 0x40100000;
MemoryArea -> virt_base;

`each page table` has `512 entries`. 
`total page tables` = 29.
