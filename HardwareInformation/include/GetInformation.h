#ifndef _GETINFORMATION_H
#define _GETINFORMATION_H

#include "stdafx.h"
#include "wbemidl.h"
#include "Wbemcli.h"

#include "wininet.h"
#include "UrlHist.h"

#include "iostream"

#include "winioctl.h"
#include "string"
#include "list"
using namespace std;

#pragma comment(lib,"Wbemuuid.lib")

#define SWEEP_BUFFER_SIZE 10000

enum DEL_CACHE_TYPE //Ҫɾ��������
{
	File,			//��ʾinternet��ʱ�ļ�
	Cookie			//��ʾCookie
};
///////////////////////////////////
//Ӳ����Ϣ����

typedef struct HardwareInformation
{
	TCHAR strName[MAX_PATH/4];
	TCHAR str[MAX_PATH/2];
	struct HardwareInformation *next;

}HardwareInformation,*pHardWareInformation;


#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

#define HARDINFOR_PROCESSOR TEXT("Win32_Processor") //CPU��Ϣ
#define PROCESSOR_Manfacturer TEXT("Manufacturer")//CPU����
#define PROCESSOR_MaxClockSpeed TEXT("MaxClockSpeed")//CPUĬ��Ƶ��
#define PROCESSOR_ExtClock TEXT("ExtClock")//CPU��Ƶ
#define PROCESSOR_CurrentClockSpeed TEXT("CurrentClockSpeed")//CPU��ǰƵ��
#define PROCESSOR_LzCacheSize TEXT("L2CacheSize")//CPU��������
#define PROCESSOR_CurrentVoltage TEXT("CurrentVoltage")//CPU��ѹ
#define PROCESSOR_ProcessorId TEXT("ProcessorId")//CPU���к�
#define PROCESSOR_DataWidth TEXT("DataWidth")//���ݿ��
#define PROCESSOR_MyCPUType TEXT("MyType")//��������


#define HARDINFOR_DISK TEXT("Win32_DiskDrive") //Ӳ����Ϣ
#define DISK_Caption TEXT("Caption")//Ӳ������
#define DISK_Size TEXT("Size")//Ӳ�̴�С
#define DISK_Mode TEXT("Model")//Ӳ�����к�
#define DISK_InterFacrType TEXT("InterfaceType")//�ӿ�����
#define DISK_TimeCount TEXT("VendorSpecific")//Ӳ��ʹ��ʱ��
#define DISK_MySerNumber TEXT("MySerNumber")//Ӳ�����к�
#define DISK_MyCache TEXT("MyCache")//Ӳ�̻���

#define HARDINFOR_GRAPHICS TEXT("Win32_VideoController") //�Կ���Ϣ
#define GRAPHICE_Caption TEXT("Caption")//�Կ�����
#define GRAPHICE_AdapterRAM TEXT("AdapterRAM")//�Դ��С
#define GRAPHICE_VideoProcessor TEXT("VideoProcessor")//�ں�����
#define GRAPHICE_DriverVersion TEXT("DriverVersion")//�����汾

#define HARDINFOR_NETWORK TEXT("Win32_NetworkAdapterConfiguration") //������Ϣ

#define HARDINFOR_MEMORY TEXT("Win32_PhysicalMemory")//�ڴ���Ϣ
#define MEMORY_Capacity TEXT("Capacity")//�ڴ��С
#define MEMORY_DeviceLocator TEXT("DeviceLocator")//���
#define MEMORY_TotalWidth TEXT("TotalWidth")//���ݿ��


#define HARDINFOR_MAINBOARD TEXT("Win32_BaseBoard") //������Ϣ
#define MAINBOARD_Manfacturer TEXT("Manufacturer")//������
#define MAINBOARD_Product TEXT("Product")//�����ͺ�
#define MAINBOARD_BIOS_ReleaseDate TEXT("BIOS_ReleaseDate")//��������
#define MAINBOARD_BIOS_Manufacturer TEXT("BIOS_Manufacturer")//BIOS����
#define MAINBOARD_BIOS_Name TEXT("BIOS_Name")//BIOS �汾
#define MAINBOARD_SerialNumber_BIOS TEXT("SerialNumber_BIOS")//�������к�


