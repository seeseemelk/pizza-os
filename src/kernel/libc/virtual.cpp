#include "cpu.hpp"
#include "debug.hpp"

extern "C" void __cxa_pure_virtual()
{
	// Do nothing.
	log("__cxa_pure_virtual() called");
	CPU::hang();
}

