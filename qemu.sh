#!/bin/sh
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug .. &&
make &&
qemu-system-i386 -kernel kernel.elf -m 8M -gdb tcp::1234 $*
