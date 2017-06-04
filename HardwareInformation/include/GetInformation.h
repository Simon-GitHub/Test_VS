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

enum DEL_CACHE_TYPE //要删除的类型
{
	File,			//表示internet临时文件
	Cookie			//表示Cookie
};
///////////////////////////////////
//硬件信息链表

typedef struct HardwareInformation
{
	TCHAR strName[MAX_PATH/4];
	TCHAR str[MAX_PATH/2];
	struct HardwareInformation *next;

}HardwareInformation,*pHardWareInformation;


#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

#define HARDINFOR_PROCESSOR TEXT("Win32_Processor") //CPU信息
#define PROCESSOR_Manfacturer TEXT("Manufacturer")//CPU厂商
#define PROCESSOR_MaxClockSpeed TEXT("MaxClockSpeed")//CPU默认频率
#define PROCESSOR_ExtClock TEXT("ExtClock")//CPU外频
#define PROCESSOR_CurrentClockSpeed TEXT("CurrentClockSpeed")//CPU当前频率
#define PROCESSOR_LzCacheSize TEXT("L2CacheSize")//CPU二级缓存
#define PROCESSOR_CurrentVoltage TEXT("CurrentVoltage")//CPU电压
#define PROCESSOR_ProcessorId TEXT("ProcessorId")//CPU序列号
#define PROCESSOR_DataWidth TEXT("DataWidth")//数据宽度
#define PROCESSOR_MyCPUType TEXT("MyType")//数据类型


#define HARDINFOR_DISK TEXT("Win32_DiskDrive") //硬盘信息
#define DISK_Caption TEXT("Caption")//硬盘名称
#define DISK_Size TEXT("Size")//硬盘大小
#define DISK_Mode TEXT("Model")//硬盘序列号
#define DISK_InterFacrType TEXT("InterfaceType")//接口类型
#define DISK_TimeCount TEXT("VendorSpecific")//硬盘使用时间
#define DISK_MySerNumber TEXT("MySerNumber")//硬盘序列号
#define DISK_MyCache TEXT("MyCache")//硬盘缓存

#define HARDINFOR_GRAPHICS TEXT("Win32_VideoController") //显卡信息
#define GRAPHICE_Caption TEXT("Caption")//显卡名称
#define GRAPHICE_AdapterRAM TEXT("AdapterRAM")//显存大小
#define GRAPHICE_VideoProcessor TEXT("VideoProcessor")//内核名称
#define GRAPHICE_DriverVersion TEXT("DriverVersion")//驱动版本

#define HARDINFOR_NETWORK TEXT("Win32_NetworkAdapterConfiguration") //网卡信息

#define HARDINFOR_MEMORY TEXT("Win32_PhysicalMemory")//内存信息
#define MEMORY_Capacity TEXT("Capacity")//内存大小
#define MEMORY_DeviceLocator TEXT("DeviceLocator")//插槽
#define MEMORY_TotalWidth TEXT("TotalWidth")//数据宽度


#define HARDINFOR_MAINBOARD TEXT("Win32_BaseBoard") //主板信息
#define MAINBOARD_Manfacturer TEXT("Manufacturer")//制造商
#define MAINBOARD_Product TEXT("Product")//主板型号
#define MAINBOARD_BIOS_ReleaseDate TEXT("BIOS_ReleaseDate")//制造日期
#define MAINBOARD_BIOS_Manufacturer TEXT("BIOS_Manufacturer")//BIOS厂商
#define MAINBOARD_BIOS_Name TEXT("BIOS_Name")//BIOS 版本
#define MAINBOARD_SerialNumber_BIOS TEXT("SerialNumber_BIOS")//主板序列号


#define HARDINFOR_DISPLAY TEXT("Win32_DisplayConfiguration") //显示器信息
#define DISPLAY_ScreenWidthXScreenHeight TEXT("")//当前分辨率

#define HARDINFOR_SOUNDCARD TEXT("Win32_SoundDevice") //声卡信息
#define SOUNDCARD_Caption TEXT("Caption")//声卡名称
#define SOUNDCARD_Manfacturer TEXT("Manfacturer")//声卡厂商

