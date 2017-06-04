#pragma once
#include <iostream>
#include <boost/regex.hpp>

using namespace std;

int test_RegEx(/*int argc, char* argv[]*/)
{
	//( 1 )   ((  3  )  2 )((  5 )4)(    6    )   
	//(\w+)://((\w+\.)*\w+)((/\w*)*)(/\w+\.\w+)?
	//^Э��://��ַ(x.x...x)/·��(n��\�ִ�)/��ҳ�ļ�(xxx.xxx)
	const char *szReg = "(\\w+)://((\\w+\\.)*\\w+)((/\\w*)*)(/\\w+\\.\\w+)?";
	const char *szStr = "http://www.cppprog.com/2009/0112/48.html";

	{    //�ַ���ƥ��
		boost::regex reg( szReg );
		bool r=boost::regex_match( szStr , reg);
		assert(r);
	}

	{    //��ȡ�Ӵ�
		boost::cmatch mat;
		boost::regex reg( szReg );
		bool r=boost::regex_match( szStr, mat, reg);
		if(r) //���ƥ��ɹ�
		{
			//��ʾ�����Ӵ�
			for(boost::cmatch::iterator itr=mat.begin(); itr!=mat.end(); ++itr)
			{
				//       ָ���Ӵ���Ӧ��λ��        ָ���Ӵ���Ӧβλ��          �Ӵ�����
				cout << itr->first-szStr << ' ' << itr->second-szStr << ' ' << *itr << endl;
			}
		}
		//Ҳ��ֱ��ȡָ��λ����Ϣ
		if(mat[4].matched) cout << "Path is" << mat[4] << endl;
	}

	{ //����
		boost::cmatch mat;
		boost::regex reg( "\\d+" );    //�����ַ����������
		if(boost::regex_search(szStr, mat, reg))
		{
			cout << "searched:" << mat[0] << endl;
		}
	}

	{ //�滻
		boost::regex reg( szReg );
		string s = boost::regex_replace( string(szStr), reg, "ftp://$2$5");
		cout << "ftp site:"<< s << endl;
	}
	{ //�滻2����<>&ת������ҳ�ַ�
		string s1 = "(<)|(>)|(&)";
		string s2 = "(?1<)(?2>)(?3&)";
		boost::regex reg( s1 );
		string s = boost::regex_replace( string("cout << a&b << endl;"), reg, s2, boost::match_default | boost::format_all);
		cout << "HTML:"<< s << endl;
	}

	{ //ʹ�õ������ҳ���������
		boost::regex reg( "\\d+" );    //�����ַ����������
		boost::cregex_iterator itrBegin = make_regex_iterator(szStr,reg); //(szStr, szStr+strlen(szStr), reg);
		boost::cregex_iterator itrEnd;
		for(boost::cregex_iterator itr=itrBegin; itr!=itrEnd; ++itr)
		{
			//       ָ���Ӵ���Ӧ��λ��        ָ���Ӵ���Ӧβλ��          �Ӵ�����
			cout << (*itr)[0].first-szStr << ' ' << (*itr)[0].second-szStr << ' ' << *itr << endl;
		}
	}

	{ //ʹ�õ���������ַ���
		boost::regex reg("/");  //��/������ַ���
		boost::cregex_token_iterator itrBegin = make_regex_token_iterator(szStr,reg,-1); //ʹ��-1����ʱ��֣�ʹ����������ʱ��ʾȡ�ڼ����Ӵ�����ʹ������ȡ�����
		boost::cregex_token_iterator itrEnd;
		for(boost::cregex_token_iterator itr=itrBegin; itr!=itrEnd; ++itr)
		{
			cout << *itr << endl;
		}
	}

	{ //ʹ�õ���������ַ���2
		boost::regex reg("(.)/(.)");  //ȡ/��ǰһ�ַ��ͺ�һ�ַ�������ַ�������ò���е�а��-_-��
		int subs[] = {1,2};        // ��һ�Ӵ��͵ڶ��Ӵ�
		boost::cregex_token_iterator itrBegin = make_regex_token_iterator(szStr,reg,subs); //ʹ��-1����ʱ��֣�ʹ����������ʱ��ʾȡ�ڼ����Ӵ�����ʹ������ȡ�����
		boost::cregex_token_iterator itrEnd;
		for(boost::cregex_token_iterator itr=itrBegin; itr!=itrEnd; ++itr)
		{
			cout << *itr << endl;
		}
	}


	cin.get();
	return 0;
}
