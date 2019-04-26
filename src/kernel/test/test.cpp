#ifdef TESTING
#include "test.hpp"
#include "debug.hpp"

using namespace Test;

UnitTestNode* Test::groups[COUNT] = {0};

static const char* group_names[] = {
		[PRE_CPU] = "Pre-CPU",
		[PRE_PMEM] = "Pre-PhysMem",
		[PRE_PAGING] = "Pre-Paging",
		[PRE_ALLOC] = "Pre-Kernel::Alloc",
		[PRE_INT] = "Pre-Interrupts",
		[PRE_PROC] = "Pre-Proc",
		[PRE_VFS] = "Pre-VFS",
		[PRE_RAMDISK] = "Pre-RAMDISK",
		[PRE_SCHEDULER] = "Pre-Scheduler",
		[OTHER] = "Other"
};

static size_t count_tests_in_group(size_t group)
{
	size_t count = 0;
	UnitTestNode* node = groups[group];
	while (node != nullptr)
	{
		count++;
		node = node->next;
	}
	return count;
}

static size_t count_tests()
{
	size_t count = 0;
	for (size_t i = 0; i < COUNT; i++)
		count += count_tests_in_group(i);
	return count;
}

void Test::execute_tests(size_t group)
{
	UnitTestNode* next_test = groups[group];
	if (next_test == nullptr)
		return;

	::Debug::printf("?RUN_GROUP %s\n", group_names[group]);
	size_t test_count = 0;
	while (next_test != nullptr)
	{
		::Debug::printf("?RUN_TEST %s\n", next_test->name);
		next_test->current();
		next_test = next_test->next;
		::Debug::printf("?TEST_GOOD\n", ++test_count);
	}
}

void Test::begin_tests()
{
	::Debug::printf("?TEST_COUNT %d\n", count_tests());
}

void Test::end_tests()
{
	::Debug::printf("?DONE\n");
}

// These are required as the unit tests require some funky magic.
extern "C" {
	void __cxa_guard_acquire() {}
	void __cxa_guard_release() {}
}

#endif
