/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright 2014-2016 Freescale Semiconductor, Inc.
 * Copyright 2017,2019-2020 NXP
 *
 */
#ifndef _FSL_S32V234_CPU_H
#define _FSL_S32V234_CPU_H

#include <config.h>

#ifndef CONFIG_SYS_DCACHE_OFF
/*
 * To start MMU before DDR is available, we create MMU table in SRAM.
 * The base address of SRAM is CONFIG_SYS_FSL_OCRAM_BASE. We use three
 * levels of translation tables here to cover 40-bit address space.
 * We use 4KB granule size, with 40 bits physical address, T0SZ=24
 * Level 0 IA[39], table address @0
 * Level 1 IA[38:30], table address @0x1000, 0x2000
 * Level 2 IA[29:21], table address @0x3000
 */

#define SECTION_SHIFT_L0	39UL
#define SECTION_SHIFT_L1	30UL
#define SECTION_SHIFT_L2	21UL
#define BLOCK_SIZE_L0	0x8000000000UL
#define BLOCK_SIZE_L1	(1 << SECTION_SHIFT_L1)
#define BLOCK_SIZE_L2	(1 << SECTION_SHIFT_L2)
#define NUM_OF_ENTRY            512
#define TCR_EL2_PS_40BIT	(2 << 16)
#define S32V_VA_BITS		(40)
#define S32V_TCR	(TCR_TG0_4K		| \
			TCR_EL2_PS_40BIT	| \
			TCR_SHARED_NON		| \
			TCR_ORGN_NC		| \
			TCR_IRGN_NC		| \
			TCR_T0SZ(S32V_VA_BITS))
#define S32V_TCR_FINAL (TCR_TG0_4K		| \
			TCR_EL2_PS_40BIT	| \
			TCR_SHARED_OUTER	| \
			TCR_ORGN_WBWA		| \
			TCR_IRGN_WBWA		| \
			TCR_T0SZ(S32V_VA_BITS))

#define CONFIG_SYS_FSL_IRAM_BASE        (IRAM_BASE_ADDR)

#define CONFIG_SYS_FSL_IRAM_SIZE        (CONFIG_SYS_MEM_SIZE)

#define CONFIG_SYS_FSL_DRAM_BASE1       0x80000000
#define CONFIG_SYS_FSL_DRAM_SIZE1       0x40000000
#define CONFIG_SYS_FSL_DRAM_BASE2       0xC0000000
#define CONFIG_SYS_FSL_DRAM_SIZE2       0x40000000
#define CONFIG_SYS_FSL_PERIPH_BASE      0x40000000

#if defined(CONFIG_S32V234)
#define CONFIG_SYS_FSL_PERIPH_SIZE      0x40000000
#elif defined(CONFIG_S32_GEN1)
#define CONFIG_SYS_FSL_PERIPH_SIZE      0x20000000
#endif

#ifdef CONFIG_PCIE_S32GEN1
/* TODO: These should go to defconfig, or even better,
 * be read from device tree
 */
#define CONFIG_SYS_PCIE0_PHYS_ADDR_LO		0x53000000ULL
#define CONFIG_SYS_PCIE1_PHYS_ADDR_LO		0x45000000ULL
#define CONFIG_SYS_PCIE0_PHYS_SIZE_LO		0x1000000
#define CONFIG_SYS_PCIE1_PHYS_SIZE_LO		0x1000000
#define CONFIG_SYS_PCIE0_PHYS_ADDR_HI		0x5800000000ULL
#define CONFIG_SYS_PCIE1_PHYS_ADDR_HI		0x4800000000ULL
#define CONFIG_SYS_PCIE0_PHYS_SIZE_HI		0x200000000
#define CONFIG_SYS_PCIE1_PHYS_SIZE_HI		0x200000000
#endif

struct sys_mmu_table {
	u64 virt_addr;
	u64 phys_addr;
	u64 size;
	u64 memory_type;
	u64 share;
};

struct table_info {
	u64 *ptr;
	u64 table_base;
	u64 entry_size;
};

