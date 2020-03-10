/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <common.h>
#include <mmc.h>
#include <miiphy.h>
#include <netdev.h>
#include "gpio/gpio.h"
#include "../common/mini_printf.h"
#include "spl/ddr.h"
#include "hardware.h"

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_MMC_DW
extern int eragon_dwmci_add_port(int index, u32 regbase, int bus_width);
#endif

#ifdef CONFIG_SPL_BUILD
void sdram_init(void)
{
    init_ddr();
}
#endif

#if defined(CONFIG_MMC) && !defined(CONFIG_DM_MMC)
static int init_dwmmc(void)
{
    int ret = 1;
#ifdef CONFIG_MMC_DW
    ret &= eragon_dwmci_add_port(0, ERAGON_MMC0_BASE, 8);
    if (ret)
#ifdef CONFIG_SPL_BUILD
        mini_printf("Error adding eMMC port (%d)\n", ret);
#else
        printf("Error adding eMMC port (%d)\n", ret);
#endif
#endif
    return ret;
}

int board_mmc_init(bd_t *bis)
{
    int ret;
    /* add the eMMC and sd ports */
    ret = init_dwmmc();
    if (ret)
#ifdef CONFIG_SPL_BUILD
        mini_printf("init_dwmmc failed\n");
#else
        printf("init_dwmmc failed\n");
#endif
    return ret;
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
#if 0
    /* Use the UART 2 */
    gpio_set_reuse(GPIOB, 0x3, GPIO_BEHARDWARE);
    gpio_set_reuse(GPIOB, 0x30, GPIO_BEHARDWARE);
#endif
    return 0;
}

int dram_init_banksize(void)
{
    gd->bd->bi_dram[0].start = PHYS_SDRAM_1 + 0x14000000;
    gd->bd->bi_dram[0].size = 0x4000000;

    return 0;
}

#ifndef CONFIG_SPL_BUILD
#if defined(CONFIG_CMD_NET) && !defined(CONFIG_DM_ETH)
int board_eth_init(bd_t *bis)
{
    int ret = 0;

#if defined(CONFIG_ETH_DESIGNWARE)
    ret = designware_initialize(GMAC_BASEADDR, PHY_INTERFACE_MODE_MII);
#endif
    return ret;
}
#endif
#endif

int board_early_init_r(void)
{
    timer_init();
    return 0;
}

