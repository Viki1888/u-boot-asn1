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

extern void ddr_fw_run(void);

void init_ddr(void)
{
unsigned int temp_reg;

apb_wr_4byte(0xfe820304, 0x00000001);
apb_wr_4byte(0xfe820030, 0x00000001);
apb_rd_4byte(0xfe820004, temp_reg);
apb_wr_4byte(0xfe820000, 0x81080020);
apb_wr_4byte(0xfe820010, 0x00008030);
apb_wr_4byte(0xfe820014, 0x00000d6d);
apb_wr_4byte(0xfe82001c, 0xae593c40);
apb_wr_4byte(0xfe820020, 0x00001202);
apb_wr_4byte(0xfe820024, 0x2640e550);
apb_wr_4byte(0xfe82002c, 0x00000001);
apb_wr_4byte(0xfe820030, 0x000000a2);
apb_wr_4byte(0xfe820034, 0x00099f01);
apb_wr_4byte(0xfe820038, 0x00560000);
apb_wr_4byte(0xfe820050, 0x00a06070);
apb_wr_4byte(0xfe820054, 0x0008001a);
apb_wr_4byte(0xfe820060, 0x00000000);
apb_wr_4byte(0xfe820064, 0x005180bb);
apb_wr_4byte(0xfe820068, 0x003c0000);
//apb_wr_4byte(0xfe820070, 0x00341594);
//apb_wr_4byte(0xfe820074, 0x00000112);
apb_wr_4byte(0xfe82007c, 0x00000300);
apb_wr_4byte(0xfe8200b8, 0x00000eac);
apb_wr_4byte(0xfe8200bc, 0x2401a178);
apb_wr_4byte(0xfe8200c0, 0x00000000);
apb_wr_4byte(0xfe8200c4, 0x00000000);
apb_wr_4byte(0xfe8200d0, 0xc0030002);
apb_wr_4byte(0xfe8200d4, 0x00030001);
apb_wr_4byte(0xfe8200d8, 0x00000705);

apb_wr_4byte(0xfe8200dc, 0x00440024);//MR1 =0x44, MR2 =0x24
apb_wr_4byte(0xfe8200e0, 0x00f00008);//MR3 =0xf0, MR13=0x08
apb_wr_4byte(0xfe8200e8, 0x0064004d);//MR11=0x64, MR12=0x4d
apb_wr_4byte(0xfe8200ec, 0x0000004d);//MR22=0x00, MR14=0x4d

apb_wr_4byte(0xfe8200e4, 0x00040008);
apb_wr_4byte(0xfe8200f0, 0x00000000);
apb_wr_4byte(0xfe8200f4, 0x0000e334);
apb_wr_4byte(0xfe820100, 0x171b2d1c);
apb_wr_4byte(0xfe820104, 0x00050528);
apb_wr_4byte(0xfe820108, 0x060e1315);
apb_wr_4byte(0xfe82010c, 0x0090a006);
apb_wr_4byte(0xfe820110, 0x0c04070c);
apb_wr_4byte(0xfe820114, 0x0f060a0a);
apb_wr_4byte(0xfe820118, 0x0801000a);
apb_wr_4byte(0xfe82011c, 0x00000602);
apb_wr_4byte(0xfe820120, 0x01011a01);
apb_wr_4byte(0xfe820124, 0x40000008);
apb_wr_4byte(0xfe820128, 0x00000d04);
apb_wr_4byte(0xfe82012c, 0x0101000c);
apb_wr_4byte(0xfe820130, 0x00020000);
apb_wr_4byte(0xfe820134, 0x0b100002);
apb_wr_4byte(0xfe820138, 0x000003eb);
apb_wr_4byte(0xfe82013c, 0x80000000);
apb_wr_4byte(0xfe820180, 0x529b0014);
apb_wr_4byte(0xfe820184, 0x02200070);
apb_wr_4byte(0xfe820188, 0x00000000);
apb_wr_4byte(0xfe820190, 0x04978208);
apb_wr_4byte(0xfe820194, 0x00090303);
apb_wr_4byte(0xfe820198, 0x03312101);
apb_wr_4byte(0xfe82019c, 0x00000000);
apb_wr_4byte(0xfe8201a0, 0x60400018);
apb_wr_4byte(0xfe8201a4, 0x0053005e);
apb_wr_4byte(0xfe8201a8, 0x80000000);
apb_wr_4byte(0xfe8201b0, 0x00000051);
apb_wr_4byte(0xfe8201b4, 0x00001708);
apb_wr_4byte(0xfe8201b8, 0x00000008);
apb_wr_4byte(0xfe8201c0, 0x00000007);
apb_wr_4byte(0xfe8201c4, 0x00000000);

//apb_wr_4byte(0xfe820200, 0x0000001f);
//apb_wr_4byte(0xfe820204, 0x00121102);
//apb_wr_4byte(0xfe820208, 0x01010000);
//apb_wr_4byte(0xfe82020c, 0x13131301);
//apb_wr_4byte(0xfe820210, 0x00001f1f);
//apb_wr_4byte(0xfe820214, 0x02000b0b);
//apb_wr_4byte(0xfe820218, 0x40050406);
//apb_wr_4byte(0xfe82021c, 0x00000f0f);
//apb_wr_4byte(0xfe820220, 0x00000000);
//apb_wr_4byte(0xfe820224, 0x0908070b);
//apb_wr_4byte(0xfe820228, 0x08080408);
//apb_wr_4byte(0xfe82022c, 0x00000006);


//by gaohf at 2020/04/02, RBC (3BA,16R,10C,32bit)
//16Gb x 2pcs = 4GB
//Mapping: {ROW, BANK, COL},R16 DO NOT USED
apb_wr_4byte(0xfe820200, 0x001f1f1f);
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

apb_wr_4byte(0xfe820240, 0x050c0560);
apb_wr_4byte(0xfe820244, 0x00000000);
apb_wr_4byte(0xfe820250, 0x48b40b04);
apb_wr_4byte(0xfe820254, 0x00000011);
apb_wr_4byte(0xfe82025c, 0xa200d6cd);
apb_wr_4byte(0xfe820264, 0x92009cee);
apb_wr_4byte(0xfe82026c, 0x9100ae55);
apb_wr_4byte(0xfe820300, 0x00000040);
apb_wr_4byte(0xfe820304, 0x00000000);
apb_wr_4byte(0xfe82030c, 0x00000000);
apb_wr_4byte(0xfe820320, 0x00000001);
apb_wr_4byte(0xfe820328, 0x00000000);
apb_wr_4byte(0xfe82036c, 0x00100010);
apb_wr_4byte(0xfe820374, 0x0000008f);
apb_wr_4byte(0xfe82037c, 0x00000000);
apb_wr_4byte(0xfe820380, 0x00000000);
apb_wr_4byte(0xfe820384, 0x00000000);
apb_wr_4byte(0xfe820400, 0x00000011);
apb_wr_4byte(0xfe820404, 0x000000b6);
apb_wr_4byte(0xfe820408, 0x0000738f);
apb_wr_4byte(0xfe820490, 0x00000001);
apb_wr_4byte(0xfe820494, 0x00110002);
apb_wr_4byte(0xfe820498, 0x05630051);
apb_wr_4byte(0xfe82049c, 0x01100803);
apb_wr_4byte(0xfe8204a0, 0x0453025a);
apb_wr_4byte(0xfe8204b4, 0x0000224c);
apb_wr_4byte(0xfe8204b8, 0x000040a8);
apb_wr_4byte(0xfe820540, 0x00000001);
apb_wr_4byte(0xfe820544, 0x00110005);
apb_wr_4byte(0xfe820548, 0x025400cc);
apb_wr_4byte(0xfe82054c, 0x01000d0c);
apb_wr_4byte(0xfe820550, 0x00f60002);
apb_wr_4byte(0xfe820564, 0x000000f5);
apb_wr_4byte(0xfe820568, 0x00000065);
apb_wr_4byte(0xfe8205f0, 0x00000001);
apb_wr_4byte(0xfe8205f4, 0x00100007);
apb_wr_4byte(0xfe8205f8, 0x06fa009a);
apb_wr_4byte(0xfe8205fc, 0x01010c04);
apb_wr_4byte(0xfe820600, 0x02af0645);
apb_wr_4byte(0xfe820614, 0x0000207d);
apb_wr_4byte(0xfe820618, 0x0000204d);
apb_wr_4byte(0xfe8206a0, 0x00000001);
apb_wr_4byte(0xfe8206a4, 0x00010006);
apb_wr_4byte(0xfe8206a8, 0x005c074e);
apb_wr_4byte(0xfe8206ac, 0x01010e02);
apb_wr_4byte(0xfe8206b0, 0x07ca01e1);
apb_wr_4byte(0xfe8206c4, 0x00002353);
apb_wr_4byte(0xfe8206c8, 0x000070c8);
apb_wr_4byte(0xfe820750, 0x00000001);
apb_wr_4byte(0xfe820754, 0x00200003);
apb_wr_4byte(0xfe820758, 0x00990067);
apb_wr_4byte(0xfe82075c, 0x01100702);
apb_wr_4byte(0xfe820760, 0x04d105b9);
apb_wr_4byte(0xfe820774, 0x0000415a);
apb_wr_4byte(0xfe820778, 0x00007381);
apb_wr_4byte(0xfe820800, 0x00000001);
apb_wr_4byte(0xfe820804, 0x0002000d);
apb_wr_4byte(0xfe820808, 0x0490045a);
apb_wr_4byte(0xfe82080c, 0x01010908);
apb_wr_4byte(0xfe820810, 0x07e70644);
apb_wr_4byte(0xfe822020, 0x00001202);
apb_wr_4byte(0xfe822024, 0x2640e550);
apb_wr_4byte(0xfe822034, 0x00099f01);
apb_wr_4byte(0xfe822050, 0x00a06070);
apb_wr_4byte(0xfe822064, 0x005100bb);
apb_wr_4byte(0xfe822068, 0x003c0000);

apb_wr_4byte(0xfe8220dc, 0x00440024);//MR1 =0x44, MR2 =0x24
apb_wr_4byte(0xfe8220e0, 0x00f00008);//MR3 =0xf0, MR13=0x08
apb_wr_4byte(0xfe8220e8, 0x0064004d);//MR11=0x64, MR12=0x4d
apb_wr_4byte(0xfe8220ec, 0x0000004d);//MR22=0x00, MR14=0x4d

apb_wr_4byte(0xfe8220f4, 0x00002334);
apb_wr_4byte(0xfe822100, 0x171b2d1c);
apb_wr_4byte(0xfe822104, 0x00050528);
apb_wr_4byte(0xfe822108, 0x060e1315);
apb_wr_4byte(0xfe82210c, 0x0090a006);
apb_wr_4byte(0xfe822110, 0x0c04070c);
apb_wr_4byte(0xfe822114, 0x0f060a0a);
apb_wr_4byte(0xfe822118, 0x0801000a);
apb_wr_4byte(0xfe82211c, 0x00000602);
apb_wr_4byte(0xfe822120, 0x01011a01);
apb_wr_4byte(0xfe822124, 0x40000008);
apb_wr_4byte(0xfe822128, 0x00000d04);
apb_wr_4byte(0xfe82212c, 0x0101000c);
apb_wr_4byte(0xfe822130, 0x00020000);
apb_wr_4byte(0xfe822134, 0x0b100002);
apb_wr_4byte(0xfe822138, 0x000003eb);
apb_wr_4byte(0xfe82213c, 0x80000000);
apb_wr_4byte(0xfe822180, 0x529b0014);
apb_wr_4byte(0xfe822190, 0x04978208);
apb_wr_4byte(0xfe822194, 0x00090303);
apb_wr_4byte(0xfe8221b4, 0x00001708);
apb_wr_4byte(0xfe8221b8, 0x00000008);
apb_wr_4byte(0xfe822240, 0x050c0560);
apb_rd_4byte(0xfe820060, temp_reg);
apb_rd_4byte(0xfe820030, temp_reg);
apb_wr_4byte(0xfe820030, 0x000000a2);

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
