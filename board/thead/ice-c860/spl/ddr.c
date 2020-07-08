/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <asm/types.h>
#include "ddr.h"

//FPGA mode
#define FPGA

int init_ddr(void)
{
#ifdef FPGA

    // release ddr
    // set *0x3fff78040 = 0xffff
    u32* ddr_reg = (u32*)0xfff78040;
    *ddr_reg = 0xffff;

#else //ASIC

#endif
    return 0;
}
