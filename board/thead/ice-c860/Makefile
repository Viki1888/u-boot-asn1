#
# Copyright (C) 2017-2020 Alibaba Group Holding Limited
#
# SPDX-License-Identifier: GPL-2.0+
#

obj-y := ice_fpga.o
obj-y += gpio/gpio.o

ifdef CONFIG_SPL_BUILD
obj-y += spl/lpddr4/ddr.o
obj-y += spl/lpddr4/ddr_phy.o
obj-y += spl/spl.o
obj-y += om/om.o
endif
