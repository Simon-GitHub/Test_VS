/*****************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * $Id: post-callback.c,v 1.7 2007-07-16 21:22:12 danf Exp $
 *
 * An example source code that issues a HTTP POST and we provide the actual
 * data through a read callback.
 *
 */
#pragma once
#include <stdio.h>
#include <string>
#include <curl/curl.h>

//const char data[]="this is what we post to the silly web server";
const char data[]="{\"account\":\"yucanlin\",\"password\":\"123456\"}";

struct WriteThis {
  const char *readptr;
  int sizeleft;
};

static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *userp)
{
  struct WriteThis *pooh = (struct WriteThis *)userp;

  if(size*nmemb < 1)
    return 0;

  if(pooh->sizeleft) {
    *(char *)ptr = pooh->readptr[0]; /* copy one single byte */
    pooh->readptr++;                 /* advance pointer */
    pooh->sizeleft--;                /* less data left */
    return 1;                        /* we return 1 byte at a time! */
  }

  return -1;                         /* no more data left to deliver */
}

static size_t process_data(void *date, size_t size, size_t number, std::string &content)
{
	long sizes = size * number;
	std::string temp;
	temp = std::string((char*)date,sizes);
	content += temp;
	return sizes;
}

int post_callback(void)
{
  CURL *curl;
  CURLcode res;

  struct WriteThis pooh;

  pooh.readptr = data;
  pooh.sizeleft = strlen(data);

  curl = curl_easy_init();
  if(curl) {
    /* First set the URL that is about to receive our POST. */
    curl_easy_setopt(curl, CURLOPT_URL,"http://192.168.0.117:8080/api/user/login-by-3d-without-encrypt");

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "cache-control: no-cache");
	headers = curl_slist_append(headers, "content-type: application/json");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	/* Now specify the POST data */
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{\"account\":\"yucanlin\",\"password\":\"123456\"}");
   /* Now specify we want to POST data */
    curl_easy_setopt(curl, CURLOPT_POST, 1);

    /* we want to use our own read function */
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);

    /* pointer to pass to our read function */
    curl_easy_setopt(curl, CURLOPT_READDATA, &pooh);

	std::string content;
	content.clear();
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, process_data);
    /* get verbose debug output please */
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
#if 0
    /*
      If you use POST to a HTTP 1.1 server, you can send data without knowing
      the size before starting the POST if you use chunked encoding. You
      enable this by adding a header like "Transfer-Encoding: chunked" with
      CURLOPT_HTTPHEADER. With HTTP 1.0 or without chunked transfer, you must
      specify the size in the request.
    */
#ifdef USE_CHUNKED
    {
      curl_slist *chunk = NULL;

      chunk = curl_slist_append(chunk, "Transfer-Encoding: chunked");
      res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
      /* use curl_slist_free_all() after the *perform() call to free this
         list again */
    }
#else
    /* Set the expected POST size. If you want to POST large amounts of data,
       consider CURLOPT_POSTFIELDSIZE_LARGE */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, pooh.sizeleft);
#endif

#ifdef DISABLE_EXPECT
    /*
      Using POST with HTTP 1.1 implies the use of a "Expect: 100-continue"
      header.  You can disable this header with CURLOPT_HTTPHEADER as usual.
      NOTE: if you want chunked transfer too, you need to combine these two
      since you can only set one list of headers with CURLOPT_HTTPHEADER. */

    /* A less good option would be to enforce HTTP 1.0, but that might also
       have other implications. */
    {
      curl_slist *chunk = NULL;

      chunk = curl_slist_append(chunk, "Expect:");
      res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
      /* use curl_slist_free_all() after the *perform() call to free this
         list again */
    }
#endif
#endif
    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);

    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  return 0;
}