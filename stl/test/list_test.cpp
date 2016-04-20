#include "list_test.h"

namespace mystl
{
	namespace list_test
	{
		void test_case1()
		{
			std_list<int> l1(10, 0);
			my_list<int> l2(10, 0);
			assert(mystl::test::container_equal(l1, l2));

			int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			std_list<int> l3(std::begin(arr), std::end(arr));
			my_list<int> l4(std::begin(arr), std::end(arr));
			assert(mystl::test::container_equal(l3, l4));

			auto l5(l1);
			auto l6(l2);
			assert(mystl::test::container_equal(l5, l6));

			auto l7 = l1;
			auto l8 = l2;
			assert(mystl::test::container_equal(l7, l8));
		}

		void test_case2()
		{
			my_list<std::string> l1;
			assert(l1.empty());
			assert(l1.size() == 0);

			std::string arr[] = { "1", "2", "3" };
			my_list<std::string> l2(std::begin(arr), std::end(arr));
			assert(!l2.empty());
			assert(l2.size() == 3);
		}

		void test_case3()
		{
			std::string arr[] = { "1", "2", "3" };
			my_list<std::string> l(std::begin(arr), std::end(arr));
			assert(l.front() == arr[0]);
			assert(l.back() == arr[2]);

			l.front() = "front";
			l.back() = "back";
			assert(l.front() == "front");
			assert(l.back() == "back");
		}

		void test_case4()
		{
			std_list<int> l1;
			my_list<int> l2;
			for (int i = 0; i != 10; ++i)
			{
				l1.push_front(i);
				l2.push_front(i);
			}
			assert(mystl::test::container_equal(l1, l2));
			for (int i = 0; i != 10; ++i)
			{
				l1.push_back(i);
				l2.push_back(i);
			}
			assert(mystl::test::container_equal(l1, l2));
			for (int i = 0; i != 5; ++i)
			{
				l1.pop_back();
				l2.pop_back();
			}
			assert(mystl::test::container_equal(l1, l2));
			for (int i = 0; i != 5; ++i)
			{
				l1.pop_front();
				l2.pop_front();
			}
			assert(mystl::test::container_equal(l1, l2));
		}

		void test_case5()
		{
			std_list<int> l1;
			my_list<int> l2;
			for (int i = 0; i != 10; ++i)
			{
				l1.push_back(i);
				l2.push_back(i);
			}
			auto rit1 = l1.rbegin();
			auto rit2 = l2.rbegin();
			for (; rit1 != l1.rend() && rit2 != l2.rend(); ++rit1, ++rit2)
			{
				assert(*rit1 == *rit2);
			}
			assert(rit1 == l1.rend() && rit2 == l2.rend());
		}

		void test_case6()
		{
			std_list<int> l1;
			my_list<int> l2;
			l1.insert(l1.end(), 10, -1);
			l2.insert(l2.end(), 10, -1);
			assert(mystl::test::container_equal(l1, l2));

			auto it1 = l1.begin();
			auto it2 = l2.begin();
			for (int i = l1.size() / 2 + 1; i != 0; --i)
			{
				++it1;
				++it2;
			}
			l1.insert(it1, 1);
			l2.insert(it2, 1);
			assert(mystl::test::container_equal(l1, l2));

			int arr[] = { 1, 2, 3, 4, 5 };
			it1 = l1.end();
			it2 = l2.end();
			l1.insert(it1, std::begin(arr), std::end(arr));
			l2.insert(it2, std::begin(arr), std::end(arr));
			assert(mystl::test::container_equal(l1, l2));
		}

		void test_case7()
		{
			std_list<int> l1;
			my_list<int> l2;
			for (int i = 0; i != 100; ++i)
			{
				l1.push_back(i);
				l2.push_back(i);
			}
			l1.erase(l1.begin()); l1.erase(--l1.end());
			l2.erase(l2.begin()); l2.erase(--l2.end());
			assert(mystl::test::container_equal(l1, l2));

			l1.erase(l1.begin(), l1.end());
			l2.erase(l2.begin(), l2.end());
			assert(mystl::test::container_equal(l1, l2));
		}

