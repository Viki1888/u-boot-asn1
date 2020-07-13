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
#ifdef CONFIG_IS_ASICF
#define CPU_DEFAULT_FREQ      1000000000
#define HSP_DEFAULT_FREQ      250000000
#define PPL_LSP_DEFAULT_FREQ  24000000
#define LSP_DEFAULT_FREQ      62500000
#define DWMMC_MAX_FREQ        25000000
#else
#define CPU_DEFAULT_FREQ  50000000
#define LSP_DEFAULT_FREQ  50000000
#define HSP_DEFAULT_FREQ  25000000
#define DWMMC_MAX_FREQ    24000000
#endif

/*-----------------------------------------------------------------------
 * Physical Memory Map
 */

// PPL base: 0x3_fe40_0000, size: 0x10000(64KB)
#define CONFIG_PPL_BSS_START_ADDR        0x3fe40d000 // base+52KB
#define CONFIG_PPL_BSS_MAX_SIZE               0x3000 // 12KB
#define CONFIG_PPL_STACK  (CONFIG_PPL_BSS_START_ADDR - 0x10)

// SPL base: 0x3_fe41_0000, size: 0x2f000(188KB)
#define CONFIG_SPL_BSS_START_ADDR       0x3fe436000  // base+160KB
#define CONFIG_SPL_BSS_MAX_SIZE              0x6000  // 32KB
#define CONFIG_SYS_SPL_MALLOC_START     0x3fe430000
#define CONFIG_SYS_SPL_MALLOC_SIZE       0x0000f000
#define CONFIG_SPL_STACK  (CONFIG_SPL_BSS_START_ADDR - 0x10)
#define CONFIG_SPL_LOAD_FIT_ADDRESS      0x40000000


#define CONFIG_SYS_INIT_SP_ADDR     (CONFIG_SYS_TEXT_BASE + SZ_1M)
#define CONFIG_SYS_LOAD_ADDR        (CONFIG_SYS_TEXT_BASE + SZ_1M)
#define CONFIG_SYS_MALLOC_LEN       SZ_8M
#define CONFIG_SYS_BOOTM_LEN        SZ_64M
#define CONFIG_SYS_CACHELINE_SIZE   128


#define UBOOT_INTERNAL_VERSION "0.2"
#define CONFIG_BOARD_PRINTF_SUPPORT
#define CONFIG_BOARD_CONSOLE_SUPPORT
#define CONFIG_BOARD_MMC_SUPPORT
#define CONFIG_BOARD_SPIFLASH_SUPPORT
#define CONFIG_SUPPORT_EMMC_BOOT


/* Network Configuration */
#define CONFIG_DW_ALTDESCRIPTOR
#define CONFIG_MII              1
#define CONFIG_PHY_MARVELL      1
#define CONFIG_NET_RETRY_COUNT  20


/* Image address in Flash */
#define FLASH_PPL_READ_ADDR     0x0
#define FLASH_PPL_SIZE          0x03000
#define FLASH_SPL_READ_ADDR     0x03000 /* 12K */
#define FLASH_SPL_SIZE          0x28000
#define FLASH_FDT_READ_ADDR     0x3c000 /* 240K */
#define FLASH_FDT_SIZE          0x04000
#define FLASH_OPENSBI_READ_ADDR 0x40000 /* 256K */
#define FLASH_OPENSBI_SIZE      0x50000
#define FLASH_UBOOT_READ_ADDR   0x90000 /* 576K */
#define FLASH_UBOOT_SIZE        0x70000


/* Environment options */

#ifdef CONFIG_IS_ASIC
#define TFTP_LOAD_DTB "tftpboot ${dtb_load_addr_virt} c910/hw_evb.dtb ; "
#define TFTP_LOAD_SLAVE_DTB "tftpboot ${dtb_load_addr_virt} c810/ice_ck810_evb.dtb ; "
#else
#define TFTP_LOAD_DTB "tftpboot ${dtb_load_addr_virt} c910/hw.dtb ; "
#define TFTP_LOAD_SLAVE_DTB "tftpboot ${dtb_load_addr_virt} c810/ice_ck810.dtb ; "
#endif

