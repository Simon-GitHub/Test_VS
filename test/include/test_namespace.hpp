#pragma once
#include <iostream>

using namespace std;

void test1()
{
	cout << "test1 out NS1" << endl;
}

namespace NS1
{
	void test1()
	{
		cout << "test1 in NS1" << endl;
	}

	void test2()
	{
		::test1();
		test1();
	}
}