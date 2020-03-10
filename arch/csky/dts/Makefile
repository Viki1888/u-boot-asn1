# SPDX-License-Identifier: GPL-2.0+

dtb-$(CONFIG_TARGET_ICE_C860) += ice-c860.dtb
dtb-$(CONFIG_TARGET_ICE_C810) += ice-c810.dtb
dtb-$(CONFIG_TARGET_TIANHUO_C810) += tianhuo-c810.dtb

targets += $(dtb-y)

DTC_FLAGS += -R 4 -p 0x1000

PHONY += dtbs
dtbs: $(addprefix $(obj)/, $(dtb-y))
	@:

clean-files := *.dtb
