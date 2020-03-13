/*
 * Copyright (C) 2020 C-SKY Microsystems
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <asm/byteorder.h>
#include <asm/io.h>
#include <hardware.h>

DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_CMD_BOOT_SLAVE
int do_bootslave(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
    u32 *slave_ddr_base;
    u32 slave_jump_addr;

    if (argc < 2) {
        return CMD_RET_USAGE;
    }

    // hold slave cpu
    writel(0, SLAVE_RESET_CONTROL);

    slave_ddr_base = simple_strtoull(argv[1], NULL, 16);
    slave_jump_addr = *slave_ddr_base;
    printf("slave_ddr_base: 0x%x\n", slave_ddr_base);
    printf("slave_jump_addr: 0x%x\n", slave_jump_addr);

    // set slave jump addr
    writel(slave_jump_addr, SYSREG_BASEADDR);

    flush_cache(slave_ddr_base, 0x10000);
    flush_cache(SYSREG_BASEADDR, 0x1000);

    // release slave cpu
    printf("reset slave cpu\n");
    writel(1, SLAVE_RESET_CONTROL);

    return 0;
}
#endif // CONFIG_CMD_BOOT_SLAVE
