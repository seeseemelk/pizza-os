# Bootloader
PizzaOS can be booted by any boot loader that supports the [Multiboot standard](https://www.gnu.org/software/grub/manual/multiboot/).
The build scripts will created an ISO that uses [GRUB](https://www.gnu.org/software/grub/) to boot the system, but this is not a requirements.

## Kernel Image
The kernel is in ELF format, so the boot loader has to be able to parse a Multiboot-Compliant ELF image.

## Initial Ramdisk
The boot loader also has to load the ramdisk into memory, and give the memory address to the kernel.
This ramdisk is nothing but a simple [tar archive](https://en.wikipedia.org/wiki/Tar_(computing)) containing the binaries that are required before a filesystem can be loaded.
