#pragma once
#include <openssl/evp.h>
#include <stdio.h>

#define N 1024
#define IN const
//Aes算法ecb模式加密文件
/**********************************************************************
函数名称：AesEncryptFile
函数功能：加密文件
处理过程：
	1.根据选择的密码算法以及口令，生成key和iv。
	2.把文件头写入密文文件
	3.循环读取原文文件数据加密后保存到密文文件路径中。
参数说明：
	szSrc:[IN] char *，待加密的原文文件路径
	szTarget:[IN] char *，加密后的密文文件保存路径
	key:[IN] unsigned char *, 口令
	iType:[IN] int, 加密类型（128或256）
	返回值：成功返回1，否则返回0	
************************************************************************/
int AesEncryptFile ( IN char * szSrc,
					 IN char * szTarget ,
					 IN unsigned char * key,
					 IN int iType)
{
	unsigned char ukey[EVP_MAX_KEY_LENGTH];
	unsigned char iv[EVP_MAX_IV_LENGTH];
	unsigned char in[N];
	int inl;   //输入数据大小
	unsigned char out[N];
	int outl;   //输出数据大小

	int isSuccess;

	FILE *fpIn;
	FILE *fpOut;

	EVP_CIPHER_CTX ctx;   //evp加密上下文环境
	const	EVP_CIPHER *cipher;

	fpIn = fopen(szSrc,"rb");
	if(fpIn==NULL)
	{
		printf("fopen szSrc failed");
		return 0;
	}
	fpOut = fopen(szTarget,"wb");
	if(fpOut==NULL)
	{
		printf("fopen szTarget failed");
		fclose(fpIn);
		return 0;
	}

	//选择算法
	if(iType == 128)
	{
		cipher = EVP_aes_128_ecb();
	}
	else if(iType == 256)
	{
		cipher = EVP_aes_256_ecb();
	}
	else
	{
		printf("iType should be 128 or 256.");
		fclose(fpIn);
		fclose(fpOut);
		return 0;
	}
	//生成ukey和iv
	int len = sizeof(key);
	EVP_BytesToKey(cipher,EVP_md5(),NULL,(const unsigned char *)key,len-1,1,ukey,iv);

	//初始化ctx，加密算法初始化
	EVP_CIPHER_CTX_init(&ctx);
	isSuccess = EVP_EncryptInit_ex(&ctx,cipher,NULL,ukey,iv);
	if(!isSuccess)
	{
		printf("EVP_EncryptInit_ex() failed");
		EVP_CIPHER_CTX_cleanup(&ctx);
		fclose(fpIn);
		fclose(fpOut);
		return 0;
	}

	//加密文件
	for(;;)
	{
		inl = fread(in,1,N,fpIn);
		if(inl<=0)
			break;

		isSuccess = EVP_EncryptUpdate(&ctx,out,&outl,in,inl);
		if(!isSuccess)
		{
			printf("EVP_EncryptInit_ex() failed");
			EVP_CIPHER_CTX_cleanup(&ctx);
			fclose(fpIn);
			fclose(fpOut);
			return 0;
		}
		fwrite(out,1,outl,fpOut);
	}
	isSuccess = EVP_EncryptFinal_ex(&ctx,out,&outl);
	if(!isSuccess)
	{
		printf("EVP_EncryptInit_ex() failed");
		EVP_CIPHER_CTX_cleanup(&ctx);
		fclose(fpIn);
		fclose(fpOut);
		return 0;
	}
		
	fwrite(out,1,outl,fpOut);
	printf("加密成功\n");
	EVP_CIPHER_CTX_cleanup(&ctx);
	fclose(fpIn);
	fclose(fpOut);
	return 1;
}

