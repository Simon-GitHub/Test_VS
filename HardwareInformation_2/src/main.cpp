#include "GetInformation.h"
#include "AllHardwareInformation.h"

int main()
{
	int iCount = 0;
	pHardWareInformation pInfo;
	ResCreateLink(pInfo);

	AllHardwareInformation a;
	std::map<std::string,std::string> m1 = a.getHardInforProcessor();//CPU信息
	std::map<std::string,std::string> m2 = a.getHardInforDisk();//硬盘信息
	std::map<std::string,std::string> m3 = a.getHardInforGraphics();//显卡信息
	//std::map<std::string,std::string> m4 = a.getHardInforNetwork();//网卡信息
	std::map<std::string,std::string> m5 = a.getHardInforMainboard();//主板信息
	//std::map<std::string,std::string> m6 = a.getHardInforDisplay();//显示器信息
	std::map<std::string,std::string> m7 = a.getHardInforSoundcard();//声卡信息
	std::vector<std::map<std::string,std::string>> m8 = a.getHardInforAdapter();//网络适配器信息
	std::map<std::string,std::string> m9 = a.getHardInforSoftSystem();//操作系统信息

	HdwGetInfo(HARDINFOR_ADAPTER,NULL,pInfo,iCount);

	SysGetWinInstallTime(pInfo);

	std::system("pause");
}