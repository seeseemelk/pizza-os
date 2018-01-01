#include <stdbool.h>

void kernel_main()
{
	char* vga = (char*) 0xB8000;
	vga[0] = 'P';
	vga[2] = 'i';
	vga[4] = 'z';
	vga[6] = 'z';
	vga[8] = 'a';
	while (true) ;
}
