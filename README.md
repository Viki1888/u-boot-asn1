# u-boot-asn1
Support U-Boot to insert unified discovery data into FDT file, which will be passed to kernel.

## Implemented Files
- `board/thead/light-c910/Kconfig` - Add CONFIG_ASN1 and CONFIG_CMD_ASN2FDT configures.
- `cmd/asn2fdt.c` - The core implementation of inserting data structure into FDT file.
- `cmd/Makefile` - Compilation rule of asn2fdt.c.

## Usage
1. set `CONFIG_ASN1=y` and `CONFIG_CMD_ASN2FDT=y` in `configs/light_a_val_sec_defconfig`.
2. genenrate a configuration file and U-Boot binary file.
3. when entering the U-Boot shell, print command `asn2fdt`.
4. boot kernel.
