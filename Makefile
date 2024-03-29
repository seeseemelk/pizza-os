# Tools to use
export CC := i386-elf-gcc
export CXX := i386-elf-g++
export LD := i386-elf-ld
export ASM := nasm

# Common flags
export CFLAGS = \
	-Wall -Wextra -Werror -pedantic -ffreestanding -std=gnu17 -nostdlib -O0 \
	-ggdb -fdiagnostics-color=always
export CXXFLAGS = \
	-Wall -Wextra -Werror -pedantic -ffreestanding -std=gnu++17 -nostdlib \
	-fno-exceptions -fno-rtti -O0 -ggdb -fomit-frame-pointer -fdiagnostics-color=always
export LDFLAGS =

# Flags for kernel/user space
export LDFLAGS_KERNEL = $(LDFLAGS)
export CFLAGS_KERNEL = $(CFLAGS)
export CXXFLAGS_KERNEL = $(CXXFLAGS)
export CXXFLAGS_USER = $(CXXFLAGS)

# Include directories
export INCLUDE_LIBKC := $(abspath src/stdlib/libc/kernel)
export INCLUDE_LIBKCXX := $(abspath src/stdlib/libcxx/kernel)

# Build directories
export BUILDDIR := $(abspath build)

# Source directories
export PKGDIR := $(abspath src/pkg)

# Libraries
export PIZZAOS_ELF := $(BUILDDIR)/pizzaos.elf
export PIZZAOS_TEST_ELF := $(BUILDDIR)/pizzaos.test.elf
export INITRD_TAR := $(BUILDDIR)/initrd.tar

# Useful dependencies for user application
export USER_COMMON := $(CRTI) $(CRTN) $(abspath src/stdlib/libc/user/linker.ld)
export USER_LIBC := $(LIBC) $(LIBKC)
export USER_LIBCXX := $(LIBCXX) $(LIBKCXX)

SHELL = /bin/bash
.SHELLFLAGS = -o pipefail -c

APPS = mcp

# Useful macros
export BASEMAKE := $(abspath Makefile.base)
include $(BASEMAKE)

.PHONY: clean all build_all build_test test check crt_obj libkc libc libkcxx kernel_test build_test_progress initrd apps _always

all:
	@mkdir -p $(BUILDDIR)
	@$(MAKE) -nBi build_all > $(BUILDDIR)/progress.txt
	@$(MAKE) build_all 2>&1 | tools/progress_make.lua $(BUILDDIR)/progress.txt `tput cols`

check test: build_test_progress
	./tools/test_runner.lua $(TEST_ARG) 2>&1

test_verbose: build_test_progress
	./tools/test_runner.lua -v 2>&1

apps: _always
	$(MAKE) BUILDDIR=$(BUILDDIR)/pkg -C src/pkg $(APPS:%=pkg_%)

build_all: pizzaos.iso
	echo "Build finished"

build_test: pizzaos.test.iso

build_test_progress:
	@mkdir -p $(BUILDDIR)
	@$(MAKE) -n build_test > $(BUILDDIR)/progress.txt
	@$(MAKE) build_test 2>&1 | tools/progress_make.lua $(BUILDDIR)/progress.txt `tput cols`

$(PIZZAOS_ELF): #libkc libkcxx crt_obj
	$(MAKE) -C src/kernel $(PIZZAOS_ELF)

$(PIZZAOS_TEST_ELF): #libkc libkcxx crt_obj
	$(MAKE) -C src/kernel $(PIZZAOS_TEST_ELF)
	
$(INITRD_TAR): apps
	$(MAKE) APPS=$(APPS) -C src/initrd $(INITRD_TAR)

pizzaos.iso: $(PIZZAOS_ELF) $(INITRD_TAR) isodir/boot/grub/grub.cfg
	@$(STATUS) GENISO $@
	cp $(PIZZAOS_ELF) $(INITRD_TAR) isodir/boot
	grub-mkrescue -o pizzaos.iso isodir 2>&1

pizzaos.test.iso: $(PIZZAOS_TEST_ELF) $(INITRD_TAR) isodir.test/boot/grub/grub.cfg
	@$(STATUS) GENISO $@
	cp $(PIZZAOS_TEST_ELF) $(INITRD_TAR) isodir.test/boot
	grub-mkrescue -o pizzaos.test.iso isodir.test 2>&1

clean:
	rm -rf $(BUILDDIR)
	rm -f pizzaos.iso pizzaos.test.iso isodir/boot/initrd.tar isodir/boot/pizzaos.elf isodir.test/boot/initrd.tar isodir.test/boot/pizzaos.test.elf
	@echo "Build directory cleaned"
	@echo

qemu: all
	qemu-system-i386 -cdrom pizzaos.iso -m 16M -gdb tcp::1234 -serial stdio $(QEMU_ARGS)

bochs: all
	bochs -q
	
_always:
