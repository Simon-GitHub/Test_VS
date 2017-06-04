/*****************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * $Id: fileupload.c,v 1.3 2007-07-12 21:11:10 danf Exp $
 */

#pragma once
#include <stdio.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include <fcntl.h>

int fileupload()
//int main(void)
{
  CURL *curl;
  CURLcode res;
  struct stat file_info;
  double speed_upload, total_time;
  FILE *fd;

  fd = fopen("debugit", "rb"); /* open file to upload */
  if(!fd) {

    return 1; /* can't continue */
  }

  stat("debugit", &file_info); /* to get the file size */

  curl = curl_easy_init();
  if(curl) {
    /* upload to this place */
	//curl_easy_setopt(curl, CURLOPT_URL, "file:///home/dast/src/curl/debug/new");
	curl_easy_setopt(curl, CURLOPT_URL, "ftp://117server:123456@192.168.0.117/����Խ�1111.avi");

    /* tell it to "upload" to the URL */
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1);

    /* set where to read from (on Windows you need to use READFUNCTION too) */
    curl_easy_setopt(curl, CURLOPT_READDATA, fd);

    /* and give the size of the upload (optional) */
    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
                     (curl_off_t)file_info.st_size);

    /* enable verbose for easier tracing */
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);

    res = curl_easy_perform(curl);

    /* now extract transfer info */
    curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &speed_upload);
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);

    fprintf(stderr, "Speed: %.3f bytes/sec during %.3f seconds\n",
            speed_upload, total_time);

    /* always cleanup */
    curl_easy_cleanup(curl);
  }
  return 0;
}