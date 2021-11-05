// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2017, STMicroelectronics - All Rights Reserved
 * Author(s): Patrice Chotard, <patrice.chotard@st.com> for STMicroelectronics.
 */

#include <common.h>
#include <asm/io.h>
#include <dwc3-uboot.h>
#include <usb.h>
#include <cpu_func.h>

#ifdef CONFIG_USB_DWC3
static struct dwc3_device dwc3_device_data = {
	.maximum_speed = USB_SPEED_HIGH,
	.dr_mode = USB_DR_MODE_PERIPHERAL,
	.index = 0,
};

int usb_gadget_handle_interrupts(int index)
{
	dwc3_uboot_handle_interrupt(index);
	return 0;
}

int board_usb_init(int index, enum usb_init_type init)
{
	dwc3_device_data.base = 0xFFE7040000UL;
	return dwc3_uboot_init(&dwc3_device_data);
}

int board_usb_cleanup(int index, enum usb_init_type init)
{
	dwc3_uboot_exit(index);
	return 0;
}

int g_dnl_board_usb_cable_connected(void)
{
	return 1;
}
#endif

#ifdef CONFIG_CMD_BOOT_SLAVE
#define E902_SYSREG_START	0xfffff48044
#define E902_SYSREG_RESET	0xfffff44024
#define E902_START_ADDRESS	0xFFEF8000
#define C910_E902_START_ADDRESS 0xFFFFEF8000
#define E902_IOPMP_BASE		0xFFFFC21000

void set_slave_cpu_entry(phys_addr_t entry)
{
    writel(entry, (void *)E902_SYSREG_START);
}

void disable_slave_cpu(void)
{
    writel(0x0, (void *)E902_SYSREG_RESET);
}

void enable_slave_cpu(void)
{
    writel(0x3, (void *)E902_SYSREG_RESET);
}

int do_bootslave(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
	writel(0xffffffff, (void *)(E902_IOPMP_BASE + 0xc0));
	disable_slave_cpu();
	set_slave_cpu_entry(E902_START_ADDRESS);
	flush_cache((uintptr_t)C910_E902_START_ADDRESS, 0x10000);
	enable_slave_cpu();

	return 0;
}
#endif
