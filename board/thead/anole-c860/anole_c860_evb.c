/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include "../common/mini_printf.h"
#include "spl/ddr.h"
#include "hardware.h"


#define SLAVE_ENTRY_BASEADDR    (u32)(0xbfff0000)
#define SLAVE_RESET_CONTROL     (u32)(0xbff48068)
#define SLAVE_ENABLE_CONTROL    (u32)(0xbe830068)


DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_SPL_BUILD
void sdram_init(void)
{
    init_ddr();
}
#endif

int dram_init(void)
{
    gd->ram_size = PHYS_SDRAM_1_SIZE;
    return 0;
}

void board_lmb_reserve(struct lmb *lmb)
{
    lmb_add(lmb, 0x80000000, 0x20000000);
}

void show_boot_progress(int val) {}

int board_early_init_f(void)
{
    return 0;
}

int dram_init_banksize(void)
{
    gd->bd->bi_dram[0].start = PHYS_SDRAM_1 + 0x14000000;
    gd->bd->bi_dram[0].size = 0x4000000;

    return 0;
}

int board_early_init_r(void)
{
    timer_init();
    return 0;
}

int disable_slave_cpu(void)
{
    writel(0, SLAVE_RESET_CONTROL);
    return 0;
}

int set_slave_cpu_entry(phys_addr_t entry)
{
    // set slave jump addr
    writel(entry, SLAVE_ENTRY_BASEADDR);
    flush_cache(SLAVE_ENTRY_BASEADDR, 0x100);
    return 0;
}

int enable_slave_cpu(void)
{
    printf("reset slave cpu\n");
    writel(2, SLAVE_ENABLE_CONTROL);
    writel(1, SLAVE_RESET_CONTROL);
}
