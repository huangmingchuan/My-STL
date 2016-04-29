#include "memory_test.h"
#include "../src/string.h"

namespace mystl
{
	namespace memory_test
	{
		void test_case1()
		{
			shared_ptr<int> sp1(new int(10));
			assert(*(sp1.get()) == 10);

			shared_ptr<int> sp2(new int(1), default_deleter<int>());
			assert(sp2.use_count() == 1);

			auto sp3(sp2);
			assert(sp3.use_count() == 2);
			{
				auto sp4 = sp2;
				assert(sp3.use_count() == 3);
				assert(sp4.use_count() == 3);

				assert(sp2.get() == sp3.get() && sp2.get() == sp4.get());
				assert(sp2 == sp3);
				assert(!(sp2 != sp4));
			}
			assert(sp3.use_count() == 2);

			shared_ptr<string> sp5(new string("hello"));
			assert(*sp5 == "hello");

			sp5->append(" world");
			assert(*sp5 == "hello world");

			auto sp6 = make_shared<string>(10, '0');
			assert(*sp6 == "0000000000");
		}

		template <typename T>
		struct Del
		{
			void operator()(T* p) { delete p; }
		};

		void test_case2()
		{
			unique_ptr<int> up1;
			assert(up1.get() == nullptr);

			unique_ptr<int> up2(new int(5));
			assert(up2.get() != nullptr);
			assert(*up2 == 5);

			unique_ptr<string, Del<string>> up3(new string("hello"), Del<string>());
			assert(up3.get() != nullptr);
			assert(*up3 == "hello");

			auto up4(std::move(up2));
			assert(*up4 == 5);
			assert(up2.get() == nullptr);

			unique_ptr<string, Del<string>> up5;
			up5 = std::move(up3);
			assert(*up5 == "hello");
			assert(up3.get() == nullptr);

			up5->append(" world");
			assert(*up5 == "hello world");

			auto p = up5.release();
			assert(*p == "hello world");
			assert(up5.get() == nullptr);

			unique_ptr<string, Del<string>> up6(p);
			assert(*up6 == "hello world");

			unique_ptr<string, Del<string>> up7(up6.release());
			assert(*up7 == "hello world");
			assert(up6.get() == nullptr);
		}

		void test_all_case()
		{
			test_case1();
			test_case2();
		}
	}
}