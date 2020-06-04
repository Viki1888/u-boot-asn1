/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

/*************************************************
ENABLED CLOCKs
OSC_CLK = 24MHz
CK860 core_clock = 1GHz
AXI Bus clock = 500MHz
AHB clock = 250MHz(middle-speed peripherals, SFC/SDIO/GMAC/I2S/RDMA..)
CFG_APB clock = 125MHz (CAN, DDR/USB/PCIe PHY...)
PERI_APB clock = 62.5MHz (most of low-speed peripherals, timer/uart/i2c...)
IAS(8core) core_clock = 700MHz
DDR core_clock = 664MHz, 2666MT mode
SDIO0/1 cclk = 100MHz (software should drived cclk_out=25/50MHz in sd_ctrl)

GATED CLOCK:
1)CK810 core_clock: enable in software driver when needed
2)VIDEO:  enable in software driver when needed
3)SCE: enable in software driver when needed
4)I2S/PCIE/USB/GMAC: enable in software driver when needed
*************************************************/

void sys_clk_config(int ddr_freq)
{
    unsigned int read,i;

    for(i=0;i<0x40;i++){
         *(volatile unsigned int*)(0xfff78040) = 0x0; //reset ddr
    }
    
    
    //Wait ALL PLLs lock
    read = *(volatile unsigned int *)(0xfff77060);
    while ((read & 0x1f) != 0x1f) {
        read = *(volatile unsigned int *)(0xfff77060);
    }


    //***********************
    //CPU DIV
    //BUS DIV
    //CPU_CNT DIV
    //***********************
    //CPU_CLK=OSC_CLK, BUS_CLK=OSC_CLK/2, CNT_CLK=OSC_CLK/8
    *(volatile unsigned int *)(0xfff77070) = 0x871;	//AXI BUS 2:1
    read = *(volatile unsigned int *)(0xfff77070);	//wait
    *(volatile unsigned int *)(0xfff77070) = 0x879;	//sync
    asm("nop"); //MUST NOT REVISE
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");

    //***********************
    //hclk DIV
    //cfg_pclk DIV
    //peri_pclk DIV
    //***********************
    //HCLK=OSC_CLK, peri_pclk=OSC_CLK/4, cfg_pclk=OSC_CLK/2
    *(volatile unsigned int *)(0xfff77090) = 0x1302;
    read = *(volatile unsigned int *)(0xfff77090); //wait
    *(volatile unsigned int *)(0xfff77090) = 0x9b0a; //sync
    asm("nop"); //MUST NOT REVISE
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");

    //***********************
    //CPU_CLK=1GHz, BUS_CLK=500MHz, CNT_CLK=125MHz
    //HCLK=250MHz, peri_clk=62.5MHz, cfg_clk=125MHz
    //***********************
    *(volatile unsigned int *)(0xfff77070) = 0x809;
    asm("nop"); //MUST NOT REVISE
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");

    //enable all hclk(250MHz)
    *(volatile unsigned int *)(0xfff77094) = 0xffffffff;


    //enable all ias_clk(700MHz)
    //*(volatile unsigned int *)(0xfff77030) = 0x0160af02; //plld, 350MHz
        //*(volatile unsigned int *)(0xfff77038) = 0x2; //re-config
    *(volatile unsigned int *)(0xfff770a0) = 0xfff;
    *(volatile unsigned int *)(0xfff78044) = 0xff;

    //SDIO0/1
    *(volatile unsigned int *)(0xfff77078) = 0x0c800c80;

    //DDRC clock
    switch (ddr_freq) {
    case 3192:
        *(volatile unsigned int *)(0xfff77020) = 0x01208502; //798MHz, 3192MT
        break;
    case 3008:
        *(volatile unsigned int *)(0xfff77020) = 0x01305e01; //752MHz, 3008MT
        break;
    case 2912:
        *(volatile unsigned int *)(0xfff77020) = 0x01305b01; //728MHz, 2912MT
        break;
    case 2816:
        *(volatile unsigned int *)(0xfff77020) = 0x01305801; //704MHz, 2816MT
        break;
    case 2720:
        *(volatile unsigned int *)(0xfff77020) = 0x01305501; //680MHz, 2720MT
        break;
    case 2656:
        *(volatile unsigned int *)(0xfff77020) = 0x01305301; //664MHz, 2656MT
        break;
    case 2592:
        *(volatile unsigned int *)(0xfff77020) = 0x01305101; //648MHz, 2592MT
        break;
    case 2496:
        *(volatile unsigned int *)(0xfff77020) = 0x01304e01; //624MHz, 2496MT
        break;
    case 2400:
        *(volatile unsigned int *)(0xfff77020) = 0x01406401; //600MHz, 2400MT
        break;
    case 2184:
        *(volatile unsigned int *)(0xfff77020) = 0x01405b01; //546MHz, 2184MT
        break;
    case 2133:
        *(volatile unsigned int *)(0xfff77020) = 0x01405801; //528MHz, 2112MT
        break;
    case 2000:
        *(volatile unsigned int *)(0xfff77020) = 0x01607d01; //500MHz, 2000MT
        break;
    case 1800:
        *(volatile unsigned int *)(0xfff77020) = 0x01404b01; //450MHz, 1800MT
        break;
    case 1600:
        *(volatile unsigned int *)(0xfff77020) = 0x01606401; //400MHz, 1600MT
        break;
    case 1392:
        *(volatile unsigned int *)(0xfff77020) = 0x01605701; //348MHz, 1392MT
        break;
    default:
        *(volatile unsigned int *)(0xfff77020) = 0x01604b01; //300MHz, 1200MT
        break;
    }

    *(volatile unsigned int *)(0xfff77024) = 0x03000000;
    *(volatile unsigned int *)(0xfff77028) = 0x3;	//reconfig
    read = *(volatile unsigned int *)(0xfff77020);	//readback
    //Wait ALL PLLs Lock
    read = *(volatile unsigned int *)(0xfff77060);
    read = *(volatile unsigned int *)(0xfff77060);
    read = *(volatile unsigned int *)(0xfff77060);
    while ((read & 0x1f) != 0x1f) {
        read = *(volatile unsigned int *)(0xfff77060);
    }

    //enable ddr_axi clocks
    *(volatile unsigned int*)(0xfff78040) = 0x0;
    *(volatile unsigned int*)(0xfff78040) = 0x0;
    *(volatile unsigned int*)(0xfff78040) = 0x0;
    *(volatile unsigned int*)(0xfff78040) = 0x0;
    *(volatile unsigned int*)(0xfff77108) = 0xe4;
    *(volatile unsigned int*)(0xfff77108) = 0xe4;
    *(volatile unsigned int*)(0xfff77108) = 0xe4;
    *(volatile unsigned int*)(0xfff78040) = 0x0;
    *(volatile unsigned int*)(0xfff78040) = 0x0;
    *(volatile unsigned int*)(0xfff78040) = 0x0;
    *(volatile unsigned int*)(0xfff78040) = 0x0;
    *(volatile unsigned int*)(0xfff78040) = 0x9;
    *(volatile unsigned int*)(0xfff78040) = 0x9;

    //disable wdg
    *(volatile unsigned int*)(0xfff78000) = 0x5ada7200;
    *(volatile unsigned int*)(0xfff78010) = 0x0;
    

    ////RMII, External 100Mbps Mode
        //*(volatile unsigned int*)(0xfe83031c) = 0x0; //CLK_OUT pad enable
        //
        ////50MHz
        //*(volatile unsigned int*)(0xfff770cc) = 0x14;
        //*(volatile unsigned int*)(0xfff770cc) = 0x80000014;
        //read = *(volatile unsigned int*)(0xfff770cc);
        //
        ////25MHz
        //*(volatile unsigned int*)(0xfff770d4) = 0x2;
        //*(volatile unsigned int*)(0xfff770d4) = 0x80000002;
        //read = *(volatile unsigned int*)(0xfff770d4);
        //
        ////enable rmii clocks
        //*(volatile unsigned int*)(0xfff770c0) = 0xd68;
    



    //MII Mode
        //*(volatile unsigned int*)(0xfe83025c) = 0x0; //MII MODE
        //*(volatile unsigned int*)(0xfe83031c) = 0x1; //CLK_OUT pad enable
        //*(volatile unsigned int*)(0xfff770c0) = 0x18a;


    //RGMII mode
    //     //PAD DS
    //     *(volatile unsigned int*)(0xfe8302cc) = 0x20202020;
    //     *(volatile unsigned int*)(0xfe8302cf) = 0x07072020;
    //     *(volatile unsigned int*)(0xfe8302d4) = 0x07070707;

    //     //PHY Mode Sel
    //     *(volatile unsigned int*)(0xfe83025c) = 0x1; //0:MII/GMII, 1:RGMII, 4:RMII
    //     //Enable TX_CLK PAD 
    //     *(volatile unsigned int*)(0xfe83031c) = 0x0; //oen

    //     //enable pll_div, 1000/4=250MHz
    //     *(volatile unsigned int*)(0xfff770cc) = 0x4;
    //     *(volatile unsigned int*)(0xfff770cc) = 0x4;
    //     *(volatile unsigned int*)(0xfff770cc) = 0x80000004;
    //     *(volatile unsigned int*)(0xfff770cc) = 0x80000004;

    // //enable gtx_clk_div, 250/2=125MHz
    //     *(volatile unsigned int*)(0xfff770d0) = 0x2;
    //     *(volatile unsigned int*)(0xfff770d0) = 0x2;
    //     *(volatile unsigned int*)(0xfff770d0) = 0x80000002;
    //     *(volatile unsigned int*)(0xfff770d0) = 0x80000002;

    //     //enable ephy_ref_clk , 1000/40=25MHz
    //     *(volatile unsigned int*)(0xfff770d8) = 0x28;
    //     *(volatile unsigned int*)(0xfff770d8) = 0x28;
    //     *(volatile unsigned int*)(0xfff770d8) = 0x80000028;
    //     *(volatile unsigned int*)(0xfff770d8) = 0x80000028;

    //     //tx/rx inv/dly
    //     *(volatile unsigned int*)(0xfff770c4) = 0x1f;//rx
    //     *(volatile unsigned int*)(0xfff770c8) = 0x00;//tx

    //     //enable ephy_ref_clk, bit12
    //     //enable rx_clk/rx_clk_n(125MHz) from PAD, bit8/9,7
    //     //enable tx_clk/tx_clk_n(125MHz) from PLL, bit3/4,2
    //     //tx_clk_out sel RGMII, bit5=0
    //     //enable tx_clk_out, bit6
    //     *(volatile unsigned int*)(0xfff770c0) = 0x13dc;



    //**************************************
    //For 4core NPU low-power at 20200601
    //**************************************
    //1) disable NPU core4~7 clock and reset
        *(volatile unsigned int *)(0xfff770a0) = 0x3f;
        *(volatile unsigned int *)(0xfff78044) = 0x0f;

    //2) disable video clock
        *(volatile unsigned int *)(0xfff77074) = 0x0;
    
    //3) disable mipi clock
        *(volatile unsigned int *)(0xfff77100) = 0x2;

    //4) disable sce clock
        *(volatile unsigned int *)(0xfff77104) = 0x0;
    
    //5) power-off NPU4~7 and VDEC
        *(volatile unsigned int *)(0xfe830368) = 0x1f0; //iso_en
        *(volatile unsigned int *)(0xfe830360) = 0x3ff00; //pd_sw_en
    
    //read = *(volatile unsigned int *)(0xfe830368);
    //read = *(volatile unsigned int *)(0xfe830368);
    //read = *(volatile unsigned int *)(0xfe830368);
    //mini_printf("ACK=%x",read);
}
