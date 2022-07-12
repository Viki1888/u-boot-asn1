/*
* Copyright (C) 2017-2020 Alibaba Group Holding Limited
*
* SPDX-License-Identifier: GPL-2.0+
*/
#include <command.h>
#include "../lib/sec_library/include/csi_sec_img_verify.h"
#include "../drivers/trng/basic_defs.h"

#define BYTE_BITS_LOW (BIT_0 | BIT_1 | BIT_2 | BIT_3)
#define BYTE_BITS_HIGH (BIT_4 | BIT_5 | BIT_6 | BIT_7)
#define BYTE_BITS_ALL (BYTE_BITS_LOW | BYTE_BITS_HIGH)

#define WJ_EFUSE_BASE               0xFFFF210000UL
#define PRINT_TITLE_WIDTH "30"

enum efuse_zone {
    EFUSE_ZONE_CVCON1 = 1,
    EFUSE_ZONE_CVCON2 = 2,
    EFUSE_ZONE_USRCON1 = 4,
    EFUSE_ZONE_USRCON2 = 8,
};

typedef struct {
    int enum_value;
    const char *value;
} efuse_enum_info;

typedef struct {
    const char *name;
    int offset;
    int cnt;
    uint32_t mask;
    int enum_count;
    bool mode_config;
    bool is_uint32;
    enum efuse_zone zone;
    efuse_enum_info enum_info[20];
} efuse_offset_info;


static const char *get_enum_str(efuse_enum_info * enum_infos, int enum_count,int enum_value) {
    int i;

    for(i = 0;i < enum_count;++i) {
        if(enum_infos[i].enum_value == enum_value) {
            return enum_infos[i].value;
        }
    }

    return NULL;
}

static void print_config_mode(uint32_t enum_value) {
    if(enum_value == 0) {
        printf("opened\r\n");
    } else if(enum_value & BYTE_BITS_LOW != 0) {
        printf("need auth\r\n");
    }else {
        printf("closed\r\n");
    }
}

static void efuse_print_bits_of_byte(const char *name,uint32_t addr, uint32_t mask,efuse_enum_info * enum_infos, int enum_count, bool is_mode_config) {
    int ret;
    uint8_t data;
    int bit_start = 0;
    uint32_t mask_temp = mask;
    uint32_t enum_value;
    const char *enum_str;

    if(!mask) {
        printf("mask could not be 0\r\n");
        return;
    }

    while(!(mask_temp & BIT_0)) {
        mask_temp >>= 1;
        ++bit_start;
    }

    printf("%-" PRINT_TITLE_WIDTH "s",name);
    ret = csi_efuse_read_raw(addr, &data, 1);
    if (ret) {
        printf("efuse read error\r\n");
        return;
    }
    
    enum_value = ((((uint32_t)data) & mask) >> bit_start);
    if(enum_count == 0) {
        if(is_mode_config) {
            print_config_mode(enum_value);
        } else {
            printf("0x%x\r\n",enum_value);
        }
    } else {
        enum_str = get_enum_str(enum_infos,enum_count,enum_value);
        if(!enum_str) {
            printf("illegal value\r\n");
        } else {
            printf("%s\r\n",enum_str);
        }
    }
}

static void efuse_print_bytes(const char *name,uint32_t addr, uint32_t cnt,bool is_uint32) {
    int ret;
    int i = 0;
    uint8_t data[1024] = {0};
    printf("%-" PRINT_TITLE_WIDTH "s", name);
    
    ret = csi_efuse_read_raw(addr, &data, cnt);
    if (ret) {
        printf("efuse read error\r\n");
        return;
    }
    printf("0x");
    if(is_uint32) {
        printf("%x",*(uint32_t*)data);
    } else {
        for(;i < cnt;++i) {
            printf("%02x",(uint32_t)(uint8_t)data[i]);
        }
    }
    
    printf("\r\n");

}

