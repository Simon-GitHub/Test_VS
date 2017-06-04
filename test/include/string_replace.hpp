#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int string_replace()
{
	char buffer[200];//��ʱ����
	char filename[50]; 
	string filein;//����Ҫ�滻��Դ�ļ���

	cout<<"Please input the source of file's name:"<<endl;
	cin>>filein;
	strcpy(filename,filein.c_str());//Դ�ļ����ַ���ת��Ϊ�ַ�����

	fstream inout(filename);//�Զ�д��ʽ��ĳһ�ļ�
	if(!inout.is_open())//�ж��Ƿ��
	{
		cout<<"Error opening file";
		exit(1);
	}

	//����ע���ǲ��Դ���
	/*while(!inout.eof())
	{
		inout.getline(buffer,100);//������ļ���ÿһ��
		cout<<buffer<<endl;
	}*/

	string sourcefile;
	int seeklength;
	string seekword,replaceword;//����Ҫ���ҵĵ��ʺ��滻Ϊ�ĵ����ַ�������
	char seekchar[20],replacechar[20];

	cout<<"Please input the name of seek and replace:"<<endl;
	cin>>seekword>>replaceword;

	strcpy(seekchar,seekword.c_str());//�����ҵ����ַ���ת��Ϊ�ַ�������
	strcpy(replacechar,replaceword.c_str());//���滻�����ַ���ת��Ϊ�ַ�������
	seeklength = seekword.length();//��¼Ҫ���ҵ��ʵĳ���
	while(!inout.eof())
	{
		inout.getline(buffer,200);//������ļ���ÿһ��
		sourcefile.append(buffer);//����ȡ�ļ����ļ����ݴ浽�ַ�����
		sourcefile.append("\n");//׷�ӻ��з�
	}
	int index=0;
	bool mark=true;
	while(mark)
	{
		index = sourcefile.find(seekword,index);//��λҪ�滻�ĵ��ʣ���¼�俪ʼλ��
		if(index == string::npos)//�滻��ϻ�û���ֵ��ʵĴ������
		{
			cout << "Didn't find the word or replace the word is OK!" << endl;
			mark=false;
		}
		else
			sourcefile.replace(index,seeklength,replaceword);//���滻�ַ����滻ָ��λ�õĵ���
	}
	inout.clear();//���Ѵ��ڵ������󣬱�����ÿ��ƫ��ѭ��ʱ���
	inout.close();
	inout.open(filename,ios::in|ios::out|ios::trunc);
	//inout.seekp(0,ios::beg);//��λ���ļ���ʼλ��
	inout<<sourcefile;//����д���滻����ļ�����
	inout.close();

	return 0;

}