//Aes算法ecb模式解密文件
/**********************************************************************
函数名称：AesDecryptFile
函数功能：解密文件
处理过程：
	1.根据选择的密码算法以及口令，生成key和iv。
	2.把文件头写入密文文件
	3.循环读取原文文件数据加密后保存到密文文件路径中。
参数说明：
	szSrc:[IN] char *，待解密的密文文件路径
	szTarget:[IN] char *，解密后的解密文件保存路径
	key:[IN] unsigned char *, 口令
	iType:[IN] int, 加密类型（128或256）
	返回值：成功返回1，否则返回0	
************************************************************************/
int AesDecryptFile ( IN char * szSrc,
					 IN char * szTarget ,
					 IN unsigned char * key,
					 IN int iType)
{
	unsigned char ukey[EVP_MAX_KEY_LENGTH];
	unsigned char iv[EVP_MAX_IV_LENGTH];
	unsigned char in[N];
	int inl;   //输入数据大小
	unsigned char out[N];
	int outl;   //输出数据大小

	int isSuccess;

	FILE *fpIn;
	FILE *fpOut;

	EVP_CIPHER_CTX ctx;   //evp加密上下文环境
	const EVP_CIPHER *cipher;

	fpIn = fopen(szSrc,"rb");
	if(fpIn==NULL)
	{
		printf("fopen szSrc failed");
		return 0;
	}
	fpOut = fopen(szTarget,"wb");
	if(fpOut==NULL)
	{
		printf("fopen szTarget failed");
		fclose(fpIn);
		return 0;
	}

	//选择算法
	if(iType == 128)
	{
		cipher = EVP_aes_128_ecb();
	}
	else if(iType == 256)
	{
		cipher = EVP_aes_256_ecb();
	}
	else
	{
		printf("iType should be 128 or 256.");
		fclose(fpIn);
		fclose(fpOut);
		return 0;
	}
	//生成ukey和iv
	int len = sizeof(key);
	EVP_BytesToKey(cipher,EVP_md5(),NULL,(const unsigned char *)key,len-1,1,ukey,iv);

	//初始化ctx，加密算法初始化
	EVP_CIPHER_CTX_init(&ctx);
	isSuccess = EVP_DecryptInit_ex(&ctx,cipher,NULL,ukey,iv);
	if(!isSuccess)
	{
		printf("EVP_DecryptInit_ex() failed");
		EVP_CIPHER_CTX_cleanup(&ctx);
		fclose(fpIn);
		fclose(fpOut);
		return 0;
	}

	//加密文件
	for(;;)
	{
		inl = fread(in,1,N,fpIn);
		if(inl<=0)
			break;

		isSuccess = EVP_DecryptUpdate(&ctx,out,&outl,in,inl);
		if(!isSuccess)
		{
			printf("EVP_EncryptInit_ex() failed");
			EVP_CIPHER_CTX_cleanup(&ctx);
			fclose(fpIn);
			fclose(fpOut);
			return 0;
		}
		fwrite(out,1,outl,fpOut);
	}
	isSuccess = EVP_DecryptFinal_ex(&ctx,out,&outl);
	if(!isSuccess)
	{
		printf("EVP_DecryptInit_ex() failed");
		EVP_CIPHER_CTX_cleanup(&ctx);
		fclose(fpIn);
		fclose(fpOut);
		return 0;
	}
		
	fwrite(out,1,outl,fpOut);
	printf("解密成功\n");
	EVP_CIPHER_CTX_cleanup(&ctx);
	fclose(fpIn);
	fclose(fpOut);
	return 1;
}

int AESEncodeDecode()
{
	char *Src = "SaveSendData.rar";
	char *TargetEnc = "SaveSendData_enc.enc";
	char *TargetDec = "SaveSendData_dec.dec";
	//unsigned char key[32] = "abcdefg";
	unsigned char key[32] = "123456789";
	int rv = AesEncryptFile(Src,TargetEnc,key,128);
	if(rv!=1)
	{
		printf("AesEncryptFile() failed");
		return 1;
	}
	rv = AesDecryptFile(TargetEnc,TargetDec,key,128);
	if(rv!=1)
	{
		printf("AesDecryptFile() failed");
		return 1;
	}
	return 0;
}