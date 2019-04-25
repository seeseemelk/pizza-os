#ifdef TESTING
#include "test.hpp"
#include "debug.hpp"
#include "cpu.hpp"

using namespace Test;
using namespace Asserts;

void Asserts::fail()
{
	CPU::reset();
}

void Asserts::fail(const char* msg)
{
	::Debug::printf("!>>>%s\n", msg);
	fail();
}

void Asserts::failStr(const char* msg, const char* expected, const char* got)
{
	::Debug::printf("!<<<%s§%s>>>%s\n", expected, got, msg);
	fail();
}

/*
void Asserts::failBool(const char* msg, bool expected, bool got)
{
	failStr(msg,
			expected ? "true" : "false",
			got ? "true" : "false");
}
*/

void Asserts::assertTrue(const char* msg, bool value)
{
	if (value == false)
		failStr(msg, "true", "false");
}

void Asserts::assertFalse(const char* msg, bool value)
{
	if (value == true)
		failStr(msg, "false", "true");
}

#endif
