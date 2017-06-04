//#include "stdafx.h"
#include "GetInformation.h"
#include "tlhelp32.h"
// #include "CApplication.h"
// #include "CRecentFile.h"
// #include "CRecentFiles.h"
#include "Lmaccess.h"
#include "shlwapi.h"

#include <winsock2.h>
#include <iphlpapi.h>	
#include "Wininet.h "
#include "resource.h"

#include "Iads.h"
#include "Adshlp.h"
#include "list"
#include "LM.h"

#include "io.h"

//extern HINSTANCE hDllInstance;
HINSTANCE hDllInstance;

#pragma comment(lib,"Netapi32.lib ")

#pragma comment(lib,"Activeds.lib")
#pragma comment(lib,"Adsiid.lib ")



#pragma comment(lib, "Wininet.lib ")
#pragma comment(lib, "IPHLPAPI.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

const WORD IDE_ATAPI_IDENTIFY = 0xA1;   // 读取ATAPI设备的命令
const WORD IDE_ATA_IDENTIFY   = 0xEC;   // 读取ATA设备的命令

//#define _WIN32_WINNT 0x0400

#include "winioctl.h"
CString m_strDiskID;

int iProcessLen = 8;
TCHAR* ArrProcess[] = {
	PROCESSOR_Manfacturer,
	PROCESSOR_MaxClockSpeed,
	PROCESSOR_ExtClock,
	PROCESSOR_CurrentClockSpeed,
	PROCESSOR_LzCacheSize,
	PROCESSOR_CurrentVoltage,
	PROCESSOR_ProcessorId,
	PROCESSOR_DataWidth,
};

int iGraphicsLen = 4;
TCHAR* ArrGraphics[] = {
	GRAPHICE_Caption,
	GRAPHICE_AdapterRAM,
	GRAPHICE_VideoProcessor,
	GRAPHICE_DriverVersion
};
int iDiskLen = 5;
TCHAR* ArrDisk[] = {
	DISK_Caption,
	DISK_Size,
	DISK_Mode,
	DISK_InterFacrType,
	DISK_TimeCount
};

int iMemoryLen = 3;
TCHAR* ArrMemory[] = {
	MEMORY_Capacity,
	MEMORY_DeviceLocator,
	MEMORY_TotalWidth
};

int iMainboardLen = 6;
TCHAR* ArrMainboard[] = {
	MAINBOARD_Manfacturer,
	MAINBOARD_Product,
	MAINBOARD_BIOS_ReleaseDate,
	MAINBOARD_BIOS_Manufacturer,
	MAINBOARD_BIOS_Name,
	MAINBOARD_SerialNumber_BIOS
};

int iDisplayLen = 1;
TCHAR* ArrDisplay[] = {
	DISPLAY_ScreenWidthXScreenHeight
};

int iSoundcardLen = 2;
TCHAR* ArrSoundcard[] = {
	SOUNDCARD_Caption,
	SOUNDCARD_Manfacturer
};

#pragma comment(lib,"Netapi32.lib")

/*================================================================  
*  
* 函 数 名：InsertLink()
*  
* 参　　数：
*   
* 参数值 
*
* 功能描述: 插入硬件信息链表
*   
* 返 回 值：链表指针或者NULL
*  
* 抛出异常：  
*  
* 作　　者：FC 2011/01/24 
*  
================================================================*/
pHardWareInformation  InsertLink(pHardWareInformation &pLink,CString &strName,CString &str)
{
	pHardWareInformation p = NULL;

	pHardWareInformation q = NULL;

	q = pLink; 

	while(q->next)
	{
		q = q->next;
	}

	int iSize = sizeof(HardwareInformation);

	p = (pHardWareInformation)malloc(iSize);

	p->next = NULL;

	memset(p,0,sizeof(HardwareInformation));

	if(sizeof(p->strName)>(strName.GetLength()*sizeof(TCHAR))&&sizeof(p->str)>(str.GetLength()*sizeof(TCHAR)))
	{
		wcscpy(p->strName,strName.GetBuffer(0));

		wcscpy(p->str,str.GetBuffer(0));
	}


	p->next = q->next;

	q->next = p;

	strName.ReleaseBuffer();

	str.ReleaseBuffer();

	return pLink;

}
char *__fastcall ConvertToString(DWORD dwDiskData[256], int nFirstIndex, int nLastIndex)
{
	static char szResBuf[1024];
	char ss[256];
	int nIndex = 0;
	int nPosition = 0;

	for(nIndex = nFirstIndex; nIndex <= nLastIndex; nIndex++)
	{
		ss[nPosition] = (char)(dwDiskData[nIndex] / 256);
		nPosition++;

		// Get low BYTE for 2nd character
		ss[nPosition] = (char)(dwDiskData[nIndex] % 256);
		nPosition++;
	}

	// End the string
	ss[nPosition] = '\0';

	int i, index=0;
	for(i=0; i<nPosition; i++)
	{
		if(ss[i]==0 || ss[i]==32)	continue;
		szResBuf[index]=ss[i];
		index++;
	}
	szResBuf[index]=0;

	return szResBuf;
}

BOOL __fastcall DoIdentify( HANDLE hPhysicalDriveIOCTL, 
						   PSENDCMDINPARAMS pSCIP,
						   PSENDCMDOUTPARAMS pSCOP, 
						   BYTE btIDCmd, 
						   BYTE btDriveNum,
						   PDWORD pdwBytesReturned)
{
	pSCIP->cBufferSize = IDENTIFY_BUFFER_SIZE;
	pSCIP->irDriveRegs.bFeaturesReg = 0;
	pSCIP->irDriveRegs.bSectorCountReg  = 1;
	pSCIP->irDriveRegs.bSectorNumberReg = 1;
	pSCIP->irDriveRegs.bCylLowReg  = 0;
	pSCIP->irDriveRegs.bCylHighReg = 0;

	pSCIP->irDriveRegs.bDriveHeadReg = (btDriveNum & 1) ? 0xB0 : 0xA0;
	pSCIP->irDriveRegs.bCommandReg = 0xEC;
	pSCIP->bDriveNumber = btDriveNum;
	pSCIP->cBufferSize = IDENTIFY_BUFFER_SIZE;

	return DeviceIoControl(	hPhysicalDriveIOCTL, 
		SMART_RCV_DRIVE_DATA,
		(LPVOID)pSCIP,
		sizeof(SENDCMDINPARAMS) - 1,
		(LPVOID)pSCOP,
		sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1,
		pdwBytesReturned, NULL);
}
int GetDiskSerAndCashe(pHardWareInformation pLink,int iType,int &iCount)
{
	int driver = 0;
	char szSerialNumber[64]={0};
	CString strErr;

	for(;driver<10;driver++)
	{
		CString sFilePath;
		sFilePath.Format(_T("\\\\.\\PHYSICALDRIVE%d"), driver);

		HANDLE hFile = INVALID_HANDLE_VALUE;
		hFile = ::CreateFile(sFilePath, 
			GENERIC_READ | GENERIC_WRITE, 
			FILE_SHARE_READ | FILE_SHARE_WRITE, 
			NULL, OPEN_EXISTING,
			0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
//			strErr.Format(_T("CreateFile Error %d"),GetLastError());
//			AfxMessageBox(strErr);
			return -1;
		}

		DWORD dwBytesReturned;
		GETVERSIONINPARAMS gvopVersionParams;
		DeviceIoControl(hFile, 
			SMART_GET_VERSION,
			NULL, 
			0, 
			&gvopVersionParams,
			sizeof(gvopVersionParams),
			&dwBytesReturned, NULL);

		if(gvopVersionParams.bIDEDeviceMap <= 0)
		{
//			strErr.Format(_T("DeviceIoControl-SMART_GET_VERSION Error %d"),GetLastError());
//			AfxMessageBox(strErr);
			return -1;
		}

		// IDE or ATAPI IDENTIFY cmd
		int btIDCmd = 0;
		SENDCMDINPARAMS InParams;
		int nDrive =0;
		btIDCmd = (gvopVersionParams.bIDEDeviceMap >> nDrive & 0x10) ? IDE_ATAPI_IDENTIFY : IDE_ATA_IDENTIFY;


		// 输出参数
		BYTE btIDOutCmd[sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1];

		if(DoIdentify(hFile,
			&InParams, 
			(PSENDCMDOUTPARAMS)btIDOutCmd,
			(BYTE)btIDCmd, 
			(BYTE)nDrive, &dwBytesReturned) == FALSE)
		{
//			strErr.Format(_T("DoIdentify Error %d"),GetLastError());
//			AfxMessageBox(strErr);
			return -1;
		}
		::CloseHandle(hFile);

		DWORD dwDiskData[256];
		USHORT *pIDSector; // 对应结构IDSECTOR，见头文件

		pIDSector = (USHORT*)((SENDCMDOUTPARAMS*)btIDOutCmd)->bBuffer;
		for(int i=0; i < 256; i++)	dwDiskData[i] = pIDSector[i];

		// 取系列号
		if(iType != 2)
		{
			ZeroMemory(szSerialNumber, sizeof(szSerialNumber));
			strcpy(szSerialNumber, ConvertToString(dwDiskData, 10, 19));

			USES_CONVERSION;
			TCHAR* szSerNumber = A2W(szSerialNumber);

			CString strDiskSer;
			CString strDiskSerName;

			strDiskSerName.Format(_T("%s"),_T("硬盘序列号"));
			strDiskSer.Format(_T("%s"),szSerNumber);

			InsertLink(pLink,strDiskSerName,strDiskSer);
			iCount++;


		}
		
		if(iType !=1)
		{
			TCHAR szCache[10] = {0};
			_itot(dwDiskData[21]/2,szCache,10);

			CString strDiskCacheName;
			CString strDiskCache;

			strDiskCache.Format(_T("%s"),szCache);
			strDiskCacheName.Format(_T("%s"),_T("硬盘缓存"));
			InsertLink(pLink,strDiskCacheName,strDiskCache);
			iCount++;
		}
	}

	return 0;
}

LRESULT _getWmiInfo(IWbemClassObject *pClassObject,LPTSTR lpTag,pHardWareInformation pLink,int &iCount)
{

	SAFEARRAY *pvNames = NULL;
	if(pClassObject->GetNames(NULL,WBEM_FLAG_ALWAYS|WBEM_MASK_CONDITION_ORIGIN,NULL,&pvNames)==S_OK)
	{
		long vbl,vbu;

		SafeArrayGetLBound(pvNames,1,&vbl);
		SafeArrayGetUBound(pvNames,1,&vbu);

		CString strPathIDAndtime;
		strPathIDAndtime.Empty();


		for(long i = vbl;i<=vbu;i++)
		{
			long index = i;
			WCHAR* wsName = NULL;
			CComVariant varValue;

			SafeArrayGetElement(pvNames,&index,&wsName);

			CComBSTR bstrName(wsName);

			if(pClassObject->Get(bstrName,0,&varValue,NULL,0)==S_OK)
			{
				CString str;

				if(varValue.vt==VT_BSTR)
				{
					str = varValue.bstrVal;
				}
				else if(varValue.vt ==(VT_ARRAY|VT_UI1))
				{
					long iLowBound = 0,iUpBound = 0;
					SafeArrayGetLBound(varValue.parray,1,&iLowBound);
					SafeArrayGetUBound(varValue.parray,1,&iUpBound);

					BYTE bSafeVar[512]={0};

					for(long j = iLowBound;j<= iUpBound;j++)
					{
						long temp = j;
						SafeArrayGetElement(varValue.parray,&temp,bSafeVar+j);
					}
					for (int i = 0; i < bSafeVar[0] - 1; i++) 
					{
						int start=i*12;
						switch(bSafeVar[start+2])
						{
						case 9:
							str.Format(TEXT("%d"),bSafeVar[start + 7]+256*bSafeVar[start + 8]);//str.Format(TEXT("%d==%d"),bSafeVar[start + 5],bSafeVar[start + 7]+256*bSafeVar[start + 8]);
							break;
						default:
							break;
						}
					}
				}
				else if(varValue.vt==VT_ARRAY)
				{
					long iLowBound = 0,iUpBound = 0;
					SafeArrayGetLBound(varValue.parray,1,&iLowBound);
					SafeArrayGetUBound(varValue.parray,1,&iUpBound);

					for(long j = iLowBound;j<= iUpBound;j++)
					{
						VARIANT *pvar = NULL;
						long temp = j;
						if(SafeArrayGetElement(varValue.parray,&temp,pvar)==S_OK &&pvar)
						{
							CComVariant varTemp;
							if(varTemp.ChangeType(VT_BSTR,pvar)==S_OK)
							{
								if(!str.IsEmpty())
									str += TEXT(",");
								str += varTemp.bstrVal;

							}
						}
					}
				}
				else
				{
					if(varValue.ChangeType(VT_BSTR) == S_OK)
						str = varValue.bstrVal;
				}
				CString strHardName;

				strHardName = CString(bstrName);

				if(strHardName.Compare(_T("ReleaseDate"))==0)
				{
					CString strMidValue;
					strMidValue =  str.Left(8);
					strMidValue.Insert(4,'-');
					strMidValue.Insert(7,'-');
					InsertLink(pLink,strHardName,strMidValue);//获取BIOS时间时候进行处理
				}
				else if(strHardName.Compare(_T("HotFixID"))==0)
				{
					iCount--;
					strPathIDAndtime += str;
					strPathIDAndtime += _T(";");

				}
				else if(strHardName.Compare(_T("InstalledOn"))==0)
				{
					strPathIDAndtime += str;

					InsertLink(pLink,strHardName,strPathIDAndtime);
				}
				else
				{
					InsertLink(pLink,strHardName,str);
				}
				
				iCount++;

			}
		}
	}

	if(pvNames)
	{
		SafeArrayDestroy(pvNames);
		return S_OK;
	}

	return E_FAIL;
}

HRESULT GetWmiInfo(LPTSTR lpszClass,LPTSTR lpTag,pHardWareInformation pLink,int &iCount)//,LPTSTR lpszTerm
{
	CoInitialize(NULL);
	//
	//获取访问WMI权限
	//

	HRESULT hSer = CoInitializeSecurity(NULL,
		-1,
		NULL,
		NULL,
		RPC_C_AUTHN_LEVEL_PKT,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		EOAC_NONE,
		0);

	if(hSer == S_OK||hSer==RPC_E_TOO_LATE)
	{
		//

		//通过IWbemLocator 和IWBemServices这两个COM接口访问WMI,获取系统信息

		//


		CComPtr<IWbemLocator>spWbemLocaor;

		if(spWbemLocaor.CoCreateInstance(CLSID_WbemAdministrativeLocator,0,CLSCTX_INPROC_SERVER|CLSCTX_LOCAL_SERVER)==S_OK)
		{
			
			CComPtr<IWbemServices>spWbemServices;
			
			TCHAR szServer[30]={0};
			if(wcscmp(lpTag,DISK_TimeCount)==0)
			{
				wcscpy(szServer,TEXT("root\\wmi"));
			}
			else
			{
				wcscpy(szServer,TEXT("root\\cimv2"));
			}
			if(spWbemLocaor->ConnectServer(szServer,NULL,NULL,NULL,0,NULL,NULL,&spWbemServices)==S_OK)
			{
				//
				//检索指定信息
				//
				  ;
				CComPtr<IEnumWbemClassObject>spEnumWbemClassObject;
				CComBSTR bstrQuery(TEXT("Select "));

				if(NULL != lpTag)
				{
					bstrQuery += T2OLE(lpTag);
					bstrQuery += T2OLE(TEXT(" "));
					bstrQuery += T2OLE(TEXT("from "));

				}
				else
				{
					
					bstrQuery += T2OLE(TEXT("* from "));
				}

				bstrQuery += T2OLE(lpszClass);

				CoSetProxyBlanket(spWbemServices,RPC_C_AUTHN_WINNT,RPC_C_AUTHZ_NONE,NULL,RPC_C_AUTHN_LEVEL_CALL,RPC_C_IMP_LEVEL_IMPERSONATE,NULL,EOAC_NONE);
	
				if(spWbemServices->ExecQuery(TEXT("WQL"),bstrQuery,WBEM_FLAG_RETURN_IMMEDIATELY,NULL,&spEnumWbemClassObject) == S_OK)
				{
	
					ULONG uCount = 1,uReturned;
					CComPtr<IWbemClassObject>spClassObject;


					if(spEnumWbemClassObject->Reset() == S_OK)
					{
						int iEnumIdx = 0;
						while(spEnumWbemClassObject->Next(WBEM_INFINITE,uCount,&spClassObject,&uReturned)==S_OK)
						{
							_getWmiInfo(spClassObject,lpTag,pLink,iCount);

							spClassObject.Release();
						}

	
					}
				}
			}
		}
	}
	else
	{
		int a = GetLastError();
	}

	CoUninitialize();

	return E_FAIL;
}

