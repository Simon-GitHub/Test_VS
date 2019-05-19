/*
	C++ Primer 第12章 动态内存
	测试C++标准库中提供的智能指针shared_ptr
*/

#include <memory>

void test_shared_ptr()
{
	// 测试智能指针的引用计数
	auto r = make_shared<int>(42);
	int rc = r.use_count(); // 1
	auto q = make_shared<int>(50);
	int qc = q.use_count(); // 1
	r = q;
	int rc2 = r.use_count(); // 2
	int qc2 = q.use_count(); // 2
}