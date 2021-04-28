#include <iostream>
#include "tinyxml.h"
#include "tinystr.h"
#include <string>
#include <windows.h>
#include <atlstr.h>
using namespace std;
CString GetAppPath()
{//��ȡӦ�ó����Ŀ¼
	TCHAR modulePath[MAX_PATH];
	GetModuleFileName(NULL, modulePath, MAX_PATH);
	CString strModulePath(modulePath);
	strModulePath = strModulePath.Left(strModulePath.ReverseFind(_T('\\')));
	return strModulePath;
}
bool CreateXmlFile(string& szFileName)
{//����xml�ļ�,szFilePathΪ�ļ������·��,�������ɹ�����true,����false
	try
	{
		//����һ��XML���ĵ�����
		TiXmlDocument *myDocument = new TiXmlDocument();
		//����һ����Ԫ�ز����ӡ�
		TiXmlElement *RootElement = new TiXmlElement("Persons");
		myDocument->LinkEndChild(RootElement);
		//����һ��PersonԪ�ز����ӡ�
		TiXmlElement *PersonElement = new TiXmlElement("Person");
		RootElement->LinkEndChild(PersonElement);
		TiXmlElement *PersonElement2 = new TiXmlElement("Person2");
		RootElement->LinkEndChild(PersonElement2);
		TiXmlElement *PersonElement3 = new TiXmlElement("Person3");
		TiXmlElement *PersonElement4 = new TiXmlElement("Person4");
		PersonElement2->LinkEndChild(PersonElement3);
		//PersonElement2->LinkEndChild(PersonElement3);
		//����PersonԪ�ص����ԡ�
		PersonElement->SetAttribute("ID", "4");
		PersonElement->SetAttribute("ID2", "2");
		PersonElement->SetAttribute("ID2", "2222");
		PersonElement->SetDoubleAttribute("ID3", 3.1);
		PersonElement->SetValue("Tag");
		//����nameԪ�ء�ageԪ�ز����ӡ�
		TiXmlElement *NameElement = new TiXmlElement("name");
		TiXmlElement *AgeElement = new TiXmlElement("age");
		PersonElement->LinkEndChild(NameElement);
		PersonElement->LinkEndChild(AgeElement);
		//����nameԪ�غ�ageԪ�ص����ݲ����ӡ�
		TiXmlText *NameContent = new TiXmlText("������");
		TiXmlText *NameContent2 = new TiXmlText("/api/user/login-by-pc192.168.0.117");
		TiXmlText *AgeContent = new TiXmlText("22");
		NameElement->LinkEndChild(NameContent);
		NameElement->LinkEndChild(NameContent2);
		AgeElement->LinkEndChild(AgeContent);
		CString appPath = GetAppPath();
		string seperator = "\\";
		string fullPath = appPath.GetBuffer(0) +seperator+szFileName;
		myDocument->SaveFile(fullPath.c_str());//���浽�ļ�
		delete myDocument;
	}
	catch (string& e)
	{
		return false;
	}
	return true;
}
bool ReadXmlFile(string& szFileName)
{//��ȡXml�ļ���������
	try
	{
		CString appPath = GetAppPath();
		string seperator = "\\";
		string fullPath = appPath.GetBuffer(0) +seperator+szFileName;
		//����һ��XML���ĵ�����
		TiXmlDocument *myDocument = new TiXmlDocument(fullPath.c_str());
		myDocument->LoadFile();
		//��ø�Ԫ�أ���Persons��
		TiXmlElement *RootElement = myDocument->RootElement();
		//�����Ԫ�����ƣ������Persons��
		cout << RootElement->Value() << endl;
		//��õ�һ��Person�ڵ㡣
		TiXmlElement *FirstPerson = RootElement->FirstChildElement();
		FirstPerson->SetAttribute("ID4","4");
		cout << FirstPerson->Value() << endl;
		TiXmlElement *SecondPerson = FirstPerson->NextSiblingElement();
		cout << SecondPerson->Value() << endl;
		//��õ�һ��Person��name�ڵ��age�ڵ��ID���ԡ�
		TiXmlElement *NameElement = FirstPerson->FirstChildElement();
		TiXmlElement *AgeElement = NameElement->NextSiblingElement();
		std::string id = FirstPerson->Attribute("ID");
		int id2Int = 0;
		std::string id2Str = FirstPerson->Attribute("ID2", &id2Int);
		TiXmlAttribute *IDAttribute = FirstPerson->FirstAttribute();
		TiXmlAttribute *IDAttribute2 = IDAttribute->Next();
		TiXmlAttribute *IDAttribute3 = FirstPerson->LastAttribute();
		//�����һ��Person��name���ݣ��������ǣ�age���ݣ�����ID���ԣ�����
		cout << NameElement->FirstChild()->Value() << endl;
		cout << AgeElement->Value() << endl;
		cout << AgeElement->FirstChild()->Value() << endl;
		cout << IDAttribute->Value()<< endl;
		cout << IDAttribute2->Value()<< endl;
		cout << IDAttribute3->Name()<< endl;
		cout << IDAttribute3->Value()<< endl;
		NameElement->FirstChild()->SetValue("����");
		myDocument->SaveFile(fullPath.c_str());//���浽�ļ�
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