#define HARDINFOR_ADAPTER TEXT("Win32_MyAdapter")//网络适配器
#define ADAPTER_MyCompant TEXT("MyCompant")//适配器厂商
#define ADAPTER_MyProduct TEXT("MyProduct")//适配器名称
#define ADAPTER_IPAddress TEXT("IPAddress")//IP地址
#define ADAPTER_MacAddress TEXT("MacAddress")//Mac地址

#define SOFT_SYSTEM TEXT("Win32_OperatingSystem")//操作系统信息
#define SYSTEM_Install TEXT("InstallDate")//安装时间
#define SYSTEM_Language TEXT("OSLanguage")//语言
#define SYSTEM_Type TEXT("Caption")//系统类型
#define SYSTEM_Name TEXT("CSName")

//内部函数调用
/*================================================================  
*  
* 函 数 名：GetWmiInfo  
*  
* 参　　数：
*   
* 参数值	
*
* 功能描述: WMI辅助函数
*  
*  
* 返 回 值： 
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/01/13  
*  
================================================================*/  
HRESULT GetWmiInfo(LPTSTR lpszClass,LPTSTR lpTag,pHardWareInformation pLink,int &iCount);
/*================================================================  
*  
* 函 数 名：DoIdentify  
*  
* 参　　数：
*   
* 参数值	
*
* 功能描述: 硬盘信息辅助函数
*  
*  
* 返 回 值： 
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/01/13  
*  
================================================================*/  
BOOL __fastcall DoIdentify( HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP,PSENDCMDOUTPARAMS pSCOP, BYTE btIDCmd, BYTE btDriveNum,PDWORD pdwBytesReturned);
/*================================================================  
*  
* 函 数 名：ConvertToString  
*  
* 参　　数：
*   
* 参数值	
*
* 功能描述: 硬盘信息辅助函数
*  
*  
* 返 回 值： 
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/01/13  
*  
================================================================*/  
char *__fastcall ConvertToString(DWORD dwDiskData[256], int nFirstIndex, int nLastIndex);
/*================================================================  
*  
* 函 数 名：DoIdentify  
*  
* 参　　数：
*   
* 参数值	
*
* 功能描述: 硬盘信息辅助函数
*  
*  
* 返 回 值： 
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/01/13  
*  
================================================================*/  
int GetDiskSerAndCashe(pHardWareInformation pLink,int iType,int &iCount);

/*================================================================  
*  
* 函 数 名：_getWmiInfo  
*  
* 参　　数：
*   
* 参数值	
*
* 功能描述: WMI辅助函数
*  
*  
* 返 回 值： 
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/01/13  
*  
================================================================*/ 
LRESULT _getWmiInfo(IWbemClassObject *pClassObject,LPTSTR lpTag,pHardWareInformation pLink,int &iCount);

/*================================================================  
*  
* 函 数 名：HdwGetInfo  
*  
* 参　　数：ClassSign 
*   
* 参数值	
*
* 功能描述: 获取硬件相关信息
*  
*  
* 返 回 值： 
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/01/13  
*  
================================================================*/  

void __stdcall HdwGetInfo(LPTSTR ClassSign,LPTSTR lpTag,pHardWareInformation &pLink,int &iCount);

/*================================================================  
*  
* 函 数 名：SysGetProcessInfo  
*  
* 参　　数： 
*   
* 参数值
*
* 功能描述: 获取进程相关信息相关信息
*  
*  
* 返 回 值： -1 错误， 0成功
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/01/17  
*  
================================================================*/ 
int __stdcall SysGetProcessInfo();

/*================================================================  
*  
* 函 数 名：SysGetThreadInfo  
*  
* 参　　数： DWORD dwPID 
*   
* 参数值
*
* 功能描述: 获取线程相关信息相关信息
*  
*  
* 返 回 值： -1 错误， 0成功
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/01/17  
*  
================================================================*/ 

int __stdcall SysGetThreadInfo(DWORD dwPID);

/*================================================================  
*  
* 函 数 名：SysGetModuleInfo  
*  
* 参　　数： DWORD dwPID 
*   
* 参数值
*
* 功能描述: 获取模块相关信息相关信息
*  
*  
* 返 回 值： -1 错误， 0成功
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/01/17  
*  
================================================================*/ 

