#
# Copyright (C) 2017-2020 Alibaba Group Holding Limited
#
# SPDX-License-Identifier: GPL-2.0+
#

obj-y += c910_ice.o

ifdef CONFIG_IS_ASICF
obj-y += clock/clock.o
endif

ifeq ($(CONFIG_SPL_BUILD)$(CONFIG_PPL_BUILD),)
obj-y += c910_cache.o
else
obj-y += gpio/gpio.o
obj-y += om/om.o
endif

ifdef CONFIG_SPL_BUILD
ifdef CONFIG_IS_ASICF
obj-y += spl/lpddr4/ddr.o
obj-y += spl/lpddr4/ddr_phy.o
else
obj-y += spl/ddr.o
endif
obj-y += spl/spl.o
endif

ifdef CONFIG_PPL_BUILD
obj-y += ppl/ppl.o
ifdef CONFIG_IS_ASICF
obj-y += ppl/clock.o
endif
endif
