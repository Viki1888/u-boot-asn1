/*
 * (C) Copyright 2018, Linaro Limited
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <command.h>
#include <env.h>
#include "sec_library.h"

#define ENV_SECIMG_LOAD     "sec_m_load"
#define VAL_SECIMG_LOAD   "ext4load mmc 0:7 0 trust_firmware.bin; ext4load mmc 0:7 $tee_addr tee.bin"

/* In order to use common bootloader for both secure boot and non-secure boot,
   we only know the boot type through reading the sec_boot field in efuse. Due to 
   the efuse is only accessed in lifecycle(DEV/OEM/PRO/RMP), we ensure it must be 
   non-secure boot in lifecycle(INIT) */
bool get_system_boot_type(void)
{
	bool btype = false; /* false: non-secure boot | true: secure boot */
	int lc = 0;
	sboot_st_t sb_flag = SECURE_BOOT_DIS;
	int ret = 0;

	ret = csi_efuse_get_lc(&lc);
	/* 0: LC_INIT, 1: LC_DEV, 2: LC_OEM, 3: LC_PRO */
	if ((ret == 0) && (lc != 0)) {
		csi_efuse_api_init();

		/* Check platform secure boot enable ? */
		ret = csi_efuse_get_secure_boot_st(&sb_flag);
		if ((ret == 0) && (sb_flag == SECURE_BOOT_EN))
			btype = true;

		csi_efuse_api_uninit();
	}

	return btype;
}

static int do_secimg_load(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    bool sb_enable = false;
    const char *secimgs_load_str = VAL_SECIMG_LOAD;
    int ret = -1;

    sb_enable = get_system_boot_type();
    if (sb_enable) {
        /* By default, the value for ENV-SEC-M-LOAD is always to load opensbi image.
         * if secure boot is enable, we force to change the value to load tee image.
         * but Never to save it in volatile-RAM
         */
        ret = env_set(ENV_SECIMG_LOAD, secimgs_load_str);
        if (ret != 0) {
            printf("Rewrite ENV (%s) fails\n", ENV_SECIMG_LOAD);
            return CMD_RET_FAILURE;
        }
    }
    
	return CMD_RET_SUCCESS;
}

U_BOOT_CMD(
	secimg_load, 1, 1,	do_secimg_load,
	"Runtime-load secure image if secure system is enable",
	NULL
);
