#ifndef TEST_HPP_
#define TEST_HPP_
#include <cstddef>
#include <cstdint>
#include "debug.hpp"

namespace Test
{
	typedef void(*UnitTest)();

	struct UnitTestNode
	{
		UnitTest current;
		UnitTestNode* next;
		const char* name;
	};

	enum Group
	{
		PRE_CPU,
		PRE_PMEM,
		PRE_PAGING,
		PRE_ALLOC,
		PRE_INT,
		PRE_PROC,
		PRE_VFS,
		PRE_RAMDISK,
		PRE_SCHEDULER,
		OTHER,
		COUNT,

		POST_CPU = PRE_PMEM,
		POST_PMEM = PRE_PAGING,
		POST_PAGING = PRE_ALLOC,
		POST_ALLOC = PRE_INT,
		POST_INT = PRE_PROC,
		POST_PROC = PRE_VFS,
		POST_VFS = PRE_RAMDISK,
		POST_RAMDISK = PRE_SCHEDULER,
		POST_SCHEDULER = PRE_SCHEDULER
	};

	extern UnitTestNode* groups[];

	void begin_tests();
	void end_tests();
	void execute_tests(size_t group);

	namespace Asserts
	{
		void fail();
		void fail(const char* msg);
		void failStr(const char* msg, const char* expected, const char* got);
		void assertTrue(const char* msg, bool value);
		void assertFalse(const char* msg, bool value);
		void assertEquals(const char* msg, u32 expected, u32 got);
		void assertEquals(const char* msg, u32 expected, void* got);
		void assertNotEquals(const char* msg, u32 not_expected, u32 got);
		void assertNotEquals(const char* msg, u32 not_expected, void* got);
	}
}

#ifdef TESTING

#define __TESTING_MAKE_NAME(a, b) a ## b
#define __TESTING_MAKE_NAME2(a, b) __TESTING_MAKE_NAME(a, b)

#define TEST(GROUP, NAME, x) \
	static void __TESTING_MAKE_NAME2(_unitTest_, __LINE__) () { \
		using namespace ::Test::Asserts; \
		x \
	} \
	__attribute__((constructor)) static void __TESTING_MAKE_NAME2(_setupUnitTest_, __LINE__)() \
	{ \
		static Test::UnitTestNode node = { \
				.current = __TESTING_MAKE_NAME2(_unitTest_, __LINE__), \
				.next = Test::groups[Test::Group::GROUP], \
				.name = NAME \
		}; \
		Test::groups[Test::Group::GROUP] = &node; \
		/* The following line is to prevent eclipse from complaining about an unused function*/ \
		if (0) {__TESTING_MAKE_NAME2(_setupUnitTest_, __LINE__)();}; \
	}

#else

#define TEST(x) \
		;

#endif

#endif