int __stdcall SysGetModuleInfo(DWORD dwPID );

/*================================================================  
*  
* 函 数 名：UsbClearTrace  
*  
* 参　　数：
*   
* 参数值
*
* 功能描述: 删除USB模块相关信息
*  
*  
* 返 回 值： -1 错误， 0成功
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/01/17  
*  
================================================================*/ 

int __stdcall UsbClearTrace();

/*================================================================  
*  
* 函 数 名：QueryKey  
*  
* 参　　数：
*   
* 参数值
*
* 功能描述: 枚举子项
*  
*  
* 返 回 值： 
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/01/18 
*  
================================================================*/ 

void  QueryKey(HKEY hKey) ;

/*=================================0==============================  
*  
* 函 数 名：WebClearIETempFile  
*  
* 参　　数：
*   
* 参数值
*
* 功能描述: 清除IE临时文件
*  
*  
* 返 回 值： TRUE 成功 FALSE 失败
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/01/20 
*  
================================================================*/ 
BOOL __stdcall WebClearIETempFile();

/*================================1===============================  
*  
* 函 数 名：WebClearAddrHistoryInDir  
*  
* 参　　数：
*   
* 参数值
*
* 功能描述: 清除网址历史记录
*  
*  
* 返 回 值： 
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/01/20 
*  
================================================================*/
HRESULT __stdcall WebClearAddrHistoryInDir();

/*================================2===============================  
*  
* 函 数 名：NetClearOnlineHistory
*  
* 参　　数：
*   
* 参数值
*
* 功能描述: 清除网络联接历史记录
*  
*  
* 返 回 值： 
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/01/20 
*  
================================================================*/

int __stdcall NetClearOnlineHistory();

/*=================================3==============================  
*  
* 函 数 名：SysClearTempFiles
*  
* 参　　数：
*   
* 参数值
*
* 功能描述: 清除系统临时文件夹
*  
*  
* 返 回 值： 
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/01/20 
*  
================================================================*/
int __stdcall SysClearTempFiles();

/*===================================4============================  
*  
* 函 数 名：SysClearFavorites 
*  
* 参　　数：
*   
* 参数值
*
* 功能描述: 清除收藏夹中的内容
*  
*  
* 返 回 值： 
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/01/20 
*  
================================================================*/
int __stdcall SysClearFavorites();

/*==================================5=============================  
*  
* 函 数 名：WebClearAddrHistoryInIECombo 
*  
* 参　　数：
*   
* 参数值
*
* 功能描述: 浏览器地址栏历史地址的清除
*  
*  
* 返 回 值： 
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/01/20 
*  
================================================================*/

int __stdcall WebClearAddrHistoryInIECombo();

/*===============================6================================  
*  
* 函 数 名：WebClearCookie
*  
* 参　　数：
*   
* 参数值
*
* 功能描述: Cookie清除
*  
*  
* 返 回 值： 
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/01/20 
*  
================================================================*/

int __stdcall WebClearCookie();

/*===============================7================================  
*  
* 函 数 名：DeleteUrlCache
*  
* 参　　数：
*   
* 参数值
*
* 功能描述: 删除Cookie辅助函数
*  
*  
* 返 回 值： 
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/01/20 
*  
================================================================*/
BOOL  DeleteUrlCache(DEL_CACHE_TYPE type);
/*===============================8================================  
*  
* 函 数 名：EmptyDirectory
*  
* 参　　数：
*   
* 参数值
*
* 功能描述: 删除目录文件辅助函数
*  
*  
* 返 回 值： 
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/01/20 
*  
================================================================*/
BOOL EmptyDirectory(LPCTSTR szPath, BOOL bDeleteDesktopIni = FALSE, BOOL bWipeIndexDat = FALSE);


/*===============================9================================  
*  
* 函 数 名：WipeFile
*  
* 参　　数：
*   
* 参数值
*
* 功能描述: 覆盖index.dat文件
*  
*  
* 返 回 值： 
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/01/20 
*  
================================================================*/
BOOL  WipeFile(LPCTSTR szDir, LPCTSTR szFile);

