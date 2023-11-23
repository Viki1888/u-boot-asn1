// SPDX-License-Identifier: GPL-2.0+
/*
 * Command for accessing SPI flash.
 *
 * Copyright (C) 2008 Atmel Corporation
 */

#include <common.h>
#include <div64.h>
#include <dm.h>
#include <malloc.h>
#include <mapmem.h>
#include <spi.h>
#include <spi_flash.h>
#include <jffs2/jffs2.h>
#include <linux/mtd/mtd.h>

#include <asm/io.h>
#include <dm/device-internal.h>

#include "legacy-mtd-utils.h"
#include "DiscoveryData.h"

extern int discoveryData_test_demo(int argc, char * const argv[]);
static int do_asn1c(cmd_tbl_t *cmdtp, int flag, int argc,
			char * const argv[])
{
	const char *cmd;
	int ret;
	printf("%s %d\n", __func__, __LINE__);
	/* need at least two arguments */
	cmd = argv[1];
	--argc;
	++argv;
	printf("%s %d\n", __func__, __LINE__);
	if (strcmp(cmd, "test") == 0) {
		discoveryData_test_demo(argc, argv);
		goto done;
	}

	return 0;

done:
	if (ret != -1)
		return ret;

usage:
	return CMD_RET_USAGE;
}

#ifdef CONFIG_CMD_ASN1_TEST
#define ASN1_TEST_HELP "\nsf test offset len		" \
		"- run a very basic destructive test"
#else
#define ASN1_TEST_HELP
#endif

U_BOOT_CMD(
	asn1, 	2,	1,	do_asn1c,
	"ASN1C sub-system",
	"test - ber encode and decode test\n"
	ASN1_TEST_HELP
);