#define HARDINFOR_DISPLAY TEXT("Win32_DisplayConfiguration") //��ʾ����Ϣ
#define DISPLAY_ScreenWidthXScreenHeight TEXT("")//��ǰ�ֱ���

#define HARDINFOR_SOUNDCARD TEXT("Win32_SoundDevice") //������Ϣ
#define SOUNDCARD_Caption TEXT("Caption")//��������
#define SOUNDCARD_Manfacturer TEXT("Manfacturer")//��������

#define HARDINFOR_ADAPTER TEXT("Win32_MyAdapter")//����������
#define ADAPTER_MyCompant TEXT("MyCompant")//����������
#define ADAPTER_MyProduct TEXT("MyProduct")//����������
#define ADAPTER_IPAddress TEXT("IPAddress")//IP��ַ
#define ADAPTER_MacAddress TEXT("MacAddress")//Mac��ַ

#define SOFT_SYSTEM TEXT("Win32_OperatingSystem")//����ϵͳ��Ϣ
#define SYSTEM_Install TEXT("InstallDate")//��װʱ��
#define SYSTEM_Language TEXT("OSLanguage")//����
#define SYSTEM_Type TEXT("Caption")//ϵͳ����
#define SYSTEM_Name TEXT("CSName")

//�ڲ���������
/*================================================================  
*  
* �� �� ����GetWmiInfo  
*  
* �Ρ�������
*   
* ����ֵ	
*
* ��������: WMI��������
*  
*  
* �� �� ֵ�� 
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/01/13  
*  
================================================================*/  
HRESULT GetWmiInfo(LPTSTR lpszClass,LPTSTR lpTag,pHardWareInformation pLink,int &iCount);
/*================================================================  
*  
* �� �� ����DoIdentify  
*  
* �Ρ�������
*   
* ����ֵ	
*
* ��������: Ӳ����Ϣ��������
*  
*  
* �� �� ֵ�� 
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/01/13  
*  
================================================================*/  
BOOL __fastcall DoIdentify( HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP,PSENDCMDOUTPARAMS pSCOP, BYTE btIDCmd, BYTE btDriveNum,PDWORD pdwBytesReturned);
/*================================================================  
*  
* �� �� ����ConvertToString  
*  
* �Ρ�������
*   
* ����ֵ	
*
* ��������: Ӳ����Ϣ��������
*  
*  
* �� �� ֵ�� 
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/01/13  
*  
================================================================*/  
char *__fastcall ConvertToString(DWORD dwDiskData[256], int nFirstIndex, int nLastIndex);
/*================================================================  
*  
* �� �� ����DoIdentify  
*  
* �Ρ�������
*   
* ����ֵ	
*
* ��������: Ӳ����Ϣ��������
*  
*  
* �� �� ֵ�� 
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/01/13  
*  
================================================================*/  
int GetDiskSerAndCashe(pHardWareInformation pLink,int iType,int &iCount);

/*================================================================  
*  
* �� �� ����_getWmiInfo  
*  
* �Ρ�������
*   
* ����ֵ	
*
* ��������: WMI��������
*  
*  
* �� �� ֵ�� 
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/01/13  
*  
================================================================*/ 
LRESULT _getWmiInfo(IWbemClassObject *pClassObject,LPTSTR lpTag,pHardWareInformation pLink,int &iCount);

/*================================================================  
*  
* �� �� ����HdwGetInfo  
*  
* �Ρ�������ClassSign 
*   
* ����ֵ	
*
* ��������: ��ȡӲ�������Ϣ
*  
*  
* �� �� ֵ�� 
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/01/13  
*  
================================================================*/  

void __stdcall HdwGetInfo(LPTSTR ClassSign,LPTSTR lpTag,pHardWareInformation &pLink,int &iCount);

/*================================================================  
*  
* �� �� ����SysGetProcessInfo  
*  
* �Ρ������� 
*   
* ����ֵ
*
* ��������: ��ȡ���������Ϣ�����Ϣ
*  
*  
* �� �� ֵ�� -1 ���� 0�ɹ�
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/01/17  
*  
================================================================*/ 
int __stdcall SysGetProcessInfo();

