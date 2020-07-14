#define apb_wr_4byte(addr,wdata) *(volatile unsigned int*)(addr) = wdata
#define apb_rd_4byte(addr,rdata) rdata = *(volatile unsigned int*)(addr)


#include <common.h>
#include <console.h>
//#include <asm/mini_printf.h>
extern void ddr_fw_run(void);

void init_ddr(void)
{
unsigned int temp_reg;
apb_wr_4byte(0x3fe820304, 0x00000001);
apb_wr_4byte(0x3fe820030, 0x00000001);
apb_rd_4byte(0x3fe820004, temp_reg);
apb_wr_4byte(0x3fe820000, 0x81080020); //1-rank
apb_wr_4byte(0x3fe820010, 0x4000b030);
apb_wr_4byte(0x3fe820014, 0x0002a3b1);
apb_wr_4byte(0x3fe82001c, 0xbe13ada3);

//DERATEEN(0x20)
//bit[12], 1: dont use TUF
//bit[10:8], tRC derate value: 3.75ns, 533MHz: 1(2 core_clk)
//bit[1], derate value: 1: core_clk < 1.875
//bit[0], enable derate: 1
//apb_wr_4byte(0x3fe820020, 0x00000100);
apb_wr_4byte(0x3fe820020, 0x00001103);

//derate MR4 readback interval: x dfi_clk(533MHz)
apb_wr_4byte(0x3fe820024, 0xf75a1151);

apb_wr_4byte(0x3fe82002c, 0x00000000);
apb_wr_4byte(0x3fe820030, 0x000000aa);
apb_wr_4byte(0x3fe820034, 0x000a3502);
apb_wr_4byte(0x3fe820038, 0x00f70001);

//RFSCTRL0(0x50)
// bit[23:20]: refresh_margin = tRFC/32, very import, suggest not change
// bit[16:12]: refresh_to_x1_x32,speculative refresh
// bit[9:4]:   refresh_burst = 8*8
// bit[2]:     1: per_bank_refresh
//apb_wr_4byte(0x3fe820050, 0x00a1f070);
apb_wr_4byte(0x3fe820050, 0x0021f3f4);

apb_wr_4byte(0x3fe820054, 0x00060015);

//RFSHCTRL0(0x60)
apb_wr_4byte(0x3fe820060, 0x00000000);

//RFSHTMG(0x64)
//bit[31]: 0:x32, 1:x1
//bit[26:16] : tRFC = ceil(tREFIpb/tCK/2) = (488/0.947)/2=257=0x101 @2122/2133MT
//bit[15]    : 0, dont use for LP4
//bit[9:0]   : tRFC(min), 280/0.947/2=147=0x93, all_bank @2122/2133MT
//           : tRFC(min), 140/0.947/2=73 =0x49, per_bank @2122/2133MT
//apb_wr_4byte(0x3fe820064, 0x00418096);
apb_wr_4byte(0x3fe820064, 0x81010049);

apb_wr_4byte(0x3fe820068, 0x00300000);
apb_wr_4byte(0x3fe8200c0, 0x00000000);
apb_wr_4byte(0x3fe8200c4, 0x00000000);
apb_wr_4byte(0x3fe8200d0, 0xc0030003);
apb_wr_4byte(0x3fe8200d4, 0x00010008);
apb_wr_4byte(0x3fe8200d8, 0x00009105);

apb_wr_4byte(0x3fe8200dc, 0x0034001b);//MR1 =0x34, MR2 =0x1b
apb_wr_4byte(0x3fe8200e0, 0x00f10008);//MR3 =0xf1, MR13=0x08
apb_wr_4byte(0x3fe8200e8, 0x0000004d);//MR11=0x00, MR12=0x4d
apb_wr_4byte(0x3fe8200ec, 0x0000004d);//MR22=0x00, MR14=0x4d

apb_wr_4byte(0x3fe8200e4, 0x00030007);
apb_wr_4byte(0x3fe8200f0, 0x00000000);
apb_wr_4byte(0x3fe8200f4, 0x0000f33d);
apb_wr_4byte(0x3fe820100, 0x14162417);
apb_wr_4byte(0x3fe820104, 0x00040420);
apb_wr_4byte(0x3fe820108, 0x050b1012);
apb_wr_4byte(0x3fe82010c, 0x00708006);
apb_wr_4byte(0x3fe820110, 0x0a04060a);
apb_wr_4byte(0x3fe820114, 0x05080808);
apb_wr_4byte(0x3fe820118, 0x0b09000e);
apb_wr_4byte(0x3fe82011c, 0x00000306);
apb_wr_4byte(0x3fe820120, 0x01011a01);
apb_wr_4byte(0x3fe820124, 0x00000011);
apb_wr_4byte(0x3fe820128, 0x00040909);
apb_wr_4byte(0x3fe82012c, 0x2701000c);
apb_wr_4byte(0x3fe820130, 0x00020000);
apb_wr_4byte(0x3fe820134, 0x0a100002);
apb_wr_4byte(0x3fe820138, 0x000001e7);
apb_wr_4byte(0x3fe82013c, 0x80000000);
apb_wr_4byte(0x3fe820180, 0x02160010);
apb_wr_4byte(0x3fe820184, 0x01b00070);
apb_wr_4byte(0x3fe820188, 0x00000000);
apb_wr_4byte(0x3fe820190, 0x03918206);
apb_wr_4byte(0x3fe820194, 0x00090202);
apb_wr_4byte(0x3fe820198, 0x07712000);
apb_wr_4byte(0x3fe82019c, 0x00000030);
apb_wr_4byte(0x3fe8201a0, 0x00400018);
apb_wr_4byte(0x3fe8201a4, 0x004b00fb);
apb_wr_4byte(0x3fe8201a8, 0x80000000);
apb_wr_4byte(0x3fe8201b0, 0x00000051);
apb_wr_4byte(0x3fe8201b4, 0x00001106);
apb_wr_4byte(0x3fe8201b8, 0x0000001e);
apb_wr_4byte(0x3fe8201c0, 0x00000007);
apb_wr_4byte(0x3fe8201c4, 0x00000000);


//by gaohf at 2020/05/29, RBC (3BA,16R,10C,32bit)
//16Gb x 2pcs = 4GB
//Mapping: {ROW, BANK, COL},R16 DO NOT USED
apb_wr_4byte(0x3fe820200, 0x001f1f1f);
apb_wr_4byte(0x3fe820204, 0x00080808);
apb_wr_4byte(0x3fe820208, 0x00000000);
apb_wr_4byte(0x3fe82020c, 0x00000000);
apb_wr_4byte(0x3fe820210, 0x00001f1f);
apb_wr_4byte(0x3fe820214, 0x07070707);
apb_wr_4byte(0x3fe820218, 0x07070707);
apb_wr_4byte(0x3fe82021c, 0x00000f0f);
apb_wr_4byte(0x3fe820220, 0x00003f3f);
apb_wr_4byte(0x3fe820224, 0x00000000);
apb_wr_4byte(0x3fe820228, 0x00000000);
apb_wr_4byte(0x3fe82022c, 0x001f1f00);

apb_wr_4byte(0x3fe820240, 0x050c0560);
apb_wr_4byte(0x3fe820244, 0x00000000);

apb_wr_4byte(0x3fe820250, 0x00c21f00);
apb_wr_4byte(0x3fe820254, 0x00000000);
apb_wr_4byte(0x3fe82025c, 0x0f000001);
apb_wr_4byte(0x3fe820264, 0x0f00007f);
apb_wr_4byte(0x3fe82026c, 0x0f00007f);
apb_wr_4byte(0x3fe820300, 0x00000000);
apb_wr_4byte(0x3fe820304, 0x00000000);
apb_wr_4byte(0x3fe82030c, 0x00000000);
apb_wr_4byte(0x3fe820320, 0x00000001);
apb_wr_4byte(0x3fe820328, 0x00000000);
apb_wr_4byte(0x3fe82036c, 0x00110001);
apb_wr_4byte(0x3fe820374, 0x0000014f);
apb_wr_4byte(0x3fe82037c, 0x00000000);
apb_wr_4byte(0x3fe820380, 0x00000000);
apb_wr_4byte(0x3fe820384, 0x00000000);
apb_wr_4byte(0x3fe820490, 0x00000001);
apb_wr_4byte(0x3fe820540, 0x00000001);
apb_wr_4byte(0x3fe8205f0, 0x00000001);
apb_wr_4byte(0x3fe8206a0, 0x00000001);
apb_wr_4byte(0x3fe820750, 0x00000001);
apb_wr_4byte(0x3fe820800, 0x00000001);


//3200mt
apb_wr_4byte(0x3fe820400, 0x00000000);
apb_wr_4byte(0x3fe820404, 0x0000000f);
apb_wr_4byte(0x3fe8204b4, 0x0000000f);
apb_wr_4byte(0x3fe820564, 0x0000000f);
apb_wr_4byte(0x3fe820614, 0x0000000f);
apb_wr_4byte(0x3fe8206c4, 0x0000000f);
apb_wr_4byte(0x3fe820774, 0x0000000f);
apb_wr_4byte(0x3fe820404, 0x0000100f);
apb_wr_4byte(0x3fe8204b4, 0x0000100f);
apb_wr_4byte(0x3fe820564, 0x0000100f);
apb_wr_4byte(0x3fe820614, 0x0000100f);
apb_wr_4byte(0x3fe8206c4, 0x0000100f);
apb_wr_4byte(0x3fe820774, 0x0000100f);
apb_wr_4byte(0x3fe820404, 0x0000100f);
apb_wr_4byte(0x3fe8204b4, 0x0000100f);
apb_wr_4byte(0x3fe820564, 0x0000100f);
apb_wr_4byte(0x3fe820614, 0x0000100f);
apb_wr_4byte(0x3fe8206c4, 0x0000100f);
apb_wr_4byte(0x3fe820774, 0x0000100f);
apb_wr_4byte(0x3fe820404, 0x0000100f);
apb_wr_4byte(0x3fe8204b4, 0x0000100f);
apb_wr_4byte(0x3fe820564, 0x0000100f);
apb_wr_4byte(0x3fe820614, 0x0000100f);
apb_wr_4byte(0x3fe8206c4, 0x0000100f);
apb_wr_4byte(0x3fe820774, 0x0000100f);
apb_wr_4byte(0x3fe820408, 0x0000400f);
apb_wr_4byte(0x3fe8204b8, 0x0000400f);
apb_wr_4byte(0x3fe820568, 0x0000400f);
apb_wr_4byte(0x3fe820618, 0x0000400f);
apb_wr_4byte(0x3fe8206c8, 0x0000400f);
apb_wr_4byte(0x3fe820778, 0x0000400f);
apb_wr_4byte(0x3fe820408, 0x0000500f);
apb_wr_4byte(0x3fe8204b8, 0x0000500f);
apb_wr_4byte(0x3fe820568, 0x0000500f);
apb_wr_4byte(0x3fe820618, 0x0000500f);
apb_wr_4byte(0x3fe8206c8, 0x0000500f);
apb_wr_4byte(0x3fe820778, 0x0000500f);
apb_wr_4byte(0x3fe820408, 0x0000500f);
apb_wr_4byte(0x3fe8204b8, 0x0000500f);
apb_wr_4byte(0x3fe820568, 0x0000500f);
apb_wr_4byte(0x3fe820618, 0x0000500f);
apb_wr_4byte(0x3fe8206c8, 0x0000500f);
apb_wr_4byte(0x3fe820778, 0x0000500f);
apb_wr_4byte(0x3fe820408, 0x0000100f);
apb_wr_4byte(0x3fe8204b8, 0x0000100f);
apb_wr_4byte(0x3fe820568, 0x0000100f);
apb_wr_4byte(0x3fe820618, 0x0000100f);
apb_wr_4byte(0x3fe8206c8, 0x0000100f);
apb_wr_4byte(0x3fe820778, 0x0000100f);

apb_wr_4byte(0x3fe820030, 0x000000a2);

//Release Resetn
apb_wr_4byte(0x3fff78040, 0x3ff);
apb_wr_4byte(0x3fff78040, 0x3ff);
apb_wr_4byte(0x3fff78040, 0x3ff);
apb_wr_4byte(0x3fff78040, 0x3ff);
apb_wr_4byte(0x3fff78040, 0x3ff);


//***********************
// VERY IMPORT
//***********************
apb_wr_4byte(0x3fe820060, 0x00000001); //disable dis_auto_refresh
//disable selref_en, powerdown_en, en_dfi_dram_clk_disable
apb_wr_4byte(0x3fe820030, 0x00000020); //selfref_sw=1
apb_wr_4byte(0x3fe820320, 0x0); //SWCTL.sw_done
apb_wr_4byte(0x3fe8201b0, 0x00000050);//bit[12:8]:0~3:Goto PHY P0~3
                                     //bit[6]:dis_dyn_adr_tri
                                     //bit[5]:dfi_init_start
                                     //bit[4]:ctl_idle_en
                                     //bit[0]:dfi_init_complete_en, mask during phy init
apb_wr_4byte(0x3fe820320, 0x1); //SWCTL.sw_done
apb_wr_4byte(0x3fe820304, 0x00000000); //disable CAM

apb_rd_4byte(0x3fe820000, temp_reg); //delay
apb_rd_4byte(0x3fe820000, temp_reg); //delay
apb_rd_4byte(0x3fe820000, temp_reg); //delay
apb_rd_4byte(0x3fe820000, temp_reg); //delay


ddr_fw_run();


//***********************************
// Switch PHY to MissionMode
//***********************************
apb_wr_4byte(0x3fe820320, 0x0);//sw_done, prepare reg write
apb_wr_4byte(0x3fe8201b0, 0x00000070);//bit[12:8]:0~3:Goto PHY P0~3
                                     //bit[6]:dis_dyn_adr_tri
                                     //bit[5]:dfi_init_start
                                     //bit[4]:ctl_idle_en
                                     //bit[0]:dfi_init_complete_en
apb_wr_4byte(0x3fe820320, 0x1);//sw_done,reg write in

//read SWSTAT, wait sw_done_ack = 1
apb_rd_4byte(0x3fe820324,temp_reg);
while(temp_reg == 0)
{
   apb_rd_4byte(0x3fe820324,temp_reg);
}

//Poll DFISTAT, wait dfi_init_complete
apb_rd_4byte(0x3fe8201bc,temp_reg);
while(temp_reg == 0)
{
   apb_rd_4byte(0x3fe8201bc,temp_reg);
}



//***********************************
// Switch CTRL to MissionMode
//***********************************
apb_wr_4byte(0x3fe820320, 0x0);//sw_done, prepare reg write
apb_wr_4byte(0x3fe8201b0, 0x00000050);//bit[12:8]:0~3:Goto PHY P0~3
                                     //bit[6]:dis_dyn_adr_tri
                                     //bit[5]:dfi_init_start
                                     //bit[4]:ctl_idle_en
                                     //bit[0]:dfi_init_complete_en
apb_wr_4byte(0x3fe8201b0, 0x00000051);//bit[12:8]:0~3:Goto PHY P0~3
apb_wr_4byte(0x3fe820030, 0x00000000);//bit[5]:soft exit from selfref
apb_wr_4byte(0x3fe820320, 0x1);//sw_done,reg write in

//read SWSTAT, wait sw_done_ack = 1
apb_rd_4byte(0x3fe820324,temp_reg);
while(temp_reg == 0)
{
   apb_rd_4byte(0x3fe820324,temp_reg);
}

//STAT, operation_mode
//      3'h0 : Init
//      3'h1 : Normal
//      3'h2 : Power-down
//      3'h3 : Self-refresh/Self-refresh power-down
apb_rd_4byte(0x3fe820004,temp_reg);
while((temp_reg&0x7) != 1)
{
   apb_rd_4byte(0x3fe820004,temp_reg);
}



//***********************************
// Enable AXI ports
//***********************************
apb_wr_4byte(0x3fe820060, 0x00000000);//enable auto_refresh
apb_wr_4byte(0x3fe820304, 0x00000000);//enable CAM
//apb_wr_4byte(0x3fe820030, 0x00000000);
apb_wr_4byte(0x3fe820030, 0x0000000a); //auto-dis-clk/pwr_down
apb_wr_4byte(0x3fe820490, 0x00000001); //port0,cpu
apb_wr_4byte(0x3fe820540, 0x00000001); //port1,sce
apb_wr_4byte(0x3fe8205f0, 0x00000001); //port2,npu
apb_wr_4byte(0x3fe8206a0, 0x00000001); //port3,npu
apb_wr_4byte(0x3fe820750, 0x00000001); //port4,video
apb_wr_4byte(0x3fe820800, 0x00000001); //port5,peri

}
