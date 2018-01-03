#!/bin/sh
cd build
cmake .. &&
make &&
qemu-system-i386 -kernel kernel.elf -m 256M -gdb tcp::1234
