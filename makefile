# Tools to use
export CC := i386-elf-gcc
export CXX := i386-elf-g++
export LD := i386-elf-ld
export ASM := nasm

# Common flags
export CFLAGS = -Wall -Wextra -ffreestanding -std=gnu17 -nostdlib -O0 -ggdb
export CXXFLAGS = -Wall -Wextra -ffreestanding -std=gnu++17 -nostdlib -fno-exceptions -fno-rtti -O0 -ggdb -fomit-frame-pointer

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

.PHONY: clean all build_all build_test test check crt_obj libkc libc libkcxx kernel_test

all:
	@mkdir -p $(BUILDDIR)
	@$(MAKE) -n build_all > $(BUILDDIR)/progress.txt
	@$(MAKE) build_all | tools/progress_make.lua $(BUILDDIR)/progress.txt `tput cols`

check test:
	@mkdir -p $(BUILDDIR)
	@$(MAKE) -n build_test > $(BUILDDIR)/progress.txt
	@$(MAKE) build_test | tools/progress_make.lua $(BUILDDIR)/progress.txt `tput cols`
	./tools/test_runner.lua $(TEST_ARG) 2>&1

test_verbose:
	@mkdir -p $(BUILDDIR)
	@$(MAKE) -n build_test > $(BUILDDIR)/progress.txt
	@$(MAKE) build_test | tools/progress_make.lua $(BUILDDIR)/progress.txt `tput cols`
	./tools/test_runner.lua -v 2>&1

build_all: pizzaos.iso
	echo "Build finished"

build_test: pizzaos.test.iso

crt_obj:
	+BUILDDIR=$(BUILDDIR)/libc $(MAKE) -C src/stdlib/libc/crt $(CRT_OBJ)

libkc:
	+BUILDDIR=$(BUILDDIR)/libc $(MAKE) -C src/stdlib/libc/kernel $(LIBKC)

libc: libkc
	+BUILDDIR=$(BUILDDIR)/libc $(MAKE) -C src/stdlib/libc/user $(LIBC)

libkcxx:
	+BUILDDIR=$(BUILDDIR)/libcxx $(MAKE) -C src/stdlib/libcxx/kernel $(LIBKCXX)

$(PIZZAOS_ELF): libkc libkcxx crt_obj
	+$(MAKE) -C src/kernel $(PIZZAOS_ELF)

$(PIZZAOS_TEST_ELF): libkc libkcxx crt_obj
	+$(MAKE) -C src/kernel $(PIZZAOS_TEST_ELF)

pizzaos.iso: $(PIZZAOS_ELF) isodir/boot/grub/grub.cfg
	$(STATUS) GENISO $@
	cp $(PIZZAOS_ELF) isodir/boot
	grub-mkrescue -o pizzaos.iso isodir 2>&1

pizzaos.test.iso: $(PIZZAOS_TEST_ELF) isodir.test/boot/grub/grub.cfg
	$(STATUS) GENISO $@
	cp $(PIZZAOS_TEST_ELF) isodir.test/boot
	grub-mkrescue -o pizzaos.test.iso isodir.test 2>&1

clean:
	rm -rf $(BUILDDIR)
	@echo "Build directory cleaned"
	@echo
