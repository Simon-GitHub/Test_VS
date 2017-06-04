
#include <curl/curl.h>

//重写把数据读入上传数据流函数
size_t read_file(void* buff, size_t size, size_t nmemb, void* userp)
{
	size_t sizes = fread(buff, size, nmemb, (FILE *)userp);
	return sizes;
}

int upload_file(const char* url, const char* file_name)
{
	if ("" == url || "" == file_name)
	{
		std::cout << "url地址或文件名为空,操作失败!" << std::endl;
		return -1;
	}
	CURLcode res;	//easy_handle定义的一些错误码

	//初始化libcurl
	res = curl_global_init(CURL_GLOBAL_ALL);
	if (res != CURLE_OK)
	{
		std::cerr << "init libcurl failed." << std::endl;
		return -1;
	}

	//获取要上传的文件指针	
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

	// 获取文件大小
	fseek(r_file, 0, 2);	
	int file_size = ftell(r_file);
	rewind(r_file);

	curl_easy_setopt(easy_handle, CURLOPT_URL, url);		//获取URL地址
	curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, TRUE);	//告诉easy_handle是做上传操作
	curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, &read_file);	//调用重写的读文件流函数
	curl_easy_setopt(easy_handle, CURLOPT_READDATA, r_file);	//往read_file()函数中传入用户自定义的数据类型
	curl_easy_setopt(easy_handle, CURLOPT_INFILE, r_file);		//定位作为上传的输入文件
	curl_easy_setopt(easy_handle, CURLOPT_INFILESIZE, file_size);	//上传的字节数

	//执行设置好的操作
	res = curl_easy_perform(easy_handle);

	//获取HTTP错误码
	int HTTP_flag = 0;
	curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE , &HTTP_flag);

	//一般清理动作
	fclose(r_file);
	curl_global_cleanup();
	curl_easy_cleanup(easy_handle);

	std::cout << "操作本地文件:" << file_name << "; url:" << url << ".";
	//检测HTTP错误码和执行操作结果
	if ((0 != check_error(HTTP_flag)) || (CURLE_OK != res))
	{
		std::cout << "失败!" << std::endl;
		return -1;
	}
	else
	{
		std::cout << "成功!" << std::endl;
		return 0;
	}
}
