#pragma once
//��������C����ʵ�ֵļ��ܺͽ��ܣ���ȡһ���ı��ļ���
//�����ݼ��ܺ�д�뵽��һ���ļ��У�Ȼ���ٽ��ܣ�д�뵽�������ļ��С�

#include <stdio.h>
#include <string.h>


#include "openssl/evp.h"
#include "openssl/x509.h"

	//���ܺ���
int  Encrypt_File()
{
	unsigned char key[EVP_MAX_KEY_LENGTH]; //������Կ������
	unsigned char iv[EVP_MAX_KEY_LENGTH]; //�����ʼ������������
	EVP_CIPHER_CTX ctx;   //EVP���������Ļ���
	unsigned char out[1024];  //�������ĵĻ�����
	int outl;
	unsigned char in[1024];   //����ԭ�ĵĻ�����
	int inl;
	int rv;
	int i;
	FILE *fpIn;
	FILE *fpOut;
	//�򿪴������ļ�
	fpIn = fopen("App.rar","rb");
	if(fpIn==NULL)
	{
		return -1;
	}
	//�򿪱������ĵ��ļ�
	fpOut = fopen("3rar.enc","wb");
	if(fpOut==NULL)
	{
		fclose(fpIn);
		return -1;
	}
	//����key��iv
	for(i=0;i<24;i++)
	{
		key[i]=i;
	}
	for(i=0;i<8;i++)
	{
		iv[i]=i;
	}

	//��ʼ��ctx
	EVP_CIPHER_CTX_init(&ctx);
	//���������㷨��key��iv
	rv = EVP_EncryptInit_ex(&ctx,EVP_des_ede3_cbc(),NULL,key,iv);
	if(rv != 1)
	{
		printf("Err\n");
		return -1;
	}
	//ѭ����ȡԭ�ģ����ܺ�󱣴浽�����ļ���
	for(;;)
	{
		inl = fread(in,1,1024,fpIn);
		if(inl <= 0)//��ȡԭ�Ľ���
			break;
		rv = EVP_EncryptUpdate(&ctx,out,&outl,in,inl);//����
		if(rv != 1)
		{
			fclose(fpIn);
			fclose(fpOut);
			EVP_CIPHER_CTX_cleanup(&ctx);
			return -1;
		}
		fwrite(out,1,outl,fpOut);//�������ĵ��ļ�
	}
	//���ܽ���
	rv = EVP_EncryptFinal_ex(&ctx,out,&outl);
	if(rv != 1)
	{
		fclose(fpIn);
		fclose(fpOut);
		EVP_CIPHER_CTX_cleanup(&ctx);
		return -1;
	}
	fwrite(out,1,outl,fpOut);  //�������ĵ��ļ�
	fclose(fpIn);
	fclose(fpOut);
	EVP_CIPHER_CTX_cleanup(&ctx); //���EVP���������Ļ���
	printf("���������\n");
	return 1;
}


//���ܺ���
int Decrypt_File()
{
	unsigned char key[EVP_MAX_KEY_LENGTH];  //������Կ������
	unsigned char iv[EVP_MAX_KEY_LENGTH];  //�����ʼ������������
	EVP_CIPHER_CTX ctx;    //EVP���������Ļ���
	unsigned char out[1024+EVP_MAX_KEY_LENGTH]; //������ܺ����ĵĻ���������
	int outl;
	unsigned char in[1024];    //�����������ݵ�����
	int inl;
	int rv;
	int i;
	FILE *fpIn;
	FILE *fpOut;

	//�򿪴����ܵ������ļ�
	fpIn = fopen("3rar.enc","rb");
	if(fpIn==NULL)
	{
		return -1;
	}
	//�򿪱������ĵ��ļ�
	fpOut = fopen("3rar.dec","wb");
	if(fpOut==NULL)
	{
		fclose(fpIn);
		return -1;
	}
	//����key��iv
	for(i=0;i<24;i++)
	{
		key[i]=i;
	}
	for(i=0;i<8;i++)
	{
		iv[i]=i;
	}

	//��ʼ��ctx
	EVP_CIPHER_CTX_init(&ctx);
	//���ý��ܵ��㷨��key��iv
	rv = EVP_DecryptInit_ex(&ctx,EVP_des_ede3_cbc(),NULL,key,iv);
	if(rv != 1)
	{
		EVP_CIPHER_CTX_cleanup(&ctx);
		return -1;
	}
	//ѭ����ȡԭ�ģ����ܺ�󱣴浽�����ļ���
	for(;;)
	{
		inl = fread(in,1,1024,fpIn);
		if(inl <= 0)
			break;
		rv = EVP_DecryptUpdate(&ctx,out,&outl,in,inl);//����
		if(rv != 1)
		{
			fclose(fpIn);
			fclose(fpOut);
			EVP_CIPHER_CTX_cleanup(&ctx);
			return -1;
		}
		fwrite(out,1,outl,fpOut);//�������ĵ��ļ�
	}
	//���ܽ���
	rv = EVP_DecryptFinal_ex(&ctx,out,&outl);
	if(rv != 1)
	{
		fclose(fpIn);
		fclose(fpOut);
		EVP_CIPHER_CTX_cleanup(&ctx);
		return -1;
	}
	fwrite(out,1,outl,fpOut);//�������ĵ��ļ�
	fclose(fpIn);
	fclose(fpOut);
	EVP_CIPHER_CTX_cleanup(&ctx);//���EVP���������Ļ���
	printf("���������\n");
	return 1;
}

int AESEncodeDecode_2()
{
	int i;
	int j;
	OpenSSL_add_all_algorithms();

	printf("|-------����ѡ��ķ�������--------|\n");
	printf("|                                |\n");
	printf("|    1:����  2:����  3:�˳�    |\n");
	printf("|                                |\n");
	printf("|---------------------------------|\n");
	printf("������ѡ��:");
	scanf("%d",&i);


	switch(i)
	{
	case 1: Encrypt_File(); break;
	case 2: Decrypt_File(); break;
	case 3: exit(0); break;
	default:
		printf("        �������,����������");
	}

	return 0;
}