#ifndef GIC_H
#define GIC_H

/*
    we use here GICv2
    GICv3 is for Cloud data center..
*/
#include <stdint.h>

#define GIC_DIST_BASE          0x08000000ULL         // Distributor
#define GIC_CPU_BASE           0x08010000ULL         // CPU Interface

extern uint64_t get_gic_dist_base(void);
extern uint64_t get_gic_cpu_base(void);

/*
 * ARM GIC Memory Map Offset
 * 0x00000-0x01FFF  CPU interface
 * 0x02000-0x0FFFF  Reserved
 * 0x10000-0x10FFF  Virtual interface control
 * 0x11000-0x1FFFF  Reserved
 * 0x20000-0x21FFF  Virtual CPU interface (4KB page offset)
 * 0x22000-0x2EFFF  Reserved
 * 0x2F000-0x30FFF  Alias of the Virtual CPU interface (64KB page offset alias)
 * 0x31FFF-0x3FFFF  Reserved
 */

/*
    GIC CPU INTERFACE REGs. 
    manage incomming interrupts in a core
*/
#define GICC_CTLR              (GIC_CPU_BASE + 0x0000) // CPU Interface Control Reg.                        R/W
#define GICC_PMR               (GIC_CPU_BASE + 0x0004) // Interrupt Priority Mask Register                  R/W
#define GICC_BPR               (GIC_CPU_BASE + 0x0008) // Binary Point Reg.(Group Priority, Subpriority)    R/W
#define GICC_IAR               (GIC_CPU_BASE + 0x000C) // Interrupt Ack Reg.                                R/O
#define GICC_EOIR              (GIC_CPU_BASE + 0x0010) // End of Interrupt Reg.                             W/O
#define GICC_RPR               (GIC_CPU_BASE + 0x0014) // Running Priority Reg.                             R/O
#define GICC_HPPIR             (GIC_CPU_BASE + 0x0018) // Highest Priority Pending Interrupt Reg.           R/O
#define GICC_ABPR              (GIC_CPU_BASE + 0x001C) // Aliased Binary Point Reg.                         R/W 
#define GICC_AIAR              (GIC_CPU_BASE + 0x0020) // Aliased Interrupt Acknowledge Reg.                R/O
#define GICC_AEOIR             (GIC_CPU_BASE + 0x0024) // Aliased End of Interrupt Reg.                     W/O
#define GICC_AHPPIR            (GIC_CPU_BASE + 0x0028) // Aliased Highest Priority Pending Interrupt Reg.   R/O
#define GICC_APR0              (GIC_CPU_BASE + 0x00D0) // Interrupt Priority Reg. 0-3                       R/W
#define GICC_NSAPRO            (GIC_CPU_BASE + 0x00E0) // Non-secure Access Priority Reg.                   R/W
#define GICC_IIDR              (GIC_CPU_BASE + 0x00FC) // CPU Interface Identification Reg.                 R/O
#define GICC_DIR               (GIC_CPU_BASE + 0x1000) // Deactivate Interrupt Reg.                         W/O

/*
    GIC Distributor
    manage interrupts to distribute to each cores
*/
#define GICD_CTLR              (GIC_DIST_BASE + 0x000) // Distributor Control Reg.                                R/W
#define GICD_TYPER             (GIC_DIST_BASE + 0x004) // Interrupt Controller Type Reg.                          R/O
#define GICD_IIDR              (GIC_DIST_BASE + 0x008) // Distributor Implementer Identification Reg.             R/O
#define GICD_IGROUPR0          (GIC_DIST_BASE + 0x080) // Interrupt Group Reg. 0                                  R/W
#define GICD_ISENABLER         (GIC_DIST_BASE + 0x100) // Interrupt Set-Enable Registers                          R/W
#define GICD_ICENABLER         (GIC_DIST_BASE + 0x180) // Interrupt Clear-Enable Registers                        R/W
#define GICD_ISPENDR           (GIC_DIST_BASE + 0x200) // Interrupt Set-Pending Registers                         R/W
#define GICD_ICPENDR           (GIC_DIST_BASE + 0x280) // Interrupt Clear-Pending Registers                       R/W
#define GICD_ISACTIVER         (GIC_DIST_BASE + 0x300) // Interrupt Set-Active Registers                          R/W
#define GICD_ICACTIVER         (GIC_DIST_BASE + 0x380) // Interrupt Clear-Active Registers                        R/W
#define GICD_IPRIORITYR0       (GIC_DIST_BASE + 0x400) // Interrupt Priority Registers 0-3                        R/W
#define GICD_ITARGETSR         (GIC_DIST_BASE + 0x800) // Interrupt Processor Targets Registers[i]                R/W
#define GICD_ICFGR             (GIC_DIST_BASE + 0xC00) // Interrupt Configuration Registers                       R/W
#define GICD_PPISR             (GIC_DIST_BASE + 0xD00) // Private Peripheral Interrupt Status Register            R/O
#define GICD_SPISR             (GIC_DIST_BASE + 0xD04) // Shared Peripheral Interrupt Status Registers            R/O
#define GICD_SGIR              (GIC_DIST_BASE + 0xF00) // Software Generated Interrupt Register                   R/W
#define GICD_CPENDSGIR         (GIC_DIST_BASE + 0xF10) // SGI Pending Registers                                   R/W
#define GICD_SPENDSGIR         (GIC_DIST_BASE + 0xF20) // SGI Pending Registers                                   R/W                 


