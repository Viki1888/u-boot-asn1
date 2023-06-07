
/*
 * (C) Copyright 2018, Linaro Limited
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <avb_verify.h>
#include <command.h>
#include <env.h>
#include <image.h>
#include <malloc.h>
#include <mmc.h>
#include <android_image.h>
#include <android_bootloader_message.h>
#include <xbc.h>

#define ENV_KERNEL_ADDR  "kernel_addr"
#define ENV_RAMDISK_ADDR "ramdisk_addr"
#define ENV_DTB_ADDR     "dtb_addr"
#define DEFAULT_KERNEL_ADDR  0x00200800
#define DEFAULT_RAMDISK_ADDR 0x02000000
#define DEFAULT_DTB_ADDR     0x01f00000
#define ENV_RAMDISK_SIZE "ramdisk_size"
#define MISC_PARTITION "misc"
#define RECOVERY_PARTITION "recovery"
#ifdef CONFIG_ANDROID_AB
#define BOOT_PARTITION "boot_a"
#define VENDOR_BOOT_PARTITION "vendor_boot_a"
#else
#define BOOT_PARTITION "boot"
#define VENDOR_BOOT_PARTITION "vendor_boot"
#endif

/*
 * BOOT IMAGE HEADER V3/V4 PAGESIZE
 * Source code:system/tools/mkbootimg/unpack_bootimg.py
 */
#define BOOT_IMAGE_HEADER_V3_PAGESIZE 4096

static struct AvbOps *avb_ops;
static struct bootloader_message* s_bcb = NULL;

/*
 *format 4 chars/bytes to a int number
 */
static int byteToInt(uint8_t* data,int offset)
{
	return data[offset+0] + (data[offset+1] << 8) + (data[offset+2] << 16) + (data[offset+3] << 24);
}

static int get_number_of_pages(int image_size, int page_size)
{
    return (image_size + page_size - 1) / page_size;
}

/**
 * header_version >=3,get dtb data from vendor_boot.img ,else boot.img.
 *
 * header_version = 4,get bootconfig data from vendor_boot.img ,
 * and append bootconfig to the end of ramdisk(initrd)
 * doc:https://www.kernel.org/doc/html/next/translations/zh_CN/admin-guide/bootconfig.html#initrd
 */
