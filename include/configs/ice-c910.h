/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <linux/sizes.h>


// #define DEBUG

/*
 * LSP : Timer UART
 * HSP : SPI0 EMMC
 */
#define CPU_DEFAULT_FREQ  50000000
#define LSP_DEFAULT_FREQ  50000000
#define HSP_DEFAULT_FREQ  25000000 // 50000000

#define DWMMC_MAX_FREQ  24000000


#define UBOOT_INTERNAL_VERSION "0.1"
#define CONFIG_BOARD_PRINTF_SUPPORT
#define CONFIG_BOARD_CONSOLE_SUPPORT
#define CONFIG_BOARD_MMC_SUPPORT
#define CONFIG_BOARD_SPIFLASH_SUPPORT


/*-----------------------------------------------------------------------
 * Physical Memory Map
 */

// PPL base: 0x3_fe43_0000, size: 0xf000(60KB)
// #define CONFIG_PPL_BSS_START_ADDR        0x3fe43c000 // base+48KB
// #define CONFIG_PPL_BSS_MAX_SIZE               0x3000 // 12KB
// #define CONFIG_PPL_STACK  (CONFIG_PPL_TEXT_BASE + 0xc000 - 0x10)

// SPL base: 0x3_fe40_0000, size: 0x30000(192KB)
#define CONFIG_SPL_BSS_START_ADDR       0x3fe428000  // base+160KB
#define CONFIG_SPL_BSS_MAX_SIZE              0x8000  // 32KB
#define CONFIG_SYS_SPL_MALLOC_START     0x3fe430000
#define CONFIG_SYS_SPL_MALLOC_SIZE       0x0000f000
#define CONFIG_SPL_STACK  (CONFIG_SPL_TEXT_BASE + 0x28000 - 0x10)
#define CONFIG_SPL_LOAD_FIT_ADDRESS      0x40000000


#define CONFIG_SYS_INIT_SP_ADDR     (CONFIG_SYS_TEXT_BASE + SZ_1M)
#define CONFIG_SYS_LOAD_ADDR        (CONFIG_SYS_TEXT_BASE + SZ_1M)
#define CONFIG_SYS_MALLOC_LEN       SZ_8M
#define CONFIG_SYS_BOOTM_LEN        SZ_64M


/* Image address in Flash */
#define FLASH_PPL_READ_ADDR     0x0
#define FLASH_PPL_SIZE          0x03000
#define FLASH_SPL_READ_ADDR     0x03000 /* 12K */
#define FLASH_SPL_SIZE          0x3c000
#define FLASH_FDT_READ_ADDR     0x3f000 /* 252K */
#define FLASH_FDT_SIZE          0x01000
#define FLASH_OPENSBI_READ_ADDR 0x40000 /* 256K */
#define FLASH_OPENSBI_SIZE      0x50000
#define FLASH_UBOOT_READ_ADDR   0x90000 /* 576K */
#define FLASH_UBOOT_SIZE        0x70000


/* Environment options */

#define BOOT_TARGET_DEVICES(func) \
    func(MMC, mmc, 0) \
    func(DHCP, dhcp, na)

#include <config_distro_bootcmd.h>

#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND "bootm $kernel_addr_r $ramdisk_addr_r $fdt_addr_r"

#define CONFIG_EXTRA_ENV_SETTINGS \
    "fdt_high=0xffffffffffffffff\0" \
    "initrd_high=0xffffffffffffffff\0" \
    "kernel_addr_r=0xc0200000\0" \
    "ramdisk_addr_r=0xc1000000\0" \
    "fdt_addr_r=0xc0f00000\0" \
    BOOTENV

#endif /* __CONFIG_H */
