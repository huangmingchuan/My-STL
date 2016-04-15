#ifndef _TEST_UTIL_H_
#define _TEST_UTIL_H_

#include <iostream>
#include <iterator>
#include <string>

namespace mystl
{
	namespace test
	{
		template <typename Container>
		void print_container(Container& container, const std::string& name = "")
		{
			std::cout << "Container " << name << ":";
			for (auto val : container)
			{
				std::cout << val << " ";
			}
			std::cout << std::endl;
		}

		template <typename Container1, typename Container2>
		bool container_equal(Container1& c1, Container2& c2)
		{
			auto first1 = std::begin(c1), last1 = std::end(c1);
			auto first2 = std::begin(c2), last2 = std::end(c2);
			for (; first1 != last1 && first2 != last2; ++first1, ++first2)
			{
				if (*first1 != *first2)
					return false;
			}
			return (first1 == last1 && first2 == last2);
		}
	}
}

#endif 