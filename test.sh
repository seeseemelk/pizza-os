#!/bin/sh
set -e
./build_test.sh
qemu-system-i386 -cdrom pizzaos.test.iso -m 16M -gdb tcp::1234 -serial stdio -display none -no-reboot $@
