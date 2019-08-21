#!/bin/sh
set -e
make pizzaos.iso -j
qemu-system-i386 -cdrom pizzaos.iso -m 16M -gdb tcp::1234 -serial stdio $@
