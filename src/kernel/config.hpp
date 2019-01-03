#ifndef CONFIG_HPP
#define CONFIG_HPP

namespace Config
{
	namespace Int
	{
		#define INT(name, value) static const int name = value;
		//#define BOOL(name, value) static const bool name = value;
		//#define STRING(name, value) static char const * const name = value;
		#include "config.h"
	}
}

#endif
