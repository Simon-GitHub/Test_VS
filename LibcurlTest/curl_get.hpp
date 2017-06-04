/*****************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * $Id: https.c,v 1.3 2007-07-12 21:11:10 danf Exp $
 */

/*
CURL *hnd = curl_easy_init();

curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
curl_easy_setopt(hnd, CURLOPT_URL, "http://192.168.0.117:8080/api/statistics/statistics-satellites-country-year-amount/?country_num=3&year_num=10");

struct curl_slist *headers = NULL;
headers = curl_slist_append(headers, "postman-token: dd6e6c2e-1c52-a797-721b-9b56a0524a6f");
headers = curl_slist_append(headers, "cache-control: no-cache");
curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

CURLcode ret = curl_easy_perform(hnd);
*/
#pragma once
#include <stdio.h>
#include <curl/curl.h>

int curl_get()
{
	CURL *hnd = curl_easy_init();

	curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
	curl_easy_setopt(hnd, CURLOPT_URL, "http://192.168.0.117:8080/api/statistics/statistics-satellites-country-year-amount/?country_num=3&year_num=10");
	curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1);

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "cache-control: no-cache");
	curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);


	CURLcode ret = curl_easy_perform(hnd);
    /* always cleanup */
    curl_easy_cleanup(hnd);
 
	return 0;
}