void __stdcall HdwGetInfo(LPTSTR ClassSign,LPTSTR lpTag,pHardWareInformation &pLink,int &iCount)
{
	if(wcscmp(ClassSign,HARDINFOR_PROCESSOR)==0)
	{
		
		if(NULL != lpTag)
		{
			if(wcscmp(lpTag,PROCESSOR_MyCPUType)==0)
			{
				//获取CPU类型
				WCHAR sztype[MAX_PATH] ={0};
				char cProStr[49]={0};
				_asm
				{

					mov eax, 0x80000002
						cpuid
						mov dword ptr cProStr    , eax
						mov dword ptr cProStr + 4  , ebx
						mov dword ptr cProStr + 8  , ecx
						mov dword ptr cProStr + 12  ,edx

						mov eax, 0x80000003
						cpuid
						mov dword ptr cProStr + 16  , eax
						mov dword ptr cProStr + 20  , ebx
						mov dword ptr cProStr + 24  , ecx
						mov dword ptr cProStr + 28  , edx

						mov eax, 0x80000004
						cpuid
						mov dword ptr cProStr + 32  , eax
						mov dword ptr cProStr + 36  , ebx
						mov dword ptr cProStr + 40  , ecx
						mov dword ptr cProStr + 44  , edx
				}

				int iLength = ::MultiByteToWideChar(CP_ACP,
					0,
					(char*)cProStr,
					-1,
					NULL,
					0);

				::MultiByteToWideChar(CP_ACP,
					0,
					(char*)cProStr,
					-1,
					sztype,
					iLength);

				CString strTypeName;
				CString strType;

				strTypeName.Format(_T("%s"),_T("CPU类型"));//IDS_STRINGSPUTYPE
				strType.Format(_T("%s"),sztype);

				InsertLink(pLink,strTypeName,strType);
				iCount++;
			}
			else
				GetWmiInfo(ClassSign,lpTag,pLink,iCount);
		}
		else
		{
			for(int i = 0;i< 8;i++)
				GetWmiInfo(ClassSign,ArrProcess[i],pLink,iCount);

			//获取CPU类型
			WCHAR sztype[MAX_PATH] ={0};
			char cProStr[49]={0};
			_asm
			{

				mov eax, 0x80000002
					cpuid
					mov dword ptr cProStr    , eax
					mov dword ptr cProStr + 4  , ebx
					mov dword ptr cProStr + 8  , ecx
					mov dword ptr cProStr + 12  ,edx

					mov eax, 0x80000003
					cpuid
					mov dword ptr cProStr + 16  , eax
					mov dword ptr cProStr + 20  , ebx
					mov dword ptr cProStr + 24  , ecx
					mov dword ptr cProStr + 28  , edx

					mov eax, 0x80000004
					cpuid
					mov dword ptr cProStr + 32  , eax
					mov dword ptr cProStr + 36  , ebx
					mov dword ptr cProStr + 40  , ecx
					mov dword ptr cProStr + 44  , edx
			}

			int iLength = ::MultiByteToWideChar(CP_ACP,
				0,
				(char*)cProStr,
				-1,
				NULL,
				0);

			::MultiByteToWideChar(CP_ACP,
				0,
				(char*)cProStr,
				-1,
				sztype,
				iLength);

			CString strTypeName;
			CString strType;

			strTypeName.Format(_T("%s"),_T("CPU类型"));//IDS_STRINGSPUTYPE
			strType.Format(_T("%s"),sztype);

			InsertLink(pLink,strTypeName,strType);
			iCount++;
		}
	}

	else if(wcscmp(ClassSign,HARDINFOR_DISK)==0)
	{
		if(NULL != lpTag)
		{
			if(wcscmp(lpTag,DISK_MySerNumber) == 0)
			{
				//GetDiskSerAndCashe(pLink,1,iCount);
				GetOtherDiskSer(pLink,iCount);
			}
			else if(wcscmp(lpTag,DISK_MyCache) == 0)
			{
				GetDiskSerAndCashe(pLink,2,iCount);
			}
			else
				GetWmiInfo(ClassSign,lpTag,pLink,iCount);
		}
		else
		{
			for(int i = 0;i< 4;i++)
				GetWmiInfo(ClassSign,ArrDisk[i],pLink,iCount);


			GetWmiInfo(TEXT("MSStorageDriver_FailurePredictData"),ArrDisk[3],pLink,iCount);

//=============================获取补丁号=============================================
/*			OSVERSIONINFO osvi;

			ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
			osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

			GetVersionEx(&osvi);

			CString strPackName;
			CString strPack;

			strPackName.Format(_T("%s"),_T("系统补丁号"));
			strPack.Format(_T("%s"),osvi.szCSDVersion);

			InsertLink(pLink,strPackName,strPack);
			iCount++;
			*/
//=============================获取缓存和硬盘序列号=============================================
			GetDiskSerAndCashe(pLink,0,iCount);
			

		}
	}
	else if(wcscmp(ClassSign,HARDINFOR_GRAPHICS)==0)
	{
		if(NULL != lpTag)
		{
			GetWmiInfo(ClassSign,lpTag,pLink,iCount);
		}
		else
		{
			for(int i = 0;i< 4;i++)
				GetWmiInfo(ClassSign,ArrGraphics[i],pLink,iCount);
		}
	}
	else if(wcscmp(ClassSign,HARDINFOR_NETWORK)==0)
	{
		GetWmiInfo(ClassSign,lpTag,pLink,iCount);
	}
	else if(wcscmp(ClassSign,HARDINFOR_MEMORY)==0)
	{
		if(NULL != lpTag)
		{
			GetWmiInfo(ClassSign,lpTag,pLink,iCount);
		}
		else
		{
			for(int i = 0;i< 3;i++)
				GetWmiInfo(ClassSign,ArrMemory[i],pLink,iCount);
		}
	}
	else if(wcscmp(ClassSign,HARDINFOR_MAINBOARD)==0)
	{
		if(NULL != lpTag)
		{
			if(wcscmp(lpTag,MAINBOARD_BIOS_ReleaseDate) ==0)
			{
				GetWmiInfo(_T("Win32_Bios"),_T("ReleaseDate"),pLink,iCount);
			}
			else if(wcscmp(lpTag,MAINBOARD_BIOS_Manufacturer) ==0)
			{
				GetWmiInfo(_T("Win32_Bios"),_T("Manufacturer"),pLink,iCount);
			}
			else if(wcscmp(lpTag,MAINBOARD_BIOS_Name) ==0)
			{
				GetWmiInfo(_T("Win32_Bios"),_T("Name"),pLink,iCount);
			}
			else if(wcscmp(lpTag,MAINBOARD_SerialNumber_BIOS) ==0)
			{
				GetWmiInfo(ClassSign,_T("SerialNumber"),pLink,iCount);
			}
			else
			{
				GetWmiInfo(ClassSign,lpTag,pLink,iCount);
			}
		}
		else
		{
			for(int i = 0;i< 6;i++)
			{
				switch(i)
				{
				case 2:
					GetWmiInfo(_T("Win32_Bios"),_T("ReleaseDate"),pLink,iCount);
					break;
				case 3:
					GetWmiInfo(_T("Win32_Bios"),_T("Manufacturer"),pLink,iCount);
					break;
				case 4:
					GetWmiInfo(_T("Win32_Bios"),_T("Name"),pLink,iCount);
					break;
				case 5:
					GetWmiInfo(ClassSign,_T("SerialNumber"),pLink,iCount);
					break;
				default:
					GetWmiInfo(ClassSign,ArrMainboard[i],pLink,iCount);

				}
			}
		}
	}
	else if(wcscmp(ClassSign,HARDINFOR_DISPLAY)==0)
	{
		if(NULL != lpTag)
		{
			GetWmiInfo(ClassSign,lpTag,pLink,iCount);
		}
		else
		{
			for(int i = 0;i< 1;i++)
				GetWmiInfo(ClassSign,ArrDisplay[i],pLink,iCount);
		}
	}
	else if(wcscmp(ClassSign,HARDINFOR_SOUNDCARD)==0)
	{
		if(NULL != lpTag)
		{
			GetWmiInfo(ClassSign,lpTag,pLink,iCount);
		}
		else
		{
			for(int i = 0;i< 2;i++)
				GetWmiInfo(ClassSign,ArrSoundcard[i],pLink,iCount);

		}
	}
	else if(wcscmp(ClassSign,HARDINFOR_ADAPTER)==0)
	{
		if(NULL != lpTag)
		{
			if(wcscmp(lpTag,ADAPTER_MyCompant)==0)
			{
				GetNicInformation(pLink,1,0,iCount);	
			}
			else if(wcscmp(lpTag,ADAPTER_MyProduct)==0)
			{
				GetNicInformation(pLink,2,0,iCount);
			}
			else if(wcscmp(lpTag,ADAPTER_MacAddress)==0)
			{
				GetNicInformation(pLink,3,0,iCount);
			}
			else if(wcscmp(lpTag,ADAPTER_IPAddress)==0) 
			{
				GetNicInformation(pLink,4,0,iCount);
			}
		}
		if(NULL == lpTag)
		{
			GetNicInformation(pLink,0,0,iCount);
		}
	}
}

int __stdcall SysGetProcessInfo()
{
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	DWORD dwPriorityClass;

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);

	if(hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return -1;
	}

	pe32.dwSize = sizeof(PROCESSENTRY32);

	if(!Process32First(hProcessSnap,&pe32))
	{
		CloseHandle(hProcessSnap);
		return -1;
	}
	do 
	{
		dwPriorityClass = 0;
		hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pe32.th32ProcessID);
		if(hProcess == NULL)
			return -1;
		else
		{
			dwPriorityClass = GetPriorityClass(hProcess);
			if(!dwPriorityClass)
				return -1;
			CloseHandle(hProcess);

			SysGetThreadInfo(pe32.th32ProcessID);
			SysGetModuleInfo(pe32.th32ProcessID);
		}
	} while (Process32Next(hProcessSnap, &pe32 ));

	CloseHandle(hProcessSnap);
	return 0;
}

int __stdcall SysGetThreadInfo(DWORD dwPID)
{
	HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
	THREADENTRY32 te32;

	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,0);
	if(hThreadSnap == INVALID_HANDLE_VALUE)
		return -1;

	te32.dwSize = sizeof(THREADENTRY32);

	if(!Thread32First(hThreadSnap,&te32))
	{
		CloseHandle(hThreadSnap);
		return -1;
	}

	do 
	{
		if(te32.th32OwnerProcessID == dwPID)
			AfxMessageBox(TEXT("输出你需要的信息!"));
	} while (Thread32Next(hThreadSnap,&te32));
	return 0;
}

int __stdcall SysGetModuleInfo(DWORD dwPID)
{
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32;

	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,dwPID);
	if(hModuleSnap == INVALID_HANDLE_VALUE)
	{
		return -1;
	}

	me32.dwSize = sizeof(MODULEENTRY32);

	if(!Module32First(hModuleSnap,&me32))
	{
		CloseHandle(hModuleSnap);
		return -1;
	}
	do 
	{
		AfxMessageBox(TEXT("输出模块相关信息!"));
	} while (Module32Next(hModuleSnap,&me32));

	CloseHandle(hModuleSnap);
	return 0;
}

//需要删除的项有来自于http://blog.china.com/u/090826/328729/200911/5446374.html
/************************************************************************************************************
*1）HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Enum\USBSTOR
*2）HKEY_LOCAL_MACHINE\SYSTEM\ControlSet002\Enum\USBSTOR
*3）HKEY_LOCAL_MACHINE\SYSTEM\ControlSet003\Enum\USBSTOR 		
*4）HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Enum\USBSTOR 
*
*
* 
*
*
************************************************************************************************************/

int __stdcall UsbClearTrace()
{
	HKEY hDelDirKey = NULL;

	long lReult  = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,_TEXT("SYSTEM\\ControlSet001\\Enum\\USBSTOR"),0,KEY_ALL_ACCESS,&hDelDirKey);

	if(lReult == ERROR_SUCCESS)
	{
		QueryKey(hDelDirKey);

		::RegCloseKey(hDelDirKey);

	}
	else
	{
		return -1;
	}


	return 0;
}

void  QueryKey(HKEY hKey) 
{ 
	TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
	DWORD    cbName;                   // size of name string 
	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
	DWORD    cchClassName = MAX_PATH;  // size of class string 
	DWORD    cSubKeys=0;               // number of subkeys 
	DWORD    cbMaxSubKey;              // longest subkey size 
	DWORD    cchMaxClass;              // longest class string 
	DWORD    cValues;              // number of values for key 
	DWORD    cchMaxValue;          // longest value name 
	DWORD    cbMaxValueData;       // longest value data 
	DWORD    cbSecurityDescriptor; // size of security descriptor 
	FILETIME ftLastWriteTime;      // last write time 

	DWORD i, retCode; 

	TCHAR achValue[MAX_VALUE_NAME] = {0}; 
	DWORD cchValue = MAX_VALUE_NAME; 

	// Get the class name and the value count. 
	retCode = RegQueryInfoKey(
		hKey,                    // key handle 
		achClass,                // buffer for class name 
		&cchClassName,           // size of class string 
		NULL,                    // reserved 
		&cSubKeys,               // number of subkeys 
		&cbMaxSubKey,            // longest subkey size 
		&cchMaxClass,            // longest class string 
		&cValues,                // number of values for this key 
		&cchMaxValue,            // longest value name 
		&cbMaxValueData,         // longest value data 
		&cbSecurityDescriptor,   // security descriptor 
		&ftLastWriteTime);       // last write time 

	// Enumerate the subkeys, until RegEnumKeyEx fails.

	if (cSubKeys)
	{
		//		printf( "\nNumber of subkeys: %d\n", cSubKeys);

		for (i=0; i<cSubKeys; i++) 
		{ 
			cbName = MAX_KEY_LENGTH;
			retCode = RegEnumKeyEx(hKey, 0,
				achKey, 
				&cbName, 
				NULL, 
				NULL, 
				NULL, 
				&ftLastWriteTime); 
			if (retCode == ERROR_SUCCESS) 
			{
				//_tprintf(TEXT("(%d) %s\n"), i+1, achKey);

				HKEY hDelDirKey = NULL;

				long lReult  = ::RegOpenKeyEx(hKey,achKey,0,KEY_ALL_ACCESS,&hDelDirKey);//

				if(lReult == ERROR_SUCCESS)
				{
					QueryKey(hDelDirKey);

					::RegCloseKey(hDelDirKey);

				}
			}
		}
	}

	::RegDeleteKey(hKey,TEXT(""));

}

BOOL  DeleteUrlCache(DEL_CACHE_TYPE type)
{
	BOOL bRet = FALSE;
	HANDLE hEntry;
	LPINTERNET_CACHE_ENTRY_INFO lpCacheEntry = NULL;  
	DWORD dwEntrySize;

	dwEntrySize = 0;
	hEntry = FindFirstUrlCacheEntry(NULL, NULL, &dwEntrySize);
	lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwEntrySize];
	hEntry = FindFirstUrlCacheEntry(NULL, lpCacheEntry, &dwEntrySize);
	if (!hEntry)
	{
		goto cleanup;
	}

	do
	{
		if (type == File &&
			!(lpCacheEntry->CacheEntryType & COOKIE_CACHE_ENTRY))
		{
			DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
		}
		else if (type == Cookie &&
			(lpCacheEntry->CacheEntryType & COOKIE_CACHE_ENTRY))
		{
			DeleteUrlCacheEntry(lpCacheEntry->lpszSourceUrlName);
		}

		dwEntrySize = 0;
		FindNextUrlCacheEntry(hEntry, NULL, &dwEntrySize);
		delete [] lpCacheEntry; 
		lpCacheEntry = (LPINTERNET_CACHE_ENTRY_INFO) new char[dwEntrySize];
	}
	while (FindNextUrlCacheEntry(hEntry, lpCacheEntry, &dwEntrySize));

	bRet = TRUE;
cleanup:
	if (lpCacheEntry)
	{
		delete [] lpCacheEntry; 
	}
	return bRet;
}

BOOL  EmptyDirectory(LPCTSTR szPath, BOOL bDeleteDesktopIni, BOOL bWipeIndexDat)
{
	WIN32_FIND_DATA wfd;
	HANDLE hFind;
	CString sFullPath;
	CString sFindFilter;
	DWORD dwAttributes = 0;

	sFindFilter = szPath;
	sFindFilter += _T("\\*.*");
	if ((hFind = FindFirstFile(sFindFilter, &wfd)) == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	do
	{
		if (_tcscmp(wfd.cFileName, _T(".")) == 0 || 
			_tcscmp(wfd.cFileName, _T("..")) == 0 ||
			(bDeleteDesktopIni == FALSE && _tcsicmp(wfd.cFileName, _T("desktop.ini")) == 0))
		{
			continue;
		}

		sFullPath = szPath;
		sFullPath += _T('\\');
		sFullPath += wfd.cFileName;

		//去掉只读属性
		dwAttributes = GetFileAttributes(sFullPath);
		if (dwAttributes & FILE_ATTRIBUTE_READONLY)
		{
			dwAttributes &= ~FILE_ATTRIBUTE_READONLY;
			SetFileAttributes(sFullPath, dwAttributes);
		}

		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			EmptyDirectory(sFullPath, bDeleteDesktopIni, bWipeIndexDat);
			RemoveDirectory(sFullPath);
		}
		else
		{
			if (bWipeIndexDat && _tcsicmp(wfd.cFileName, _T("index.dat")) == 0)
			{
				WipeFile(szPath, wfd.cFileName);
			}
			DeleteFile(sFullPath);
		}
	}
	while (FindNextFile(hFind, &wfd));
	FindClose(hFind);

	return TRUE;
}

