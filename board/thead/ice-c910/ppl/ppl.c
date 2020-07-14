/*
* Copyright (C) 2017-2020 Alibaba Group Holding Limited
*
* SPDX-License-Identifier: GPL-2.0+
*/

#include <linux/types.h>
#include <common.h>
#include <console.h>
#include <asm/io.h>
#include <asm/spl.h>
#include "../../common/uart.h"
#include "../../common/spiflash/spiflash.h"
#include "../../common/mmc/emmc.h"
#include "../../common/mini_printf.h"
#include "../hardware.h"
#include "../gpio/gpio.h"
#include "../om/om.h"


extern int vm_init(void);
extern int vm_init_early(void);
extern void sys_clk_config(int cpu_freq, int ddr_freq);

int printf(const char *fmt, ...)
{
    return 0;
}

void hang(void)
{
    while (1);
}

void * memset(void * s, int c, size_t count)
{
    unsigned long *sl = (unsigned long *) s;
    char *s8 = (char *)sl;
    while (count--)
        *s8++ = c;

    return s;
}

void * memcpy(void *dest, const void *src, size_t count)
{
    unsigned long *dl = (unsigned long *)dest, *sl = (unsigned long *)src;
    char *d8, *s8;

    if (src == dest)
        return dest;

    /* while all data is aligned (common case), copy a word at a time */
    if ((((ulong)dest | (ulong)src) & (sizeof(*dl) - 1)) == 0) {
        while (count >= sizeof(*dl)) {
            *dl++ = *sl++;
            count -= sizeof(*dl);
        }
    }
    /* copy the reset one byte at a time */
    d8 = (char *)dl;
    s8 = (char *)sl;
    while (count--)
        *d8++ = *s8++;

    return dest;
}

#ifdef CONFIG_IS_ASICF
static void print_some_freq(void)
{
	u32 read = 0;

	mini_printf("\n\n---- Welcome to ICE EVB_BOARD T-HEAD ----\n\n");
        read = *(volatile unsigned int*)(0x3fff77120);
	mini_printf("CPU_CLK = %dMHz\n",read/1000);
        read = *(volatile unsigned int*)(0x3fff77124);
	mini_printf("AXI_CLK = %dMHz\n",read/1000);
        read = *(volatile unsigned int*)(0x3fff77130);
	mini_printf("AHB_CLK = %dMHz\n",read/1000);
        read = *(volatile unsigned int*)(0x3fff77140);
	mini_printf("NPU_CLK = %dMHz\n",read/1000);
        read = *(volatile unsigned int*)(0x3fff7712c);
        read = *(volatile unsigned int*)(0x3fff7712c);
	mini_printf("DDR_CK = %d MT\n",read/1000*4);
	mini_printf("GMAC = RGMII MODE\n");
}
#endif

void board_init_f(ulong dummy)
{
#ifdef CONFIG_IS_ASICF
    int ddr_freq = 1600;
    int cpu_freq = 1200; //Legal value for ICE

    sys_clk_config(cpu_freq, ddr_freq);
#endif
    uart_open(CONSOLE_UART_BASE);
    mini_printf("\nWelcome to PPL!\n");
#ifdef CONFIG_IS_ASICF
    print_some_freq();
#endif
}

#ifdef DEBUG_RAM_IMAGE
static void ram_load_image(u32 offset, u32 size, phys_addr_t baseaddr)
{
    // please use gdb load image to ram
}
#endif

static void spiflash_load_image(u32 offset, u32 size, phys_addr_t baseaddr)
{
    int i, retlen;

    for (i = 0; i < (size + 255) / 256; i++)
        spiflash_read(0, offset + (i * 256), (u8 *)(baseaddr + (i * 256)), 256, (u32 *)&retlen);
}

static void emmc_load_image(u32 offset, u32 size, phys_addr_t baseaddr)
{
    int i;

    for (i = 0; i < (size + 511) / 512; i++)
        emmc_emmc_read(0, (offset + (i * 512)) / 0x200, 512, (u8 *)(baseaddr + (i * 512)));
}

void board_init_r(gd_t *gd, ulong dummy)
{
    u32 ret;
    s8 om_judge;
    void (*load_image)(u32 offset, u32 size, phys_addr_t baseaddr);
    void (*image_entry)(void);
    /* Because of the relocation of uboot, the address of uboot in DDR will change.
    So we prepare the uboot at the address which is calculated by uboot itself.
    Different DDR address and size will create different uboot address. */
    phys_addr_t spl_baseaddr = CONFIG_SPL_TEXT_BASE;

    mini_printf("The U-Boot-ppl start.\n");
    mini_printf("U-Boot version is 2020.03, internal version is %s\n", UBOOT_INTERNAL_VERSION);

    load_image = NULL;
    om_judge = get_boot_select();
    switch (om_judge) {
#ifdef DEBUG_RAM_IMAGE
    case 0x0:
        mini_printf("This is ram mode.\n");
        load_image = ram_load_image;
        break;
#endif
    case 0x1:
        /* The mode of spi flash */
        mini_printf("This is spiflash mode.\n");
        load_image = spiflash_load_image;
        break;
    case 0x2:
        /* The mode of emmc */
        mini_printf("This is eMMC mode.\n");
        ret = emmc_host_init(NULL);
        if (ret != 0) {
            mini_printf("The eMMC is not exist.\n");
            break;
        }
        mini_printf("eMMC init ready.\n");
        load_image = emmc_load_image;
        break;
    default:
        mini_printf("OM mode is %x, please check the OM.\n", om_judge);
        break;
    }

    if (load_image) {
        load_image(FLASH_SPL_READ_ADDR, FLASH_SPL_SIZE, spl_baseaddr);
        image_entry = (void (*)(void))(spl_baseaddr);
        image_entry();
    }

    asm volatile ("ebreak\n");
    while (1);
}
