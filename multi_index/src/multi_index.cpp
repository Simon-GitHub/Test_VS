#include "stdafx.h"

#include <list>
#include <string>
#include <iostream>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>

#include <boost/lambda/lambda.hpp>
/*
����:boost.Multi_Index���ʹ��
���ܣ�����std::map�����������ǿ����ö���ؼ�����������
������boost::multi_index::multi_index_containerΪ�����ڵ����ݽ��������ͼ
     ��ͬ�������ֶ�(Key)��Ӧ��ͬ����ͼ
	 boost::multi_index::multi_index_container�������Բ��ò�ͬ�ı�﷽ʽʵ��
	 ͬ���Ĺ��ܡ�
	 ����ֻ�о�һ��������������ӡ�ɾ�����޸ġ�������¼������ʾ�����룬����Ȥ�Ļ�
	 ����ͨ���ο������еĹ�����ַ�õ����ʹ��������ʽ�ﵽ���﹦�������֪ʶ��
������Windows 8.1 64bit��Ӣ�İ棩
	  Visual Studio 2013 Professional with SP1
	  boost 1.55
ע�⣺��д��template��صĴ���һ�������������ȷ�Ĵ���λ��
      ���Խ���ÿ���һ����������дһ��������������ĺ�����
���������ڣ�2014-5-10
Ӧ�÷�Χ�������Լ����ڴ����ݿ�
�ο����ϣ�http://www.boost.org/doc/libs/1_55_0/libs/multi_index/doc/index.html
*/

#pragma region ���� employee 
//ʹ�����ֿռ���Ϊ�˷�ֹ�����������Ƴ�ͻ
namespace kagula
{
	namespace datatype
	{
		//Ϊ�˺���Ĵ�����ͨ��id��name����ȡ��ͼ���������������ṹ
		struct id{};
		struct name{};
		//employee������ʾ�������б�Ľṹ
		struct employee
		{
			int          id;
			std::wstring name;
			employee(int id, const std::wstring& name) :id(id), name(name){}

			//����<����������ú�����Ϊid����ͼ����id�ֶε���������
			bool operator<(const employee& e)const{ return id<e.id; }
			//����<=�����������Ϊ�˵õ�ָ��id�ֶ�ֵ��Χ�ļ�¼
			bool operator<=(const employee& e)const{ return id<=e.id; }
			//����<<�����������Ϊ�����Ƿ����ӡ��¼������
			friend std::wostream& operator<<(std::wostream& os, const employee& dt);
		};
		std::wostream& operator<<(std::wostream& os, const employee& dt)
		{
			os << L"[" << dt.id << L',' << dt.name.c_str() << L"]";
			return os;
		}
		//����Ĵ��룬����ָ����¼name�ֶε�ֵҪ�õ�change_name��
		//ͨ�����ʾ�������֪������޸ķ������������ģ�ָ����¼�������ֶ�ֵ
		struct change_name
		{
			change_name(const std::wstring& new_name) :new_name(new_name){}

			void operator()(employee& e)
			{
				e.name = new_name;
			}

		private:
			std::wstring new_name;
		};

		struct change_id
		{
			change_id(const int& new_id) :new_id(new_id){}

			void operator()(employee& e)
			{
				e.id = new_id;
			}

		private:
			int new_id;
		};

		// �������id��name�ؼ��ʵĶ�ֵ������������
		// ���������id��name������ͼ
		typedef boost::multi_index::multi_index_container<
			employee,

