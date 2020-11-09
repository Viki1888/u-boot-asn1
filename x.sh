make clean
make distclean
make ice_evb_c910_defconfig
make ARCH=riscv CROSS_COMPILE=~/nixon/bin/riscv64-unknown-linux-gnu- -j

~/nixon/bin/riscv64-unknown-linux-gnu-objdump -d u-boot > 123
