#pragma once
#include "tinyxml.h"

void init_msg(const char* type, unsigned char* data)
{
	int pos = 2;
	time_t ltime;
	struct tm * pcurtime;
	time(&ltime);
	pcurtime = localtime(&ltime);

	std::string		satid_ = "12345678";
	memcpy(data + pos, satid_.c_str(), 8);
	pos += 8;
	data[pos++] = (pcurtime->tm_year + 1900) % 256;
	data[pos++] = (pcurtime->tm_year + 1900) / 256;
	data[pos++] = pcurtime->tm_mon + 1;
	data[pos++] = pcurtime->tm_mday;
	data[pos++] = (char)pcurtime->tm_hour;
	data[pos++] = (char)pcurtime->tm_min;
	data[pos++] = (char)pcurtime->tm_sec;

	data[pos++] = 'R';
	memcpy(data + pos, type, 4);
	pos += 4;
	data[pos++] = ':';
	memcpy(data + pos, "TMF", 3);
	pos += 3;
	memcpy(data + pos, "MTP", 3);
	pos += 3;
}

void ReadWriteFile()
{
	unsigned char	msgbuffer_[20480] = "<STCSTA><FS>YYYY</FS></STCSTA>";
	TiXmlDocument *doc = new TiXmlDocument();
	doc->Parse((char*)msgbuffer_);

	TiXmlElement *rootElement = doc->FirstChild("STCSTA")->FirstChild("FS")->ToElement();
	const char* strcontent = rootElement->GetText();
	char strcomd[20];
	int i = '\0';
	//memset(strcomd,'\0',sizeof(strcomd));
	memset(strcomd,0,sizeof(strcomd));
	sprintf(strcomd,"0000%-12s",strcontent);
	{
		unsigned char	msg_[20480];
		memset(msg_, 0, sizeof(msg_));
		init_msg("COMD", msg_);
		std::cout << msg_+2 << endl;
		std::cout << (int)(msg_+10) << endl;
		printf("%d",msg_+10);

		int yearMin, yearMax, mon, day, hour, min, sec;
		yearMin = msg_[10];
		yearMax = msg_[11];
		mon = msg_[12];
		day = msg_[13];
		hour = msg_[14];
		min = msg_[15];
		sec = msg_[16];
		char str[20];
		sprintf(str, "%d-%d-%d %d:%d:%d", yearMax*256+yearMin,mon,day,hour,min,sec);
		printf("%s",str);
	}
	char buf[20];
	int len_1 = sizeof(buf);
	int len_2 = strlen(buf);
	char* buffer = (char*)malloc(70);
	int len_3 = sizeof(buffer);
	int len_4 = strlen(buffer);
}