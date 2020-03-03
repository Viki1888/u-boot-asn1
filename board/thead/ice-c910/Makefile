#
# Copyright (C) 2017-2020 Alibaba Group Holding Limited
#
# SPDX-License-Identifier: GPL-2.0+
#

obj-y += c910_ice.o

ifneq ($(CONFIG_SPL_BUILD)$(CONFIG_PPL_BUILD),)
obj-y += gpio/gpio.o
obj-y += om/om.o
endif

obj-$(CONFIG_SPL_BUILD) += spl/spl.o
obj-$(CONFIG_SPL_BUILD) += spl/ddr.o

obj-$(CONFIG_PPL_BUILD) += ppl/ppl.o
obj-$(CONFIG_PPL_BUILD) += om/om.o

