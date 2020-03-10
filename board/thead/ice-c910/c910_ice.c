/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <asm/asm.h>
#include <asm/types.h>


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
    // # Physical Memory Protection(PMP) configurations
    // # refer to <C960MP用户手册.docx>, chapter:"物理内存保护"
    // # ddr:         0x0 00000000 ~ 0x2 00000000, TOR rwx        (0x0f)
    // set $pmpaddr0 = 0x200000000 >> 2
    csr_write(pmpaddr0, 0x200000000 >> 2);

    // # peripherals: 0x3 f0000000 ~ 0x4 00000000, NAPOT rw       (0x1b)
    // set $pmpaddr1 = 0x3f0000000 >> 2 | ((0x10000000 - 1) >> 3)
    csr_write(pmpaddr1, 0x3f0000000 >> 2 | ((0x10000000 - 1) >> 3));

    // # 0x00000000 ~ 0x10000000 NAPOT no access                  (0x98)
    // set $pmpaddr6 = 0x00000000 >> 2 | ((0x10000000 - 1) >> 3)
    csr_write(pmpaddr6, 0x00000000 >> 2 | ((0x10000000 - 1) >> 3));

    // # default:     0x0 10000000 ~ 0x100 00000000 TOR no access (0x88)
    // set $pmpaddr7 = 0xffffffffff >> 2
    csr_write(pmpaddr7, 0xffffffffff >> 2);

    // # Be care! we must put background deny entries in the end of
    // # pmpaddrx with lowest priority and set lock bit for m state deny.
    // # Access needn't lock bit for the m state.
    // set $pmpcfg0 = 0x8898000000001b0f
    csr_write(pmpcfg0, 0x8898000000001b0f);

    return 0;
}

int sdram_init(void) {
    // release ddr
    // set *0x3fff78040 = 0xffff
    u32* ddr_reg = (u32*)0x3fff78040;
    *ddr_reg = 0xffff;
    // udelay(100000);
    // ddr_init();

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
    csr_write(mcor, 0x70013);

    // Enable L2 Cache
    // set $mccr2 = 0xe0010009
    csr_write(mccr2, 0xe0010009);

    // Enable L1 Cache
    // set $mhcr = 0x11ff
    csr_write(mhcr, 0x11ff);

    // Enable CPU Features
    // set $mxstatus = 0x638000
    // set $mhint = 0x16e30c
    csr_write(mxstatus, 0x638000);
    csr_write(mhint, 0x16e30c);
}
#endif

void board_lmb_reserve(struct lmb *lmb)
{
    lmb_add(lmb, (u64)(0x80000000), (u64)(0x20000000));
}

int board_init(void)
{
	/* For now do nothing */

	return 0;
}
