#include "vector_test.h"

namespace mystl
{
	namespace vector_test
	{
		void test_case1()
		{
			using namespace std;
			my_vector<string> v1(10, "hmc");
			std_vector<string> v2(10, "hmc");
			assert(mystl::test::container_equal(v1, v2));

			my_vector<string> v3(10);
			std_vector<string> v4(10);
			assert(mystl::test::container_equal(v3, v4));

			array<string, 3> arr = { "abc", "def", "ghi" };
			my_vector<string> v5(begin(arr), end(arr));
			std_vector<string> v6(begin(arr), end(arr));
			assert(mystl::test::container_equal(v5, v6));
		}

		void test_case2()
		{
			std_vector<int> temp1(10, 0);
			my_vector<int> temp2(10, 0);
			std::cout << "-------------- 1 -------------" << std::endl;
			auto v1(temp1);
			auto v2(temp2); 
			assert(mystl::test::container_equal(v1, v2));
			std::cout << "-------------- 2 -------------" << std::endl;
			auto v3(std::move(temp1));
			auto v4(std::move(temp2));
			assert(mystl::test::container_equal(v3, v4));
			std::cout << "-------------- 3 -------------" << std::endl;
			std_vector<int> v5;
			my_vector<int> v6;
			v5 = v1;
			v6 = v2;
			assert(mystl::test::container_equal(v5, v6));
			std::cout << "-------------- 4 -------------" << std::endl;
			std_vector<int> v7;
			my_vector<int> v8;
			v7 = std::move(v3);
			v8 = std::move(v4);
			assert(mystl::test::container_equal(v7, v8));
			std::cout << "------------------------------" << std::endl;
		}

		void test_all_case()
		{
			test_case1();
			test_case2();
		}
	}
}