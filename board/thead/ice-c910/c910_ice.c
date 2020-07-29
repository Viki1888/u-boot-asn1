/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <asm/asm.h>
#include <asm/io.h>
#include <asm/types.h>
#include <lmb.h>
#include <cpu_func.h>
#include <stdio.h>
#include "hardware.h"

#define CSR_MCOR         0x7c2
#define CSR_MHCR         0x7c1
#define CSR_MCCR2        0x7c3
#define CSR_MHINT        0x7c5
#define CSR_MXSTATUS     0x7c0
#define CSR_PLIC_BASE    0xfc1
#define CSR_MRMR         0x7c6
#define CSR_MRVBR        0x7c7


#define csr_write(csr, val)                                        \
    ({                                                         \
        unsigned long __v = (unsigned long)(val);          \
        __asm__ __volatile__("csrw " __ASM_STR(csr) ", %0" \
                    :                             \
                    : "rK"(__v)                   \
                    : "memory");                  \
    })



int vm_init(void)
{
    // # ddr:         0x0 00000000 ~ 0x1 00000000
    csr_write(pmpaddr3, 0x0 >> 2 | ((0x100000000 - 1) >> 3));

    csr_write(pmpcfg0, 0x889800001f1b1f1d);

    return 0;
}

int io_init(void)
{
    // #################################
    // # Release DMAC reset
    // #################################
    // set *0x3fff78050 |= (1 << 0)
    u32* dmac_reg = (u32*)0x3fff78050;
    *dmac_reg |= (1 << 0);

    return 0;
}

#ifdef CONFIG_SPL_BUILD
int spl_enable_cache(void)
{
    // Invalidate & Clear IBP BTB BHT ICache & DCache
    // set $mcor = 0x70013
    csr_write(CSR_MCOR, 0x70013);

    // Enable L2 Cache
    // set $mccr2 = 0xe0010009
    csr_write(CSR_MCCR2, 0xe0410009);

    // Enable L1 Cache
    // set $mhcr = 0x11ff
    csr_write(CSR_MHCR, 0x11ff);

    // Enable CPU Features
    // set $mxstatus = 0x638000
    // set $mhint = 0x16e30c
    csr_write(CSR_MXSTATUS, 0x638000);
    csr_write(CSR_MHINT, 0x16e30c);

	return 0;
}
#endif

void board_lmb_reserve(struct lmb *lmb)
{
    lmb_add(lmb, (u64)(0x80000000), (u64)(0x20000000));
}

extern int clock_init(void);

int board_init(void)
{
	clock_init();
	return 0;
}

int disable_slave_cpu(void)
{
    writel(0, (void *)SLAVE_RESET_CONTROL);

    return 0;
}

int set_slave_cpu_entry(phys_addr_t entry)
{
    // set slave jump addr
    writel(entry, (void *)SYSREG_BASEADDR);
    flush_cache(SYSREG_BASEADDR, 0x1000);

    return 0;
}

int enable_slave_cpu(void)
{
    printf("reset slave cpu\n");
    writel(1, (void *)SLAVE_RESET_CONTROL);

	return 0;
}