/*================================================================  
*  
* �� �� ����SysGetThreadInfo  
*  
* �Ρ������� DWORD dwPID 
*   
* ����ֵ
*
* ��������: ��ȡ�߳������Ϣ�����Ϣ
*  
*  
* �� �� ֵ�� -1 ���� 0�ɹ�
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/01/17  
*  
================================================================*/ 

int __stdcall SysGetThreadInfo(DWORD dwPID);

/*================================================================  
*  
* �� �� ����SysGetModuleInfo  
*  
* �Ρ������� DWORD dwPID 
*   
* ����ֵ
*
* ��������: ��ȡģ�������Ϣ�����Ϣ
*  
*  
* �� �� ֵ�� -1 ���� 0�ɹ�
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/01/17  
*  
================================================================*/ 

int __stdcall SysGetModuleInfo(DWORD dwPID );

/*================================================================  
*  
* �� �� ����UsbClearTrace  
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������: ɾ��USBģ�������Ϣ
*  
*  
* �� �� ֵ�� -1 ���� 0�ɹ�
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/01/17  
*  
================================================================*/ 

int __stdcall UsbClearTrace();

/*================================================================  
*  
* �� �� ����QueryKey  
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������: ö������
*  
*  
* �� �� ֵ�� 
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/01/18 
*  
================================================================*/ 

void  QueryKey(HKEY hKey) ;

/*=================================0==============================  
*  
* �� �� ����WebClearIETempFile  
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������: ���IE��ʱ�ļ�
*  
*  
* �� �� ֵ�� TRUE �ɹ� FALSE ʧ��
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/01/20 
*  
================================================================*/ 
BOOL __stdcall WebClearIETempFile();

/*================================1===============================  
*  
* �� �� ����WebClearAddrHistoryInDir  
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������: �����ַ��ʷ��¼
*  
*  
* �� �� ֵ�� 
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/01/20 
*  
================================================================*/
HRESULT __stdcall WebClearAddrHistoryInDir();

/*================================2===============================  
*  
* �� �� ����NetClearOnlineHistory
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������: �������������ʷ��¼
*  
*  
* �� �� ֵ�� 
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/01/20 
*  
================================================================*/

int __stdcall NetClearOnlineHistory();

/*=================================3==============================  
*  
* �� �� ����SysClearTempFiles
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������: ���ϵͳ��ʱ�ļ���
*  
*  
* �� �� ֵ�� 
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/01/20 
*  
================================================================*/
int __stdcall SysClearTempFiles();

/*===================================4============================  
*  
* �� �� ����SysClearFavorites 
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������: ����ղؼ��е�����
*  
*  
* �� �� ֵ�� 
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/01/20 
*  
================================================================*/
int __stdcall SysClearFavorites();

/*==================================5=============================  
*  
* �� �� ����WebClearAddrHistoryInIECombo 
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������: �������ַ����ʷ��ַ�����
*  
*  
* �� �� ֵ�� 
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/01/20 
*  
================================================================*/

int __stdcall WebClearAddrHistoryInIECombo();

/*===============================6================================  
*  
* �� �� ����WebClearCookie
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������: Cookie���
*  
*  
* �� �� ֵ�� 
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/01/20 
*  
================================================================*/

int __stdcall WebClearCookie();

/*===============================7================================  
*  
* �� �� ����DeleteUrlCache
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������: ɾ��Cookie��������
*  
*  
* �� �� ֵ�� 
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/01/20 
*  
================================================================*/
BOOL  DeleteUrlCache(DEL_CACHE_TYPE type);
/*===============================8================================  
*  
* �� �� ����EmptyDirectory
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������: ɾ��Ŀ¼�ļ���������
*  
*  
* �� �� ֵ�� 
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/01/20 
*  
================================================================*/
BOOL EmptyDirectory(LPCTSTR szPath, BOOL bDeleteDesktopIni = FALSE, BOOL bWipeIndexDat = FALSE);


/*===============================9================================  
*  
* �� �� ����WipeFile
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������: ����index.dat�ļ�
*  
*  
* �� �� ֵ�� 
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/01/20 
*  
================================================================*/
BOOL  WipeFile(LPCTSTR szDir, LPCTSTR szFile);

