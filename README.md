# My-STL
简单的 STL，采用 C++ 11 实现。

目的：在于掌握 STL 以及 C++模版编程。
编译环境：VS2013。

背景：
---------
其实在《C++ Primer》这本书中，用了大半本书的内容在教你怎样实现一个 vector，也提到了智能指针的实现。因此自己实现一个简单的 STL非常有必要，也能够加深对于基础知识的理解。

举个小例子，我在 vector 的拷贝、移动构造函数和拷贝、移动赋值运算符中加了一条输出语句，因此当我运行下面的代码时：
```cpp
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
```

结果会是这样：
！[image](https://github.com/huangmingchuan/My-STL/raw/master/pic/pic1.png)

那么什么是拷贝构造、什么是移动构造，什么是拷贝赋值、什么是移动赋值就自然而然理解了。

内容：
-------------
####目前已实现的有：

* STL 基本组件
	* 空间配置器
	* reverse_iterator
	* iterator_traits
	* vector
	* list

性能测试：
-------------
待续...