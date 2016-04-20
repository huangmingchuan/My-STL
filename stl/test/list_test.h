#ifndef _LIST_TEST_H_
#define _LIST_TEST_H_

#include "test_util.h"
#include "../src/list.h"

#include <list>
#include <cassert>
#include <functional>
#include <string>
#include <random>

namespace mystl
{
	namespace list_test
	{
		template <typename T>
		using std_list = std::list<T>;

		template <typename T>
		using my_list = mystl::list<T>;

		void test_all_case();
	}
}

#endif 