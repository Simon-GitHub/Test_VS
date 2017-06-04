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

// 小细节测试
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
	{ // 父子类转换测试
		ParentTest *parentTest = new ChildTest;
		ChildTest *childTest = dynamic_cast<ChildTest*>(parentTest);
		childTest->test_c1();
	}
	{ // 结构体初始化
		// 基本信息
		typedef struct BASIC_INFO
		{
			std::string		m_SatModelNo;					// 卫星型号
			std::string		m_InternalSatelliteName;		// 对内卫星名称
			std::string		m_ExternalSatelliteName;		// 对外卫星名称
			std::string		m_Nationality;					// 国别
			std::string		m_NORADNumber;					// NORAD编号
			std::string		m_COSPARNumber;					// COSPAR编号
			std::string		m_UserOrOperator;				// 用户或运营商（已弃用，分成下面两个）
			std::string		m_User;							// 用户
			std::string		m_Operator;						// 运营商
			std::string		m_ManufactureUnit;				// 研制单位
			std::string		m_ManufactureUnitNationality;	// 承研单位国别
			std::string		m_ApplicationType;				// 应用类型
			std::string		m_Platform;						// 平台
			std::string		m_Orbit;						// 轨道
			std::string		m_CarryRocketName;				// 运载火箭
			std::string		m_LaunchTimeYear;				// 发射时间年份
			std::string		m_LaunchTime;					// 发射时间
			std::string		m_LaunchPlace;					// 发射地点
			double			m_ServiceYears;					// 服役时间 new1
			bool			m_FirstLaunchSatellite;			// 是否为首发卫星
			std::string		m_MainTaskDescribe;				// 主要任务描述
			std::string		m_SatStatus;					// 卫星状态 new1
		}BASIC_INFO_STRUCT;

		// 轨道信息
		typedef struct ORBIT_INFO
		{
			std::string			m_UpdateTime;				// 更新时间
			double				m_OrbitHeight;				// 轨道高度
			double				m_OrbitCycle;				// 轨道周期
			double				m_OrbitDipAngle;			// 轨道倾角
			double				m_PerigeeAngle;				// 近地点角
			std::vector<double>	m_Position;					// 卫星位置(已弃用)
			std::vector<double>	m_Orbit6Ele;				// 轨道六要素[六要素，a(半长轴) e(偏心率) Omiga(升交点赤经) i(轨道倾角) w(近地点幅角) f(真近点角)，单位：米，度]
			double				m_Jd;						// 历元时刻
		}ORBIT_INFO_STRUCT;

		// 主要性能指标
		typedef struct MAIN_PROPERTY_INFO
		{
			double				m_LaunchQuality;				// 发射质量
			double				m_SatelliteDryWeight;			// 卫星干重
			double				m_DesignLifetime;				// 设计寿命
			double				m_BeginOfLifetimePower;			// 寿命初期功率（BOL）
			double				m_EndOfLifetimePower;			// 寿命末期功率（EOL）
			std::string			m_PostureStableWay;				// 姿态稳定方式
			std::string			m_PropellingSystemType;			// 推进系统类型
			std::string			m_Reliability;					// 可靠性（EOL）
		}MAIN_PROPERTY_INFO_STRUCT;

		// 有效载荷
		typedef struct EFFECTIVE_LOAD
		{ 
			std::string		m_name;					// 载荷名称
			int				m_number;				// 载荷数量
			std::string		m_type;					// 载荷类型
			std::string		m_manufactureUnit;		// 研制单位
			std::string		m_mainTechnicalParam;	// 主要技术指标
		}EFFECTIVE_LOAD_STRUCT;

		// 卫星信息
		struct BasicInfo 
		{
			std::string							m_UUID;					// UUID
			std::string							m_Name;					// 名称
			std::string							m_Constellation;		// 星座
			std::string							m_Type;					// 类型
			BASIC_INFO_STRUCT					m_BasicInfo;			// 基本信息
			ORBIT_INFO_STRUCT					m_OrbitInfo;			// 轨道信息
			MAIN_PROPERTY_INFO_STRUCT			m_MainPropertyInfo;		// 主要性能指标
			std::vector<EFFECTIVE_LOAD_STRUCT>	m_EffectiveLoad;		// 有效载荷信息
		};

		BasicInfo basicInfo2;
		BasicInfo basicInfo = {"123456789"};
		basicInfo2 = basicInfo;
	}
	{ // 结构体初始化
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
	{ // 日期时间字符串转年月日时分秒
		std::string str= "2016-10-01 12:01:01";
		int year = 0, month = 0, day = 0, hour = 0, min = 0, sec = 0;
		sscanf(str.c_str(),"%d-%d-%d %d:%d:%d",&year,&month,&day,&hour,&min,&sec);
	}
	
	{ // 字节序
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
	{ // sprintf格式
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

	{ // 条件宏		
#ifdef PRINT_TEST
		std::cout << "PRINT_TEST" << std::endl;
#endif
		int size_of_int = sizeof(unsigned int);
		int size_of_char = sizeof(char);
		int size_of_char_x = sizeof(char *);
	}
	{ // 进制
		// 十六进制
		int z1 = 0x1;
		int z2 = 0x10;
		int z3 = 0x100;
		int z4 = 0x1000;
		// 八进制
		int y1 = 0001;
		int y2 = 0010;
		int y3 = 0100;
	}
	
	{ // 字符串拆分
		std::string keyword(" abc 堆栈帧  成功 成功    ");
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
	
	{ // 子字符串判断
		std::string str1("发生a法律上几分");
		if (std::strstr(str1.c_str(),"a法律"))
		{
			const char * s = strstr(str1.c_str(),"a法律");
			int i = 0;
			++i;
		}
	}
}