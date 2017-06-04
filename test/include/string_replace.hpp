#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int string_replace()
{
	char buffer[200];//临时缓存
	char filename[50]; 
	string filein;//保存要替换的源文件名

	cout<<"Please input the source of file's name:"<<endl;
	cin>>filein;
	strcpy(filename,filein.c_str());//源文件名字符串转换为字符数组

	fstream inout(filename);//以读写方式打开某一文件
	if(!inout.is_open())//判断是否打开
	{
		cout<<"Error opening file";
		exit(1);
	}

	//以下注释是测试代码
	/*while(!inout.eof())
	{
		inout.getline(buffer,100);//读入打开文件的每一行
		cout<<buffer<<endl;
	}*/

	string sourcefile;
	int seeklength;
	string seekword,replaceword;//定义要查找的单词和替换为的单词字符串变量
	char seekchar[20],replacechar[20];

	cout<<"Please input the name of seek and replace:"<<endl;
	cin>>seekword>>replaceword;

	strcpy(seekchar,seekword.c_str());//将查找单词字符串转换为字符数数组
	strcpy(replacechar,replaceword.c_str());//将替换单词字符串转换为字符数数组
	seeklength = seekword.length();//记录要查找单词的长度
	while(!inout.eof())
	{
		inout.getline(buffer,200);//读入打开文件的每一行
		sourcefile.append(buffer);//将读取文件的文件内容存到字符串中
		sourcefile.append("\n");//追加换行符
	}
	int index=0;
	bool mark=true;
	while(mark)
	{
		index = sourcefile.find(seekword,index);//定位要替换的单词，记录其开始位置
		if(index == string::npos)//替换完毕或没发现单词的处理语句
		{
			cout << "Didn't find the word or replace the word is OK!" << endl;
			mark=false;
		}
		else
			sourcefile.replace(index,seeklength,replaceword);//用替换字符串替换指定位置的单词
	}
	inout.clear();//打开已存在的流对象，必须在每次偏移循环时清空
	inout.close();
	inout.open(filename,ios::in|ios::out|ios::trunc);
	//inout.seekp(0,ios::beg);//定位到文件开始位置
	inout<<sourcefile;//重新写入替换后的文件内容
	inout.close();

	return 0;

}