// testJsonParse.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/date_time.hpp>

using namespace boost::property_tree;
using namespace boost::gregorian;
using namespace boost;
using namespace std;

void test1()
{
	string strJson;
	strJson = "{\"people\":[{\"firstName\":\"Brett\",\"lastName\":\"McLaughlin\",\"email\":\"aaaa\"},{\"firstName\":\"Jason\",\"lastName\":\"Hunter\",\"email\":\"bbbb\"},{\"firstName\":\"Elliotte\",\"lastName\":\"Harold\",\"email\":\"cccc\"}]}"; 
	std::string stra,strc;
	vector<string> vecStr;
	ptree pt,p1,p2,p3,p4;
	stringstream stream;
	stream << strJson;
	read_json<ptree>( stream, pt);
	p1 = pt.get_child("people");
	for (ptree::iterator it = p1.begin(); it != p1.end(); ++it)
	{
		p2 = it->second; //first为空
		stra = p2.get<string>("firstName");
		vecStr.push_back(stra);
	}
}

void test2()
{
	string strJson;
	strJson = "{\"programmers\":[{\"firstName\":\"Brett\",\"lastName\":\"McLaughlin\",\"email\":\"aaaa\"},{\"firstName\":\"Jason\",\"lastName\":\"Hunter\",\"email\":\"bbbb\"},{\"firstName\":\"Elliotte\",\"lastName\":\"Harold\",\"email\":\"cccc\"}],\"authors\":[{\"firstName\":\"Isaac\",\"lastName\":\"Asimov\",\"genre\":\"sciencefiction\"},{\"firstName\":\"Tad\",\"lastName\":\"Williams\",\"genre\":\"fantasy\"},{\"firstName\":\"Frank\",\"lastName\":\"Peretti\",\"genre\":\"christianfiction\"}],\"musicians\":[{\"firstName\":\"Eric\",\"lastName\":\"Clapton\",\"instrument\":\"guitar\"},{\"firstName\":\"Sergei\",\"lastName\":\"Rachmaninoff\",\"instrument\":\"piano\"}]}"; 
	std::string stra,strc;
	vector<string> vecStr;
	ptree pt,p1,p2,p3,p4;
	stringstream stream;
	stream << strJson;
	read_json<ptree>( stream, pt);
	p1 = pt.get_child("programmers");
	for (ptree::iterator it = p1.begin(); it != p1.end(); ++it)
	{
		p2 = it->second; //first为空
		stra = p2.get<string>("firstName");
		vecStr.push_back(stra);

// 		p3 = p2.get_child("markers");
// 		for (ptree::iterator ita = p3.begin(); ita != p3.end(); ++ita)
// 		{
// 			p4 = ita->second;
// 			strc = p4.get<string>("markerItemLink");			
// 		}

	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	test2();
	system("pause");
	return 0;
}