static uint32_t efuse_print_lifecycle(void) {
    int ret;
    int lc = 0;
    ret = wj_efuse_get_lc(WJ_EFUSE_BASE, &lc);
    printf("%-" PRINT_TITLE_WIDTH "s","DEV_LIFE_CYCLE:");
	if (ret) {
        printf("get failed\r\n");
		return 0;
	}

    if(lc == 0) {
        printf("LC_INIT\r\n");
    } else if(lc == 1) {
        printf("LC_DEV\r\n");
        return EFUSE_ZONE_CVCON1 | EFUSE_ZONE_CVCON2 | EFUSE_ZONE_USRCON1 | EFUSE_ZONE_USRCON2;
    } else if(lc == 2) {
        printf("LC_OEM\r\n");
        return EFUSE_ZONE_CVCON1 | EFUSE_ZONE_CVCON2 | EFUSE_ZONE_USRCON2;
    } else if(lc == 3) {
        printf("LC_PRO\r\n");
        return EFUSE_ZONE_CVCON1 | EFUSE_ZONE_CVCON2 | EFUSE_ZONE_USRCON2;
    } else if(lc == 4) {
        printf("LC_RMA\r\n");
        return EFUSE_ZONE_CVCON2;
    } else if(lc == 5) {
        printf("LC_RIP\r\n");
    } else if(lc == 6) {
        printf("LC_KILL_KEY1\r\n");
    } else if(lc == 7) {
        printf("LC_KILL_KEY0\r\n");
    } else {
        printf("LC_MAX\r\n");
    }

    return 0;
}


