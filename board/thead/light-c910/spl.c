/*
* Copyright (C) 2017-2020 Alibaba Group Holding Limited
*
* SPDX-License-Identifier: GPL-2.0+
*/

#include <linux/types.h>
#include <common.h>
#include <console.h>
#include <cpu_func.h>
#include <asm/csr.h>
#include <asm/io.h>
#include <asm/barrier.h>
#include <spl.h>
#include <asm/spl.h>
#include <asm/arch-thead/boot_mode.h>
#include <string.h>
#include "../common/uart.h"
#include "../common/mini_printf.h"

extern void init_ddr(void);
extern void cpu_clk_config(int cpu_freq);
extern void ddr_clk_config(int ddr_freq);
extern void show_sys_clk(void);

struct light_reset_list {
        u32 val;
        u64 reg;
};

static struct light_reset_list light_pre_reset_lists[] = {
	{0x00000037, 0xFFFFF4403C}, /* Aon: Audio sys rst */
};

static struct light_reset_list light_post_reset_lists[] = {
	{0x00000001, 0xFFFF0151B0}, /* AP rst_gen: NPU rst */
	{0xFFFFFFFF, 0xFFFF041028}, /* DSP sys_reg: DSP rst */
	{0x00000001, 0xFFFF529000}, /* VO sys_reg: GPU rst */
	{0x00000003, 0xFFEF528000}, /* VO sys_reg: GPU rst */
	{0x00000007, 0xFFFF529004}, /* VO sys_reg: DPU rst */
};

static void light_pre_reset_config(void)
{
	/* Reset VI/VO/VP/DSP/NPU/GPU/DPU */
	int i = 0;
	int entry_size;

	entry_size = ARRAY_SIZE(light_pre_reset_lists);

	while (i < entry_size) {
		writel(light_pre_reset_lists[i].val, (void *)(light_pre_reset_lists[i].reg));
		i++;
	}
}

static void light_post_reset_config(void)
{
	/* Reset VI/VO/VP/DSP/NPU/GPU/DPU */
	int i = 0;
	int entry_size;

	entry_size = ARRAY_SIZE(light_post_reset_lists);

	while (i < entry_size) {
		writel(light_post_reset_lists[i].val, (void *)(light_post_reset_lists[i].reg));
		i++;
	}
}

void setup_ddr_pmp(void)
{
	/* clear pmp entry0,entry1 setting in bootrom */
	writel(0x0 >> 12, (void *)(PMP_BASE_ADDR + 0x104));
	writel(0x0 >> 12, (void *)(PMP_BASE_ADDR + 0x100));
	writel(0x0 >> 12, (void *)(PMP_BASE_ADDR + 0x10c));
	writel(0x0 >> 12, (void *)(PMP_BASE_ADDR + 0x108));

	writel(0, (void *)(PMP_BASE_ADDR + 0x000));

	sync_is();
}

void cpu_performance_enable(void)
{
#define CSR_MHINT2_E	0x7cc
	csr_write(CSR_MHINT2_E, csr_read(CSR_MHINT2_E) | 0x8000);
	csr_write(CSR_MCCR2, 0xe24a0009);
	csr_write(CSR_MHCR, 0x11ff);
	csr_write(CSR_MXSTATUS, 0x638000);
	csr_write(CSR_MHINT, 0x6e30c);
}

static int bl1_img_have_head(unsigned long img_src_addr)
{
    uint8_t *buffer = (uint8_t *)img_src_addr;

    if (memcmp(header_magic, &buffer[4], 4) == 0) {
        return 1;
    }

    return 0;
}

static void light_board_init_r(gd_t *gd, ulong dummy)
{
	void (*entry)(long, long);
	void *sram_uboot_start;

	cpu_performance_enable();

	sram_uboot_start = (void *)(CONFIG_SPL_TEXT_BASE + CONFIG_SPL_MAX_SIZE);
	if (bl1_img_have_head((unsigned long)SRAM_BASE_ADDR) == 1) {
		uint32_t sign_en, encrypt_en;
		img_header_t *phead;

		phead = (img_header_t *)SRAM_BASE_ADDR;
		sign_en = phead->option_flag & 0x1;
		encrypt_en = phead->option_flag & 0x2;
		printf("image has header, sign %s, encrypt %s\n",
			sign_en ? "en" : "disabled",
			encrypt_en ? "en" : "disabled");
	} else {
		printf("image has no header\n");
	}
	memcpy((void *)CONFIG_SYS_TEXT_BASE, sram_uboot_start, CONFIG_SYS_MONITOR_LEN);
	entry = (void (*)(long, long))CONFIG_SYS_TEXT_BASE;
	invalidate_icache_all();
	flush_dcache_range(CONFIG_SYS_TEXT_BASE, CONFIG_SYS_TEXT_BASE + CONFIG_SYS_MONITOR_LEN);
	entry(0, 0);

	while (1);
}

void board_init_f(ulong dummy)
{
	int ret;

	light_pre_reset_config();
	cpu_clk_config(0);
	light_post_reset_config();

	ret = spl_early_init();
	if (ret) {
		printf("spl_early_init() failed: %d\n", ret);
		hang();
	}
	arch_cpu_init_dm();
	preloader_console_init();

	ddr_clk_config(0);

	init_ddr();
	mdelay(1000);
	setup_ddr_pmp();

	printf("ddr initialized, jump to uboot\n");
	light_board_init_r(NULL, 0);
}

void board_boot_order(u32 *spl_boot_list)
{
#define SOC_OM_ADDRBASE        0xffef018010
	switch (readl((void *)SOC_OM_ADDRBASE) & 0x7) {
	case 0:
	case 1:
	case 2:
	case 3:
		/* usb boot */
		break;
	case 4:
		/* emmc boot */
		spl_boot_list[0] = BOOT_DEVICE_MMC1;
		break;
	case 5:
		/* sd boot */
		spl_boot_list[0] = BOOT_DEVICE_MMC1;
		break;
	case 6:
		/* qspi-nand boot */
		spl_boot_list[0] = BOOT_DEVICE_NAND;
		break;
	case 7:
		/* spi-nor boot */
		spl_boot_list[0] = BOOT_DEVICE_SPI;
		break;
	default:
		spl_boot_list[0] = BOOT_DEVICE_NONE;
	}

	cpu_performance_enable();
}
