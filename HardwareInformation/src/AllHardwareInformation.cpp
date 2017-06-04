#include "AllHardwareInformation.h"

AllHardwareInformation::AllHardwareInformation()
{

}

AllHardwareInformation::~AllHardwareInformation()
{

}

std::string AllHardwareInformation::TCHAR2STRING(TCHAR * STR)
{
	int iLen = WideCharToMultiByte(CP_ACP,0,STR,-1,NULL,0,NULL,NULL);
	char* chRtn = new char[iLen*sizeof(char)];
	WideCharToMultiByte(CP_ACP,0,STR,-1,chRtn,iLen,NULL,NULL);
	return chRtn;
}

std::map<std::string,std::string> AllHardwareInformation::getHardInfor(LPTSTR ClassSign,LPTSTR lpTag)
{
	int iCount = 0;
	pHardWareInformation pInfo;
	ResCreateLink(pInfo);
	HdwGetInfo(ClassSign,lpTag,pInfo,iCount);
	std::map<std::string,std::string> hardwareInfoMap;
	pInfo = pInfo->next;
	while (pInfo)
	{
		hardwareInfoMap.insert(make_pair(TCHAR2STRING(pInfo->strName),TCHAR2STRING(pInfo->str))); 
		pInfo = pInfo->next;
	}
	return hardwareInfoMap;
}

std::vector<std::map<std::string,std::string>> AllHardwareInformation::getHardInforVec(LPTSTR ClassSign,LPTSTR lpTag)
{
	int iCount = 0;
	pHardWareInformation pInfo;
	ResCreateLink(pInfo);
	HdwGetInfo(ClassSign,lpTag,pInfo,iCount);
	std::vector<std::map<std::string,std::string>> hardwareInfoMapVec;
	std::map<std::string,std::string> hardwareInfoMap;
	pInfo = pInfo->next;
	while (pInfo)
	{
		std::vector<std::map<std::string,std::string>>::iterator ite=hardwareInfoMapVec.begin();
		for (;ite!=hardwareInfoMapVec.end();++ite)
		{
			std::map<std::string,std::string>::iterator iter = ite->find(TCHAR2STRING(pInfo->strName));
			if (iter == ite->end()) break;
		}
		if (ite!=hardwareInfoMapVec.end())
		{
			ite->insert(make_pair(TCHAR2STRING(pInfo->strName),TCHAR2STRING(pInfo->str))); 
		}
		else
		{
			std::map<std::string,std::string> hardwareInfoMap;
			hardwareInfoMap.insert(make_pair(TCHAR2STRING(pInfo->strName),TCHAR2STRING(pInfo->str)));
			hardwareInfoMapVec.push_back(hardwareInfoMap);
		}
		pInfo = pInfo->next;
	}
	return hardwareInfoMapVec;
}

std::map<std::string,std::string> AllHardwareInformation::getHardInforProcessor()
{
	return getHardInfor(HARDINFOR_PROCESSOR,NULL);
}

std::map<std::string,std::string> AllHardwareInformation::getHardInforDisk()
{
	return getHardInfor(HARDINFOR_DISK,NULL);
}

std::map<std::string,std::string> AllHardwareInformation::getHardInforGraphics()
{
	return getHardInfor(HARDINFOR_GRAPHICS,NULL);
}

std::map<std::string,std::string> AllHardwareInformation::getHardInforNetwork()
{
	return getHardInfor(HARDINFOR_NETWORK,TEXT("Caption"));
}

std::map<std::string,std::string> AllHardwareInformation::getHardInforMainboard()
{
	return getHardInfor(HARDINFOR_MAINBOARD,NULL);
}

std::map<std::string,std::string> AllHardwareInformation::getHardInforDisplay()
{
	return getHardInfor(HARDINFOR_DISPLAY,NULL);
}

std::map<std::string,std::string> AllHardwareInformation::getHardInforSoundcard()
{
	return getHardInfor(HARDINFOR_SOUNDCARD,NULL);
}

std::vector<std::map<std::string,std::string>> AllHardwareInformation::getHardInforAdapter()
{
	return getHardInforVec(HARDINFOR_ADAPTER,NULL);
}

std::map<std::string,std::string> AllHardwareInformation::getHardInforSoftSystem()
{
	std::map<std::string,std::string> softSystemMap;
	pHardWareInformation pInfo;
	ResCreateLink(pInfo);
	int rValue = SysGetWinInstallTime(pInfo);
	if (rValue != -1)
	{
		pInfo = pInfo->next;
		softSystemMap[TCHAR2STRING(pInfo->strName)] = TCHAR2STRING(pInfo->str);
	}

	rValue = SysGetWinLaguage(pInfo);
	if (rValue != -1)
	{
		pInfo = pInfo->next;
		softSystemMap[TCHAR2STRING(pInfo->strName)] = TCHAR2STRING(pInfo->str);
	}

	rValue = SysGetWinProductName(pInfo);
	if (rValue != -1)
	{
		pInfo = pInfo->next;
		softSystemMap[TCHAR2STRING(pInfo->strName)] = TCHAR2STRING(pInfo->str);
	}

	rValue = SysGetWinHostName(pInfo);
	if (rValue != -1)
	{
		pInfo = pInfo->next;
		softSystemMap[TCHAR2STRING(pInfo->strName)] = TCHAR2STRING(pInfo->str);
	}

	return softSystemMap;
}