int do_secconfig(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
    int i;
    uint32_t zone_access;
    efuse_offset_info efuse_infos[] = {
        {"DEFUALT_VIORST_EN:",1,1,BYTE_BITS_LOW,0,false,false,EFUSE_ZONE_CVCON1},
        {"RCEN_CFG_DIS:",     1,1,BYTE_BITS_HIGH,0,false,false,EFUSE_ZONE_CVCON1},
        {"CV_DSP0_JTAG_MODE:",  6,1,BYTE_BITS_ALL,0,true,false,EFUSE_ZONE_CVCON1},
        {"CV_DSP1_JTAG_MODE:",7,1,BYTE_BITS_ALL,0,true,false,EFUSE_ZONE_CVCON1},
        {"CV_C910T_JTAG_MODE:",8,1,BYTE_BITS_ALL,0,true,false,EFUSE_ZONE_CVCON1},
        {"CV_C910R_JTAG_MODE:",9,1,BYTE_BITS_ALL,0,true,false,EFUSE_ZONE_CVCON1},
        {"CV_C906_JTAG_MODE:",10,1,BYTE_BITS_ALL,0,true,false,EFUSE_ZONE_CVCON1},
        {"CV_E902_JTAG_MODE:",11,1,BYTE_BITS_ALL,0,true,false,EFUSE_ZONE_CVCON1},
        {"CV_CHIP_DBG_MODE:", 12,1,BYTE_BITS_ALL,0,true,false,EFUSE_ZONE_CVCON1},
        {"CV_DFT_MODE:",      13,1,BYTE_BITS_ALL,0,true,false,EFUSE_ZONE_CVCON1},
        {"EIP120SI_SCAEN:",   14,1,BIT_0 | BIT_1,0,false,false,EFUSE_ZONE_CVCON1},
        {"EIP120SII_SCAEN:",  14,1,BIT_2 | BIT_3,0,false,false,EFUSE_ZONE_CVCON1},
        {"EIP150_SCAEN:",     14,1,BIT_4 | BIT_5,0,false,false,EFUSE_ZONE_CVCON1},
        {"CV_BROM_DBG_DIS:",  15,1,0,0,false,false,EFUSE_ZONE_CVCON1},
        {"SECURE_BOOT:",      16,1,BYTE_BITS_ALL,2,false,false,EFUSE_ZONE_CVCON2,{{0x5a,"secure"},{0x00,"unsecure"}}},
        {"BROM_PRINT_DIS:",   17,1,BYTE_BITS_LOW,2,false,false,EFUSE_ZONE_CVCON2,{{0xa,"disabled"},{0x00,"enabled"}}},
        {"CV_BROM_CCT_DIS:",  17,1,BYTE_BITS_HIGH,2,false,false,EFUSE_ZONE_CVCON2,{{0xa,"disabled"},{0x00,"enabled"}}},
        {"BROM_ICACHE_EN:",   18,1,BIT_0 | BIT_1,2,false,false,EFUSE_ZONE_CVCON2,{{0x2,"disabled"},{0x00,"enabled"}}},
        {"BROM_DCACHE_EN:",   18,1,BIT_2 | BIT_3,2,false,false,EFUSE_ZONE_CVCON2,{{0x2,"disabled"},{0x00,"enabled"}}},
        {"TRNG_BYPASS:",      19,1,BYTE_BITS_LOW,2,false,false,EFUSE_ZONE_CVCON2,{{0xa,"disabled"},{0x00,"enabled"}}},
        {"CV_USB_FASTBOOT_DIS:",19,1,BYTE_BITS_HIGH,2,false,false,EFUSE_ZONE_CVCON2,{{0xa,"disabled"},{0x00,"enabled"}}},
        {"DIGEST_SCHEME:",    20,1,BYTE_BITS_LOW,2,false,false,EFUSE_ZONE_CVCON2,{{0x4,"DIGEST_HASH_SHA256"},{0x7,"DIGEST_HASH_SM3"}}},
        {"SIGN_SCHEME:",      20,1,BYTE_BITS_HIGH,2,false,false,EFUSE_ZONE_CVCON2,{{0x2,"SIGNATURE_RSA_2048"},{0x5,"SIGNATURE_SM2"}}},
        {"IMAGE_BL1_ENC:",    21,1,BYTE_BITS_ALL,2,false,false,EFUSE_ZONE_CVCON2,{{0x5a,"encrypted"},{0x00,"unencrypted"}}},
        {"USR_DSP0_JTAG_MODE:",134,1,BYTE_BITS_ALL,0,true,false,EFUSE_ZONE_USRCON1},
        {"USR_DSP1_JTAG_MODE:",135,1,BYTE_BITS_ALL,0,true,false,EFUSE_ZONE_USRCON1},
        {"USR_C910T_JTAG_MODE:",136,1,BYTE_BITS_ALL,0,true,false,EFUSE_ZONE_USRCON1},
        {"USR_C910R_JTAG_MODE:",137,1,BYTE_BITS_ALL,0,true,false,EFUSE_ZONE_USRCON1},
        {"USR_C906_JTAG_MODE:",138,1,BYTE_BITS_ALL,0,true,false,EFUSE_ZONE_USRCON1},
        {"USR_E902_JTAG_MODE:",139,1,BYTE_BITS_ALL,0,true,false,EFUSE_ZONE_USRCON1},
        {"USR_CHIP_DBG_MODE:",140,1,BYTE_BITS_ALL,0,true,false,EFUSE_ZONE_USRCON1},
        {"USR_DFT_MODE:",     141,1,BYTE_BITS_ALL,0,true,false,EFUSE_ZONE_USRCON1},
        {"BOOT_OFFSET:",      144,4,0,0,false,true,EFUSE_ZONE_USRCON2},
        {"FIX:",              148,1,0,0,false,false,EFUSE_ZONE_USRCON2},
        {"BOOT_OFFSET_BAK:",  149,4,0,0,false,true,EFUSE_ZONE_USRCON2},
        {"BOOT_INDEX_BAK:",   153,1,0,0,false,false,EFUSE_ZONE_USRCON2},
        {"USR_BROM_CCT_DIS:", 154,1,BYTE_BITS_LOW,2,false,false,EFUSE_ZONE_USRCON2,{{0xa,"disabled"},{0x00,"enabled"}}},
        {"USR_USB_FASTBOOT_DIS:",154,1,BYTE_BITS_HIGH,2,false,false,EFUSE_ZONE_USRCON2,{{0xa,"disabled"},{0x00,"enabled"}}},
        {"IMAGE_BL2_ENC:",    155,1,BYTE_BITS_ALL,2,false,false,EFUSE_ZONE_USRCON2,{{0x5a,"encrypted"},{0x00,"unencrypted"}}},
        {"IMAGE_BL3_ENC:",    156,1,BYTE_BITS_ALL,2,false,false,EFUSE_ZONE_USRCON2,{{0x5a,"encrypted"},{0x00,"unencrypted"}}},
        {"IMAGE_BL4_ENC:",    157,1,BYTE_BITS_ALL,2,false,false,EFUSE_ZONE_USRCON2,{{0x5a,"encrypted"},{0x00,"unencrypted"}}},
    };

    zone_access = efuse_print_lifecycle();
 
    for(i = 0;i < sizeof(efuse_infos) / sizeof(efuse_infos[0]);++i) {
        if((efuse_infos[i].zone & zone_access) != efuse_infos[i].zone) {
            continue;
        }

        if(efuse_infos[i].mask != 0) {
            efuse_print_bits_of_byte(efuse_infos[i].name,efuse_infos[i].offset,efuse_infos[i].mask,efuse_infos[i].enum_info,efuse_infos[i].enum_count,efuse_infos[i].mode_config);
        } else {
            efuse_print_bytes(efuse_infos[i].name,efuse_infos[i].offset,efuse_infos[i].cnt,efuse_infos[i].is_uint32);
        }
    } 

    return 0;
}


U_BOOT_CMD(secconfig, 4, 1, do_secconfig,
	   "list efuse configs of security",
	   "");
