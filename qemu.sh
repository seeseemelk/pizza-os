#!/bin/sh
cd build
make
qemu -kernel kernel.bin