/*===============================10================================  
*  
* �� �� ����DLL_Office_ClearWordHistory
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������: ���word����ʷ��¼
*  
*  
* �� �� ֵ�� 
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/01/21 
*  
================================================================*/
int __stdcall DLL_Office_ClearWordHistory();

/*===============================11================================  
*  
* �� �� ����SetSecurityStrategy()
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������: 
*  
*  
* �� �� ֵ�� 
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/01/24 
*  
================================================================*/
int __stdcall SetSecurityStrategy();

/*===============================12================================  
*  
* �� �� ����GetSecurityStrategy()
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������: 
*  
*  
* �� �� ֵ�� 
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/01/24 
*  
================================================================*/
int __stdcall GetSecurityStrategy();

/*===============================13================================  
*  
* �� �� ����SysGetPwdAlterTime()
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������: ��ȡ���һ���޸������ʱ����
*  
*  
* �� �� ֵ�� 
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/01/24 
*  
================================================================*/

int __stdcall SysGetPwdAlterTime();

/*===============================14================================  
*  
* �� �� ����SysGetWinPwdLen()
*  
* �Ρ�������
*   
* ����ֵ 
*
* ��������: ��ȡ�޸�����ĳ���
*   
* �� �� ֵ��  
*  
* �׳��쳣��  
*  
* �������ߣ�FC 2011/01/24 
*  
================================================================*/

int __stdcall SysGetWinPwdLen();

/*===============================15================================  
*  
* �� �� ����SysGetWinLockTime()
*  
* �Ρ�������
*   
* ����ֵ 
*
* ��������: ��ȡ������ʱ����
*   
* �� �� ֵ��  
*  
* �׳��쳣��  
*  
* �������ߣ�FC 2011/01/24 
*  
================================================================*/

int __stdcall SysGetWinLockTime();

/*================================================================  
*  
* �� �� ����ResCreateLink()
*  
* �Ρ�������
*   
* ����ֵ 
*
* ��������: ����Ӳ����Ϣ����
*   
* �� �� ֵ������ָ�����NULL 
*  
* �׳��쳣��  
*  
* �������ߣ�FC 2011/01/24 
*  
================================================================*/
 pHardWareInformation __stdcall ResCreateLink(pHardWareInformation &pLink);

/*================================================================  
*  
* �� �� ����ResDeleteLink()
*  
* �Ρ�������
*   
* ����ֵ 
*
* ��������: ɾ��Ӳ����Ϣ����
*   
* �� �� ֵ���ɹ���TRUE ʧ�ܣ�FALSE
*  
* �׳��쳣��  
*  
* �������ߣ�FC 2011/01/24 
*  
================================================================*/
BOOL __stdcall ResDeleteLink(pHardWareInformation &pLink);
/*================================================================  
*  
* �� �� ����GetNicInformation()
*  
* �Ρ�������
*   
* ����ֵ 
*
* ��������: ��ȡ������������Ϣ
*   
* �� �� ֵ���ɹ���TRUE ʧ�ܣ�FALSE
*  
* �׳��쳣��  
*  
* �������ߣ�FC 2011/01/24 
*  
================================================================*/
int GetNicInformation(pHardWareInformation &pLinkint ,int iType,int ivariety,int &iCount);
/*================================================================  
*  
* �� �� ����int SysGetWinInstallTime()
*  
* �Ρ�������
*   
* ����ֵ 
*
* ��������: ��ȡϵͳ��װʱ��
*   
* �� �� ֵ���ɹ�����װʱ�� ʧ�ܣ�-1
*  
* �׳��쳣��  
*  
* �������ߣ�FC 2011/01/24 
*  
================================================================*/
int SysGetWinInstallTime(pHardWareInformation &pLink);
/*================================================================  
*  
* �� �� ����int SysGetWinLaguage()
*  
* �Ρ�������
*   
* ����ֵ 
*
* ��������: ��ȡϵͳ����
*   
* �� �� ֵ���ɹ�����װʱ�� ʧ�ܣ�-1
*  
* �׳��쳣��  
*  
* �������ߣ�FC 2011/01/24 
*  
================================================================*/
int SysGetWinLaguage(pHardWareInformation &pLink);
/*================================================================  
*  
* �� �� ����int SysGetWinProductName()
*  
* �Ρ�������
*   
* ����ֵ 
*
* ��������: ��ȡϵͳ����
*   
* �� �� ֵ���ɹ�����װʱ�� ʧ�ܣ�-1
*  
* �׳��쳣��  
*  
* �������ߣ�FC 2011/01/24 
*  
================================================================*/
int SysGetWinProductName(pHardWareInformation &pLink);
/*================================================================  
*  
* �� �� ����int SysGetWinHostName()
*  
* �Ρ�������
*   
* ����ֵ 
*
* ��������: ��ȡϵͳ��
*   
* �� �� ֵ���ɹ�����װʱ�� ʧ�ܣ�-1
*  
* �׳��쳣��  
*  
* �������ߣ�FC 2011/01/24 
*  
================================================================*/
int SysGetWinHostName(pHardWareInformation &pLink);
/*================================================================  
*  
* �� �� ����int SysGetLocalNic()
*  
* �Ρ�������
*   
* ����ֵ ivariety:0 ����NIC 1 ����NIC�����������
*
* ��������: ��ȡ����NIC�����Ϣ
*   
* �� �� ֵ���ɹ�����װʱ�� ʧ�ܣ�-1
*  
* �׳��쳣��  
*  
* �������ߣ�FC 2011/01/24 
*  
================================================================*/
int SysGetLocalNic(pHardWareInformation &pLink,int & itype,int ivariety,int& iCount);


