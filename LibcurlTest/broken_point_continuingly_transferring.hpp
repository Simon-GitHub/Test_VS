#if 0
#pragma once
//����CURLOPT_RESUME_FROM_LARGE ʵ���ļ��ϵ���������
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#include <curl/curl.h>
//�������ΪCURLOPT_HEADERFUNCTION��������
/* ��httpͷ����ȡ�ļ�size*/
size_t getcontentlengthfunc(void *ptr, size_t size, size_t nmemb, void *stream) 
{
	int r;
	long len = 0;
	r = sscanf((const char *)ptr, "Content-Length: %ld\n", &len);
	if (r) /* Microsoft: we don't read the specs */
		*((long *) stream) = len;

	return size * nmemb;
}

/* ���������ļ� */
size_t wirtefunc(void *ptr, size_t size, size_t nmemb, void *stream)
{
	return fwrite(ptr, size, nmemb, (FILE *)stream);
}

/*��ȡ�ϴ��ļ� */
size_t readfunc(void *ptr, size_t size, size_t nmemb, void *stream)
{
	FILE *f = (FILE *)stream;
	size_t n;

	if (ferror(f))
		return CURL_READFUNC_ABORT;

	n = fread(ptr, size, nmemb, f) * size;

	return n;
}

// ���� �����ϴ��ļ�����
int download(CURL *curlhandle, const char * remotepath, const char * localpath,long timeout, long tries)
{
	FILE *f;
	curl_off_t local_file_len = -1 ;
	long filesize =0 ;

	CURLcode r = CURLE_GOT_NOTHING;
	int c;
	struct stat file_info;
	int use_resume = 0;
	/* �õ������ļ���С */
	//if(access(localpath,F_OK) ==0)
	if(stat(localpath, &file_info) == 0) 
	{
		local_file_len =  file_info.st_size;
		use_resume  = 1;
	}
	//����׷�ӷ�ʽ���ļ�������ʵ���ļ��ϵ���������
	f = fopen(localpath, "ab+"); 
	if (f == NULL) {
		perror(NULL);
		return 0;
	}

	//curl_easy_setopt(curlhandle, CURLOPT_UPLOAD, 1L);

	curl_easy_setopt(curlhandle, CURLOPT_URL, remotepath);
	curl_easy_setopt(curlhandle, CURLOPT_CONNECTTIMEOUT, timeout);  // �������ӳ�ʱ����λ��
	//����http ͷ��������
	curl_easy_setopt(curlhandle, CURLOPT_HEADERFUNCTION, getcontentlengthfunc);
	curl_easy_setopt(curlhandle, CURLOPT_HEADERDATA, &filesize);
	// �����ļ�������λ�ø�libcurl
	curl_easy_setopt(curlhandle, CURLOPT_RESUME_FROM_LARGE, use_resume?local_file_len:0);
#if 1
	curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, f);
	curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, wirtefunc);
#else
	curl_easy_setopt(curlhandle, CURLOPT_READFUNCTION, readfunc);
	curl_easy_setopt(curlhandle, CURLOPT_READDATA, f);
#endif
	curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 1L);

	r = curl_easy_perform(curlhandle);
	fclose(f);
	if (r == CURLE_OK)
		return 1;
	else {
		fprintf(stderr, "%s\n", curl_easy_strerror(r));
		return 0;
	}
}

int broken_point_continuingly_transferring()
//int main(int c, char **argv)
{
	CURL *curlhandle = NULL;
	curl_global_init(CURL_GLOBAL_ALL);
	curlhandle = curl_easy_init();

	//download(curlhandle, "ftp://user:pass@host/path/file", "C:\\file", 0, 3);
	download(curlhandle, "ftp://117server:123456@192.168.0.117/77.xml", "D:\\77.xml", 0, 3);
	//download(curlhandle , "http://software.sky-union.cn/index.asp","/work/index.asp",1,3);
	curl_easy_cleanup(curlhandle);
	curl_global_cleanup();
	return 0;
}
#endif