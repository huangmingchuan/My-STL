#include <iostream>
#include <memory>

#include "src\allocator.h"
#include "src\construct.h"
#include "src\uninitialized_functions.h"

using namespace std;

int main()
{
	auto p = mystl::allocator<int>::allocate(100);
	auto last = p + 100;
	//mystl::uninitialized_fill(p, p + 100, 88);
	for (int i = 0; i != 100; ++i)
	{
		mystl::allocator<int>::construct(p + i, i);
	}
	for (; p != last; ++p)
		cout << *p << ", ";
	cout << endl;

	int array[100];
	mystl::uninitialized_fill_n(array, 100, 99);
	for (auto n : array)
		cout << n << ", ";
	return 0;
}