# Tools to use
export CC := i386-elf-gcc
export CXX := i386-elf-g++
export LD := i386-elf-ld
export ASM := nasm

# Common flags
export CFLAGS = -Wall -Wextra -ffreestanding -std=gnu17 -nostdlib -O3
export CXXFLAGS = -Wall -Wextra -ffreestanding -std=gnu++17 -nostdlib -fno-exceptions -fno-rtti -O3 -fomit-frame-pointer

# Flags for kernel/user space
export CFLAGS_KERNEL = $(CFLAGS)
export CFLAGS_USER = $(CFLAGS)
export CXXFLAGS_KERNEL = $(CXXFLAGS)
export CXXFLAGS_USER = $(CXXFLAGS)

# Include directories
export INCLUDE_LIBKC := $(abspath src/stdlib/libc/kernel)
export INCLUDE_LIBKCXX := $(abspath src/stdlib/libcxx/kernel)

# Build directories
export BUILDDIR := $(abspath build)

# Libraries
export CRT_OBJ := $(BUILDDIR)/libc/crt.o
export LIBC := $(BUILDDIR)/libc/libc.a
export LIBKC := $(BUILDDIR)/libc/libkc.a
export LIBCXX := $(BUILDDIR)libcxx/libcxx.a
export LIBKCXX := $(BUILDDIR)/libcxx/libkcxx.a
export PIZZAOS_ELF := $(BUILDDIR)/pizzaos.elf
export PIZZAOS_TEST_ELF := $(BUILDDIR)/pizzaos.test.elf

# Useful macros
export DEFAULTMAKE := $(abspath default.make)
include $(DEFAULTMAKE)

.PHONY: clean all_verbose

all:
	@mkdir -p $(BUILDDIR)
	@$(MAKE) -n all_verbose > $(BUILDDIR)/progress.txt
	@$(MAKE) all_verbose | tools/progress_make.lua $(BUILDDIR)/progress.txt `tput cols`

all_verbose: pizzaos.iso $(PIZZAOS_TEST_ELF)
	echo "Build finished"

$(CRT_OBJ):
	+BUILDDIR=$(BUILDDIR)/libc $(MAKE) -C src/stdlib/libc/crt $@

$(LIBKC):
	+BUILDDIR=$(BUILDDIR)/libc $(MAKE) -C src/stdlib/libc/kernel $@

$(LIBC): $(LIBKC)
	+BUILDDIR=$(BUILDDIR)/libc $(MAKE) -C src/stdlib/libc/user $@

$(LIBKCXX):
	+BUILDDIR=$(BUILDDIR)/libcxx $(MAKE) -C src/stdlib/libcxx/kernel $@

$(PIZZAOS_ELF) $(PIZZAOS_TEST_ELF): $(LIBKC) $(LIBKCXX) $(CRT_OBJ)
	+$(MAKE) -C src/kernel $@

pizzaos.iso: $(PIZZAOS_ELF) isodir/boot/grub/grub.cfg
	$(STATUS) GENISO $@
	grub-mkrescue -o pizzaos.iso isodir 2>&1

clean:
	rm -rf $(BUILDDIR)
	@echo "Build directory cleaned"
	@echo

#include src/stdlib/libc/makefile
