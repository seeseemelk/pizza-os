#include <stdbool.h>
#include <stdio.h>
#include "config.h"
#include "dev/devices.h"
#include "dev/tty.h"
#include "arch/pc/dev/vga.h"

void kernel_main()
{
#ifdef ENABLE_VGA
	tty_set_tty(vga_init());
#endif

	tty_clear();
	tty_print("Starting pizza-os (yum!)...\n");

	tty_print("Reached end of kernel_main()\n");

	printf("Hello world!\n");
	printf("Percent test %%\n");
	printf("Decimal test %d\n", 1385ul);
	printf("Hexadecimal test 0x%x\n", 0xc0ffee);
	printf("Hexadecimal test 0x%X\n", 0xc0ffee);
}
