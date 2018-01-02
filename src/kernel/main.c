#include <stdbool.h>
#include "config.h"
#include "dev/devices.h"
#include "arch/pc/dev/vga.h"

void kernel_main()
{
#ifdef ENABLE_VGA
	vga_init();
#endif

	/*char* vga = (char*) 0xB8000;
	vga[0] = 'P';
	vga[2] = 'i';
	vga[4] = 'z';
	vga[6] = 'z';
	vga[8] = 'a';
	while (true) ;*/
}