/*===============================10================================  
*  
* 函 数 名：DLL_Office_ClearWordHistory
*  
* 参　　数：
*   
* 参数值
*
* 功能描述: 清楚word打开历史记录
*  
*  
* 返 回 值： 
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/01/21 
*  
================================================================*/
int __stdcall DLL_Office_ClearWordHistory();

/*===============================11================================  
*  
* 函 数 名：SetSecurityStrategy()
*  
* 参　　数：
*   
* 参数值
*
* 功能描述: 
*  
*  
* 返 回 值： 
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/01/24 
*  
================================================================*/
int __stdcall SetSecurityStrategy();

/*===============================12================================  
*  
* 函 数 名：GetSecurityStrategy()
*  
* 参　　数：
*   
* 参数值
*
* 功能描述: 
*  
*  
* 返 回 值： 
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/01/24 
*  
================================================================*/
int __stdcall GetSecurityStrategy();

/*===============================13================================  
*  
* 函 数 名：SysGetPwdAlterTime()
*  
* 参　　数：
*   
* 参数值
*
* 功能描述: 获取最后一次修改密码的时间间隔
*  
*  
* 返 回 值： 
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/01/24 
*  
================================================================*/

int __stdcall SysGetPwdAlterTime();

/*===============================14================================  
*  
* 函 数 名：SysGetWinPwdLen()
*  
* 参　　数：
*   
* 参数值 
*
* 功能描述: 获取修改密码的长度
*   
* 返 回 值：  
*  
* 抛出异常：  
*  
* 作　　者：FC 2011/01/24 
*  
================================================================*/

int __stdcall SysGetWinPwdLen();

/*===============================15================================  
*  
* 函 数 名：SysGetWinLockTime()
*  
* 参　　数：
*   
* 参数值 
*
* 功能描述: 获取锁屏的时间间隔
*   
* 返 回 值：  
*  
* 抛出异常：  
*  
* 作　　者：FC 2011/01/24 
*  
================================================================*/

int __stdcall SysGetWinLockTime();

/*================================================================  
*  
* 函 数 名：ResCreateLink()
*  
* 参　　数：
*   
* 参数值 
*
* 功能描述: 创建硬件信息链表
*   
* 返 回 值：链表指针或者NULL 
*  
* 抛出异常：  
*  
* 作　　者：FC 2011/01/24 
*  
================================================================*/
 pHardWareInformation __stdcall ResCreateLink(pHardWareInformation &pLink);

/*================================================================  
*  
* 函 数 名：ResDeleteLink()
*  
* 参　　数：
*   
* 参数值 
*
* 功能描述: 删除硬件信息链表
*   
* 返 回 值：成功：TRUE 失败：FALSE
*  
* 抛出异常：  
*  
* 作　　者：FC 2011/01/24 
*  
================================================================*/
BOOL __stdcall ResDeleteLink(pHardWareInformation &pLink);
/*================================================================  
*  
* 函 数 名：GetNicInformation()
*  
* 参　　数：
*   
* 参数值 
*
* 功能描述: 获取网络适配器信息
*   
* 返 回 值：成功：TRUE 失败：FALSE
*  
* 抛出异常：  
*  
* 作　　者：FC 2011/01/24 
*  
================================================================*/
int GetNicInformation(pHardWareInformation &pLinkint ,int iType,int ivariety,int &iCount);
/*================================================================  
*  
* 函 数 名：int SysGetWinInstallTime()
*  
* 参　　数：
*   
* 参数值 
*
* 功能描述: 获取系统安装时间
*   
* 返 回 值：成功：安装时间 失败：-1
*  
* 抛出异常：  
*  
* 作　　者：FC 2011/01/24 
*  
================================================================*/
int SysGetWinInstallTime(pHardWareInformation &pLink);
/*================================================================  
*  
* 函 数 名：int SysGetWinLaguage()
*  
* 参　　数：
*   
* 参数值 
*
* 功能描述: 获取系统语言
*   
* 返 回 值：成功：安装时间 失败：-1
*  
* 抛出异常：  
*  
* 作　　者：FC 2011/01/24 
*  
================================================================*/
int SysGetWinLaguage(pHardWareInformation &pLink);
/*================================================================  
*  
* 函 数 名：int SysGetWinProductName()
*  
* 参　　数：
*   
* 参数值 
*
* 功能描述: 获取系统类型
*   
* 返 回 值：成功：安装时间 失败：-1
*  
* 抛出异常：  
*  
* 作　　者：FC 2011/01/24 
*  
================================================================*/
int SysGetWinProductName(pHardWareInformation &pLink);
/*================================================================  
*  
* 函 数 名：int SysGetWinHostName()
*  
* 参　　数：
*   
* 参数值 
*
* 功能描述: 获取系统名
*   
* 返 回 值：成功：安装时间 失败：-1
*  
* 抛出异常：  
*  
* 作　　者：FC 2011/01/24 
*  
================================================================*/
int SysGetWinHostName(pHardWareInformation &pLink);
/*================================================================  
*  
* 函 数 名：int SysGetLocalNic()
*  
* 参　　数：
*   
* 参数值 ivariety:0 本地NIC 1 其它NIC（如虚拟机）
*
* 功能描述: 获取本地NIC相关信息
*   
* 返 回 值：成功：安装时间 失败：-1
*  
* 抛出异常：  
*  
* 作　　者：FC 2011/01/24 
*  
================================================================*/
int SysGetLocalNic(pHardWareInformation &pLink,int & itype,int ivariety,int& iCount);


