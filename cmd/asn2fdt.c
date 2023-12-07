// SPDX-License-Identifier: GPL-2.0+
/*
 * Command for accessing SPI flash.
 *
 * Copyright (C) 2008 Atmel Corporation
 */
#include <common.h>
#include <string.h>
#include <fdt_support.h>
#include <linux/libfdt.h>

typedef struct {
    int 	version;
    int 	has_c;
    int 	has_d;
    int 	has_f;
    int 	has_sv39;
    int 	has_sv48;
    int 	has_sv57;
    int 	has_svpbmt;
    int 	has_zihpm;
    int 	has_zicsr;
    char 	*ext_tag;
    char 	*ext_payload;
    char 	*rvv_vlen;
} asn_data_t;
#define ASN_SUBNODE_START	10	/* rvv, ext start pos*/
#define SCRATCHPAD	1024		/* bytes of scratchpad memory */

static const char *const asn_member_table[] = {
    "version",
    "c",
    "d",
    "f",
    "sv39",
    "sv48",
    "sv57",
    "svpbmt",
    "zihpm",
    "zicsr",
    "ext",
    "tag",
    "payload",
    "rvv",
    "vlen",
};

static asn_data_t asn_data = {
    1,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    "v0.7Vector",
    "\x10\x20\x30\x40\x60",
    "0001",
};

static int covert_bytes(struct fdt_header *blob, char *pathp, const char *prop,
                        char *data, int data_size)
{
    int  nodeoffset;	/* node offset from libfdt */
    int  ret;		/* return value */

    if (!data || !blob) {
        printf("null pointer error\n");
    }

    nodeoffset = fdt_path_offset(blob, pathp);
    if (nodeoffset < 0) {
        /*
        * Not found or something else bad happened.
        */
        printf("libfdt fdt_path_offset() returned %s\n",
               fdt_strerror(nodeoffset));
        return 1;
    }

    ret = fdt_setprop(blob, nodeoffset, prop, data, data_size);
    if (ret < 0) {
        printf("libfdt fdt_setprop(): %s\n", fdt_strerror(ret));
        return 1;
    }

    return 0;
}

static int convertFDTNode(struct fdt_header *blob, asn_data_t *data,
                          const char *path)
{
    int *p = NULL;
    int i = 0, j = 0;
    char cmd[64] = {0};
    int ret = 0;

    if (!blob || !data || !path) {
        printf("null pointer error\n");
        return CMD_RET_FAILURE;
    }

    p = (int *)data;

    /* add integer properties */
    for (; i < ASN_SUBNODE_START; i++, j++) {
        snprintf(cmd, sizeof(cmd), "fdt set %s %s <%x>", path, asn_member_table[j],
                 p[i]);
        if (run_command(cmd, 0)) {
            printf("adding %dth property failed\n", i);
            return CMD_RET_FAILURE;
        }
    }

    /* add ext subnode */
    snprintf(cmd, sizeof(cmd), "fdt mknode %s %s", path, asn_member_table[j]);
    ret = run_command(cmd, 0);
    if (ret != 0 && ret != -FDT_ERR_EXISTS) {
        printf("fdt mknode failed\n");
        return CMD_RET_FAILURE;
    }

    snprintf(cmd, sizeof(cmd), "%s/%s", path, asn_member_table[j]);
    if (covert_bytes(blob, cmd, asn_member_table[j + 1], data->ext_tag,
                     strlen(data->ext_tag) + 1)) {
        printf("fdt set string error\n");
        return CMD_RET_FAILURE;
    }

    snprintf(cmd, sizeof(cmd), "%s/%s", path, asn_member_table[j]);
    if (covert_bytes(blob, cmd, asn_member_table[j + 2], data->ext_payload,
                     strlen(data->ext_payload))) {
        printf("fdt set string error\n");
        return CMD_RET_FAILURE;
    }

    /* add rvv subnode*/
    j += 3;
    snprintf(cmd, sizeof(cmd), "fdt mknode %s %s", path, asn_member_table[j]);
    ret = run_command(cmd, 0);
    if (ret != 0 && ret != -FDT_ERR_EXISTS) {
        printf("fdt mknode failed\n");
        return CMD_RET_FAILURE;
    }

    snprintf(cmd, sizeof(cmd), "%s/%s", path, asn_member_table[j]);
    if (covert_bytes(blob, cmd, asn_member_table[j + 1], data->rvv_vlen,
                     strlen(data->rvv_vlen) + 1)) {
        printf("fdt set string error\n");
        return CMD_RET_FAILURE;
    }

    return 0;
}

/*
 * Add asn_data to fdt
 */
static int asn2fdtdemo(asn_data_t data)
{
    unsigned int extradata = 128;
    char *dtb_file = env_get("fdt_file");
    void *dtb_addr = (void *)(uintptr_t)env_get_hex("dtb_addr", 0);
    const char *path = "/cpus/cpu@0";
    struct fdt_header *blob;
    const char *loadfdt =
        "ext4load mmc ${mmcdev}:${mmcbootpart} $dtb_addr ${fdt_file};";
    char cmd[64] = {0};
    int len = 0;
    char *bootcmd_load = NULL;
    char *pos = NULL;
    /* 1. Load fdt file */
    snprintf(cmd, sizeof(cmd), "ext4load mmc 0:2 %p %s", dtb_addr, dtb_file);

    if (run_command(cmd, 0)) {
        printf("load dtb file failed\n");
        return CMD_RET_FAILURE;
    }

    /* 2. set fdt working addr */
    snprintf(cmd, sizeof(cmd), "fdt addr %p", dtb_addr);
    if (run_command(cmd, 0)) {
        printf("setting fdt addr failed\n");
        return CMD_RET_FAILURE;
    }

    blob = (struct fdt_header *)dtb_addr;

    /* 3. resize fdt file */
    snprintf(cmd, sizeof(cmd), "fdt resize %x", extradata);
    if (run_command(cmd, 0)) {
        printf("resizing fdt addr failed\n");
        return CMD_RET_FAILURE;
    }

    /* 4. add properties */
    if (convertFDTNode(blob, &data, path)) {
        return CMD_RET_FAILURE;
    }

    /* 5. change bootcmd_load */
    bootcmd_load = env_get("bootcmd_load");
    if (!bootcmd_load) {
        printf("bootcmd_load not found\n");
        return CMD_RET_USAGE;
    }

    len = strlen(loadfdt);
    pos = strstr(bootcmd_load, loadfdt);

    if (pos != NULL) {
        memmove(pos, pos + len, strlen(pos + len) + 1);
    }

    env_set("bootcmd_load", bootcmd_load);
    return 0;
}

static int do_asn2fdt(cmd_tbl_t *cmdtp, int flag, int argc,
                      char *const argv[])
{
    if (asn2fdtdemo(asn_data)) {
        return CMD_RET_USAGE;
    }

    return 0;
}

U_BOOT_CMD(
    asn2fdt, 	2,	1,	do_asn2fdt,
    "Convert ASN1 data to FDT node",
    ""
);
