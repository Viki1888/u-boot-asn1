/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#define apb_wr_4byte(addr,wdata) *(volatile unsigned int*)(addr) = wdata
#define apb_rd_4byte(addr,rdata) rdata = *(volatile unsigned int*)(addr)
#define apb_wr_2byte(addr,wdata) *(volatile unsigned short*)(addr) = (unsigned short)(wdata)
#define apb_rd_2byte(addr,rdata) rdata = (unsigned int)(*(volatile unsigned short*)(addr))

#include <common.h>
#include <console.h>
// #include <asm/mini_printf.h>

extern void ddr_fw_run(void);

void init_ddr(void)
{
    unsigned int temp_reg;
    apb_wr_4byte(0xfe820304, 0x00000001);
    apb_wr_4byte(0xfe820030, 0x00000001);
    apb_rd_4byte(0xfe820004, temp_reg);
    apb_wr_4byte(0xfe820000, 0x83080020);//2-Ranks
    apb_wr_4byte(0xfe820010, 0x0000e030);
    apb_wr_4byte(0xfe820014, 0x000301c8);
    apb_wr_4byte(0xfe82001c, 0xaa75b6fc);
    apb_wr_4byte(0xfe820020, 0x00001202);
    apb_wr_4byte(0xfe820024, 0xc609cfea);
    apb_wr_4byte(0xfe82002c, 0x00000001);
    apb_wr_4byte(0xfe820030, 0x00000000);
    apb_wr_4byte(0xfe820034, 0x00409104);
    apb_wr_4byte(0xfe820038, 0x006a0002);
    apb_wr_4byte(0xfe820050, 0x00210000);
    apb_wr_4byte(0xfe820054, 0x00190010);
    apb_wr_4byte(0xfe820060, 0x00000001);
    apb_wr_4byte(0xfe820064, 0x006180e0);
    apb_wr_4byte(0xfe820068, 0x00480000);
    apb_wr_4byte(0xfe820070, 0x003f7f10);
    apb_wr_4byte(0xfe820074, 0x00000390);
    apb_wr_4byte(0xfe82007c, 0x00000300);
    apb_wr_4byte(0xfe8200b8, 0x01000eec);
    apb_wr_4byte(0xfe8200bc, 0x0703c114);
    apb_wr_4byte(0xfe8200c0, 0x00000000);
    apb_wr_4byte(0xfe8200c4, 0x00000000);

    apb_wr_4byte(0xfe8200d0, 0xc0020002); //phy train, MUST
    apb_wr_4byte(0xfe8200d4, 0x00020002);
    apb_wr_4byte(0xfe8200d8, 0x0000c105);
    apb_wr_4byte(0xfe8200dc, 0x0054002d);///MR1=0x54, MR2=0x2d
    apb_wr_4byte(0xfe8200e0, 0x00330000);//MR3=0x33,MR13=0x0
    apb_wr_4byte(0xfe8200e8, 0x0064004d);//MR11=0x64, MR12=0x4d
    apb_wr_4byte(0xfe8200ec, 0x0004004f);//MR22=0x4, MR14=4f
    apb_wr_4byte(0xfe8200e4, 0x00030005);
    apb_wr_4byte(0xfe8200f0, 0x00000020);
    apb_wr_4byte(0xfe8200f4, 0x000003df);
    apb_wr_4byte(0xfe820100, 0x1a203622);
    apb_wr_4byte(0xfe820104, 0x00060630);
    apb_wr_4byte(0xfe820108, 0x070e0f14);
    apb_wr_4byte(0xfe82010c, 0x00b0c006);
    apb_wr_4byte(0xfe820110, 0x0f04080f);
    apb_wr_4byte(0xfe820114, 0x02040c0c);
    apb_wr_4byte(0xfe820118, 0x01010007);
    apb_wr_4byte(0xfe82011c, 0x00000402);
    apb_wr_4byte(0xfe820120, 0x01010101);
    apb_wr_4byte(0xfe820124, 0x00000019);
    apb_wr_4byte(0xfe820128, 0x00010606);
    apb_wr_4byte(0xfe82012c, 0x01010006);
    apb_wr_4byte(0xfe820130, 0x00020000);
    apb_wr_4byte(0xfe820134, 0x0c100002);
    apb_wr_4byte(0xfe820138, 0x000000e6);
    apb_wr_4byte(0xfe82013c, 0x80000000);
    apb_wr_4byte(0xfe820180, 0xd3200018);
    apb_wr_4byte(0xfe820184, 0x028d4c76);
    apb_wr_4byte(0xfe820188, 0x00000000);
    apb_wr_4byte(0xfe820190, 0x0497820a);
    apb_wr_4byte(0xfe820194, 0x00090303);
    apb_wr_4byte(0xfe820198, 0x0350b100);
    apb_wr_4byte(0xfe82019c, 0x00000071);
    apb_wr_4byte(0xfe8201a0, 0x80400018);
    apb_wr_4byte(0xfe8201a4, 0x00450055);
    apb_wr_4byte(0xfe8201a8, 0x00000000);
    apb_wr_4byte(0xfe8201b0, 0x00000051);
    apb_wr_4byte(0xfe8201b4, 0x0000170a);
    apb_wr_4byte(0xfe8201b8, 0x00000008);
    apb_wr_4byte(0xfe8201c0, 0x00000001);
    apb_wr_4byte(0xfe8201c4, 0x00000000);

    //by gaohf at 2020/02/26, RBC (3BA,16R,10C,32bit)
    //16Gb x 2pcs * 2RANK = 8GB
    //Mapping: {RANK, ROW, BANK, COL},R16 DO NOT USED
    apb_wr_4byte(0xfe820200, 0x001f1f17);
    apb_wr_4byte(0xfe820204, 0x00080808);
    apb_wr_4byte(0xfe820208, 0x00000000);
    apb_wr_4byte(0xfe82020c, 0x00000000);
    apb_wr_4byte(0xfe820210, 0x00001f1f);
    apb_wr_4byte(0xfe820214, 0x07070707);
    apb_wr_4byte(0xfe820218, 0x07070707);
    apb_wr_4byte(0xfe82021c, 0x00000f0f);
    apb_wr_4byte(0xfe820220, 0x00003f3f);
    apb_wr_4byte(0xfe820224, 0x00000000);
    apb_wr_4byte(0xfe820228, 0x00000000);
    apb_wr_4byte(0xfe82022c, 0x001f1f00);

    apb_wr_4byte(0xfe820240, 0x06080a58);
    apb_wr_4byte(0xfe820244, 0x00000000);
    apb_wr_4byte(0xfe820250, 0x00c21f00);
    apb_wr_4byte(0xfe820254, 0x00000000);
    apb_wr_4byte(0xfe82025c, 0x0f000001);
    apb_wr_4byte(0xfe820264, 0x0f00007f);
    apb_wr_4byte(0xfe82026c, 0x0f00007f);
    apb_wr_4byte(0xfe820300, 0x00000000);
    apb_wr_4byte(0xfe820304, 0x00000000);
    apb_wr_4byte(0xfe82030c, 0x00000000);
    apb_wr_4byte(0xfe820320, 0x00000001);
    apb_wr_4byte(0xfe820328, 0x00000000);
    apb_wr_4byte(0xfe82036c, 0x00110001);
    apb_wr_4byte(0xfe820374, 0x0000014f);
    apb_wr_4byte(0xfe82037c, 0x00000000);
    apb_wr_4byte(0xfe820380, 0x00000000);
    apb_wr_4byte(0xfe820384, 0x00000000);
    apb_wr_4byte(0xfe820490, 0x00000001);
    apb_wr_4byte(0xfe820540, 0x00000001);
    apb_wr_4byte(0xfe8205f0, 0x00000001);
    apb_wr_4byte(0xfe8206a0, 0x00000001);
    apb_wr_4byte(0xfe820750, 0x00000001);
    apb_wr_4byte(0xfe820800, 0x00000001);
    apb_wr_4byte(0xfe822020, 0x00001202);
    apb_wr_4byte(0xfe822024, 0xc609cfea);
    apb_wr_4byte(0xfe822034, 0x00409104);
    apb_wr_4byte(0xfe822050, 0x00210000);
    apb_wr_4byte(0xfe822064, 0x006180e0);
    apb_wr_4byte(0xfe822068, 0x00480000);
    apb_wr_4byte(0xfe8220dc, 0x0054002d);//MR1=0x54, MR2=0x2d
    apb_wr_4byte(0xfe8220e0, 0x00330000);//MR3=0x33,MR13=0x0
    apb_wr_4byte(0xfe8220e8, 0x0064004d);//MR11=0x64, MR12=0x4d
    apb_wr_4byte(0xfe8220ec, 0x0004004f);//MR22=0x4, MR14=4f
    apb_wr_4byte(0xfe8220f4, 0x000003df);
    apb_wr_4byte(0xfe822100, 0x1a203622);
    apb_wr_4byte(0xfe822104, 0x00060630);
    apb_wr_4byte(0xfe822108, 0x070e0f14);
    apb_wr_4byte(0xfe82210c, 0x00b0c006);
    apb_wr_4byte(0xfe822110, 0x0f04080f);
    apb_wr_4byte(0xfe822114, 0x02040c0c);
    apb_wr_4byte(0xfe822118, 0x01010007);
    apb_wr_4byte(0xfe82211c, 0x00000402);
    apb_wr_4byte(0xfe822120, 0x01010101);
    apb_wr_4byte(0xfe822124, 0x00000019);
    apb_wr_4byte(0xfe822128, 0x00010606);
    apb_wr_4byte(0xfe82212c, 0x01010006);
    apb_wr_4byte(0xfe822130, 0x00020000);
    apb_wr_4byte(0xfe822134, 0x0c100002);
    apb_wr_4byte(0xfe822138, 0x000000e6);
    apb_wr_4byte(0xfe82213c, 0x80000000);
    apb_wr_4byte(0xfe822180, 0xd3200018);
    apb_wr_4byte(0xfe822190, 0x0497820a);
    apb_wr_4byte(0xfe822194, 0x00090303);
    apb_wr_4byte(0xfe8221b4, 0x0000170a);
    apb_wr_4byte(0xfe8221b8, 0x00000008);
    apb_wr_4byte(0xfe822240, 0x06080a58);
    apb_rd_4byte(0xfe820060, temp_reg);
    apb_wr_4byte(0xfe820400, 0x00000000);
    apb_wr_4byte(0xfe820404, 0x0000000f);
    apb_wr_4byte(0xfe8204b4, 0x0000000f);
    apb_wr_4byte(0xfe820564, 0x0000000f);
    apb_wr_4byte(0xfe820614, 0x0000000f);
    apb_wr_4byte(0xfe8206c4, 0x0000000f);
    apb_wr_4byte(0xfe820774, 0x0000000f);
    apb_wr_4byte(0xfe820404, 0x0000100f);
    apb_wr_4byte(0xfe8204b4, 0x0000100f);
    apb_wr_4byte(0xfe820564, 0x0000100f);
    apb_wr_4byte(0xfe820614, 0x0000100f);
    apb_wr_4byte(0xfe8206c4, 0x0000100f);
    apb_wr_4byte(0xfe820774, 0x0000100f);
    apb_wr_4byte(0xfe820404, 0x0000100f);
    apb_wr_4byte(0xfe8204b4, 0x0000100f);
    apb_wr_4byte(0xfe820564, 0x0000100f);
    apb_wr_4byte(0xfe820614, 0x0000100f);
    apb_wr_4byte(0xfe8206c4, 0x0000100f);
    apb_wr_4byte(0xfe820774, 0x0000100f);
    apb_wr_4byte(0xfe820404, 0x0000100f);
    apb_wr_4byte(0xfe8204b4, 0x0000100f);
    apb_wr_4byte(0xfe820564, 0x0000100f);
    apb_wr_4byte(0xfe820614, 0x0000100f);
    apb_wr_4byte(0xfe8206c4, 0x0000100f);
    apb_wr_4byte(0xfe820774, 0x0000100f);
    apb_wr_4byte(0xfe820408, 0x0000400f);
    apb_wr_4byte(0xfe8204b8, 0x0000400f);
    apb_wr_4byte(0xfe820568, 0x0000400f);
    apb_wr_4byte(0xfe820618, 0x0000400f);
    apb_wr_4byte(0xfe8206c8, 0x0000400f);
    apb_wr_4byte(0xfe820778, 0x0000400f);
    apb_wr_4byte(0xfe820408, 0x0000500f);
    apb_wr_4byte(0xfe8204b8, 0x0000500f);
    apb_wr_4byte(0xfe820568, 0x0000500f);
    apb_wr_4byte(0xfe820618, 0x0000500f);
    apb_wr_4byte(0xfe8206c8, 0x0000500f);
    apb_wr_4byte(0xfe820778, 0x0000500f);
    apb_wr_4byte(0xfe820408, 0x0000500f);
    apb_wr_4byte(0xfe8204b8, 0x0000500f);
    apb_wr_4byte(0xfe820568, 0x0000500f);
    apb_wr_4byte(0xfe820618, 0x0000500f);
    apb_wr_4byte(0xfe8206c8, 0x0000500f);
    apb_wr_4byte(0xfe820778, 0x0000500f);
    apb_wr_4byte(0xfe820408, 0x0000100f);
    apb_wr_4byte(0xfe8204b8, 0x0000100f);
    apb_wr_4byte(0xfe820568, 0x0000100f);
    apb_wr_4byte(0xfe820618, 0x0000100f);
    apb_wr_4byte(0xfe8206c8, 0x0000100f);
    apb_wr_4byte(0xfe820778, 0x0000100f);

    apb_rd_4byte(0xfe820060, temp_reg); //delay
    apb_rd_4byte(0xfe820060, temp_reg); //delay
    apb_rd_4byte(0xfe820060, temp_reg); //delay

    //Release Resetn
    apb_wr_4byte(0xfff78040, 0x3ff);
    apb_wr_4byte(0xfff78040, 0x3ff);
    apb_wr_4byte(0xfff78040, 0x3ff);
    apb_wr_4byte(0xfff78040, 0x3ff);
    apb_wr_4byte(0xfff78040, 0x3ff);

    //***********************
    // VERY IMPORT
    //***********************
    apb_wr_4byte(0xfe820060, 0x00000001); //disable dis_auto_refresh
    //disable selref_en, powerdown_en, en_dfi_dram_clk_disable
    apb_wr_4byte(0xfe820030, 0x00000020); //selfref_sw=1
    apb_wr_4byte(0xfe820320, 0x0); //SWCTL.sw_done
    apb_wr_4byte(0xfe8201b0, 0x00000050);//bit[12:8]:0~3:Goto PHY P0~3
                                        //bit[6]:dis_dyn_adr_tri
                                        //bit[5]:dfi_init_start
                                        //bit[4]:ctl_idle_en
                                        //bit[0]:dfi_init_complete_en, mask during phy init
    apb_wr_4byte(0xfe820320, 0x1); //SWCTL.sw_done
    apb_wr_4byte(0xfe820304, 0x00000000); //disable CAM

    apb_rd_4byte(0xfe820000, temp_reg); //delay
    apb_rd_4byte(0xfe820000, temp_reg); //delay
    apb_rd_4byte(0xfe820000, temp_reg); //delay
    apb_rd_4byte(0xfe820000, temp_reg); //delay

    ddr_fw_run();

    //***********************************
    // Switch PHY to MissionMode
    //***********************************
    apb_wr_4byte(0xfe820320, 0x0);//sw_done, prepare reg write
    apb_wr_4byte(0xfe8201b0, 0x00000070);//bit[12:8]:0~3:Goto PHY P0~3
                                        //bit[6]:dis_dyn_adr_tri
                        //bit[5]:dfi_init_start
                        //bit[4]:ctl_idle_en
                        //bit[0]:dfi_init_complete_en
    apb_wr_4byte(0xfe820320, 0x1);//sw_done,reg write in

    //read SWSTAT, wait sw_done_ack = 1
    apb_rd_4byte(0xfe820324,temp_reg);
    while(temp_reg == 0)
    {
        apb_rd_4byte(0xfe820324,temp_reg);
    }

    //Poll DFISTAT, wait dfi_init_complete
    apb_rd_4byte(0xfe8201bc,temp_reg);
    while(temp_reg == 0)
    {
        apb_rd_4byte(0xfe8201bc,temp_reg);
    }

    //***********************************
    // Switch CTRL to MissionMode
    //***********************************
    apb_wr_4byte(0xfe820320, 0x0);//sw_done, prepare reg write
    apb_wr_4byte(0xfe8201b0, 0x00000050);//bit[12:8]:0~3:Goto PHY P0~3
                                        //bit[6]:dis_dyn_adr_tri
                        //bit[5]:dfi_init_start
                        //bit[4]:ctl_idle_en
                        //bit[0]:dfi_init_complete_en
    apb_wr_4byte(0xfe8201b0, 0x00000051);//bit[12:8]:0~3:Goto PHY P0~3
    apb_wr_4byte(0xfe820030, 0x00000000);//bit[5]:soft exit from selfref
    apb_wr_4byte(0xfe820320, 0x1);//sw_done,reg write in

    //read SWSTAT, wait sw_done_ack = 1
    apb_rd_4byte(0xfe820324,temp_reg);
    while(temp_reg == 0)
    {
        apb_rd_4byte(0xfe820324,temp_reg);
    }

    //STAT, operation_mode
    //      3'h0 : Init
    //      3'h1 : Normal
    //      3'h2 : Power-down
    //      3'h3 : Self-refresh/Self-refresh power-down
    apb_rd_4byte(0xfe820004,temp_reg);
    while((temp_reg&0x7) != 1)
    {
        apb_rd_4byte(0xfe820004,temp_reg);
    }

    //***********************************
    // Enable AXI ports
    //***********************************
    apb_wr_4byte(0xfe820060, 0x00000000);//enable auto_refresh
    apb_wr_4byte(0xfe820304, 0x00000000);//enable CAM
    apb_wr_4byte(0xfe820030, 0x00000000);
    //apb_wr_4byte(0xfe820030, 0x0000000a); //auto-dis-clk/pwr_down
    apb_wr_4byte(0xfe820490, 0x00000001);
    apb_wr_4byte(0xfe820540, 0x00000001);
    apb_wr_4byte(0xfe8205f0, 0x00000001);
    apb_wr_4byte(0xfe8206a0, 0x00000001);
    apb_wr_4byte(0xfe820750, 0x00000001);
    apb_wr_4byte(0xfe820800, 0x00000001);

}

