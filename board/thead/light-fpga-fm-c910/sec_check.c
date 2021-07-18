/*
* Copyright (C) 2017-2021 Alibaba Group Holding Limited
*
* SPDX-License-Identifier: GPL-2.0+
*/

#include <common.h>
#include <asm/arch-thead/boot_mode.h>
#include "../../../lib/sec_library/include/csi_sec_img_verify.h"

extern int csi_efuse_api_int(void);
extern uint32_t rambus_crypto_init(void);

int image_have_head(unsigned long img_src_addr)
{
    uint8_t *buffer = (uint8_t *)img_src_addr;

    if (memcmp(header_magic, &buffer[4], 4) == 0) {
        return 1;
    }

    return 0;
}

int csi_sec_init(void)
{
	int ret;
	char *version;

	/* Initialize eFuse module */
	ret = csi_efuse_api_int();
	if (ret) {
		printf("efuse init faild[%d]\n", ret);
		 goto exit;
	}

	ret = rambus_crypto_init();
	if (ret) {
		printf("rambus_crypto_init faild[%d]\n", ret);
		goto exit;
	}

	csi_sec_get_lib_version(&version);
	csi_sec_set_boot_stage(0);
	printf("sec version: %s \n", version);

exit:
	return ret;
}

int csi_sec_image_verify(img_type_t type, long addr)
{
	int ret;
	int sbi_size;

	if (image_have_head(addr) == 0)
		return 0;

	if (type == T_SBI) {
		img_header_t *phead = (img_header_t *)addr;
		sbi_size = phead->image_size;
	}

	ret = csi_sec_custom_image_verify(addr, SRAM_BASE_ADDR);
	if (ret)
		goto exit;

	if (type == T_SBI)
		memmove((void *)0x0, (const void *)(addr + HEADER_SIZE), sbi_size);

	return ret;
exit:
	printf("image verfy faild[%d], hand here ...\n", ret);
	asm volatile("ebreak\n");
	while (1);
	return ret;
}
