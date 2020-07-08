/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __ASM_ARCH_HARDWARE_H__
#define __ASM_ARCH_HARDWARE_H__


#define GPIOA_BASEADDR       (u64)(0x3ffff7100)
#define GPIOB_BASEADDR       (u64)(0x3ffff7200)
#define TIMER_BASEADDR       (u64)(0x3ffff7000)
#define EMMC_BASEADDR        (u64)(0x3fffb0000)
#define OM_BASEADDR          (u64)(0x3fff72050) // GPIOB_BASEADDR + 0x50
#define SPI0_BASEADDR        (u64)(0x3fff75000)
#define UART0_BASEADDR       (u64)(0x3fff73000)
#define UART1_BASEADDR       (u64)(0x3fff73400)
#define SYSREG_BASEADDR      (u64)(0x3ffff0000)
#define SLAVE_RESET_CONTROL  (u64)(0x3fe830068)


#define CONSOLE_UART_BASE    UART0_BASEADDR


#endif /* __ASM_ARCH_HARDWARE_H__ */
