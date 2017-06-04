#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

// 注1：在用STL模版库的时候头文件不要加.h，要用命名空间



void show_sequence(std::vector<int>::const_iterator first, std::vector<int>::const_iterator last)

	// 注2：const_iterator为 vector 中元素的一个 const 迭代器类型，这个类型定义为指向 const T 的迭代器类型

{
	std::cout<<"{";
	std::copy(first, last, std::ostream_iterator<int>(std::cout, " "));

	// 注3：copy()的前两个参数是迭代器，它们定义了要复制元素的容器中的源间隔符，复制从 first 开始，一直向前，直到 last 为止，但不包括 last.这两个参数可以接受任何迭代器。copy()函数的第三个参数是指定元素复制的目标位置的迭代器。

	// 注4：std::ostream_iterator<int>(std::cout, " ")作用为：创建输出迭代器。第一个参数 cout 是应用这个迭代器的流，第二个参数是用于分隔所传送元素的分隔符。

	// 注5：copy()算法把每个源元素赋予目标对象，但并不知道它处理的目标对象是什么类型。copy()函数仅把要复制的对象传送给第三个参数指定的迭代器，并假定迭代器知道应把他们放在什么地方。这样，copy()函数就独立于源对象和它复制的目标对象。
	std::cout<<"}"<<std::endl;

}
void inspect_vector(const std::vector<int>& v)

	// 注6：因为这里用的是 const  类型，所以这个函数调用的上一个函数接收时必须要用 const_iterator 

	//如果这里用的不是 const  类型，那么上面就可以直接用 iterator 了

{
	std::cout<<"vector has "<<v.size()<<" elements: ";
	show_sequence(v.begin(), v.end());
}
int test_ostream_iterator()
{
	std::vector<int> v;  //现在容器中有0个元素

	int values[] = {1,3,5,7};
	v.insert(v.end(), values+1, values+3); //现在容器中有2个元素分别为:3,5

	inspect_vector(v);

	return 0;
}
