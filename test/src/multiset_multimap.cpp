/**
* 实例chapter1606
* 源文件chapter1606.cpp
* multiset、multimap容器基本操作实例
*/
#include <iostream>
#include <map>
#include <set>
#include <string>
#include "multiset_multimap.h"
using namespace std;
void test_map()
{
	cout << sizeof(string) << endl;
	map<int,string> testMap;
	cout << sizeof(map<int,string>) << endl;
	cout << sizeof(testMap) << endl;
	for(int i = 0;i < 5;i++)
	{
		testMap.insert(map<int,string>::value_type(i,"aaa")); //通过insert()向testMap容器插入元素值
	}
	cout << sizeof(testMap) << endl;
	cout << testMap[0] << endl;
	for (auto ite = testMap.begin();ite != testMap.end();++ite)
	{
		cout << ite->first << ite->second << endl;
	}
}
int multiset_multimap()
{
	test_map();
	//第一部分代码
	multiset<int> testSet;//定义整型类型集合容器testSet
	multimap<int,char> testMap; //定义整型和字符型键值对的map容器testMap
	multiset<int>::iterator iter1; //定义集合容器迭代器iter1
	multimap<int,char>::iterator iter2; //定义map容器迭代器iter2
	for(int i = 0;i < 5;i++) //循环结构控制，变量i从0到值5，循环5次
	{
		testSet.insert(i); //通过insert()方法插入元素i进入集合testSet容器
		testMap.insert(multimap<int,char>::value_type(i,65+i)); //通过insert()向testMap容器插入元素值
	}
	cout<<"mulitiset elements:"; //提示打印输出容器mulitiset元素
	for(iter1 = testSet.begin();iter1 != testSet.end();iter1++) //通过for循环遍历容器元素
	{
		cout<<*iter1<<" "; //输出迭代器指向元素值
	}
	cout<<endl;
	//第二部分代码
	cout<<"mulitimap elements:"<<endl; //提示打印输出容器mulitimap元素
	for(iter2 = testMap.begin();iter2 != testMap.end();iter2++) //通过for循环遍历容器元素
	{
		cout<<iter2->first<<""<<iter2->second<<endl; //输出迭代器指向的元素值
	}
	iter1 = testSet.begin(); //通过begin()方法获取容器testSet的首元素位置赋值给iter1指向
	testSet.insert(iter1,1); //通过insert()方法在iter1指定位置处插入元素值1
	cout<<"mulitiset elements:"; //提示打印输出容器testSet元素
	for(iter1 = testSet.begin();iter1 != testSet.end();iter1++) //通过for循环遍历容器元素
	{
		cout<<*iter1<<" "; //输出迭代器指向的元素值
	}
	cout<<endl;
	testMap.insert(multimap<int,char>::value_type(1,65+3)); //通过insert()方法向容器testMap中插入元素值
	testMap.insert(multimap<int,char>::value_type(1,65+3));
	cout<<"mulitimap elements:"<<endl; //提示打印输出容器元素内容
	for(iter2 = testMap.begin();iter2 != testMap.end();iter2++) //通过for循环遍历容器元素
	{
		cout<<iter2->first<<""<<iter2->second<<endl; //输出迭代器指向的元素值
	}

	pair<multimap<int,char>::iterator,multimap<int,char>::iterator> pair_iter = testMap.equal_range(1);
	for (multimap<int,char>::iterator temp_iter = pair_iter.first;temp_iter!=pair_iter.second;temp_iter++)
	{
		cout<<temp_iter->first<<""<<temp_iter->second<<endl; //输出迭代器指向的元素值
	}

	testSet.clear(); //通过clear()方法清空容器testSet
	testMap.clear(); //通过clear()方法清空容器testMap
	return 0;
}