static void prepare_data_from_vendor_boot(struct andr_img_hdr *hdr,int dtb_start,uint8_t** buf_bootconfig,int* vendor_bootconfig_size)
{
	if (!hdr)
	{
		printf("invalid hdr\n");
		return;
	}
	int ret;
	disk_partition_t part_info;
	struct blk_desc *dev_desc  = blk_get_dev("mmc", CONFIG_FASTBOOT_FLASH_MMC_DEV);
	printf("blk_get_dev %s\n", VENDOR_BOOT_PARTITION);
	if (!dev_desc || dev_desc->type == DEV_TYPE_UNKNOWN) {
			printf("invalid mmc device\n");
			return;
	}
	/* Get boot partition info */
	ret = part_get_info_by_name(dev_desc, VENDOR_BOOT_PARTITION, &part_info);
	if (ret < 0) {
			printf("cannot find %s partition\n", VENDOR_BOOT_PARTITION);
			return;
	}

	uint8_t* vendor_boot_data = malloc(part_info.size * part_info.blksz);
	ret = blk_dread(dev_desc, part_info.start, part_info.size, vendor_boot_data);
	// vendor_boot.img
	//* +------------------------+
	//* | vendor boot header     | o pages
	//* +------------------------+
	//* | vendor ramdisk section | p pages
	//* +------------------------+
	//* | dtb                    | q pages
	//* +------------------------+
	//* | vendor ramdisk table   | r pages
	//* +------------------------+
	//* | bootconfig             | s pages
	//* +------------------------+
	//* o = (2124 + page_size - 1) / page_size
	//* p = (vendor_ramdisk_size + page_size - 1) / page_size
	//* q = (dtb_size + page_size - 1) / page_size
	//* r = (vendor_ramdisk_table_size + page_size - 1) / page_size
	//* s = (vendor_bootconfig_size + page_size - 1) / page_size

	// see system/tools/mkbootimg/unpack_bootimg.py
	// info.boot_magic = unpack('8s', args.boot_img.read(8))[0].decode()
	// info.header_version = unpack('I', args.boot_img.read(4))[0]
	// info.page_size = unpack('I', args.boot_img.read(4))[0]
	// info.kernel_load_address = unpack('I', args.boot_img.read(4))[0]
	// info.ramdisk_load_address = unpack('I', args.boot_img.read(4))[0]
	// info.vendor_ramdisk_size = unpack('I', args.boot_img.read(4))[0]
	// info.cmdline = cstr(unpack('2048s', args.boot_img.read(2048))[0].decode())
	// info.tags_load_address = unpack('I', args.boot_img.read(4))[0]
	// info.product_name = cstr(unpack('16s', args.boot_img.read(16))[0].decode())
	// info.header_size = unpack('I', args.boot_img.read(4))[0]
	// info.dtb_size = unpack('I', args.boot_img.read(4))[0]
	// info.dtb_load_address = unpack('Q', args.boot_img.read(8))[0]
	// info.vendor_ramdisk_table_size = unpack('I', args.boot_img.read(4))[0]
	// vendor_ramdisk_table_entry_num = unpack('I', args.boot_img.read(4))[0]
	// vendor_ramdisk_table_entry_size = unpack('I', args.boot_img.read(4))[0]
	// info.vendor_bootconfig_size = unpack('I', args.boot_img.read(4))[0]
	// num_vendor_ramdisk_table_pages = get_number_of_pages(
	// 	info.vendor_ramdisk_table_size, page_size)
	// vendor_ramdisk_table_offset = page_size * (
	// 	num_boot_header_pages + num_boot_ramdisk_pages + num_boot_dtb_pages)
	// bootconfig_offset = page_size * (num_boot_header_pages
	// 	+ num_boot_ramdisk_pages + num_boot_dtb_pages
	// 	+ num_vendor_ramdisk_table_pages)

	int vendor_boot_pagesize=byteToInt(vendor_boot_data,12);//offset 12
	printf("vendor_boot pagesize:%d\n",vendor_boot_pagesize);
	int vendor_ramdisk_size=byteToInt(vendor_boot_data,24);//offset 24
	printf("vendor_boot vendor_ramdisk_size:%d\n",vendor_ramdisk_size);
	int dtb_size=byteToInt(vendor_boot_data,2100);//offset 2100
	printf("vendor_boot dtb_size:%d\n",dtb_size);
	int o=(2124 + vendor_boot_pagesize - 1) / vendor_boot_pagesize;
	int p=(vendor_ramdisk_size + vendor_boot_pagesize - 1) / vendor_boot_pagesize;
	int dtb_offset = vendor_boot_pagesize * (o + p);
	hdr->dtb_size=dtb_size;
	memcpy((void *)(uint64_t)dtb_start, vendor_boot_data + dtb_offset, hdr->dtb_size);

	printf("vendor_boot dtb_offset:%d\n",dtb_offset);
	int q=(hdr->dtb_size + vendor_boot_pagesize - 1) / vendor_boot_pagesize;
	int vendor_ramdisk_table_size=byteToInt(vendor_boot_data,2112);//offset 2112
	printf("vendor_boot vendor_ramdisk_table_size:%d\n",vendor_ramdisk_table_size);
	int r=(vendor_ramdisk_table_size + vendor_boot_pagesize - 1) / vendor_boot_pagesize;
	*vendor_bootconfig_size=byteToInt(vendor_boot_data,2124);//offset 2124
	printf("vendor_boot vendor_bootconfig_size:%d\n",*vendor_bootconfig_size);
//	int s=(*vendor_bootconfig_size + vendor_boot_pagesize - 1) // vendor_boot_pagesize;
	*buf_bootconfig = malloc(*vendor_bootconfig_size);
	int bootconfig_offset=vendor_boot_pagesize * (o + p + q + r);
	memcpy(*buf_bootconfig, vendor_boot_data + bootconfig_offset, *vendor_bootconfig_size);
	printf("\nvendor_boot bootconfig:\n%s\n",*buf_bootconfig);
	free(vendor_boot_data);
}

