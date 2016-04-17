#ifndef _VECTOR_TEST_H_
#define _VECTOR_TEST_H_

#include "../src/vector.h"
#include "test_util.h"

#include <vector>
#include <array>
#include <cassert>
#include <iostream>
#include <iterator>
#include <string>

namespace mystl
{
	namespace vector_test
	{
		template <typename T>
		using std_vector = std::vector<T>;
		
		template <typename T>
		using my_vector = mystl::vector<T>;

		void test_case1();
		void test_case2();

		void test_all_case();
	}
}

#endif