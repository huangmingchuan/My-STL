#include <iostream>
#include <memory>
#include <string>

#include "src\allocator.h"
#include "src\construct.h"
#include "src\uninitialized_functions.h"
#include "src\vector.h"

using namespace std;

int main()
{
	int array[3] = { 1, 2, 3 };
	mystl::vector<int> vec(array, array + 3);

	cout << *vec.begin() << endl;
	cout << *(++vec.begin()) << endl;
	cout << *(--vec.end()) << endl;
	cout << "------------------" << endl;

	mystl::vector<int> vec2(3, 9);
	cout << *vec2.begin() << endl;
	cout << *(++vec2.begin()) << endl;
	cout << *(--vec2.end()) << endl;

	mystl::vector<string> svec(10, "hello");
	for (auto s : svec) cout << s << endl;
	return 0;
}