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

void Asserts::assertEquals(const char* msg, u32 expected, u32 got)
{
	if (got != expected)
	{
		::Debug::printf("!<<<0x%X§0x%X>>>%s\n", expected, got, msg);
		fail();
	}
}

#endif