/*================================================================  
*  
* �� �� ����int AdValidation()
*  
* �Ρ�������
*   
* ����ֵ 
*
* ��������: ��֤���û���¼��֤
*   
* �� �� ֵ���ɹ���0 ʧ�ܣ�-1
*  
* �׳��쳣��  
*  
* �������ߣ�FC 2011/01/24 
*  
================================================================*/
int AdValidation(TCHAR* szUserName,TCHAR* szPwd,TCHAR* szDomain);

/*================================================================  
*  
* �� �� ����int AdGetDomInfor()
*  
* �Ρ�������
*   
* ����ֵ 
*
* ��������: ��ȡ����֯�ṹ�����Ϣ
*   
* �� �� ֵ���ɹ���0 ʧ�ܣ�-1
*  
* �׳��쳣��  
*  
* �������ߣ�FC 2011/01/24 
*  
================================================================*/
int AdGetDomInfor(pHardWareInformation &pLink,TCHAR* szUserName,TCHAR* szDepart,int & icount);

/*================================================================  
*  
* �� �� ����AdGetDomName
*  
* �Ρ�������
*   
* ����ֵ 
*
* ��������: ��ȡ�����֣��ڲ�ʹ��
*   
* �� �� ֵ���ɹ���0 ʧ�ܣ�-1
*  
* �׳��쳣��  
*  
* �������ߣ�FC 2011/01/24 
*  
================================================================*/
int AdGetDomName(TCHAR* szDomName);

/*================================================================  
*  
* �� �� ����InsertDomaUser
*  
* �Ρ�������
*   
* ����ֵ 
*
* ��������: ������Ϣ��������
*   
* �� �� ֵ���ɹ���0 ʧ�ܣ�-1
*  
* �׳��쳣��  
*  
* �������ߣ�FC 2011/01/24 
*  
================================================================*/
int InsertDomaUser(pHardWareInformation &pLink,WCHAR* szLADP,WCHAR* szVar,int & icount);

/*================================================================  
*  
* �� �� ����InsertDomaDepart
*  
* �Ρ�������
*   
* ����ֵ 
*
* ��������: ������Ϣ��������
*   
* �� �� ֵ���ɹ���0 ʧ�ܣ�-1
*  
* �׳��쳣��  
*  
* �������ߣ�FC 2011/01/24 
*  
================================================================*/
int InsertDomaDepart(pHardWareInformation &pLink,WCHAR* szLADP,int & icount);

/*================================================================  
*  
* �� �� ����GetAdInformation
*  
* �Ρ�������
*   
* ����ֵ 
*
* ��������: �����ṩ��ȡAD��Ϣ
*   
* �� �� ֵ���ɹ���0 ʧ�ܣ�-1
*  
* �׳��쳣��  
*  
* �������ߣ�FC 2011/01/24 
*  
================================================================*/
//typedef int (WINAPI *ADFun)(string strtype,int itype,int& icount);

