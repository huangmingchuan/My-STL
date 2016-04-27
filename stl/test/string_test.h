#ifndef _STRING_TEST_H_
#define _STRING_TEST_H_

#include "test_util.h"
#include "../src/string.h"

#include <string>
#include <iterator>
#include <cassert>
#include <vector>

namespace mystl
{
	namespace string_test
	{
		using std_string = std::string;

		using my_string = mystl::string;
			
		void test_all_case();
	}
}

#endif 