		void test_case8()
		{
			my_list<int> l1, l2;
			l1.push_back(1); l1.push_back(1); l1.push_back(1);
			l2.push_back(2); l2.push_back(2);
			
			l1.swap(l2);
			assert(l1.size() == 2 && l2.size() == 3);
		}

		void test_case9()
		{
			std::random_device rd;
			std_list<int> l1;
			my_list<int> l2;
			for (int i = 0; i != 10; ++i)
			{
				auto ret = rd() % 65536;
				l1.push_back(ret);
				l2.push_back(ret);
			}

			l1.sort(); 
			l2.sort();
			assert(mystl::test::container_equal(l1, l2));

			l1.sort(std::greater<int>());
			l2.sort(std::greater<int>());
			assert(mystl::test::container_equal(l1, l2));
		}

		void test_case10()
		{
			int arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			std_list<int> l1(std::begin(arr), std::end(arr));
			my_list<int> l2(std::begin(arr), std::end(arr));

			l1.reverse();
			l2.reverse();
			assert(mystl::test::container_equal(l1, l2));
		}

		void test_case11()
		{
			int arr1[] = { 1, 3, 5, 7, 9 }, arr2[] = { 0, 2, 4, 6, 8 };
			my_list<int> first1(std::begin(arr1), std::end(arr1)), second1(std::begin(arr2), std::end(arr2));
			std_list<int> first2(std::begin(arr1), std::end(arr1)), second2(std::begin(arr2), std::end(arr2));

			first1.merge(second1);
			first2.merge(second2);
			assert(mystl::test::container_equal(first1, first2));
		}

		void test_case12()
		{
			int arr[] = { 0, 0, 1, 2, 2, 3, 3, 4, 4, 4, 5, 5, 6, 7, 8, 8, 9, 11 };
			std_list<int> l1(std::begin(arr), std::end(arr));
			my_list<int> l2(std::begin(arr), std::end(arr));

			l1.unique();
			l2.unique();
			assert(mystl::test::container_equal(l1, l2));
		}

		void test_case13()
		{
			int arr[] = { 17, 89, 7, 14, 89, 0, 1, 4 };
			std_list<int> l1(std::begin(arr), std::end(arr));
			my_list<int> l2(std::begin(arr), std::end(arr));

			l1.remove(89);
			l2.remove(89);
			assert(mystl::test::container_equal(l1, l2));

			auto f = [](int n) { return n % 2 == 0; };
			l1.remove_if(f);
			l2.remove_if(f);
			assert(mystl::test::container_equal(l1, l2));
		}

		void test_case14()
		{
			std_list<int> l1(10, 0), l3(10, 1);
			my_list<int> l2(10, 0), l4(10, 1);

			l1.splice(l1.begin(), l3);
			l2.splice(l2.begin(), l4);
			assert(mystl::test::container_equal(l1, l2));

			auto l5 = l1;
			auto l6 = l2;
			l1.splice(l1.end(), l5, l5.begin());
			l2.splice(l2.end(), l6, l6.begin());
			assert(mystl::test::container_equal(l1, l2));

			auto it1 = l1.begin();
			auto it2 = l2.begin();
			for (auto i = 0; i != l1.size() / 2; ++i)
			{
				++it1;
				++it2;
			}
			l1.splice(it1, l5, l5.begin(), l5.end());
			l2.splice(it2, l6, l6.begin(), l6.end());
			assert(mystl::test::container_equal(l1, l2));
		}

		void test_case15()
		{
			my_list<int> l1(10, 1), l2(10, 0), l3(10, 1);

			assert(l1 == l3);
			assert(l1 != l2);
		}

		void test_all_case()
		{
			test_case1();
			test_case2();
			test_case3();
			test_case4();
			test_case5();
			test_case6();
			test_case7();
			test_case8();
			test_case9();
			test_case10();
			test_case11();
			test_case12();
			test_case13();
			test_case14();
			test_case15();
		}
	}
}