static const struct sys_mmu_table s32_early_mmu_table[] = {
#ifdef CONFIG_TARGET_TYPE_S32GEN1_EMULATOR
	{ CONFIG_SYS_FSL_DRAM_BASE1, CONFIG_SYS_FSL_DRAM_BASE1,
	  CONFIG_SYS_FSL_DRAM_SIZE1, MT_NORMAL, PTE_BLOCK_OUTER_SHARE },
#else
	{ CONFIG_SYS_FSL_DRAM_BASE1, CONFIG_SYS_FSL_DRAM_BASE1,
	  CONFIG_SYS_FSL_DRAM_SIZE1, MT_NORMAL_NC, PTE_BLOCK_OUTER_SHARE },
#endif
#ifdef CONFIG_S32_GEN1
	{ CONFIG_SYS_FSL_IRAM_BASE, CONFIG_SYS_FSL_IRAM_BASE,
	  CONFIG_SYS_FSL_IRAM_SIZE, MT_NORMAL, PTE_BLOCK_OUTER_SHARE },
	{ CONFIG_SYS_FSL_PERIPH_BASE, CONFIG_SYS_FSL_PERIPH_BASE,
	  CONFIG_SYS_FSL_PERIPH_SIZE, MT_DEVICE_NGNRNE,
			PTE_BLOCK_NON_SHARE | PTE_BLOCK_PXN | PTE_BLOCK_UXN },
#else	/* S32V234 */
	{ CONFIG_SYS_FSL_IRAM_BASE, CONFIG_SYS_FSL_IRAM_BASE,
	  CONFIG_SYS_FSL_IRAM_SIZE, MT_NORMAL_NC, PTE_BLOCK_OUTER_SHARE },
	{ CONFIG_SYS_FSL_PERIPH_BASE, CONFIG_SYS_FSL_PERIPH_BASE,
	  CONFIG_SYS_FSL_PERIPH_SIZE, MT_DEVICE_NGNRNE, PTE_BLOCK_NON_SHARE },
	{ CONFIG_SYS_FSL_DRAM_BASE2, CONFIG_SYS_FSL_DRAM_BASE2,
	  CONFIG_SYS_FSL_DRAM_SIZE2, MT_NORMAL_NC, PTE_BLOCK_OUTER_SHARE },
#endif /* CONFIG_S32_GEN1 */
	{ CONFIG_SYS_FSL_FLASH0_BASE, CONFIG_SYS_FSL_FLASH0_BASE,
	  CONFIG_SYS_FSL_FLASH0_SIZE, MT_NORMAL_NC, PTE_BLOCK_OUTER_SHARE } ,
#ifdef CONFIG_SYS_FSL_FLASH1_BASE
	{ CONFIG_SYS_FSL_FLASH1_BASE, CONFIG_SYS_FSL_FLASH1_BASE,
	  CONFIG_SYS_FSL_FLASH1_SIZE, MT_NORMAL_NC, PTE_BLOCK_OUTER_SHARE },
#endif
};

static const struct sys_mmu_table s32_final_mmu_table[] = {
#ifdef CONFIG_TARGET_TYPE_S32GEN1_EMULATOR
	{ CONFIG_SYS_FSL_DRAM_BASE1, CONFIG_SYS_FSL_DRAM_BASE1,
	  CONFIG_SYS_FSL_DRAM_SIZE1, MT_NORMAL, PTE_BLOCK_OUTER_SHARE },
#else
	{ CONFIG_SYS_FSL_DRAM_BASE1, CONFIG_SYS_FSL_DRAM_BASE1,
	  CONFIG_SYS_FSL_DRAM_SIZE1, MT_NORMAL, PTE_BLOCK_OUTER_SHARE },
#endif
#ifdef CONFIG_S32_GEN1
	{ CONFIG_SYS_FSL_IRAM_BASE, CONFIG_SYS_FSL_IRAM_BASE,
	  CONFIG_SYS_FSL_IRAM_SIZE, MT_NORMAL, PTE_BLOCK_OUTER_SHARE },
	{ CONFIG_SYS_FSL_PERIPH_BASE, CONFIG_SYS_FSL_PERIPH_BASE,
	  CONFIG_SYS_FSL_PERIPH_SIZE, MT_DEVICE_NGNRNE,
			PTE_BLOCK_NON_SHARE | PTE_BLOCK_PXN | PTE_BLOCK_UXN },
#else
	{ CONFIG_SYS_FSL_IRAM_BASE, CONFIG_SYS_FSL_IRAM_BASE,
	  CONFIG_SYS_FSL_IRAM_SIZE, MT_NORMAL, PTE_BLOCK_OUTER_SHARE },
	{ CONFIG_SYS_FSL_PERIPH_BASE, CONFIG_SYS_FSL_PERIPH_BASE,
	  CONFIG_SYS_FSL_PERIPH_SIZE, MT_DEVICE_NGNRNE, PTE_BLOCK_NON_SHARE },
	{ CONFIG_SYS_FSL_DRAM_BASE2, CONFIG_SYS_FSL_DRAM_BASE2,
	  CONFIG_SYS_FSL_DRAM_SIZE2, MT_NORMAL, PTE_BLOCK_OUTER_SHARE },
#endif /* CONFIG_S32_GEN1 */
	{ CONFIG_SYS_FSL_FLASH0_BASE, CONFIG_SYS_FSL_FLASH0_BASE,
	  CONFIG_SYS_FSL_FLASH0_SIZE, MT_NORMAL, PTE_BLOCK_OUTER_SHARE },
#ifdef CONFIG_SYS_FSL_FLASH1_BASE
	{ CONFIG_SYS_FSL_FLASH1_BASE, CONFIG_SYS_FSL_FLASH1_BASE,
	  CONFIG_SYS_FSL_FLASH1_SIZE, MT_NORMAL, PTE_BLOCK_OUTER_SHARE },
#endif
#if defined(CONFIG_PCIE_S32GEN1)
	/* TODO: for CONFIG_DM_PCI, we should get address/size from
	 * device tree
	 */
	{ CONFIG_SYS_PCIE0_PHYS_ADDR_HI, CONFIG_SYS_PCIE0_PHYS_ADDR_HI,
	  CONFIG_SYS_PCIE0_PHYS_SIZE_HI, MT_DEVICE_NGNRNE,
	  PTE_BLOCK_NON_SHARE | PTE_BLOCK_PXN | PTE_BLOCK_UXN },
	{ CONFIG_SYS_PCIE1_PHYS_ADDR_HI, CONFIG_SYS_PCIE1_PHYS_ADDR_HI,
	  CONFIG_SYS_PCIE1_PHYS_SIZE_HI, MT_DEVICE_NGNRNE,
	  PTE_BLOCK_NON_SHARE | PTE_BLOCK_PXN | PTE_BLOCK_UXN },
#endif
};

#endif

u32 cpu_mask(void);
int cpu_numcores(void);

#endif /* _FSL_S32V234_CPU_H */