#pragma once
#include "GetInformation.h"
#include <map>
#include <vector>

class AllHardwareInformation
{
public:
	AllHardwareInformation();
	~AllHardwareInformation();
private:
	std::string TCHAR2STRING(TCHAR * STR);
	std::map<std::string,std::string> getHardInfor(LPTSTR ClassSign,LPTSTR lpTag);
	std::vector<std::map<std::string,std::string>> getHardInforVec(LPTSTR ClassSign,LPTSTR lpTag);
public:
	std::map<std::string,std::string> getHardInforProcessor();//CPU��Ϣ
	std::map<std::string,std::string> getHardInforDisk();//Ӳ����Ϣ
	std::map<std::string,std::string> getHardInforGraphics();//�Կ���Ϣ
	std::map<std::string,std::string> getHardInforNetwork();//������Ϣ
	std::map<std::string,std::string> getHardInforMainboard();//������Ϣ
	std::map<std::string,std::string> getHardInforDisplay();//��ʾ����Ϣ
	std::map<std::string,std::string> getHardInforSoundcard();//������Ϣ
	std::vector<std::map<std::string,std::string>> getHardInforAdapter();//������������Ϣ
	std::map<std::string,std::string> getHardInforSoftSystem();//����ϵͳ��Ϣ
};