			boost::multi_index::indexed_by<
			//��һ����ͼ��ʹ��employeeʵ����Ϊkey�����Բ�����employee::operator<���������������˻���id�ֶ�
			//boost::multi_index::ordered_unique�е�uniqueָ��keyֻ��Ψһ���������޷�����id�ֶ�ֵ��ͬ�ļ�¼��
			//boost::multi_index::ordered_unique�е�orderedָ����һ����ͼ������key���������У���������������˳�����ҵ�
			//����������Զ�����boost::multi_index::ordered_unique���͵���ͼ
			//boost::multi_index::identity�Ѷ�����Ϊ������key
			//boost::multi_index::tag<id>��Ϊ���Ժ���ͨ��id��ȡ��һ����ͼ
			//�����ȥ��boost::multi_index::tag<id>������㲻��Ҫ��������id��ȡ��ͼ�Ļ�
			boost::multi_index::ordered_unique<boost::multi_index::tag<id>, boost::multi_index::identity<employee> >,
			//�ڶ�����ͼ������key��employee�����name�ֶΣ���������key������std::wstring
			//����ʹ����std::wstring::operator<�������������
			//boost::multi_index::ordered_non_unique�е�non_uniqueָ�����key�������ظ�.
			//boost::multi_index::ordered_non_unique�е�orderedָ���ڶ���ͼ�������key��������	.
			//����㲻��Ҫ��ͼ����key��˳�����У�������hashed����ordered������
			//boost::multi_index::ordered_non_unique ������Ϊ boost::multi_index::hashed_non_unique
			//�Ϳ��Ա���Բ��������������Ҫ����<boost/multi_index/hashed_index.hpp>ͷ�ļ�
			//boost::multi_index::member�Ѷ���ĳ�Ա��Ϊ������key
			//boost::multi_index::tag<name>��Ϊ���Ժ�Ĵ�����ͨ��name��ȡ�ڶ�����ͼ������ͼ�ܶ��ʱ��������ܽ���ǳ�����
			boost::multi_index::ordered_non_unique<boost::multi_index::tag<name>, boost::multi_index::member<employee, std::wstring, &employee::name> >
			//������ͼ�Ķ��������������﷨��ʽ
			//(ordered_unique | ordered_non_unique)	<[(key extractor)[, (comparison predicate)]]>
			//��Ϊkey�Ķ������ͱ���ʵ��<�����������������ָ����comparison predicate.
			>

		> employee_set;
	}
}
#pragma endregion

using namespace kagula::datatype;


void print_out_by_id(const employee_set& es)
{
	//�õ�id��ͼ�����ǻ���id�ֶεģ�
	const employee_set::index<id>::type& id_index = es.get<id>();
	
	//����ע�͵��Ĵ��룬�ǻ������������ͼ��0�����һ����ͼ��1����ڶ�����ͼ����������
	//const employee_set::nth_index<0>::type& id_index = es.get<0>();
	
	//����ͬstd::set����һ���ķ�ʽ��ӡԪ��
	std::copy(
		id_index.begin(), id_index.end(),
		std::ostream_iterator<employee, wchar_t>(std::wcout));
	std::wcout << std::endl << std::endl;
}

void print_out_by_name(const employee_set& es )
{
	//�õ�name��ͼ�����ǻ���name�ֶεģ�
	const employee_set::index<name>::type& name_index = es.get<name>();

	//����ע�͵��Ĵ��룬�ǻ������������ͼ������ڶ�����ͼ����������
	//const employee_set::nth_index<1>::type& name_index = es.get<1>();

	//����ͬstd::set����һ���ķ�ʽ��ӡԪ��
	std::copy(
		name_index.begin(), name_index.end(),
		std::ostream_iterator<employee,wchar_t>(std::wcout));
	std::wcout << std::endl << std::endl;

	/************************************************************************/
	const employee_set::nth_index<1>::type& nameIndex = es.get<1>();
	std::copy(
		nameIndex.rbegin(), nameIndex.rend(),
		std::ostream_iterator<employee,wchar_t>(std::wcout));
	std::wcout << std::endl << std::endl;
	/************************************************************************/
	/************************************************************************/
	const int i = 1;
	const employee_set::nth_index<i>::type& i_nameIndex = es.get<i>();
	std::copy(
		i_nameIndex.begin(), i_nameIndex.end(),
		std::ostream_iterator<employee,wchar_t>(std::wcout));
	std::wcout << std::endl << std::endl;
	/************************************************************************/
}

//������������
void CreateSample(employee_set& table)
{
	table.insert(employee(0, L"Z"));
	table.insert(employee(1, L"Z"));
	table.insert(employee(2, L"X"));
	//Error:����������¼�޷��ɹ����룬��Ϊ�ڽ�����һ����ͼʱָ����id����Ψһ
	//      ����ִ�в�����Ϊ2�ļ�¼��û��Ч��, ��table��idΪ2�ļ�¼������ԭ����ֵ
	//      ���Ǵ���Ҳ�����׳�Excetion��
	//table.insert(employee(2, L"�²����idΪ2�ļ�¼"));

	//����name�ֶε�ֵΪ"Z"�ɹ�
	//��Ȼname�ֶε�ֵ�Ѿ����ڣ��������ڶ���ͼ��ʱ��ָ����name�ֶε�ֵ�������ظ�
	//�����������Ӽ�¼���ܿ�����������
	table.insert(employee(3, L"Z"));


	table.insert(employee(100, L"Judy Smith"));
	table.insert(employee(101, L"Judy Smith"));

	table.insert(employee(200, L"Anna Jones"));
	table.insert(employee(201, L"Anna Jones"));
}


