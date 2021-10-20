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
#include <asm/arch-thead/light-iopmp.h>

#define SYSCLK_USB_CTRL		0xFFFC02D104
#define REF_SSP_EN		0xFFEC03F034
#define USB3_DRD_SWRST		0xFFEC02C014
#define SOC_OM_ADDRBASE		0xFFEF018010
#define QSPI1_CS0_PINMUX	0xFFE7f3c400
#define QSPI0_CS0_PINMUX	0xFFEC007400
#define SPI_CS0_PINMUX		0xFFEC007404

#define LIGHT_IOPMP_DEFAULT_ATTR	0xffffffff
#define LIGHT_IOPMP_DEFAULT_OFF		0xc0

static struct light_reset_list {
        u32 val;
        u64 reg;
} light_reset_lists[] = {
	{0x0000000F, 0xFFFF015220}, /* VP/VO/VI/DSP */
	{0x00000001, 0xFFFF0151B0}, /* NPU */
	{0xFFFFFFFF, 0xFFFF041028}, /* DSP */
	{0x00000001, 0xFFFF529000}, /* GPU */
	{0x00000007, 0xFFFF529004}, /* DPU */
};

static struct light_iopmp_list {
        int iopmp_type;
        u64 reg;
} light_iopmp_lists[] = {
        {IOPMP_EMMC, 0xFFFC028000},
        {IOPMP_SDIO0, 0xFFFC029000},
        {IOPMP_SDIO1, 0xFFFC02A000},
        {IOPMP_USB0, 0xFFFC02E000},
        {IOPMP_AO, 0xFFFFC21000},
        {IOPMP_AUD, 0xFFFFC22000},
        {IOPMP_CHIP_DBG, 0xFFFFC37000},
        {IOPMP_EIP120I, 0xFFFF220000},
        {IOPMP_EIP120II, 0xFFFF230000},
        {IOPMP_EIP120III, 0xFFFF240000},
        {IOPMP_ISP0, 0xFFF4080000},
        {IOPMP_ISP1, 0xFFF4081000},
        {IOPMP_DSP, 0xFFF4082000},
        {IOPMP_DW200, 0xFFF4083000},
        {IOPMP_VENC, 0xFFFCC60000},
        {IOPMP_VDEC, 0xFFFCC61000},
        {IOPMP_G2D, 0xFFFCC62000},
        {IOPMP_FCE, 0xFFFCC63000},
        {IOPMP_NPU, 0xFFFF01C000},
        {IOPMP0_DPU, 0xFFFF520000},
        {IOPMP1_DPU, 0xFFFF521000},
        {IOPMP_GPU, 0xFFFF522000},
        {IOPMP_GMAC1, 0xFFFC001000},
        {IOPMP_GMAC2, 0xFFFC002000},
        {IOPMP_DMAC, 0xFFFFC20000},
        {IOPMP_TEE_DMAC, 0xFFFF250000},
};

static void light_iopmp_config(void)
{
	int i = 0;
	int entry_size;

	entry_size = ARRAY_SIZE(light_iopmp_lists);

	while (i < entry_size) {
		writel(LIGHT_IOPMP_DEFAULT_ATTR, (void *)(light_iopmp_lists[i].reg) + LIGHT_IOPMP_DEFAULT_OFF);
		i++;
	}
}

static void light_reset_config(void)
{
	/* Reset VI/VO/VP/DSP/NPU/GPU/DPU */
	int i = 0;
	int entry_size;

	entry_size = ARRAY_SIZE(light_reset_lists);

	while (i < entry_size) {
		writel(light_reset_lists[i].val, (void *)(light_reset_lists[i].reg));
		i++;
	}
}

static void gmac_clk_config(void)
{
	writel(1 << 13, (void *)(0xffe7030004));
	writel(1 << 13, (void *)(0xffe7030008));
	udelay(10000);
}

static void usb_clk_config(void)
{
	writel(readl((void *)SYSCLK_USB_CTRL) | 0xf, (void *)SYSCLK_USB_CTRL);
	writel(readl((void *)REF_SSP_EN) | 0x1, (void *)REF_SSP_EN);

	udelay(10);

	writel(0x0, (void *)USB3_DRD_SWRST);
	udelay(1000);
	writel(0x7, (void *)USB3_DRD_SWRST);
}

static void spi_hw_init(void)
{

	/*spi0 cs0 gpio2-15 */
	writel(readl((void *)SPI_CS0_PINMUX) | (0x3 << 28), (void *)SPI_CS0_PINMUX);
	/*set spi0 driving strength and pull-up */
	writel(readl((void *)(0xffec00701c)) | (0x135 | (0x135 <<16)), (void *)(0xffec00701c));
	writel(readl((void *)(0xffec007020)) | (0x135 | (0x135 <<16)), (void *)(0xffec007020));

	/*qspi0 cs0 gpio2-3 */
	writel(readl((void *)QSPI0_CS0_PINMUX) | (0x3 << 12), (void *)QSPI0_CS0_PINMUX);
	/*set qspi1 driving strength and pull-up */
	writel(readl((void *)(0xffec007004)) | (0x108 | (0x108 <<16)), (void *)(0xffec007004));
	writel(readl((void *)(0xffec007008)) | (0x138 | (0x138 <<16)), (void *)(0xffec007008));
	writel(readl((void *)(0xffec00700c)) | (0x138 | (0x138 <<16)), (void *)(0xffec00700c));

	/*qspi1 cs0 gpio0-1 */
	writel(readl((void *)QSPI1_CS0_PINMUX) | (0x3 << 4), (void *)QSPI1_CS0_PINMUX);
	/*set qspi1 driving strength and pull-up */
	writel(readl((void *)(0xffe7f3c000)) | (0x108 | (0x108 <<16)), (void *)(0xffe7f3c000));
	writel(readl((void *)(0xffe7f3c004)) | (0x138 | (0x138 <<16)), (void *)(0xffe7f3c004));
	writel(readl((void *)(0xffe7f3c008)) | (0x138 | (0x138 <<16)), (void *)(0xffe7f3c008));

}

int board_init(void)
{
	light_reset_config();
	light_iopmp_config();

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