int __stdcall GetAdToExport(string strInfo,int itype/*,ADFun AdCallBackFun*/);
/*================================================================  
*  
* �� �� ����int AccountIsEmptyPassword(TCHAR* domainname, TCHAR* username)
*  
* �Ρ�������
*   
* ����ֵ 
*
* ��������: ��ȡϵͳ�ʻ������Ƿ�Ϊ��
*   
* �� �� ֵ���ɹ���0 ʧ�ܣ�-1
*  
* �׳��쳣��  
*  
* �������ߣ�FC 2011/01/24 
*  
================================================================*/
int __stdcall AccountIsEmptyPassword(TCHAR* domainname, TCHAR* username);

/*================================================================  
*  
* �� �� ����int GetOtherEmptyPwd(TCHAR* domainname, TCHAR* username)
*  
* �Ρ�������
*   
* ����ֵ 
*
* ��������: ��ȡϵͳ�ʻ������Ƿ�Ϊ��
*   
* �� �� ֵ���ɹ���0 ʧ�ܣ�-1
*  
* �׳��쳣��  
*  
* �������ߣ�FC 2011/01/24 
*  
================================================================*/
int __stdcall GetOtherEmptyPwd(TCHAR* szDomain, TCHAR* szUserName);
/*================================================================  
*  
* �� �� ����AccountIsForbid
*  
* �Ρ�������
*   
* ����ֵ 
*
* ��������: ��ȡ�ʻ��Ƿ񱻽�ֹ
*   
* �� �� ֵ��
*  
* �׳��쳣��  
*  
* �������ߣ�FC 2011/01/24 
*  
================================================================*/
int __stdcall AccountIsForbid(long lType,TCHAR* szName);

/*================================================================  
*  
* �� �� ����SysGetUserModelInfo
*  
* �Ρ�������
*   
* ����ֵ 
*
* ��������: ��ȡϵͳ���������Ϣ
*   
* �� �� ֵ��
*  
* �׳��쳣��  
*  
* �������ߣ�FC 2011/01/24 
*  
================================================================*/
int __stdcall SysGetUserModelInfo(int& iMaxPwd,int& iLockout_dur,int& iLockout_threshold);

/*================================================================  
*  
* �� �� ����SysServicesIsIn
*  
* �Ρ�������
*   
* ����ֵ 
*
* ��������: ��ȡ�����Ƿ����
*   
* �� �� ֵ���ɹ���0 ʧ�ܣ�-1
*  
* �׳��쳣��  
*  
* �������ߣ�FC 2011/01/24 
*  
================================================================*/
int __stdcall SysServicesIsIn(TCHAR* szServiceName);
/*================================================================  
*  
* �� �� ����SysCDROMIsForbit
*  
* �Ρ�������
*   
* ����ֵ 
*
* ��������: ��ȡ�����Ƿ񱻽�����Ϣ
*   
* �� �� ֵ���ɹ���0 ʧ�ܣ�-1
*  
* �׳��쳣��  
*  
* �������ߣ�FC 2011/01/24 
*  
================================================================*/
int __stdcall SysCDROMIsForbit();

/*================================================================  
*  
* �� �� ����SysGetShareName
*  
* �Ρ�������
*   
* ����ֵ 
*
* ��������: ��ȡ����Ŀ¼����
*   
* �� �� ֵ���ɹ���0 ʧ�ܣ�-1
*  
* �׳��쳣��  
*  
* �������ߣ�FC 2011/01/24 
*  
================================================================*/
int __stdcall SysGetShareName(pHardWareInformation &pLink,int& iCount);

/*================================================================  
*  
* �� �� ����GetUsbDeviceInfo
*  
* �Ρ�������
*   
* ����ֵ 
*
* ��������: ��ȡ�����USB�豸��μ�¼
*   
* �� �� ֵ���ɹ���0 ʧ�ܣ�-1
*  
* �׳��쳣��  
*  
* �������ߣ�FC 2011/10/19 
*  
================================================================*/
int __stdcall GetUsbDeviceInfo(pHardWareInformation &pLink,int& iCount,WCHAR* szReg,int iType);

