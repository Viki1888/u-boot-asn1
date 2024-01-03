# u-boot-asn1
## Introduction
The project "u-boot-asn1" serves as a proof of concept for passing Unified Discovery Data to the kernel. It converts the Unified Discovery Data into properties of the FDT /cpus/cpu0 node within U-Boot. During kernel boot, the Unified Discovery Data is handed off to the kernel along with the FDT file.

## Reference
1. https://github.com/riscv/configuration-structure/tree/master - RISC-V Unified Discovery Task Group Repository.
2. https://lionet.info/asn1c/documentation.html - ASN.1 documentation
3. https://lionet.info/asn1c/download.html _ Online ASN.1 compiler

## Source code
- `board/thead/light-c910/Kconfig` - Add `CONFIG_ASN1` and `CONFIG_CMD_ASN2FDT` configures.
- `cmd/asn2fdt.c` - The core implementation of inserting data structure into FDT file.
- `cmd/Makefile` - Compilation rule of asn2fdt.c.

## Build
1. Open ASN1 configuration option: In `configs/light_a_val_sec_defconfig`, insert:
```
CONFIG_ASN1=y
CONFIG_CMD_ASN2FDT=y
```
2. Generate the configuration file `.config`: Execute the following command in project root directory:
```Shell
make light_a_val_sec_defconfig
```
3. Build the project and generate `u-boot-with-spl.bin`: Execute the following command in project root directory:
```Shell
./run.sh
```
Note: change the compiler path to your local compiler path.
4. Burn `u-boot-with-spl.bin` into Xuantie Light A board.

## Run
1. Boot the Light A board. Press any key to enter into U-Boot shell.
2. Print command `asn2fdt` in U-Boot shell as following:
```
C910 Light# asn2fdt
91039 bytes read in 1 ms (86.8 MiB/s)
```
3. Boot kernel
```
C910 Light# boot
43140 bytes read in 1 ms (41.1 MiB/s)
5280624 bytes read in 21 ms (239.8 MiB/s)
87256 bytes read in 1 ms (83.2 MiB/s)
641416 bytes read in 2 ms (305.9 MiB/s)
22944768 bytes read in 74 ms (295.7 MiB/s)
6400 bytes read in 1 ms (6.1 MiB/s)
sec version: 1.4

---------------------------------------------
entry point: 0x544d4253
image size: 4736 Bytes
head version: 0x2
image version: 0x100
image checksum: 0xfc2ff
image run addr: 0x100000
image offset: 0x580
image digest scheme: 4
image sign scheme: 2
image encrypt type: 0

---------------------------------------------
87256 bytes read in 1 ms (83.2 MiB/s)
1608 bytes read in 1 ms (1.5 MiB/s)
## Flattened Device Tree blob at 03800000
   Booting using the fdt blob at 0x3800000
   Using Device Tree in place at 0000000003800000, end 0000000003819fff

Starting kernel ...

```
4. when entering kernel, log in 'root' account. Then check the `/cpus/cpu@0` node in modified FDT file:
```
root@light-a-val:~# cd  /sys/firmware/devicetree/base/cpus/cpu@0
root@light-a-val:/sys/firmware/devicetree/base/cpus/cpu@0# ls
 c                 cpu-freq       dvdd-supply                    operating-points   sv57
 clock-latency     cpu-icache     dynamic-power-coefficient      phandle            svpbmt
 clock-names       cpu-l2cache    ext                            reg                version
 clocks            cpu-tlb        f                              riscv,isa          zicsr
 compatible        cpu-vector     interrupt-controller           rvv                zihpm
'#cooling-cells'   d              light,dvddm-operating-points   status
 cpu-cacheline     device_type    mmu-type                       sv39
 cpu-dcache        dvddm-supply   name                           sv48
```
However, if not open the asn2 configuration and not execute `asn2fdt`` command, the FDT /cpus/cpu@0 node will look like:
```
root@light-a-val:~# cd  /sys/firmware/devicetree/base/cpus/cpu@0
root@light-a-val:/sys/firmware/devicetree/base/cpus/cpu@0# ls
 clock-latency     cpu-cacheline   cpu-tlb        dynamic-power-coefficient      operating-points
 clock-names       cpu-dcache      cpu-vector     interrupt-controller           phandle
 clocks            cpu-freq        device_type    light,dvddm-operating-points   reg
 compatible        cpu-icache      dvddm-supply   mmu-type                       riscv,isa
'#cooling-cells'   cpu-l2cache     dvdd-supply    name                           status

```
It can be observed that some new properties, such as `sv57`, `rvv` has been newly inserted to the /cpus/cpu@0 node.

