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
#include <string.h>
#include "../common/uart.h"
#include "../common/mini_printf.h"

extern void init_ddr(void);
extern void cpu_clk_config(int cpu_freq);
extern void ddr_clk_config(int ddr_freq);
extern void show_sys_clk(void);

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

void board_init_f(ulong dummy)
{
	int ret;

	cpu_clk_config(0);

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

#if 0
void board_init_r(gd_t *gd, ulong dummy)
{
	void (*entry)(long, long);

#define UBOOT_SIZE (512 * 1024)
	memcpy((void *)CONFIG_SYS_TEXT_BASE, (void *)(0xffe0000000 + CONFIG_SPL_MAX_SIZE), UBOOT_SIZE);
	entry = (void (*)(long, long))CONFIG_SYS_TEXT_BASE;
	invalidate_icache_all();
	flush_dcache_range(CONFIG_SYS_TEXT_BASE, CONFIG_SYS_TEXT_BASE + UBOOT_SIZE);
	entry(0, 0);

	asm volatile("ebreak\n");
	while (1);
}
#endif
