/*****************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * $Id: multi-post.c,v 1.5 2007-07-12 21:11:10 danf Exp $
 *
 * This is an example application source code using the multi interface
 * to do a multipart formpost without "blocking".
 */
#pragma once
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <curl/curl.h>

std::string UTF8_To_string(const std::string & str) 
{ 
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0); 
	wchar_t * pwBuf = new wchar_t[nwLen + 1];
	memset(pwBuf, 0, nwLen * 2 + 2); 
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen); 
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL); 
	char * pBuf = new char[nLen + 1]; 
	memset(pBuf, 0, nLen + 1);
	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL); 
	std::string retStr = pBuf; 
	delete []pBuf; 
	delete []pwBuf; 
	pBuf = NULL; 
	pwBuf = NULL; 
	return retStr; 
}
static size_t process_data_2(void *date, size_t size, size_t number, std::string &content)
{
	long sizes = size * number;
	std::string temp;
	temp = std::string((char*)date,sizes);
	content += temp;
	return sizes;
}

int post_callback_2()
{
  CURL *curl;

  int still_running;

  struct curl_httppost *formpost=NULL;
  struct curl_httppost *lastptr=NULL;
  struct curl_slist *headers=NULL;
  static const char buf[] = "Expect:";

  curl_formadd(&formpost,
	  &lastptr,
	  CURLFORM_COPYNAME, "target_obj",
	  CURLFORM_COPYCONTENTS, "{\"cn_name\": \"广州12\",\"en_name\": \"Guangzhou\",\"longitude\": 100, \"latitude\": 200, \"category\": \"china_observe_and_control_station\"}",
	  CURLFORM_END);

  curl_formadd(&formpost,
	  &lastptr,
	  CURLFORM_COPYNAME, "icon",
	  CURLFORM_FILE, "./city.png",
	  CURLFORM_CONTENTTYPE, "image/png",
	  CURLFORM_END);

  curl = curl_easy_init();

  /* initalize custom header list (stating that Expect: 100-continue is not
     wanted */
  headers = curl_slist_append(headers, buf);
  if(curl) {

    /* what URL that receives this POST */
	curl_easy_setopt(curl, CURLOPT_URL,"http://192.168.0.117:8080/api/target/add-target");
	headers = curl_slist_append(headers, "cache-control: no-cache");
	headers = curl_slist_append(headers, "content-type: multipart/form-data");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	std::string content;
	content.clear();
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, process_data_2);

    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);

    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

	curl_easy_perform(curl);

    /* always cleanup */
    curl_easy_cleanup(curl);

    /* free slist */
    curl_slist_free_all (headers);
	content = UTF8_To_string(content);
  }
  return 0;
}



