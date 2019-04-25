#!/bin/sh
set +e
rm -rf build
rm -f isodir/boot/{pizzaos.elf, initrd.elf}
rm -f isodir.test/boot/{pizzaos.test.elf, initrd.test.elf}
rm -f pizzaos.iso pizzaos.test.elf