BOOL WipeFile(LPCTSTR szDir, LPCTSTR szFile)
{
	CString sPath;
	HANDLE	hFile;
	DWORD	dwSize;
	DWORD	dwWrite;
	char	sZero[SWEEP_BUFFER_SIZE];
	memset(sZero, 0, SWEEP_BUFFER_SIZE);

	sPath = szDir;
	sPath += _T('\\');
	sPath += szFile;

	hFile = CreateFile(sPath, GENERIC_WRITE, 
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	dwSize = GetFileSize(hFile, NULL);

	//skip file header (actually, I don't know the file format of index.dat)
	dwSize -= 64;
	SetFilePointer(hFile, 64, NULL, FILE_BEGIN);

	while (dwSize > 0)
	{
		if (dwSize > SWEEP_BUFFER_SIZE)
		{
			WriteFile(hFile, sZero, SWEEP_BUFFER_SIZE, &dwWrite, NULL);
			dwSize -= SWEEP_BUFFER_SIZE;
		}
		else
		{
			WriteFile(hFile, sZero, dwSize, &dwWrite, NULL);
			break;
		}
	}

	CloseHandle(hFile);
	return TRUE;
}


BOOL __stdcall WebClearIETempFile()
{
	TCHAR szPath[MAX_PATH] = {0};

	DeleteUrlCache(File);

	if(SHGetSpecialFolderPath(NULL,szPath,CSIDL_INTERNET_CACHE,FALSE))
	{
		EmptyDirectory(szPath);
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}

HRESULT __stdcall WebClearAddrHistoryInDir()
{
	HRESULT hr;
	TCHAR szPath[MAX_PATH] = {0};
	CoInitialize(NULL);

	IUrlHistoryStg2* pUrlHistoryStg2 = NULL;

	hr = CoCreateInstance(CLSID_CUrlHistory,NULL,
		CLSCTX_INPROC_SERVER,IID_IUrlHistoryStg2,
		(void**)&pUrlHistoryStg2);

	if(SUCCEEDED(hr))
	{
		hr = pUrlHistoryStg2->ClearHistory();
		pUrlHistoryStg2->Release();
	}
	CoUninitialize();

	GetWindowsDirectory(szPath,MAX_PATH);
	_tcscat(szPath,_T("\\History"));

	EmptyDirectory(szPath,FALSE,TRUE);

	if(SHGetSpecialFolderPath(NULL,szPath,CSIDL_HISTORY,FALSE))
	{
		EmptyDirectory(szPath,FALSE,TRUE);
		return 0;
	}
	else
	{
		return -1;
	}

}


int __stdcall NetClearOnlineHistory()
{
	TCHAR szPath[MAX_PATH] = {0};
	if(SHGetSpecialFolderPath(NULL,szPath,CSIDL_NETHOOD,FALSE))
	{
		EmptyDirectory(szPath);
		return 0;
	}
	else
	{
		return -1;
	}
}

int __stdcall SysClearTempFiles()
{
	TCHAR szPath[MAX_PATH] = {0};

	if(GetTempPath(MAX_PATH,szPath))//得到系统临时文件夹
	{
		EmptyDirectory(szPath,TRUE);
		return 0;
	}
	else
	{
		return -1;
	}
}

int __stdcall SysClearFavorites()
{
	TCHAR szPath[MAX_PATH]={0};

	if(SHGetSpecialFolderPath(NULL,szPath,CSIDL_FAVORITES,FALSE))
	{
		EmptyDirectory(szPath);
		return 0;
	}
	else
	{
		return -1;
	}
}

int __stdcall WebClearAddrHistoryInIECombo()
{
	SHDeleteKey(HKEY_CURRENT_USER,TEXT("Software\\Microsoft\\Internet Explorer\\TypedURLs"));
	return 0;
}

int __stdcall WebClearCookie()
{
	TCHAR szPath[MAX_PATH] = {0};
	DeleteUrlCache(Cookie);

	if(SHGetSpecialFolderPath(NULL,szPath,CSIDL_COOKIES,FALSE))
	{
		EmptyDirectory(szPath);
		return 0;
	}
	else
	{
		return -1;
	}
}

int __stdcall DLL_Office_ClearWordHistory()
{
/*	CLSID clsid;
	HRESULT hr;

	IUnknown *pUnknown = NULL;
	IDispatch *pDispatch = NULL;
	CApplication app = NULL;

	::CoInitialize(NULL);

	hr = ::CLSIDFromProgID(TEXT("Word.Application"),&clsid);
	if(FAILED(hr))
	{
		::MessageBox(NULL,TEXT("出现错误,没有安装OFFICE"),TEXT(""),MB_OK);
		return -1;
	}


	hr = ::GetActiveObject(clsid,NULL,&pUnknown);
	if(FAILED(hr))
	{
		::MessageBox(NULL,TEXT("没有运行的OFFICE"),TEXT(""),MB_OK);
		return -1;
	}
	try
	{
		hr = pUnknown->QueryInterface(IID_IDispatch,(LPVOID*)&app);
		if(FAILED(hr))
			throw(TEXT("没有区得IDispatchPtr"));

		pUnknown->Release();
		pUnknown = NULL;

		CRecentFiles pAPP_recentFiles;

		pAPP_recentFiles = app.get_RecentFiles();


		long lCount;
		CRecentFile Files_rectFile;
		lCount = pAPP_recentFiles.get_Count();
		for(int i=1;lCount>0&&i<=lCount;i++)
		{
			Files_rectFile = pAPP_recentFiles.Item(i);

			Files_rectFile.Delete();
		}


	}
	catch(LPCTSTR lpErr)
	{
		::CoUninitialize();
		return -1;	
	}

	if(pUnknown)
		pUnknown->Release();
	if(app)
		app.ReleaseDispatch();

	::CoUninitialize();*/
	return 0;
}

int __stdcall SetSecurityStrategy()
{
	TCHAR szUserName[MAX_PATH] = {0};
	DWORD dwSize = MAX_PATH;

	PUSER_INFO_1 pUserinfo;


	GetUserName(szUserName,&dwSize);

	NetUserGetInfo(NULL,szUserName,1,(BYTE**)&pUserinfo);

	CString strInfo;

	strInfo.Format(TEXT("age = %d"),pUserinfo->usri1_password_age);


	return 0;
}
int __stdcall SysGetPwdAlterTime()
{
	TCHAR szUserName[MAX_PATH] = {0};
	DWORD dwSize = MAX_PATH;

	PUSER_INFO_1 pUserinfo;


	GetUserName(szUserName,&dwSize);

	NetUserGetInfo(NULL,szUserName,1,(BYTE**)&pUserinfo);

	CString strInfo;
	long iAgeLen = pUserinfo->usri1_password_age/(60*60*24);

	strInfo.Format(TEXT("age = %d"),iAgeLen);

	return iAgeLen;
}

int __stdcall SysGetWinPwdLen()
{
	PUSER_MODALS_INFO_0 pUSerModalsInfo;

	NetUserModalsGet(NULL,0,(BYTE**)&pUSerModalsInfo);

	CString strUserModlsInfo;

	strUserModlsInfo.Format(TEXT("%d"),pUSerModalsInfo->usrmod0_min_passwd_len);

	return pUSerModalsInfo->usrmod0_min_passwd_len;
}

int __stdcall SysGetWinLockTime()
{
	HKEY hLockKey = NULL;
	TCHAR szIsLock[20] ={0};

	DWORD dwType=REG_SZ;
	DWORD dwCount=0;
	
	long lResult;

	lResult  = ::RegOpenKeyEx(HKEY_CURRENT_USER,_TEXT("Software\\Policies\\Microsoft\\Windows\\Control Panel\\desktop"),0,KEY_ALL_ACCESS,&hLockKey);

	
	if(lResult != ERROR_SUCCESS)
	{
		lResult  = ::RegOpenKeyEx(HKEY_CURRENT_USER,_TEXT("Control Panel\\desktop"),0,KEY_ALL_ACCESS,&hLockKey);

		if(lResult != ERROR_SUCCESS)
		{
			return -1;
		}
	}


	lResult = RegQueryValueEx(hLockKey,_T("ScreenSaveActive"), NULL, &dwType,NULL, &dwCount);

	if(lResult != ERROR_SUCCESS)
	{
		return -1;
	}
	
	RegQueryValueEx(hLockKey,_T("ScreenSaveActive"), NULL, &dwType,(BYTE*)szIsLock, &dwCount);

	if(_tcscmp(szIsLock,_T("0"))==0)
	{
		return -2;
	}
	//=================判断是否设置了密码=========================================================
	lResult = RegQueryValueEx(hLockKey,_T("ScreenSaverIsSecure"), NULL, &dwType,NULL, &dwCount);

	if(lResult != ERROR_SUCCESS)
	{
		return -1;
	}
	else
	{
		TCHAR szIsSec[20] ={0};
		RegQueryValueEx(hLockKey,_T("ScreenSaverIsSecure"), NULL, &dwType,(BYTE*)szIsSec, &dwCount);

		if(_tcscmp(szIsSec,_T("0"))==0)
		{
			return -3;
		}
	}
	//========================================================================================
	
	::RegCloseKey(hLockKey);


	int iTime = 0;
	SystemParametersInfo(SPI_GETSCREENSAVETIMEOUT,0,&iTime,0);

	CString strTimeInfo;

	strTimeInfo.Format(TEXT("Time=%d"),iTime);

	return iTime;

}

pHardWareInformation __stdcall ResCreateLink(pHardWareInformation &pLink)
{
	pHardWareInformation p = (pHardWareInformation)malloc(sizeof(HardwareInformation));

	memset(p,0,sizeof(HardwareInformation));

	p->next = NULL;

	pLink = p;

	return pLink;
}


BOOL __stdcall ResDeleteLink(pHardWareInformation &pLink)
{
	pHardWareInformation p = NULL;

	p = pLink;

	while(p->next)
	{
		pLink = pLink->next;

		free(p);

		p = pLink;
	}

	free(p);

	return TRUE;
}

int GetNicInformation(pHardWareInformation &pLink,int iType,int ivariety,int &iCount)
{
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;
	UINT i;

	char szValue[MAX_PATH] ={0};

	ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
	pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(sizeof (IP_ADAPTER_INFO));
	if (pAdapterInfo == NULL) 
	{
		return -1;
	}

	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) 
	{
		FREE(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(ulOutBufLen);
		if (pAdapterInfo == NULL) 
		{
			return -1;
		}
	}

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) 
	{
		pAdapter = pAdapterInfo;
		while (pAdapter) 
		{

			int ivir_or_loc =(strstr(pAdapter->Description,"PCI") > 0||pAdapter->Type==71);

			if(ivariety == 1)
			{
				if(!ivir_or_loc)
				{
					pAdapter = pAdapter->Next;
					continue;
				}

			}
			else if(ivariety == 2)
			{
				if(ivir_or_loc)
				{
					pAdapter = pAdapter->Next;
					continue;
				}
			}

			CString strDes;
			
			USES_CONVERSION;

			LPWSTR szAdapter = NULL;

			szAdapter = A2W(pAdapter->Description);

			strDes.Format(TEXT("%s"),szAdapter);

			int iLeng = strDes.Find(' ');

			int iNextLen = strDes.ReverseFind('-');

			if(iNextLen ==-1)
			{
				iNextLen = strDes.GetLength();
			}

			CString strCompany;
			CString strNicName;

			strCompany.Format(_T("%s"),_T("厂商名称"));
			strNicName.Format(_T("%s"),_T("网卡名称"));
			
			if(iType ==0||iType==1)
			{
				InsertLink(pLink,strCompany,strDes.Left(iLeng +1));
				iCount++;
			}
			
			if(iType ==0||iType==2)
			{
				InsertLink(pLink,strNicName,strDes.Mid(iLeng,iNextLen-iLeng));
				iCount++;
			}
			
			
			CString strMacName;
			CString strMacAddress;
			CString strMidValue;

			strMacName.Format(_T("%s"),_T("MAC地址"));
			for (i = 0; i < pAdapter->AddressLength; i++) 
			{
				if (i == (pAdapter->AddressLength - 1))
				{
					strMidValue.Empty();
					strMidValue.Format(_T("%.2X"),(int) pAdapter->Address[i]);
					strMacAddress +=strMidValue;
				}
				else
				{
					strMidValue.Empty();
					strMidValue.Format(_T("%.2X-"),(int) pAdapter->Address[i]);
					strMacAddress +=strMidValue;
				}
			}

			if(iType ==0||iType==3)
			{
				InsertLink(pLink,strMacName,strMacAddress);
				iCount++;
			}

			CString strIPName;
			CString strIPAddress;

			strIPName.Format(_T("%s"),_T("IP地址"));

			LPWSTR lpAddress = NULL;

			PIP_ADDR_STRING pIPString = &(pAdapter->IpAddressList);

			do
			{
				lpAddress = A2W(pIPString->IpAddress.String);
				strIPAddress.Format(_T("%s"),lpAddress);

				if(iType ==0||iType==4)
				{
					InsertLink(pLink,strIPName,strIPAddress);
					iCount++;
				}
				
				pIPString = pIPString->Next;
				
			}while(pIPString);

			pAdapter = pAdapter->Next;

		} 
	}
	else 
	{
		return -1;
	}
	if (pAdapterInfo)
		FREE(pAdapterInfo);

	return 0;

}
int SysGetWinInstallTime(pHardWareInformation &pLink)
{
	int iCount = 0;
	//int iResult  =  GetWmiInfo(SOFT_SYSTEM,SYSTEM_Install,pLink,iCount);
	int iResult  = GetOtherInstallTime(pLink, iCount);

	return iResult;
}

int SysGetWinLaguage(pHardWareInformation &pLink)
{
	int iCount = 0;
	int iResult  =  GetWmiInfo(SOFT_SYSTEM,SYSTEM_Language,pLink,iCount);
	return iResult;
}

int SysGetWinProductName(pHardWareInformation &pLink)
{
	int iCount = 0;
	//int iResult  =  GetWmiInfo(SOFT_SYSTEM,SYSTEM_Type,pLink,iCount);
	int iResult  =  GetOtherSysType(pLink,iCount);
	return iResult;
}

int SysGetWinHostName(pHardWareInformation &pLink)
{
	int iCount = 0;
	int iResult  =  GetWmiInfo(SOFT_SYSTEM,SYSTEM_Name,pLink,iCount);
	return iResult;
}

int SysGetLocalNic(pHardWareInformation &pLink,int & itype,int ivariety,int& iCount)
{
	int iErr = 0;

	if(itype == 0)
	{
		iErr =  GetNicInformation(pLink,4,ivariety,iCount);
	}
	else if(itype == 1) 
	{
		iErr = GetNicInformation(pLink,3,ivariety,iCount);
	}
	else
	{
		return -2;
	}
	return iErr;
}
int AdValidation(TCHAR* szUserName,TCHAR* szPwd,TCHAR* szDomain)
{
	HANDLE  hdlTokenHandle  ;  
	TOKEN_PRIVILEGES   tkp;    

	//得到当前进程的令牌     
	OpenProcessToken(GetCurrentProcess,   TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,   &hdlTokenHandle);    
	//得到特权的Luid  关机特权 TEXT("SeShutdownPrivilege")   
	LookupPrivilegeValue(NULL,   SE_TCB_NAME, &tkp.Privileges[0].Luid);     

	//指定一次设置的特权属性的个数    
	tkp.PrivilegeCount=1;   

	//虽然用户可能有特权,但其状态缺省是disable的    

	tkp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED; ;     
	AdjustTokenPrivileges(hdlTokenHandle,   false,   &tkp, 0,   (PTOKEN_PRIVILEGES)NULL,   0);     

	HANDLE   user;  //  m_strDomName.GetBuffer(0) 

	if(LogonUser(szUserName,szDomain,szPwd,LOGON32_LOGON_INTERACTIVE,LOGON32_PROVIDER_DEFAULT,&user))     
	{     
		return 0;
	}     
	else     
	{     
		return -1;
	}   

}
int InsertDomaUser(pHardWareInformation &pLink,WCHAR* szLADP,WCHAR* szVar,int & icount)
{

	HRESULT hr;

	IADs *pADs = NULL;
	VARIANT  varAtribute;
	LPWSTR  strAtribute; 

	
	hr = ADsOpenObject(szLADP, NULL,NULL,ADS_SECURE_AUTHENTICATION, IID_IADs,(void**) &pADs);
	if (!SUCCEEDED(hr))   
	{ 
		return -1;  
	}


	VariantInit(&varAtribute);

	hr=pADs->Get(szVar,&varAtribute);

	switch (varAtribute.vt)  
	{
	case VT_BSTR:
		{  
			strAtribute = varAtribute.bstrVal;  
		}  
		break;  
	default:  
		strAtribute = L"";  
		break;  
	}
		
	//插入链表
	InsertLink(pLink,CString(szVar),CString(strAtribute));
	icount++;

	if (pADs)   
	{  
		pADs->Release();  
	} 
	return 0;
}
HRESULT PrintAllObjects(pHardWareInformation &pLink,IADsContainer* pContainer,int & icount)
{
	HRESULT hr;
	LPWSTR strName;

	if(NULL == pContainer) 
	{
		return E_INVALIDARG;
	}

	IEnumVARIANT *pEnum = NULL;

	hr = ADsBuildEnumerator(pContainer, &pEnum);
	if(SUCCEEDED(hr))
	{
		VARIANT var;
		ULONG ulFetched = 0L;

		while(S_OK == (hr = ADsEnumerateNext(pEnum, 1, &var, &ulFetched)) && (ulFetched > 0))
		{
			IADs *pADs;

			hr = V_DISPATCH(&var)->QueryInterface(IID_IADs, (void**)&pADs);
			if(SUCCEEDED(hr))
			{
				CComBSTR sbstr;
				IADsContainer *pChildContainer;

				hr = pADs->get_Name(&sbstr);
				if(SUCCEEDED(hr))
				{
					strName = sbstr; 

					LPWSTR lpsz = wcschr(strName , (_TUCHAR)'=');  
					int pos = (lpsz == NULL) ? -1 : (int)(lpsz - strName);  

					LPWSTR strDisplayName;
					if (pos > 0)   
					{  
						strDisplayName =  strName + pos + 1;  
					} 
					//插入数据strDisplayName
					InsertLink(pLink,CString(_T("用户名")),CString(strDisplayName));
					icount++;

				}

				hr = pADs->QueryInterface(IID_IADsContainer, (void**)&pChildContainer);
				if(SUCCEEDED(hr))
				{
					PrintAllObjects(pLink,pChildContainer,icount);
				}

				pADs->Release();
			}

			VariantClear(&var);
		}

		ADsFreeEnumerator(pEnum);
	}
	return hr;
}
int InsertDomaDepart(pHardWareInformation &pLink,WCHAR* szLADP,int & icount)
{
	HRESULT hr;  
	IADsContainer *pContainer = NULL;  

	hr = ADsGetObject( szLADP, IID_IADsContainer, (void**) &pContainer );  
	if (!SUCCEEDED(hr))   
	{ 
		return -1;
	}  
	PrintAllObjects(pLink,pContainer,icount);

	if (pContainer)  
	{  
		pContainer->Release();  
	}   

	return 0; 
}
int AdGetDomInfor(pHardWareInformation &pLink,TCHAR* szUserName,TCHAR* szDepart,int & icount)
{
	CoInitialize(NULL);
	CString strLADP;
 
	if(szUserName != NULL && szDepart != NULL)
	{
		return -1;
	}
	else if(szUserName != NULL)
	{
		TCHAR szDomaName[MAX_PATH/2] = {0};
		AdGetDomName(szDomaName);

		strLADP.Format(_T("LDAP://%s,OU=%s,DC=%s,DC=com"),szUserName,szDepart,szDomaName);

		InsertDomaUser(pLink,strLADP.GetBuffer(0),_T("displayName"),icount);
		InsertDomaUser(pLink,strLADP.GetBuffer(0),_T("extensionName"),icount);
		InsertDomaUser(pLink,strLADP.GetBuffer(0),_T("ipPhone"),icount);
		InsertDomaUser(pLink,strLADP.GetBuffer(0),_T("otherPager"),icount);
		InsertDomaUser(pLink,strLADP.GetBuffer(0),_T("comment"),icount);
		
	}
	else if(szDepart)
	{
		TCHAR szDomaName[MAX_PATH/2] = {0};
		AdGetDomName(szDomaName);

		strLADP.Format(_T("LDAP://OU=%s,DC=%s,DC=com"),szDepart,szDomaName);

		InsertDomaDepart(pLink,strLADP.GetBuffer(0),icount);
	}

	CoUninitialize();

	return 0;
}

int AdGetDomName(TCHAR* szDomName)
{
	HANDLE hToken;
	BOOL bSuccess;
	BOOL bRet = FALSE;

	UCHAR InfoBuffer[512] = {0};
	DWORD cbInfoBuffer = 512;

	TCHAR DomainName[MAX_PATH/2] ={0};
	TCHAR UserName[257] = {0};

	SID_NAME_USE snu;

	DWORD cchUser = 257;
	DWORD cchDomain = MAX_PATH/2;

	if(!OpenThreadToken(GetCurrentThread(),	TOKEN_QUERY, TRUE, &hToken)) 
	{

		if(GetLastError() == ERROR_NO_TOKEN) 
		{
			
			if(!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken ))
			{
				return -1;
			}

		} 
		else 
		{
			return -1;
		}
		bSuccess = GetTokenInformation(hToken, TokenUser, InfoBuffer, cbInfoBuffer, &cbInfoBuffer);
		if(!bSuccess) 
		{
			return -1;
		}
		else 
		{
			bRet = LookupAccountSid(NULL, ((PTOKEN_USER)InfoBuffer)->User.Sid, UserName, &cchUser,DomainName,&cchDomain, &snu );
			if (!bRet) 
			{
				
				CloseHandle(hToken);
				return -1;
			}
			else 
			{
				wcscpy(szDomName,DomainName);
			}
		}
	}
	return 0;
}