#define CONFIG_EXTRA_ENV_SETTINGS \
    "fdt_high=0xffffffffffffffff\0" \
    "initrd_high=0xffffffffffffffff\0" \
    "ppl_start_sector=0\0"      /* uboot ppl start sector = FLASH_PPL_READ_ADDR / 0x200 */ \
    "spl_start_sector=0x18\0"   /* uboot ppl start sector = FLASH_SPL_READ_ADDR / 0x200 */ \
    "uboot_start_sector=0x480\0"   /* uboot start sector = FLASH_UBOOT_READ_ADDR / 0x200 */ \
    "dtb_start_sector=0x1000\0"    /* dtb start sector */ \
    "dtb_size_sectors=0x100\0"     /* dtb size in sectors -> 128KB */ \
    "linux_start_sector=0x2000\0"  /* linux start sector */  \
    "linux_size_sectors=0xa000\0"  /* linux size in sectors -> 20MB */ \
    "ramdisk_start_sector=0xc000\0" /* ramdisk start sector */ \
    "ramdisk_size_sectors=0x10000\0" /* ramdisk size in sectors -> 32MB */ \
    "slave_spl_start_sector=0x1000\0" /* uboot spl slave start sector */ \
    "slave_fdt_start_sector=0x1180\0"   /* slave fdt start sector */ \
    "slave_uboot_start_sector=0x1200\0"  /* slave uboot start sector */ \
    "slave_dtb_start_sector=0x41000\0" \
    "slave_linux_start_sector=0x42000\0" \
    "slave_spl_load_addr_virt=0x20f00000\0" \
    "sram_addr_virt=0x3fe400000\0"   /* PHYS_SRAM_1 */ \
    "dtb_load_addr_virt=0x01f00000\0" \
    "linux_load_addr_virt=0x00200000\0" \
    "ramdisk_load_addr_virt=0x02000000\0" \
    "update_dtb=" \
        TFTP_LOAD_DTB \
        "setexpr fw_sz ${filesize} / 0x200 ; " \
        "setexpr fw_sz ${fw_sz} + 1 ; " \
        "mmc write ${dtb_load_addr_virt} ${dtb_start_sector} ${fw_sz} ; " \
        "setenv dtb_size_sectors ${fw_sz} ; " \
        "saveenv ; " \
        "\0" \
    "update_linux=" \
        "tftpboot ${linux_load_addr_virt} c910/uImage ; " \
        "setexpr fw_sz ${filesize} / 0x200 ; " \
        "setexpr fw_sz ${fw_sz} + 1 ; " \
        "mmc write ${linux_load_addr_virt} ${linux_start_sector} ${fw_sz} ; " \
        "setenv linux_size_sectors ${fw_sz} ; " \
        "saveenv ; " \
        "\0" \
    "update_ramdisk=" \
        "tftpboot ${ramdisk_load_addr_virt} c910/rootfs.img ; " \
        "setexpr fw_sz ${filesize} / 0x200 ; " \
        "setexpr fw_sz ${fw_sz} + 1 ; " \
        "mmc write ${ramdisk_load_addr_virt} ${ramdisk_start_sector} ${fw_sz} ; " \
        "setenv ramdisk_size_sectors ${fw_sz} ; " \
        "saveenv ; " \
        "\0" \
    "update_slave_spl=" \
        "tftpboot ${dtb_load_addr_virt} c810/u-boot-spl.bin ; " \
        "setexpr fw_sz ${filesize} / 0x200 ; " \
        "setexpr fw_sz ${fw_sz} + 1 ; " \
        "mmc dev 0 1 ; "  /* uboot -> eMMC BOOT PARTITION #1 */ \
        "mmc write ${dtb_load_addr_virt} ${slave_spl_start_sector} ${fw_sz} ; " \
        "mmc dev 0 0 ; "  /* restore to USER PARTITION */ \
        "\0" \
    "update_slave_fdt=" \
        "tftpboot ${dtb_load_addr_virt} c810/dt.dtb ; " \
        "setexpr fw_sz ${filesize} / 0x200 ; " \
        "setexpr fw_sz ${fw_sz} + 1 ; " \
        "mmc dev 0 1 ; "  /* uboot -> eMMC BOOT PARTITION #1 */ \
        "mmc write ${dtb_load_addr_virt} ${slave_fdt_start_sector} ${fw_sz} ; " \
        "mmc dev 0 0 ; "  /* restore to USER PARTITION */ \
        "\0" \
    "update_slave_uboot=" \
        "tftpboot ${dtb_load_addr_virt} c810/u-boot.bin ; " \
        "setexpr fw_sz ${filesize} / 0x200 ; " \
        "setexpr fw_sz ${fw_sz} + 1 ; " \
        "mmc dev 0 1 ; "  /* uboot -> eMMC BOOT PARTITION #1 */ \
        "mmc write ${dtb_load_addr_virt} ${slave_uboot_start_sector} ${fw_sz} ; " \
        "mmc dev 0 0 ; "  /* restore to USER PARTITION */ \
        "\0" \
    "update_slave_dtb=" \
        TFTP_LOAD_SLAVE_DTB \
        "setexpr fw_sz ${filesize} / 0x200 ; " \
        "setexpr fw_sz ${fw_sz} + 1 ; " \
        "mmc write ${dtb_load_addr_virt} ${slave_dtb_start_sector} ${fw_sz} ; " \
        "\0" \
    "update_slave_linux=" \
        "tftpboot ${linux_load_addr_virt} c810/uImage ; " \
        "setexpr fw_sz ${filesize} / 0x200 ; " \
        "setexpr fw_sz ${fw_sz} + 1 ; " \
        "mmc write ${linux_load_addr_virt} ${slave_linux_start_sector} ${fw_sz} ; " \
        "\0" \
    "boot_slave=" \
        "mmc dev 0 1 ; "  /* uboot -> eMMC BOOT PARTITION #1 */ \
        "mmc read ${slave_spl_load_addr_virt} ${slave_spl_start_sector} 0x40;" \
        "mmc dev 0 0 ; "  /* restore to USER PARTITION */ \
        "cp ${slave_spl_load_addr_virt} ${sram_addr_virt} 0x8000;" \
        "bootslave ${sram_addr_virt}; " \
        "\0"

#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND \
        "mmc read ${dtb_load_addr_virt} ${dtb_start_sector} ${dtb_size_sectors} ; " \
        "mmc read ${linux_load_addr_virt} ${linux_start_sector} ${linux_size_sectors} ; " \
        "mmc read ${ramdisk_load_addr_virt} ${ramdisk_start_sector} ${ramdisk_size_sectors} ; " \
        "run boot_slave; " \
        "bootm ${linux_load_addr_virt} - ${dtb_load_addr_virt}"


#endif /* __CONFIG_H */
