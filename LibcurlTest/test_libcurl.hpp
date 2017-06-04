#pragma once
#include <stdio.h>
#include <string>
#include <curl/curl.h>
#include <QString>

std::string string_To_UTF8(const std::string & str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char * pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete []pwBuf;
	delete []pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;
}

static size_t process_data1(void *date, size_t size, size_t number, std::string &content)
{
	long sizes = size * number;
	std::string temp;
	temp = std::string((char*)date,sizes);
	content += temp;
	return sizes;
}

int test_libcurl()
{
#if 1
	//std::string str = "通信";
	//str = string_To_UTF8(str);
	//std::string url = std::string("http://192.168.0.117:8080/api/statistics/compare-technology-by-application-area/get-satellite-information-by-apparea-and-satellite/?Apparea_name=")+str+std::string("&satellite_num=8");
	std::string s = "通信";
	s = string_To_UTF8(s);
	QString str = QString::fromLocal8Bit(s.c_str());
	QString url = QString("http://192.168.0.117:8080/api/statistics/compare-technology-by-application-area/get-satellite-information-by-apparea-and-satellite/?Apparea_name=")+str+QString("&satellite_num=8");

	CURL *hnd = curl_easy_init();

	curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
	curl_easy_setopt(hnd, CURLOPT_URL, url.toLocal8Bit().data());

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "cache-control: no-cache");
	curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

	std::string content;
	content.clear();
	curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &content);
	curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, process_data1);

	CURLcode ret = curl_easy_perform(hnd);
	curl_easy_cleanup(hnd);
#else

  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
  if(curl) {
    /* First set the URL that is about to receive our POST. This URL can
       just as well be a https:// URL if that is what should receive the
       data. */
    curl_easy_setopt(curl, CURLOPT_URL, "http://postit.example.com/moo.cgi");
    /* Now specify the POST data */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "name=daniel&project=curl");

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);

    /* always cleanup */
    curl_easy_cleanup(curl);
  }
#endif
  return 0;
}