/*
int __stdcall GetAdToExport(string strtype ,int itype,list<string> &strList,int & icount)
{
	pHardWareInformation pInfo =NULL;
	pHardWareInformation pInfoFir;
	
	USES_CONVERSION;
	TCHAR* szUnicodeType = A2W(strtype.c_str());

	ResCreateLink(pInfo);

	pInfoFir = pInfo;
	switch(itype)
	{
	case 0://获取人员属性信息
		AdGetDomInfor(pInfo,szUnicodeType,NULL,icount);
		break;
	case 1://获取部门人员信息
		AdGetDomInfor(pInfo,NULL,szUnicodeType,icount);
		break;
	default:
		break;
	}

	pInfo = pInfo->next;
	while(pInfo != NULL)
	{		
		strList.push_back(string(W2A(pInfo->str)));
		pInfo = pInfo->next;
	}

	ResDeleteLink(pInfoFir);
	
	return 0;
}
*/
pHardWareInformation pInfoFir = NULL;
int AdCallBack(string strtype,int itype,int& icount)
{
	pHardWareInformation pInfo = NULL;
	pInfoFir = NULL;

	USES_CONVERSION;
	TCHAR* szUnicodeType = A2W(strtype.c_str());

	ResCreateLink(pInfo);

	pInfoFir = pInfo;
	switch(itype)
	{
	case 0://获取人员信息
		AdGetDomInfor(pInfo,szUnicodeType,NULL,icount);
		break;
	case 1:
		AdGetDomInfor(pInfo,NULL,szUnicodeType,icount);
		break;
	default:
		break;
	}
	
	return 0;
}
int __stdcall GetAdToExport(string strInfo,int itype/*,ADFun AdCallBackFun*/)
{
	pHardWareInformation pInfo =NULL;
	pInfo = pInfoFir;
	int iCount = 0;
	
	//::MessageBoxA(NULL,strInfo.c_str(),"写文件类容",MB_OK);
	AdCallBack(strInfo,itype,iCount);

	pInfo = pInfo->next;

	while(pInfo != NULL)
	{		
		pInfo = pInfo->next;
	}

	ResDeleteLink(pInfoFir);

	return 0;
}

int __stdcall AccountIsEmptyPassword(TCHAR* domainname, TCHAR* username)
{

	int itype = 0;
	DWORD dValue = 0;
	PUSER_INFO_1 pUserinfo;
	PUSER_INFO_3 pUserinfoNext;

	int iExpiredValue = 0;
	int aErr;

	NET_API_STATUS netResult;

	aErr = GetLastError();

	//判断用户不能更改密码是否被勾选
	netResult = NetUserGetInfo(NULL,username,1,(BYTE**)&pUserinfo);

	//NetUserGetInfo(NULL,username,3,(BYTE**)&pUserinfoNext);

	if(netResult != NERR_Success)
	{
		return -3;
	}

	if(((pUserinfo->usri1_flags)&UF_PASSWD_CANT_CHANGE) == UF_PASSWD_CANT_CHANGE)
	{
		itype = 1;

		dValue = pUserinfo->usri1_flags;
	
		pUserinfo->usri1_flags = ((pUserinfo->usri1_flags)^UF_PASSWD_CANT_CHANGE);

		NetUserSetInfo(NULL,username,1,(BYTE*)pUserinfo,NULL);
		

	}

	//判断下次登录修改密码是否勾选，当Guest密码为空的时候,该选项的勾选会被去掉
	if(((pUserinfo->usri1_flags)&UF_PASSWORD_EXPIRED) == UF_PASSWORD_EXPIRED)
	{
		if(itype != 1)
		{
			dValue = pUserinfo->usri1_flags;
		}

		iExpiredValue = 1;
	}
	
/*****************************************************************************/
	NET_API_STATUS dwStatus; 
	int iResult;

	dwStatus = NetUserChangePassword(domainname, username, L"", L"");  
	if ((dwStatus == NERR_Success) || (dwStatus == NERR_PasswordTooShort))  
	{   // 密码为空  
		iResult = -1;  
	}  
	else if(dwStatus == ERROR_INVALID_PASSWORD)  
	{   // 密码不为空  
		iResult = 0;  
	}  
	else  
	{   // 其它错误  
		iResult = -2;
 
	} 
/********************************************************************************/
	
	if(itype == 1 ||iExpiredValue == 1)
	{

		NetUserGetInfo(NULL,username,3,(BYTE**)&pUserinfoNext);

		pUserinfoNext->usri3_password_expired = 1;
	
		int ierr = NetUserSetInfo(NULL,username,3,(BYTE*)pUserinfoNext,NULL);

	}

	NetUserGetInfo(NULL,username,1,(BYTE**)&pUserinfo);


	return iResult;

}
int __stdcall AccountIsForbid(long lType,TCHAR* szName)
{
	TCHAR szUserName[MAX_PATH] = {0};
	DWORD dwSize = MAX_PATH;

	PUSER_INFO_1 pUserinfo;

	NET_API_STATUS netResult;
	netResult = NetUserGetInfo(NULL,szName,1,(BYTE**)&pUserinfo);

	if(netResult != NERR_Success)
	{
		return -3;
	}
	switch(lType)
	{
	case 0:
		if(((pUserinfo->usri1_flags)&UF_PASSWORD_EXPIRED) == UF_PASSWORD_EXPIRED)
		{
			return 0;
		}
		else
		{
			return -1;
		}
		break;
	case 1:
		if(((pUserinfo->usri1_flags)&UF_PASSWD_CANT_CHANGE) == UF_PASSWD_CANT_CHANGE)
		{
			return 0;
		}
		else
		{
			return -1;
		}
		break;
	case 2:
		if(((pUserinfo->usri1_flags)&UF_DONT_EXPIRE_PASSWD) == UF_DONT_EXPIRE_PASSWD)
		{
			return 0;
		}
		else
		{
			return -1;
		}
		break;
	case 3:
		if(((pUserinfo->usri1_flags)&UF_ACCOUNTDISABLE) == UF_ACCOUNTDISABLE)
		{
			return 0;
		}
		else
		{
			return -1;
		}
		break;
	case 4:
		if(((pUserinfo->usri1_flags)&UF_LOCKOUT) == UF_LOCKOUT)
		{
			return 0;
		}
		else
		{
			return -1;
		}
		break;
	default:
		return -1;
		break;
	}

}

int __stdcall SysGetUserModelInfo(int& iMaxPwd,int& iLockout_dur,int& iLockout_threshold)
{
	PUSER_MODALS_INFO_3 pUSerModalsInfo;

	NetUserModalsGet(NULL,3,(BYTE**)&pUSerModalsInfo);

	CString strUserModlsInfo;

	PUSER_MODALS_INFO_0 pUSerModalsPwd;

	NET_API_STATUS nStatus = NetUserModalsGet(NULL,0,(BYTE**)&pUSerModalsPwd);
	if(nStatus == NERR_Success)
	{

		if(pUSerModalsInfo->usrmod3_lockout_threshold == 0)
		{
			iLockout_dur = 0;
		}
		else
		{
			iLockout_dur = pUSerModalsInfo->usrmod3_lockout_duration/60;
			iLockout_threshold = pUSerModalsInfo->usrmod3_lockout_threshold;
		}
		
		if(pUSerModalsPwd->usrmod0_max_passwd_age == -1)
		{
			iMaxPwd = 0;
		}
		else
		{
			iMaxPwd = pUSerModalsPwd->usrmod0_max_passwd_age/24/60/60;
		}	

		return 0;
	}
	else
	{
		return -1;
	}
		
}

int __stdcall SysServicesIsIn(TCHAR* szServiceName)
{
	SC_HANDLE schSCManager;

	schSCManager = OpenSCManager( 
		NULL,                    
		NULL,                    
		STANDARD_RIGHTS_READ|SC_MANAGER_ENUMERATE_SERVICE|SC_MANAGER_QUERY_LOCK_STATUS);//SC_MANAGER_ALL_ACCESS

	if (NULL == schSCManager) 
	{
		return -3;
	}

	SC_HANDLE hService;
	hService = OpenService(schSCManager,szServiceName,STANDARD_RIGHTS_READ|
														SERVICE_QUERY_CONFIG|
														SERVICE_QUERY_STATUS|
														SERVICE_INTERROGATE|
														SERVICE_ENUMERATE_DEPENDENTS
														);//SERVICE_ALL_ACCESS
	if(hService == NULL)
	{
		CloseServiceHandle(schSCManager);
		return -2;
	}
	DWORD dwBytesNeeded, cbBufSize, dwError; 
	LPQUERY_SERVICE_CONFIG lpsc; 


	if( !QueryServiceConfig( 
		hService, 
		NULL, 
		0, 
		&dwBytesNeeded))
	{
		dwError = GetLastError();
		if( ERROR_INSUFFICIENT_BUFFER == dwError )
		{
			cbBufSize = dwBytesNeeded;
			lpsc = (LPQUERY_SERVICE_CONFIG) LocalAlloc(LMEM_FIXED, cbBufSize);
		}
		else
		{
			CloseServiceHandle(hService); 
			CloseServiceHandle(schSCManager);

			return -3;
		}
	}

	if( !QueryServiceConfig( 
		hService, 
		lpsc, 
		cbBufSize, 
		&dwBytesNeeded) ) 
	{
		LocalFree(lpsc); 

		CloseServiceHandle(hService); 
		CloseServiceHandle(schSCManager);
		return -3;
	}

	if(lpsc->dwStartType == SERVICE_DISABLED )
	{
		LocalFree(lpsc); 
		CloseServiceHandle(hService); 
		CloseServiceHandle(schSCManager);

		return 0;

	}
	else
	{
		LocalFree(lpsc); 
		CloseServiceHandle(hService); 
		CloseServiceHandle(schSCManager);

		return -1;
	}
}
int __stdcall SysCDROMIsForbit()
{
	TCHAR buf[MAX_PATH] = {0}; 
	DWORD len = GetLogicalDriveStrings(sizeof(buf)/sizeof(TCHAR),buf);
	CString strDisks;

	for(DWORD i = 0;i < len;i++)
	{
		strDisks = buf+i*4;

		if(DRIVE_CDROM == GetDriveType(strDisks))
		{
		
			return 0;//含有光驱
		}
	}

	return -1;//不含有光驱
}

int __stdcall SysGetShareName(pHardWareInformation &pLink,int& iCount)
{
	//int iResult  =  GetWmiInfo(_T("Win32_Share"),_T("Name"),pLink,iCount);
	int iResult = GetOtherShareFile(pLink,iCount);

	return iResult;
}

int __stdcall GetUsbDeviceInfo(pHardWareInformation &pLink,int& iCount,WCHAR* szReg,int iType)
{
	HKEY hDelDirKey = NULL;
	int iResult = 0;
	
	//打开相应的注册表项
	long lReult  = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,szReg,0,KEY_READ,&hDelDirKey);//_TEXT("SYSTEM\\CurrentControlSet\\Enum\\USBSTOR")\\Control\\DeviceClasses\\{53f56307-b6bf-11d0-94f2-00a0c91efb8b}
	
	if(lReult == ERROR_SUCCESS)
	{
		//获取查找的键值
		iResult = GetRegKeyName(hDelDirKey,pLink,iCount,iType);

		::RegCloseKey(hDelDirKey);

	}
	else
	{
		iResult = -1;
	}
	return iResult;
}

