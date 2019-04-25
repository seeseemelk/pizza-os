#ifdef TESTING
#include "test.hpp"
#include "debug.hpp"

using namespace Test;

UnitTestNode* Test::next_test = nullptr;

void Test::execute_tests()
{
	int test_count = 0;
	while (next_test != nullptr)
	{
		::Debug::printf("?RUN_TEST %d %s\n", ++test_count, next_test->name);
		next_test->current();
		next_test = next_test->next;
		::Debug::printf("?TEST_GOOD %d\n", ++test_count);
	}

	::Debug::printf("?DONE\n");
}

// These are required as the unit tests require some funky magic.
extern "C" {
	void __cxa_guard_acquire() {}
	void __cxa_guard_release() {}
}

#endif
