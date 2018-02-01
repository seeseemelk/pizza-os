#!/bin/sh
./floppy.sh &&
qemu-system-i386 -fda build/pizzaos.img -gdb tcp::1234 $*
