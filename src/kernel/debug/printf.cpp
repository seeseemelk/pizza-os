#include "debug.hpp"

#include "libc/stdarg.hpp"
#include "libc/stddef.hpp"


using namespace Debug;

static char conversion[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
static void print_hex(unsigned int num)
{
	for (int i = 0; i < 8; i++)
	{
		char c = conversion[(num & 0xF0000000) >> 28];
		num <<= 4;
		Debug::put(c);
	}
}

static void print_uinteger(unsigned int num)
{
	char arr[sizeof(num) * 4 + 1] = {0};
	arr[0] = '0';

	int i = 0;
	while (num > 0)
	{
		arr[i++] = num % 10 + '0';
		num /= 10;
	}

	while (i >= 0)
	{
		if (arr[i] != 0)
			Debug::put(arr[i]);
		i--;
	}
}

void print_bool(bool v)
{
	if (v)
		Debug::puts("true");
	else
		Debug::puts("false");
}

void Debug::printf(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	bool special = false;
	while (*fmt != 0)
	{
		if (special)
		{
			switch (*fmt)
			{
			case '%':
				Debug::put('%');
				break;
			case 'd':
				print_uinteger(va_arg(args, unsigned int));
				break;
			case 'X':
				print_hex(va_arg(args, unsigned int));
				break;
			case 's':
				Debug::puts(va_arg(args, const char*));
				break;
			case 'b':
				print_bool(va_arg(args, int));
				break;
			default:
				Debug::puts("[BAD TOKEN]");
				break;
			}
			special = false;
		}
		else if (*fmt == '%')
			special = true;
		else
			Debug::put(*fmt);
		fmt++;
	}

	va_end(args);
}
