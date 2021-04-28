#include <iostream>
#include "tinyxml.h"
#include "tinystr.h"
#include <string>
#include <windows.h>
#include <atlstr.h>
using namespace std;
CString GetAppPath()
{//获取应用程序根目录
	TCHAR modulePath[MAX_PATH];
	GetModuleFileName(NULL, modulePath, MAX_PATH);
	CString strModulePath(modulePath);
	strModulePath = strModulePath.Left(strModulePath.ReverseFind(_T('\\')));
	return strModulePath;
}
bool CreateXmlFile(string& szFileName)
{//创建xml文件,szFilePath为文件保存的路径,若创建成功返回true,否则false
	try
	{
		//创建一个XML的文档对象。
		TiXmlDocument *myDocument = new TiXmlDocument();
		//创建一个根元素并连接。
		TiXmlElement *RootElement = new TiXmlElement("Persons");
		myDocument->LinkEndChild(RootElement);
		//创建一个Person元素并连接。
		TiXmlElement *PersonElement = new TiXmlElement("Person");
		RootElement->LinkEndChild(PersonElement);
		TiXmlElement *PersonElement2 = new TiXmlElement("Person2");
		RootElement->LinkEndChild(PersonElement2);
		TiXmlElement *PersonElement3 = new TiXmlElement("Person3");
		TiXmlElement *PersonElement4 = new TiXmlElement("Person4");
		PersonElement2->LinkEndChild(PersonElement3);
		//PersonElement2->LinkEndChild(PersonElement3);
		//设置Person元素的属性。
		PersonElement->SetAttribute("ID", "4");
		PersonElement->SetAttribute("ID2", "2");
		PersonElement->SetAttribute("ID2", "2222");
		PersonElement->SetDoubleAttribute("ID3", 3.1);
		PersonElement->SetValue("Tag");
		//创建name元素、age元素并连接。
		TiXmlElement *NameElement = new TiXmlElement("name");
		TiXmlElement *AgeElement = new TiXmlElement("age");
		PersonElement->LinkEndChild(NameElement);
		PersonElement->LinkEndChild(AgeElement);
		//设置name元素和age元素的内容并连接。
		TiXmlText *NameContent = new TiXmlText("周星星");
		TiXmlText *NameContent2 = new TiXmlText("/api/user/login-by-pc192.168.0.117");
		TiXmlText *AgeContent = new TiXmlText("22");
		NameElement->LinkEndChild(NameContent);
		NameElement->LinkEndChild(NameContent2);
		AgeElement->LinkEndChild(AgeContent);
		CString appPath = GetAppPath();
		string seperator = "\\";
		string fullPath = appPath.GetBuffer(0) +seperator+szFileName;
		myDocument->SaveFile(fullPath.c_str());//保存到文件
		delete myDocument;
	}
	catch (string& e)
	{
		return false;
	}
	return true;
}
bool ReadXmlFile(string& szFileName)
{//读取Xml文件，并遍历
	try
	{
		CString appPath = GetAppPath();
		string seperator = "\\";
		string fullPath = appPath.GetBuffer(0) +seperator+szFileName;
		//创建一个XML的文档对象。
		TiXmlDocument *myDocument = new TiXmlDocument(fullPath.c_str());
		myDocument->LoadFile();
		//获得根元素，即Persons。
		TiXmlElement *RootElement = myDocument->RootElement();
		//输出根元素名称，即输出Persons。
		cout << RootElement->Value() << endl;
		//获得第一个Person节点。
		TiXmlElement *FirstPerson = RootElement->FirstChildElement();
		FirstPerson->SetAttribute("ID4","4");
		cout << FirstPerson->Value() << endl;
		TiXmlElement *SecondPerson = FirstPerson->NextSiblingElement();
		cout << SecondPerson->Value() << endl;
		//获得第一个Person的name节点和age节点和ID属性。
		TiXmlElement *NameElement = FirstPerson->FirstChildElement();
		TiXmlElement *AgeElement = NameElement->NextSiblingElement();
		std::string id = FirstPerson->Attribute("ID");
		int id2Int = 0;
		std::string id2Str = FirstPerson->Attribute("ID2", &id2Int);
		TiXmlAttribute *IDAttribute = FirstPerson->FirstAttribute();
		TiXmlAttribute *IDAttribute2 = IDAttribute->Next();
		TiXmlAttribute *IDAttribute3 = FirstPerson->LastAttribute();
		//输出第一个Person的name内容，即周星星；age内容，即；ID属性，即。
		cout << NameElement->FirstChild()->Value() << endl;
		cout << AgeElement->Value() << endl;
		cout << AgeElement->FirstChild()->Value() << endl;
		cout << IDAttribute->Value()<< endl;
		cout << IDAttribute2->Value()<< endl;
		cout << IDAttribute3->Name()<< endl;
		cout << IDAttribute3->Value()<< endl;
		NameElement->FirstChild()->SetValue("马大哈");
		myDocument->SaveFile(fullPath.c_str());//保存到文件
	}
	catch (string& e)
	{
		return false;
	}
	return true;
}
int main()
{
	string fileName = "info.xml";
	CreateXmlFile(fileName);
	ReadXmlFile(fileName);
}