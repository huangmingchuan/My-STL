#include "string_test.h"

#include <fstream>

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

		void test_case3() 
		{
			std_string t1("hello world");
			my_string t2("hello world");

			std_string s1; s1 = 'a';
			my_string s2; s2 = 'a';
			assert(mystl::test::container_equal(s1, s2));

			std_string s3; s3 = "hello";
			my_string s4; s4 = "hello";
			assert(mystl::test::container_equal(s3, s4));

			std_string s5; s5 = t1;
			my_string s6; s6 = t2;
			assert(mystl::test::container_equal(s5, s6));

			std_string s7; s7 = std::move(t1);
			my_string s8; s8 = std::move(t2);
			assert(mystl::test::container_equal(s7, s8));
		}

		void test_case4()
		{
			my_string str("test string");
			std_string s(str.begin(), str.end());
			auto i = 0;
			for (auto it = str.begin(); it != str.end(); ++it, ++i)
			{
				assert(*it == s[i]);
			}

			i = s.size() - 1;
			for (auto it = str.rbegin(); it != str.rend(); ++it, --i)
			{
				assert(*it == s[i]);
			}
		}

		void test_case5()
		{
			my_string s;
			assert(s.size() == 0);
			assert(s.length() == 0);

			s = "hello world";
			assert(s.size() == 11);
			assert(s.length() == 11);
		}

		void test_case6()
		{
			std_string s1("hello world");
			my_string s2("hello world");

			s1.resize(5);
			s2.resize(5);
			assert(mystl::test::container_equal(s1, s2));

			s1.resize(20, 'z');
			s2.resize(20, 'z');
			assert(mystl::test::container_equal(s1, s2));

			s1.resize(6, 'a');
			s2.resize(6, 'a');
			assert(mystl::test::container_equal(s1, s2));

			s1.resize(100);
			s2.resize(100);
			assert(mystl::test::container_equal(s1, s2));
		}

		void test_case7()
		{
			my_string s;
			s.reserve(10);
			assert(s.size() == 0);
			assert(s.capacity() == 10);
		}

		void test_case8()
		{
			my_string s;
			assert(s.empty());

			s = "hello world";
			assert(!s.empty());

			s.clear();
			assert(s.empty());
		}

		void test_case9()
		{
			my_string s;
			s.resize(10);
			for (auto i = 0; i != s.size(); ++i)
			{
				s[i] = 'a' + i;
			}
			assert(s == "abcdefghij");

			s.back() = 'Z';
			s.front() = 'A';
			assert(s == "AbcdefghiZ");
		}

		void test_case10()
		{
			std_string s1;
			my_string s2;
			for (auto i = 0; i != 10; ++i)
			{
				s1.push_back('a' + i);
				s2.push_back('a' + i);
			}
			assert(mystl::test::container_equal(s1, s2));
		}

		void test_case11()
		{
			std_string s1;
			my_string s2;
			s1.insert(s1.begin(), 'A');
			s2.insert(s2.begin(), 'A');
			assert(mystl::test::container_equal(s1, s2));

			s1.insert(s1.end(), 2, 'Z');
			s2.insert(s2.end(), 2, 'Z');
			assert(mystl::test::container_equal(s1, s2));

			size_t n = 2;
			s1.insert(2, 10, '@');
			s2.insert(2, 10, '@');
			assert(mystl::test::container_equal(s1, s2));

			s1.insert(0, "hello, world");
			s2.insert(0, "hello, world");
			assert(mystl::test::container_equal(s1, s2));

			s1.insert(s1.size() - 1, "hello", 3);
			s2.insert(s2.size() - 1, "hello", 3);
			assert(mystl::test::container_equal(s1, s2));

			std_string s3;
			my_string s4;

			s3.insert(s3.begin(), s1.begin(), s1.end());
			s4.insert(s4.begin(), s2.begin(), s2.end());
			assert(mystl::test::container_equal(s3, s4));

			s3.insert(1, s1);
			s4.insert(1, s2);
			assert(mystl::test::container_equal(s3, s4));

			std_string t1("hellohellohello");
			my_string t2("hellohellohello");
			s3.insert(s3.size(), t1, 7, t1.size() - 7);
			s4.insert(s4.size(), t2, 7, t2.size() - 7);
			assert(mystl::test::container_equal(s3, s4));
		}

		void test_case12()
		{
			std_string s1;
			my_string s2;

			s1.append(std_string("abc"));
			s2.append(my_string("abc"));
			assert(mystl::test::container_equal(s1, s2));

			s1.append(std_string("123456789"), 1, 3);
			s2.append(my_string("123456789"), 1, 3);
			assert(mystl::test::container_equal(s1, s2));

			
			s1.append("hello");
			s2.append("hello");
			assert(mystl::test::container_equal(s1, s2));

			s1.append("world", 0, 5);
			s2.append("world", 0, 5);
			assert(mystl::test::container_equal(s1, s2));

			s1.append(10, 'A');
			s2.append(10, 'A');
			assert(mystl::test::container_equal(s1, s2));

			std_string s3; s3.append(s1.begin(), s1.end());
			my_string s4; s4.append(s2.begin(), s2.end());
			assert(mystl::test::container_equal(s3, s4));
		}

		void test_case13()
		{
			std_string s1;
			my_string s2;

			s1 += 'A';
			s2 += 'A';
			assert(mystl::test::container_equal(s1, s2));

			s1 += "hello";
			s2 += "hello";
			assert(mystl::test::container_equal(s1, s2));

			s1 += std_string("world");
			s2 += my_string("world");
			assert(mystl::test::container_equal(s1, s2));
		}

		void test_case14()
		{
			std_string s1("hello world");
			my_string s2("hello world");

			s1.pop_back();
			s2.pop_back();
			assert(mystl::test::container_equal(s1, s2));
		}

		void test_case15()
		{
			std_string s1("hello world");
			my_string s2("hello world");

			s1.erase(s1.begin() + 1);
			s2.erase(s2.begin() + 1);
			assert(mystl::test::container_equal(s1, s2));

			s1.erase(2, s1.size() - 4);
			s2.erase(2, s2.size() - 4);
			assert(mystl::test::container_equal(s1, s2));

			s1.erase(s1.begin(), s1.end());
			s2.erase(s2.begin(), s2.end());
			assert(mystl::test::container_equal(s1, s2));
		}
		
		void test_case16()
		{
			std_string s1("hellohellohello"), t1("worldworldworld");
			my_string s2("hellohellohello"), t2("worldworldworld");

			s1.replace(0, 3, t1);
			s2.replace(0, 3, t2);
			assert(mystl::test::container_equal(s1, s2));

			s1.replace(s1.begin(), s1.begin() + s1.size() / 2, t1);
			s2.replace(s2.begin(), s2.begin() + s2.size() / 2, t2);
			assert(mystl::test::container_equal(s1, s2));

			s1.replace(0, s1.size(), t1, 0, t1.size());
			s2.replace(0, s2.size(), t2, 0, t2.size());
			assert(mystl::test::container_equal(s1, s2));

			s1.replace(0, s1.size(), "123456789");
			s2.replace(0, s2.size(), "123456789");
			assert(mystl::test::container_equal(s1, s2));

			s1.replace(s1.begin(), s1.end(), std_string("hello"));
			s2.replace(s2.begin(), s2.end(), my_string("hello"));
			assert(mystl::test::container_equal(s1, s2));

			s1.replace(s1.begin(), s1.end(), "hongshanqu", 10);
			s2.replace(s2.begin(), s2.end(), "hongshanqu", 10);
			assert(mystl::test::container_equal(s1, s2));

			s1.replace(0, s1.size(), 10, 'Z');
			s2.replace(0, s2.size(), 10, 'Z');
			assert(mystl::test::container_equal(s1, s2));

			s1.replace(s1.begin(), s1.end(), 10, 'A');
			s2.replace(s2.begin(), s2.end(), 10, 'A');
			assert(mystl::test::container_equal(s1, s2));

			s1.replace(s1.begin(), s1.end(), t1.begin(), t1.end());
			s2.replace(s2.begin(), s2.end(), t2.begin(), t2.end());
			assert(mystl::test::container_equal(s1, s2));
		}

		void test_case17()
		{
			my_string s1("hello");
			my_string s2("world");

			s1.swap(s2);
			assert(s1 == "world");
			assert(s2 == "hello");
		}

		void test_case18()
		{
			char buff[20];
			my_string str("test string...");
			std::size_t length = str.copy(buff, 6, 5);
			buff[length] = '\0';
			assert(my_string("string") == buff);
		}

		void test_case19()
		{
			my_string str("There are two needles in this haystack with needles.");
			my_string str2("needle");

			auto found = str.find(str2);
			assert(found == 14);


			found = str.find("needles are small", found + 1, 6);
			assert(found == 44);

			found = str.find(my_string("wuhan"));
			assert(found == my_string::npos);

			found = str.find("haystack");
			assert(found == 30);

			found = str.find('.');
			assert(found == 51);

			str.replace(str.find(str2), str2.length(), "preposition");
			assert(mystl::test::container_equal(str, my_string("There are two prepositions in this haystack with needles.")));
		}

		void test_case20()
		{
			my_string str("The sixth sick sheik's sixth sheep's sick.");
			my_string key("sixth");

			auto found = str.rfind(key);
			assert(found == 23);

			found = str.rfind(key, 24);
			assert(found == 23);

			found = str.rfind('.');
			assert(found == str.size() - 1);

			found = str.rfind("The");
			assert(found == 0);

			found = str.rfind("sick111", 10, 4);
			assert(found == 10);
		}

		void test_case21()
		{
			my_string str("Please, replace the vowels in this sentence by asterisks.");
			my_string key("aeiou");
			const char *arr = "aeiou";

			auto found = str.find_first_of(arr);
			assert(found == 2);

			found = str.find_first_of(arr, found + 1);
			assert(found == 3);

			found = str.find_first_of(arr, found + 1, 1);
			assert(found == 12);

			found = str.find_first_of(key, found + 1);
			assert(found == 14);

			found = str.find_first_of('v', found + 1);
			assert(found == 20);
		}

		void test_case22()
		{
			my_string str("1234567890098765432112345678900");

			auto found = str.find_last_of('6');
			assert(found == 25);

			found = str.find_last_of('6', found - 1);
			assert(found == 14);

			found = str.find_last_of("01", 11, 2);
			assert(found == 10);

			found = str.find_last_of(my_string("#1"), 19);
			assert(found == 19);
		}

		void test_case23()
		{
			my_string str("look for non-alphabetic characters...");

			auto found = str.find_first_not_of("abcdefghijklmnopqrstuvwxyz ");
			assert(found == 12);
		}

		void test_case24()
		{
			my_string str("1234567890-0987654321");

			auto found = str.find_last_not_of("023456789", str.size() - 2);
			assert(found == 10);
		}

		void test_case25()
		{
			my_string str = "We think in generalities, but we live in details.";

			auto s = str.substr(3, 5);
			assert(mystl::test::container_equal(s, my_string("think")));
		}

		void test_case26()
		{
			my_string str1("green apple");
			my_string str2("red apple");

			assert(str1.compare(str2) != 0);
			assert(str1.compare(6, 5, "apple") == 0);
			assert(str2.compare(str2.size() - 5, 5, "apple") == 0);
			assert(str1.compare(6, 5, str2, 4, 5) == 0);
		}

		void test_case27()
		{
			my_string firstlevel("com");
			my_string secondlevel("cplusplus");
			my_string scheme("http://");

			auto hostname = "www." + secondlevel + '.' + firstlevel;
			auto url = scheme + hostname;

			assert(mystl::test::container_equal(url, my_string("http://www.cplusplus.com")));
		}

		void test_case28()
		{
			my_string foo = "alpha";
			my_string bar = "beta";

			assert(!(foo == bar));
			assert(foo != bar);
			assert(foo < bar);
			assert(!(foo > bar));
			assert(foo <= bar);
			assert(!(foo >= bar));
		}

		void test_case29()
		{
			my_string name;
			std::ifstream in("test/string_test.txt");

			if (in)
			{
				in >> name;
				assert(name == "huangmingchuan");
				in.close();
			}
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
			test_case16();
			test_case17();
			test_case18();
			test_case19();
			test_case20();
			test_case21();
			test_case22();
			test_case23();
			test_case24();
			test_case25();
			test_case26();
			test_case27();
			test_case28();
			test_case29();
		}
	}
}