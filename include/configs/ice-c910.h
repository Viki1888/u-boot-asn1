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


/*-----------------------------------------------------------------------
 * Physical Memory Map
 */

// PPL base: 0x3_fe43_0000, size: 0xf000(60KB)
#define CONFIG_PPL_BSS_START_ADDR        0x3fe43c000 // base+48KB
#define CONFIG_PPL_BSS_MAX_SIZE               0x3000 // 12KB
#define CONFIG_PPL_STACK  (CONFIG_PPL_TEXT_BASE + 0xc000 - 0x10)

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


#define UBOOT_INTERNAL_VERSION "0.1"
#define CONFIG_BOARD_PRINTF_SUPPORT
#define CONFIG_BOARD_CONSOLE_SUPPORT
#define CONFIG_BOARD_MMC_SUPPORT
#define CONFIG_BOARD_SPIFLASH_SUPPORT


/* Network Configuration */
#define CONFIG_DW_ALTDESCRIPTOR
#define CONFIG_MII		1
#define CONFIG_PHY_MARVELL	1
#define CONFIG_NET_RETRY_COUNT	20


/* Image address in Flash */
#define FLASH_PPL_READ_ADDR     0x0
#define FLASH_PPL_SIZE          0x03000
#define FLASH_SPL_READ_ADDR     0x03000 /* 12K */
#define FLASH_SPL_SIZE          0x39000
#define FLASH_FDT_READ_ADDR     0x3c000 /* 240K */
#define FLASH_FDT_SIZE          0x04000
#define FLASH_OPENSBI_READ_ADDR 0x40000 /* 256K */
#define FLASH_OPENSBI_SIZE      0x50000
#define FLASH_UBOOT_READ_ADDR   0x90000 /* 576K */
#define FLASH_UBOOT_SIZE        0x70000


/* Environment options */

#ifdef CONFIG_IS_ASIC
#define TFTP_LOAD_DTB "tftpboot ${dtb_load_addr_virt} ice-c910-evb.dtb ; "
#else
#define TFTP_LOAD_DTB "tftpboot ${dtb_load_addr_virt} ice-c910.dtb ; "
#endif

#define CONFIG_EXTRA_ENV_SETTINGS \
    "uboot_start_sector=0x480\0"   /* uboot start sector = FLASH_UBOOT_READ_ADDR / 0x200 */ \
    "dtb_start_sector=0x1000\0"    /* dtb start sector */ \
    "dtb_size_sectors=0x1000\0"    /* dtb size in sectors -> 2MB */ \
    "linux_start_sector=0x2000\0"  /* linux start sector */  \
    "linux_size_sectors=0xa000\0"  /* linux size in sectors -> 20MB */ \
    "ramdisk_start_sector=c000\0"  /* ramdisk start sector */ \
    "ramdisk_size_sectors=10000\0" /* ramdisk size in sectors -> 32MB */ \
    "dtb_load_addr_virt=0x8f000000\0" \
    "linux_load_addr_virt=0x90000000\0" \
    "ramdisk_load_addr_virt=0x91000000\0" \
    "update_uboot=" \
        "tftpboot ${dtb_load_addr_virt} u-boot.bin ; " \
        "setexpr fw_sz ${filesize} / 0x200 ; " \
        "setexpr fw_sz ${fw_sz} + 1 ; " \
        "mmc dev 0 1 ; "  /* uboot -> eMMC BOOT PARTITION #1 */ \
        "mmc write ${dtb_load_addr_virt} ${uboot_start_sector} ${fw_sz} ; " \
        "mmc dev 0 0 ; "  /* restore to USER PARTITION */ \
        "\0" \
    "update_dtb=" \
        TFTP_LOAD_DTB \
        "setexpr fw_sz ${filesize} / 0x200 ; " \
        "setexpr fw_sz ${fw_sz} + 1 ; " \
        "mmc write ${dtb_load_addr_virt} ${dtb_start_sector} ${fw_sz} ; " \
        "setenv dtb_size_sectors ${fw_sz} ; " \
        "saveenv ; " \
        "\0" \
    "update_ramdisk=" \
        "tftpboot ${ramdisk_load_addr_virt} rootfs.cpio.gz ; " \
        "setexpr fw_sz ${filesize} / 0x200 ; " \
        "setexpr fw_sz ${fw_sz} + 1 ; " \
        "mmc write ${ramdisk_load_addr_virt} ${ramdisk_start_sector} ${fw_sz} ; " \
        "setenv ramdisk_size_sectors ${fw_sz} ; " \
        "saveenv ; " \
        "\0" \
    "update_linux=" \
        "tftpboot ${linux_load_addr_virt} uImage ; " \
        "setexpr fw_sz ${filesize} / 0x200 ; " \
        "setexpr fw_sz ${fw_sz} + 1 ; " \
        "mmc write ${linux_load_addr_virt} ${linux_start_sector} ${fw_sz} ; " \
        "setenv linux_size_sectors ${fw_sz} ; " \
        "saveenv ; " \
        "\0"

#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND \
        "mmc read ${dtb_load_addr_virt} ${dtb_start_sector} ${dtb_size_sectors} ; " \
        "mmc read ${ramdisk_load_addr_virt} ${ramdisk_start_sector} ${ramdisk_size_sectors} ; " \
        "mmc read ${linux_load_addr_virt} ${linux_start_sector} ${linux_size_sectors} ; " \
        "bootm ${linux_load_addr_virt} ${ramdisk_load_addr_virt} ${dtb_load_addr_virt}"

#endif /* __CONFIG_H */
