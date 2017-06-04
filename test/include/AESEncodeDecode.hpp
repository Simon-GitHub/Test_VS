#pragma once
#include <openssl/evp.h>
#include <stdio.h>

#define N 1024
#define IN const
//Aes�㷨ecbģʽ�����ļ�
/**********************************************************************
�������ƣ�AesEncryptFile
�������ܣ������ļ�
������̣�
	1.����ѡ��������㷨�Լ��������key��iv��
	2.���ļ�ͷд�������ļ�
	3.ѭ����ȡԭ���ļ����ݼ��ܺ󱣴浽�����ļ�·���С�
����˵����
	szSrc:[IN] char *�������ܵ�ԭ���ļ�·��
	szTarget:[IN] char *�����ܺ�������ļ�����·��
	key:[IN] unsigned char *, ����
	iType:[IN] int, �������ͣ�128��256��
	����ֵ���ɹ�����1�����򷵻�0	
************************************************************************/
int AesEncryptFile ( IN char * szSrc,
					 IN char * szTarget ,
					 IN unsigned char * key,
					 IN int iType)
{
	unsigned char ukey[EVP_MAX_KEY_LENGTH];
	unsigned char iv[EVP_MAX_IV_LENGTH];
	unsigned char in[N];
	int inl;   //�������ݴ�С
	unsigned char out[N];
	int outl;   //������ݴ�С

	int isSuccess;

	FILE *fpIn;
	FILE *fpOut;

	EVP_CIPHER_CTX ctx;   //evp���������Ļ���
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

	//ѡ���㷨
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
	//����ukey��iv
	int len = sizeof(key);
	EVP_BytesToKey(cipher,EVP_md5(),NULL,(const unsigned char *)key,len-1,1,ukey,iv);

	//��ʼ��ctx�������㷨��ʼ��
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

	//�����ļ�
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
	printf("���ܳɹ�\n");
	EVP_CIPHER_CTX_cleanup(&ctx);
	fclose(fpIn);
	fclose(fpOut);
	return 1;
}

//Aes�㷨ecbģʽ�����ļ�
/**********************************************************************
�������ƣ�AesDecryptFile
�������ܣ������ļ�
������̣�
	1.����ѡ��������㷨�Լ��������key��iv��
	2.���ļ�ͷд�������ļ�
	3.ѭ����ȡԭ���ļ����ݼ��ܺ󱣴浽�����ļ�·���С�
����˵����
	szSrc:[IN] char *�������ܵ������ļ�·��
	szTarget:[IN] char *�����ܺ�Ľ����ļ�����·��
	key:[IN] unsigned char *, ����
	iType:[IN] int, �������ͣ�128��256��
	����ֵ���ɹ�����1�����򷵻�0	
************************************************************************/
int AesDecryptFile ( IN char * szSrc,
					 IN char * szTarget ,
					 IN unsigned char * key,
					 IN int iType)
{
	unsigned char ukey[EVP_MAX_KEY_LENGTH];
	unsigned char iv[EVP_MAX_IV_LENGTH];
	unsigned char in[N];
	int inl;   //�������ݴ�С
	unsigned char out[N];
	int outl;   //������ݴ�С

	int isSuccess;

	FILE *fpIn;
	FILE *fpOut;

	EVP_CIPHER_CTX ctx;   //evp���������Ļ���
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

	//ѡ���㷨
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
	//����ukey��iv
	int len = sizeof(key);
	EVP_BytesToKey(cipher,EVP_md5(),NULL,(const unsigned char *)key,len-1,1,ukey,iv);

	//��ʼ��ctx�������㷨��ʼ��
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

	//�����ļ�
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
	printf("���ܳɹ�\n");
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