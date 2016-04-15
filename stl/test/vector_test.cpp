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
		}

		void test_all_case()
		{
			test_case1();
		}
	}
}