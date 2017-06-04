
#include <curl/curl.h>

//WRITEFUNCTION回调函数的实现,在这里只做简单的处理
size_t process_data(void *buffer, size_t size, size_t nmemb, std::string& user_p)
{	
	user_p = (char*)buffer;

	return nmemb;
}

const int FILE_EXIST = 200;

int down_file(const char* url, const char* down_file_name)
{
	// 初始化libcurl
	CURLcode return_code;
	return_code = curl_global_init(CURL_GLOBAL_ALL);
	if (CURLE_OK != return_code)
	{
		cerr << "init libcurl failed." << endl;
		curl_global_cleanup();
		return -1;
	}

	// 获取easy handle
	CURL *easy_handle = curl_easy_init();
	if (NULL == easy_handle)
	{		
		cerr << "get a easy handle failed." << endl;
		curl_easy_cleanup(easy_handle);
		curl_global_cleanup();
		return -1;
	}

	// 设置easy handle属性
	return_code = curl_easy_setopt(easy_handle, CURLOPT_URL, url);

	//设置回调函数
	return_code = curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, &process_data);

	//回调函数的额外参数
	std::string connectx;
	return_code = curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &connectx);

	// 执行数据请求
	return_code = curl_easy_perform(easy_handle);	

	//判断获取响应的http地址是否存在,若存在则返回200,400以上则为不存在,一般不存在为404错误
	int retcode = 0;
	return_code = curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE , &retcode);  
	if (CURLE_OK == return_code && FILE_EXIST == retcode)
	{
		double length = 0;
		return_code = curl_easy_getinfo(easy_handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD , &length); 
		FILE *fp = fopen(save_name.c_str(), "wb+");
		fwrite(connectx.c_str(), 1, length, fp);	//返回实际写入文本的长度,若不等于length则写文件发生错误.
		fclose(fp);
	}
	else
	{
		std::cout << "请求的文件不存在!" << std::endl;
		curl_easy_cleanup(easy_handle);
		curl_global_cleanup();
		return -1;
	}

	// 释放资源	
	curl_easy_cleanup(easy_handle);
	curl_global_cleanup();

	return 0;
}