int  GetRegKeyName(HKEY hKey,pHardWareInformation &pLink,int& iCount,int iType)
{
	TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
	DWORD    cbName;                   // size of name string 
	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
	DWORD    cchClassName = MAX_PATH;  // size of class string 
	DWORD    cSubKeys=0;               // number of subkeys 
	DWORD    cbMaxSubKey;              // longest subkey size 
	DWORD    cchMaxClass;              // longest class string 
	DWORD    cValues;              // number of values for key 
	DWORD    cchMaxValue;          // longest value name 
	DWORD    cbMaxValueData;       // longest value data 
	DWORD    cbSecurityDescriptor; // size of security descriptor 
	FILETIME ftLastWriteTime;      // last write time 
	FILETIME ftLastWriteTimeKey;      // last write time 

	DWORD i, retCode; 

	TCHAR achValue[MAX_VALUE_NAME] = {0}; 
	DWORD cchValue = MAX_VALUE_NAME; 


	// Get the class name and the value count. 
	retCode = RegQueryInfoKey(
		hKey,                    
		achClass,                
		&cchClassName,           // size of class string 
		NULL,                    // reserved 
		&cSubKeys,               // number of subkeys 
		&cbMaxSubKey,            // longest subkey size 
		&cchMaxClass,            // longest class string 
		&cValues,                // number of values for this key 
		&cchMaxValue,            // longest value name 
		&cbMaxValueData,         // longest value data 
		&cbSecurityDescriptor,   // security descriptor 
		&ftLastWriteTime);       // last write time 

	// Enumerate the subkeys, until RegEnumKeyEx fails.
	if (cSubKeys)
	{
		for (i=0; i<cSubKeys; i++) 
		{ 
			cbName = MAX_KEY_LENGTH;
			retCode = RegEnumKeyEx(hKey, i,
				achKey, 
				&cbName, 
				NULL, 
				NULL, 
				NULL, 
				&ftLastWriteTimeKey); 
			if (retCode == ERROR_SUCCESS) 
			{

				HKEY hDelDirKey = NULL;

				long lReult  = ::RegOpenKeyEx(hKey,achKey,0,KEY_READ,&hDelDirKey);//
				if(lReult == ERROR_SUCCESS)
				{
					DWORD dwCount = 0;
					DWORD dwType = REG_SZ;
					DWORD dwNextType = REG_DWORD;
					WCHAR szValue[MAX_PATH] = {0};
					CString strValue;
					CString strName = _T("Value");

					DWORD dwValue;
					int iCaption = 0;

					LONG lResult = RegQueryValueEx(hDelDirKey,_T("Capabilities"),NULL,&dwType,NULL,&dwCount);
					if(lResult == ERROR_SUCCESS)
					{
						dwCount++;
						lResult = RegQueryValueEx(hDelDirKey,_T("Capabilities"),NULL,&dwType,(LPBYTE)&dwValue,&dwCount);
						if(lResult == ERROR_SUCCESS)
						{
							iCaption = dwValue;
						}
					}

					//判断是否是USBSTOR设备
					WCHAR szUSBType[MAX_PATH] = {0};

					//读取项相关属性并做判断
					lResult = RegQueryValueEx(hDelDirKey,_T("Service"),NULL,&dwType,NULL,&dwCount);
					if(lResult == ERROR_SUCCESS || iType == 0)
					{
						dwCount++;
						lResult = RegQueryValueEx(hDelDirKey,_T("Service"),NULL,&dwType,(LPBYTE)szUSBType,&dwCount);
						if(lResult == ERROR_SUCCESS || iType == 0)
						{
							//判断属性是否存在、是否是USBSTOR设备
							if(_tcscmp(_tcslwr(szUSBType),_T("disk")) == 0 || _tcscmp(_tcslwr(szUSBType),_T("usbstor")) == 0 ||(_tcslen(szUSBType) == 0 && iType == 0 && (dwValue == 20 || dwValue == 16)))
							{
								lResult = RegQueryValueEx(hDelDirKey,_T("FriendlyName"),NULL,&dwType,NULL,&dwCount);
								if(lResult == ERROR_SUCCESS)
								{

									SYSTEMTIME stLastWriteTime;
									SYSTEMTIME stLastWriteTimeTz;
									FileTimeToSystemTime(&ftLastWriteTimeKey,&stLastWriteTimeTz);

									SystemTimeToTzSpecificLocalTime(NULL,&stLastWriteTimeTz,&stLastWriteTime);

									dwCount++;
									lResult = RegQueryValueEx(hDelDirKey,_T("FriendlyName"),NULL,&dwType,(LPBYTE)szValue,&dwCount);
									if(lResult == ERROR_SUCCESS)
									{	
										strValue.Format(_T("%s;%s;%d-%d-%d %d:%d:%d"),achKey,szValue,
											stLastWriteTime.wYear,stLastWriteTime.wMonth,stLastWriteTime.wDay,
											stLastWriteTime.wHour,stLastWriteTime.wMinute,stLastWriteTime.wSecond);
										InsertLink(pLink,strName,strValue);

										iCount++;
									}

								}
								else
								{
									lResult = RegQueryValueEx(hDelDirKey,_T("LocationInformation"),NULL,&dwType,NULL,&dwCount);
									if(lResult == ERROR_SUCCESS)
									{

										SYSTEMTIME stLastWriteTime;
										SYSTEMTIME stLastWriteTimeTz;
										FileTimeToSystemTime(&ftLastWriteTimeKey,&stLastWriteTimeTz);

										SystemTimeToTzSpecificLocalTime(NULL,&stLastWriteTimeTz,&stLastWriteTime);

										dwCount++;
										lResult = RegQueryValueEx(hDelDirKey,_T("LocationInformation"),NULL,&dwType,(LPBYTE)szValue,&dwCount);
										if(lResult == ERROR_SUCCESS)
										{	
											//传出ID、名称、最后插拔时间
											strValue.Format(_T("%s;%s;%d-%d-%d %d:%d:%d"),achKey,szValue,
												stLastWriteTime.wYear,stLastWriteTime.wMonth,stLastWriteTime.wDay,
												stLastWriteTime.wHour,stLastWriteTime.wMinute,stLastWriteTime.wSecond);
											InsertLink(pLink,strName,strValue);

											iCount++;
										}

									}
									else
									{
										LONG lResult = RegQueryValueEx(hDelDirKey,_T("ClassGUID"),NULL,&dwType,NULL,&dwCount);
										if(lResult == ERROR_SUCCESS)
										{
											SYSTEMTIME stLastWriteTime;
											SYSTEMTIME stLastWriteTimeTz;
											FileTimeToSystemTime(&ftLastWriteTimeKey,&stLastWriteTimeTz);

											SystemTimeToTzSpecificLocalTime(NULL,&stLastWriteTimeTz,&stLastWriteTime);

											dwCount++;
											lResult = RegQueryValueEx(hDelDirKey,_T("ClassGUID"),NULL,&dwType,(LPBYTE)szValue,&dwCount);
											if(lResult == ERROR_SUCCESS)
											{	
												strValue.Format(_T("%s;%s;%d-%d-%d %d:%d:%d"),achKey,_T(""),
													stLastWriteTime.wYear,stLastWriteTime.wMonth,stLastWriteTime.wDay,
													stLastWriteTime.wHour,stLastWriteTime.wMinute,stLastWriteTime.wSecond);
												InsertLink(pLink,strName,strValue);

												iCount++;
											}
										}
									}
								}
							}

						}

					}

					GetRegKeyName(hDelDirKey,pLink,iCount,iType);

					::RegCloseKey(hDelDirKey);
				}
			}
		}
	}

	return 0;
}

int __stdcall GetMediaNormal(TCHAR* szExeName)
{
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);

	if(hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return -1;
	}

	pe32.dwSize = sizeof(PROCESSENTRY32);

	if(!Process32First(hProcessSnap,&pe32))
	{
		CloseHandle(hProcessSnap);
		return -1;
	}
	do 
	{

	  if(_tcscmp(_tcslwr(pe32.szExeFile),szExeName) == 0)
	  {
		  CloseHandle(hProcessSnap);
		  return 1;
	  }
	} while (Process32Next(hProcessSnap, &pe32 ));
	
	CloseHandle(hProcessSnap);

	return 0;
}

int __stdcall GetServicesNormal(TCHAR* szServicesName)
{
	SC_HANDLE schSCManager;
	SERVICE_STATUS_PROCESS ssStatus; 

	schSCManager = OpenSCManager( 
		NULL,                    
		NULL,                    
		STANDARD_RIGHTS_READ|SC_MANAGER_ENUMERATE_SERVICE|SC_MANAGER_QUERY_LOCK_STATUS);//SC_MANAGER_ALL_ACCESS

	if (NULL == schSCManager) 
	{
		return -3;
	}

	SC_HANDLE hService;
	hService = OpenService(schSCManager,szServicesName,STANDARD_RIGHTS_READ|
														SERVICE_QUERY_CONFIG|
														SERVICE_QUERY_STATUS|
														SERVICE_INTERROGATE|
														SERVICE_ENUMERATE_DEPENDENTS
														);//SERVICE_ALL_ACCESS
	if(hService == NULL)
	{
		CloseServiceHandle(schSCManager);
		return -1;
	}
	DWORD dwBytesNeeded, cbBufSize, dwError; 
	LPQUERY_SERVICE_CONFIG lpsc; 


	if( !QueryServiceConfig( 
		hService, 
		NULL, 
		0, 
		&dwBytesNeeded))
	{
		dwError = GetLastError();
		if( ERROR_INSUFFICIENT_BUFFER == dwError )
		{
			cbBufSize = dwBytesNeeded;
			lpsc = (LPQUERY_SERVICE_CONFIG) LocalAlloc(LMEM_FIXED, cbBufSize);
		}
		else
		{
			CloseServiceHandle(hService); 
			CloseServiceHandle(schSCManager);

			return -3;
		}
	}

	if( !QueryServiceConfig( 
		hService, 
		lpsc, 
		cbBufSize, 
		&dwBytesNeeded) ) 
	{
		LocalFree(lpsc); 

		CloseServiceHandle(hService); 
		CloseServiceHandle(schSCManager);
		return -3;
	}

	if(lpsc->dwStartType == SERVICE_DISABLED )
	{
		LocalFree(lpsc); 
		CloseServiceHandle(hService); 
		CloseServiceHandle(schSCManager);

		return -1;

	}
	else
	{
		if (!QueryServiceStatusEx( 
			hService,             // handle to service 
			SC_STATUS_PROCESS_INFO, // info level
			(LPBYTE) &ssStatus,             // address of structure
			sizeof(SERVICE_STATUS_PROCESS), // size of structure
			&dwBytesNeeded ) )              // if buffer too small
		{
			return -2; 
		}
		
		if(ssStatus.dwCurrentState == SERVICE_RUNNING)
		{
			LocalFree(lpsc); 
			CloseServiceHandle(hService); 
			CloseServiceHandle(schSCManager);

			return 0;
		}
	}

	LocalFree(lpsc); 
	CloseServiceHandle(hService); 
	CloseServiceHandle(schSCManager);

	return -1;
}

int __stdcall GetPatchInfo(pHardWareInformation &pLink,int& iCount)
{

	int iResult  =  GetWmiInfo(_T("Win32_QuickFixEngineering where HotFixID !='File 1'"),_T("HotFixID"),pLink,iCount);// where HotFixID!='File 1' //DesCription

	return iResult;

}

int __stdcall GetLionIfo(pHardWareInformation &pLink,int& iCount)
{
	TCHAR szVirPath[MAX_PATH] = {0};
	TCHAR szDirPath[MAX_PATH] = {0};


	::GetEnvironmentVariable(_T("Rav"),szVirPath,MAX_PATH);
	int iEnd = _tcslen(szVirPath);

	if(szVirPath[iEnd] != '\\')
	{
		_tcscat(szVirPath,_T("\\"));
	}

	_tcscpy(szDirPath,szVirPath);
	
	_tcscat(szVirPath,_T("def\\virusdb.cfg"));
	
	TCHAR szLibVer[50] = {0};
	TCHAR szLibTime[50] = {0};
	GetPrivateProfileString(_T("SUMMARY"),_T("ENDVER"),NULL,szLibVer,50,szVirPath);
	CString strLibVerName;
	CString strLibVer;
	strLibVerName.Format(_T("%s"),_T("病毒库版本"));
	strLibVer.Format(_T("%s"),szLibVer);
	InsertLink(pLink,strLibVerName,strLibVer);

	GetPrivateProfileString(_T("SUMMARY"),_T("UPDATETIME"),NULL,szLibTime,50,szVirPath);
	CString strLibTimeName;
	CString strLibTime;
	strLibTimeName.Format(_T("%s"),_T("病毒库升级时间"));
	strLibTime.Format(_T("%s"),szLibTime);
	InsertLink(pLink,strLibTimeName,strLibTime);
	
	_tcscat(szDirPath,_T("Update\\CompsVer.inf"));
	TCHAR szProVer[50] = {0};
	TCHAR szProTime[50] = {0};

	GetPrivateProfileString(_T("UPDATE"),_T("Version"),NULL,szProVer,50,szDirPath);
	CString strProVerName;
	CString strProVer;
	strProVerName.Format(_T("%s"),_T("软件版本"));
	strProVer.Format(_T("%s"),szProVer);
	InsertLink(pLink,strProVerName,strProVer);

	GetPrivateProfileString(_T("UPDATE"),_T("Time"),NULL,szProTime,50,szDirPath);
	CString strProTimeName;
	CString strProTime;
	strProTimeName.Format(_T("%s"),_T("软件升级时间"));
	strProTime.Format(_T("%s"),szProTime);
	InsertLink(pLink,strProTimeName,strProTime);

	TCHAR szDocumDir[MAX_PATH] = {0};
	::GetEnvironmentVariable(_T("RavData"),szDocumDir,MAX_PATH);


	int iEndNext = _tcslen(szDocumDir);

	if(szDocumDir[iEndNext] != '\\')
	{
		_tcscat(szDocumDir,_T("\\"));
	}

	_tcscat(szDocumDir,_T("Rav.ini"));

	TCHAR szScanAllTime[50] = {0};
	GetPrivateProfileString(_T("SCANNER"),_T("LastScanAll"),NULL,szScanAllTime,50,szDocumDir);
	CString strScanAllTimeName;
	CString strScanAllTime;
	strScanAllTimeName.Format(_T("%s"),_T("全盘扫描时间"));
	strScanAllTime.Format(_T("%s"),szScanAllTime);
	InsertLink(pLink,strScanAllTimeName,strScanAllTime);

	iCount = iCount + 5;


	return 0;
}

int __stdcall GetDoubleSystem()
{
	TCHAR szCountValue[MAX_PATH*2] = {0};

	HKEY hLockKey = NULL;
	TCHAR szBootPath[50] ={0};

	DWORD dwType=REG_SZ;
	DWORD dwCount=0;


	long lResult  = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,_TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Setup"),0,KEY_ALL_ACCESS,&hLockKey);

	if(lResult != ERROR_SUCCESS)
	{
		return -1;
	}

	lResult = RegQueryValueEx(hLockKey,_T("BootDir"), NULL, &dwType,NULL, &dwCount);

	if(lResult != ERROR_SUCCESS)
	{
		return -1;
	}

	RegQueryValueEx(hLockKey,_T("BootDir"), NULL, &dwType,(BYTE*)szBootPath, &dwCount);

	RegCloseKey(hLockKey);


	_tcscat(szBootPath,_T("boot.ini"));
	
	//当boot.ini没找到的时候枚举所有的硬盘
	if(_taccess(szBootPath,0) == -1)
	{
		TCHAR szLogicDisk[MAX_PATH] = {0};
		
		//获得硬盘上所有硬盘驱动器路径
		DWORD dwLen = GetLogicalDriveStrings(sizeof(szLogicDisk)/sizeof(TCHAR),szLogicDisk);

		TCHAR* pMidPath = NULL;

		pMidPath = szLogicDisk;

		for(int i = 0; i<dwLen ;i++)
		{
			if(szLogicDisk[i] != '\0')
			{
				
				if(szLogicDisk[i+1] == '\0')
				{
					TCHAR szMidPath[50] = {0};
					
					_tcscpy(szMidPath,pMidPath);

					pMidPath = szLogicDisk + i + 2;

					_tcscat(szMidPath,_T("boot.ini"));

					memset(szBootPath,0,50);

					_tcscpy(szBootPath,szMidPath);


					if(_taccess(szBootPath,0) == 0)
					{
						break;
					}

					if(szLogicDisk[i+2] == '\0')
					{
						break;
					}

				}
			}
		}
	}

	TCHAR szReString[MAX_PATH*2] = {0};
	TCHAR* pValue = szReString;
	
	GetPrivateProfileSection(_T("operating systems"),szReString,MAX_PATH*2,szBootPath);

	if(GetLastError() != 0)
	{
		return -1;
	}
	int iCount  = 0;
	int iValue = 0;

	for(int i = 0;i < MAX_PATH*2;i++)
	{
		if(szReString[i] == 0 && szReString[i+1] == 0)
		{
			break;
		}
		if(szReString[i] != 0 && szReString[i+1] == 0)
		{
			TCHAR szMidValue[MAX_PATH] = {0};

			_tcscpy(szMidValue,pValue+iValue);

			if(_tcsstr(szMidValue,_T("disk")) != NULL)
			{
				
				if(_tcsstr(szCountValue,szMidValue) == NULL)
				{
					_tcscat(szCountValue,szMidValue);
					iCount++;
					
				}
				
			}
			if(i+2 >= MAX_PATH)
			{
				break;
			}
			iValue = i+2;
		}	 
	}

	if(iCount == 0)
	{
		iCount = 1;
	}

	return iCount;
}

