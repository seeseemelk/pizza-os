cd build &&
cmake .. &&
make &&
rm -rf iso &&
mkdir -p iso/boot/grub &&
echo "menuentry \"PizzaOS\" {" > iso/boot/grub/grub.cfg &&
echo "	multiboot /boot/kernel.elf" >> iso/boot/grub/grub.cfg &&
echo "}" >> iso/boot/grub/grub.cfg &&
cp kernel.elf iso/boot/ &&
grub-mkrescue -o pizzaos.iso iso