/*================================================================  
*  
* 函 数 名：int AdValidation()
*  
* 参　　数：
*   
* 参数值 
*
* 功能描述: 验证域用户登录验证
*   
* 返 回 值：成功：0 失败：-1
*  
* 抛出异常：  
*  
* 作　　者：FC 2011/01/24 
*  
================================================================*/
int AdValidation(TCHAR* szUserName,TCHAR* szPwd,TCHAR* szDomain);

/*================================================================  
*  
* 函 数 名：int AdGetDomInfor()
*  
* 参　　数：
*   
* 参数值 
*
* 功能描述: 获取域组织结构相关信息
*   
* 返 回 值：成功：0 失败：-1
*  
* 抛出异常：  
*  
* 作　　者：FC 2011/01/24 
*  
================================================================*/
int AdGetDomInfor(pHardWareInformation &pLink,TCHAR* szUserName,TCHAR* szDepart,int & icount);

/*================================================================  
*  
* 函 数 名：AdGetDomName
*  
* 参　　数：
*   
* 参数值 
*
* 功能描述: 获取域名字，内部使用
*   
* 返 回 值：成功：0 失败：-1
*  
* 抛出异常：  
*  
* 作　　者：FC 2011/01/24 
*  
================================================================*/
int AdGetDomName(TCHAR* szDomName);

/*================================================================  
*  
* 函 数 名：InsertDomaUser
*  
* 参　　数：
*   
* 参数值 
*
* 功能描述: 将域信息插入链表
*   
* 返 回 值：成功：0 失败：-1
*  
* 抛出异常：  
*  
* 作　　者：FC 2011/01/24 
*  
================================================================*/
int InsertDomaUser(pHardWareInformation &pLink,WCHAR* szLADP,WCHAR* szVar,int & icount);

/*================================================================  
*  
* 函 数 名：InsertDomaDepart
*  
* 参　　数：
*   
* 参数值 
*
* 功能描述: 将域信息插入链表
*   
* 返 回 值：成功：0 失败：-1
*  
* 抛出异常：  
*  
* 作　　者：FC 2011/01/24 
*  
================================================================*/
int InsertDomaDepart(pHardWareInformation &pLink,WCHAR* szLADP,int & icount);

/*================================================================  
*  
* 函 数 名：GetAdInformation
*  
* 参　　数：
*   
* 参数值 
*
* 功能描述: 向外提供获取AD信息
*   
* 返 回 值：成功：0 失败：-1
*  
* 抛出异常：  
*  
* 作　　者：FC 2011/01/24 
*  
================================================================*/
//typedef int (WINAPI *ADFun)(string strtype,int itype,int& icount);