/*
    GIC Distributor Array Registers (n = register index by 4byte)
*/
#define GICD_ISENABLER(n)      (GIC_DIST_BASE + 0x100 + ((n) * 4)) // Interrupt Set-Enable (n=0..31)
#define GICD_ICENABLER(n)      (GIC_DIST_BASE + 0x180 + ((n) * 4)) // Interrupt Clear-Enable (n=0..31)
#define GICD_ISPENDR(n)        (GIC_DIST_BASE + 0x200 + ((n) * 4)) // Set-Pending (n=0..31)
#define GICD_ICPENDR(n)        (GIC_DIST_BASE + 0x280 + ((n) * 4)) // Clear-Pending (n=0..31)
#define GICD_IPRIORITYR(n)     (GIC_DIST_BASE + 0x400 + ((n) * 4)) // Priority (8bit * 4 = 4 IRQ per register, n=0..254)
#define GICD_ITARGETSR(n)      (GIC_DIST_BASE + 0x800 + ((n) * 4)) // Target Core (8bit * 4 = 4 IRQ per register, n=0..254)
#define GICD_ICFGR(n)          (GIC_DIST_BASE + 0xC00 + ((n) * 4)) // Config (Edge/Level, 2bit * 16 = 16 IRQ per register, n=0..63)
#define GICD_PPISR             (GIC_DIST_BASE + 0xD00)             // PPI Status (0~31)
#define GICD_SPISR(n)          (GIC_DIST_BASE + 0xD04 + (((n) - 1) * 4)) // SPI Status (n=1..31)

#define GIC_DISTRIBUTOR_ON()     (*((volatile uint32_t *)GICD_CTLR) |= (1U << 0))
#define GIC_DISTRIBUTOR_OFF()    (*((volatile uint32_t *)GICD_CTLR) &= ~(1U << 0))
#define GIC_DIST_INT_ON(irq)     (*((volatile uint32_t *)GICD_ISENABLER((irq) / 32)) = (1U << ((irq) % 32)))
#define GIC_DIST_INT_OFF(irq)    (*((volatile uint32_t *)GICD_ICENABLER((irq) / 32)) = (1U << ((irq) % 32)))

#define GIC_INTERFACE_ON()       (*((volatile uint32_t *)GICC_CTLR) |= (1U << 0))
#define GIC_INTERFACE_OFF()      (*((volatile uint32_t *)GICC_CTLR) &= ~(1U << 0))

/*
    Set Priority Level(0 ~ N)
*/
#define GIC_PRIO_16_LEVELS(level)     (((level) & 0x0F) << 4) // 0~15 Bit [3:0] = 0b0000
#define GIC_PRIO_32_LEVELS(level)     (((level) & 0x1F) << 3) // 0~31 Bit [2:0] = 0b000
#define GIC_PRIO_64_LEVELS(level)     (((level) & 0x3F) << 2) // 0~63 Bit [1:0] = 0b00
#define GIC_PRIO_128_LEVELS(level)    (((level) & 0x7F) << 1) // 0~127 Bit [0] = 0b0
#define GIC_PRIO_256_LEVELS(level)    ((level) & 0xFF)        // 0~255 Bit [0] = 0b0

#define GIC_CPU_PRIO_SET(priority)    (*((volatile uint32_t *)GICC_PMR) = GIC_PRIO_256_LEVELS(priority))
#define GIC_CPU_PRIO_READ()           (*((volatile uint32_t *)GICC_PMR))
#define GIC_CPU_PRIO_READ_MASKED()    ((*((volatile uint32_t *)GICC_PMR)) & 0b11111111)

/* 
    Priority Implemented Bit Masks
*/
#define GIC_PRIO_MASK_16_LEVELS       0xF0U  // Bits [7:4] implemented (Bit [3:0] = 0)
#define GIC_PRIO_MASK_32_LEVELS       0xF8U  // Bits [7:3] implemented (Bit [2:0] = 0)
#define GIC_PRIO_MASK_64_LEVELS       0xFCU  // Bits [7:2] implemented (Bit [1:0] = 0)
#define GIC_PRIO_MASK_128_LEVELS      0xFEU  // Bits [7:1] implemented (Bit [0]   = 0)
#define GIC_PRIO_MASK_256_LEVELS      0xFFU  // Bits [7:0] implemented (All bits)

/*
    Binary Point Register (BPR) Configuration (0 ~ 7) [2:0]
*/
#define GIC_CPU_SET_BPR(bpr)          (*((volatile uint32_t *)GICC_BPR) = ((bpr) & 0b111))
#define GIC_CPU_READ_BPR()            ((*((volatile uint32_t *)GICC_BPR)) & 0b111)

#endif