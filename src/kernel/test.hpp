#ifndef TEST_HPP_
#define TEST_HPP_

namespace Test
{
	typedef void(*UnitTest)();

	struct UnitTestNode
	{
		UnitTest current;
		UnitTestNode* next;
		const char* name;
	};

	extern UnitTestNode* next_test;

	void execute_tests();

	namespace Asserts
	{
		void fail();
		void fail(const char* msg);
		void failStr(const char* msg, const char* expected, const char* got);
		//void failBool(const char* msg, bool expected, bool got);
		void assertTrue(const char* msg, bool value);
		void assertFalse(const char* msg, bool value);
	}
}

#ifdef TESTING
#define TEST(NAME, x) \
	static void _unitTest() { \
		using namespace ::Test::Asserts; \
		x \
	} \
	__attribute__((constructor)) static void _setupUnitTest() \
	{ \
		static Test::UnitTestNode node = { \
				.current = _unitTest, \
				.next = Test::next_test, \
				.name = NAME\
		}; \
		Test::next_test = &node; \
	}
#else

#define TEST(x) \
		;

#endif

#endif
