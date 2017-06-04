#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <string.h>
#include <string>
#include <iostream>

using namespace std;

char * Base64Encode(const char* input, int length, bool with_new_line);
char * Base64Decode(char* input, int length, bool with_new_line);

/************************************************************************/

//demo.cpp
// g++ demo.cpp -o demo -lcrypto
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/applink.c>

#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
using namespace std;

//加密
std::string EncodeRSAKeyFile( const std::string& strPemFileName, const std::string& strData )
{
	if (strPemFileName.empty() || strData.empty())
	{
		assert(false);
		return "";
	}
	FILE* hPubKeyFile = fopen(strPemFileName.c_str(), "rb");
	if( hPubKeyFile == NULL )
	{
		assert(false);
		return ""; 
	}
	std::string strRet;
	RSA* pRSAPublicKey = RSA_new();
	if(PEM_read_RSA_PUBKEY(hPubKeyFile, &pRSAPublicKey, 0, 0) == NULL)
	{
		assert(false);
		return "";
	}

	int nLen = RSA_size(pRSAPublicKey);
	char* pEncode = new char[nLen + 1];
	int ret = RSA_public_encrypt(strData.length(), (const unsigned char*)strData.c_str(), (unsigned char*)pEncode, pRSAPublicKey, RSA_PKCS1_PADDING);
	if (ret >= 0)
	{
		strRet = std::string(pEncode, ret);
	}
	delete[] pEncode;
	RSA_free(pRSAPublicKey);
	fclose(hPubKeyFile);
	CRYPTO_cleanup_all_ex_data(); 
	return strRet;
}

//解密
std::string DecodeRSAKeyFile( const std::string& strPemFileName, const std::string& strData )
{
	if (strPemFileName.empty() || strData.empty())
	{
		assert(false);
		return "";
	}
	FILE* hPriKeyFile = fopen(strPemFileName.c_str(),"rb");
	if( hPriKeyFile == NULL )
	{
		assert(false);
		return "";
	}
	std::string strRet;
	RSA* pRSAPriKey = RSA_new();
	if(PEM_read_RSAPrivateKey(hPriKeyFile, &pRSAPriKey, 0, 0) == NULL)
	{
		assert(false);
		return "";
	}
	int nLen = RSA_size(pRSAPriKey);
	char* pDecode = new char[nLen+1];

	int ret = RSA_private_decrypt(strData.length(), (const unsigned char*)strData.c_str(), (unsigned char*)pDecode, pRSAPriKey, RSA_PKCS1_PADDING);
	if(ret >= 0)
	{
		strRet = std::string((char*)pDecode, ret);
	}
	delete [] pDecode;
	RSA_free(pRSAPriKey);
	fclose(hPriKeyFile);
	CRYPTO_cleanup_all_ex_data(); 
	return strRet;
}

string test_OpenSSL()
{
	//原文
	//const string one = "skl;dfhas;lkdfhslk;dfhsidfhoiehrfoishfsidf";
	const string one = "{\"account\":\"yucanlin\",\"password\":\"123456\"}";
	cout << "one: " << one << endl;

	//密文（二进制数据）
	string two = EncodeRSAKeyFile("pubkey.pem", one);
	cout << "two: " << two << endl;

	//顺利的话，解密后的文字和原文是一致的
	string three = DecodeRSAKeyFile("prikey.pem", two);
	cout << "three: " << three << endl;
	return two;
}/************************************************************************/
void test_OpenSSL_Base64()
{
	cout << "With new line? y/n ";
	string option;
	cin >> option;
	bool with_new_line = ( ("y" == option || "Y" == option) ? true : false );

	//string enc_input = "Henry Alfred Kissinger is a German-born American writer, political scientist, diplomat, and businessman. A recipient of the Nobel Peace Prize, he served as National Security Advisor and later concurrently as Secretary of State in the administrations of Presidents Richard Nixon and Gerald Ford.";
	string enc_input = test_OpenSSL();

	cout << endl << "To be encoded:" << endl << "~" << enc_input << "~" << endl << endl;

	char * enc_output = Base64Encode(enc_input.c_str(), enc_input.length(), with_new_line);
	cout << "Base64 Encoded:" << endl << "~" << enc_output << "~" << endl << endl;

	string dec_input = enc_output;
	char * dec_output = Base64Decode((char *)dec_input.c_str(), dec_input.length(), with_new_line);
	cout << "Base64 Decoded:" << endl << "~" << dec_output << "~" << endl << endl;

	free(enc_output);
	free(dec_output);
}

char * Base64Encode(const char * input, int length, bool with_new_line)
{
	BIO * bmem = NULL;
	BIO * b64 = NULL;
	BUF_MEM * bptr = NULL;

	b64 = BIO_new(BIO_f_base64());
	if(!with_new_line) {
		BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
	}
	bmem = BIO_new(BIO_s_mem());
	b64 = BIO_push(b64, bmem);
	BIO_write(b64, input, length);
	BIO_flush(b64);
	BIO_get_mem_ptr(b64, &bptr);

	char * buff = (char *)malloc(bptr->length + 1);
	memcpy(buff, bptr->data, bptr->length);
	buff[bptr->length] = 0;

	BIO_free_all(b64);

	return buff;
}

char * Base64Decode(char * input, int length, bool with_new_line)
{
	BIO * b64 = NULL;
	BIO * bmem = NULL;
	char * buffer = (char *)malloc(length);
	memset(buffer, 0, length);

	b64 = BIO_new(BIO_f_base64());
	if(!with_new_line) {
		BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
	}
	bmem = BIO_new_mem_buf(input, length);
	bmem = BIO_push(b64, bmem);
	BIO_read(bmem, buffer, length);

	BIO_free_all(bmem);

	return buffer;
}