/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __ASM_ARCH_HARDWARE_H__
#define __ASM_ARCH_HARDWARE_H__

#define INTC_BASEADDR        (u32)(0x1fba3000) // TODO: modify it according to ICE address map
#define GPIOA_BASEADDR       (u32)(0x1fbac000) // TODO: modify it according to ICE address map
#define GPIOB_BASEADDR       (u32)(0x1fbad000) // TODO: modify it according to ICE address map
#define GPIOC_BASEADDR       (u32)(0x1fbae000) // TODO: modify it according to ICE address map
#define GPIOD_BASEADDR       (u32)(0x1fbaf000) // TODO: modify it according to ICE address map
#define TIMER_BASEADDR       (u32)(0x1fba2000) // TODO: modify it according to ICE address map
#define EMMC_BASEADDR        (u64)(0x3fffb0000)
#define OM_BASEADDR          (u64)(0x3fff72000) // GPIOB_BASEADDR + 0x50
#define SPI0_BASEADDR        (u64)(0x3fff75000)
#define UART0_BASEADDR       (u64)(0x3fff73000)
#define UART1_BASEADDR       (u32)(0xf7018000)


#define CONSOLE_UART_BASE    UART0_BASEADDR

#endif /* __ASM_ARCH_HARDWARE_H__ */
