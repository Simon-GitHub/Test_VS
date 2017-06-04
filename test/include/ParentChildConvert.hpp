#pragma once

#include <iostream>

class ParentTest
{
public:
	ParentTest(){}
	~ParentTest(){}
	virtual void test_p1()
	{
		std::cout << "test_p1" << std::endl;
	}
};

class ChildTest : public ParentTest
{
public:
	ChildTest(){}
	~ChildTest(){}
	void test_c1()
	{
		std::cout << "test_c1" << std::endl;
	}
};