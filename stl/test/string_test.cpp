#include "string_test.h"

namespace mystl
{
	namespace string_test
	{
		void test_case1()
		{
			const char* ptr = "hello world";
			std_string s1(ptr);
			my_string s2(ptr);
			assert(mystl::test::container_equal(s1, s2));

			std_string s3(ptr, 5);
			my_string s4(ptr, 5);
			assert(mystl::test::container_equal(s3, s4));

			std_string s5(10, 'z');
			my_string s6(10, 'z');
			assert(mystl::test::container_equal(s5, s6));

			char arr[] = "abcdef";
			std_string s7(std::begin(arr), std::end(arr));
			my_string s8(std::begin(arr), std::end(arr));
			assert(mystl::test::container_equal(s7, s8));
		}

		void test_case2()
		{
			std_string temp1("hello world");
			my_string temp2("hello world");

			std_string s1(temp1);
			my_string s2(temp2);
			assert(mystl::test::container_equal(s1, s2));

			std_string s3(std::move(s1));
			my_string s4(std::move(s2));
			assert(mystl::test::container_equal(s3, s4));

			std_string s5(temp1, 1);
			my_string s6(temp2, 1);
			assert(mystl::test::container_equal(s5, s6));

			std_string s7(temp1, 0, 5);
			my_string s8(temp2, 0, 5);
			assert(mystl::test::container_equal(s7, s8));
		}

		void test_all_case()
		{
			test_case1();
			test_case2();
		}
	}
}