int __stdcall GetAdToExport(string strInfo,int itype/*,ADFun AdCallBackFun*/);
/*================================================================  
*  
* 函 数 名：int AccountIsEmptyPassword(TCHAR* domainname, TCHAR* username)
*  
* 参　　数：
*   
* 参数值 
*
* 功能描述: 获取系统帐户密码是否为空
*   
* 返 回 值：成功：0 失败：-1
*  
* 抛出异常：  
*  
* 作　　者：FC 2011/01/24 
*  
================================================================*/
int __stdcall AccountIsEmptyPassword(TCHAR* domainname, TCHAR* username);

/*================================================================  
*  
* 函 数 名：int GetOtherEmptyPwd(TCHAR* domainname, TCHAR* username)
*  
* 参　　数：
*   
* 参数值 
*
* 功能描述: 获取系统帐户密码是否为空
*   
* 返 回 值：成功：0 失败：-1
*  
* 抛出异常：  
*  
* 作　　者：FC 2011/01/24 
*  
================================================================*/
int __stdcall GetOtherEmptyPwd(TCHAR* szDomain, TCHAR* szUserName);
/*================================================================  
*  
* 函 数 名：AccountIsForbid
*  
* 参　　数：
*   
* 参数值 
*
* 功能描述: 获取帐户是否被禁止
*   
* 返 回 值：
*  
* 抛出异常：  
*  
* 作　　者：FC 2011/01/24 
*  
================================================================*/
int __stdcall AccountIsForbid(long lType,TCHAR* szName);

/*================================================================  
*  
* 函 数 名：SysGetUserModelInfo
*  
* 参　　数：
*   
* 参数值 
*
* 功能描述: 获取系统密码策略信息
*   
* 返 回 值：
*  
* 抛出异常：  
*  
* 作　　者：FC 2011/01/24 
*  
================================================================*/
int __stdcall SysGetUserModelInfo(int& iMaxPwd,int& iLockout_dur,int& iLockout_threshold);

/*================================================================  
*  
* 函 数 名：SysServicesIsIn
*  
* 参　　数：
*   
* 参数值 
*
* 功能描述: 获取服务是否禁用
*   
* 返 回 值：成功：0 失败：-1
*  
* 抛出异常：  
*  
* 作　　者：FC 2011/01/24 
*  
================================================================*/
int __stdcall SysServicesIsIn(TCHAR* szServiceName);
/*================================================================  
*  
* 函 数 名：SysCDROMIsForbit
*  
* 参　　数：
*   
* 参数值 
*
* 功能描述: 获取光驱是否被禁用信息
*   
* 返 回 值：成功：0 失败：-1
*  
* 抛出异常：  
*  
* 作　　者：FC 2011/01/24 
*  
================================================================*/
int __stdcall SysCDROMIsForbit();

/*================================================================  
*  
* 函 数 名：SysGetShareName
*  
* 参　　数：
*   
* 参数值 
*
* 功能描述: 获取共享目录名称
*   
* 返 回 值：成功：0 失败：-1
*  
* 抛出异常：  
*  
* 作　　者：FC 2011/01/24 
*  
================================================================*/
int __stdcall SysGetShareName(pHardWareInformation &pLink,int& iCount);

/*================================================================  
*  
* 函 数 名：GetUsbDeviceInfo
*  
* 参　　数：
*   
* 参数值 
*
* 功能描述: 获取计算机USB设备插拔记录
*   
* 返 回 值：成功：0 失败：-1
*  
* 抛出异常：  
*  
* 作　　者：FC 2011/10/19 
*  
================================================================*/
int __stdcall GetUsbDeviceInfo(pHardWareInformation &pLink,int& iCount,WCHAR* szReg,int iType);

/*================================================================  
*  
* 函 数 名：GetRegKeyName  
*  
* 参　　数：HKEY hKey:键的名称
*   
* 参数值
*
* 功能描述: 枚举USB存储设备名称
*  
* 返 回 值： 成功：0 失败：-1
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/10/19 
*  
================================================================*/ 
int  GetRegKeyName(HKEY hKey,pHardWareInformation &pLink,int& iCount,int iType);
//int  GetRegKeyName(HKEY hKey,TCHAR* szPidValue,TCHAR* szNameValue,TCHAR* szIDValue);