void select_stat(const employee_set& table = employee_set() )
{
	if(table.empty()) return;
	std::wcout << L"��һ����ͼ����id�ֶε���������" << std::endl; 
	print_out_by_id(table);

	{
		std::wcout << L"��һ����ͼ����id�ֶεĽ����ӡ" << std::endl;
		const employee_set::index<id>::type&  viewId = table.get<id>();
		std::copy(viewId.rbegin(), viewId.rend(),
			std::ostream_iterator<employee, wchar_t>(std::wcout));
		std::wcout << std::endl << std::endl;
	}

	std::wcout << L"�ڶ�����ͼ����name�ֶε���������" << std::endl;
	print_out_by_name(table);

	{
		std::wcout << L"������id==2�ļ�¼" << std::endl;
		employee cond(2, L"");
		employee_set::index<id>::type::iterator iter = table.find(cond);
		std::wcout << *iter << std::endl << std::endl;
	}

	{
		std::wcout << L"ͨ��boost::lambda������ָ��id��Χ�ļ�¼����" << std::endl;;
		std::pair<employee_set::iterator, employee_set::iterator> p;
		p = table.range(employee(100, L"") <= boost::lambda::_1, boost::lambda::_1 <= employee(200, L"")); // 100<= x <=200
		//���汻ע�͵��Ĵ��룬��ʾ�����ȡָ����Χ�ļ�¼
		//p = table.range(employee(100, L"")<boost::lambda::_1, boost::lambda::_1<employee(200, L""));   // 100<  x < 200
		//p = table.range(employee(100, L"") <= boost::lambda::_1, boost::lambda::_1<employee(200, L""));  // 100<= x < 200
		//p = table.range(employee(100, L"") <= boost::lambda::_1, boost::multi_index::unbounded); // 100 <= x
		//p = table.range(boost::multi_index::unbounded, boost::lambda::_1<employee(200, L""));  //   x <  200
		//p = table.range(boost::multi_index::unbounded, boost::multi_index::unbounded); // equiv. to std::make_pair(s.begin(),s.end())		
		for (employee_set::iterator it = p.first; it != p.second; it++)
		{
			std::wcout << *it << L" , ";
		}
		std::wcout << std::endl << std::endl;
	}

	{
		std::wcout << L"������name==Judy Smith�ĵ�һ����¼" << std::endl;
		const employee_set::index<name>::type& viewName = table.get<name>();
		employee_set::index<name>::type::iterator it = viewName.find(L"Judy Smith");
		std::wcout << *it << std::endl << std::endl;

		std::wcout << L"ͳ��name==Judy Smith�ļ�¼�ж���" << std::endl;
		unsigned int count = viewName.count(L"Judy Smith");
		std::wcout << count << std::endl << std::endl;
	}

	{
		std::wcout << L"������name==z�ļ�¼����" << std::endl;;
		const employee_set::index<name>::type& viewName = table.get<name>();
		std::pair<employee_set::index<name>::type::iterator, employee_set::index<name>::type::iterator> p;

		p = viewName.equal_range(L"Z"); 

		for (employee_set::index<name>::type::iterator it = p.first; it != p.second; it++)
		{
			std::wcout << *it << L" , ";
		}
		std::wcout << std::endl << std::endl;
	}

}

