#!/bin/sh

echo "Generating mboot image ..."
sh ./mboot.sh ./u-boot-spl.bin ./u-boot.bin .

echo "Generating uboot_ram image ..."
bash ./multi_bin.sh ./uboot_ram.img  ./dt.dtb ./u-boot.bin
