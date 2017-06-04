#pragma once
#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
using std::wstring;
#include <locale>
#include <boost/regex.hpp>
using namespace boost;

void MatchWords(string sToMatch)
{
	regex rg("(\\w*)");
	smatch sm;
	bool b = regex_match( sToMatch, sm, rg );
	cout << "匹配结果：" << sm[1].str() << endl;
}
void MatchWords(wstring wsToMatch)
{
	wregex wrg(L"(\\w*)");
	wsmatch wsm;
	bool b = regex_match( wsToMatch, wsm, wrg );
	int iLen= wcstombs( NULL, wsm[1].str().c_str(), 0 );
	char *lpsz= new char[iLen+1];
	int i= wcstombs( lpsz, wsm[1].str().c_str(), iLen );
	lpsz[iLen] = '\0';
	string sToMatch(lpsz);
	delete []lpsz;
	cout << "匹配结果：" << sToMatch << endl;
}

void test_RegEx_2()
{
	string sToMatch("数超限");
	MatchWords( sToMatch );
	sToMatch = string("烦死了分角色而建立");
	MatchWords( sToMatch );
	setlocale( LC_CTYPE, "" );
	int iWLen= mbstowcs( NULL, sToMatch.c_str(),
	sToMatch.length() );
	wchar_t *lpwsz= new wchar_t[iWLen+1];
	int i= mbstowcs( lpwsz, sToMatch.c_str(), sToMatch.length() );
	wstring wsToMatch(lpwsz);
	delete []lpwsz;
	MatchWords( wsToMatch );
}