static void prepare_data(const uint8_t* data)
{
	struct andr_img_hdr *hdr = (struct andr_img_hdr *)map_sysmem((phys_addr_t)data, 0);
	if (IMAGE_FORMAT_ANDROID == genimg_get_format(hdr)) {
		int dtb_start = env_get_hex(ENV_DTB_ADDR, DEFAULT_DTB_ADDR);
		uint8_t* buf_bootconfig = NULL;
		int size_bootconfig=0;
		printf("\nboot header_version:%d\n",hdr->header_version);
		if(hdr->header_version>=3){
			// see system/tools/mkbootimg/unpack_bootimg.py
			hdr->kernel_size=byteToInt((uint8_t *)data,8);
			hdr->ramdisk_size=byteToInt((uint8_t *)data,12);
			hdr->page_size = BOOT_IMAGE_HEADER_V3_PAGESIZE;
			prepare_data_from_vendor_boot(hdr,dtb_start,&buf_bootconfig,&size_bootconfig);
		}
		printf("boot kernel_size:%d\n",hdr->kernel_size);
		printf("boot ramdisk_size:%d\n",hdr->ramdisk_size);
		printf("boot page_size:%d\n",hdr->page_size);
		printf("dtb_size:%d\n",hdr->dtb_size);
		int kernel_start = env_get_hex(ENV_KERNEL_ADDR, DEFAULT_KERNEL_ADDR);
		int ramdisk_start = env_get_hex(ENV_RAMDISK_ADDR, DEFAULT_RAMDISK_ADDR);
		// see system/tools/mkbootimg/unpack_bootimg.py
 		int page_size = hdr->page_size;
		int num_header_pages = 1;
		int num_kernel_pages = get_number_of_pages(hdr->kernel_size, page_size);
		int num_ramdisk_pages = get_number_of_pages(hdr->ramdisk_size, page_size);
		int kernel_offset = page_size * num_header_pages;
		int ramdisk_offset = page_size * (num_header_pages + num_kernel_pages);
		int dtb_offset = page_size * (num_header_pages + num_kernel_pages + num_ramdisk_pages);

		printf("0x%x:%d 0x%x:%d 0x%x:%d \n",
			kernel_offset, hdr->kernel_size, ramdisk_offset, hdr->ramdisk_size, dtb_offset, hdr->dtb_size);

 		if (kernel_start + hdr->kernel_size > ramdisk_start || kernel_start + hdr->kernel_size > dtb_start) {
			printf("kernel overlap!!!");
		}
 		else
		{
			memcpy((void *)(uint64_t)kernel_start, data + kernel_offset, hdr->kernel_size);//kernel
			memcpy((void *)(uint64_t)ramdisk_start, data + ramdisk_offset, hdr->ramdisk_size);//ramdisk
			if( hdr->header_version < 3)
			{
				//set ramdisk size for bootm
				env_set_hex(ENV_RAMDISK_SIZE, hdr->ramdisk_size);
				memcpy((void *)(uint64_t)dtb_start, data + dtb_offset, hdr->dtb_size);//dtb
			}
			else
			{
				//get bootconfig form vendor_boot.img and append bootconfig to ramdisk
				char* bootconfig_params=(char*)buf_bootconfig;
				int ret = addBootConfigParameters(bootconfig_params, size_bootconfig,
                            ramdisk_start + hdr->ramdisk_size , 0);
				if(ret==-1)
				{
					printf("\nadd BootConfig Parameters error!!!\n");
				}
				else
				{
					printf("\nramdisk size is changed,new value is:%d\n",hdr->ramdisk_size + ret);
					//set ramdisk size for bootm
					env_set_hex(ENV_RAMDISK_SIZE, hdr->ramdisk_size + ret);
				}
			}
		}
		if (buf_bootconfig != NULL) {
			free(buf_bootconfig);
		}
	}
	unmap_sysmem(hdr);
}

static int prepare_boot_data(const AvbSlotVerifyData *out_data){
	int res = CMD_RET_FAILURE;
	int i = 0;
	printf("prepare_boot_data start\n");
	for (i = 0; i < out_data->num_loaded_partitions; i++) {
		const AvbPartitionData* loaded_partition = &out_data->loaded_partitions[i];
		printf("prepare_boot_data [%d] %ld %d \n", i, loaded_partition->data_size, loaded_partition->preloaded);
		if ( loaded_partition->partition_name != NULL) {
			prepare_data(loaded_partition->data);
			printf("partition_name=%s, data=0x%hhn, data_size=%ld\n", 
					loaded_partition->partition_name, loaded_partition->data, loaded_partition->data_size);
		}
	}
	return res;
}