/*================================================================  
*  
* �� �� ����GetRegKeyName  
*  
* �Ρ�������HKEY hKey:��������
*   
* ����ֵ
*
* ��������: ö��USB�洢�豸����
*  
* �� �� ֵ�� �ɹ���0 ʧ�ܣ�-1
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/10/19 
*  
================================================================*/ 
int  GetRegKeyName(HKEY hKey,pHardWareInformation &pLink,int& iCount,int iType);
//int  GetRegKeyName(HKEY hKey,TCHAR* szPidValue,TCHAR* szNameValue,TCHAR* szIDValue);

/*================================================================  
*  
* �� �� ����GetCountRegKey  
*  
* �Ρ�������HKEY hKey:��������
*   
* ����ֵ
*
* ��������: ��ȡUSB�洢�豸��Ϣ����
*  
* �� �� ֵ�� �ɹ���0 ʧ�ܣ�-1
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/10/19 
*  
================================================================*/ 
//int  GetCountRegKey(HKEY hKey,pHardWareInformation &pLink,int& iCount);

/*================================================================  
*  
* �� �� ����GetMediaNormal
*  
* �Ρ�������
*   
* ����ֵ 
*
* ��������: �жϽ����Ƿ����
*   
* �� �� ֵ���ɹ���0 ʧ�ܣ�-1
*  
* �׳��쳣��  
*  
* �������ߣ�FC 2011/10/19 
*  
================================================================*/
int __stdcall GetMediaNormal(TCHAR* szExeName);

/*================================================================  
*  
* �� �� ����GetServicesNormal
*  
* �Ρ�������
*   
* ����ֵ 
*
* ��������: �жϷ����Ƿ�����
*   
* �� �� ֵ���ɹ���0 ʧ�ܣ�-1
*  
* �׳��쳣��  
*  
* �������ߣ�FC 2011/10/19 
*  
================================================================*/
int __stdcall GetServicesNormal(TCHAR* szServicesName);

/*================================================================  
*  
* �� �� ����GetPatchInfo  
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������: ö�ٲ�����Ϣ
*  
* �� �� ֵ�� �ɹ���0 ʧ�ܣ�-1
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/10/19 
*  
================================================================*/ 
int __stdcall GetPatchInfo(pHardWareInformation &pLink,int& iCount);

/*================================================================  
*  
* �� �� ����GetLionIfo  
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������: ��ȡ����ɱ�������Ϣ��ʨ�ӣ�lion��
*  
* �� �� ֵ�� �ɹ���0 ʧ�ܣ�-1
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/10/19 
*  
================================================================*/ 
int __stdcall GetLionIfo(pHardWareInformation &pLink,int& iCount);

/*================================================================  
*  
* �� �� ����GetDoubleSystem
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������:�ж��Ƿ�װ��˫ϵͳ
*  
* �� �� ֵ�� �ɹ������ذ�װ�Ĳ���ϵͳ���� ʧ�ܣ�-1
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/10/19 
*  
================================================================*/ 
int __stdcall GetDoubleSystem();

/*================================================================  
*  
* �� �� ����GetOtherAccount
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������:�ж��Ƿ��г�Administrator guest������ʻ�
*  
* �� �� ֵ�� �ɹ������ذ�װ�Ĳ���ϵͳ���� ʧ�ܣ�-1
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/10/19 
*  
================================================================*/ 
int __stdcall GetOtherAccount(pHardWareInformation &pLink,int& iCount);

/*================================================================  
*  
* �� �� ����GetWireLessModule
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������:����ģ���Ƿ����
*  
* �� �� ֵ�� ��������ģ���DeviceID
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/10/19 
*  
================================================================*/

int __stdcall GetWireLessModule(int iDeviceID,pHardWareInformation &pLink,int& iCount);

int GetAdaWirelessValue(HKEY hKey,int & iRetValue);

/*================================================================  
*  
* �� �� ����GetExeFileName(TCHAR* szExeName)
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������:����ģ���Ƿ����
*  
* �� �� ֵ�� ��������ģ���DeviceID
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/10/19 
*  
================================================================*/

int __stdcall GetExeFileName(TCHAR* szDir,TCHAR* szExeName);