int __stdcall GetOtherAccount(pHardWareInformation &pLink,int& iCount)
{
	LPUSER_INFO_0 pBuf = NULL;
	LPUSER_INFO_0 pTmpBuf;
	DWORD dwLevel = 0;
	DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
	DWORD dwEntriesRead = 0;
	DWORD dwTotalEntries = 0;
	DWORD dwResumeHandle = 0;
	DWORD i;
	DWORD dwTotalCount = 0;
	NET_API_STATUS nStatus;


	do 
	{
		nStatus = NetUserEnum(NULL,
			dwLevel,
			FILTER_NORMAL_ACCOUNT, 
			(LPBYTE*)&pBuf,
			dwPrefMaxLen,
			&dwEntriesRead,
			&dwTotalEntries,
			&dwResumeHandle);

		if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA))
		{
			if ((pTmpBuf = pBuf) != NULL)
			{

				for (i = 0; (i < dwEntriesRead); i++)
				{

					if (pTmpBuf == NULL)
					{
						break;
					}

					CString strValue;
					CString strValueName;
					strValue.Format(_T("%s"), pTmpBuf->usri0_name);
					strValueName.Format(_T("%s"),_T("用户名称"));

					InsertLink(pLink,strValueName,strValue);
					iCount++;

					pTmpBuf++;
					dwTotalCount++;
				}
			}
		}

		if (pBuf != NULL)
		{
			NetApiBufferFree(pBuf);
			pBuf = NULL;
		}
	}while (nStatus == ERROR_MORE_DATA); 

	if (pBuf != NULL)
		NetApiBufferFree(pBuf);

	return 0;
}
int GetAdaWirelessValue(HKEY hKey,int & iRetValue)
{
	TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
	DWORD    cbName;                   // size of name string 
	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
	DWORD    cchClassName = MAX_PATH;  // size of class string 
	DWORD    cSubKeys=0;               // number of subkeys 
	DWORD    cbMaxSubKey;              // longest subkey size 
	DWORD    cchMaxClass;              // longest class string 
	DWORD    cValues;              // number of values for key 
	DWORD    cchMaxValue;          // longest value name 
	DWORD    cbMaxValueData;       // longest value data 
	DWORD    cbSecurityDescriptor; // size of security descriptor 
	FILETIME ftLastWriteTime;      // last write time 

	DWORD i, retCode; 

	int iReturn = 0;

	TCHAR achValue[MAX_VALUE_NAME] = {0}; 
	DWORD cchValue = MAX_VALUE_NAME; 

	// Get the class name and the value count. 
	retCode = RegQueryInfoKey(
		hKey,                    
		achClass,                
		&cchClassName,           // size of class string 
		NULL,                    // reserved 
		&cSubKeys,               // number of subkeys 
		&cbMaxSubKey,            // longest subkey size 
		&cchMaxClass,            // longest class string 
		&cValues,                // number of values for this key 
		&cchMaxValue,            // longest value name 
		&cbMaxValueData,         // longest value data 
		&cbSecurityDescriptor,   // security descriptor 
		&ftLastWriteTime);       // last write time 

	// Enumerate the subkeys, until RegEnumKeyEx fails.

	if (cSubKeys)
	{
		for (i=0; i<cSubKeys; i++) 
		{ 
			cbName = MAX_KEY_LENGTH;
			retCode = RegEnumKeyEx(hKey, i,
				achKey, 
				&cbName, 
				NULL, 
				NULL, 
				NULL, 
				&ftLastWriteTime); 
			if (retCode == ERROR_SUCCESS) 
			{

				HKEY hDelDirKey = NULL;

				long lReult  = ::RegOpenKeyEx(hKey,achKey,0,KEY_READ,&hDelDirKey);
				if(lReult == ERROR_SUCCESS)
				{
					DWORD dwCount = 0;
					DWORD dwType = REG_DWORD;
					int iMediaValue  = 0;
					
					LONG lResult = RegQueryValueEx(hDelDirKey,_T("MediaSubType"),NULL,&dwType,NULL,&dwCount);
					if(lResult == ERROR_SUCCESS)
					{
						SYSTEMTIME stLastWriteTime;
						FileTimeToSystemTime(&ftLastWriteTime,&stLastWriteTime);

						dwCount++;
						lResult = RegQueryValueEx(hDelDirKey,_T("MediaSubType"),NULL,&dwType,(LPBYTE)&iMediaValue,&dwCount);
						if(lResult == ERROR_SUCCESS)
						{
							
							if(iMediaValue == 2)
							{
								iRetValue = 1;
							}
						}
					}
					GetAdaWirelessValue(hDelDirKey,iRetValue);

					::RegCloseKey(hDelDirKey);

				}
			}
		}
	}
	
	return iReturn;
}
int __stdcall GetWireLessModule(int iDeviceID,pHardWareInformation &pLink,int& iCount)
{
	HKEY hDelDirKey = NULL;
	int iResult = 0;

	long lReult = 0;

	int iValue = 0;

	switch(iDeviceID)
	{
	case 0:
		 GetWmiInfo(_T("Win32_InfraredDevice"),_T("PNPDeviceID"),pLink,iCount);
		break;
	case 1:
		GetWmiInfo(_T("Win32_1394Controller"),_T("PNPDeviceID"),pLink,iCount);
		break;
	case 2:
		lReult  = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,_TEXT("SYSTEM\\CurrentControlSet\\Control\\Network\\{4D36E972-E325-11CE-BFC1-08002BE10318}"),0,KEY_READ,&hDelDirKey);//KEY_ALL_ACCESS

		if(lReult == ERROR_SUCCESS)
		{
			 GetAdaWirelessValue(hDelDirKey,iValue);

			 iResult = iValue;

			::RegCloseKey(hDelDirKey);

		}
		else
		{
			iResult = -1;
		}
		
		break;
	case 3:
		GetWmiInfo(_T("Win32_NetworkAdapter"),_T("Caption"),pLink,iCount);
		break;
	default:
		break;
	}

	return iResult;
}

int __stdcall GetExeFileName(TCHAR* szDir,TCHAR* szExeName)
{
	int iResult = 0;

	CFileFind class_Find;

	CString CS_Directory;

	CS_Directory.Empty();

	CS_Directory.Format(_T("%s"),szDir);

	CS_Directory += _T("\\*.*");

	BOOL bResult = class_Find.FindFile(CS_Directory);

	while(bResult)
	{
		bResult = class_Find.FindNextFile();

		if(!class_Find.IsDots())
		{
			if(class_Find.IsDirectory())
			{
				CString strFilePath = class_Find.GetFilePath();
				GetExeFileName(strFilePath.GetBuffer(0),szExeName);
			}
			else
			{
				CString CS_filename = class_Find.GetFileName();

				if(_tcscmp(_tcslwr(CS_filename.GetBuffer(0)),_tcslwr(szExeName)) == 0)
				{
					iResult =  1;
				}
			}
			
		}
	}
	class_Find.Close();

	return iResult;
}

int __stdcall GetOtherSerNo(pHardWareInformation &pLink,int& iCount)
{
	typedef char* (__stdcall *GetSerNo)(int,char*);

	TCHAR szFilePath[MAX_PATH] = {0};
	int iResult = 0;

	GetModuleFileName(hDllInstance,szFilePath,MAX_PATH);

	TCHAR szDir[10] = {0};
	TCHAR szDirName[MAX_PATH/2] = {0};

	_tsplitpath(szFilePath,szDir,szDirName,NULL,NULL);

	memset(szFilePath,0,MAX_PATH);

	_stprintf(szFilePath,_T("%s%s%s"),szDir,szDirName,_T("GetDiskSerial.dll"));	


	HINSTANCE  hGetSer;

	hGetSer = LoadLibrary(szFilePath);
	if(hGetSer == NULL)
	{
		iResult = -1;
	}
	else
	{
		GetSerNo GetSerFun = (GetSerNo)GetProcAddress(hGetSer,"GetSerialNumber");
		if(GetSerFun == NULL)
		{
			iResult = -2;
		}
		else
		{
			for(int iSerNum = 0;iSerNum<10;iSerNum++)
			{

				CString sFilePath;
				sFilePath.Format(_T("\\\\.\\PHYSICALDRIVE%d"), iSerNum);

				HANDLE hFile = INVALID_HANDLE_VALUE;
				hFile = ::CreateFile(sFilePath, 
					GENERIC_READ | GENERIC_WRITE, 
					FILE_SHARE_READ | FILE_SHARE_WRITE, 
					NULL, OPEN_EXISTING,
					0, NULL);
				if (hFile == INVALID_HANDLE_VALUE)
				{
					continue;
				}
				CloseHandle(hFile);
				
//				::MessageBox(NULL,szFilePath,_T(""),MB_OK);
				USES_CONVERSION;
				char szNo[MAX_PATH] = {0};
				char* szValue = GetSerFun(iSerNum,szNo);
//				::MessageBoxA(NULL,szValue,"",MB_OK);
				if(_tcslen(A2W(szValue)) > 0)
				{
					CString strName;
					CString strValue;

					strName.Format(_T("%s"),_T("硬盘序列号"));
					strValue.Format(_T("%s"),A2W(szValue));
					
//					::MessageBox(NULL,strName,_T(""),MB_OK);
					InsertLink(pLink,strName,strValue);
//					::MessageBox(NULL,strValue,_T(""),MB_OK);
					iCount++;

					szValue = NULL;
				}
			}
		}
		FreeLibrary(hGetSer);
	}

	return iResult;
}

int __stdcall GetOtherEmptyPwd(TCHAR* szDomain, TCHAR* szUserName)
{
	HANDLE  hdlTokenHandle  ;  
	TOKEN_PRIVILEGES   tkp;    

	//得到当前进程的令牌     
	OpenProcessToken(GetCurrentProcess,   TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,   &hdlTokenHandle);    
	//得到特权的Luid  关机特权 TEXT("SeShutdownPrivilege")   
	LookupPrivilegeValue(NULL,   SE_TCB_NAME, &tkp.Privileges[0].Luid);     

	//指定一次设置的特权属性的个数    
	tkp.PrivilegeCount=1;   

	//虽然用户可能有特权,但其状态缺省是disable的    

	tkp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED; ;     
	AdjustTokenPrivileges(hdlTokenHandle,false,&tkp,0,(PTOKEN_PRIVILEGES)NULL,0);     

	HANDLE   user;  

	int itype = 0;
	DWORD dValue = 0;
	PUSER_INFO_1 pUserinfo;

	int iExpiredValue = 0;
	int iLockCount = 0;

	NET_API_STATUS netResult;

	//判断用户不能更改密码是否被勾选
	netResult = NetUserGetInfo(NULL,szUserName,1,(BYTE**)&pUserinfo);

	if(netResult != NERR_Success)
	{
		return -3;
	}

	dValue = pUserinfo->usri1_flags;

	if(((pUserinfo->usri1_flags)&UF_PASSWD_CANT_CHANGE) == UF_PASSWD_CANT_CHANGE)
	{
		pUserinfo->usri1_flags = ((pUserinfo->usri1_flags)^UF_PASSWD_CANT_CHANGE);
		NetUserSetInfo(NULL,szUserName,1,(BYTE*)pUserinfo,NULL);
	}

	DWORD dwResult;

	if(((pUserinfo->usri1_flags)&UF_ACCOUNTDISABLE) == UF_ACCOUNTDISABLE)
	{
		pUserinfo->usri1_flags = ((pUserinfo->usri1_flags)^UF_ACCOUNTDISABLE);
		dwResult =  NetUserSetInfo(NULL,szUserName,1,(BYTE*)pUserinfo,NULL);
	}

	if(((pUserinfo->usri1_flags)&UF_LOCKOUT) == UF_LOCKOUT)
	{
		pUserinfo->usri1_flags = ((pUserinfo->usri1_flags)^UF_LOCKOUT);
		NetUserSetInfo(NULL,szUserName,1,(BYTE*)pUserinfo,NULL);
	}

	int iResult;

	CString strErr;
	int iErr;

	LogonUser(szUserName,szDomain,_T(""),LOGON32_LOGON_INTERACTIVE,LOGON32_PROVIDER_DEFAULT,&user);
	
	iErr = GetLastError();  
	if(iErr == ERROR_ACCOUNT_RESTRICTION)
	{     
		iResult =  -1;
	}
	else if(iErr == 1326||dwResult == 2245)
	{		
		iResult = 0;
	}
	else     
	{  
		iResult =  -2;
	}
	
	pUserinfo->usri1_flags = dValue;
	NetUserSetInfo(NULL,szUserName,1,(BYTE*)pUserinfo,NULL);

	return iResult;	
}

int __stdcall GetOtherShareFile(pHardWareInformation &pLink,int& iCount)
{
	PSHARE_INFO_502 pSi502,pSi502Out;
	NET_API_STATUS nas;
	DWORD er = 0, tr = 0, resume = 0, j= 0;	
	int tmpBreak = 0, i =0, itPos = 0;
	CString errorString;

	do 
	{
		nas = ::NetShareEnum(NULL,502,(LPBYTE *)&pSi502Out,MAX_PREFERRED_LENGTH,&er,&tr,&resume);

		if(nas == ERROR_SUCCESS || nas == ERROR_MORE_DATA)
		{
			pSi502 = pSi502Out;
			for(j=0; j<er; j++)
			{
				CString strValue;
				CString strValueName;
				strValue.Format(_T("%s"), pSi502->shi502_netname);
				strValueName.Format(_T("%s"), pSi502->shi502_path);

															
				InsertLink(pLink,strValueName,strValue);
				iCount++;

				pSi502++;
	
			}
			NetApiBufferFree(pSi502Out);
		}
	} while (nas == ERROR_MORE_DATA);
	
	return 0;
}

int __stdcall GetOtherInstallTime(pHardWareInformation &pLink,int& iCount)
{
	HKEY hInstallKey = NULL;
	int iInstaTime = 0;

	DWORD dwType=REG_DWORD;
	DWORD dwCount=0;


	long lResult  = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,_TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"),0,KEY_READ|KEY_WOW64_64KEY,&hInstallKey);

	if(lResult != ERROR_SUCCESS)
	{
		return -1;
	}

	lResult = RegQueryValueEx(hInstallKey,_T("InstallDate"), NULL, &dwType,NULL, &dwCount);
	if(lResult != ERROR_SUCCESS)
	{
		return -1;
	}

	RegQueryValueEx(hInstallKey,_T("InstallDate"), NULL, &dwType,(BYTE*)&iInstaTime, &dwCount);


	CTime t(iInstaTime);

	CString strValueName;
	strValueName.Format(_T("%s"),_T("安装时间"));

	InsertLink(pLink,strValueName,t.Format("%Y%m%d"));
	iCount++;

	RegCloseKey(hInstallKey);

	return 0;
}

int __stdcall GetOtherSysType(pHardWareInformation &pLink,int& iCount)
{
	TCHAR szSysType[MAX_PATH] = {0};
	HKEY hInstallKey = NULL;

	DWORD dwType=REG_SZ;
	DWORD dwCount=0;


	long lResult  = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,_TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"),0,KEY_ALL_ACCESS,&hInstallKey);

	if(lResult != ERROR_SUCCESS)
	{
		return -1;
	}

	lResult = RegQueryValueEx(hInstallKey,_T("ProductName"), NULL, &dwType,NULL, &dwCount);
	if(lResult != ERROR_SUCCESS)
	{
		return -1;
	}

	RegQueryValueEx(hInstallKey,_T("ProductName"), NULL, &dwType,(BYTE*)szSysType, &dwCount);

	CString strValueName;
	strValueName.Format(_T("%s"),_T("系统版本"));

	CString strSysType;
	strSysType.Format(_T("%s"),szSysType);

	InsertLink(pLink,strValueName,strSysType);

	RegCloseKey(hInstallKey);
	return 0; 

}

int __stdcall GetSofeWareInfo(pHardWareInformation &pLink,int& iCount,int iDerial/*开关*/)
{
	const int MAX_LEG = 256*sizeof(TCHAR);

	HKEY hKey;
	DWORD dwRegNum = MAX_LEG;

	CString strValue;

	TCHAR regBufferName[MAX_LEG] = {0};

	long lResult  = RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"),NULL,KEY_READ,&hKey);

	if(lResult != ERROR_SUCCESS)
	{
		return -1;
	}

	lResult = RegQueryInfoKey(hKey,NULL,NULL,NULL,&dwRegNum,NULL,NULL,NULL,NULL,NULL,NULL,NULL);

	if(lResult != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return -1;
	}

	for(int i = 0;i <(int)dwRegNum;i++)
	{
		DWORD dwRegSize = MAX_LEG;

		RegEnumKeyEx(hKey,i,regBufferName,&dwRegSize,NULL,NULL,NULL,NULL);

		DWORD dwType;
		HKEY hSubKey;

		lResult = RegOpenKeyEx(hKey,regBufferName,NULL,KEY_READ,&hSubKey);
		if(lResult != ERROR_SUCCESS)
		{
			return -1;
		}
		

		dwRegSize = MAX_LEG;
		//判断是否是软件补丁，是则跳出循
		TCHAR szRegParent[MAX_LEG] = {0};
		lResult = RegQueryValueEx(hSubKey,_T("ParentKeyName"),0,&dwType,(LPBYTE)szRegParent,&dwRegSize);
		if(lResult == ERROR_SUCCESS)
		{
			continue;
		}

		dwRegSize = MAX_LEG;
		TCHAR szRegName[MAX_LEG] = {0};

		lResult = RegQueryValueEx(hSubKey,_T("DisplayName"),0,&dwType,(LPBYTE)szRegName,&dwRegSize);
		if(lResult != ERROR_SUCCESS)
		{
			if(lResult != ERROR_FILE_NOT_FOUND)
			{
				continue;
			}
			
		}


		dwRegSize = MAX_LEG;
		TCHAR szRegVersion[MAX_LEG] = {0};
		lResult = RegQueryValueEx(hSubKey,_T("DisplayVersion"),0,&dwType,(LPBYTE)szRegVersion,&dwRegSize);
		if(lResult != ERROR_SUCCESS)
		{
			if(lResult != ERROR_FILE_NOT_FOUND)
			{
				continue;
			}
		}
		
		dwRegSize = MAX_LEG;
		TCHAR szRegIniTime[MAX_LEG] = {0};
		lResult = RegQueryValueEx(hSubKey,_T("InstallDate"),0,&dwType,(LPBYTE)szRegIniTime,&dwRegSize);
		if(lResult != ERROR_SUCCESS)
		{
			//如果获取不到安装时间信息，则取子项安装时间
			FILETIME fileLastTime;
			RegQueryInfoKey(hSubKey,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,&fileLastTime);

			SYSTEMTIME sTime,stLocal;
			FileTimeToSystemTime(&fileLastTime,&sTime);

			SystemTimeToTzSpecificLocalTime(NULL,&sTime,&stLocal);

			_stprintf(szRegIniTime,_T("%d-%02d-%02d"),stLocal.wYear,stLocal.wMonth,stLocal.wDay);
		}
		else
		{
			
			CString strTimeInfo,strTime;
			strTimeInfo.Format(_T("%s"),szRegIniTime);

			int iPrePos = 0;
			int iPos = strTimeInfo.Find(_T('-'),iPrePos);
			int i = 0;

			if(iPos == 2 ||iPos == 1)
			{
				CString strMon,strDay,strYear;
				while(iPos != -1)
				{
					switch(i)
					{
					case 0:
						strMon = strTimeInfo.Mid(iPrePos,iPos - iPrePos);
						break;
					case 1:
						strDay = strTimeInfo.Mid(iPrePos,iPos - iPrePos);
						strYear = strTimeInfo.Mid(iPos+1,4);
						break;
					default:
						break;
					}
					iPrePos = iPos +1;
					iPos = strTimeInfo.Find(_T('-'),iPrePos);
					i++;
				}
				strTime += strYear + _T("-") + strMon + _T("-") +strDay;
			}
			else if(iPos == -1)
			{
				int iRegTime = _ttoi(szRegIniTime);

				int iYear = iRegTime/10000;
				int iMonth = (iRegTime/100)%100;
				int iDay = iRegTime%100;

				strTime.Format(_T("%d-%d-%d"),iYear,iMonth,iDay);
			}

			_tcscpy(szRegIniTime,strTime.GetBuffer(0));
			strTime.ReleaseBuffer();
		}
		
		//如果开关值为 0 则查取所有注册表中相关的信息，其它则查取 控制面版//添加删除 中有关的信息
		if(iDerial == 0)
		{
			int iSysComponent;
			DWORD dwCount=0;
			lResult = RegQueryValueEx(hSubKey,_T("SystemComponent"),0,&dwType,(LPBYTE)&iSysComponent,&dwCount);

			if(iSysComponent == 1)
			{
				RegCloseKey(hSubKey);
				continue;
			}
		}

		CString strValueName;
		strValueName.Format(_T("%s"),_T("软件白名单"));

		if(_tcslen(szRegName) > 0 && _tcslen(szRegVersion) > 0)
		{
			strValue.Format(_T("%s%s%s%s%s"),szRegName,_T(";"),szRegVersion,_T(";"),szRegIniTime);

			InsertLink(pLink,strValueName,strValue);
			iCount++;

		}

		RegCloseKey(hSubKey);
	}
	
	RegCloseKey(hKey);

	return 0;
}

