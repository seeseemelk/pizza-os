#include <cstddef>
#include <cstring>

static void write_string(const char* str)
{
	char* vga = (char*) 0xB8000;
	while (*str != 0)
	{
		*vga = *str;
		str++;
		vga += 2;
	}
}

extern "C" void kernel_main()
{
	write_string("Selina is een schatje!");
}
