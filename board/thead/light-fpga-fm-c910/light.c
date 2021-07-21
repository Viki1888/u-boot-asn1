/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <asm/types.h>
#include <thead/clock_config.h>
#include <linux/bitops.h>

void gmac_clk_config(void)
{
	writel(1 << 13, (void *)(0xffe7030004));
	writel(1 << 13, (void *)(0xffe7030008));
	udelay(10000);
}

void usb_clk_config(void)
{
	writel(0x0, (void *)(0xffef01406c));
	udelay(1000);
	writel(0x7, (void *)(0xffef01406c));
}

static void spi_hw_init(void)
{

/*spi0 cs0 gpio2-15 */
#define SPI_CS0_PINMUX 0xffec007404
	writel(readl((void *)SPI_CS0_PINMUX) | (0x3 << 28), (void *)SPI_CS0_PINMUX);
/*set spi0 driving strength and pull-up */
	writel(readl((void *)(0xffec00701c)) | (0x135 | (0x135 <<16)), (void *)(0xffec00701c));
	writel(readl((void *)(0xffec007020)) | (0x135 | (0x135 <<16)), (void *)(0xffec007020));

/*qspi0 cs0 gpio2-3 */
#define QSPI0_CS0_PINMUX 0xffec007400
	writel(readl((void *)QSPI0_CS0_PINMUX) | (0x3 << 12), (void *)QSPI0_CS0_PINMUX);
/*set qspi1 driving strength and pull-up */
	writel(readl((void *)(0xffec007004)) | (0x108 | (0x108 <<16)), (void *)(0xffec007004));
	writel(readl((void *)(0xffec007008)) | (0x138 | (0x138 <<16)), (void *)(0xffec007008));
	writel(readl((void *)(0xffec00700c)) | (0x138 | (0x138 <<16)), (void *)(0xffec00700c));

/*qspi1 cs0 gpio0-1 */
#define QSPI1_CS0_PINMUX 0xffe7f3c400
	writel(readl((void *)QSPI1_CS0_PINMUX) | (0x3 << 4), (void *)QSPI1_CS0_PINMUX);
/*set qspi1 driving strength and pull-up */
	writel(readl((void *)(0xffe7f3c000)) | (0x108 | (0x108 <<16)), (void *)(0xffe7f3c000));
	writel(readl((void *)(0xffe7f3c004)) | (0x138 | (0x138 <<16)), (void *)(0xffe7f3c004));
	writel(readl((void *)(0xffe7f3c008)) | (0x138 | (0x138 <<16)), (void *)(0xffe7f3c008));

}

int board_init(void)
{
#define GMAC_IOPMP   0xfff7038000
	writel(0xffffffff, (void *)(GMAC_IOPMP+0xc0));

#define USB_IOPMP   0xfff703a000
	writel(0xffffffff, (void *)(USB_IOPMP+0xc0));

	usb_clk_config();
	gmac_clk_config();
	spi_hw_init();

	clk_config();

	return 0;
}

#ifdef LIGHT_IMAGE_WRITER
static void light_usb_boot_check(void)
{
	int boot_mode;

#define SOC_OM_ADDRBASE        0xffef018010
	boot_mode = readl((void *)SOC_OM_ADDRBASE) & 0x7;
	if (boot_mode & BIT(2))
		return;

#ifdef CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG
	env_set("usb_fastboot", "yes");
#endif
	run_command("env default -a -f", 0);
	run_command("run gpt_partition", 0);
	run_command("fastboot usb 0", 0);
}
#endif

int board_late_init(void)
{
#ifdef LIGHT_IMAGE_WRITER
	light_usb_boot_check();
#endif
	return 0;
}
