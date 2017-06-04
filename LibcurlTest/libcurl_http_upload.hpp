
#include <curl/curl.h>

//��д�����ݶ����ϴ�����������
size_t read_file(void* buff, size_t size, size_t nmemb, void* userp)
{
	size_t sizes = fread(buff, size, nmemb, (FILE *)userp);
	return sizes;
}

int upload_file(const char* url, const char* file_name)
{
	if ("" == url || "" == file_name)
	{
		std::cout << "url��ַ���ļ���Ϊ��,����ʧ��!" << std::endl;
		return -1;
	}
	CURLcode res;	//easy_handle�����һЩ������

	//��ʼ��libcurl
	res = curl_global_init(CURL_GLOBAL_ALL);
	if (res != CURLE_OK)
	{
		std::cerr << "init libcurl failed." << std::endl;
		return -1;
	}

	//��ȡҪ�ϴ����ļ�ָ��	
	FILE* r_file = fopen(file_name, "rb");
	if (0 == r_file)
	{
		std::cerr << "the file /" " << file_name << " /"that you will read is not exist" << std::endl;
		return -1;
	}

	CURL* easy_handle;

	easy_handle = curl_easy_init();
	if (0 == easy_handle)
	{
		std::cout << "get a easy_handle handle fail!" << std::endl;
		fclose(r_file);
		curl_global_cleanup();
		curl_easy_cleanup(easy_handle);
		return -1;
	}

	// ��ȡ�ļ���С
	fseek(r_file, 0, 2);	
	int file_size = ftell(r_file);
	rewind(r_file);

	curl_easy_setopt(easy_handle, CURLOPT_URL, url);		//��ȡURL��ַ
	curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, TRUE);	//����easy_handle�����ϴ�����
	curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, &read_file);	//������д�Ķ��ļ�������
	curl_easy_setopt(easy_handle, CURLOPT_READDATA, r_file);	//��read_file()�����д����û��Զ������������
	curl_easy_setopt(easy_handle, CURLOPT_INFILE, r_file);		//��λ��Ϊ�ϴ��������ļ�
	curl_easy_setopt(easy_handle, CURLOPT_INFILESIZE, file_size);	//�ϴ����ֽ���

	//ִ�����úõĲ���
	res = curl_easy_perform(easy_handle);

	//��ȡHTTP������
	int HTTP_flag = 0;
	curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE , &HTTP_flag);

	//һ��������
	fclose(r_file);
	curl_global_cleanup();
	curl_easy_cleanup(easy_handle);

	std::cout << "���������ļ�:" << file_name << "; url:" << url << ".";
	//���HTTP�������ִ�в������
	if ((0 != check_error(HTTP_flag)) || (CURLE_OK != res))
	{
		std::cout << "ʧ��!" << std::endl;
		return -1;
	}
	else
	{
		std::cout << "�ɹ�!" << std::endl;
		return 0;
	}
}