static void prepare_partition_data(const char *name)
{
        int ret;
        disk_partition_t part_info;
        struct blk_desc *dev_desc  = blk_get_dev("mmc", CONFIG_FASTBOOT_FLASH_MMC_DEV);
        printf("prepare_partition_data %s\n", name);
        if (!dev_desc || dev_desc->type == DEV_TYPE_UNKNOWN) {
                printf("invalid mmc device\n");
                return;
        }
        /* Get boot partition info */
        ret = part_get_info_by_name(dev_desc, name, &part_info);
        if (ret < 0) {
                printf("cannot find %s partition\n", name);
                return;
        }

        uint8_t* data = malloc(part_info.size * part_info.blksz);
        ret = blk_dread(dev_desc, part_info.start, part_info.size, data);
        prepare_data(data);
        free(data);
        printf("prepare_partition_data %s, read=%d, %ld,%ld,%ld\n", name, ret, part_info.start, part_info.size, part_info.blksz);
}

static void clear_bcb(void)
{
	int ret;
	disk_partition_t part_info;
	struct blk_desc *dev_desc  = blk_get_dev("mmc", CONFIG_FASTBOOT_FLASH_MMC_DEV);

	//bcb clear and store
	memset(s_bcb, 0, sizeof(struct bootloader_message));

        if (!dev_desc || dev_desc->type == DEV_TYPE_UNKNOWN) {
                printf("invalid mmc device\n");
                return;
        }
        /* Get boot partition info */
        ret = part_get_info_by_name(dev_desc, MISC_PARTITION, &part_info);
        if (ret < 0) {
                printf("cannot find misc partition\n");
                return;
        }

	ret = blk_dwrite(dev_desc, part_info.start, part_info.size, s_bcb);
	printf("clear_bcb write=%d, %ld,%ld,%ld\n", ret, part_info.start, part_info.size, part_info.blksz);
}

static int do_bootandroid(struct cmd_tbl_s *cmdtp, int flag, int argc,
						char * const argv[]) {
	
	const char * const requested_partitions[] = {"boot", NULL};
	AvbSlotVerifyResult slot_result;
	AvbSlotVerifyData *out_data;
	//char *cmdline;
	//char *extra_args;
#ifdef CONFIG_ANDROID_AB
	char *slot_suffix = "_a";
#else
	char *slot_suffix = "";
#endif
	size_t bytes_read = 0;
	AvbSlotVerifyFlags flags = 0;
	int res = CMD_RET_FAILURE;

	if (avb_ops)
		avb_ops_free(avb_ops);

	avb_ops = avb_ops_alloc(0);

	if (!avb_ops)
	{
		printf("Failed to initialize avb2\n");
		return CMD_RET_FAILURE;
	}

	if (s_bcb)
		free(s_bcb);

	s_bcb = malloc(sizeof(struct bootloader_message));

	if (!s_bcb)
	{
		printf("Failed to initialize bcb\n");
		return CMD_RET_FAILURE;
	}

	if (AVB_IO_RESULT_OK == avb_ops->read_from_partition(avb_ops, 
				MISC_PARTITION,
				0, 
				sizeof(struct bootloader_message), 
				s_bcb,
				&bytes_read)) {
		printf("Bcb read %ld bytes, %s\n", bytes_read, s_bcb->command);\
	}
	else
	{
		printf("Bcb read failed\n");
		return CMD_RET_FAILURE;
	}

	if (0 == strncmp(s_bcb->command, "bootonce-bootloader", strlen("bootonce-bootloader"))
		|| 0 == strncmp(s_bcb->command, "boot-recovery", strlen("boot-recovery")))
	{
		printf("fastboot usb 0\n");
		clear_bcb();
		run_command("fastboot usb 0", 0);
		return CMD_RET_SUCCESS;
	}
	else
	{
		printf("normal\n");
	}

	slot_result =
		avb_slot_verify(avb_ops,
				requested_partitions,
				slot_suffix,
				flags,
				AVB_HASHTREE_ERROR_MODE_RESTART_AND_INVALIDATE,
				&out_data);

	if (AVB_SLOT_VERIFY_RESULT_OK == slot_result)
	{
		printf("Verification passed successfully\n");
                res = prepare_boot_data(out_data);
	}
	else
	{
		printf("Verification failed, slot_result=%d\n", slot_result);
		prepare_partition_data(BOOT_PARTITION);
	}

	if (out_data)
		avb_slot_verify_data_free(out_data);

	return res;
}

U_BOOT_CMD(
	bootandroid, 2,	1, do_bootandroid,
	"bootandroid   - boot android bootimg from device\n",
	"mmc0 | mmc1 | mmc2 | mmcX]\n    "
	"- boot application image stored in storage device like mmc\n"
);

