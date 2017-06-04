#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <WinSock2.h> // htonl()
#pragma comment(lib, "ws2_32"); // htonl()
#pragma message("comment(lib, \"ws2_32\")");
#define __VC__
#ifndef __VC__
#error "comment+++++++++++++++++++";
#endif

#include "ParentChildConvert.hpp"
#include <map>
#include <set>

// Сϸ�ڲ���
void test()
{
	{
		std::string str = "dafgagagagagertyu";
		str.erase(--str.end());
	}
	{
		std::vector<std::string> vecStr;
		vecStr.push_back("d");
		vecStr.push_back("a");
		vecStr.push_back("c");
		vecStr.push_back("aa");
		vecStr.push_back("a");

		std::map<std::string,std::string> mapStrStr;
		mapStrStr["d"] = "dd";
		mapStrStr["a"] = "dd";
		mapStrStr["aa"] = "bb";
		mapStrStr.insert(std::map<std::string,std::string>::value_type("d","cc"));
		mapStrStr["d"] = "cc";

	}
	{
		std::map<std::string, int> mapStrInt;
		mapStrInt["a"] = 1;
		mapStrInt["b"] = 2;
		mapStrInt["a"] = 3;
		mapStrInt.insert(std::pair<std::string, int>("c", 5));
		mapStrInt.insert(std::pair<std::string, int>("a", 5));
		mapStrInt.insert(std::pair<std::string, int>("a", 10));
		mapStrInt.insert(std::map<std::string, int>::value_type("a", 10));

		std::map<std::string, int> mapStrInt2;
		mapStrInt2["aa"] = 0;
		mapStrInt2.insert(mapStrInt.begin(),mapStrInt.end());

		std::map<std::string, int> mapStrInt3;
		mapStrInt2.insert(mapStrInt3.begin(),mapStrInt3.end());

		for (std::map<std::string, int>::iterator it = mapStrInt.begin(); it != mapStrInt.end(); ++it)
		{
			it->second = 88;
		}

		std::set<int> setInt;
		setInt.insert(2);
		setInt.insert(2000);
		setInt.insert(200);
		setInt.insert(2);
		int i = setInt.size();
	}
	{
		std::set<int> setInt;
		setInt.insert(2);
		setInt.insert(2000);
		setInt.insert(200);
		setInt.insert(2);
		int i = setInt.size();
	}
	{ // ������ת������
		ParentTest *parentTest = new ChildTest;
		ChildTest *childTest = dynamic_cast<ChildTest*>(parentTest);
		childTest->test_c1();
	}
	{ // �ṹ���ʼ��
		// ������Ϣ
		typedef struct BASIC_INFO
		{
			std::string		m_SatModelNo;					// �����ͺ�
			std::string		m_InternalSatelliteName;		// ������������
			std::string		m_ExternalSatelliteName;		// ������������
			std::string		m_Nationality;					// ����
			std::string		m_NORADNumber;					// NORAD���
			std::string		m_COSPARNumber;					// COSPAR���
			std::string		m_UserOrOperator;				// �û�����Ӫ�̣������ã��ֳ�����������
			std::string		m_User;							// �û�
			std::string		m_Operator;						// ��Ӫ��
			std::string		m_ManufactureUnit;				// ���Ƶ�λ
			std::string		m_ManufactureUnitNationality;	// ���е�λ����
			std::string		m_ApplicationType;				// Ӧ������
			std::string		m_Platform;						// ƽ̨
			std::string		m_Orbit;						// ���
			std::string		m_CarryRocketName;				// ���ػ��
			std::string		m_LaunchTimeYear;				// ����ʱ�����
			std::string		m_LaunchTime;					// ����ʱ��
			std::string		m_LaunchPlace;					// ����ص�
			double			m_ServiceYears;					// ����ʱ�� new1
			bool			m_FirstLaunchSatellite;			// �Ƿ�Ϊ�׷�����
			std::string		m_MainTaskDescribe;				// ��Ҫ��������
			std::string		m_SatStatus;					// ����״̬ new1
		}BASIC_INFO_STRUCT;

		// �����Ϣ
		typedef struct ORBIT_INFO
		{
			std::string			m_UpdateTime;				// ����ʱ��
			double				m_OrbitHeight;				// ����߶�
			double				m_OrbitCycle;				// �������
			double				m_OrbitDipAngle;			// ������
			double				m_PerigeeAngle;				// ���ص��
			std::vector<double>	m_Position;					// ����λ��(������)
			std::vector<double>	m_Orbit6Ele;				// �����Ҫ��[��Ҫ�أ�a(�볤��) e(ƫ����) Omiga(������ྭ) i(������) w(���ص����) f(������)����λ���ף���]
			double				m_Jd;						// ��Ԫʱ��
		}ORBIT_INFO_STRUCT;

		// ��Ҫ����ָ��
		typedef struct MAIN_PROPERTY_INFO
		{
			double				m_LaunchQuality;				// ��������
			double				m_SatelliteDryWeight;			// ���Ǹ���
			double				m_DesignLifetime;				// �������
			double				m_BeginOfLifetimePower;			// �������ڹ��ʣ�BOL��
			double				m_EndOfLifetimePower;			// ����ĩ�ڹ��ʣ�EOL��
			std::string			m_PostureStableWay;				// ��̬�ȶ���ʽ
			std::string			m_PropellingSystemType;			// �ƽ�ϵͳ����
			std::string			m_Reliability;					// �ɿ��ԣ�EOL��
		}MAIN_PROPERTY_INFO_STRUCT;

		// ��Ч�غ�
		typedef struct EFFECTIVE_LOAD
		{ 
			std::string		m_name;					// �غ�����
			int				m_number;				// �غ�����
			std::string		m_type;					// �غ�����
			std::string		m_manufactureUnit;		// ���Ƶ�λ
			std::string		m_mainTechnicalParam;	// ��Ҫ����ָ��
		}EFFECTIVE_LOAD_STRUCT;

		// ������Ϣ
		struct BasicInfo 
		{
			std::string							m_UUID;					// UUID
			std::string							m_Name;					// ����
			std::string							m_Constellation;		// ����
			std::string							m_Type;					// ����
			BASIC_INFO_STRUCT					m_BasicInfo;			// ������Ϣ
			ORBIT_INFO_STRUCT					m_OrbitInfo;			// �����Ϣ
			MAIN_PROPERTY_INFO_STRUCT			m_MainPropertyInfo;		// ��Ҫ����ָ��
			std::vector<EFFECTIVE_LOAD_STRUCT>	m_EffectiveLoad;		// ��Ч�غ���Ϣ
		};

		BasicInfo basicInfo2;
		BasicInfo basicInfo = {"123456789"};
		basicInfo2 = basicInfo;
	}
	{ // �ṹ���ʼ��
		struct ChildStruct1
		{
			int				int_num;
			double			double_num;
			std::string		string_str;
		};
		struct TestStruct 
		{
			int				int_num;
			double			double_num;
			std::string		string_str;
			ChildStruct1	cs1;
			std::vector<ChildStruct1> cs1_vec;
		};

		TestStruct ts = {0};
		int cs1_int = ts.cs1.int_num;
		double cs1_doubel = ts.cs1.double_num;
		std::string cs1_string = ts.cs1.string_str;
		TestStruct ts2;
	}
	{ // ����ʱ���ַ���ת������ʱ����
		std::string str= "2016-10-01 12:01:01";
		int year = 0, month = 0, day = 0, hour = 0, min = 0, sec = 0;
		sscanf(str.c_str(),"%d-%d-%d %d:%d:%d",&year,&month,&day,&hour,&min,&sec);
	}
	
	{ // �ֽ���
		int num = 0x12345678;
		unsigned char* pc = (unsigned char*)(&num);
		printf("local order:\n");
		printf("[0]:0x%X addr:%u\n", pc[0],&pc[0]);
		printf("[1]:0x%X addr:%u\n", pc[1],&pc[1]);
		printf("[2]:0x%X addr:%u\n", pc[2],&pc[2]);
		printf("[3]:0x%X addr:%u\n", pc[3],&pc[3]);
		num = htonl(num);
		printf("htonl order:\n");
		printf("[0]:0x%X addr:%u\n", pc[0],&pc[0]);
		printf("[1]:0x%X addr:%u\n", pc[1],&pc[1]);
		printf("[2]:0x%X addr:%u\n", pc[2],&pc[2]);
		printf("[3]:0x%X addr:%u\n", pc[3],&pc[3]);
	}
	{ // sprintf��ʽ
		const char* strcontent = "abcdefg";
		char strcomd[20];
		memset(strcomd,0,20);
		sprintf(strcomd,"%12s",strcontent);
		std::cout << "strcomd = " << "\"" << strcomd << "\"" << std::endl;
		std::cout << "sizeof(strcomd) = " << sizeof(strcomd) << std::endl;

		char strcomd2[20];
		memset(strcomd2,0,20);
		sprintf(strcomd2,"%-12s",strcontent);
		std::cout << "strcomd2 = " << "\"" << strcomd2 << "\"" << std::endl;
		std::cout << "sizeof(strcomd2) = " << sizeof(strcomd2) << std::endl;

		char strcomd3[20];
		memset(strcomd3,0,20);
		sprintf(strcomd3,"0000%-12s",strcontent);
		std::cout << "strcomd3 = " << "\"" << strcomd3 << "\"" << std::endl;
		std::cout << "sizeof(strcomd3) = " << sizeof(strcomd3) << std::endl;

		char strcomd4[20];
		memset(strcomd4,0,20);
		sprintf(strcomd4,"1111%-12s",strcontent);
		std::cout << "strcomd4 = " << "\"" << strcomd4 << "\"" << std::endl;
		std::cout << "sizeof(strcomd4) = " << sizeof(strcomd4) << std::endl;
	}

	{ // ������		
#ifdef PRINT_TEST
		std::cout << "PRINT_TEST" << std::endl;
#endif
		int size_of_int = sizeof(unsigned int);
		int size_of_char = sizeof(char);
		int size_of_char_x = sizeof(char *);
	}
	{ // ����
		// ʮ������
		int z1 = 0x1;
		int z2 = 0x10;
		int z3 = 0x100;
		int z4 = 0x1000;
		// �˽���
		int y1 = 0001;
		int y2 = 0010;
		int y3 = 0100;
	}
	
	{ // �ַ������
		std::string keyword(" abc ��ջ֡  �ɹ� �ɹ�    ");
		char delims[] = " ";
		char *result = NULL;
		result = strtok( (char*)keyword.c_str(), delims );
		std::vector<std::string> keywordVec;
		while( result != NULL ) 
		{
			keywordVec.push_back(std::string(result));
			result = strtok( NULL, delims );
		}
	}
	
	{ // ���ַ����ж�
		std::string str1("����a�����ϼ���");
		if (std::strstr(str1.c_str(),"a����"))
		{
			const char * s = strstr(str1.c_str(),"a����");
			int i = 0;
			++i;
		}
	}
}