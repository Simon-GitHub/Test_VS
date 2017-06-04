#include <stdio.h>
#include <string.h>
#include <string>
#include <time.h>
#include "test_OpenSSL_Base64.hpp"

int main()
{
	//test_OpenSSL();
	test_OpenSSL_Base64();
	////////////////////////////////////////////////////////////////
	//char a[10] = "abcd";
	//char saveDataFlagFileName[512];
	//strcpy(&saveDataFlagFileName[0], ".\\SaveData.flag");
	////sprintf(saveDataFlagFileName, ".\\SaveData.flag");
	//char saveDataFileBuffer[1024]= {0};
	//FILE * saveDataFileFd;
	//saveDataFileFd = fopen(saveDataFlagFileName, "r");
	//if (saveDataFileFd == NULL)
	//{
	//	printf("File open failure!\n");
	//}
	//if (!fgets(saveDataFileBuffer, 1024, saveDataFileFd))
	//{
	//	printf("File read error!\n");
	//}
	//fclose(saveDataFileFd);
	////////////////////////////////////////////////////////////////
#if 0
	char filename[256] = {0};
	std::string name("hello");
	time_t t = time( 0 );
	struct tm * ii;
	ii = localtime(&t);
	strftime( filename, sizeof( filename ), "\\%Y%m%d_%H%M%S",localtime( &t ) );
	std::string dataname = filename + name + ".dat";
	std::string indexname = filename + name + ".dex";
#endif
	system("pause");
	return 0;
}