void delete_stat(employee_set& table)
{
	//�ӱ���ɾ��idֵΪ2�ĵļ�¼��name�ֶο���Ϊ����ֵ
	{
		std::wcout << L"ɾ��id==2�ļ�¼" << std::endl;
		employee cond(2, L"");
		int nCount = table.erase(cond); 
		//nCountΪ1����Ϊ���ǵ�idֵ��Ψһ�ġ�
		//���id�ֶβ���Ψһ�ģ����ǿ���ͨ���ж�nCount,ɾ�����з��������ļ�¼
		std::wcout << L"��" << nCount << L"����¼��ɾ����" << std::endl << std::endl;
	}

	//�ӱ���ɾ��name�ֶ�ֵΪZ��ȫ����¼
	{
		std::wcout << L"ɾ��name==Z�����м�¼" << std::endl;
		employee_set::index<name>::type& viewName = table.get<name>();
		while ( table.erase(*viewName.find(L"Z")) > 0 );
		print_out_by_id(table);
	}

	//���½�������
	table.clear();
	CreateSample(table);

	//ɾ��id��ָ����Χ�ļ�¼
	std::wcout << L"ɾ��100<= id <=200�����м�¼" << std::endl;
	std::pair<employee_set::iterator, employee_set::iterator> p;
	p = table.range(employee(100, L"") <= boost::lambda::_1, boost::lambda::_1 <= employee(200, L"")); // 100<= x <=200
	table.erase(p.first, p.second);
	print_out_by_id(table);
}

void update_stat(employee_set& table)
{
	std::wcout << L"�滻��ΪAnna Jones�ļ�¼ΪAnna Smith������ֻ�޸ķ��������ĵ�һ����¼!" << std::endl;
	typedef employee_set::index<name>::type employee_set_by_name;
	//��ΪҪ�޸�iter��ָ������������������䲻��ʹ��const����
	employee_set_by_name& name_index = table.get<name>();

	employee_set_by_name::iterator it = name_index.find(L"Anna Jones");
	//�޸�key��ֵ��key������Զ�д
	name_index.modify_key(it, boost::lambda::_1 = L"Anna Smith");
	print_out_by_id(table);

	std::wcout << L"����ΪAnna Smith�ĵ�һ����¼�޸�ΪAnna Jones" << std::endl;
	//�������ַ��������޸Ķ���ĳ�����Ե�ֵ��������Ҫ����change_name��
	it = name_index.find(L"Anna Smith");
	name_index.modify(it, change_name(L"Anna Jones"));
	print_out_by_id(table);

	std::wcout << L"����ΪAnna Jones�ĵ�һ����¼��id�޸�Ϊ8888" << std::endl;
	//�������ַ��������޸Ķ���ĳ�����Ե�ֵ��������Ҫ����change_id��
	it = name_index.find(L"Anna Jones");
	name_index.modify(it, change_id(8888));
	print_out_by_id(table);


	{
		std::wcout << L"�޸�����name==Z�ļ�¼Ϊkagula" << std::endl;
		employee_set_by_name& viewName = table.get<name>();
		employee_set_by_name::iterator it;
		while ( (it = viewName.find(L"Z")) != viewName.end() )
		{
			viewName.modify(it, change_name(L"kagula"));
		}
		print_out_by_id(table);

		//Warnning:
		//��Ҫ�����������У�ʹ������Ĵ������޸����з��������ļ�¼
		//��Ϊ���û���ϱߵĴ���Σ�����Ĵ���ֻ���޸ĵ�һ�����������ļ�¼
		//�������˳�ѭ����
		std::wcout << L"�޸�����name==kagula�ļ�¼ΪZ" << std::endl;
		viewName = table.get<name>();
		std::pair<employee_set_by_name::iterator, employee_set_by_name::iterator> p;
		p = viewName.equal_range(L"kagula");
		for (employee_set_by_name::iterator it = p.first; it != p.second; it++)
		{
			viewName.modify(it, change_name(L"Z"));
		}
		print_out_by_id(table);
	}


	//��Ϊ�޸ļ�¼��ֵ��ͨ��it�������Կ���ʹ��select_stat���ܵ�֪ʶ�޸�ָ����Χ�ļ�¼
}

int _tmain(int argc, _TCHAR* argv[])
{
	//ʹstd::wcout�ܹ���ӡ����
	std::wcout.imbue(std::locale(std::locale("chs")));

	//��������
	//�ȼ�SQL,Add������ع���
	employee_set table;
	CreateSample(table);

	//������Եȼ�SQL,Select������ع���
	select_stat(table);
	select_stat();
	
	//������Եȼ�SQL,Update������ع���
	update_stat(table);

	//������Եȼ�SQL,Delete������ع���
	delete_stat(table);

	//��ͣ���������������
	system("pause");
	return 0;
}