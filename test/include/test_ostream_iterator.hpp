#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

// ע1������STLģ����ʱ��ͷ�ļ���Ҫ��.h��Ҫ�������ռ�



void show_sequence(std::vector<int>::const_iterator first, std::vector<int>::const_iterator last)

	// ע2��const_iteratorΪ vector ��Ԫ�ص�һ�� const ���������ͣ�������Ͷ���Ϊָ�� const T �ĵ���������

{
	std::cout<<"{";
	std::copy(first, last, std::ostream_iterator<int>(std::cout, " "));

	// ע3��copy()��ǰ���������ǵ����������Ƕ�����Ҫ����Ԫ�ص������е�Դ����������ƴ� first ��ʼ��һֱ��ǰ��ֱ�� last Ϊֹ���������� last.�������������Խ����κε�������copy()�����ĵ�����������ָ��Ԫ�ظ��Ƶ�Ŀ��λ�õĵ�������

	// ע4��std::ostream_iterator<int>(std::cout, " ")����Ϊ�������������������һ������ cout ��Ӧ������������������ڶ������������ڷָ�������Ԫ�صķָ�����

	// ע5��copy()�㷨��ÿ��ԴԪ�ظ���Ŀ����󣬵�����֪���������Ŀ�������ʲô���͡�copy()��������Ҫ���ƵĶ����͸�����������ָ���ĵ����������ٶ�������֪��Ӧ�����Ƿ���ʲô�ط���������copy()�����Ͷ�����Դ����������Ƶ�Ŀ�����
	std::cout<<"}"<<std::endl;

}
void inspect_vector(const std::vector<int>& v)

	// ע6����Ϊ�����õ��� const  ���ͣ���������������õ���һ����������ʱ����Ҫ�� const_iterator 

	//��������õĲ��� const  ���ͣ���ô����Ϳ���ֱ���� iterator ��

{
	std::cout<<"vector has "<<v.size()<<" elements: ";
	show_sequence(v.begin(), v.end());
}
int test_ostream_iterator()
{
	std::vector<int> v;  //������������0��Ԫ��

	int values[] = {1,3,5,7};
	v.insert(v.end(), values+1, values+3); //������������2��Ԫ�طֱ�Ϊ:3,5

	inspect_vector(v);

	return 0;
}