/*================================================================  
*  
* 函 数 名：GetCountRegKey  
*  
* 参　　数：HKEY hKey:键的名称
*   
* 参数值
*
* 功能描述: 获取USB存储设备信息个数
*  
* 返 回 值： 成功：0 失败：-1
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/10/19 
*  
================================================================*/ 
//int  GetCountRegKey(HKEY hKey,pHardWareInformation &pLink,int& iCount);

/*================================================================  
*  
* 函 数 名：GetMediaNormal
*  
* 参　　数：
*   
* 参数值 
*
* 功能描述: 判断进程是否存在
*   
* 返 回 值：成功：0 失败：-1
*  
* 抛出异常：  
*  
* 作　　者：FC 2011/10/19 
*  
================================================================*/
int __stdcall GetMediaNormal(TCHAR* szExeName);

/*================================================================  
*  
* 函 数 名：GetServicesNormal
*  
* 参　　数：
*   
* 参数值 
*
* 功能描述: 判断服务是否正常
*   
* 返 回 值：成功：0 失败：-1
*  
* 抛出异常：  
*  
* 作　　者：FC 2011/10/19 
*  
================================================================*/
int __stdcall GetServicesNormal(TCHAR* szServicesName);

/*================================================================  
*  
* 函 数 名：GetPatchInfo  
*  
* 参　　数：
*   
* 参数值
*
* 功能描述: 枚举补丁信息
*  
* 返 回 值： 成功：0 失败：-1
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/10/19 
*  
================================================================*/ 
int __stdcall GetPatchInfo(pHardWareInformation &pLink,int& iCount);

/*================================================================  
*  
* 函 数 名：GetLionIfo  
*  
* 参　　数：
*   
* 参数值
*
* 功能描述: 获取瑞星杀毒软件信息（狮子：lion）
*  
* 返 回 值： 成功：0 失败：-1
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/10/19 
*  
================================================================*/ 
int __stdcall GetLionIfo(pHardWareInformation &pLink,int& iCount);

/*================================================================  
*  
* 函 数 名：GetDoubleSystem
*  
* 参　　数：
*   
* 参数值
*
* 功能描述:判断是否安装了双系统
*  
* 返 回 值： 成功：返回安装的操作系统个数 失败：-1
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/10/19 
*  
================================================================*/ 
int __stdcall GetDoubleSystem();

/*================================================================  
*  
* 函 数 名：GetOtherAccount
*  
* 参　　数：
*   
* 参数值
*
* 功能描述:判断是否还有除Administrator guest以外的帐户
*  
* 返 回 值： 成功：返回安装的操作系统个数 失败：-1
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/10/19 
*  
================================================================*/ 
int __stdcall GetOtherAccount(pHardWareInformation &pLink,int& iCount);

/*================================================================  
*  
* 函 数 名：GetWireLessModule
*  
* 参　　数：
*   
* 参数值
*
* 功能描述:无线模块是否存在
*  
* 返 回 值： 返回无线模块的DeviceID
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/10/19 
*  
================================================================*/

int __stdcall GetWireLessModule(int iDeviceID,pHardWareInformation &pLink,int& iCount);

int GetAdaWirelessValue(HKEY hKey,int & iRetValue);

/*================================================================  
*  
* 函 数 名：GetExeFileName(TCHAR* szExeName)
*  
* 参　　数：
*   
* 参数值
*
* 功能描述:无线模块是否存在
*  
* 返 回 值： 返回无线模块的DeviceID
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/10/19 
*  
================================================================*/

int __stdcall GetExeFileName(TCHAR* szDir,TCHAR* szExeName);

/*================================================================  
*  
* 函 数 名：GetOtherSerNo(pHardWareInformation &pLink,int& iCount)
*  
* 参　　数：
*   
* 参数值
*
* 功能描述:另一种获取硬盘序列号
*  
* 返 回 值： 返回硬盘序列号
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/10/19 
*  
================================================================*/

int __stdcall GetOtherSerNo(pHardWareInformation &pLink,int& iCount);

