/*
 * Copyright (C) 2018-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <asm/clock.h>
#include <miiphy.h>

void gmac_clk_config(u32 interface)
{
#ifdef CONFIG_IS_ASIC
	if (interface == PHY_INTERFACE_MODE_MII) {
		*(volatile unsigned int *)(0xbe83025c) = 0x0; //MII MODE
		*(volatile unsigned int *)(0xbe83031c) = 0x1; //CLK_OUT pad disable
		*(volatile unsigned int *)(0xbff770c0) = 0x18a;
	} else if (interface == PHY_INTERFACE_MODE_RMII) {
		*(volatile unsigned int *)(0xbe83025c) = 0x4; //RMII MODE

		//RMII, External 100Mbps Mode
		*(volatile unsigned int *)(0xbe83031c) = 0x0; //CLK_OUT pad enable

		//50MHz
		*(volatile unsigned int *)(0xbff770cc) = 0x14;
		*(volatile unsigned int *)(0xbff770cc) = 0x80000014;

		//25MHz
		*(volatile unsigned int *)(0xbff770d4) = 0x2;
		*(volatile unsigned int *)(0xbff770d4) = 0x80000002;

		//enable rmii clocks
		*(volatile unsigned int *)(0xbff770c0) = 0xd68;
	} else if (interface == PHY_INTERFACE_MODE_RGMII) {

		// PAD CFG
		*(volatile unsigned int *)0xbe8302cc = 0x20202020;
		*(volatile unsigned int *)0xbe8302d0 = 0x07072020;
		*(volatile unsigned int *)0xbe8302d4 = 0x07070707;

		// RX delay
		*(volatile unsigned int *)0xbff770c4 = 0x1f;
		// TX delay
		*(volatile unsigned int *)0xbff770c8 = 0;

		// set GMAC PHY interface, 0:MII/GMII, 1:RGMII, 4:RMII
		*(volatile unsigned int *)0xbe83025c = 1;

		// set GMAC TXCLK direction, 1:input, 0:output
		//     MII        : TXCLK is input
		//     GMII/RGMII : TXCLK is output
		*(volatile unsigned int *)0xbe83031c = 0;

		// enable pll_div, 1000/4=250MHz
		*(volatile unsigned int *)0xbff770cc = 0x4;
		*(volatile unsigned int *)0xbff770cc = 0x80000004;
#if 0
		//
		// enable gtx_clk_div, 250/2=125MHz for 1Gbps speed
		//
		*(volatile unsigned int *)0xbff770d0 = 0x2
		*(volatile unsigned int *)0xbff770d0 = 0x80000002
#else
		//
		// enable gtx_clk_div, 250/10=25MHz for 100Mbps speed
		//
		*(volatile unsigned int *)0xbff770d0 = 0xa;
		*(volatile unsigned int *)0xbff770d0 = 0x8000000a;
#endif
		// enable gmac clocks
		*(volatile unsigned int *)0xbff770c0 = 0x13dc;
	}
#endif
}

void clock_init(void)
{
	// release phy by gpio0_0
	*(volatile unsigned int*)(0xbff71000) = 0x1;

#ifdef CONFIG_RGMII
	gmac_clk_config(PHY_INTERFACE_MODE_RGMII);
#else
	gmac_clk_config(PHY_INTERFACE_MODE_MII);
#endif
}
