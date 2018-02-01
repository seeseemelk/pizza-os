#!/bin/sh
cd build &&
#cmake -DCMAKE_BUILD_TYPE=Debug .. &&
cmake .. &&
make &&

echo "DEFAULT PizzaOS"          > syslinux.cfg &&
echo "LABEL PizzaOS"           >> syslinux.cfg &&
echo "SAY Loading PizzaOS"  >> syslinux.cfg &&
echo "KERNEL mboot.c32"  >> syslinux.cfg &&
echo "APPEND kernel.elf"  >> syslinux.cfg &&

rm -f pizzaos.img &&
fallocate -l 1474560  pizzaos.img &&
mkdosfs pizzaos.img &&
syslinux --mbr -i -s pizzaos.img &&
mcopy -i pizzaos.img /usr/lib/syslinux/bios/mboot.c32 ::/ &&
mcopy -i pizzaos.img /usr/lib/syslinux/bios/libcom32.c32 ::/ &&
mcopy -i pizzaos.img kernel.elf ::/ &&
mcopy -i pizzaos.img syslinux.cfg ::/
