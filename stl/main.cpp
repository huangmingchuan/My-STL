#include "test\vector_test.h"
#include "test\list_test.h"
#include "test\string_test.h"
#include "test\memory_test.h"

int main()
{
	mystl::vector_test::test_all_case();
	mystl::list_test::test_all_case();
	mystl::string_test::test_all_case();
	mystl::memory_test::test_all_case();

	system("pause");
	return 0;
}