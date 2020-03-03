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

void board_init_f(ulong dummy)
{
    /* Clear global data */
    uart_open(CONSOLE_UART_BASE);
    mini_printf("\nWellcome to PPL!\n");
}

void board_init_r(gd_t *gd, ulong dummy)
{
    int	i;
    s8 om_judge;
    /* Because of the relocation of uboot, the address of uboot in DDR will change.
    So we prepare the uboot at the address which is calculated by uboot itself.
    Different DDR address and size will create different uboot address. */
    phys_addr_t spl_baseaddr = CONFIG_SPL_TEXT_BASE;
    void (*image_entry)(void);
    u32	retlen;
    u32	ret;

    mini_printf("The U-Boot-ppl start.\n");
    mini_printf("U-Boot version is 2020.07, internal version is %s\n", UBOOT_INTERNAL_VERSION);

    om_judge = get_boot_select();
    switch (om_judge) {
    case 0x1:
        /* The mode of spi flash */
        mini_printf("This is spiflash mode.\n");
        for (i = 0; i < (FLASH_SPL_SIZE + 255) / 256; i++) {
            spiflash_read(0, FLASH_SPL_READ_ADDR + (i * 256), (u8 *)(spl_baseaddr + (i * 256)), 256, &retlen);
        }
        image_entry = (void (*)(void))(spl_baseaddr);
        image_entry();
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
        for (i = 0; i < (FLASH_SPL_SIZE + 511) / 512; i++) {
            emmc_emmc_read(0, (FLASH_SPL_READ_ADDR + (i * 512)) / 0x200, 512, (u8*)(spl_baseaddr + (i * 512)));
        }
        image_entry = (void (*)(void))(spl_baseaddr);
        image_entry();
        break;
    default:
        mini_printf("OM mode is %x, please check the OM.\n", om_judge);
        break;
    }
    while (1);
}