/*================================================================  
*  
* 函 数 名：GetOtherShareFile(pHardWareInformation &pLink,int& iCount)
*  
* 参　　数：
*   
* 参数值
*
* 功能描述:另一种获取共享文件
*  
* 返 回 值： 共享文件
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/10/19 
*  
================================================================*/

int __stdcall GetOtherShareFile(pHardWareInformation &pLink,int& iCount);

/*================================================================  
*  
* 函 数 名：GetOtherInstallTime(pHardWareInformation &pLink,int& iCount)
*  
* 参　　数：
*   
* 参数值
*
* 功能描述:另一种获取系统安装时间
*  
* 返 回 值： 共享文件
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/10/19 
*  
================================================================*/

int __stdcall GetOtherInstallTime(pHardWareInformation &pLink,int& iCount);


/*================================================================  
*  
* 函 数 名：GetOtherSysType(pHardWareInformation &pLink,int& iCount)
*  
* 参　　数：
*   
* 参数值
*
* 功能描述:另一种获取操作系统类型
* 返 回 值： 共享文件
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/10/19 
*  
================================================================*/

int __stdcall GetOtherSysType(pHardWareInformation &pLink,int& iCount);

/*================================================================  
*  
* 函 数 名：GetSofeWareInfo(pHardWareInformation &pLink,int& iCount)
*  
* 参　　数：
*   
* 参数值
*
* 功能描述: 获取系统已安装软件
* 返 回 值：已安装软件（只包含在注册表中包含的信息）
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/10/19 
*  
================================================================*/

int __stdcall GetSofeWareInfo(pHardWareInformation &pLink,int& iCount,int iDerial/*开关*/);

/*================================================================  
*  
* 函 数 名：GetPatchInfoNext(pHardWareInformation &pLink,int& iCount)
*  
* 参　　数：
*   
* 参数值
*
* 功能描述: 获取系统补丁名称
* 返 回 值：
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/10/19 
*  
================================================================*/
int __stdcall GetPatchInfoNext(pHardWareInformation &pLink,int& iCount);

/*================================================================  
*  
* 函 数 名：GetKillVirusInfo(TCHAR* szTime,pHardWareInformation &pLink,int& iCount)
*  
* 参　　数：
*   
* 参数值
*
* 功能描述: 获取瑞星杀毒软件病毒查杀信息 包括查杀的文件名、路径、时间
* 返 回 值：
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/10/19 
*  
================================================================*/
int __stdcall GetKillVirusInfo(TCHAR* szTime,pHardWareInformation &pLink,int& iCount);
/*================================================================  
*  
* 函 数 名：GetOtherDiskSer(pHardWareInformation &pLink,int& iCount)
*  
* 参　　数：
*   
* 参数值
*
* 功能描述: 另一种方法获取硬盘序列号
* 返 回 值：
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/10/19 
*  
================================================================*/
int __stdcall GetOtherDiskSer(pHardWareInformation &pLink,int& iCount);
/*================================================================  
*  
* 函 数 名：GetOtherCDROMInfo(pHardWareInformation &pLink,int& iCount)
*  
* 参　　数：
*   
* 参数值
*
* 功能描述: 另一种获取CDROM名称
* 返 回 值：
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/10/19 
*  
================================================================*/
int __stdcall GetOtherCDROMInfo(pHardWareInformation &pLink,int& iCount);

/*================================================================  
*  
* 函 数 名：GetLogInfo(pHardWareInformation &pLink,int& iCount)
*  
* 参　　数：
*   
* 参数值
*
* 功能描述: 获取日志信息
* 返 回 值：
*  
* 抛出异常： 
*  
* 作　　者：FC 2011/10/19 
*  
================================================================*/
int __stdcall GetLogInfo(pHardWareInformation &pLink,int& iCount);
/*================================================================  
*  
* 函 数 名：GetPatchInfoLastTime(pHardWareInformation &pLink,int& iCount)
*  
* 参　　数：
*   
* 参数值
*
* 功能描述: 获取系统补丁最后安装时间
* 返 回 值：
*  
* 抛出异常： 
*  
* 作　　者：FC 2013/08/16 
*  
================================================================*/
int __stdcall GetPatchInfoLastTime(pHardWareInformation &pLink,int& iCount);

#endif
