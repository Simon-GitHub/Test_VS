#include "GetInformation.h"
#include "AllHardwareInformation.h"

int main()
{
	int iCount = 0;
	pHardWareInformation pInfo;
	ResCreateLink(pInfo);

	AllHardwareInformation a;
	std::map<std::string,std::string> m1 = a.getHardInforProcessor();//CPU��Ϣ
	std::map<std::string,std::string> m2 = a.getHardInforDisk();//Ӳ����Ϣ
	std::map<std::string,std::string> m3 = a.getHardInforGraphics();//�Կ���Ϣ
	//std::map<std::string,std::string> m4 = a.getHardInforNetwork();//������Ϣ
	std::map<std::string,std::string> m5 = a.getHardInforMainboard();//������Ϣ
	//std::map<std::string,std::string> m6 = a.getHardInforDisplay();//��ʾ����Ϣ
	std::map<std::string,std::string> m7 = a.getHardInforSoundcard();//������Ϣ
	std::vector<std::map<std::string,std::string>> m8 = a.getHardInforAdapter();//������������Ϣ
	std::map<std::string,std::string> m9 = a.getHardInforSoftSystem();//����ϵͳ��Ϣ

	HdwGetInfo(HARDINFOR_ADAPTER,NULL,pInfo,iCount);

	SysGetWinInstallTime(pInfo);

	std::system("pause");
}