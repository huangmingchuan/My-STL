#include "test\vector_test.h"
#include "test\list_test.h"

int main()
{
	mystl::vector_test::test_all_case();
	mystl::list_test::test_all_case();

	system("pause");
	return 0;
}