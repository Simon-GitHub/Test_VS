
#include <curl/curl.h>

//WRITEFUNCTION�ص�������ʵ��,������ֻ���򵥵Ĵ���
size_t process_data(void *buffer, size_t size, size_t nmemb, std::string& user_p)
{	
	user_p = (char*)buffer;

	return nmemb;
}

const int FILE_EXIST = 200;

int down_file(const char* url, const char* down_file_name)
{
	// ��ʼ��libcurl
	CURLcode return_code;
	return_code = curl_global_init(CURL_GLOBAL_ALL);
	if (CURLE_OK != return_code)
	{
		cerr << "init libcurl failed." << endl;
		curl_global_cleanup();
		return -1;
	}

	// ��ȡeasy handle
	CURL *easy_handle = curl_easy_init();
	if (NULL == easy_handle)
	{		
		cerr << "get a easy handle failed." << endl;
		curl_easy_cleanup(easy_handle);
		curl_global_cleanup();
		return -1;
	}

	// ����easy handle����
	return_code = curl_easy_setopt(easy_handle, CURLOPT_URL, url);

	//���ûص�����
	return_code = curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, &process_data);

	//�ص������Ķ������
	std::string connectx;
	return_code = curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &connectx);

	// ִ����������
	return_code = curl_easy_perform(easy_handle);	

	//�жϻ�ȡ��Ӧ��http��ַ�Ƿ����,�������򷵻�200,400������Ϊ������,һ�㲻����Ϊ404����
	int retcode = 0;
	return_code = curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE , &retcode);  
	if (CURLE_OK == return_code && FILE_EXIST == retcode)
	{
		double length = 0;
		return_code = curl_easy_getinfo(easy_handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD , &length); 
		FILE *fp = fopen(save_name.c_str(), "wb+");
		fwrite(connectx.c_str(), 1, length, fp);	//����ʵ��д���ı��ĳ���,��������length��д�ļ���������.
		fclose(fp);
	}
	else
	{
		std::cout << "������ļ�������!" << std::endl;
		curl_easy_cleanup(easy_handle);
		curl_global_cleanup();
		return -1;
	}

	// �ͷ���Դ	
	curl_easy_cleanup(easy_handle);
	curl_global_cleanup();

	return 0;
}