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
	std::map<std::string,std::string> getHardInforProcessor();//CPU信息
	std::map<std::string,std::string> getHardInforDisk();//硬盘信息
	std::map<std::string,std::string> getHardInforGraphics();//显卡信息
	std::map<std::string,std::string> getHardInforNetwork();//网卡信息
	std::map<std::string,std::string> getHardInforMainboard();//主板信息
	std::map<std::string,std::string> getHardInforDisplay();//显示器信息
	std::map<std::string,std::string> getHardInforSoundcard();//声卡信息
	std::vector<std::map<std::string,std::string>> getHardInforAdapter();//网络适配器信息
	std::map<std::string,std::string> getHardInforSoftSystem();//操作系统信息
};