/*================================================================  
*  
* �� �� ����GetOtherSerNo(pHardWareInformation &pLink,int& iCount)
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������:��һ�ֻ�ȡӲ�����к�
*  
* �� �� ֵ�� ����Ӳ�����к�
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/10/19 
*  
================================================================*/

int __stdcall GetOtherSerNo(pHardWareInformation &pLink,int& iCount);

/*================================================================  
*  
* �� �� ����GetOtherShareFile(pHardWareInformation &pLink,int& iCount)
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������:��һ�ֻ�ȡ�����ļ�
*  
* �� �� ֵ�� �����ļ�
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/10/19 
*  
================================================================*/

int __stdcall GetOtherShareFile(pHardWareInformation &pLink,int& iCount);

/*================================================================  
*  
* �� �� ����GetOtherInstallTime(pHardWareInformation &pLink,int& iCount)
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������:��һ�ֻ�ȡϵͳ��װʱ��
*  
* �� �� ֵ�� �����ļ�
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/10/19 
*  
================================================================*/

int __stdcall GetOtherInstallTime(pHardWareInformation &pLink,int& iCount);


/*================================================================  
*  
* �� �� ����GetOtherSysType(pHardWareInformation &pLink,int& iCount)
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������:��һ�ֻ�ȡ����ϵͳ����
* �� �� ֵ�� �����ļ�
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/10/19 
*  
================================================================*/

int __stdcall GetOtherSysType(pHardWareInformation &pLink,int& iCount);

/*================================================================  
*  
* �� �� ����GetSofeWareInfo(pHardWareInformation &pLink,int& iCount)
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������: ��ȡϵͳ�Ѱ�װ���
* �� �� ֵ���Ѱ�װ�����ֻ������ע����а�������Ϣ��
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/10/19 
*  
================================================================*/

int __stdcall GetSofeWareInfo(pHardWareInformation &pLink,int& iCount,int iDerial/*����*/);

/*================================================================  
*  
* �� �� ����GetPatchInfoNext(pHardWareInformation &pLink,int& iCount)
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������: ��ȡϵͳ��������
* �� �� ֵ��
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/10/19 
*  
================================================================*/
int __stdcall GetPatchInfoNext(pHardWareInformation &pLink,int& iCount);

/*================================================================  
*  
* �� �� ����GetKillVirusInfo(TCHAR* szTime,pHardWareInformation &pLink,int& iCount)
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������: ��ȡ����ɱ�����������ɱ��Ϣ ������ɱ���ļ�����·����ʱ��
* �� �� ֵ��
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/10/19 
*  
================================================================*/
int __stdcall GetKillVirusInfo(TCHAR* szTime,pHardWareInformation &pLink,int& iCount);
/*================================================================  
*  
* �� �� ����GetOtherDiskSer(pHardWareInformation &pLink,int& iCount)
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������: ��һ�ַ�����ȡӲ�����к�
* �� �� ֵ��
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/10/19 
*  
================================================================*/
int __stdcall GetOtherDiskSer(pHardWareInformation &pLink,int& iCount);
/*================================================================  
*  
* �� �� ����GetOtherCDROMInfo(pHardWareInformation &pLink,int& iCount)
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������: ��һ�ֻ�ȡCDROM����
* �� �� ֵ��
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/10/19 
*  
================================================================*/
int __stdcall GetOtherCDROMInfo(pHardWareInformation &pLink,int& iCount);

/*================================================================  
*  
* �� �� ����GetLogInfo(pHardWareInformation &pLink,int& iCount)
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������: ��ȡ��־��Ϣ
* �� �� ֵ��
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2011/10/19 
*  
================================================================*/
int __stdcall GetLogInfo(pHardWareInformation &pLink,int& iCount);
/*================================================================  
*  
* �� �� ����GetPatchInfoLastTime(pHardWareInformation &pLink,int& iCount)
*  
* �Ρ�������
*   
* ����ֵ
*
* ��������: ��ȡϵͳ�������װʱ��
* �� �� ֵ��
*  
* �׳��쳣�� 
*  
* �������ߣ�FC 2013/08/16 
*  
================================================================*/
int __stdcall GetPatchInfoLastTime(pHardWareInformation &pLink,int& iCount);

#endif