int __stdcall GetPatchInfoNext(pHardWareInformation &pLink,int& iCount)
{

	int iResult  =  GetWmiInfo(_T("Win32_QuickFixEngineering"),_T("ServicePackInEffect"),pLink,iCount);// where HotFixID!='File 1'

	return iResult;

}

int __stdcall GetKillVirusInfo(TCHAR* szTime,pHardWareInformation &pLink,int& iCount)
{
	//
	//获取数据库文件路径
	TCHAR szVirPath[MAX_PATH] = {0};
	TCHAR szDirPath[MAX_PATH] = {0};


	::GetEnvironmentVariable(_T("Rav"),szVirPath,MAX_PATH);
	int iEnd = _tcslen(szVirPath);

	if(szVirPath[iEnd] != '\\')
	{
		_tcscat(szVirPath,_T("\\"));
	}

	_tcscpy(szDirPath,szVirPath);
	_tcscat(szVirPath,_T("ClientLog.mdb"));
	
	//
	//mdb文件被独占方式打开，所以将mdb文件拷贝到当前目录
	TCHAR szDesAdrr[MAX_PATH] = {0};
	GetModuleFileName(NULL,szDesAdrr,MAX_PATH);

	CString strDesDir;
	strDesDir.Format(_T("%s"),szDesAdrr);

	iEnd = strDesDir.ReverseFind(_T('\\'));

	CString strLastDir;
	strLastDir = strDesDir.Mid(0,iEnd);

	strLastDir += _T("\\ClientLog.mdb");
	
	BOOL bCopyFile = CopyFile(szVirPath,strLastDir.GetBuffer(0),FALSE);
	if(!bCopyFile)
	{
		return -5;
	}
	
	int iErr = GetLastError();

	CString strConnDB;
	strConnDB.Format(_T("%s%s%s"),_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source="),strLastDir.GetBuffer(0),_T(";Jet OLEDB:Database Password=RisingVirusInfo"));

	int iResult = 0;;
	_ConnectionPtr m_pConnection;//连接access数据库的链接对象
	_RecordsetPtr m_pRecordset;//结果集对象

	CoInitialize(NULL);//初始化

	m_pConnection.CreateInstance(__uuidof(Connection));//实例化对象

	try
	{
		m_pConnection->Open(strConnDB.GetBuffer(0),"","",adModeUnknown);
	}
	catch(_com_error err)
	{
		//连接数据库失败
		return -1;
	}

	try
	{
//		m_pRecordset.CreateInstance(__uuidof(Recordset));

//		CString sql = _T("(select * from Table_VirusAlertBak)union ALL(select * from Table_VirusAlert)");

		CString sql;

		if(_tcslen(szTime) >0)
		{
			//判断是不在指定的时间范围内
			COleDateTime oleTimeNode;
			oleTimeNode.ParseDateTime(szTime);


			CTime tNode(oleTimeNode.GetYear(),oleTimeNode.GetMonth(),oleTimeNode.GetDay(),oleTimeNode.GetHour(),oleTimeNode.GetMinute(),oleTimeNode.GetSecond());

			__time64_t tTime64 = tNode.GetTime();

			sql = _T("(select * from Table_VirusAlertBak where ActionEndTime > tTime64)union ALL(select * from Table_VirusAlert  where ActionEndTime > tTime64)");
		}
		else
		{
			sql = _T("(select * from Table_VirusAlertBak)union ALL(select * from Table_VirusAlert)");
		}

		m_pRecordset->Open(sql.GetBuffer(0),_variant_t((IDispatch*)m_pConnection,true),adOpenStatic,adLockOptimistic,adCmdText);
	}
	catch(_com_error err)
	{
		if(m_pConnection->State)
		{
			m_pConnection->Close();
			m_pConnection = NULL;
		}
		//获取记录集指针失败
		return -2;
	}

	try
	{
		if(m_pRecordset->BOF)
		{
			if(m_pConnection->State)
			{
				m_pRecordset->Close();
				m_pRecordset = NULL;

				m_pConnection->Close();
				m_pConnection=NULL;
			}
			
			//表中数据为空
			return -3;
		}

		//游标定位到第一条记录
		m_pRecordset->MoveFirst();
		
		//
		//从记录集中获取数据放在var中
		_variant_t varFileName;
		_variant_t varFilePath;
		_variant_t varKillTime;

		
		while(!(m_pRecordset->adoEOF))
		{
			WCHAR szName[MAX_PATH] = {0};
			WCHAR szFilePath[MAX_PATH] = {0};
			WCHAR szKillTime[MAX_PATH] = {0};

			CString strReturnValue;
			strReturnValue.Empty();
			USES_CONVERSION;
			//
			//获取文件名称
			varFileName = m_pRecordset->GetCollect(_T("FileName"));

			if(varFileName.vt != VT_NULL)
			{
				wcsncpy(szName,OLE2W((_bstr_t)varFileName),(MAX_PATH/2)-1);

				//MessageBox(NULL,szName,_T(""),MB_OK);

			}

			//
			//获取文件路径
			varFilePath = m_pRecordset->GetCollect(_T("Path"));
			if(varFilePath.vt != VT_NULL)
			{
				wcsncpy(szFilePath,OLE2W((_bstr_t)varFilePath),(MAX_PATH/2)-1);
			}
			
			//
			//获取系统时间
			varKillTime = m_pRecordset->GetCollect(_T("ActionEndTime"));
			CString strTimeRe;
			strTimeRe.Empty();
			if(varKillTime.vt != VT_NULL)
			{
				CTime t(varKillTime.lVal);

				strTimeRe = t.Format("%Y-%m-%d %H:%M:%S");
			}

			if(_tcslen(szTime) >0)
			{
				//判断是不在指定的时间范围内
				COleDateTime oleTimeNode;
				oleTimeNode.ParseDateTime(szTime);

				COleDateTime oleTimeLocal;
				oleTimeLocal.ParseDateTime(strTimeRe);

				CTime tNode(oleTimeNode.GetYear(),oleTimeNode.GetMonth(),oleTimeNode.GetDay(),oleTimeNode.GetHour(),oleTimeNode.GetMinute(),oleTimeNode.GetSecond());
				CTime tLocal(oleTimeLocal.GetYear(),oleTimeLocal.GetMonth(),oleTimeLocal.GetDay(),oleTimeLocal.GetHour(),oleTimeLocal.GetMinute(),oleTimeLocal.GetSecond());

				CTimeSpan spanTime = tLocal - tNode;


				if(spanTime.GetHours() < 0)
				{
					m_pRecordset->MoveNext();
					continue;
				}
			}

			strReturnValue += szName;
			strReturnValue += _T(";");
			strReturnValue += szFilePath;
			strReturnValue += _T(";");
			strReturnValue += strTimeRe;

			CString strKillHeard;
			strKillHeard.Empty();

			strKillHeard.Format(_T("%s"),_T("木马感染记录"));

			InsertLink(pLink,strKillHeard,strReturnValue);
			iCount++;
	
			m_pRecordset->MoveNext();
		}
	}
	catch(_com_error err)
	{
		//其它错误
		iResult =  -4;
	}

	if(m_pConnection->State)
	{
		m_pRecordset->Close();
		m_pRecordset = NULL;
		
		m_pConnection->Close();
		m_pConnection = NULL;
	}

	DeleteFile(strLastDir.GetBuffer(0));

	//连到具体某个mdb ，此处的
	return iResult;
}

//
/*===================================获取硬盘序列号==================================
=======================================================================================
======================================================================================*/

#define  DFP_RECEIVE_DRIVE_DATA   0x0007c088
BOOL DoIDENTIFYNEXT(HANDLE hPhysicalDriveIOCTL, PSENDCMDINPARAMS pSCIP,
				 PSENDCMDOUTPARAMS pSCOP, BYTE bIDCmd, BYTE bDriveNum,
				 PDWORD lpcbBytesReturned)
{
	// Set up data structures for IDENTIFY command.
	pSCIP -> cBufferSize = IDENTIFY_BUFFER_SIZE;
	pSCIP -> irDriveRegs.bFeaturesReg = 0;
	pSCIP -> irDriveRegs.bSectorCountReg = 1;
	//pSCIP -> irDriveRegs.bSectorNumberReg = 1;
	pSCIP -> irDriveRegs.bCylLowReg = 0;
	pSCIP -> irDriveRegs.bCylHighReg = 0;

	// Compute the drive number.
	pSCIP -> irDriveRegs.bDriveHeadReg = 0xA0 | ((bDriveNum & 1) << 4);

	// The command can either be IDE identify or ATAPI identify.
	pSCIP -> irDriveRegs.bCommandReg = bIDCmd;
	pSCIP -> bDriveNumber = bDriveNum;
	pSCIP -> cBufferSize = IDENTIFY_BUFFER_SIZE;

	return ( DeviceIoControl (hPhysicalDriveIOCTL, DFP_RECEIVE_DRIVE_DATA,
		(LPVOID) pSCIP,
		sizeof(SENDCMDINPARAMS) - 1,
		(LPVOID) pSCOP,
		sizeof(SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1,
		lpcbBytesReturned, NULL) );
}
void WriteConstantString (char *entry, char *string)
{
}

char HardDriveModelNumber [1024];

char* ConvertToString (DWORD diskdata [256],int firstIndex,int lastIndex,char* buf)
{
	int index = 0;
	int position = 0;

	//  each integer has two characters stored in it backwards
	for (index = firstIndex; index <= lastIndex; index++)
	{
		//  get high byte for 1st character
		buf [position++] = (char) (diskdata [index] / 256);

		//  get low byte for 2nd character
		buf [position++] = (char) (diskdata [index] % 256);
	}

	//  end the string 
	buf[position] = '\0';

	//  cut off the trailing blanks
	for (index = position - 1; index > 0 && isspace(buf [index]); index--)
		buf [index] = '\0';

	return buf;
}

char HardDriveSerialNumber [1024];

void PrintIdeInfo (int drive, DWORD diskdata [256])
{
	char serialNumber [1024];
	char modelNumber [1024];
	char revisionNumber [1024];
	char bufferSize [32];

	__int64 sectors = 0;
	__int64 bytes = 0;

	//  copy the hard drive serial number to the buffer
	ConvertToString (diskdata, 10, 19, serialNumber);
	ConvertToString (diskdata, 27, 46, modelNumber);
	ConvertToString (diskdata, 23, 26, revisionNumber);
	sprintf (bufferSize, "%u", diskdata [21] * 512);

	if (0 == HardDriveSerialNumber[0] &&
		//  serial number must be alphanumeric
		//  (but there can be leading spaces on IBM drives)
		(isalnum (serialNumber [0]) || isalnum (serialNumber [19])))
	{
		strcpy (HardDriveSerialNumber, serialNumber);
		strcpy (HardDriveModelNumber, modelNumber);
	}

	if(!m_strDiskID.IsEmpty())
		m_strDiskID += ";";

	m_strDiskID += serialNumber;

}

#define  MAX_IDE_DRIVES  16

#define  IDE_ATAPI_IDENTIFY  0xA1  //  Returns ID sector for ATAPI.
#define  IDE_ATA_IDENTIFY    0xEC  //  Returns ID sector for ATA.

#define  DFP_GET_VERSION          0x00074080

typedef struct _GETVERSIONOUTPARAMS
{
	BYTE bVersion;      // Binary driver version.
	BYTE bRevision;     // Binary driver revision.
	BYTE bReserved;     // Not used.
	BYTE bIDEDeviceMap; // Bit map of IDE devices.
	DWORD fCapabilities; // Bit mask of driver capabilities.
	DWORD dwReserved[4]; // For future use.
} GETVERSIONOUTPARAMS, *PGETVERSIONOUTPARAMS, *LPGETVERSIONOUTPARAMS;

int GetIDEDiskNo()
{
	int done = FALSE;
	int drive = 0;

	for (drive = 0; drive < MAX_IDE_DRIVES; drive++)
	{
		HANDLE hPhysicalDriveIOCTL = 0;

		//  Try to get a handle to PhysicalDrive IOCTL, report failure
		//  and exit if can't.
		TCHAR driveName [256];

		swprintf (driveName, _T("\\\\.\\PhysicalDrive%d"), drive);

		//  Windows NT, Windows 2000, must have admin rights
		hPhysicalDriveIOCTL = CreateFile (driveName,
			GENERIC_READ | GENERIC_WRITE, 
			FILE_SHARE_READ | FILE_SHARE_WRITE , NULL,
			OPEN_EXISTING, 0, NULL);

		if (hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE)
		{
			return -1;
		}
		else
		{
			GETVERSIONOUTPARAMS VersionParams;
			DWORD               cbBytesReturned = 0;

			// Get the version, etc of PhysicalDrive IOCTL
			memset ((void*) &VersionParams, 0, sizeof(VersionParams));

			if ( ! DeviceIoControl (hPhysicalDriveIOCTL, DFP_GET_VERSION,
				NULL, 
				0,
				&VersionParams,
				sizeof(VersionParams),
				&cbBytesReturned, NULL) )
			{         
				return -1;
			}

			// If there is a IDE device at number "i" issue commands
			// to the device
			if (VersionParams.bIDEDeviceMap <= 0)
			{
				return -2;
			}
			else
			{
				BYTE             bIDCmd = 0;   // IDE or ATAPI IDENTIFY cmd
				SENDCMDINPARAMS  scip;
				BYTE IdOutCmd [sizeof (SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE - 1];
				//SENDCMDOUTPARAMS OutCmd;

				// Now, get the ID sector for all IDE devices in the system.
				// If the device is ATAPI use the IDE_ATAPI_IDENTIFY command,
				// otherwise use the IDE_ATA_IDENTIFY command
				bIDCmd = (VersionParams.bIDEDeviceMap >> drive & 0x10) ? IDE_ATAPI_IDENTIFY : IDE_ATA_IDENTIFY;

				memset (&scip, 0, sizeof(scip));
				memset (IdOutCmd, 0, sizeof(IdOutCmd));

				if ( DoIDENTIFYNEXT(hPhysicalDriveIOCTL, 
					&scip, 
					(PSENDCMDOUTPARAMS)&IdOutCmd, 
					(BYTE) bIDCmd,
					(BYTE) drive,
					&cbBytesReturned))
				{
					DWORD diskdata [256];
					int ijk = 0;
					USHORT *pIdSector = (USHORT *)
						((PSENDCMDOUTPARAMS) IdOutCmd) -> bBuffer;

					for (ijk = 0; ijk < 256; ijk++)
						diskdata [ijk] = pIdSector [ijk];

					PrintIdeInfo(drive, diskdata);

					done = TRUE;
				}
			}

			CloseHandle (hPhysicalDriveIOCTL);
		}
	}
	return 0;
}


typedef struct _SRB_IO_CONTROL
{
	ULONG HeaderLength;
	UCHAR Signature[8];
	ULONG Timeout;
	ULONG ControlCode;
	ULONG ReturnCode;
	ULONG Length;
} SRB_IO_CONTROL, *PSRB_IO_CONTROL;

typedef struct _IDSECTOR
{
	USHORT  wGenConfig;
	USHORT  wNumCyls;
	USHORT  wReserved;
	USHORT  wNumHeads;
	USHORT  wBytesPerTrack;
	USHORT  wBytesPerSector;
	USHORT  wSectorsPerTrack;
	USHORT  wVendorUnique[3];
	CHAR    sSerialNumber[20];
	USHORT  wBufferType;
	USHORT  wBufferSize;
	USHORT  wECCSize;
	CHAR    sFirmwareRev[8];
	CHAR    sModelNumber[40];
	USHORT  wMoreVendorUnique;
	USHORT  wDoubleWordIO;
	USHORT  wCapabilities;
	USHORT  wReserved1;
	USHORT  wPIOTiming;
	USHORT  wDMATiming;
	USHORT  wBS;
	USHORT  wNumCurrentCyls;
	USHORT  wNumCurrentHeads;
	USHORT  wNumCurrentSectorsPerTrack;
	ULONG   ulCurrentSectorCapacity;
	USHORT  wMultSectorStuff;
	ULONG   ulTotalAddressableSectors;
	USHORT  wSingleWordDMA;
	USHORT  wMultiWordDMA;
	BYTE    bReserved[128];
} IDSECTOR, *PIDSECTOR;

#define  SENDIDLENGTH  sizeof (SENDCMDOUTPARAMS) + IDENTIFY_BUFFER_SIZE

#define  IOCTL_SCSI_MINIPORT_IDENTIFY  ((FILE_DEVICE_SCSI << 16) + 0x0501)
#define  IOCTL_SCSI_MINIPORT 0x0004D008  //  see NTDDSCSI.H for definition

#define  FILE_DEVICE_SCSI              0x0000001b

int ReadIdeDriveAsScsiDriveInNT (TCHAR* szDiskID)
{
	int done = FALSE;
	int controller = 0;

	for (controller = 0; controller < 16; controller++)
	{
		HANDLE hScsiDriveIOCTL = 0;
		TCHAR   driveName [256];

		//  Try to get a handle to PhysicalDrive IOCTL, report failure
		//  and exit if can't.
		swprintf (driveName, _T("\\\\.\\Scsi%d:"), controller);

		//  Windows NT, Windows 2000, any rights should do
		hScsiDriveIOCTL = CreateFile (driveName,
			GENERIC_READ | GENERIC_WRITE, 
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, 0, NULL);

		if (hScsiDriveIOCTL != INVALID_HANDLE_VALUE)
		{
			int drive = 0;

			for (drive = 0; drive < 2; drive++)
			{
				char buffer [sizeof (SRB_IO_CONTROL) + SENDIDLENGTH];
				SRB_IO_CONTROL *p = (SRB_IO_CONTROL *) buffer;
				SENDCMDINPARAMS *pin =
					(SENDCMDINPARAMS *) (buffer + sizeof (SRB_IO_CONTROL));
				DWORD dummy;

				memset (buffer, 0, sizeof (buffer));
				p -> HeaderLength = sizeof (SRB_IO_CONTROL);
				p -> Timeout = 10000;
				p -> Length = SENDIDLENGTH;
				p -> ControlCode = IOCTL_SCSI_MINIPORT_IDENTIFY;
				strncpy ((char *) p -> Signature, "SCSIDISK", 8);

				pin -> irDriveRegs.bCommandReg = IDE_ATA_IDENTIFY;
				pin -> bDriveNumber = drive;

				if (DeviceIoControl (hScsiDriveIOCTL, IOCTL_SCSI_MINIPORT, 
					buffer,
					sizeof (SRB_IO_CONTROL) +
					sizeof (SENDCMDINPARAMS) - 1,
					buffer,
					sizeof (SRB_IO_CONTROL) + SENDIDLENGTH,
					&dummy, NULL))
				{
					SENDCMDOUTPARAMS *pOut =
						(SENDCMDOUTPARAMS *) (buffer + sizeof (SRB_IO_CONTROL));
					IDSECTOR *pId = (IDSECTOR *) (pOut -> bBuffer);
					if (pId -> sModelNumber [0])
					{
						DWORD diskdata [256];
						int ijk = 0;
						USHORT *pIdSector = (USHORT *) pId;

						for (ijk = 0; ijk < 256; ijk++)
							diskdata [ijk] = pIdSector [ijk];

						PrintIdeInfo(controller * 2 + drive, diskdata);

						done = TRUE;
					}
				}
			}
			CloseHandle (hScsiDriveIOCTL);
		}
		else
		{
			return -3;
		}
	}

	return done;
}

char * flipAndCodeBytes (const char * str,
						 int pos,
						 int flip,
						 char * buf)
{
	int i;
	int j = 0;
	int k = 0;

	buf [0] = '\0';
	if (pos <= 0)
		return buf;

	if ( ! j)
	{
		char p = 0;

		// First try to gather all characters representing hex digits only.
		j = 1;
		k = 0;
		buf[k] = 0;
		for (i = pos; j && str[i] != '\0'; ++i)
		{
			char c = tolower(str[i]);

			if (isspace(c))
				c = '0';

			++p;
			buf[k] <<= 4;

			if (c >= '0' && c <= '9')
				buf[k] |= (unsigned char) (c - '0');
			else if (c >= 'a' && c <= 'f')
				buf[k] |= (unsigned char) (c - 'a' + 10);
			else
			{
				j = 0;
				break;
			}

			if (p == 2)
			{
				if (buf[k] != '\0' && ! isprint(buf[k]))
				{
					j = 0;
					break;
				}
				++k;
				p = 0;
				buf[k] = 0;
			}

		}
	}

	if ( ! j)
	{
		// There are non-digit characters, gather them as is.
		j = 1;
		k = 0;
		for (i = pos; j && str[i] != '\0'; ++i)
		{
			char c = str[i];

			if ( ! isprint(c))
			{
				j = 0;
				break;
			}

			buf[k++] = c;
		}
	}

	if ( ! j)
	{
		// The characters are not there or are not printable.
		k = 0;
	}

	buf[k] = '\0';

	if (flip)
		// Flip adjacent characters
		for (j = 0; j < k; j += 2)
		{
			char t = buf[j];
			buf[j] = buf[j + 1];
			buf[j + 1] = t;
		}

		// Trim any beginning and end space
		i = j = -1;
		for (k = 0; buf[k] != '\0'; ++k)
		{
			if (! isspace(buf[k]))
			{
				if (i < 0)
					i = k;
				j = k;
			}
		}

		if ((i >= 0) && (j >= 0))
		{
			for (k = i; (k <= j) && (buf[k] != '\0'); ++k)
				buf[k - i] = buf[k];
			buf[k - i] = '\0';
		}

		return buf;
}
int ReadPhysicalDriveInNTWithZeroRights (TCHAR * szDiskID)
{
	int done = FALSE;
	int drive = 0;

	for (drive = 0; drive < MAX_IDE_DRIVES; drive++)
	{
		HANDLE hPhysicalDriveIOCTL = 0;

		//  Try to get a handle to PhysicalDrive IOCTL, report failure
		//  and exit if can't.
		TCHAR driveName [256];

		swprintf (driveName, _T("\\\\.\\PhysicalDrive%d"), drive);

		//  Windows NT, Windows 2000, Windows XP - admin rights not required
		hPhysicalDriveIOCTL = CreateFile (driveName, 0,
			FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, 0, NULL);
		if (hPhysicalDriveIOCTL == INVALID_HANDLE_VALUE)
		{

		}
		else
		{
			STORAGE_PROPERTY_QUERY query;
			DWORD cbBytesReturned = 0;
			char buffer [10000];

			memset ((void *) & query, 0, sizeof (query));
			query.PropertyId = StorageDeviceProperty;
			query.QueryType = PropertyStandardQuery;

			memset (buffer, 0, sizeof (buffer));

			if ( DeviceIoControl (hPhysicalDriveIOCTL, IOCTL_STORAGE_QUERY_PROPERTY,
				& query,
				sizeof (query),
				& buffer,
				sizeof (buffer),
				& cbBytesReturned, NULL) )
			{         
				STORAGE_DEVICE_DESCRIPTOR * descrip = (STORAGE_DEVICE_DESCRIPTOR *) & buffer;
				char serialNumber [1000];
				char modelNumber [1000];
				char vendorId [1000];
				char productRevision [1000];

				flipAndCodeBytes (buffer,
					descrip -> VendorIdOffset,
					0, vendorId );
				flipAndCodeBytes (buffer,
					descrip -> ProductIdOffset,
					0, modelNumber );
				flipAndCodeBytes (buffer,
					descrip -> ProductRevisionOffset,
					0, productRevision );
				flipAndCodeBytes (buffer,
					descrip -> SerialNumberOffset,
					1, serialNumber );

				if (0 == HardDriveSerialNumber [0] &&
					//  serial number must be alphanumeric
					//  (but there can be leading spaces on IBM drives)
					(isalnum (serialNumber [0]) || isalnum (serialNumber [19])))
			 {
				 strcpy (HardDriveSerialNumber, serialNumber);
				 strcpy (HardDriveModelNumber, modelNumber);

				 if(!m_strDiskID.IsEmpty())
					 m_strDiskID += ";";

				 m_strDiskID += serialNumber;

				 done = TRUE;
			 }

				// Get the disk drive geometry.
				memset (buffer, 0, sizeof(buffer));
				if ( ! DeviceIoControl (hPhysicalDriveIOCTL,
					IOCTL_DISK_GET_DRIVE_GEOMETRY_EX,
					NULL,
					0,
					&buffer,
					sizeof(buffer),
					&cbBytesReturned,
					NULL))
				{

				}
				else
				{         
					DISK_GEOMETRY_EX* geom = (DISK_GEOMETRY_EX*) &buffer;
					int fixed = (geom->Geometry.MediaType == FixedMedia);
					__int64 size = geom->DiskSize.QuadPart;

				}
			}
			else
			{
				DWORD err = GetLastError ();
			}

			CloseHandle (hPhysicalDriveIOCTL);
		}
	}

	return done;
}

int __stdcall GetOtherDiskSer(pHardWareInformation &pLink,int& iCount)
{
	m_strDiskID.Empty();
	GetIDEDiskNo();

	ReadIdeDriveAsScsiDriveInNT(NULL);

	ReadPhysicalDriveInNTWithZeroRights(NULL);

	m_strDiskID.Replace(_T(" "),_T(""));


	CString strValueName;
	strValueName.Format(_T("%s"),_T("另一获取硬盘序列号"));
	
	int iSeparePos = 0;
	int iSeparePre = 0;
	

	iSeparePos = m_strDiskID.Find(_T(';'),iSeparePre);

	if(iSeparePos == -1)
	{
		InsertLink(pLink,strValueName,m_strDiskID);
		iCount++;
	}
	else
	{
		CString strCompareValue;
		strCompareValue.Empty();

		CString strAddValue;
		
		while(iSeparePos != -1)
		{
			strAddValue.Empty();

			strAddValue += m_strDiskID.Mid(iSeparePre,iSeparePos-iSeparePre);

			if(strCompareValue.Find(strAddValue.MakeUpper().GetBuffer(0)) == -1)
			{
				InsertLink(pLink,strValueName,strAddValue);
				iCount++;
			}
			
			strCompareValue += strAddValue;
			strCompareValue += _T("");

			iSeparePre = iSeparePos + 1;
			iSeparePos =  m_strDiskID.Find(_T(';'),iSeparePre);

		}
		
		strAddValue.Empty();
		strAddValue += m_strDiskID.Mid(iSeparePre,m_strDiskID.GetLength()-iSeparePre);

		if(strCompareValue.Find(strAddValue.MakeUpper().GetBuffer(0)) == -1)
		{
			InsertLink(pLink,strValueName,strAddValue);
			iCount++;
		}

	}
		
	return 0;
}

#include "setupapi.h"
#include "devguid.h"

#pragma comment(lib,"setupapi.lib")

int __stdcall GetOtherCDROMInfo(pHardWareInformation &pLink,int& iCount)
{
	//得到设备信息集
	HDEVINFO hDevInfo = SetupDiGetClassDevs(
			(LPGUID)&GUID_DEVCLASS_CDROM,
			NULL,
			0,
			DIGCF_PRESENT);
	if(hDevInfo == INVALID_HANDLE_VALUE)
	{
		SetupDiDestroyDeviceInfoList(hDevInfo);
		return -1;
	}

	TCHAR szBuf[MAX_PATH] = {0};
	SP_DEVINFO_DATA spDevInfoData = {sizeof(SP_DEVINFO_DATA)};

	//开始列举设备
	DWORD i;
	for(i = 0; SetupDiEnumDeviceInfo(hDevInfo,i,&spDevInfoData);i++)
	{
		if(SetupDiGetDeviceRegistryProperty(
			hDevInfo,
			&spDevInfoData,
			SPDRP_FRIENDLYNAME,
			NULL,
			(PBYTE)szBuf,
			MAX_PATH,
			NULL))
		{
			CString strValueName;
			strValueName.Empty();
			strValueName.Format(_T("%s"),_T("另一获取CDROM序列号"));

			CString strValue;
			strValue.Empty();
			strValue.Format(_T("%s"),szBuf);

			InsertLink(pLink,strValueName,strValue);
			iCount++;
		}
	}

	return 0;
}

int __stdcall GetLogInfo(pHardWareInformation &pLink,int& iCount)
{

	HKEY hsysLogKey = NULL;
	HKEY hsecLogKey = NULL;

	DWORD dwType=REG_DWORD;
	DWORD dwCount=0;

	DWORD dSysSize;
	DWORD dSysDay;

	long lResult  = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,_TEXT("SYSTEM\\CurrentControlSet\\Services\\Eventlog\\System"),0,KEY_READ,&hsysLogKey);


	if(lResult != ERROR_SUCCESS)
	{

		return -1;
	}
	CString strSysValueName;
	strSysValueName.Empty();
	strSysValueName.Format(_T("%s"),_T("SYSTEM MAXSIZE"));

	CString strSysValue;
	strSysValue.Empty();

	lResult = RegQueryValueEx(hsysLogKey,_T("MaxSize"), NULL, &dwType,NULL, &dwCount);

	if(lResult != ERROR_SUCCESS)
	{
		InsertLink(pLink,strSysValueName,strSysValue);
		iCount++;
		return -1;
	}

	RegQueryValueEx(hsysLogKey,_T("MaxSize"), NULL, &dwType,(BYTE*)&dSysSize, &dwCount);


	strSysValue.Format(_T("%.2f"),(double)dSysSize/(1024*1024));


	InsertLink(pLink,strSysValueName,strSysValue);
	iCount++;

	//============================================================================================
	lResult = RegQueryValueEx(hsysLogKey,_T("Retention"), NULL, &dwType,NULL, &dwCount);

	CString strSysDayName;
	strSysDayName.Empty();
	strSysDayName.Format(_T("%s"),_T("SYS  day"));

	CString strSysDayValue;
	strSysDayValue.Empty();
	if(lResult != ERROR_SUCCESS)
	{
		InsertLink(pLink,strSysDayName,strSysDayValue);
		iCount++;
		return -1;
	}

	RegQueryValueEx(hsysLogKey,_T("Retention"), NULL, &dwType,(BYTE*)&dSysDay, &dwCount);

	strSysDayValue.Format(_T("%d"),dSysDay/(24*60*60));

	InsertLink(pLink,strSysDayName,strSysDayValue);
	iCount++;


	::RegCloseKey(hsysLogKey);

	//======================================================================================================安全日志
	DWORD dwSecSize;
	DWORD dwSecDay;

	lResult  = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,_TEXT("SYSTEM\\CurrentControlSet\\Services\\Eventlog\\Security"),0,KEY_READ,&hsecLogKey);

	if(lResult != ERROR_SUCCESS)
	{
		return -1;
	}

	lResult = RegQueryValueEx(hsecLogKey,_T("MaxSize"), NULL, &dwType,NULL, &dwCount);

	CString strSecMaxName;
	strSecMaxName.Empty();
	strSecMaxName.Format(_T("%s"),_T("SEC MAXSIZE"));

	CString strSecMaxValue;
	strSecMaxValue.Empty();

	if(lResult != ERROR_SUCCESS)
	{
		InsertLink(pLink,strSecMaxName,strSecMaxValue);
		iCount++;
		return -1;
	}

	RegQueryValueEx(hsecLogKey,_T("MaxSize"), NULL, &dwType,(BYTE*)&dwSecSize, &dwCount);
	strSecMaxValue.Format(_T("%.2f"),(double)dwSecSize/(1024*1024));


	InsertLink(pLink,strSecMaxName,strSecMaxValue);
	iCount++;


	lResult = RegQueryValueEx(hsecLogKey,_T("Retention"), NULL, &dwType,NULL, &dwCount);

	CString strSecDayName;
	strSecDayName.Empty();
	strSecDayName.Format(_T("%s"),_T("SEC  day"));

	CString strSecDayxValue;
	strSecDayxValue.Empty();
	if(lResult != ERROR_SUCCESS)
	{
		InsertLink(pLink,strSecDayName,strSecDayxValue);
		iCount++;
		return -1;
	}

	RegQueryValueEx(hsecLogKey,_T("Retention"), NULL, &dwType,(BYTE*)&dwSecDay, &dwCount);


	strSecDayxValue.Format(_T("%d"),dwSecDay/(24*60*60));

	InsertLink(pLink,strSecDayName,strSecDayxValue);
	iCount++;

	::RegCloseKey(hsecLogKey);

	return 0;
}

int __stdcall GetPatchInfoLastTime(pHardWareInformation &pLink,int& iCount)
{

	int iResult  =  GetWmiInfo(_T("Win32_QuickFixEngineering where HotFixID!='File 1'"),_T("HotFixID,InstalledOn"),pLink,iCount);// where HotFixID!='File 1'


	return iResult;

}