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

		void test_case3()
		{
			my_vector<int> v1, v2;
			for (int i = 0; i != 100; ++i)
			{
				v1.push_back(i);
				v2.push_back(i);
			}
			assert(v1 == v2);
			assert(!(v1 != v2));
		}

		void test_case4()
		{
			my_vector<int> vec;
			for (int i = 1; i <= 5; ++i) vec.push_back(i);
			auto i = 1;
			for (my_vector<int>::iterator it = vec.begin(); it != vec.end(); ++it, ++i)
			{
				assert(*it == i);
			}
			i = 1;
			for (my_vector<int>::const_iterator it = vec.cbegin(); it != vec.cend(); ++it, ++i)
			{
				assert(*it == i);
			}
		}

		void test_case5()
		{
			my_vector<int> vecs(5);
			int i = 0;
			my_vector<int>::reverse_iterator rit = vecs.rbegin();
			for (; rit != vecs.rend(); ++rit)
			{
				*rit = ++i;
			}
			i = 5;
			for (my_vector<int>::iterator it = vecs.begin(); it != vecs.end(); ++it, --i)
			{
				assert(*it == i);
			}
			i = 1;
			for (my_vector<int>::reverse_iterator rit = vecs.rbegin(); rit != vecs.rend(); ++rit, ++i)
			{
				assert(*rit == i);
			}
			i = 5;
			for (auto it : vecs)
			{
				assert(it == i--);
			}
		}

		void test_case6()
		{
			my_vector<int> v(11, 0);
			assert(v.size() == 11);

			v.resize(5);
			assert(v.size() == 5);

			v.resize(20);
			auto i = v.size();
			assert(i == 20);
		}

		void test_case7()
		{
			my_vector<int> v(10,1);
			assert(v.size() == 10);
			assert(v.capacity() == 10);
			v.reserve(20);
			assert(v.size() == 10);
			assert(v.capacity() == 20);
		}

		void test_case8()
		{
			std_vector<int> v1(10);
			my_vector<int> v2(10);
			for (unsigned i = 0; i < 10; ++i)
			{
				v1[i] = i;
				v2[i] = i;
			}
			assert(mystl::test::container_equal(v1, v2));
			v1.front() = 99;
			v2.front() = 99;
			v1.back() = 100;
			v2.back() = 100;
			assert(mystl::test::container_equal(v1, v2));
		}

		void test_case9()
		{
			std_vector<int> v1(5);
			my_vector<int> v2(5);

			auto p1 = v1.data();
			auto p2 = v2.data();
			*p1 = 10; ++p1; *p1 = 20; p1[2] = 100;
			*p2 = 10; ++p2; *p2 = 20; p2[2] = 100;

			assert(mystl::test::container_equal(v1, v2));
		}

		void test_case10()
		{
			my_vector<int> foo(3, 100);
			my_vector<int> bar(2, 200);
			assert(mystl::test::container_equal(foo, my_vector<int>{ 100, 100, 100 }));
			assert(mystl::test::container_equal(bar, my_vector<int>{ 200, 200 }));
			foo.swap(bar);
			assert(mystl::test::container_equal(bar, my_vector<int>{ 100, 100, 100 }));
			assert(mystl::test::container_equal(foo, my_vector<int>{ 200, 200 }));
		}

		void test_case11()
		{
			std_vector<std::string> v1;
			my_vector<std::string> v2;
			v1.push_back("hello "); v1.push_back("world");
			v2.push_back("hello "); v2.push_back("world");
			assert(mystl::test::container_equal(v1, v2));
			v1.pop_back();
			v2.pop_back();
			assert(mystl::test::container_equal(v1, v2));
		}

		void test_case12()
		{
			std_vector<int> v1;
			my_vector<int> v2;

			v1.insert(v1.begin(), 0);
			v2.insert(v2.begin(), 0);
			assert(mystl::test::container_equal(v1, v2));

			v1.insert(v1.end(), 1);
			v2.insert(v2.end(), 1);
			assert(mystl::test::container_equal(v1, v2));

			v1.insert(v1.begin() + v1.size() / 2, 10, 0);
			v2.insert(v2.begin() + v2.size() / 2, 10, 0);
			assert(mystl::test::container_equal(v1, v2));

			int arr[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
			v1.insert(v1.end(), std::begin(arr), std::end(arr));
			v2.insert(v2.end(), std::begin(arr), std::end(arr));
			assert(mystl::test::container_equal(v1, v2));
		}

		void test_case13()
		{
			std_vector<int> v1;
			my_vector<int> v2;
			for (int i = 1; i <= 10; i++)
			{
				v1.push_back(i);
				v2.push_back(i);
			}
			v1.erase(v1.begin() + 5);
			v2.erase(v2.begin() + 5);
			assert(mystl::test::container_equal(v1, v2));

			v1.erase(v1.begin(), v1.begin() + 3);
			v2.erase(v2.begin(), v2.begin() + 3);
			assert(mystl::test::container_equal(v1, v2));
		}

		void test_case14()
		{
			my_vector<int> foo(3, 20);
			my_vector<int> bar(2, 30);

			assert(!(foo == bar));
			assert(foo != bar);
		}

		class TestItem
		{
		public:
			TestItem()
			{
				++count;
			}
			TestItem(const TestItem & other)
			{
				++count;
			}

			virtual ~TestItem()
			{
				--count;
			}

			static int getCount()
			{
				return count;
			}
		private:
			static int count;
		};
		int TestItem::count = 0;

		void test_case15()
		{
			assert(TestItem::getCount() == 0);
			{
				typedef mystl::vector<TestItem> TVector;
				TVector t(10);
				t.push_back(TestItem());
				t.push_back(TestItem());
				t.push_back(TestItem());
				t.insert(t.begin(), t.begin(), t.begin() + 1);
			}
			assert(TestItem::getCount() == 0);
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